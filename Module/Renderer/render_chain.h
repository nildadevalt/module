#pragma once

#include <RBX/client_offsets.hpp>
#include <dxgi.h>
#include <limits>
#include <initializer_list>

namespace module::core::graphics::detail
{
    inline bool readable(uintptr_t address, size_t size)
    {
        if (!address || !size || size > (std::numeric_limits<uintptr_t>::max)() - address)
            return false;

        MEMORY_BASIC_INFORMATION region{};
        if (!VirtualQuery(reinterpret_cast<const void*>(address), &region, sizeof(region)) ||
            region.State != MEM_COMMIT || (region.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
            return false;

        const DWORD protection = region.Protect & 0xff;
        if (protection != PAGE_READONLY && protection != PAGE_READWRITE &&
            protection != PAGE_WRITECOPY && protection != PAGE_EXECUTE_READ &&
            protection != PAGE_EXECUTE_READWRITE && protection != PAGE_EXECUTE_WRITECOPY)
            return false;

        const auto begin = reinterpret_cast<uintptr_t>(region.BaseAddress);
        return address >= begin && address - begin < region.RegionSize &&
            size <= region.RegionSize - (address - begin);
    }

    inline bool read_pointer(uintptr_t address, uintptr_t& value)
    {
        value = 0;
        if (!readable(address, sizeof(value)))
            return false;
        __try
        {
            value = *reinterpret_cast<const uintptr_t*>(address);
            return value != 0;
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            value = 0;
            return false;
        }
    }

    inline bool executable(uintptr_t address)
    {
        MEMORY_BASIC_INFORMATION region{};
        if (!address || !VirtualQuery(reinterpret_cast<const void*>(address), &region, sizeof(region)) ||
            region.State != MEM_COMMIT || (region.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
            return false;
        const DWORD protection = region.Protect & 0xff;
        return protection == PAGE_EXECUTE || protection == PAGE_EXECUTE_READ ||
            protection == PAGE_EXECUTE_READWRITE || protection == PAGE_EXECUTE_WRITECOPY;
    }

    struct SwapChainCandidate
    {
        IDXGISwapChain* swapchain = nullptr;
        void** vtable = nullptr;
    };

    inline bool resolve_swapchain(uintptr_t render_job, SwapChainCandidate& result)
    {
        result = {};
        uintptr_t view = 0, device = 0, swapchain = 0, vtable = 0;
        if (!render_job ||
            !read_pointer(render_job + ClientOffsets::RenderView::RenderJobToView, view) ||
            !read_pointer(view + ClientOffsets::RenderView::ViewToDevice, device) ||
            !read_pointer(device + ClientOffsets::RenderView::DeviceToSwapChain, swapchain) ||
            !read_pointer(swapchain, vtable) || !readable(vtable, 18 * sizeof(void*)))
            return false;

        // Probe the methods used during setup and by the two render hooks.
        for (const size_t slot : {size_t{7}, size_t{8}, size_t{12}, size_t{13}})
        {
            uintptr_t method = 0;
            if (!read_pointer(vtable + slot * sizeof(void*), method) || !executable(method))
                return false;
        }
        result.swapchain = reinterpret_cast<IDXGISwapChain*>(swapchain);
        result.vtable = reinterpret_cast<void**>(vtable);
        return true;
    }
}
