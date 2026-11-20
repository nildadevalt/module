// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
#pragma once

#include <string.h>

// Compiler codegen control macros
#ifdef _MSC_VER
#define LUAU_NORETURN __declspec(noreturn)
#define LUAU_NOINLINE __declspec(noinline)
#define LUAU_MAYBE_UNUSED
#define LUAU_FORCEINLINE __forceinline
#define LUAU_LIKELY(x) x
#define LUAU_UNLIKELY(x) x
#define LUAU_UNREACHABLE() __assume(false)
#define LUAU_DEBUGBREAK() __debugbreak()
#else
#define LUAU_NORETURN __attribute__((__noreturn__))
#define LUAU_NOINLINE __attribute__((noinline))
#define LUAU_MAYBE_UNUSED __attribute__((unused))
#define LUAU_FORCEINLINE inline __attribute__((always_inline))
#define LUAU_LIKELY(x) __builtin_expect(x, 1)
#define LUAU_UNLIKELY(x) __builtin_expect(x, 0)
#define LUAU_UNREACHABLE() __builtin_unreachable()
#define LUAU_DEBUGBREAK() __builtin_trap()
#endif

// LUAU_FALLTHROUGH is a C++11-compatible alternative to [[fallthrough]] for use in the VM library
#if defined(__clang__) && defined(__has_warning)
#if __has_feature(cxx_attributes) && __has_warning("-Wimplicit-fallthrough")
#define LUAU_FALLTHROUGH [[clang::fallthrough]]
#else
#define LUAU_FALLTHROUGH
#endif
#elif defined(__GNUC__) && __GNUC__ >= 7
#define LUAU_FALLTHROUGH [[gnu::fallthrough]]
#else
#define LUAU_FALLTHROUGH
#endif

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define LUAU_BIG_ENDIAN
#endif

namespace Luau
{

using AssertHandler = int (*)(const char* expression, const char* file, int line, const char* function);

inline AssertHandler& assertHandler()
{
    static AssertHandler handler = nullptr;
    return handler;
}

// We want 'inline' to correctly link this function declared in the header
// But we also want to prevent compiler from inlining this function when optimization and assertions are enabled together
// Reason for that is that compilation times can increase significantly in such a configuration
LUAU_NOINLINE inline int assertCallHandler(const char* expression, const char* file, int line, const char* function)
{
    if (AssertHandler handler = assertHandler())
        return handler(expression, file, line, function);

    return 1;
}

} // namespace Luau

#if !defined(NDEBUG) || defined(LUAU_ENABLE_ASSERT)
#define LUAU_ASSERT(expr) ((void)(!!(expr) || (Luau::assertCallHandler(#expr, __FILE__, __LINE__, __FUNCTION__) && (LUAU_DEBUGBREAK(), 0))))
#define LUAU_ASSERTENABLED
#else
#define LUAU_ASSERT(expr) (void)sizeof(!!(expr))
#endif

namespace Luau
{

template<typename T>
struct FValue
{
    static FValue* list;

    T value;
    bool dynamic;
    const char* name;
    FValue* next;
    unsigned int version = 0;

    FValue(const char* name, T def, bool dynamic)
        : value(def)
        , dynamic(dynamic)
        , name(name)
        , next(list)
    {
        list = this;
    }

    LUAU_FORCEINLINE operator T() const
    {
        return value;
    }
};

template<typename T>
FValue<T>* FValue<T>::list = nullptr;

struct FValueVersionSetter
{
    FValueVersionSetter(const char* name, unsigned int version)
    {
        bool found = false;
        for (Luau::FValue<bool>* flag = Luau::FValue<bool>::list; flag; flag = flag->next)
        {
            if (strcmp(flag->name, name) == 0)
            {
                flag->version = version;
                found = true;
            }
        }
        for (Luau::FValue<int>* flag = Luau::FValue<int>::list; flag; flag = flag->next)
        {
            if (strcmp(flag->name, name) == 0)
            {
                flag->version = version;
                found = true;
            }
        }
        LUAU_ASSERT(found && "LUAU_FLAGVERSION must appear after the flag definition in the same source file");
    }
};

} // namespace Luau

namespace Luau
{
template<bool V>
struct FConstant
{
    static constexpr bool value = V;
    constexpr operator bool() const { return V; }
};
}

#define LUAU_FLAG_EXPAND(x) x
#define LUAU_FLAG_SECOND(a, b, ...) b
#define LUAU_FLAG_PROBE(...) LUAU_FLAG_EXPAND(LUAU_FLAG_SECOND(__VA_ARGS__, 0, 0))
#define LUAU_FLAG_ISPINNED(flag) LUAU_FLAG_PROBE(LUAU_FLAG_PINNED_##flag)
#define LUAU_FASTFLAG(flag) LUAU_FASTFLAG_SEL(LUAU_FLAG_ISPINNED(flag), flag)
#define LUAU_FASTFLAG_SEL(pinned, flag) LUAU_FASTFLAG_SEL2(pinned, flag)
#define LUAU_FASTFLAG_SEL2(pinned, flag) LUAU_FASTFLAG_##pinned(flag)
#define LUAU_FASTFLAG_0(flag) \
    namespace FFlag \
    { \
    extern Luau::FValue<bool> flag; \
    }
#define LUAU_FASTFLAG_1(flag)
#define LUAU_FASTFLAGVARIABLE(flag) LUAU_FASTFLAGVARIABLE_SEL(LUAU_FLAG_ISPINNED(flag), flag)
#define LUAU_FASTFLAGVARIABLE_SEL(pinned, flag) LUAU_FASTFLAGVARIABLE_SEL2(pinned, flag)
#define LUAU_FASTFLAGVARIABLE_SEL2(pinned, flag) LUAU_FASTFLAGVARIABLE_##pinned(flag)
#define LUAU_FASTFLAGVARIABLE_0(flag) \
    namespace FFlag \
    { \
    Luau::FValue<bool> flag(#flag, false, false); \
    }
#define LUAU_FASTFLAGVARIABLE_1(flag)

#define LUAU_FLAG_PINNED_LuauCIProto ~, 1
#define LUAU_FLAG_PINNED_LuauDirectFieldGet ~, 1
#define LUAU_FLAG_PINNED_LuauUdataDirectAccess6 ~, 1
#define LUAU_FLAG_PINNED_LuauUdataMetatablePinned ~, 1
#define LUAU_FLAG_PINNED_LuauAutoStack ~, 1
#define LUAU_FLAG_PINNED_LuauCloneTableFix ~, 1
#define LUAU_FLAG_PINNED_LuauVirtualBcBuilder ~, 1
#define LUAU_FLAG_PINNED_LuauStoreConstKeywordBegin ~, 1
#define LUAU_FLAG_PINNED_LuauTrackPrefixLocal ~, 1
#define LUAU_FLAG_PINNED_LuauAllowGlobalDeclarationToBeCalledClass ~, 1
#define LUAU_FLAG_PINNED_LuauDisallowExternClassInTypeDefinitions ~, 1
#define LUAU_FLAG_PINNED_LuauFrozenMetaButterfly ~, 1
#define LUAU_FLAG_PINNED_LuauNewPointerEncode ~, 1
#define LUAU_FLAG_PINNED_LuauTableArrayShrinkOrder ~, 1
#define LUAU_FLAG_PINNED_LuauEasyStateInit ~, 1
#define LUAU_FLAG_PINNED_LuauFastpcall ~, 1
#define LUAU_FLAG_PINNED_LuauLoadRemapOptionalUserdata ~, 1
#define LUAU_FLAG_PINNED_LuauCostModel ~, 1
#define LUAU_FLAG_PINNED_LuauEmitCallFeedback ~, 1

namespace FFlag
{
inline constexpr Luau::FConstant<true> LuauCIProto{};
inline constexpr Luau::FConstant<true> LuauDirectFieldGet{};
inline constexpr Luau::FConstant<false> LuauUdataDirectAccess6{};
inline constexpr Luau::FConstant<true> LuauUdataMetatablePinned{};
inline constexpr Luau::FConstant<true> LuauAutoStack{};
inline constexpr Luau::FConstant<true> LuauCloneTableFix{};
inline constexpr Luau::FConstant<true> LuauVirtualBcBuilder{};
inline constexpr Luau::FConstant<true> LuauStoreConstKeywordBegin{};
inline constexpr Luau::FConstant<true> LuauTrackPrefixLocal{};
inline constexpr Luau::FConstant<true> LuauAllowGlobalDeclarationToBeCalledClass{};
inline constexpr Luau::FConstant<true> LuauDisallowExternClassInTypeDefinitions{};
inline constexpr Luau::FConstant<true> LuauFrozenMetaButterfly{};
inline constexpr Luau::FConstant<true> LuauNewPointerEncode{};
inline constexpr Luau::FConstant<true> LuauTableArrayShrinkOrder{};
inline constexpr Luau::FConstant<true> LuauEasyStateInit{};
inline constexpr Luau::FConstant<true> LuauFastpcall{};
inline constexpr Luau::FConstant<true> LuauLoadRemapOptionalUserdata{};
inline constexpr Luau::FConstant<true> LuauCostModel{};
inline constexpr Luau::FConstant<true> LuauEmitCallFeedback{};
}
#define LUAU_FASTINT(flag) \
    namespace FInt \
    { \
    extern Luau::FValue<int> flag; \
    }
#define LUAU_FASTINTVARIABLE(flag, def) \
    namespace FInt \
    { \
    Luau::FValue<int> flag(#flag, def, false); \
    }

#define LUAU_DYNAMIC_FASTFLAG(flag) \
    namespace DFFlag \
    { \
    extern Luau::FValue<bool> flag; \
    }
#define LUAU_DYNAMIC_FASTFLAGVARIABLE(flag, def) \
    namespace DFFlag \
    { \
    Luau::FValue<bool> flag(#flag, def, true); \
    }
#define LUAU_DYNAMIC_FASTINT(flag) \
    namespace DFInt \
    { \
    extern Luau::FValue<int> flag; \
    }
#define LUAU_DYNAMIC_FASTINTVARIABLE(flag, def) \
    namespace DFInt \
    { \
    Luau::FValue<int> flag(#flag, def, true); \
    }

#define LUAU_FLAGVERSION(flag, version) LUAU_FLAGVERSION_SEL(LUAU_FLAG_ISPINNED(flag), flag, version)
#define LUAU_FLAGVERSION_SEL(pinned, flag, version) LUAU_FLAGVERSION_SEL2(pinned, flag, version)
#define LUAU_FLAGVERSION_SEL2(pinned, flag, version) LUAU_FLAGVERSION_##pinned(flag, version)
#define LUAU_FLAGVERSION_0(flag, version) \
    static_assert((version) != 0, "LUAU_FLAGVERSION version cannot be 0"); \
    static Luau::FValueVersionSetter flag##_VersionSetter(#flag, version);
#define LUAU_FLAGVERSION_1(flag, version)

#if defined(__GNUC__)
#define LUAU_PRINTF_ATTR(fmt, arg) __attribute__((format(printf, fmt, arg)))
#else
#define LUAU_PRINTF_ATTR(fmt, arg)
#endif
