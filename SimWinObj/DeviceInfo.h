#pragma once
#include <vector>
#include <winioctl.h>
using namespace std;

#define CTL_QUERY_DRIVER_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //查询驱动对象
#define CTL_QUERY_DEVICE_FOR_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) //查询驱动上创建的设备对象
#define CTL_QUERY_DEVICE_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS) //根据当前设备对象查询下一个设备对象
#define CTL_QUERY_ATTACHED_DEVICE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS) //根据设备对象查询它绑定的设备对象


typedef struct tag_DEVICE_INFO
{
	TCHAR strDeviceName[50]; //设备名称
	void *pDeviceObject; //设备对象指针
	void *pDrierObject;//驱动对象指针
	TCHAR strAttachedDevice[50]; //绑定在它之上的设备名称
	tag_DEVICE_INFO* pAttachedDevicePointer; //绑定在它之上的设备对象的指针
	DWORD DevieStack;//设备栈的大小
	tag_DEVICE_INFO* pNextDevice; //下一个设备对象的指针
}DEVICE_INFO, *PDEVICE_INFO;

typedef struct tag_DRIVER_INFO
{
	CString strDriverName; //驱动名称
	void *pDriverPointer; //驱动对象的指针
	PDEVICE_INFO pDeviceObject; //驱动下设备对象的指针
}DRIVER_INFO, *PDRIVER_INFO;

void GetDriverInof(LPTSTR pDriverName, void **ppDriverPointer);
void GetDeviceFromDriver(void *pDriverPointer);
void GetNextDevice(PDEVICE_INFO pDeviceObject);
void GetAttachedDevice(PDEVICE_INFO pDeviceObject);