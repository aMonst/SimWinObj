#include "StdAfx.h"
#include "DeviceInfo.h"

void GetDriverInof(LPTSTR pDriverName, void **ppDriverPointer)
{
	HANDLE hDevice = CreateFile(_T("\\\\.\\QueryDriver"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	void *pointer;
	DWORD dwRetLength;
	BOOL bRet = FALSE;
	if (INVALID_HANDLE_VALUE == hDevice)
	{
		MessageBox(NULL, _T("���豸ʧ��"), _T("����"), MB_OK);
		return;
	}
	
	bRet = DeviceIoControl(hDevice, CTL_QUERY_DRIVER_INFO, pDriverName, sizeof(TCHAR)*(_tcslen(pDriverName) + 1), &pointer, sizeof(pointer), &dwRetLength, NULL);
	if(!bRet)
	{
		MessageBox(NULL, _T("��ȡ������Ϣ����"), _T("����"), MB_OK);
		CloseHandle(hDevice);
		return;
	}
	*ppDriverPointer = pointer;
	CloseHandle(hDevice);
}

void GetDeviceFromDriver(void *pDriverPointer, PDEVICE_INFO pDeviceInfo)
{
	HANDLE hDevice = CreateFile(_T("\\\\.\\QueryDriver"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwRetLength;
	BOOL bRet = FALSE;
	if (INVALID_HANDLE_VALUE == hDevice)
	{
		MessageBox(NULL, _T("���豸ʧ��"), _T("����"), MB_OK);
		return;
	}
	
	bRet = DeviceIoControl(hDevice, CTL_QUERY_DEVICE_FOR_DRIVER, pDriverPointer, 4, pDeviceInfo, sizeof(DEVICE_INFO), &dwRetLength, NULL);
	if(!bRet)
	{
		MessageBox(NULL, _T("��ȡ������Ϣ����"), _T("����"), MB_OK);
		CloseHandle(hDevice);
		return;
	}

	CloseHandle(hDevice);
}
void GetNextDevice(PDEVICE_INFO pDeviceObject)
{
	PDEVICE_INFO pNextDevice = new DEVICE_INFO;
	pDeviceObject->pNextDevice = pNextDevice;
	
	//�������л�ȡ��һ���豸����Ϣ
	HANDLE hDevice = CreateFile(_T("\\\\.\\QueryDriver"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwRetLength;
	BOOL bRet = FALSE;
	if (INVALID_HANDLE_VALUE == hDevice)
	{
		MessageBox(NULL, _T("���豸ʧ��"), _T("����"), MB_OK);
		return;
	}

	bRet = DeviceIoControl(hDevice, CTL_QUERY_DEVICE_INFO, &pDeviceObject, 4, pNextDevice, sizeof(DEVICE_INFO), &dwRetLength, NULL);
	if(!bRet)
	{
		MessageBox(NULL, _T("��ȡ������Ϣ����"), _T("����"), MB_OK);
		CloseHandle(hDevice);
		return;
	}

	CloseHandle(hDevice);
}

void GetAttachedDevice(PDEVICE_INFO pDeviceObject)
{

}