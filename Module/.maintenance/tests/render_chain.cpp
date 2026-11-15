#include "Renderer/render_chain.h"
#include <array>
#include <cstdio>
#include <cstring>

using namespace module::core::graphics::detail;

static int executable_method() { return 0; }

template<size_t Size>
static void put_pointer(std::array<unsigned char, Size>& storage, size_t offset, uintptr_t value)
{
    std::memcpy(storage.data() + offset, &value, sizeof(value));
}

int main()
{
    std::array<unsigned char, 0x240> job{};
    std::array<unsigned char, 0x80> view{};
    std::array<unsigned char, 0xC0> device{};
    std::array<unsigned char, 0x20> old_view{};
    std::array<void*, 18> vtable{};
    vtable.fill(reinterpret_cast<void*>(&executable_method));
    void** swapchain = vtable.data();
    const auto job_address = reinterpret_cast<uintptr_t>(job.data());

    // Literal positions in the supplied client layout independently pin the resolver.
    put_pointer(job, 0x1E0, reinterpret_cast<uintptr_t>(view.data()));
    put_pointer(view, 0x10, reinterpret_cast<uintptr_t>(device.data()));
    put_pointer(device, 0x88, reinterpret_cast<uintptr_t>(&swapchain));
    // The old route contains the exact poison pointer recorded in Crashlog_22356.
    put_pointer(job, 0x1D8, reinterpret_cast<uintptr_t>(old_view.data()));
    put_pointer(old_view, 0x8, uintptr_t{0x8E00000023});

    unsigned passed = 0;
    SwapChainCandidate candidate{};
    auto expect = [&](bool condition, const char* name)
    {
        if (!condition) { std::fprintf(stderr, "FAIL: %s\n", name); return false; }
        ++passed;
        return true;
    };
    if (!expect(resolve_swapchain(job_address, candidate) &&
        candidate.swapchain == reinterpret_cast<IDXGISwapChain*>(&swapchain), "current offsets ignore the poisoned old chain")) return 1;
    if (!expect(!resolve_swapchain(0, candidate) && !candidate.swapchain && !candidate.vtable, "null job clears previous output")) return 1;
    if (!expect(!resolve_swapchain(uintptr_t{0x8E00000023}, candidate), "crash-log pointer is rejected")) return 1;

    put_pointer(view, 0x10, uintptr_t{0x8E00000023});
    if (!expect(!resolve_swapchain(job_address, candidate) && !candidate.swapchain, "invalid intermediate pointer is rejected")) return 1;
    put_pointer(view, 0x10, reinterpret_cast<uintptr_t>(device.data()));
    put_pointer(device, 0x88, 0);
    if (!expect(!resolve_swapchain(job_address, candidate), "not-ready swapchain is rejected")) return 1;
    put_pointer(device, 0x88, reinterpret_cast<uintptr_t>(&swapchain));
    vtable[12] = device.data();
    if (!expect(!resolve_swapchain(job_address, candidate), "non-executable COM method is rejected")) return 1;
    vtable[12] = reinterpret_cast<void*>(&executable_method);

    SYSTEM_INFO system{};
    GetSystemInfo(&system);
    void* guarded = VirtualAlloc(nullptr, system.dwPageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!guarded) return 1;
    *static_cast<uintptr_t*>(guarded) = reinterpret_cast<uintptr_t>(view.data());
    DWORD old_protection = 0;
    if (!VirtualProtect(guarded, system.dwPageSize, PAGE_READWRITE | PAGE_GUARD, &old_protection)) return 1;
    uintptr_t value = 1;
    const bool guard_rejected = !read_pointer(reinterpret_cast<uintptr_t>(guarded), value) && value == 0;
    VirtualFree(guarded, 0, MEM_RELEASE);
    if (!expect(guard_rejected, "guarded memory is rejected without consuming the guard")) return 1;
    if (!expect(resolve_swapchain(job_address, candidate), "resolver recovers after a not-ready chain")) return 1;
    std::printf("Passed %u renderer pointer-chain regression checks.\n", passed);
}
