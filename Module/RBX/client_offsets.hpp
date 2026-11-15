// Source: hyperion_win_live.txt, SHA-256 59eafdc3ee64e80b80985087b7c30e28cd4495cc39233b2e67707d095784b1eb
#pragma once
#include <cstdint>
#include <cstddef>
#include <Windows.h>
#include <memory>
#include <string>

struct lua_State;

#define REBASE(Address) ((Address) == 0 ? uintptr_t{0} : ((Address) + reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr))))

typedef enum { SUCCESS, YIELD, ERR } CoroutineResult;

namespace RBX
{
    struct LiveThreadRef {
        int _Refs;
        char Pad0[0x24];
        lua_State* thread;
        int32_t    thread_ref;
        int32_t    objectId;
    };

    struct debugger_result_t {
        std::int32_t result;
        std::int32_t unk[0x4];
    };
}

namespace ClientOffsets
{
    const uintptr_t Print                                = REBASE(0x1CD77F0);
    const uintptr_t RawScheduler                         = REBASE(0x8F1E408);
    const uintptr_t OpcodeLookupTable                    = REBASE(0x7419400);
    const uintptr_t AppdataInfoState                     = 0x360;
    const uintptr_t AppdataInfo                          = REBASE(0x1495240);
    const uintptr_t SingleSurfaceApp                     = REBASE(0x8336C00);
    const uintptr_t SingleSurfaceAppVft                  = REBASE(0x7159208);
    const uintptr_t LuaVMLoad                            = REBASE(0x42C8020);
    const uintptr_t FireMouseClick                       = REBASE(0x3CC86D0);
    const uintptr_t FireRightMouseClick                  = REBASE(0x3CC8870);

    const uintptr_t FireMouseHoverEnter                  = REBASE(0x3CC9C40);
    const uintptr_t FireMouseHoverLeave                  = REBASE(0x3CC9DE0);
    const uintptr_t IsLegalSendEvent                     = REBASE(0x4851140);

    const uintptr_t FireTouchInterest                    = REBASE(0x14D0000);
    const uintptr_t FireProximityPrompt                  = REBASE(0x312A6F0);

    const uintptr_t TaskTable                            = REBASE(0x8E56DB0);
    const uintptr_t TaskSynchronize                      = REBASE(0x43B9810);
    const uintptr_t TaskDesynchronize                    = REBASE(0x43B9D90);
    const uintptr_t TaskDefer                            = REBASE(0x43BAAD0);
    const uintptr_t TaskSpawn                            = REBASE(0x43BAFB0);
    const uintptr_t TaskDelay                            = REBASE(0x43BB4C0);
    const uintptr_t TaskWait                             = REBASE(0x43BB7C0);
    const uintptr_t TaskCancel                           = REBASE(0x43BBAA0);

    const uintptr_t ScriptContextResume                  = REBASE(0x4379900);
    const uintptr_t GetScriptVMState                     = REBASE(0x42D3B20);
    const uintptr_t CastArgs                             = REBASE(0x422A710);
    const uintptr_t ConnectionDisconnect                 = REBASE(0x4297720);

    namespace InstanceBridge {
        const uintptr_t Push                             = REBASE(0x42B6E70);
    }

    namespace RobloxProperty {
        const uintptr_t GetProperty                      = REBASE(0x1D16EE0);
        const uintptr_t KTable                           = REBASE(0x85FEA80);
        const uintptr_t NameManagerPtr                   = REBASE(0x8659048);
        const uintptr_t InternPropertyName               = REBASE(0x2AA0950);
        const uintptr_t LookupPropertyName               = REBASE(0x2AA0D60);
        constexpr uintptr_t PropertyMapOffset            = 0x1E0;
        constexpr uintptr_t NameId                       = 0x20;
        constexpr uint32_t MaxNameId                     = 0x4000;
    }

    namespace MemberKind {
        constexpr uint32_t Property                      = 0;
        constexpr uint32_t Event                         = 1;
        constexpr uint32_t Function                      = 2;
        constexpr uint32_t Callback                      = 4;
    }

    namespace MemberMap {
        constexpr uintptr_t Capacity                     = 0x1E4;
        constexpr uintptr_t Mask                         = 0x1F8;
    }

    namespace CallbackDesc {
        constexpr uintptr_t IsAsync                      = 0x80;
        constexpr uintptr_t InstanceOffset               = 0x88;
    }

    namespace CallbackChain {
        constexpr uintptr_t HolderObject                 = 0x0;
        // Native callback captures use a shared_ptr's object, not its control
        // block. Verified against the supplied decrypted client and live refs.
        constexpr uintptr_t CaptureObject                = 0x10;
        constexpr uintptr_t CaptureContext               = 0x0;
        constexpr uintptr_t ContextLiveRef               = 0x10;
        constexpr uintptr_t FunctionImpl                 = 0x18;
        constexpr uintptr_t LambdaLiveRef                = 0x10;
        constexpr uintptr_t LiveRefAlive                 = 0x10;
        constexpr uintptr_t LiveRefThread                = 0x28;
        constexpr uintptr_t LiveRefValueRef              = 0x34;
        const uintptr_t BinderVftable                    = REBASE(0x0);
        const uintptr_t AsyncTrampoline                  = REBASE(0xB8D6E0);
        const uintptr_t LambdaVftable                    = REBASE(0x0);
    }

    namespace RobloxThread {
        const uintptr_t IdentityPtr                          = REBASE(0x862AC48);
        const uintptr_t Impersonator                         = REBASE(0x1CEA200);
        const uintptr_t GetTlsPointer                        = REBASE(0xC700);
        constexpr uintptr_t IdentityStructIdentity           = 0x0;
        constexpr uintptr_t IdentityStructCapabilities       = 0x28;
        constexpr uintptr_t IdentityStructCapabilityResolver = 0x30;
    }

    namespace Flags {
        const uintptr_t LockViolationInstanceCrash       = REBASE(0x827DAC8);
        const uintptr_t LockViolationScriptCrash         = REBASE(0x8268820);
        const uintptr_t LuaStepIntervalMsOverrideEnabled = REBASE(0x8269190);
        const uintptr_t TaskSchedulerTargetFps           = REBASE(0x8244258);
    }

    namespace Bytecode {
        constexpr unsigned OpcodeWireMultiplier          = 227;
    }

    namespace Luau {
        const uintptr_t Luau_Execute                     = REBASE(0x26A91B0);
        const uintptr_t LuaO_NilObject                   = REBASE(0x69919C8);
        const uintptr_t LuaH_DummyNode                   = REBASE(0x69767A8);
        const uintptr_t LuaC_Step                        = REBASE(0x268F7F0);
        const uintptr_t GcStep                           = REBASE(0x268F480);
        const uintptr_t Freeobj                          = REBASE(0x268DE20);
        const uintptr_t Luau_Load                        = REBASE(0x2691BD0);
        const uintptr_t LuaT_Eventnames                  = REBASE(0x6991A50);
        const uintptr_t LuaT_Typenames                   = REBASE(0x69919E0);
    }

    const uintptr_t LuaD_throw                           = REBASE(0x266A470);
    const uintptr_t luaF_freeproto                       = REBASE(0x26A2FC0);
    const uintptr_t luaF_newproto                        = REBASE(0x26A2000);
    const uintptr_t luaL_argerrorL                       = REBASE(0x2673020);
    const uintptr_t currfuncname                         = REBASE(0x2672FA0);
    const uintptr_t luaG_runerrorL                       = REBASE(0x26A17A0); 
    const uintptr_t luaO_pushvfstring                    = REBASE(0x269F600);
    const uintptr_t luaD_rawrunprotected                 = REBASE(0x266A440);
    const uintptr_t luaA_toobject                        = REBASE(0x2664200);
    const uintptr_t lua_pushvfstring                     = REBASE(0x2665620);
    const uintptr_t pseudo2addr                          = REBASE(0x2664140);
    const uintptr_t get_capabilites                      = REBASE(0x1D18C30);
    const uintptr_t lua_clock                            = REBASE(0x26C49E0);

    namespace LuaError {
        const uintptr_t readonlyerror                    = REBASE(0x26A0C10);
        const uintptr_t methoderror                      = REBASE(0x26A0BD0);
        const uintptr_t indexerror                       = REBASE(0x26A0AD0);
        const uintptr_t ordererror                       = REBASE(0x26A0A50);
        const uintptr_t forerrorL                        = REBASE(0x26A0920);
    }

    const uintptr_t newpage                              = REBASE(0x26C3910);
    const uintptr_t newclasspage                         = REBASE(0x26C41A0); 
    const uintptr_t freeclasspage                        = REBASE(0x26C3750);
    const uintptr_t freeblock                            = REBASE(0x26C32A0);
    const uintptr_t luaM_free                            = REBASE(0x26C3350);
    const uintptr_t luaM_freegco                         = REBASE(0x26C3400);
    const uintptr_t luaM_visitgco                        = REBASE(0x26C3600);
    const uintptr_t luaM_toobig                          = REBASE(0x26C3270);

    namespace Signal {
        constexpr uintptr_t next              = 0x10;
        constexpr uintptr_t enabled           = 0x20;
        constexpr uintptr_t ListHead          = 0x8;

        const uintptr_t ConnectionDisconnectRaw = REBASE(0x4B477B0);
        const uintptr_t ConnectionUnlink        = REBASE(0x4B47C00);
        const uintptr_t ConnectionPushFront     = REBASE(0x4AF11D0);
        const uintptr_t ConnectionConnected     = REBASE(0x4B47940);

        namespace SignalSlotWrapper {
            constexpr uintptr_t Ptr       = 0x10;
        }

        namespace SignalSlot {
            constexpr uintptr_t disconnected     = 0x20;
            constexpr uintptr_t engineDisconnect = 0x20;
            constexpr uintptr_t isSubmitTask     = 0xB4;
            constexpr uintptr_t isOnce           = 0xB5;
            constexpr uintptr_t onceFired        = 0xB6;
            constexpr uintptr_t connection       = 0x8;
            namespace Refs {
                constexpr uintptr_t luaThreadRefId = 0x30;
                constexpr uintptr_t functionRefId  = 0x34;
            }
        }

    }

    namespace Slot_wrapper {
        constexpr uintptr_t weak_object_ref   = 0x68;
        constexpr uintptr_t thread            = 0x28;
    }

    namespace Connection {
        constexpr uintptr_t next                   = 0x10;
        constexpr uintptr_t enabled                = 0x20;
        constexpr uintptr_t slot_wrapper           = 0x30;
    }

    namespace jobs_offsets {
        const uintptr_t start         = 0xC8;
        const uintptr_t end           = 0xD0;
        const uintptr_t scriptContext = 0x1C0;
        const uintptr_t job_name      = 0x18;
    }

    namespace RunService {
        constexpr uintptr_t RenderStepBindings = 0x110;

        namespace BindingTree {
            constexpr uintptr_t left     = 0x0;
            constexpr uintptr_t parent   = 0x8;
            constexpr uintptr_t right    = 0x10;
            constexpr uintptr_t isNil    = 0x19;
            constexpr uintptr_t priority = 0x20;
            constexpr uintptr_t vecFirst = 0x28;
            constexpr uintptr_t vecLast  = 0x30;
        }

        namespace Binding {
            constexpr uintptr_t stride         = 0x50;
            constexpr uintptr_t name           = 0x0;
            constexpr uintptr_t nameSize       = 0x10;
            constexpr uintptr_t nameCaps       = 0x18;
            constexpr uintptr_t callback       = 0x20;
            constexpr uintptr_t callbackObject = 0x30;
        }

        namespace CallbackRef {
            constexpr uintptr_t owner         = 0x10;
            constexpr uintptr_t thread        = 0x28;
            constexpr uintptr_t threadRefId   = 0x30;
            constexpr uintptr_t functionRefId = 0x34;
        }

        constexpr uintptr_t MaxNameLength      = 0x400;
        constexpr int       MaxPriorityBuckets = 4096;
        constexpr int       MaxBindingsPerNode = 4096;
        constexpr uint32_t  MaxRefId           = 0x00FFFFFF;
    }

    namespace DataModel {
        // Measured in the live client: native owner -> Instance subobject.
        constexpr uintptr_t InstanceSubobject           = 0x1F0;
        const uintptr_t Children                         = 0x70;
        const uintptr_t PlaceId                          = 0x1A0;
        const uintptr_t FakeDataModelToDataModel         = 0x1F8;
        const uintptr_t FakeDataModelPointer             = REBASE(0x8F8E968);
        const uintptr_t InParallelPhase                  = 0x55D;
    }

    namespace ScriptContext {
        const uintptr_t Face                             = 0xA28;
        const uintptr_t RequireBypass                    = 0xC30;
        const uintptr_t DataModel                        = 0x100;
    }

    namespace BasePart {
        const uintptr_t Primitive                        = 0x178;
        namespace PrimitiveStructure {
            const uintptr_t Overlap                      = 0x200;
        }
    }

    namespace Instance {
        const uintptr_t ClassDescriptor                  = 0x18;
        const uintptr_t SharedRefControl                 = 0x10;
        const uintptr_t SharedRefUseCount                = 0x8;
    }

    namespace ClassDescriptor {
        const uintptr_t Name                             = 0x8;
    }

    namespace Property {
        const uintptr_t Name                             = 0x8;
        const uintptr_t LockSlot                         = 0x44;
        const uintptr_t TypeInfo                         = 0x70;
        const uintptr_t InstanceOffset                   = 0x80;  
        const uintptr_t Flags                            = 0xAC;
        const uintptr_t GetSet                           = 0xB0;
        const uintptr_t GetSetRef                        = 0xB8;
        const uintptr_t GetSetVftGetter                  = 0x18;
        const uintptr_t GetSetVftSetter                  = 0x20;
        const uintptr_t RefDescriptorVftGetValue         = 0x100;
        const uintptr_t ValueGetUnavailable              = REBASE(0xA866F0);
        const uintptr_t ValueSetUnavailable              = REBASE(0x7C7140);
        const uintptr_t OptionalCoordinateFrameDtor      = REBASE(0x1CF82D0);
        const uintptr_t IdentityCoordinateFrame          = REBASE(0x6868918);
        const uintptr_t OptionalCoordinateFrameEngaged   = 0x2;
        const uintptr_t OptionalCoordinateFrameTagMask   = 0x3;
        const uintptr_t NetAssetRefRelease               = REBASE(0x1D0AB80);
        const uintptr_t NetAssetRefOwnerRep              = 0x8;
        const uintptr_t NetAssetRefHandleGuid            = 0x10;
        const uintptr_t NetAssetRefHandleVersion         = 0x18;
        const uintptr_t NetAssetRefHandleType            = 0x20;
        const uintptr_t NetAssetManagerGet               = REBASE(0x1D0B4A0);
        const uintptr_t NetAssetProxyHolder              = 0x8;
        const uintptr_t NetAssetProxyVftSolidMesh        = REBASE(0x70FE568);
        const uintptr_t SolidMeshSize                    = REBASE(0x2C2BA60);
        const uintptr_t SolidMeshWrite                   = REBASE(0x2C2BB40);
        const uintptr_t SharedRepWeakCount               = 0xC;
        const uintptr_t SharedRepDeleteThisVftSlot       = 0x8;
        const uintptr_t PushContent                      = REBASE(0x42872D0);
        constexpr uint32_t ContentSourceTypeMax          = 4;
        const uintptr_t ReadCapabilities                 = 0x38;
        constexpr uint64_t SecurityLevelCapabilities     = 0x3FULL;
        constexpr uint32_t FlagScriptable                = 0x10;
    }

    namespace TypeInfo {
        const uintptr_t Name                             = 0x8;
        const uintptr_t TypeNum                          = 0x30;
    }

    namespace EnumItem {
        const uintptr_t Name                             = 0x8;
        const uintptr_t Value                            = 0x30;
    }

    namespace AssetContentMap {
        const uintptr_t Size                             = 0x0;
        const uintptr_t Capacity                         = 0x4;
        const uintptr_t Control                          = 0x8;
        const uintptr_t Slots                            = 0x10;
        const uintptr_t BucketShift                      = 0x1C;
        const uintptr_t HeaderHighWater                  = 0x4;
        const uintptr_t HeaderLeafWordBase               = 0x10;
        const uintptr_t HeaderBitmap                     = 0x18;
        const uintptr_t SlotStride                       = 0x50;
        const uintptr_t SlotKey                          = 0x0;
        const uintptr_t SlotValue                        = 0x20;
        const uintptr_t Destroy                          = REBASE(0xA98610);
    }

    namespace RemoteEvent {
        const uintptr_t OwningInstance                   = 0x18;
        const uintptr_t SignatureFromDescriptor          = 0x60;
        const uintptr_t SignatureTypesStart              = 0x8;
        const uintptr_t SignatureTypeStride              = 0x70;
        const uintptr_t RaiseEventVftSlot                = 0x18;
    }

    namespace LocalScript {
        const uintptr_t Actor                            = 0x60;
        const uintptr_t ProtectedString                  = 0x178;
    }

    namespace ModuleScript {
        const uintptr_t ProtectedString                  = 0x120;
    }

    namespace ProtectedString {
        const uintptr_t Data                             = 0x18;
        const uintptr_t Size                             = 0x28;
    }

    namespace RenderView {
        const uintptr_t RenderJobToView                  = 0x1E0;
        const uintptr_t ViewToDevice                     = 0x10;
        const uintptr_t DeviceToSwapChain                = 0x88;
    }

    namespace RakNet {
        const uintptr_t       PeerVTable        = REBASE(0x71A0638);
        const int             VTableSlotCount   = 124;
        const int             SendSlot          = 20;
        const uint32_t        JobToPeerOff      = 0x1E0;
        constexpr const char* JobNameSend       = "Net Peer Send";
        constexpr const char* JobNameRecv       = "Net PacketReceive";
    }

    namespace RbxTransport {
        const uintptr_t       ConnVTable          = REBASE(0x7206440);
        const int             VTableSlotCount     = 28;

        const int             SendSlot            = 7;
        const int             SendRawSlot         = 6;

        const uintptr_t       Send                = REBASE(0x3B52CB0);
        const uintptr_t       SendRaw             = REBASE(0x3B52A00);

        const int             ConnShadowSlots     = 29;

        namespace RakNetConn {
            const uintptr_t   VTable              = REBASE(0x7293E58);
            const int         VTableSlotCount     = 28;
            const uintptr_t   Send                = REBASE(0x4A27830);
            const uintptr_t   SendRaw             = REBASE(0x3B52190);
        }
        const uintptr_t       Disconnect          = REBASE(0x3B53D50);
        const uintptr_t       Flush               = REBASE(0x3B541E0);


        const int             PayloadDataSlot     = 1;
        const uint32_t        PayloadLenOff       = 0x030;
        const uint32_t        RawPayloadDataOff   = 0x010;
        const uint32_t        RawPayloadLenOff    = 0x030;

        namespace Client {
            const uintptr_t   MainVTable           = REBASE(0x728FA78);
            const uintptr_t   InterfaceVTable      = REBASE(0x728FA20);
            const uint32_t    InterfaceVPtrOff     = 0x98;
            const int         InterfaceSlotCount   = 10;
            const int         OnReceiveSlot        = 2;
            const uintptr_t   OnReceive            = REBASE(0x490B000);
            const uint32_t    OwnerConnOff         = 0xF58;
            const uint32_t    OwnerConnScanLimit   = 0x11B8;
        }

        namespace Channel {
            const uintptr_t   InterfaceVTable      = REBASE(0x7193280);
        }

        namespace AppConnection {
            const int         VTableSlotCount      = 48;
        }
    }

    namespace RobloxThread {
        const uintptr_t GetDataModelLockOwner = REBASE(0xC8A0);
        const uintptr_t GetCapabilities       = REBASE(0x1D18C30);
    }

    namespace Scheduler {
        const uintptr_t StepWaitingHybridScripts = REBASE(0x43B00A0);
    }

    namespace FakeDataModel {
        const uintptr_t     DataModelDeleterPointer = REBASE(0x8F8E960);
        const uintptr_t     Pointer                 = REBASE(0x8F8E968);
    }

    namespace Signal {
        const uintptr_t WeakThreadRefVftable         = REBASE(0x7043E00);
        const uintptr_t FunctionSlotVftable          = REBASE(0x7268EE0);
        const uintptr_t FunctionSlotAlternateVftable = REBASE(0x7268E28);
        const uintptr_t WaitSlotVftable              = REBASE(0x7268BA8);
        const uintptr_t WeakObjectRefVftable         = REBASE(0x703B548);
        const uintptr_t SignalType                   = REBASE(0x8F640B0);
        const uintptr_t ConnectionType               = REBASE(0x8E671E4);
        const uintptr_t GetArguments                 = REBASE(0x4342380);
        const uintptr_t RaiseEventInvocation         = REBASE(0x1D2D000);
        const uintptr_t Once                         = REBASE(0x4237AE0);
        const uintptr_t Wait                         = REBASE(0x4237C80);
        const uintptr_t AsyncCallbackVftable         = REBASE(0x0);
        const uintptr_t SyncCallbackVftable          = REBASE(0x0);
        const uintptr_t CallbackBinderVftable        = REBASE(0x0);
    }

    namespace SharedString {
        const uintptr_t Create  = REBASE(0x4AFC3A0);
        const uintptr_t Release = REBASE(0x7EB520);
        const uintptr_t NodeString = 0x18;
        const uintptr_t NodeRefCount = 0x40;
    }

    namespace Flags {
        const uintptr_t Registry                           = REBASE(0x865E868);
        const uintptr_t ValueGetSetIntVftable              = REBASE(0x7298248);
        const uintptr_t ValueGetSetDynamicIntVftable       = REBASE(0x7298018);
        const uintptr_t ValueGetSetStringVftable           = REBASE(0x7297F00);
        const uintptr_t ValueGetSetBoolVftable             = REBASE(0x7297CD0);
        const uintptr_t ValueGetSetDynamicBoolVftable      = REBASE(0x7298130);
        const uintptr_t ValueGetSetChannelVftable          = REBASE(0x7297DE8);
        const uintptr_t LuauCIProto                        = REBASE(0x0);
        const uintptr_t LuauCallFeedback                   = REBASE(0x0);
        const uintptr_t LuauBackedgeHeapCheck              = REBASE(0x0);
        const uintptr_t LuauPromoteProto                   = REBASE(0x82B94C0);
        const uintptr_t DebugLuauUserDefinedClassesRuntime = REBASE(0x82B94A0);
        const uintptr_t LuauFastpcall                      = REBASE(0x81E60C0);
        const uintptr_t UseRbxTransportClient              = REBASE(0x825C600);
        const uintptr_t RakNetFallbackToRbxTransport       = REBASE(0x8274A50);
        const uintptr_t SendsGoThroughConnection           = REBASE(0x8234448);
        const uintptr_t RefactorIngressFlow                = REBASE(0x8189A90);
        namespace Reg {
            constexpr uintptr_t ListHead      = 0x8;
            constexpr uintptr_t Buckets       = 0x18;
            constexpr uintptr_t Mask          = 0x30;
            constexpr uintptr_t NodePrevLink  = 0x8;
            constexpr uintptr_t NodeNext      = 0x0;
            constexpr uintptr_t NodeKey       = 0x10;
            constexpr uintptr_t NodeEntry     = 0x30;
            constexpr uintptr_t EntryStorage  = 0x20;
            constexpr uintptr_t EntrySnapshot = 0x28;
            constexpr size_t    MaxFlags      = 65536;
        }
    }

    namespace InstanceBridge {
        const uintptr_t UserdataIndex    = REBASE(0x42B4900);
        const uintptr_t UserdataNewIndex = REBASE(0x42B5B00);
        const uintptr_t Index            = REBASE(0x42B2A50);
        const uintptr_t NewIndex         = REBASE(0x42B22A0);
        const uintptr_t UserdataNamecall = REBASE(0x42B61D0);
        const uintptr_t RegisterUserdata = REBASE(0x2669140);
        const uintptr_t UserdataTag      = REBASE(0x8F64AB0);
    }

    namespace Luau {
        const uintptr_t NewUserdataTagged       = REBASE(0x2668590);
        const uintptr_t Throw                   = REBASE(0x266A470);
        const uintptr_t CallInternal            = REBASE(0x2632230);
        const uintptr_t CallYieldable           = REBASE(0x26361C0);
        const uintptr_t Execute                 = REBASE(0x26B6520);
        const uintptr_t ExecuteSingleStep       = REBASE(0x26A99B0);
        const uintptr_t GetTypeName             = REBASE(0x2664E30);
        const uintptr_t GetObjectTypeName       = REBASE(0x26A49C0);
        const uintptr_t GetObjectTypeNameString = REBASE(0x26A4830);
        const uintptr_t GetLine                 = REBASE(0x26A05D0);
    }
}

namespace ClientBuild {
    inline constexpr const char* Version = "version-4ea8e85481e34f46";
}

// Measured in RobloxPlayerBeta_4ea8e85481e34f46_decrypted.exe:
// luaO_pushvfstring's branch at RVA 0x269F66B and resize call at RVA 0x269FE71.
namespace ClientOffsets::Derived {
    inline const uintptr_t StringTableHashSaltFlag = REBASE(0x82B93E0);
    inline const uintptr_t ResizeSaltedStringTable = REBASE(0x268FFA0);
}
