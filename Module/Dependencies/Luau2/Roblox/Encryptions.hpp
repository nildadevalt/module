#pragma once

// The client's 2c6fd5e0 Luau sources expect their cipher wrappers here. Our tree
// already defines them (VMValue1/VMValue2/VMValue3 plus the *_ENC macros) in
// RBX/update.hpp, so include that instead of a second copy.
#include <RBX/update.hpp>
