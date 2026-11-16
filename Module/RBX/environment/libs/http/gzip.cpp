#include "gzip.hpp"

#include <cstdint>
#include <cstring>
#include <vector>

namespace
{
    // Bit reader over a byte buffer, least-significant-bit first as DEFLATE requires.
    struct BitReader
    {
        const std::uint8_t* data;
        std::size_t size;
        std::size_t pos = 0;   // byte offset
        std::uint32_t bitBuf = 0;
        int bitCount = 0;
        bool failed = false;

        BitReader(const std::uint8_t* d, std::size_t s) : data(d), size(s) {}

        int Bits(int need)
        {
            while (bitCount < need)
            {
                if (pos >= size)
                {
                    failed = true;
                    return 0;
                }
                bitBuf |= static_cast<std::uint32_t>(data[pos++]) << bitCount;
                bitCount += 8;
            }

            const int value = static_cast<int>(bitBuf & ((1u << need) - 1u));
            bitBuf >>= need;
            bitCount -= need;
            return value;
        }

        void AlignByte()
        {
            const int drop = bitCount % 8;
            bitBuf >>= drop;
            bitCount -= drop;
        }
    };

    // Canonical Huffman table: (code length -> symbol) decoded one bit at a time.
    // Simplicity over speed; HTTP bodies here are small.
    struct Huffman
    {
        // counts[len] = number of codes of that length, symbols in canonical order
        std::uint16_t counts[16] = {};
        std::vector<std::uint16_t> symbols;

        bool Build(const std::uint8_t* lengths, int count)
        {
            for (int i = 0; i < 16; ++i)
                counts[i] = 0;
            for (int i = 0; i < count; ++i)
                counts[lengths[i]]++;

            counts[0] = 0;

            // Guard against over-subscribed tables.
            int left = 1;
            for (int len = 1; len < 16; ++len)
            {
                left <<= 1;
                left -= counts[len];
                if (left < 0)
                    return false;
            }

            std::uint16_t offsets[16] = {};
            for (int len = 1; len < 15; ++len)
                offsets[len + 1] = static_cast<std::uint16_t>(offsets[len] + counts[len]);

            symbols.assign(count, 0);
            for (int i = 0; i < count; ++i)
            {
                if (lengths[i])
                    symbols[offsets[lengths[i]]++] = static_cast<std::uint16_t>(i);
            }
            return true;
        }

        int Decode(BitReader& br) const
        {
            int code = 0;
            int first = 0;
            int index = 0;

            for (int len = 1; len < 16; ++len)
            {
                code |= br.Bits(1);
                if (br.failed)
                    return -1;

                const int count = counts[len];
                if (code - first < count)
                    return symbols[index + (code - first)];

                index += count;
                first = (first + count) << 1;
                code <<= 1;
            }
            return -1;
        }
    };

    const std::uint16_t kLengthBase[29] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
                                            35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
    const std::uint8_t kLengthExtra[29] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
                                            3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
    const std::uint16_t kDistBase[30] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
                                          257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
                                          8193, 12289, 16385, 24577 };
    const std::uint8_t kDistExtra[30] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
                                          7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };

    bool InflateBlock(BitReader& br, std::string& out)
    {
        for (;;)
        {
            const int last = br.Bits(1);
            const int type = br.Bits(2);
            if (br.failed)
                return false;

            if (type == 0)
            {
                // stored
                br.AlignByte();
                if (br.bitCount >= 8)
                    return false;
                br.bitBuf = 0;
                br.bitCount = 0;

                if (br.pos + 4 > br.size)
                    return false;
                const int len = br.data[br.pos] | (br.data[br.pos + 1] << 8);
                const int nlen = br.data[br.pos + 2] | (br.data[br.pos + 3] << 8);
                br.pos += 4;
                if ((len ^ 0xFFFF) != nlen)
                    return false;
                if (br.pos + static_cast<std::size_t>(len) > br.size)
                    return false;
                out.append(reinterpret_cast<const char*>(br.data + br.pos), static_cast<std::size_t>(len));
                br.pos += static_cast<std::size_t>(len);
            }
            else if (type == 1 || type == 2)
            {
                Huffman litLen, dist;
                if (type == 1)
                {
                    std::uint8_t ll[288];
                    int i = 0;
                    for (; i < 144; ++i) ll[i] = 8;
                    for (; i < 256; ++i) ll[i] = 9;
                    for (; i < 280; ++i) ll[i] = 7;
                    for (; i < 288; ++i) ll[i] = 8;
                    std::uint8_t dd[30];
                    for (i = 0; i < 30; ++i) dd[i] = 5;
                    if (!litLen.Build(ll, 288) || !dist.Build(dd, 30))
                        return false;
                }
                else
                {
                    const int hlit = br.Bits(5) + 257;
                    const int hdist = br.Bits(5) + 1;
                    const int hclen = br.Bits(4) + 4;
                    if (br.failed)
                        return false;

                    static const int order[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
                    std::uint8_t clen[19] = {};
                    for (int i = 0; i < hclen; ++i)
                        clen[order[i]] = static_cast<std::uint8_t>(br.Bits(3));
                    if (br.failed)
                        return false;

                    Huffman codeLen;
                    if (!codeLen.Build(clen, 19))
                        return false;

                    std::uint8_t lengths[320] = {};
                    int index = 0;
                    while (index < hlit + hdist)
                    {
                        const int sym = codeLen.Decode(br);
                        if (sym < 0)
                            return false;

                        if (sym < 16)
                        {
                            lengths[index++] = static_cast<std::uint8_t>(sym);
                        }
                        else if (sym == 16)
                        {
                            if (index == 0)
                                return false;
                            const std::uint8_t prev = lengths[index - 1];
                            const int repeat = 3 + br.Bits(2);
                            for (int i = 0; i < repeat && index < hlit + hdist; ++i)
                                lengths[index++] = prev;
                        }
                        else if (sym == 17)
                        {
                            const int repeat = 3 + br.Bits(3);
                            index += repeat;
                        }
                        else
                        {
                            const int repeat = 11 + br.Bits(7);
                            index += repeat;
                        }
                        if (br.failed)
                            return false;
                    }

                    if (index > hlit + hdist)
                        return false;

                    if (!litLen.Build(lengths, hlit) || !dist.Build(lengths + hlit, hdist))
                        return false;
                }

                for (;;)
                {
                    const int sym = litLen.Decode(br);
                    if (sym < 0)
                        return false;

                    if (sym < 256)
                    {
                        out.push_back(static_cast<char>(sym));
                    }
                    else if (sym == 256)
                    {
                        break;
                    }
                    else
                    {
                        const int li = sym - 257;
                        if (li >= 29)
                            return false;

                        const int length = kLengthBase[li] + br.Bits(kLengthExtra[li]);
                        const int ds = dist.Decode(br);
                        if (ds < 0 || ds >= 30)
                            return false;

                        const int distance = kDistBase[ds] + br.Bits(kDistExtra[ds]);
                        if (br.failed)
                            return false;
                        if (distance <= 0 || static_cast<std::size_t>(distance) > out.size())
                            return false;

                        // Copy may overlap the output being produced.
                        std::size_t src = out.size() - static_cast<std::size_t>(distance);
                        for (int i = 0; i < length; ++i)
                            out.push_back(out[src + static_cast<std::size_t>(i)]);
                    }
                }
            }
            else
            {
                return false; // reserved block type
            }

            if (last)
                return true;
        }
    }
}

bool Gzip::LooksGzipped(const std::string& data)
{
    return data.size() > 2 &&
        static_cast<unsigned char>(data[0]) == 0x1f &&
        static_cast<unsigned char>(data[1]) == 0x8b;
}

bool Gzip::Decompress(const std::string& data, std::string& out)
{
    if (!LooksGzipped(data))
        return false;

    const auto* bytes = reinterpret_cast<const std::uint8_t*>(data.data());
    const std::size_t size = data.size();

    if (size < 18)
        return false;

    const std::uint8_t method = bytes[2];
    if (method != 8) // only DEFLATE
        return false;

    const std::uint8_t flags = bytes[3];

    std::size_t offset = 10; // fixed header

    if (flags & 0x04) // FEXTRA
    {
        if (offset + 2 > size)
            return false;
        const int xlen = bytes[offset] | (bytes[offset + 1] << 8);
        offset += 2 + static_cast<std::size_t>(xlen);
    }
    if (flags & 0x08) // FNAME
    {
        while (offset < size && bytes[offset] != 0) offset++;
        offset++;
    }
    if (flags & 0x10) // FCOMMENT
    {
        while (offset < size && bytes[offset] != 0) offset++;
        offset++;
    }
    if (flags & 0x02) // FHCRC
        offset += 2;

    if (offset >= size)
        return false;

    // Trailer is 8 bytes (CRC32 + ISIZE); exclude it from the inflate window.
    if (size < offset + 8)
        return false;

    const std::size_t deflateEnd = size - 8;

    BitReader br(bytes + offset, deflateEnd - offset);
    std::string result;
    result.reserve(deflateEnd * 3);

    if (!InflateBlock(br, result))
        return false;

    out = std::move(result);
    return true;
}