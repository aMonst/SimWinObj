#pragma once
#include "DeviceInfo.h"
#include <vector>
using namespace std;

typedef struct _UNICODE_STRING
{
	USHORT  Length;
	USHORT  MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef ULONG NTSTATUS;
// �������Զ���  
typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length;
	HANDLE RootDirectory;
	UNICODE_STRING *ObjectName;
	ULONG Attributes;
	PSECURITY_DESCRIPTOR SecurityDescriptor;
	PSECURITY_QUALITY_OF_SERVICE SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

// ������Ϣ����  
typedef struct _DIRECTORY_BASIC_INFORMATION {
	UNICODE_STRING ObjectName;
	UNICODE_STRING ObjectTypeName;
} DIRECTORY_BASIC_INFORMATION, *PDIRECTORY_BASIC_INFORMATION;

// ����ֵ��״̬���Ͷ���  
#define OBJ_CASE_INSENSITIVE    0x00000040L  
#define DIRECTORY_QUERY            (0x0001)  
#define STATUS_SUCCESS            ((NTSTATUS)0x00000000L) // ntsubauth  
#define STATUS_MORE_ENTRIES        ((NTSTATUS)0x00000105L)  
#define STATUS_BUFFER_TOO_SMALL    ((NTSTATUS)0xC0000023L)  
// ��ʼ���������Ժ궨��  
#define InitializeObjectAttributes( p, n, a, r, s ) { \
	(p)->Length = sizeof(OBJECT_ATTRIBUTES); \
	(p)->RootDirectory = r; \
	(p)->Attributes = a; \
	(p)->ObjectName = n; \
	(p)->SecurityDescriptor = s; \
	(p)->SecurityQualityOfService = NULL; \
}
// �ַ�����ʼ��  

//�����洢�豸�����������Ƶ�����
extern vector<DRIVER_INFO> g_DriverNameList;
BOOL EnumDriver();