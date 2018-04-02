#include <util/threading.h>
#include <obs-module.h>
#include "shared-memory-camera.h"

#define do_log(level, format, ...) \
	blog(level, "[shared_memory_output: '%s'] " format, \
			obs_output_get_name(context->output), ##__VA_ARGS__)

#define warn(format, ...)  do_log(LOG_WARNING, format, ##__VA_ARGS__)
#define info(format, ...)  do_log(LOG_INFO,    format, ##__VA_ARGS__)
#define debug(format, ...) do_log(LOG_DEBUG,   format, ##__VA_ARGS__)

struct shared_memory_output {
	obs_output_t *output;
	struct camera_t *camera;
	int width;
	int height;
};

static const char *shared_memory_output_getname(void *unused)
{
	UNUSED_PARAMETER(unused);
	return "Shared Memory Output";
}

static void *shared_memory_output_create(obs_data_t *settings, obs_output_t *output)
{
	struct shared_memory_output *context = bzalloc(sizeof(*context));
	info("shared_memory_output_create");

	context->output = output;
	context->camera = shared_memory_camera_create(L"obs_shared_memory_camera");
	UNUSED_PARAMETER(settings);

	return context;
}

static void shared_memory_output_destroy(void *data)
{
	struct shared_memory_output *context = data;
	info("shared_memory_output_destroy");

	shared_memory_camera_destroy(context->camera);
	bfree(context);
}

static bool shared_memory_output_start(void *data)
{
	struct shared_memory_output *context = data;
	info("shared_memory_output_start");

	video_t *video = obs_output_video(context->output);
	const struct video_output_info *voi = video_output_get_info(video);

	context->width = (int32_t)obs_output_get_width(context->output);
	context->height = (int32_t)obs_output_get_height(context->output);

	obs_output_begin_data_capture(context->output, 0);
	
	return true;
}

static void shared_memory_output_stop(void *data, uint64_t ts)
{
	struct shared_memory_output *context = data;
	info("shared_memory_output_stop");

	obs_output_end_data_capture(context->output);
	UNUSED_PARAMETER(ts);
}

static void shared_memory_output_raw_video(void *param, struct video_data *video)
{
	struct shared_memory_output *context = param;
	UNUSED_PARAMETER(video);

	if (true) { 
		struct frame_t *frame = shared_memory_camera_lock_write_buffer(context->camera);
		if (frame) {
			static uint64_t counter = 0;
			frame->counter = counter++;
			frame->timestamp = GetTickCount64();
			frame->cx = context->width;
			frame->cy = context->height;
			frame->pitch = video->linesize[0];
			frame->color_format = 0;
			frame->spliter = 0xFFFFFFFF;

			if (video->data[0])
			{
				memcpy(frame->pixels, video->data[0], context->height * video->linesize[0]);

				if (false) {
					FILE *fp = fopen("c:\\1.rgb", "wb+");
					fwrite((const char*)frame->pixels, 1, context->height * video->linesize[0], fp);
					fclose(fp);
				}
			}
		}
		shared_memory_camera_unlock_write_buffer(context->camera);
	}
}

struct obs_output_info shared_memory_output_info = {
	.id                 = "shared_memory_output",
	.flags              = OBS_OUTPUT_VIDEO,
	.get_name           = shared_memory_output_getname,
	.create             = shared_memory_output_create,
	.destroy            = shared_memory_output_destroy,
	.start              = shared_memory_output_start,
	.stop               = shared_memory_output_stop,
	.raw_video          = shared_memory_output_raw_video
};
