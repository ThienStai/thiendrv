#include "structs.h"
#include <ntifs.h>
typedef unsigned short WORD;
typedef struct _SE_AUDIT_PROCESS_CREATION_INFO
{
	POBJECT_NAME_INFORMATION ImageFileName;
} SE_AUDIT_PROCESS_CREATION_INFO, * PSE_AUDIT_PROCESS_CREATION_INFO;

typedef struct _MMSUPPORT
{
	LIST_ENTRY WorkingSetExpansionLinks;
	WORD LastTrimStamp;
	WORD NextPageColor;
	ULONG PageFaultCount;
	ULONG PeakWorkingSetSize;
	ULONG Spare0;
	ULONG MinimumWorkingSetSize;
	ULONG MaximumWorkingSetSize;
	ULONG Claim;
	ULONG Spare[1];
	ULONG WorkingSetPrivateSize;
	ULONG WorkingSetSizeOverhead;
	ULONG WorkingSetSize;
	PKEVENT ExitEvent;
	EX_PUSH_LOCK WorkingSetMutex;
	PVOID AccessLog;
} MMSUPPORT, * PMMSUPPORT;
typedef struct _HARDWARE_PTE
{
	union
	{
		ULONG Valid;
		ULONG Write;
		ULONG Owner;
		ULONG WriteThrough;
		ULONG CacheDisable;
		ULONG Accessed;
		ULONG Dirty;
		ULONG LargePage;
		ULONG Global;
		ULONG CopyOnWrite;
		ULONG Prototype;
		ULONG reserved0;
		ULONG PageFrameNumber;
		ULONG reserved1;
		ULONG LowPart;
	};
	ULONG HighPart;
} HARDWARE_PTE, * PHARDWARE_PTE;
typedef struct __EPROCESS_QUOTA_BLOCK
{
	WORD buffer[15];                                                        //0x0
	struct _LIST_ENTRY QuotaList;                                           //0x30
	ULONG ReferenceCount;                                                   //0x38
	ULONG ProcessCount;                                                     //0x3c
} _EPROCESS_QUOTA_BLOCK;
typedef struct __PAGEFAULT_HISTORY
{
	ULONG CurrentIndex;                                                     //0x0
	ULONG MaxIndex;                                                         //0x4
	ULONG SpinLock;                                                         //0x8
	VOID* Reserved;                                                         //0xc
	struct _PROCESS_WS_WATCH_INFORMATION WatchInfo[1];                      //0x10
} _PAGEFAULT_HISTORY;
typedef struct _MM_AVL_TABLE {
	//MMADDRESS_NODE BalancedRoot;
	ULONG DepthOfTree;
	ULONG Unused;
	ULONG NumberGenericTableElements;
	PVOID NodeHint;
	PVOID NodeFreeHint;
} MM_AVL_TABLE, * PMM_AVL_TABLE;
typedef struct _MMADDRESS_NODE
{
	ULONG u1;
	PMMADDRESS_NODE LeftChild;
	PMMADDRESS_NODE RightChild;
	ULONG StartingVpn;
	ULONG EndingVpn;
} MMADDRESS_NODE, * PMMADDRESS_NODE;
typedef struct _EX_FAST_REF
{
	union
	{
		PVOID Object;
		ULONG RefCnt;
		ULONG Value;
	};
} EX_FAST_REF, * PEX_FAST_REF;


typedef struct _MM_AVL_TABLE
{
	MMADDRESS_NODE BalancedRoot;
	ULONG DepthOfTree;
	ULONG Unused;
	ULONG NumberGenericTableElements;
	PVOID NodeHint;
	PVOID NodeFreeHint;
} MM_AVL_TABLE, * PMM_AVL_TABLE;

typedef struct _KEXECUTE_OPTIONS
{
	ULONG ExecuteDisable;
	ULONG ExecuteEnable;
	ULONG DisableThunkEmulation;
	ULONG Permanent;
	ULONG ExecuteDispatchEnable;
	ULONG ImageDispatchEnable;
	ULONG Spare;
} KEXECUTE_OPTIONS, * PKEXECUTE_OPTIONS;


typedef struct _KGDTENTRY
{
	WORD LimitLow;
	WORD BaseLow;
	ULONG HighWord;
} KGDTENTRY, * PKGDTENTRY;


typedef struct _KIDTENTRY
{
	WORD Offset;
	WORD Selector;
	WORD Access;
	WORD ExtendedOffset;
} KIDTENTRY, * PKIDTENTRY;


typedef struct _KPROCESS
{
	DISPATCHER_HEADER Header;
	LIST_ENTRY ProfileListHead;
	ULONG DirectoryTableBase;
	ULONG Unused0;
	KGDTENTRY LdtDescriptor;
	KIDTENTRY Int21Descriptor;
	WORD IopmOffset;
	UCHAR Iopl;
	UCHAR Unused;
	ULONG ActiveProcessors;
	ULONG KernelTime;
	ULONG UserTime;
	LIST_ENTRY ReadyListHead;
	SINGLE_LIST_ENTRY SwapListEntry;
	PVOID VdmTrapcHandler;
	LIST_ENTRY ThreadListHead;
	ULONG ProcessLock;
	ULONG Affinity;
	union
	{
		ULONG AutoAlignment;
		ULONG DisableBoost;
		ULONG DisableQuantum;
		ULONG ReservedFlags;
		LONG ProcessFlags;
	};
	CHAR BasePriority;
	CHAR QuantumReset;
	UCHAR State;
	UCHAR ThreadSeed;
	UCHAR PowerState;
	UCHAR IdealNode;
	UCHAR Visited;
	union
	{
		KEXECUTE_OPTIONS Flags;
		UCHAR ExecuteOptions;
	};
	ULONG StackCount;
	LIST_ENTRY ProcessListEntry;
	UINT64 CycleTime;
} KPROCESS, * PKPROCESS;

typedef struct _EPROCESS
{
	KPROCESS Pcb;
	EX_PUSH_LOCK ProcessLock;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER ExitTime;
	EX_RUNDOWN_REF RundownProtect;
	PVOID UniqueProcessId;
	LIST_ENTRY ActiveProcessLinks;
	ULONG QuotaUsage[3];
	ULONG QuotaPeak[3];
	ULONG CommitCharge;
	ULONG PeakVirtualSize;
	ULONG VirtualSize;
	LIST_ENTRY SessionProcessLinks;
	PVOID DebugPort;
	union
	{
		PVOID ExceptionPortData;
		ULONG ExceptionPortValue;
		ULONG ExceptionPortState;
	};
	//PHANDLE_TABLE ObjectTable;
	EX_FAST_REF Token;
	ULONG WorkingSetPage;
	EX_PUSH_LOCK AddressCreationLock;
	PETHREAD RotateInProgress;
	PETHREAD ForkInProgress;
	ULONG HardwareTrigger;
	//PMM_AVL_TABLE PhysicalVadRoot;
	PVOID CloneRoot;
	ULONG NumberOfPrivatePages;
	ULONG NumberOfLockedPages;
	PVOID Win32Process;
	PEJOB Job;
	PVOID SectionObject;
	PVOID SectionBaseAddress;
	_EPROCESS_QUOTA_BLOCK* QuotaBlock;
	_PAGEFAULT_HISTORY* WorkingSetWatch;
	PVOID Win32WindowStation;
	PVOID InheritedFromUniqueProcessId;
	PVOID LdtInformation;
	PVOID VadFreeHint;
	PVOID VdmObjects;
	PVOID DeviceMap;
	PVOID EtwDataSource;
	PVOID FreeTebHint;
	union
	{
		HARDWARE_PTE PageDirectoryPte;
		UINT64 Filler;
	};
	PVOID Session;
	UCHAR ImageFileName[16];
	LIST_ENTRY JobLinks;
	PVOID LockedPagesList;
	LIST_ENTRY ThreadListHead;
	PVOID SecurityPort;
	PVOID PaeTop;
	ULONG ActiveThreads;
	ULONG ImagePathHash;
	ULONG DefaultHardErrorProcessing;
	LONG LastThreadExitStatus;
	PPEB Peb;
	EX_FAST_REF PrefetchTrace;
	LARGE_INTEGER ReadOperationCount;
	LARGE_INTEGER WriteOperationCount;
	LARGE_INTEGER OtherOperationCount;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;
	ULONG CommitChargeLimit;
	ULONG CommitChargePeak;
	PVOID AweInfo;
	SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;
	MMSUPPORT Vm;
	LIST_ENTRY MmProcessLinks;
	ULONG ModifiedPageCount;
	ULONG Flags2;
	ULONG JobNotReallyActive;
	ULONG AccountingFolded;
	ULONG NewProcessReported;
	ULONG ExitProcessReported;
	ULONG ReportCommitChanges;
	ULONG LastReportMemory;
	ULONG ReportPhysicalPageChanges;
	ULONG HandleTableRundown;
	ULONG NeedsHandleRundown;
	ULONG RefTraceEnabled;
	ULONG NumaAware;
	ULONG ProtectedProcess;
	ULONG DefaultPagePriority;
	ULONG PrimaryTokenFrozen;
	ULONG ProcessVerifierTarget;
	ULONG StackRandomizationDisabled;
	ULONG Flags;
	ULONG CreateReported;
	ULONG NoDebugInherit;
	ULONG ProcessExiting;
	ULONG ProcessDelete;
	ULONG Wow64SplitPages;
	ULONG VmDeleted;
	ULONG OutswapEnabled;
	ULONG Outswapped;
	ULONG ForkFailed;
	ULONG Wow64VaSpace4Gb;
	ULONG AddressSpaceInitialized;
	ULONG SetTimerResolution;
	ULONG BreakOnTermination;
	ULONG DeprioritizeViews;
	ULONG WriteWatch;
	ULONG ProcessInSession;
	ULONG OverrideAddressSpace;
	ULONG HasAddressSpace;
	ULONG LaunchPrefetched;
	ULONG InjectInpageErrors;
	ULONG VmTopDown;
	ULONG ImageNotifyDone;
	ULONG PdeUpdateNeeded;
	ULONG VdmAllowed;
	ULONG SmapAllowed;
	ULONG ProcessInserted;
	ULONG DefaultIoPriority;
	ULONG SparePsFlags1;
	LONG ExitStatus;
	WORD Spare7;
	union
	{
		struct
		{
			UCHAR SubSystemMinorVersion;
			UCHAR SubSystemMajorVersion;
		};
		WORD SubSystemVersion;
	};
	UCHAR PriorityClass;
	MM_AVL_TABLE VadRoot;
	ULONG Cookie;
	//ALPC_PROCESS_CONTEXT AlpcContext;
} EPROCESS, * PEPROCESS;

