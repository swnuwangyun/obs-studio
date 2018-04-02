#pragma once

#include <map>
#include "MOSource.h"




class MOVideoDevice : public MOSource
{
public:
	MOVideoDevice();
	~MOVideoDevice();

	static const char* id();
	static std::map<std::wstring, std::wstring> avilableDevices();

	std::wstring video_device_id();
};

