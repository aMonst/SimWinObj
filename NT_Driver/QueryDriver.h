#include <ntddk.h>
#define CTL_QUERY_DRIVER_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //根据驱动对象查询第一个设备对象
#define CTL_QUERY_DEVICE_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) //根据当前设备对象查询下一个设备对象
#define CTL_QUERY_ATTACHED_DEVICE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS) //根据设备对象查询它绑定的设备对象

typedef struct tag_DEVICE_INFO
{
	TCHAR strDeviceName[50]; //设备名称
	void *pDeviceObject; //设备对象指针
	void *pDrierObject;//驱动对象指针
	TCHAR strAttachedDevice[50]; //绑定在它之上的设备名称
	struct tag_DEVICE_INFO* pAttachedDevicePointer; //绑定在它之上的设备对象的指针
	ULONG DevieStack;//设备栈的大小
	struct tag_DEVICE_INFO* pNextDevice; //下一个设备对象的指针
}DEVICE_INFO, *PDEVICE_INFO;

typedef struct tag_DRIVER_INFO
{
	void *pDriverPointer; //驱动对象的指针
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

