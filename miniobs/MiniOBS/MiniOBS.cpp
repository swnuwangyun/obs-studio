#include "stdafx.h"
#include "MiniOBS.h"
#include <string>
#include <map>
#include "libtext.h"
#include "libpath.h"
#include "libfile.h"
#include "liblog.h"
#include "obs-data.h"

using namespace std;
using namespace liblog;

#define TAG	L"miniobs"

static void do_log(int log_level, const char *msg, va_list args, void *param)
{
	char str[4096];
	vsnprintf(str, 4095, msg, args);
	Log::i(L"libobs", libtext::string2wstring(str));
}

MiniOBS* MiniOBS::s_instance = NULL;

MiniOBS::MiniOBS()
{
	Log::i(TAG, L"Begin create MiniOBS instance");
	base_set_log_handler(do_log, NULL);
	Log::i(TAG, L"End create MiniOBS instance");
}

MiniOBS::~MiniOBS()
{
	Log::i(TAG, L"Begin destroy MiniOBS instance");
	obs_shutdown();
	Log::i(TAG, L"End destroy MiniOBS instance");
}

MiniOBS* MiniOBS::instance()
{
	if (s_instance == NULL)
		s_instance = new MiniOBS();
	return s_instance;
}

void MiniOBS::destroyInstance()
{
	if (s_instance)
		delete s_instance;
}

void MiniOBS::bind(HWND hwnd)
{
	Log::i(TAG, L"Begin bind");

	// Startup obs
	Log::i(TAG, L"Call obs_startup");
	if (!obs_startup("en-US", nullptr, nullptr))
		throw "Failed to startup obs";

	// Load params
	Log::i(TAG, L"Load params");
	wstring path = libpath::combine(libpath::getTempPath(), L"miniobs.ini");
	if (libfile::isFileExist(path) == false)
		throw "miniobs.ini is missing";
	map<wstring, wstring> dict = libtext::readKeyValuesFromFile(path);

	// Reset video
	Log::i(TAG, L"Call obs_reset_video");
	struct obs_video_info ovi;
	ovi.adapter = 0;
	ovi.base_width = libtext::wstringToInt(dict[L"base_width"]);
	ovi.base_height = libtext::wstringToInt(dict[L"base_height"]);;
	ovi.fps_num = libtext::wstringToInt(dict[L"fps_num"]);
	ovi.fps_den = 1;
	ovi.graphics_module = "libobs-d3d11.dll";
	ovi.output_format = VIDEO_FORMAT_NV12;
	ovi.output_width = ovi.base_width;
	ovi.output_height = ovi.base_height;
	ovi.colorspace = VIDEO_CS_601;
	ovi.range = VIDEO_RANGE_PARTIAL;
	ovi.scale_type = OBS_SCALE_BICUBIC;
	ovi.gpu_conversion = true;
	if (obs_reset_video(&ovi) != 0)
		throw "Couldn't initialize video";

	// Load all modules
	Log::i(TAG, L"Call obs_load_all_modules");
	obs_load_all_modules();

	// Config scene
#if 0
	obs_data_t *settings = obs_get_source_defaults("color_source");
	obs_data_set_int(settings, "color", 0xFFFF0000);
	obs_source_t *source = obs_source_create("color_source", "source1", settings, NULL);
#endif

#if 0
	obs_data_t *settings = obs_get_source_defaults("monitor_capture");
	obs_data_set_int(settings, "monitor", 0);
	obs_data_set_bool(settings, "capture_cursor", true);
	obs_data_set_bool(settings, "compatibility", false);
	obs_source_t *source = obs_source_create("monitor_capture", "source1", NULL, NULL);
#endif
	
#if 1
	obs_data_t *settings = obs_get_source_defaults("game_capture");

	//obs_data_set_string(settings, "capture_mode", "window");
	//std::string window = libtext::string_To_UTF8("tran.avi - VLC media player:QWidget:vlc.exe");
	//obs_data_set_string(settings, "window", window.c_str());
	//obs_data_set_bool(settings, "sli_compatibility", true);
	//obs_data_set_bool(settings, "force_scaling", false);
	//obs_data_set_string(settings, "scale_res", "1920x1080");
	//obs_data_set_bool(settings, "capture_cursor", false);
	//obs_data_set_bool(settings, "allow_transparency", false);
	//obs_data_set_bool(settings, "limit_framerate", false);
	//obs_data_set_bool(settings, "capture_overlays", false);
	//obs_data_set_bool(settings, "anti_cheat_hook", true);

	{
		string capture_mode = libtext::wstring_To_UTF8(dict[L"capture_mode"]);
		obs_data_set_string(settings, "capture_mode", capture_mode.c_str());

		string window = libtext::wstring_To_UTF8(dict[L"window"]);
		obs_data_set_string(settings, "window", window.c_str());

		enum window_priority {
			WINDOW_PRIORITY_CLASS,
			WINDOW_PRIORITY_TITLE,
			WINDOW_PRIORITY_EXE,
		};
		string prioritystr = libtext::wstring_To_UTF8(dict[L"priority"]);
		window_priority priority = WINDOW_PRIORITY_CLASS;
		if (prioritystr == "class")
			priority = WINDOW_PRIORITY_CLASS;
		else if (prioritystr == "title")
			priority = WINDOW_PRIORITY_TITLE;
		else if (prioritystr == "exe")
			priority = WINDOW_PRIORITY_EXE;
		obs_data_set_int(settings, "priority", priority);

		bool sli_compatibility = dict[L"sli_compatibility"] == L"1";
		obs_data_set_bool(settings, "sli_compatibility", sli_compatibility);

		bool force_scaling = dict[L"force_scaling"] == L"1";
		obs_data_set_bool(settings, "force_scaling", false);

		string scale_res = libtext::wstring_To_UTF8(dict[L"scale_res"]);
		obs_data_set_string(settings, "scale_res", scale_res.c_str());

		bool capture_cursor = dict[L"capture_cursor"] == L"1";
		obs_data_set_bool(settings, "capture_cursor", false);

		bool allow_transparency = dict[L"allow_transparency"] == L"1";
		obs_data_set_bool(settings, "allow_transparency", false);

		bool limit_framerate = dict[L"limit_framerate"] == L"1";
		obs_data_set_bool(settings, "limit_framerate", false);

		bool capture_overlays = dict[L"capture_overlays"] == L"1";
		obs_data_set_bool(settings, "capture_overlays", false);

		bool anti_cheat_hook = dict[L"anti_cheat_hook"] == L"1";
		obs_data_set_bool(settings, "anti_cheat_hook", true);
	}
	
	obs_source_t *source = obs_source_create("game_capture", "source1", settings, NULL);
#endif
	obs_scene_t *scene = obs_scene_create("main scene");
	obs_sceneitem_t *item = obs_scene_add(scene, source);

	struct vec2 scale;
	vec2_set(&scale, 1, 1);
	obs_sceneitem_set_scale(item, &scale);

	struct vec2 pos;
	pos.x = 0;
	pos.y = 0;
	obs_sceneitem_set_pos(item, &pos);

	obs_set_output_source(0, obs_scene_get_source(scene));

	// Create display and bind render callback
	RECT rc;
	GetClientRect(hwnd, &rc);
	gs_init_data info = {};
	info.cx = rc.right;
	info.cy = rc.bottom;
	info.format = GS_RGBA;
	info.zsformat = GS_ZS_NONE;
	info.window.hwnd = hwnd;

	m_display = obs_display_create(&info);
	obs_display_add_draw_callback(m_display, render, NULL);

	Log::i(TAG, L"End bind");
}

void MiniOBS::render(void *data, uint32_t cx, uint32_t cy)
{
	obs_render_main_texture();
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(cx);
	UNUSED_PARAMETER(cy);
}