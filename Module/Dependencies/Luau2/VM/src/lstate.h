// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
// This code is based on Lua 5.x implementation licensed under MIT License; see lua_LICENSE.txt for details
#pragma once

#include "lobject.h"
#include "ltm.h"
#include "ludata.h"

// registry
#define registry(L) (&L->global->registry)

// extra stack space to handle TM calls and some other extras
#define EXTRA_STACK 5

#define BASIC_CI_SIZE 8

#define BASIC_STACK_SIZE (2 * LUA_MINSTACK)

// clang-format off
struct stringtable {
    uint32_t saltmult; // 0x0
    int size; // 0x4
    uint32_t saltadd; // 0x8
    TString** hash; // 0x10
    uint32_t nuse; // 0x18
    uint8_t hashshift; // 0x1C
};

// clang-format on

/*
** informations about a call
**
** the general Lua stack frame structure is as follows:
** - each function gets a stack frame, with function "registers" being stack slots on the frame
** - function arguments are associated with registers 0+
** - function locals and temporaries follow after; usually locals are a consecutive block per scope, and temporaries are allocated after this, but
*this is up to the compiler
**
** when function doesn't have varargs, the stack layout is as follows:
** ^ (func) ^^ [fixed args] [locals + temporaries]
** where ^ is the 'func' pointer in CallInfo struct, and ^^ is the 'base' pointer (which is what registers are relative to)
**
** when function *does* have varargs, the stack layout is more complex - the runtime has to copy the fixed arguments so that the 0+ addressing still
*works as follows:
** ^ (func) [fixed args] [varargs] ^^ [fixed args] [locals + temporaries]
**
** computing the sizes of these individual blocks works as follows:
** - the number of fixed args is always matching the `numparams` in a function's Proto object; runtime adds `nil` during the call execution as
*necessary
** - the number of variadic args can be computed by evaluating (ci->base - ci->func - 1 - numparams)
**
** the CallInfo structures are allocated as an array, with each subsequent call being *appended* to this array (so if f calls g, CallInfo for g
*immediately follows CallInfo for f)
** the `nresults` field in CallInfo is set by the caller to tell the function how many arguments the caller is expecting on the stack after the
*function returns
** the `flags` field in CallInfo contains internal execution flags that are important for pcall/etc, see LUA_CALLINFO_*
*/
// clang-format off

struct CallInfo {
    Proto* p; // 0x0
    TValue* func; // 0x8
    TValue* base; // 0x10
    TValue* top; // 0x18
    union {
        const Instruction* savedpc; // 0x20
        int errfunc; // 0x20
    }; // 0x20
    int nresults; // 0x28
    unsigned int flags; // 0x2C
};
// clang-format on

#define LUA_CALLINFO_RETURN (1 << 0) // should the interpreter return after returning from this callinfo? first frame must have this set
#define LUA_CALLINFO_HANDLE (1 << 1) // should the error thrown during execution get handled by continuation from this callinfo? func must be C
#define LUA_CALLINFO_NATIVE (1 << 2) // should this function be executed using execution callback for native code
#define LUA_CALLINFO_OPYIELD (1 << 3) // call frame has yielded on a non-call opcode and requires luaV_finishop

#define curr_func(L) (clvalue(L->ci->func))
#define ci_func(ci) (clvalue((ci)->func))
#define f_isLua(ci) (!ci_func(ci)->isC)
#define isLua(ci) (ttisfunction((ci)->func) && f_isLua(ci))

struct GCStats
{
    // data for proportional-integral controller of heap trigger value
    int32_t triggerterms[32] = {0};
    uint32_t triggertermpos = 0;
    int32_t triggerintegral = 0;

    size_t atomicstarttotalsizebytes = 0;
    size_t endtotalsizebytes = 0;
    size_t heapgoalsizebytes = 0;

    double starttimestamp = 0;
    double atomicstarttimestamp = 0;
    double endtimestamp = 0;
};

struct GCCycleMetrics
{
    size_t starttotalsizebytes = 0;
    size_t heaptriggersizebytes = 0;

    double pausetime = 0.0; // time from end of the last cycle to the start of a new one

    double starttimestamp = 0.0;
    double endtimestamp = 0.0;

    double marktime = 0.0;
    double markassisttime = 0.0;
    double markmaxexplicittime = 0.0;
    size_t markexplicitsteps = 0;
    size_t markwork = 0;

    double atomicstarttimestamp = 0.0;
    size_t atomicstarttotalsizebytes = 0;
    double atomictime = 0.0;

    // specific atomic stage parts
    double atomictimeupval = 0.0;
    double atomictimeweak = 0.0;
    double atomictimegray = 0.0;
    double atomictimeembedder = 0.0;
    double atomictimeclear = 0.0;

    double sweeptime = 0.0;
    double sweepassisttime = 0.0;
    double sweepmaxexplicittime = 0.0;
    size_t sweepexplicitsteps = 0;
    size_t sweepwork = 0;

    size_t assistwork = 0;
    size_t explicitwork = 0;

    size_t propagatework = 0;
    size_t propagateagainwork = 0;

    size_t endtotalsizebytes = 0;
};

struct GCMetrics
{
    double stepexplicittimeacc = 0.0;
    double stepassisttimeacc = 0.0;

    // when cycle is completed, last cycle values are updated
    uint64_t completedcycles = 0;

    GCCycleMetrics lastcycle;
    GCCycleMetrics currcycle;
};

// Callbacks that can be used to to redirect code execution from Luau bytecode VM to a custom implementation (AoT/JiT/sandboxing/...)
struct lua_ExecutionCallbacks
{
    void* context;
    void (*close)(lua_State* L);                 // called when global VM state is closed
    void (*destroy)(lua_State* L, Proto* proto); // called when function is destroyed
    int (*enter)(lua_State* L, Proto* proto);    // called when function is about to start/resume (when execdata is present), return 0 to exit VM
    void (*disable)(lua_State* L, Proto* proto); // called when function has to be switched from native to bytecode in the debugger
    size_t (*getmemorysize)(lua_State* L, Proto* proto); // called to request the size of memory associated with native part of the Proto
    uint8_t (*gettypemapping)(lua_State* L, const char* str, size_t len); // called to get the userdata type index
    char* (*getcounterdata)(
        lua_State* L,
        Proto* proto,
        size_t* count
    ); // called to get the execution counter data and count {uint32_t, uint32_t, uint64_t}
    Proto* (*inlinefunction)(lua_State* L, Closure* caller, Closure* target, uint32_t pc); // called when inlining threshold is reached
    void* reservedClientCallbacks[2];
};

struct lua_UdataDirectAccessData
{
    TValue indextm;
    TValue newindextm;
    TValue namecalltm;
    lua_UserdataDirectAccess index;
    lua_UserdataDirectAccess newindex;
    lua_UserdataDirectNamecall namecall;
};

/*
** `global state', shared by all threads of this state
*/
// clang-format off
struct registryfree_value
{
private:
    int _value;

public:
    registryfree_value() : _value(0) {}
    registryfree_value(int val) : _value(val) {}
    registryfree_value(const registryfree_value& other) : _value(other._value) {}

    void operator=(const registryfree_value& value)
    {
        _value = value._value;
    }

    void operator=(const int& value)
    {
        _value = (_value & 0xF0000000) | (value & 0xFFFFFFF);
    }

    operator const int() const
    {
        return _value & 0xFFFFFFF;
    }

    bool operator==(const int& value) const
    {
        return (_value & 0xFFFFFFF) == value;
    }

    bool operator!=(const int& value) const
    {
        return (_value & 0xFFFFFFF) != value;
    }
};

typedef registryfree_value registryfree_t;
typedef registryfree_value RegistryFree;

struct lua_Page;
struct lua_jmpbuf;
struct lua_CageAlloc { void* ptr; };

// clang-format off
struct LuauPageset { alignas(8) unsigned char opaque[0x150]; };

struct global_State {
    uint8_t gcdisabled; // 0x0
    unsigned char gcstate; // 0x1
    unsigned char currentwhite; // 0x2
    size_t GCthreshold; // 0x8
    size_t totalbytes; // 0x10
    int gcgoal; // 0x18
    int gcstepsize; // 0x1C
    int gcstepmul; // 0x20
    stringtable strt; // 0x28
    lua_Alloc frealloc; // 0x48
    void* ud; // 0x50
    GCObject* gray; // 0x58
    GCObject* grayagain; // 0x60
    GCObject* weak; // 0x68
    lua_Page* freepages[41]; // 0x70
    UpVal uvhead; // 0x1B8
    lua_State* mainthread; // 0x1E0
    lua_Page* sweepgcopage; // 0x1E8
    lua_Page* freegcopages[40]; // 0x1F0
    lua_Page* allgcopages; // 0x330
    TString* tmname[TM_N]; // 0x338
    TString* ttname[LUA_T_COUNT]; // 0x3E0
    LuaTable* mt[LUA_T_COUNT]; // 0x448
    lua_CageAlloc cagealloc; // 0x4B0
    lua_Page* allgcopages_cage; // 0x4B8
    lua_Page* sweepgcopage_cage; // 0x4C0
    lua_Page* freegcopages_cage[40]; // 0x4C8
    void* cageud; // 0x608
    TValue pseudotemp; // 0x610
    TValue registry; // 0x620
    registryfree_value registryfree; // 0x630
    alignas(16) uint64_t rngstate; // 0x640
    lua_Callbacks cb; // 0x648
    unsigned __int64 ptrenckey[4]; // 0x6B8
    lua_ExecutionCallbacks ecb; // 0x6D8
    alignas(16) uint8_t ecbdata[LUA_EXECUTION_CALLBACK_STORAGE]; // 0x730
    lua_UdataDirectAccessData udatadirect[UTAG_INTERNAL_LIMIT]; // 0x930
    LuauPageset taggedpageset[8]; // 0x2DC0
    int sweeppageset; // 0x3840
    size_t memcatbytes[LUA_MEMORY_CATEGORIES]; // 0x3848
    void (*udatagc[LUA_UTAG_LIMIT])(struct lua_State*, void*); // 0x4048
    lua_UserdataMark udatamark[LUA_UTAG_LIMIT]; // 0x4448
    LuaTable* udatamt[LUA_UTAG_LIMIT]; // 0x4848
    TValue weakregistry; // 0x4C48
    int weakregistryfree; // 0x4C58
    int weakregistrytop; // 0x4C5C
    lua_EmbedderGc embeddergc; // 0x4C60
    TString* lightuserdataname[LUA_LUTAG_LIMIT]; // 0x4C68
    struct LuaTable* udatadirectfields[UTAG_INTERNAL_LIMIT]; // 0x5068
    Closure* builtinPcall; // 0x5478
    Closure* builtinXpcall; // 0x5480
    unsigned __int64 ptrenckeynew[8]; // 0x5488
    bool ptrencactive; // 0x54C8
    struct GCStats gcstats; // 0x54D0
    unsigned int lastprotoid; // 0x5588
    unsigned int lastclassid; // 0x558C
#ifdef LUAI_GCMETRICS
    GCMetrics gcmetrics; // 0x5590
    char gap_0[0x8];
#endif
};
// clang-format on

/*
** `per thread' state
*/
// clang-format off
struct lua_State {
    CommonHeader; // 0x0
    uint8_t status; // 0x3
    uint8_t activememcat; // 0x4
    bool singlestep; // 0x5
    bool isactive; // 0x6
    TString* namecall; // 0x8
    struct RobloxExtraSpace* userdata; // 0x10
    CallInfo* end_ci; // 0x18
    CallInfo* base_ci; // 0x20
    unsigned short nCcalls; // 0x28
    unsigned short baseCcalls; // 0x2A
    unsigned int cachedslot; // 0x2C
    UpVal* openupval; // 0x30
    GCObject* gclist; // 0x38
    LSTATE_STACKSIZE_ENC<int> stacksize; // 0x40
    int size_ci; // 0x44
    TValue* stack_last; // 0x48
    CallInfo* ci; // 0x50
    TValue* stack; // 0x58
    global_State* global; // 0x60
    TValue* top; // 0x68
    TValue* base; // 0x70
    LuaTable* gt; // 0x78
    LuaTable* finalizers; // 0x80
};
// clang-format on

/*
** Union of all collectible objects
*/
union GCObject
{
    GCheader gch;
    struct TString ts;
    struct Udata u;
    struct Closure cl;
    struct LuaTable h;
    struct Proto p;
    struct UpVal uv;
    struct lua_State th; // thread
    struct LuauBuffer buf;
    struct LuauClass lclass;
    struct LuauObject lobject;
    struct LuauVector vec;
};

// macros to convert a GCObject into a specific value
#define gco2ts(o) check_exp((o)->gch.tt == LUA_TSTRING, &((o)->ts))
#define gco2u(o) check_exp((o)->gch.tt == LUA_TUSERDATA, &((o)->u))
#define gco2cl(o) check_exp((o)->gch.tt == LUA_TFUNCTION, &((o)->cl))
#define gco2h(o) check_exp((o)->gch.tt == LUA_TTABLE, &((o)->h))
#define gco2p(o) check_exp((o)->gch.tt == LUA_TPROTO, &((o)->p))
#define gco2uv(o) check_exp((o)->gch.tt == LUA_TUPVAL, &((o)->uv))
#define gco2th(o) check_exp((o)->gch.tt == LUA_TTHREAD, &((o)->th))
#define gco2buf(o) check_exp((o)->gch.tt == LUA_TBUFFER, &((o)->buf))
#define gco2class(o) check_exp((o)->gch.tt == LUA_TCLASS, &((o)->lclass))
#define gco2object(o) check_exp((o)->gch.tt == LUA_TOBJECT, &((o)->lobject))
#define gco2vec(o) check_exp((o)->gch.tt == LUA_TVECTOR, &((o)->vec))

// macro to convert any Lua object into a GCObject
#define obj2gco(v) check_exp(iscollectable(v), cast_to(GCObject*, (v) + 0))

LUAI_FUNC lua_State* luaE_newthread(lua_State* L);
LUAI_FUNC void luaE_freethread(lua_State* L, lua_State* L1, struct lua_Page* page);

#include "client_abi_checks.h"
