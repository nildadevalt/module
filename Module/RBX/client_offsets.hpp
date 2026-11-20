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
    const uintptr_t Print                                = REBASE(0x1BF4F90);
    const uintptr_t RawScheduler                         = REBASE(0x8DA5D98);
    const uintptr_t OpcodeLookupTable                    = REBASE(0x72C97B0);
    const uintptr_t AppdataInfoState                     = 0x360;
    const uintptr_t AppdataInfo                          = REBASE(0x13B5770);
    const uintptr_t SingleSurfaceApp                     = REBASE(0x81B7680);
    const uintptr_t SingleSurfaceAppVft                  = REBASE(0x7007380);
    const uintptr_t LuaVMLoad                            = REBASE(0x4123AB0);
    const uintptr_t FireMouseClick                       = REBASE(0x3B3CAC0);
    const uintptr_t FireRightMouseClick                  = REBASE(0x3B3CC60);

    const uintptr_t FireMouseHoverEnter                  = REBASE(0x3B3E030);
    const uintptr_t FireMouseHoverLeave                  = REBASE(0x3B3E1D0);
    const uintptr_t IsLegalSendEvent                     = REBASE(0x468CBB0);

    const uintptr_t FireTouchInterest                    = REBASE(0x13EDE10);
    const uintptr_t FireProximityPrompt                  = REBASE(0x2FC3D70);

    const uintptr_t TaskTable                            = REBASE(0x8CDD140);
    const uintptr_t TaskSynchronize                      = REBASE(0x4210B70);
    const uintptr_t TaskDesynchronize                    = REBASE(0x42110D0);
    const uintptr_t TaskDefer                            = REBASE(0x4211E10);
    const uintptr_t TaskSpawn                            = REBASE(0x42122E0);
    const uintptr_t TaskDelay                            = REBASE(0x42127E0);
    const uintptr_t TaskWait                             = REBASE(0x4212AD0);
    const uintptr_t TaskCancel                           = REBASE(0x4212DA0);

    const uintptr_t ScriptContextResume                  = REBASE(0x41D21D0);
    const uintptr_t GetScriptVMState                     = REBASE(0x412F350);
    const uintptr_t CastArgs                             = REBASE(0x408A130);
    const uintptr_t ConnectionDisconnect                 = REBASE(0x40F6670);

    namespace InstanceBridge {
        const uintptr_t Push                             = REBASE(0x41140D0);
    }

    namespace RobloxProperty {
        const uintptr_t GetProperty                      = REBASE(0x1C2F7D0);
        const uintptr_t KTable                           = REBASE(0x8483460);
        const uintptr_t NameManagerPtr                   = REBASE(0x84DD518);
        const uintptr_t InternPropertyName               = REBASE(0x296B310);
        const uintptr_t LookupPropertyName               = REBASE(0x296B720);
        constexpr uintptr_t PropertyMapOffset            = 0x1E8;
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
        constexpr uintptr_t Capacity                     = 0x1EC;
        constexpr uintptr_t Mask                         = 0x200;
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
        const uintptr_t AsyncTrampoline                  = REBASE(0xB12C90);
        const uintptr_t LambdaVftable                    = REBASE(0x0);
    }

    namespace RobloxThread {
        const uintptr_t IdentityPtr                          = REBASE(0x84AED78);
        const uintptr_t Impersonator                         = REBASE(0x42D1D50);
        const uintptr_t GetTlsPointer                        = REBASE(0xC700);
        constexpr uintptr_t IdentityStructIdentity           = 0x0;
        constexpr uintptr_t IdentityStructCapabilities       = 0x28;
        constexpr uintptr_t IdentityStructCapabilityResolver = 0x30;
    }

    namespace Flags {
        // Kept in sync with FFlagOffsets::FFlags in fflags.hpp (theo's dump for
        // this build). The pre-update values below were stale and made e.g.
        // setfpscap write to the wrong address.
        const uintptr_t LockViolationInstanceCrash       = REBASE(0x813E980);
        const uintptr_t LockViolationScriptCrash         = REBASE(0x8128820);
        const uintptr_t LuaStepIntervalMsOverrideEnabled = REBASE(0x8129250);
        const uintptr_t TaskSchedulerTargetFps           = REBASE(0x8104130);
    }

    namespace Bytecode {
        constexpr unsigned OpcodeWireMultiplier          = 227;
    }

    namespace Luau {
        const uintptr_t Luau_Execute                     = REBASE(0x258E0C0);
        const uintptr_t LuaO_NilObject                   = REBASE(0x67DBCA8);
        const uintptr_t LuaH_DummyNode                   = REBASE(0x67C4778);
        const uintptr_t LuaC_Step                        = REBASE(0x2575E10);
        const uintptr_t GcStep                           = REBASE(0x2575AF0);
        const uintptr_t Freeobj                          = REBASE(0x2574580);
        const uintptr_t Luau_Load                        = REBASE(0x25781D0);
        const uintptr_t LuaT_Eventnames                  = REBASE(0x67DBD50);
        const uintptr_t LuaT_Typenames                   = REBASE(0x67DBCD0);
    }

    const uintptr_t LuaD_throw                           = REBASE(0x2551BB0);
    const uintptr_t luaF_freeproto                       = REBASE(0x2587E10);
    const uintptr_t luaF_newproto                        = REBASE(0x2586E60);
    const uintptr_t luaL_argerrorL                       = REBASE(0x255A830);
    const uintptr_t currfuncname                         = REBASE(0x255A7B0);
    const uintptr_t luaG_runerrorL                       = REBASE(0x2586610); 
    const uintptr_t luaO_pushvfstring                    = REBASE(0x2584460);
    const uintptr_t luaD_rawrunprotected                 = REBASE(0x2551B80);
    const uintptr_t luaA_toobject                        = REBASE(0x254B840);
    const uintptr_t lua_pushvfstring                     = REBASE(0x254CD40);
    const uintptr_t pseudo2addr                          = REBASE(0x254B780);
    const uintptr_t get_capabilites                      = REBASE(0x1C31460);
    const uintptr_t lua_clock                            = REBASE(0x25A9F10);

    namespace LuaError {
        const uintptr_t readonlyerror                    = REBASE(0x2585A80);
        const uintptr_t methoderror                      = REBASE(0x2585A40);
        const uintptr_t indexerror                       = REBASE(0x2585940);
        const uintptr_t ordererror                       = REBASE(0x25858C0);
        const uintptr_t forerrorL                        = REBASE(0x2585790);
    }

    const uintptr_t newpage                              = REBASE(0x25A8E40);
    const uintptr_t newclasspage                         = REBASE(0x25A96D0); 
    const uintptr_t freeclasspage                        = REBASE(0x25A8C80);
    const uintptr_t freeblock                            = REBASE(0x25A87C0);
    const uintptr_t luaM_free                            = REBASE(0x25A8870);
    const uintptr_t luaM_freegco                         = REBASE(0x25A8920);
    const uintptr_t luaM_visitgco                        = REBASE(0x25A8B20);
    const uintptr_t luaM_toobig                          = REBASE(0x25A8790);

    namespace Signal {
        constexpr uintptr_t next              = 0x10;
        constexpr uintptr_t enabled           = 0x20;
        constexpr uintptr_t ListHead          = 0x8;

        const uintptr_t ConnectionDisconnectRaw = REBASE(0x4968FD0);
        const uintptr_t ConnectionUnlink        = REBASE(0x4969420);
        const uintptr_t ConnectionPushFront     = REBASE(0x49698D0);
        const uintptr_t ConnectionConnected     = REBASE(0x4969160);

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
        const uintptr_t FakeDataModelPointer             = REBASE(0x8E16368);
        const uintptr_t InParallelPhase                  = 0x55D;
    }

    namespace ScriptContext {
        const uintptr_t Face                             = 0x9E8;
        const uintptr_t RequireBypass                    = 0xBC4;
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
        const uintptr_t ValueGetUnavailable              = REBASE(0xA1A140);
        const uintptr_t ValueSetUnavailable              = REBASE(0x783B60);
        const uintptr_t OptionalCoordinateFrameDtor      = REBASE(0x1C148E0);
        const uintptr_t IdentityCoordinateFrame          = REBASE(0x66AA3E0);
        const uintptr_t OptionalCoordinateFrameEngaged   = 0x2;
        const uintptr_t OptionalCoordinateFrameTagMask   = 0x3;
        const uintptr_t NetAssetRefRelease               = REBASE(0x1C23F40);
        const uintptr_t NetAssetRefOwnerRep              = 0x8;
        const uintptr_t NetAssetRefHandleGuid            = 0x10;
        const uintptr_t NetAssetRefHandleVersion         = 0x18;
        const uintptr_t NetAssetRefHandleType            = 0x20;
        const uintptr_t NetAssetManagerGet               = REBASE(0x1C244A0);
        const uintptr_t NetAssetProxyHolder              = 0x8;
        const uintptr_t NetAssetProxyVftSolidMesh        = REBASE(0x6FAC560);
        const uintptr_t SolidMeshSize                    = REBASE(0x2AFCC50);
        const uintptr_t SolidMeshWrite                   = REBASE(0x2AFCD30);
        const uintptr_t SharedRepWeakCount               = 0xC;
        const uintptr_t SharedRepDeleteThisVftSlot       = 0x8;
        const uintptr_t PushContent                      = REBASE(0x40E6730);
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
        const uintptr_t Destroy                          = REBASE(0xA2BC20);
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
        const uintptr_t DeviceToSwapChain                = 0x80;
    }

    namespace RakNet {
        const uintptr_t       PeerVTable        = REBASE(0x704EE60);
        const int             VTableSlotCount   = 123;
        const int             SendSlot          = 20;
        const uint32_t        JobToPeerOff      = 0x1E0;
        constexpr const char* JobNameSend       = "Net Peer Send";
        constexpr const char* JobNameRecv       = "Net PacketReceive";
    }

    namespace RbxTransport {
        const uintptr_t       ConnVTable          = REBASE(0x70B4F38);
        const int             VTableSlotCount     = 27;

        const int             SendSlot            = 7;
        const int             SendRawSlot         = 6;

        const uintptr_t       Send                = REBASE(0x39CE700);
        const uintptr_t       SendRaw             = REBASE(0x39CE420);

        const int             ConnShadowSlots     = 29;

        namespace RakNetConn {
            const uintptr_t   VTable              = REBASE(0x7293E58);
            const int         VTableSlotCount     = 28;
            const uintptr_t   Send                = REBASE(0x4A27830);
            const uintptr_t   SendRaw             = REBASE(0x3B52190);
        }
        const uintptr_t       Disconnect          = REBASE(0x39CF7C0);
        const uintptr_t       Flush               = REBASE(0x39CFC50);


        const int             PayloadDataSlot     = 1;
        const uint32_t        PayloadLenOff       = 0x030;
        const uint32_t        RawPayloadDataOff   = 0x010;
        const uint32_t        RawPayloadLenOff    = 0x030;

        namespace Client {
            const uintptr_t   MainVTable           = REBASE(0x713EC90);
            const uintptr_t   InterfaceVTable      = REBASE(0x713EC38);
            const uint32_t    InterfaceVPtrOff     = 0x98;
            const int         InterfaceSlotCount   = 10;
            const int         OnReceiveSlot        = 2;
            const uintptr_t   OnReceive            = REBASE(0x490B000);
            const uint32_t    OwnerConnOff         = 0xF58;
            const uint32_t    OwnerConnScanLimit   = 0x11B8;
        }

        namespace Channel {
            const uintptr_t   InterfaceVTable      = REBASE(0x7041A88);
        }

        namespace AppConnection {
            const int         VTableSlotCount      = 48;
        }
    }

    namespace RobloxThread {
        const uintptr_t GetDataModelLockOwner = REBASE(0xC8A0);
        const uintptr_t GetCapabilities       = REBASE(0x1C31460);
    }

    namespace Scheduler {
        const uintptr_t StepWaitingHybridScripts = REBASE(0x4207740);
    }

    namespace FakeDataModel {
        const uintptr_t     DataModelDeleterPointer = REBASE(0x8E16360);
        const uintptr_t     Pointer                 = REBASE(0x8E16368);
    }

    namespace Signal {
        const uintptr_t WeakThreadRefVftable         = REBASE(0x6EF1AC8);
        const uintptr_t FunctionSlotVftable          = REBASE(0x7117DF0);
        const uintptr_t FunctionSlotAlternateVftable = REBASE(0x7117C68);
        const uintptr_t WaitSlotVftable              = REBASE(0x7117BD8);
        const uintptr_t WeakObjectRefVftable         = REBASE(0x6EE9288);
        const uintptr_t SignalType                   = REBASE(0x8DEBA50);
        const uintptr_t ConnectionType               = REBASE(0x8CED504);
        const uintptr_t GetArguments                 = REBASE(0x419C7D0);
        const uintptr_t RaiseEventInvocation         = REBASE(0x1C44010);
        const uintptr_t Once                         = REBASE(0x40F4F40);
        const uintptr_t Wait                         = REBASE(0x40F50E0);
        const uintptr_t AsyncCallbackVftable         = REBASE(0x0);
        const uintptr_t SyncCallbackVftable          = REBASE(0x0);
        const uintptr_t CallbackBinderVftable        = REBASE(0x0);
    }

    namespace SharedString {
        const uintptr_t Create  = REBASE(0x49745C0);
        const uintptr_t Release = REBASE(0x7B3A70);
        const uintptr_t NodeString = 0x18;
        const uintptr_t NodeRefCount = 0x40;
    }

    namespace Flags {
        const uintptr_t Registry                           = REBASE(0x84E2BF8);
        const uintptr_t ValueGetSetIntVftable              = REBASE(0x7147A20);
        const uintptr_t ValueGetSetDynamicIntVftable       = REBASE(0x7147B38);
        const uintptr_t ValueGetSetStringVftable           = REBASE(0x7147C50);
        const uintptr_t ValueGetSetBoolVftable             = REBASE(0x7147908);
        const uintptr_t ValueGetSetDynamicBoolVftable      = REBASE(0x71477F0);
        const uintptr_t ValueGetSetChannelVftable          = REBASE(0x7147D68);
        const uintptr_t LuauCIProto                        = REBASE(0x0);
        const uintptr_t LuauCallFeedback                   = REBASE(0x0);
        const uintptr_t LuauBackedgeHeapCheck              = REBASE(0x0);
        const uintptr_t LuauPromoteProto                   = REBASE(0x81674E0);
        const uintptr_t DebugLuauUserDefinedClassesRuntime = REBASE(0x81674C0);
        const uintptr_t LuauFastpcall                      = REBASE(0x81E60C0);
        const uintptr_t UseRbxTransportClient              = REBASE(0x811C498);
        const uintptr_t RakNetFallbackToRbxTransport       = REBASE(0x8135AB8);
        const uintptr_t SendsGoThroughConnection           = REBASE(0x80F44C0);
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
        const uintptr_t UserdataIndex    = REBASE(0x4111C30);
        const uintptr_t UserdataNewIndex = REBASE(0x4112D80);
        const uintptr_t Index            = REBASE(0x410FD80);
        const uintptr_t NewIndex         = REBASE(0x410F5E0);
        const uintptr_t UserdataNamecall = REBASE(0x4113430);
        const uintptr_t RegisterUserdata = REBASE(0x2550850);
        const uintptr_t UserdataTag      = REBASE(0x8DEC450);
    }

    namespace Luau {
        const uintptr_t NewUserdataTagged       = REBASE(0x254FC70);
        const uintptr_t Throw                   = REBASE(0x2551BB0);
        const uintptr_t CallInternal            = REBASE(0x254DD60);
        const uintptr_t CallYieldable           = REBASE(0x2551C90);
        const uintptr_t Execute                 = REBASE(0x259B710);
        const uintptr_t ExecuteSingleStep       = REBASE(0x258E8D0);
        const uintptr_t GetTypeName             = REBASE(0x254C4D0);
        const uintptr_t GetObjectTypeName       = REBASE(0x25897E0);
        const uintptr_t GetObjectTypeNameString = REBASE(0x2589650);
        const uintptr_t GetLine                 = REBASE(0x2585450);
    }
}

namespace ClientBuild {
    inline constexpr const char* Version = "version-2c6fd5e09aaf4cfa";
}

// Measured in RobloxPlayerBeta_4ea8e85481e34f46_decrypted.exe:
// luaO_pushvfstring's branch at RVA 0x269F66B and resize call at RVA 0x269FE71.
namespace ClientOffsets::Derived {
    inline const uintptr_t StringTableHashSaltFlag = REBASE(0x82B93E0);
    inline const uintptr_t ResizeSaltedStringTable = REBASE(0x268FFA0);
}
