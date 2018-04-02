#pragma once

#include <string>
#include "obs.h"

class MOSource
{
public:
	MOSource();
	~MOSource();

protected:
	obs_source_t *m_source;
	obs_data_t *m_settings;
};

