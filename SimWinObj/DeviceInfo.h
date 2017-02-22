#pragma once
#include <vector>
#include <winioctl.h>
using namespace std;

#define CTL_QUERY_DRIVER_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //��ѯ��������
#define CTL_QUERY_DEVICE_FOR_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) //��ѯ�����ϴ������豸����
#define CTL_QUERY_DEVICE_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS) //���ݵ�ǰ�豸�����ѯ��һ���豸����
#define CTL_QUERY_ATTACHED_DEVICE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS) //�����豸�����ѯ���󶨵��豸����


typedef struct tag_DEVICE_INFO
{
	TCHAR strDeviceName[50]; //�豸����
	void *pDeviceObject; //�豸����ָ��
	void *pDrierObject;//��������ָ��
	TCHAR strAttachedDevice[50]; //������֮�ϵ��豸����
	tag_DEVICE_INFO* pAttachedDevicePointer; //������֮�ϵ��豸�����ָ��
	DWORD DevieStack;//�豸ջ�Ĵ�С
	tag_DEVICE_INFO* pNextDevice; //��һ���豸�����ָ��
}DEVICE_INFO, *PDEVICE_INFO;

typedef struct tag_DRIVER_INFO
{
	CString strDriverName; //��������
	void *pDriverPointer; //���������ָ��
	PDEVICE_INFO pDeviceObject; //�������豸�����ָ��
}DRIVER_INFO, *PDRIVER_INFO;

void GetDriverInof(LPTSTR pDriverName, void **ppDriverPointer);
void GetDeviceFromDriver(void *pDriverPointer);
void GetNextDevice(PDEVICE_INFO pDeviceObject);
void GetAttachedDevice(PDEVICE_INFO pDeviceObject);