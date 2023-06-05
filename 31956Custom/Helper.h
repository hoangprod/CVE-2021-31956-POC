#pragma once

BOOL  InitializeUndocumentedAPI();
UINT_PTR LeakEPProcess(DWORD ProcessId);
void PrintByteArray(const UCHAR* byteArray, size_t size);
