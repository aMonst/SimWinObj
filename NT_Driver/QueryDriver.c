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
				pInBuffer = (WCHAR*)(pIrp->AssociatedIrp.SystemBuffer);
				DbgPrint("Driver name:%ws\n", pInBuffer);
				pDriverObj = QueryDriverInfo;
				RtlZeroMemory(pInBuffer, cbout);
				RtlCopyMemory(pInBuffer, pDriverObj, cbout);
				info = cbout;
			}
			break;
		
		case CTL_QUERY_DEVICE_INFO:
			{	
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

PDRIVER_OBJECT QueryDriverInfo(WCHAR* pDriverName)
{
	UNICODE_STRING uDriverName;
	PDRIVER_OBJECT DriverObject;
	
	ObReferenceObjectByName(&uDriverName, OBJ_CASE_INSENSITIVE, NULL, 0, ( POBJECT_TYPE ) IoDriverObjectType, KernelMode, NULL, &DriverObject);
	DbgPrint("DriverObject: %08x", uDriverName);
	return DriverObject;
}