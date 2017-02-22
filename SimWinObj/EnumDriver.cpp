#include "stdafx.h"
#include "EnumDriver.h"
vector<DRIVER_INFO> g_DriverNameList;
typedef VOID(CALLBACK* RTLINITUNICODESTRING)(PUNICODE_STRING, PCWSTR);

RTLINITUNICODESTRING RtlInitUnicodeString;
// 打开对象  
typedef NTSTATUS(WINAPI *ZWOPENDIRECTORYOBJECT)(
	OUT PHANDLE DirectoryHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
	);
ZWOPENDIRECTORYOBJECT ZwOpenDirectoryObject;
// 查询对象  
typedef
NTSTATUS
(WINAPI *ZWQUERYDIRECTORYOBJECT)(
								 IN HANDLE DirectoryHandle,
								 OUT PVOID Buffer,
								 IN ULONG BufferLength,
								 IN BOOLEAN ReturnSingleEntry,
								 IN BOOLEAN RestartScan,
								 IN OUT PULONG Context,
								 OUT PULONG ReturnLength OPTIONAL
								 );
ZWQUERYDIRECTORYOBJECT ZwQueryDirectoryObject;
// 关闭已经打开的对象  
typedef NTSTATUS (WINAPI *ZWCLOSE)(IN HANDLE Handle);
ZWCLOSE ZwClose;

BOOL EnumDriver()
{
	HMODULE hNtdll = NULL;
	UNICODE_STRING     strDirName;
	OBJECT_ATTRIBUTES  oba;
	NTSTATUS           ntStatus;
	HANDLE             hDirectory;
	CString strDriverName;
	hNtdll = LoadLibrary(_T("ntdll.dll"));
	if (NULL == hNtdll)
	{
		return FALSE;
	}

	RtlInitUnicodeString = (RTLINITUNICODESTRING)GetProcAddress(hNtdll, "RtlInitUnicodeString");
	ZwOpenDirectoryObject = (ZWOPENDIRECTORYOBJECT)GetProcAddress(hNtdll, "ZwOpenDirectoryObject");
	ZwQueryDirectoryObject = (ZWQUERYDIRECTORYOBJECT)GetProcAddress(hNtdll, "ZwQueryDirectoryObject");
	ZwClose = (ZWCLOSE)GetProcAddress(hNtdll, "ZwClose");

	RtlInitUnicodeString(&strDirName, _T("\\Driver"));
	InitializeObjectAttributes(&oba, &strDirName, OBJ_CASE_INSENSITIVE, NULL, NULL);
	ntStatus = ZwOpenDirectoryObject(&hDirectory, DIRECTORY_QUERY, &oba);
	if (ntStatus != STATUS_SUCCESS)
	{
		return FALSE;
	}

	PDIRECTORY_BASIC_INFORMATION   pBuffer = NULL;
	PDIRECTORY_BASIC_INFORMATION   pBuffer2 = NULL;
	ULONG    ulLength = 0x800;    // 2048  
	ULONG    ulContext = 0;
	ULONG    ulRet = 0;
	// 查询目录对象  
	do
	{
		if (pBuffer != NULL)
		{
			free(pBuffer);
		}
		ulLength = ulLength * 2;
		pBuffer = (PDIRECTORY_BASIC_INFORMATION)malloc(ulLength);
		if (NULL == pBuffer)
		{
			if (pBuffer != NULL)
			{
				free(pBuffer);
			}
			if (hDirectory != NULL)
			{
				ZwClose(hDirectory);
			}

			return FALSE;
		}
		ntStatus = ZwQueryDirectoryObject(hDirectory, pBuffer, ulLength, FALSE, TRUE, &ulContext, &ulRet);
	} while (ntStatus == STATUS_MORE_ENTRIES || ntStatus == STATUS_BUFFER_TOO_SMALL);

	if (STATUS_SUCCESS == ntStatus)
	{
		pBuffer2 = pBuffer;
		while ((pBuffer2->ObjectName.Length != 0) && (pBuffer2->ObjectTypeName.Length != 0))
		{
			DRIVER_INFO DriverInfo;
			DriverInfo.strDriverName = pBuffer2->ObjectName.Buffer;
			g_DriverNameList.push_back(DriverInfo);
			pBuffer2++;
		}
	}

	if (pBuffer != NULL)
	{
		free(pBuffer);
	}
	if (hDirectory != NULL)
	{
		ZwClose(hDirectory);
	}

	return TRUE;
}