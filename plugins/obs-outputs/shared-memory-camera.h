#pragma once

#include <stdio.h>
#include <Windows.h>

#define MAX_FRAME_SIZE	(3840*3840*4 + 1024*4)

struct frame_t{
	unsigned long long counter;
	unsigned long long timestamp;
	unsigned int cx;
	unsigned int cy;
	unsigned int pitch;
	unsigned int color_format;
	unsigned int spliter;
	unsigned char pixels[MAX_FRAME_SIZE];
};

struct camera_t{
	wchar_t *camera_name;
	HANDLE file_mapping_handle;
	HANDLE mutex_handle;
	HANDLE ready_event_handle;
	struct frame_t *frame;
};

extern struct camera_t* shared_memory_camera_create(wchar_t* camera_name);

extern struct frame_t* shared_memory_camera_lock_write_buffer(struct camera_t *camera);

extern void shared_memory_camera_unlock_write_buffer(struct camera_t *camera);

extern void shared_memory_camera_destroy(struct camera_t *camera);