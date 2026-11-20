#pragma once

#include <cstdint>

#include <RBX/update.hpp>

// The client's Luau sources address a few globals through Offsets::. Most already
// exist in RBX/update.hpp; luaF_freeproto does not, so it is supplied here.
namespace Offsets
{
    inline const std::uintptr_t luaF_freeproto = ClientOffsets::luaF_freeproto;
}
