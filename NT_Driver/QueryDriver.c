#include <ntddk.h>
#include "QueryDriver.h"

#pragma PAGEDCODE
VOID HelloDDKUnload(IN PDRIVER_OBJECT  DriverObject )
{
	PDEVICE_OBJECT	pNextObj;
	UNICODE_STRING uSymbolicName;
	RtlInitUnicodeString(&uSymbolicName, L"\\??\\QueryDriver");
	KdPrint(("Enter DriverUnload\n"));
	pNextObj = DriverObject->DeviceObject;
	
	IoDeleteSymbolicLink(&uSymbolicName);
	IoDeleteDevice(pNextObj);
}

#pragma INITCODE
NTSTATUS DriverEntry (
			IN PDRIVER_OBJECT pDriverObject,
			IN PUNICODE_STRING pRegistryPath	) 
{
	NTSTATUS status;
	int i;
	DbgPrint(("Enter DriverEntry\n"));

	//设置卸载函数
	pDriverObject->DriverUnload = HelloDDKUnload;

	//设置派遣函数
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION + 1; ++i)
		pDriverObject->MajorFunction[i] = HelloDDKDispatchRoutin;

	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HelloDDKDeviceIOControl;
	
	//创建驱动设备对象
	status = CreateDevice(pDriverObject);

	DbgPrint(("Leave DriverEntry\n"));
	return status;
}

/************************************************************************
* 函数名称:CreateDevice
* 功能描述:初始化设备对象
* 参数列表:
      pDriverObject:从I/O管理器中传进来的驱动对象
* 返回 值:返回初始化状态
*************************************************************************/
#pragma INITCODE
NTSTATUS CreateDevice (
		IN PDRIVER_OBJECT	pDriverObject) 
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;
	//创建设备名称
	UNICODE_STRING devName;
	UNICODE_STRING symLinkName;
	
	RtlInitUnicodeString(&devName,L"\\Device\\QueryDriver");
	
	//创建设备
	status = IoCreateDevice(pDriverObject,
						0,
						&devName,
						FILE_DEVICE_UNKNOWN,
						0, TRUE,
						&pDevObj);
	if (!NT_SUCCESS(status))
		return status;

	pDevObj->Flags |= DO_BUFFERED_IO;

	//创建符号链接
	RtlInitUnicodeString(&symLinkName,L"\\??\\QueryDriver");
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	return STATUS_SUCCESS;
}

#pragma PAGEDCODE
NTSTATUS HelloDDKDispatchRoutin(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	PIO_STACK_LOCATION stack;
	UCHAR type;
	NTSTATUS status;
	static char* irpname[] = 
	{
		"IRP_MJ_CREATE",
		"IRP_MJ_CREATE_NAMED_PIPE",
		"IRP_MJ_CLOSE",
		"IRP_MJ_READ",
		"IRP_MJ_WRITE",
		"IRP_MJ_QUERY_INFORMATION",
		"IRP_MJ_SET_INFORMATION",
		"IRP_MJ_QUERY_EA",
		"IRP_MJ_SET_EA",
		"IRP_MJ_FLUSH_BUFFERS",
		"IRP_MJ_QUERY_VOLUME_INFORMATION",
		"IRP_MJ_SET_VOLUME_INFORMATION",
		"IRP_MJ_DIRECTORY_CONTROL",
		"IRP_MJ_FILE_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CONTROL",
		"IRP_MJ_INTERNAL_DEVICE_CONTROL",
		"IRP_MJ_SHUTDOWN",
		"IRP_MJ_LOCK_CONTROL",
		"IRP_MJ_CLEANUP",
		"IRP_MJ_CREATE_MAILSLOT",
		"IRP_MJ_QUERY_SECURITY",
		"IRP_MJ_SET_SECURITY",
		"IRP_MJ_POWER",
		"IRP_MJ_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CHANGE",
		"IRP_MJ_QUERY_QUOTA",
		"IRP_MJ_SET_QUOTA",
		"IRP_MJ_PNP",
	};
	KdPrint(("Enter HelloDDKDispatchRoutin\n"));

	stack = IoGetCurrentIrpStackLocation(pIrp);
	//建立一个字符串数组与IRP类型对应起来
	type = stack->MajorFunction;
	if (type >= sizeof(irpname)/sizeof(char*))
		KdPrint((" - Unknown IRP, major type %X\n", type));
	else
		KdPrint(("\t%s\n", irpname[type]));

	status = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	KdPrint(("Leave HelloDDKDispatchRoutin\n"));

	return status;
}

VOID
GetDeviceObjectInfo( PDEVICE_OBJECT DevObj )
{

}

#pragma PAGEDCODE
NTSTATUS HelloDDKDeviceIOControl(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp)
{
	PIO_STACK_LOCATION stack;
	ULONG cbin;
	ULONG cbout;
	ULONG code;
	ULONG info;
	PDRIVER_OBJECT pDriverObj;
	
	NTSTATUS status = STATUS_SUCCESS;
	WCHAR* pInBuffer = NULL;
	KdPrint(("Enter HelloDDKDeviceIOControl\n"));
	
	//得到当前堆栈
	stack = IoGetCurrentIrpStackLocation(pIrp);
	//得到输入缓冲区大小
	cbin = stack->Parameters.DeviceIoControl.InputBufferLength;
	//得到输出缓冲区大小
	cbout = stack->Parameters.DeviceIoControl.OutputBufferLength;
	//得到IOCTL码
	code = stack->Parameters.DeviceIoControl.IoControlCode;
	
	info = 0;

	switch (code)
	{						// process request
		case CTL_QUERY_DRIVER_INFO: 
			{
				UNICODE_STRING DriverName;
				PDRIVER_OBJECT DriverObject;
				PDEVICE_OBJECT DeviceObject;
				pInBuffer = (WCHAR*)pIrp->AssociatedIrp.SystemBuffer;
				DbgPrint("Driver name:%ws\n", pInBuffer);
				RtlInitUnicodeString( &DriverName, pInBuffer);
				ObReferenceObjectByName( &DriverName, OBJ_CASE_INSENSITIVE, NULL, 0, ( POBJECT_TYPE )IoDriverObjectType, KernelMode, NULL, &DriverObject);
				RtlZeroMemory(pInBuffer, cbout);
				RtlCopyMemory(pInBuffer, &DriverObject, cbout);
				info = cbout;
			}
			break;
		
		case CTL_QUERY_DEVICE_FOR_DRIVER:
			{
				DEVICE_INFO DeviceInfo;
				PDEVICE_OBJECT pDeviceObj;
				PDRIVER_OBJECT pDriverObj;
				PDEVICE_INFO pOutBuffer;
				UNICODE_STRING uDeviceName;
				UNICODE_STRING uAttachedName;
				pDriverObj = *(PDRIVER_OBJECT*)(pIrp->AssociatedIrp.SystemBuffer);
				
				if(NULL == pDriverObj)
				{
					break;
				}
				pDeviceObj = pDriverObj->DeviceObject;
				if(NULL == pDeviceObj)
				{
					RtlZeroMemory(&DeviceInfo, sizeof(DEVICE_INFO));
				}else
				{
					DeviceInfo.pDeviceObject = pDeviceObj;
					DeviceInfo.pDrierObject = pDeviceObj->DriverObject;
					DeviceInfo.DevieStack = pDeviceObj->StackSize;
					//获取设备名称
					uDeviceName.Buffer = ExAllocatePoolWithTag(PagedPool, sizeof(WCHAR) * MAX_DEVICE_NAME_LENGTH, 'cinU');
					RtlZeroMemory(uDeviceName.Buffer, sizeof(WCHAR) * MAX_DEVICE_NAME_LENGTH);
					uDeviceName.MaximumLength = MAX_DEVICE_NAME_LENGTH * sizeof(WCHAR);
					GetDeviceName(pDeviceObj, &uDeviceName);
					RtlCopyMemory(DeviceInfo.strDeviceName, uDeviceName.Buffer, uDeviceName.Length + sizeof(WCHAR));
					ExFreePoolWithTag(uDeviceName.Buffer, 'cinU');
					
					//获取它绑定的设备的名称
					uAttachedName.Buffer = ExAllocatePoolWithTag(PagedPool, sizeof(WCHAR) * MAX_DEVICE_NAME_LENGTH, 'cinU');
					RtlZeroMemory(uAttachedName.Buffer, sizeof(WCHAR) * MAX_DEVICE_NAME_LENGTH);
					uAttachedName.MaximumLength = MAX_DEVICE_NAME_LENGTH * sizeof(WCHAR);
					GetDeviceName(pDeviceObj->AttachedDevice, &uAttachedName);
					RtlCopyMemory(DeviceInfo.strAttachedDevice, uDeviceName.Buffer, uDeviceName.Length + sizeof(WCHAR));
					ExFreePoolWithTag(uAttachedName.Buffer, 'cinU');
					
					DeviceInfo.pNextDevice = (PDEVICE_INFO)(pDeviceObj->NextDevice);
					DeviceInfo.pAttachedDevicePointer = (PDEVICE_INFO)(pDeviceObj->AttachedDevice);
				}
				
				pOutBuffer = pIrp->AssociatedIrp.SystemBuffer;
				RtlCopyMemory(pOutBuffer, &DeviceInfo, sizeof(DEVICE_INFO));
				info = cbout;
			}
			break;
		case CTL_QUERY_DEVICE_INFO:
			{
				PDEVICE_INFO pDeviceInfo;
				DEVICE_INFO NextDevice;
				PDEVICE_OBJECT pDeviceObj;
				UNICODE_STRING uDeviceName;
				UNICODE_STRING uAttachedName;
				PDEVICE_INFO pOutBuffer;

				pDeviceInfo = *(PDEVICE_INFO*)(pIrp->AssociatedIrp.SystemBuffer);
				pDeviceObj = ((PDEVICE_OBJECT)(pDeviceInfo->pDeviceObject))->NextDevice;
				if(NULL != pDeviceObj)
				{
					NextDevice.pDeviceObject = pDeviceObj;
					NextDevice.pDrierObject = pDeviceObj->DriverObject;
					NextDevice.DevieStack = pDeviceObj->StackSize;
					//获取设备名称
					uDeviceName.Buffer = ExAllocatePoolWithTag(PagedPool, sizeof(WCHAR) * MAX_DEVICE_NAME_LENGTH, 'cinU');
					RtlZeroMemory(uDeviceName.Buffer, sizeof(WCHAR) * MAX_DEVICE_NAME_LENGTH);
					uDeviceName.MaximumLength = MAX_DEVICE_NAME_LENGTH * sizeof(WCHAR);
					GetDeviceName(pDeviceObj, &uDeviceName);
					RtlCopyMemory(NextDevice.strDeviceName, uDeviceName.Buffer, uDeviceName.Length + sizeof(WCHAR));
					ExFreePoolWithTag(uDeviceName.Buffer, 'cinU');
					
					//获取它绑定的设备的名称
					uAttachedName.Buffer = ExAllocatePoolWithTag(PagedPool, sizeof(WCHAR) * MAX_DEVICE_NAME_LENGTH, 'cinU');
					RtlZeroMemory(uAttachedName.Buffer, sizeof(WCHAR) * MAX_DEVICE_NAME_LENGTH);
					uAttachedName.MaximumLength = MAX_DEVICE_NAME_LENGTH * sizeof(WCHAR);
					GetDeviceName(pDeviceObj->AttachedDevice, &uAttachedName);
					RtlCopyMemory(NextDevice.strAttachedDevice, uDeviceName.Buffer, uDeviceName.Length + sizeof(WCHAR));
					ExFreePoolWithTag(uAttachedName.Buffer, 'cinU');
					
					NextDevice.pNextDevice = (PDEVICE_INFO)(pDeviceObj->NextDevice);
					NextDevice.pAttachedDevicePointer = (PDEVICE_INFO)(pDeviceObj->AttachedDevice);
				}else
				{
					RtlZeroMemory(&NextDevice, sizeof(DEVICE_INFO));
				}
				
				pOutBuffer = pIrp->AssociatedIrp.SystemBuffer;
				RtlCopyMemory(pOutBuffer, &NextDevice, sizeof(DEVICE_INFO));
				info = cbout;
			}
			break;
		case CTL_QUERY_ATTACHED_DEVICE:
			{
				
			}
			break;
		default:
			status = STATUS_INVALID_VARIANT;
	}

	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = info;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	KdPrint(("Leave HelloDDKDeviceIOControl\n"));

	return status;
}

void GetDeviceName(PDEVICE_OBJECT pDeviceObj, PUNICODE_STRING pDeviceName)
{
	POBJECT_HEADER ObjectHeader;
	POBJECT_HEADER_NAME_INFO ObjectNameInfo; 

	if ( pDeviceObj == NULL )
	{
		DbgPrint( "pDeviceObj is NULL!\n" );
		return;
	}
	// 得到对象头
	ObjectHeader = OBJECT_TO_OBJECT_HEADER( pDeviceObj );

	if ( ObjectHeader )
	{
		// 查询设备名称并打印
		ObjectNameInfo = OBJECT_HEADER_TO_NAME_INFO( ObjectHeader );

		if ( ObjectNameInfo && ObjectNameInfo->Name.Buffer )
		{
			DbgPrint( "Driver Name:%wZ - Device Name:%wZ - Driver Address:0x%x - Device Address:0x%x\n",
					&pDeviceObj->DriverObject->DriverName,
					&ObjectNameInfo->Name,
					pDeviceObj->DriverObject,
					pDeviceObj );
			RtlCopyUnicodeString(pDeviceName, &ObjectNameInfo->Name);
		}

		// 对于没有名称的设备，则打印 NULL
		else if ( pDeviceObj->DriverObject )
		{
			DbgPrint( "Driver Name:%wZ - Device Name:%S - Driver Address:0x%x - Device Address:0x%x\n",
					&pDeviceObj->DriverObject->DriverName,
					L"NULL",
					pDeviceObj->DriverObject,
					pDeviceObj );
			
			pDeviceName->Length = 0;
		}
	  }
}