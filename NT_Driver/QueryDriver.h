#include <ntddk.h>
#define CTL_QUERY_DRIVER_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //�������������ѯ��һ���豸����
#define CTL_QUERY_DEVICE_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) //���ݵ�ǰ�豸�����ѯ��һ���豸����
#define CTL_QUERY_ATTACHED_DEVICE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS) //�����豸�����ѯ���󶨵��豸����

typedef struct tag_DEVICE_INFO
{
	TCHAR strDeviceName[50]; //�豸����
	void *pDeviceObject; //�豸����ָ��
	void *pDrierObject;//��������ָ��
	TCHAR strAttachedDevice[50]; //������֮�ϵ��豸����
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

