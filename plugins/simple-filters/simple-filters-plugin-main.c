#include <obs-module.h>

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE("simple-filters", "en-US")

extern struct obs_source_info volume_filter;

bool obs_module_load(void)
{
	obs_register_source(&volume_filter);
	return true;
}
