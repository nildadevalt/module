#pragma once

#include <string>

// Minimal self-contained gzip/DEFLATE decoder.
//
// The build has no zlib and the Roblox binary does not export one, but
// debUNC checks that gzip responses are transparently decoded. This
// implements the RFC 1951 inflate algorithm (plus the RFC 1952 gzip
// wrapper) so HTTP bodies can be decompressed without a new dependency.

namespace Gzip
{
    // Returns true and fills `out` when `data` holds a valid gzip stream.
    // Returns false (leaving `out` untouched) when the stream is invalid.
    bool Decompress(const std::string& data, std::string& out);

    // True when the buffer starts with the gzip magic bytes 1f 8b.
    bool LooksGzipped(const std::string& data);
}
