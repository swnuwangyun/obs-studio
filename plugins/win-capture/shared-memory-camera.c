#include "shared-memory-camera.h"

struct camera_t*  shared_memory_camera_create(wchar_t* camera_name)
{
	struct camera_t *camera = (struct camera_t*)malloc(sizeof(struct camera_t));

	if (camera)
	{
		memset(camera, 0, sizeof(struct camera_t));
		camera->camera_name = camera_name;
	}

	return camera;
}

struct frame_t* shared_memory_camera_lock_write_buffer(struct camera_t* camera)
{
	if (camera->file_mapping_handle==0)
		camera->file_mapping_handle = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, sizeof(struct frame_t), camera->camera_name);

	if (camera->frame==NULL)
		camera->frame = (struct frame_t*)MapViewOfFile(camera->file_mapping_handle, FILE_MAP_WRITE, 0, 0, 0);

	if (camera->mutex_handle == 0)
	{
		wchar_t name[1024];
		wsprintf(name, L"%s_mutex", camera->camera_name);
		camera->mutex_handle = CreateMutex(NULL, FALSE, name);
	}

	if (camera->ready_event_handle == 0)
	{
		wchar_t name[1024];
		wsprintf(name, L"%s_ready_event", camera->camera_name);
		camera->ready_event_handle = CreateEvent(NULL, FALSE, FALSE, name);
	}

	if (camera->mutex_handle)
		WaitForSingleObject(camera->mutex_handle, INFINITE);

	return camera->frame;
}

void shared_memory_camera_unlock_write_buffer(struct camera_t* camera)
{
	if (camera->mutex_handle)
		ReleaseMutex(camera->mutex_handle);

	if (camera->ready_event_handle)
		SetEvent(camera->ready_event_handle);
}

void shared_memory_camera_destroy(struct camera_t* camera)
{
	if (camera->mutex_handle)
		CloseHandle(camera->mutex_handle);

	if (camera->frame)
		UnmapViewOfFile(camera->frame);

	if (camera->file_mapping_handle)
		CloseHandle(camera->file_mapping_handle);
}