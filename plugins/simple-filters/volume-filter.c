#include <obs-module.h>
#include <math.h>
#include <Windows.h>

struct volume_data {
	obs_source_t *context;
	size_t channels;
	float factor;
};

static const char *volume_name(void *unused)
{
	OutputDebugString(L"====================================================volume_name\r\n");
	UNUSED_PARAMETER(unused);
	return obs_module_text("Volume Control");
}

static void volume_destroy(void *data)
{
	OutputDebugString(L"====================================================volume_destroy\r\n");
	struct volume_data *gf = data;
	bfree(gf);
}

static void volume_update(void *data, obs_data_t *s)
{
	OutputDebugString(L"====================================================volume_update\r\n");
	struct volume_data *gf = data;
	double val = obs_data_get_double(s, "volume");
	gf->channels = audio_output_get_channels(obs_get_audio());
	gf->factor = (float)val;
}

static void *volume_create(obs_data_t *settings, obs_source_t *filter)
{
	OutputDebugString(L"====================================================volume_create\r\n");
	struct volume_data *gf = bzalloc(sizeof(*gf));
	gf->context = filter;
	volume_update(gf, settings);
	return gf;
}

static struct obs_audio_data *volume_filter_audio(void *data, struct obs_audio_data *audio)
{
	struct volume_data *gf = data;
	const size_t channels = gf->channels;
	float **adata = (float**)audio->data;
	const float factor = gf->factor;

	for (size_t c = 0; c < channels; c++) {
		if (audio->data[c]) {
			for (size_t i = 0; i < audio->frames; i++) {
				adata[c][i] *= factor;
			}
		}
	}

	return audio;
}

static void volume_defaults(obs_data_t *s)
{
	OutputDebugString(L"====================================================volume_defaults\r\n");
	obs_data_set_default_double(s, "volume", 1.0f);
}

static obs_properties_t *volume_properties(void *data)
{
	OutputDebugString(L"====================================================volume_properties\r\n");
	obs_properties_t *ppts = obs_properties_create();

	obs_properties_add_float_slider(ppts, "volume", "Volume", 0, 1.0, 0.1);

	UNUSED_PARAMETER(data);
	return ppts;
}

struct obs_source_info volume_filter = {
	.id = "volume_filter",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_AUDIO,
	.get_name = volume_name,
	.create = volume_create,
	.destroy = volume_destroy,
	.update = volume_update,
	.filter_audio = volume_filter_audio,
	.get_defaults = volume_defaults,
	.get_properties = volume_properties,
};