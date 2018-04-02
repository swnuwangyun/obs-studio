#pragma once

#include <string>
#include <Windows.h>

template<class T>
class SharedMemoryReader
{
public:
	SharedMemoryReader(std::wstring name)
	{
		this->m_name = name;
		this->m_data = NULL;
		this->m_fileMappingHandle = NULL;
		this->m_mutexHandle = NULL;
		this->m_readyEventHandle = NULL;
	}

	~SharedMemoryReader(void)
	{
		if (this->m_readyEventHandle)
			CloseHandle(this->m_readyEventHandle);

		if (this->m_mutexHandle)
			CloseHandle(this->m_mutexHandle);

		if (this->m_data)
			UnmapViewOfFile(this->m_data);

		if (this->m_fileMappingHandle)
			CloseHandle(this->m_fileMappingHandle);
	}

	T* lock()
	{
		if(this->m_fileMappingHandle==NULL)
		{
			this->m_fileMappingHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, this->m_name.c_str());
			if (this->m_fileMappingHandle)
				this->m_data = (T*)MapViewOfFile(this->m_fileMappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		}

		if(this->m_mutexHandle==NULL)
		{
			this->m_mutexHandle = OpenMutex(MUTEX_ALL_ACCESS, FALSE, (this->m_name+L"_mutex").c_str());
			ReleaseMutex(this->m_mutexHandle);
		}

		if(this->m_readyEventHandle==NULL)
		{
			this->m_readyEventHandle = OpenEvent(EVENT_ALL_ACCESS, FALSE, (this->m_name+L"_ready_event").c_str());
		}

		if(this->m_readyEventHandle)
			if(WaitForSingleObject(this->m_readyEventHandle, 1)!=WAIT_OBJECT_0)
				return NULL;

		if(this->m_mutexHandle)
			WaitForSingleObject(this->m_mutexHandle, INFINITE);

		return this->m_data;
	}

	void unlock()
	{
		if(this->m_mutexHandle)
			ReleaseMutex(this->m_mutexHandle);
	}

private:
	std::wstring m_name;
	T *m_data;
	HANDLE m_fileMappingHandle;
	HANDLE m_mutexHandle;
	HANDLE m_readyEventHandle;
};