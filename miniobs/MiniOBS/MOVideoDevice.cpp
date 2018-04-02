#include "stdafx.h"
#include "MOVideoDevice.h"
#include "libtext.h"

const char* MOVideoDevice::id()
{
	return "dshow_input";
}

std::map<std::wstring, std::wstring> MOVideoDevice::avilableDevices()
{
	std::map<std::wstring, std::wstring> maps;
	obs_properties_t *properties = obs_get_source_properties(MOVideoDevice::id());
	obs_property_t *property = obs_properties_first(properties);
	while (property) {
		const char *name = obs_property_name(property);
		if (strcmp(name, "video_device_id") == 0)
		{
			size_t count = obs_property_list_item_count(property);
			for (int idx = 0; idx < count; idx++)
			{
				const char *name = obs_property_list_item_name(property, idx);
				const char *path = obs_property_list_item_string(property, idx);
				maps[libtext::UTF8_To_wstring(std::string(name))] = libtext::UTF8_To_wstring(std::string(path));
			}
		}
		obs_property_next(&property);
	}
	return maps;
}

MOVideoDevice::MOVideoDevice()
{
	obs_data_t *settings = obs_get_source_defaults(MOVideoDevice::id());
	obs_data_set_string(settings, "video_device_id", "USB2.0 HD UVC WebCam:\\\\?\\usb#22vid_04f2&pid_b3a3&mi_00#227&3124cdc1&0&0000#22{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global");
	obs_data_set_bool(settings, "active", true);
	obs_data_set_bool(settings, "deactivate_when_not_showing", true);
	obs_data_set_int(settings, "res_type", 0);
	obs_source_t *source = obs_source_create(MOVideoDevice::id(), "camera source", settings, NULL);

	this->m_source = source;
	this->m_settings = settings;
}

MOVideoDevice::~MOVideoDevice()
{
}

std::wstring MOVideoDevice::video_device_id()
{
	const char *v = obs_data_get_string(this->m_settings, "video_device_id");
	return libtext::UTF8_To_wstring(v);
}