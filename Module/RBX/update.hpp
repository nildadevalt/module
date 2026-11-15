#pragma once

#include <Windows.h>
#include <RBX/client_offsets.hpp>
#include <cstdint>
#include <memory>
#include <string>

// ══════════════════════════════════════════════════════════════════════════════
// EncryptionsHelper.hpp
// ══════════════════════════════════════════════════════════════════════════════

template<typename T>
struct VMValue0
{
private:
    T Storage;

public:
    operator const T() const
    {
        return Storage;
    }

    void operator=(const T& Value)
    {
        Storage = Value;
    }

    const T operator->() const
    {
        return Storage;
    }

    T Get() const
    {
        return Storage;
    }

    void Set(const T& Value)
    {
        Storage = Value;
    }
};

template<typename T>
struct VMValue1
{
private:
    T Storage;

public:
    operator const T() const
    {
        return (T)((uintptr_t)Storage - (uintptr_t)this);
    }

    void operator=(const T& Value)
    {
        Storage = (T)((uintptr_t)Value + (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }

    T Get() const
    {
        return operator const T();
    }

    void Set(const T& Value)
    {
        operator=(Value);
    }
};

template<typename T>
struct VMValue2
{
private:
    T Storage;

public:
    operator const T() const
    {
        return (T)((uintptr_t)this - (uintptr_t)Storage);
    }

    void operator=(const T& Value)
    {
        Storage = (T)((uintptr_t)this - (uintptr_t)Value);
    }

    const T operator->() const
    {
        return operator const T();
    }

    T Get() const
    {
        return operator const T();
    }

    void Set(const T& Value)
    {
        operator=(Value);
    }
};

template<typename T>
struct VMValue3
{
private:
    T Storage;

public:
    operator const T() const
    {
        return (T)((uintptr_t)this ^ (uintptr_t)Storage);
    }

    void operator=(const T& Value)
    {
        Storage = (T)((uintptr_t)Value ^ (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }

    T Get() const
    {
        return operator const T();
    }

    void Set(const T& Value)
    {
        operator=(Value);
    }
};

template<typename T>
struct VMValue4
{
private:
    T Storage;

public:
    operator const T() const
    {
        return (T)((uintptr_t)this + (uintptr_t)Storage);
    }

    void operator=(const T& Value)
    {
        Storage = (T)((uintptr_t)Value - (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }

    T Get() const
    {
        return operator const T();
    }

    void Set(const T& Value)
    {
        operator=(Value);
    }
};

// ══════════════════════════════════════════════════════════════════════════════
// Encryptions.hpp
// ══════════════════════════════════════════════════════════════════════════════
#define CLOSURE_DEBUGNAME_ENC VMValue0
#define PROTO_P_ENC VMValue0
#define CLOSURE_CONT_ENC VMValue2
#define LSTATE_STACKSIZE_ENC VMValue1
#define PROTO_ABSLINEINFO_ENC VMValue4
#define PROTO_DEBUGINSN_ENC VMValue1
#define PROTO_DEBUGNAME_ENC VMValue1
#define PROTO_LINEINFO_ENC VMValue4
#define PROTO_LOCVARS_ENC VMValue4
#define PROTO_SOURCE_ENC VMValue2
#define PROTO_TYPEINFO_ENC VMValue4
#define PROTO_UPVALUES_ENC VMValue2
#define PROTO_USERDATA_ENC VMValue3
#define TSTRING_HASH_ENC VMValue1
#define UDATA_META_ENC VMValue1


#define proto_lineinfo         PROTO_LINEINFO_ENC
#define proto_debuginsn        PROTO_DEBUGINSN_ENC
#define proto_typeinfo         PROTO_TYPEINFO_ENC
#define proto_abslineinfo      PROTO_ABSLINEINFO_ENC
#define proto_source           PROTO_SOURCE_ENC
#define proto_locvars          PROTO_LOCVARS_ENC
#define proto_upvalues         PROTO_UPVALUES_ENC
#define proto_debugname        PROTO_DEBUGNAME_ENC
#define proto_userdata         PROTO_USERDATA_ENC
#define udata_meta             UDATA_META_ENC
#define closure_cont           CLOSURE_CONT_ENC
#define tstring_hash           TSTRING_HASH_ENC
#define lstate_stacksize       LSTATE_STACKSIZE_ENC

struct lua_State;
struct YieldState;
struct YieldingLuaThread;

struct BytecodeSpan { const char* data; std::size_t size; };
static_assert(sizeof(BytecodeSpan) == 0x10);
using LuaVMLoad_t = int32_t(__fastcall*)(lua_State*, const BytecodeSpan*, const char*, int32_t);


struct LookupResult
{
    std::uint32_t index;
    std::uint8_t missing;
    std::uint8_t reserved[3];
};

namespace Offsets
{
    const uintptr_t Print = ClientOffsets::Print;
    const uintptr_t OpcodeLookupTable = ClientOffsets::OpcodeLookupTable;
    const uintptr_t ScriptContextResume = ClientOffsets::ScriptContextResume;
    const uintptr_t GetLuaStateForInstance = ClientOffsets::GetScriptVMState;
    const uintptr_t LuaVMLoad = ClientOffsets::LuaVMLoad;
    inline const uintptr_t luaD_throw = ClientOffsets::LuaD_throw;

    const uintptr_t TaskDefer = ClientOffsets::TaskDefer;

    namespace reflections
    {
        const uintptr_t CastArgs = ClientOffsets::CastArgs;
    }

    namespace Properties
    {
        const uintptr_t CallbackOffsetInDescriptor = ClientOffsets::CallbackDesc::InstanceOffset;
        const uintptr_t CallbackOwnerOffset = ClientOffsets::CallbackChain::HolderObject;
        const uintptr_t CallbackFunctionOffset = ClientOffsets::CallbackChain::FunctionImpl;
        const uintptr_t LiveThreadRefOffset = ClientOffsets::CallbackChain::LambdaLiveRef;
        const uintptr_t CallbackThreadOffset = ClientOffsets::CallbackChain::LiveRefThread;
        const uintptr_t LuaStateGlobalOffset = 0x60;
        const uintptr_t FunctionRefIdOffset = ClientOffsets::CallbackChain::LiveRefValueRef;
    }

    namespace Signals
    {
        const uintptr_t FireTouchInterest = ClientOffsets::FireTouchInterest;
        const uintptr_t DisconnectConnect = ClientOffsets::ConnectionDisconnect;
    }

    namespace Flags
    {
         const uintptr_t FireMouseClick = ClientOffsets::FireMouseClick;
         const uintptr_t FireRightMouseClick = ClientOffsets::FireRightMouseClick;
         const uintptr_t FireMouseHoverEnter = ClientOffsets::FireMouseHoverEnter;
         const uintptr_t FireMouseHoverLeave = ClientOffsets::FireMouseHoverLeave;
         const uintptr_t FireProximityPrompt = ClientOffsets::FireProximityPrompt;
         const uintptr_t TaskSchedulerTargetFps = ClientOffsets::Flags::TaskSchedulerTargetFps;
    }
    namespace Luau
    {
        const uintptr_t Luau_Execute = ClientOffsets::Luau::Luau_Execute;
        const uintptr_t LuaO_NilObject = ClientOffsets::Luau::LuaO_NilObject;
        const uintptr_t LuaH_DummyNode = ClientOffsets::Luau::LuaH_DummyNode;
    }

    namespace DataModel
    {
        const uintptr_t Children = ClientOffsets::DataModel::Children;
        const uintptr_t ClassDescriptor = ClientOffsets::Instance::ClassDescriptor;
        const uintptr_t FakeDataModelToDataModel = ClientOffsets::DataModel::FakeDataModelToDataModel;
        const uintptr_t FakeDataModelPointer = ClientOffsets::DataModel::FakeDataModelPointer;

    }

    namespace signal_offsets
    {
        constexpr uintptr_t signature = ClientOffsets::RemoteEvent::SignatureFromDescriptor;

        namespace connection
        {
            constexpr uintptr_t next = ClientOffsets::Connection::next;
            constexpr uintptr_t enabled = ClientOffsets::Connection::enabled;
            constexpr uintptr_t slot_wrapper = ClientOffsets::Connection::slot_wrapper;
            constexpr uintptr_t slot_wrapper_self = 0x38;
        }

        namespace slot_wrapper
        {
            constexpr uintptr_t thread = ClientOffsets::Slot_wrapper::thread;
            constexpr uintptr_t function_id = ClientOffsets::Signal::SignalSlot::Refs::functionRefId;
            constexpr uintptr_t weak_object_ref_fallback = 0x38;
            constexpr uintptr_t weak_object_ref = ClientOffsets::Slot_wrapper::weak_object_ref;
            constexpr uintptr_t weak_object_ref_legacy = 0x48;
        }

        namespace getter
        {
            constexpr uintptr_t chain = 0x68;
            constexpr uintptr_t self = 0x18;
            constexpr uintptr_t container = 0x50;
            constexpr uintptr_t container_thread = 0x28;
        }
    }

    namespace Connection
    {
        inline constexpr uintptr_t enabled = ClientOffsets::Connection::enabled;
        inline constexpr uintptr_t next = ClientOffsets::Connection::next;
        inline constexpr uintptr_t slot_wrapper = ClientOffsets::Connection::slot_wrapper;
        inline constexpr uintptr_t slot_wrapper_self = 0x38;
    }

    namespace SlotWrapper
    {
        inline constexpr uintptr_t function_id = ClientOffsets::Signal::SignalSlot::Refs::functionRefId;
        inline constexpr uintptr_t thread = ClientOffsets::Slot_wrapper::thread;
        inline constexpr uintptr_t weak_object_ref = ClientOffsets::Slot_wrapper::weak_object_ref;
    }

    namespace connection = signal_offsets::connection;
    namespace slot_wrapper = signal_offsets::slot_wrapper;

    namespace signal_slot
    {
        inline constexpr uintptr_t is_once = ClientOffsets::Signal::SignalSlot::isOnce;
    }
    namespace Extras
    {
        const uintptr_t k_table = ClientOffsets::RobloxProperty::KTable;
    }

    namespace ExtraSpace
    {
        const uintptr_t RequireBypass = ClientOffsets::ScriptContext::RequireBypass;
        const uintptr_t ScriptContextToResume = ClientOffsets::ScriptContext::Face;
    }

    namespace Scripts
    {
        const uintptr_t LocalScriptByteCode = ClientOffsets::LocalScript::ProtectedString;
        const uintptr_t ModuleScriptByteCode = ClientOffsets::ModuleScript::ProtectedString;
    }

    namespace InstanceBridge
    {
        const uintptr_t Push = ClientOffsets::InstanceBridge::Push;
    }


    namespace ClassDescriptor
    {
        const uintptr_t GetPropertyData = ClientOffsets::RobloxProperty::InternPropertyName;
        const uintptr_t GetProperty = ClientOffsets::RobloxProperty::GetProperty;
    }

    namespace RobloxThread
    {
        const uintptr_t IdentityPtr = ClientOffsets::RobloxThread::IdentityPtr;
        const uintptr_t GetTlsPointer = ClientOffsets::RobloxThread::GetTlsPointer;
    }

    namespace Scheduler
    {
        const uintptr_t TaskScheduler = ClientOffsets::RawScheduler;//?
        const uintptr_t JobStart = ClientOffsets::jobs_offsets::start;
        const uintptr_t JobEnd = ClientOffsets::jobs_offsets::end;
        const uintptr_t JobTypeName = ClientOffsets::jobs_offsets::job_name;
    }

    namespace Render
    {
        const uintptr_t RenderJobToRenderView = ClientOffsets::RenderView::RenderJobToView;
        const uintptr_t RenderViewToRobloxDevice = ClientOffsets::RenderView::ViewToDevice;
        const uintptr_t RobloxDeviceToSwapChain = ClientOffsets::RenderView::DeviceToSwapChain;
    }


    namespace Capabilities
    {
        const uintptr_t GetCapabilities = ClientOffsets::RobloxThread::GetCapabilities;
        const uintptr_t Capabilities = ClientOffsets::RobloxThread::IdentityStructCapabilities;
    }
    namespace Instance
    {
        const uintptr_t PropertyDescriptorBitFlags = ClientOffsets::Property::Flags;
        const uintptr_t Properties = 0x28;
        const uintptr_t ScriptableMask = ClientOffsets::Property::FlagScriptable;
        const uintptr_t ClassDescriptor = ClientOffsets::Instance::ClassDescriptor;
        const uintptr_t ClassName = ClientOffsets::ClassDescriptor::Name;
        const uintptr_t Primitive = ClientOffsets::BasePart::Primitive;
        const uintptr_t Overlap = ClientOffsets::BasePart::PrimitiveStructure::Overlap;
        const uintptr_t Parent = 0x60;
        const uintptr_t Children = ClientOffsets::DataModel::Children;
        inline constexpr uintptr_t Name = 0x68;
        inline constexpr uintptr_t NameString = 0x8;
    }
    namespace Raknet
    {
        const uintptr_t RakPeer = ClientOffsets::RakNet::JobToPeerOff;
        const uintptr_t RakPeerVTableSize = ClientOffsets::RakNet::VTableSlotCount * sizeof(uintptr_t);
        const uintptr_t idx_Connect = 13;
        const uintptr_t idx_Send = ClientOffsets::RakNet::SendSlot;
        const uintptr_t idx_Receive = 25;
    }
}

namespace BytecodeOffset
{
    inline constexpr uintptr_t Pointer = ClientOffsets::ProtectedString::Data;
    inline constexpr uintptr_t Size = ClientOffsets::ProtectedString::Size;
    inline constexpr uintptr_t LocalScriptByteCode = ClientOffsets::LocalScript::ProtectedString;
    inline constexpr uintptr_t ModuleScriptByteCode = ClientOffsets::ModuleScript::ProtectedString;

}

namespace Roblox
{
    inline uintptr_t* k_table = reinterpret_cast<uintptr_t*>(Offsets::Extras::k_table);
    inline auto Print = (uintptr_t(*)(int, const char*, ...))Offsets::Print;
    inline auto Luau_Execute = (void(__fastcall*)(lua_State*))Offsets::Luau::Luau_Execute;
    inline auto GetLuaStateForInstance = (lua_State * (__fastcall*)(uint64_t, uint64_t*, uint64_t*))Offsets::GetLuaStateForInstance;
    inline auto ScriptContextResume = reinterpret_cast<int(__fastcall*)(uintptr_t, RBX::debugger_result_t*, RBX::LiveThreadRef**, int32_t, bool, const std::string*)>(Offsets::ScriptContextResume);

    inline LuaVMLoad_t LuaVMLoad = reinterpret_cast<LuaVMLoad_t>(Offsets::LuaVMLoad);

    namespace Signal
    {
        inline auto FireTouchInterest = (void(__fastcall*)(uintptr_t, uintptr_t, uintptr_t, bool, bool))Offsets::Signals::FireTouchInterest;
    }

    namespace InstanceBridge
    {
        inline auto UintPtr = (void(__fastcall*)(lua_State*, std::uintptr_t))Offsets::InstanceBridge::Push;
        inline auto SharedInstance = (void(__fastcall*)(lua_State*, std::shared_ptr<std::uintptr_t*>))Offsets::InstanceBridge::Push;
        inline auto Void = (void(__fastcall*)(lua_State*, void**))Offsets::InstanceBridge::Push;
        inline auto Void2 = (void(__fastcall*)(lua_State*, void*))Offsets::InstanceBridge::Push;
        inline auto Weak = (void(__fastcall*)(lua_State*, std::weak_ptr<std::uintptr_t>))Offsets::InstanceBridge::Push;
    }
    using InternT = std::uintptr_t(__fastcall*)(std::uintptr_t, const char*);
    using FindBucketT = LookupResult * (__fastcall*)(std::uintptr_t, LookupResult*, const std::uintptr_t*, std::uint32_t);

    inline std::uintptr_t InternName(const char* name)
    {
        const auto function = reinterpret_cast<InternT>(Offsets::ClassDescriptor::GetPropertyData);
        const auto manager = *reinterpret_cast<const uintptr_t*>(ClientOffsets::RobloxProperty::NameManagerPtr);
        return manager && name ? function(manager, name) : 0;
    }

    inline LookupResult* FindBucket(std::uintptr_t table, LookupResult* output, const std::uintptr_t* key, std::uint32_t mode)
    {
        const auto function = reinterpret_cast<FindBucketT>(Offsets::ClassDescriptor::GetProperty);
        return function(table, output, key, mode);
    }
    using fireclick_t = void(__fastcall*)(void* clickDetector, float distance, void* player);
    inline auto FireMouseClick = reinterpret_cast<fireclick_t>(Offsets::Flags::FireMouseClick);
    inline auto FireRightMouseClick = reinterpret_cast<fireclick_t>(Offsets::Flags::FireRightMouseClick);

    using firehover_t = void(__fastcall*)(void* clickDetector, void* player);
    inline auto FireMouseHover = reinterpret_cast<firehover_t>(Offsets::Flags::FireMouseHoverEnter);
    inline auto FireMouseHoverLeave = reinterpret_cast<firehover_t>(Offsets::Flags::FireMouseHoverLeave);

    inline auto CastArgs = reinterpret_cast<uintptr_t(__fastcall*)(lua_State*, int, void*, bool, int)>(Offsets::reflections::CastArgs);
    inline auto GetCapabilities = (__int64(__fastcall*)(int*))Offsets::Capabilities::GetCapabilities;

    using TGetIdentityStruct = void*(__fastcall*)(uintptr_t, uint32_t);
    inline uintptr_t GetIdentityStruct(uintptr_t identityData)
    {
        if (!identityData) return 0;
        return reinterpret_cast<uintptr_t>(reinterpret_cast<TGetIdentityStruct>(Offsets::RobloxThread::GetTlsPointer)(identityData, 1));
    }

    inline auto FireProximityPrompt = (void(__thiscall*)(uintptr_t))Offsets::Flags::FireProximityPrompt;
    inline auto FireTouchInterest = (void(__fastcall*)(uintptr_t, uintptr_t, uintptr_t, bool, bool))Offsets::Signals::FireTouchInterest;

    inline auto PushInstance = (void(__fastcall*)(lua_State*, void*))Offsets::InstanceBridge::Push;
    inline auto DisconnectConnection = (void(__fastcall*)(uintptr_t*))Offsets::Signals::DisconnectConnect;

    using GetPropertyT = uintptr_t(*)(uintptr_t, uintptr_t*);
    using GetPropertyDataT = uintptr_t(*)(uintptr_t, uintptr_t*);

    inline GetPropertyT GetProperty = reinterpret_cast<GetPropertyT>(Offsets::ClassDescriptor::GetProperty);
    inline GetPropertyDataT GetPropertyData = reinterpret_cast<GetPropertyDataT>(Offsets::ClassDescriptor::GetPropertyData);


    inline auto Void2 = reinterpret_cast<void(__fastcall*)(lua_State*, void*)>(Offsets::InstanceBridge::Push);

    using TaskDeferT = int(__fastcall*)(lua_State*);
    inline TaskDeferT TaskDefer = reinterpret_cast<TaskDeferT>(Offsets::TaskDefer);

    using LuaD_ThrowT = void(__fastcall*)(lua_State*, int);
    inline LuaD_ThrowT LuaD_Throw = reinterpret_cast<LuaD_ThrowT>(Offsets::luaD_throw);


}
