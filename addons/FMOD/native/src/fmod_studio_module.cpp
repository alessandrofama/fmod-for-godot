#include "fmod_studio_module.h"

using namespace godot;

FMODStudioModule* FMODStudioModule::singleton = nullptr;

void FMODStudioModule::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("init"), &FMODStudioModule::init);
	ClassDB::bind_method(D_METHOD("shutdown"), &FMODStudioModule::shutdown);
	ClassDB::bind_method(D_METHOD("get_studio_system"), &FMODStudioModule::get_studio_system_ref);

	BIND_CONSTANT(FMOD_STUDIO_INIT_NORMAL);
	BIND_CONSTANT(FMOD_STUDIO_INIT_LIVEUPDATE);
	BIND_CONSTANT(FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS);
	BIND_CONSTANT(FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE);
	BIND_CONSTANT(FMOD_STUDIO_INIT_DEFERRED_CALLBACKS);
	BIND_CONSTANT(FMOD_STUDIO_INIT_LOAD_FROM_UPDATE);
	BIND_CONSTANT(FMOD_STUDIO_INIT_MEMORY_TRACKING);

	BIND_CONSTANT(FMOD_STUDIO_PARAMETER_READONLY);
	BIND_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC);
	BIND_CONSTANT(FMOD_STUDIO_PARAMETER_GLOBAL);
	BIND_CONSTANT(FMOD_STUDIO_PARAMETER_DISCRETE);
	BIND_CONSTANT(FMOD_STUDIO_PARAMETER_LABELED);

	BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_PREUPDATE);
	BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_POSTUPDATE);
	BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_BANK_UNLOAD);
	BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_CONNECTED);
	BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_DISCONNECTED);
	BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_ALL);

	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_CREATED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_DESTROYED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_STARTING);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_STARTED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_RESTARTED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_STOPPED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_START_FAILED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_CREATED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_DESTROYED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_REAL_TO_VIRTUAL);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_VIRTUAL_TO_REAL);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_START_EVENT_COMMAND);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_NESTED_TIMELINE_BEAT);
	BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_ALL);

	BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_NORMAL);
	BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_NONBLOCKING)
	BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES)
	BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_UNENCRYPTED)

	BIND_CONSTANT(FMOD_STUDIO_COMMANDCAPTURE_NORMAL);
	BIND_CONSTANT(FMOD_STUDIO_COMMANDCAPTURE_FILEFLUSH);
	BIND_CONSTANT(FMOD_STUDIO_COMMANDCAPTURE_SKIP_INITIAL_STATE);

	BIND_CONSTANT(FMOD_STUDIO_COMMANDREPLAY_NORMAL);
	BIND_CONSTANT(FMOD_STUDIO_COMMANDREPLAY_SKIP_CLEANUP);
	BIND_CONSTANT(FMOD_STUDIO_COMMANDREPLAY_FAST_FORWARD);
	BIND_CONSTANT(FMOD_STUDIO_COMMANDREPLAY_SKIP_BANK_LOAD);

	// Enums

	BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_UNLOADING);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_UNLOADED);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_LOADING);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_LOADED);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_ERROR);

	BIND_ENUM_CONSTANT(FMOD_STUDIO_LOAD_MEMORY);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_LOAD_MEMORY_POINT);

	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_GAME_CONTROLLED);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC_DISTANCE);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC_EVENT_CONE_ANGLE);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC_EVENT_ORIENTATION);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC_DIRECTION);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC_ELEVATION);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC_LISTENER_ORIENTATION);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC_SPEED);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC_SPEED_ABSOLUTE);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_AUTOMATIC_DISTANCE_NORMALIZED);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PARAMETER_MAX);

	BIND_ENUM_CONSTANT(FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_USER_PROPERTY_TYPE_STRING);

	BIND_ENUM_CONSTANT(FMOD_STUDIO_EVENT_PROPERTY_CHANNELPRIORITY);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_EVENT_PROPERTY_SCHEDULE_DELAY);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_EVENT_PROPERTY_SCHEDULE_LOOKAHEAD);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_EVENT_PROPERTY_MINIMUM_DISTANCE);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_EVENT_PROPERTY_COOLDOWN);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_EVENT_PROPERTY_MAX);

	BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_PLAYING);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_SUSTAINING);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPED);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_STARTING);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPING);

	BIND_ENUM_CONSTANT(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_STOP_IMMEDIATE);

	BIND_ENUM_CONSTANT(FMOD_STUDIO_INSTANCETYPE_NONE);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_INSTANCETYPE_SYSTEM);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_INSTANCETYPE_EVENTDESCRIPTION);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_INSTANCETYPE_EVENTINSTANCE);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_INSTANCETYPE_PARAMETERINSTANCE);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_INSTANCETYPE_BUS);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_INSTANCETYPE_VCA);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_INSTANCETYPE_BANK);
	BIND_ENUM_CONSTANT(FMOD_STUDIO_INSTANCETYPE_COMMANDREPLAY);
}

bool FMODStudioModule::initialize_fmod()
{
	if (!ERROR_CHECK(FMOD::Studio::System::create(&studio_system)))
	{
		return false;
	}

	FMOD::System* core_system;

	if (!ERROR_CHECK(studio_system->getCoreSystem(&core_system)))
	{
		return false;
	}

	if (!ERROR_CHECK(core_system->setFileSystem(&FMODGodotBlockingIO::file_open, &FMODGodotBlockingIO::file_close,
				&FMODGodotBlockingIO::file_read, &FMODGodotBlockingIO::file_seek, 0, 0,
				-1)))
	{
		return false;
	}

	const String settings_path = get_platform_setting_path(PlatformSettingsPath::FMOD_SETTINGS_PATH);

	unsigned int ouput_type =
			static_cast<unsigned int>(get_platform_project_setting(settings_path + String("fmod_output_type")));

	if (!ERROR_CHECK(core_system->setOutput(static_cast<FMOD_OUTPUTTYPE>(ouput_type))))
	{
		String warning_message = "[WARNING]: " + String("Failed to set the selected Output Type. Initializing FMOD with OUTPUTTYPE_NOSOUND.") + " " + __FUNCTION__ + " in " + __FILE__ +
				":" + String::num(__LINE__);
		UtilityFunctions::push_warning(warning_message);
		core_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	}

	const unsigned int sample_rate =
			static_cast<unsigned int>(get_platform_project_setting(settings_path + String("sample_rate")));
	const unsigned int speaker_mode =
			static_cast<unsigned int>(get_platform_project_setting(settings_path + String("speaker_mode")));

	if (!ERROR_CHECK(core_system->setSoftwareFormat(sample_rate, static_cast<FMOD_SPEAKERMODE>(speaker_mode), 0)))
	{
		return false;
	}

	const unsigned int buffer_length =
			static_cast<unsigned int>(get_platform_project_setting(settings_path + String("dsp_buffer_length")));
	const int num_buffers = static_cast<int>(get_platform_project_setting(settings_path + String("dsp_buffer_count")));

	if (buffer_length > 0 && num_buffers > 0)
	{
		core_system->setDSPBufferSize(buffer_length, num_buffers);
	}

	FMOD_ADVANCEDSETTINGS fmod_advanced_settings{};
	fmod_advanced_settings.cbSize = sizeof(FMOD_ADVANCEDSETTINGS);

	fmod_advanced_settings.randomSeed = static_cast<unsigned int>(clock());

	const int real_channels =
			static_cast<int>(get_platform_project_setting(settings_path + String("real_channel_count")));

	if (OS::get_singleton()->has_feature("editor") || OS::get_singleton()->has_feature("standalone"))
	{
		fmod_advanced_settings.maxVorbisCodecs = real_channels;
		fmod_advanced_settings.maxFADPCMCodecs = real_channels;
	}

	if (!ERROR_CHECK(core_system->setSoftwareChannels(real_channels)))
	{
		return false;
	}

	FMOD_STUDIO_INITFLAGS studio_init_flags = FMOD_STUDIO_INIT_NORMAL;

	const bool enable_live_update =
			static_cast<bool>(get_platform_project_setting(settings_path + String("enable_live_update")));
	const unsigned short profile_port =
			static_cast<int>(get_platform_project_setting(settings_path + String("live_update_port")));

	if (enable_live_update)
	{
		studio_init_flags |= FMOD_STUDIO_INIT_LIVEUPDATE;
		fmod_advanced_settings.profilePort = profile_port;
	}

	if (!ERROR_CHECK(core_system->setAdvancedSettings(&fmod_advanced_settings)))
	{
		return false;
	}

	const bool enable_memory_tracking =
			static_cast<bool>(get_platform_project_setting(settings_path + String("enable_memory_tracking")));

	if (enable_memory_tracking)
	{
		studio_init_flags |= FMOD_STUDIO_INIT_MEMORY_TRACKING;
	}

	const String encryption_key =
			static_cast<String>(get_platform_project_setting(settings_path + String("encryption_key")));

	if (!encryption_key.is_empty())
	{
		FMOD_STUDIO_ADVANCEDSETTINGS fmod_studio_advanced_settings{};
		fmod_studio_advanced_settings.cbsize = sizeof(FMOD_STUDIO_ADVANCEDSETTINGS);

		fmod_studio_advanced_settings.encryptionkey = encryption_key.utf8().get_data();

		if (!ERROR_CHECK(studio_system->setAdvancedSettings(&fmod_studio_advanced_settings)))
		{
			return false;
		}
	}

	const int virtual_channels =
			static_cast<int>(get_platform_project_setting(settings_path + String("virtual_channel_count")));

	FMOD_INITFLAGS core_init_flags = FMOD_INIT_NORMAL;

	bool init_with_3d_righthanded = static_cast<bool>(get_platform_project_setting(settings_path + String("init_with_3d_righthanded")));

	if (init_with_3d_righthanded)
	{
		core_init_flags |= FMOD_INIT_3D_RIGHTHANDED;
	}

	distance_scale_2d = static_cast<float>(get_platform_project_setting(settings_path + String("distance_scale_2d")));

	if (!ERROR_CHECK(studio_system->initialize(virtual_channels, studio_init_flags, core_init_flags, nullptr)))
	{
		return false;
	}

	studio_system_ref = create_ref<StudioApi::StudioSystem>();
	studio_system_ref->set_instance(studio_system);

#ifdef FMOD_OSX
	AudioUnit audio_unit;
	core_system->getOutputHandle((void**)&audio_unit);
	AudioDeviceID device_id;
	UInt32 audio_device_id_size = sizeof(device_id);
	AudioUnitGetProperty(audio_unit, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global, 0, &device_id,
			&audio_device_id_size);

	// note(alex): deprecated, but still used in the FMOD docs. Is there an alternative to this?
	UInt32 buffer_frame_size = static_cast<UInt32>(buffer_length);
	AudioDeviceSetProperty(device_id, NULL, 0, FALSE, kAudioDevicePropertyBufferFrameSize, sizeof(buffer_frame_size),
			&buffer_frame_size);
#endif

	return true;
}

void FMODStudioModule::initialize_debug_functions()
{
	const String settings_path = get_platform_setting_path(PlatformSettingsPath::FMOD_SETTINGS_PATH);

	unsigned int fmod_debug_flags =
			static_cast<unsigned int>(get_platform_project_setting(settings_path + String("fmod_debug_flags")));
	unsigned int fmod_debug_mode =
			static_cast<unsigned int>(get_platform_project_setting(settings_path + String("fmod_debug_mode")));

	ERROR_CHECK(FMOD::Debug_Initialize((FMOD_DEBUG_FLAGS)fmod_debug_flags, (FMOD_DEBUG_MODE)fmod_debug_mode,
			(FMOD_DEBUG_CALLBACK)debug_monitor, "fmod.log"));
}

bool FMODStudioModule::shutdown_fmod()
{
	if (studio_system->isValid())
	{
		ERROR_CHECK(studio_system->unloadAll());
		ERROR_CHECK(studio_system->update());

		if (ERROR_CHECK(studio_system->release()))
		{
			return true;
		}
	}

	return false;
}

FMODStudioModule* FMODStudioModule::get_singleton()
{
	return singleton;
}

FMODStudioModule::FMODStudioModule()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;

#if defined(_DEBUG)
	initialize_debug_functions();
#endif

	ERROR_CHECK(FMOD::Memory_Initialize(nullptr, 0, FMODGodotMemory::user_alloc, FMODGodotMemory::user_realloc,
			FMODGodotMemory::user_free, FMOD_MEMORY_ALL));
}

FMODStudioModule::~FMODStudioModule()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

bool FMODStudioModule::init()
{
	bool result = initialize_fmod();

	if (!result)
	{
		ERROR_CHECK(FMOD_ERR_INITIALIZATION);
		return result;
	}

	String message = "[FMOD] Initialized Runtime System";
	UtilityFunctions::print(message);

	result = true;

	const String settings_path = get_platform_setting_path(PlatformSettingsPath::FMOD_SETTINGS_PATH);
	TypedArray<String> banks_to_load_at_startup = get_platform_project_setting(settings_path + String("banks_to_load_at_startup"));

	for (int64_t i = 0; i < banks_to_load_at_startup.size(); i++)
	{
		String bank_path = banks_to_load_at_startup[i];
		studio_system_ref->load_bank_file(bank_path.utf8().get_data(), FMOD_STUDIO_LOAD_BANK_NORMAL, false);
	}

	return result;
}

void FMODStudioModule::shutdown()
{
	if (shutdown_fmod())
	{
		String message = "[FMOD] Shut down Runtime System";
		UtilityFunctions::print(message);
	}
}

Ref<StudioApi::StudioSystem> FMODStudioModule::get_studio_system_ref()
{
	return studio_system_ref;
}

FMOD::Studio::System* FMODStudioModule::get_studio_system() const
{
	return studio_system;
}

FMOD::System* FMODStudioModule::get_core_system() const
{
	FMOD::System* core_system;
	studio_system->getCoreSystem(&core_system);
	return core_system;
}