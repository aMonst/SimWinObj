#include <ntddk.h>
#define CTL_QUERY_DRIVER_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //��ѯ��������
#define CTL_QUERY_DEVICE_FOR_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) //��ѯ�����ϴ������豸����
#define CTL_QUERY_DEVICE_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS) //���ݵ�ǰ�豸�����ѯ��һ���豸����
#define CTL_QUERY_ATTACHED_DEVICE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS) //�����豸�����ѯ���󶨵��豸����

typedef struct tag_DEVICE_INFO
{
	WCHAR strDeviceName[50]; //�豸����
	void *pDeviceObject; //�豸����ָ��
	void *pDrierObject;//��������ָ��
	WCHAR strAttachedDevice[50]; //������֮�ϵ��豸����
	struct tag_DEVICE_INFO* pAttachedDevicePointer; //������֮�ϵ��豸�����ָ��
	ULONG DevieStack;//�豸ջ�Ĵ�С
	struct tag_DEVICE_INFO* pNextDevice; //��һ���豸�����ָ��
}DEVICE_INFO, *PDEVICE_INFO;

typedef struct tag_DRIVER_INFO
{
	void *pDriverPointer; //���������ָ��
}DRIVER_INFO, *PDRIVER_INFO;

PDRIVER_OBJECT QueryDriverInfo(WCHAR* pDriverName);
NTSTATUS QueryDeviceInfo(PDEVICE_OBJECT DevObj);

NTSTATUS HelloDDKDispatchRoutin(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);
NTSTATUS HelloDDKDeviceIOControl(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
VOID HelloDDKUnload(IN PDRIVER_OBJECT  DriverObject );
NTSTATUS CreateDevice (IN PDRIVER_OBJECT	pDriverObject);

NTSTATUS
ObReferenceObjectByName(
    IN PUNICODE_STRING ObjectName,
    IN ULONG Attributes,
    IN PACCESS_STATE PassedAccessState OPTIONAL,
    IN ACCESS_MASK DesiredAccess OPTIONAL,
    IN POBJECT_TYPE ObjectType,
    IN KPROCESSOR_MODE AccessMode,
    IN OUT PVOID ParseContext OPTIONAL,
    OUT PVOID *Object
    );
	
extern POBJECT_TYPE *IoDriverObjectType;

//�豸���������Ϣ
typedef struct _OBJECT_CREATE_INFORMATION
{
    ULONG Attributes;
    HANDLE RootDirectory;
    PVOID ParseContext;
    KPROCESSOR_MODE ProbeMode;
    ULONG PagedPoolCharge;
    ULONG NonPagedPoolCharge;
    ULONG SecurityDescriptorCharge;
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    PSECURITY_QUALITY_OF_SERVICE SecurityQos;
    SECURITY_QUALITY_OF_SERVICE SecurityQualityOfService;
} OBJECT_CREATE_INFORMATION, * POBJECT_CREATE_INFORMATION;

typedef struct _OBJECT_HEADER
{
    LONG PointerCount;
    union
    {
        LONG HandleCount;
        PSINGLE_LIST_ENTRY SEntry;
    };
    POBJECT_TYPE Type;
    UCHAR NameInfoOffset;
    UCHAR HandleInfoOffset;
    UCHAR QuotaInfoOffset;
    UCHAR Flags;
    union
    {
        POBJECT_CREATE_INFORMATION ObjectCreateInfo;
        PVOID QuotaBlockCharged;
    };

    PSECURITY_DESCRIPTOR SecurityDescriptor;
    QUAD Body;
} OBJECT_HEADER, * POBJECT_HEADER;

#define NUMBER_HASH_BUCKETS 37
typedef struct _OBJECT_DIRECTORY
{
    struct _OBJECT_DIRECTORY_ENTRY* HashBuckets[NUMBER_HASH_BUCKETS];
    struct _OBJECT_DIRECTORY_ENTRY** LookupBucket;
    BOOLEAN LookupFound;
    USHORT SymbolicLinkUsageCount;
    struct _DEVICE_MAP* DeviceMap;
} OBJECT_DIRECTORY, * POBJECT_DIRECTORY;

typedef struct _OBJECT_HEADER_NAME_INFO
{
    POBJECT_DIRECTORY Directory;
    UNICODE_STRING Name;
    ULONG Reserved;
#if DBG
    ULONG Reserved2 ;
    LONG DbgDereferenceCount ;
#endif
} OBJECT_HEADER_NAME_INFO, * POBJECT_HEADER_NAME_INFO;

#define OBJECT_TO_OBJECT_HEADER( o ) \
    CONTAINING_RECORD( (o), OBJECT_HEADER, Body )

#define OBJECT_HEADER_TO_NAME_INFO( oh ) ((POBJECT_HEADER_NAME_INFO) \
    ((oh)->NameInfoOffset == 0 ? NULL : ((PCHAR)(oh) - (oh)->NameInfoOffset)))

void GetDeviceName(PDEVICE_OBJECT pDeviceObj, PUNICODE_STRING pDeviceName);

