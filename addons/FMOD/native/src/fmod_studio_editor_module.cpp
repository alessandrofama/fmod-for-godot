#include "fmod_studio_editor_module.h"

using namespace godot;

FMODStudioEditorModule* FMODStudioEditorModule::singleton = nullptr;

void FMODSettings::_bind_methods()
{
}

FMODSettings::FMODSettings()
{
	add_fmod_settings();

	if (Engine::get_singleton()->is_editor_hint())
	{
		Error error = ProjectSettings::get_singleton()->save();
		if (error)
		{
			UtilityFunctions::push_error(
					String("Encountered error {0} when saving FMOD Project Settings").format(Array::make(error)));
		}
		else
		{
			UtilityFunctions::print("[FMOD] Saved Project Settings");
		}
	}
}

void FMODSettings::add_fmod_settings()
{
	const String settings_path = get_platform_setting_path(PlatformSettingsPath::FMOD_SETTINGS_PATH);

	add_setting(settings_path + String("banks_path"), "res://banks/Desktop", Variant::Type::STRING, PROPERTY_HINT_DIR);
	add_setting(settings_path + String("fmod_output_type"), 0, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"AUTODETECT, UNKNOWN, NOSOUND, WAVWRITER, NOSOUND_NRT, WAVWRITER_NRT, WASAPI, ASIO, PULSEAUDIO, ALSA, "
			"COREAUDIO, AUDIOTRACK, OPENSL, AUDIOOUT, AUDIO3D, WEBAUDIO, NNAUDIO, WINSONIC, AAUDIO");
	add_setting(settings_path + String("fmod_debug_flags"), 0, Variant::Type::INT, PROPERTY_HINT_FLAGS,
			"Error,Warning,Log");
	add_setting(settings_path + String("fmod_debug_mode"), 1, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"TTY,File,Godot Output");
	add_setting(settings_path + String("banks_to_load_at_startup"), TypedArray<String>(), Variant::Type::ARRAY, PROPERTY_HINT_ARRAY_TYPE,
			String("{0}/{1}:").format(Array::make(Variant::Type::STRING, PROPERTY_HINT_FILE)));
	add_setting(settings_path + String("banks_preload_samples"), false, Variant::Type::BOOL);
	add_setting(settings_path + String("banks_flags"), 0, Variant::Type::INT, PROPERTY_HINT_FLAGS,
			"Non Blocking,Decompress Samples,Unencrypted");
	add_setting(settings_path + String("sample_rate"), 48000, Variant::Type::INT);
	add_setting(settings_path + String("speaker_mode"), 3, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"Default,Raw,Mono,Stereo,Quad,Surround,5.1,7.1,7.1.4");
	add_setting(settings_path + String("dsp_buffer_length"), 512, Variant::Type::INT);
	add_setting(settings_path + String("dsp_buffer_count"), 4, Variant::Type::INT);
	add_setting(settings_path + String("real_channel_count"), 32, Variant::Type::INT);
	add_setting(settings_path + String("virtual_channel_count"), 128, Variant::Type::INT);
	add_setting(settings_path + String("enable_live_update"), true, Variant::Type::BOOL);
	add_setting(settings_path + String("live_update_port"), 9264, Variant::Type::INT);
	add_setting(settings_path + String("enable_memory_tracking"), false, Variant::Type::BOOL);
	add_setting(settings_path + String("encryption_key"), "", Variant::Type::STRING);
	add_setting(settings_path + String("distance_scale_2d"), 1.0f, Variant::Type::FLOAT);
	add_setting(settings_path + String("init_with_3d_righthanded"), true, Variant::Type::BOOL);
}

void FMODSettings::add_setting(const String& name, const Variant& default_value, Variant::Type type, PropertyHint hint,
		const String& hint_string, int usage)
{
	Dictionary setting;
	setting["name"] = name;
	setting["type"] = type;
	setting["hint"] = hint;
	setting["hint_string"] = hint_string;
	setting["usage"] = usage;

	if (ProjectSettings::get_singleton()->has_setting(name))
	{
		ProjectSettings::get_singleton()->add_property_info(setting);
		return;
	}

	ProjectSettings::get_singleton()->set_setting(name, default_value);
	ProjectSettings::get_singleton()->add_property_info(setting);
}

void FMODStudioEditorModule::_bind_methods()
{
	ClassDB::bind_static_method("FMODStudioEditorModule", D_METHOD("get_singleton"),
			&FMODStudioEditorModule::get_singleton);
	ClassDB::bind_method(D_METHOD("init"), &FMODStudioEditorModule::init);
	ClassDB::bind_method(D_METHOD("shutdown"), &FMODStudioEditorModule::shutdown);

	ClassDB::bind_method(D_METHOD("create_icon", "icon_path", "icon_type"), &FMODStudioEditorModule::create_icon);
	ClassDB::bind_method(D_METHOD("get_icon", "icon_type"), &FMODStudioEditorModule::get_icon);
	ClassDB::bind_method(D_METHOD("generate_cache", "project_info"), &FMODStudioEditorModule::generate_cache);
	ClassDB::bind_method(D_METHOD("get_project_info_from_banks"), &FMODStudioEditorModule::get_project_info_from_banks);
	ClassDB::bind_method(D_METHOD("create_tree", "list"), &FMODStudioEditorModule::create_tree);
	ClassDB::bind_method(D_METHOD("sort_items_by_path", "a", "b"), &FMODStudioEditorModule::sort_items_by_path);
	ClassDB::bind_method(D_METHOD("sort_parameters_by_name", "a", "b"), &FMODStudioEditorModule::sort_parameters_by_name);
	ClassDB::bind_method(D_METHOD("create_tree_items", "root", "items", "parent"),
			&FMODStudioEditorModule::create_tree_items);
	ClassDB::bind_method(D_METHOD("play_event", "guid"), &FMODStudioEditorModule::play_event);
	ClassDB::bind_method(D_METHOD("stop_events", "allow_fadeout"), &FMODStudioEditorModule::stop_events);
	ClassDB::bind_method(D_METHOD("load_all_banks"), &FMODStudioEditorModule::load_all_banks);
	ClassDB::bind_method(D_METHOD("set_preview_parameter", "parameter_name", "value"),
			&FMODStudioEditorModule::set_preview_parameter);

	ClassDB::bind_method(D_METHOD("set_is_initialized", "initialized"), &FMODStudioEditorModule::set_is_initialized);
	ClassDB::bind_method(D_METHOD("get_is_initialized"), &FMODStudioEditorModule::get_is_initialized);
	ClassDB::bind_method(D_METHOD("poll_banks_loading_state", "timer"), &FMODStudioEditorModule::poll_banks_loading_state);
	ClassDB::bind_method(D_METHOD("have_all_banks_loaded"), &FMODStudioEditorModule::get_all_banks_loaded);

	ADD_SIGNAL(MethodInfo("banks_loading"));
	ADD_SIGNAL(MethodInfo("banks_loaded"));
	ADD_SIGNAL(MethodInfo("cache_generated"));

	BIND_ENUM_CONSTANT(FMOD_ICONTYPE_PROJECT);
	BIND_ENUM_CONSTANT(FMOD_ICONTYPE_FOLDER_CLOSED);
	BIND_ENUM_CONSTANT(FMOD_ICONTYPE_FOLDER_OPENED);
	BIND_ENUM_CONSTANT(FMOD_ICONTYPE_EVENT);
	BIND_ENUM_CONSTANT(FMOD_ICONTYPE_BANK);
	BIND_ENUM_CONSTANT(FMOD_ICONTYPE_SNAPSHOT);
	BIND_ENUM_CONSTANT(FMOD_ICONTYPE_BUS);
	BIND_ENUM_CONSTANT(FMOD_ICONTYPE_VCA);
	BIND_ENUM_CONSTANT(FMOD_ICONTYPE_C_PARAMETER);

	BIND_ENUM_CONSTANT(FMOD_ASSETTYPE_NONE);
	BIND_ENUM_CONSTANT(FMOD_ASSETTYPE_EVENT);
	BIND_ENUM_CONSTANT(FMOD_ASSETTYPE_SNAPSHOT);
	BIND_ENUM_CONSTANT(FMOD_ASSETTYPE_BANK);
	BIND_ENUM_CONSTANT(FMOD_ASSETTYPE_BUS);
	BIND_ENUM_CONSTANT(FMOD_ASSETTYPE_VCA);
	BIND_ENUM_CONSTANT(FMOD_ASSETTYPE_GLOBAL_PARAMETER);
	BIND_ENUM_CONSTANT(FMOD_ASSETTYPE_FOLDER);
}

bool FMODStudioEditorModule::initialize_fmod()
{
	const String settings_path = get_platform_setting_path(PlatformSettingsPath::FMOD_SETTINGS_PATH);

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

	if (!ERROR_CHECK(core_system->setOutput(FMOD_OUTPUTTYPE::FMOD_OUTPUTTYPE_AUTODETECT)))
	{
		return false;
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

	if (!ERROR_CHECK(studio_system->initialize(virtual_channels, studio_init_flags, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr)))
	{
		return false;
	}

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

bool FMODStudioEditorModule::shutdown_fmod()
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

Dictionary FMODStudioEditorModule::get_event_list(const FMOD::Studio::Bank* bank, int count) const
{
	auto list = std::vector<FMOD::Studio::EventDescription*>(static_cast<size_t>(count));
	Array events;
	Array snapshots;

	if (ERROR_CHECK(bank->getEventList(list.data(), count, nullptr)))
	{
		for (const auto& item : list)
		{
			if (item != nullptr)
			{
				bool is_snapshot = false;
				item->isSnapshot(&is_snapshot);

				if (is_snapshot)
				{
					Ref<EventAsset> event = create_ref<EventAsset>();
					event->set_event_ref(item);
					snapshots.append(event);
				}
				else
				{
					Ref<EventAsset> event = create_ref<EventAsset>();
					event->set_event_ref(item);
					events.append(event);
				}
			}
		}
	}

	Dictionary result;
	result["events"] = events;
	result["snapshots"] = snapshots;

	return result;
}

Array FMODStudioEditorModule::get_bus_list(const FMOD::Studio::Bank* bank, int count) const
{
	auto list = std::vector<FMOD::Studio::Bus*>(static_cast<size_t>(count));
	Array busses;

	if (ERROR_CHECK(bank->getBusList(list.data(), count, nullptr)))
	{
		for (const auto& item : list)
		{
			if (item != nullptr)
			{
				Ref<BusAsset> bus = create_ref<BusAsset>();
				bus->set_bus_ref(item);

				busses.append(bus);
			}
		}
	}

	return busses;
}

Array FMODStudioEditorModule::get_vca_list(const FMOD::Studio::Bank* bank, int count) const
{
	auto list = std::vector<FMOD::Studio::VCA*>(static_cast<size_t>(count));
	Array vcas;

	if (ERROR_CHECK(bank->getVCAList(list.data(), count, nullptr)))
	{
		for (const auto& item : list)
		{
			if (item != nullptr)
			{
				Ref<VCAAsset> vca = create_ref<VCAAsset>();
				vca->set_vca_ref(item);

				vcas.append(vca);
			}
		}
	}

	return vcas;
}

Array FMODStudioEditorModule::get_global_parameter_list(int count) const
{
	auto list = std::vector<FMOD_STUDIO_PARAMETER_DESCRIPTION>(static_cast<size_t>(count));
	Array parameters;

	if (ERROR_CHECK(get_singleton()->studio_system->getParameterDescriptionList(list.data(), count, &count)))
	{
		for (const auto& parameter_description : list)
		{
			if (parameter_description.name != nullptr)
			{
				if (parameter_description.flags & FMOD_STUDIO_PARAMETER_GLOBAL)
				{
					Ref<ParameterAsset> parameter = create_ref<ParameterAsset>();
					parameter->set_parameter_ref(parameter_description);

					parameters.append(parameter);
				}
			}
		}
	}

	return parameters;
}

Array FMODStudioEditorModule::get_bank_file_infos(const String& bank_path) const
{
	Array files;

	Ref<DirAccess> dir = DirAccess::open(bank_path);

	if (DirAccess::get_open_error() == godot::Error::OK)
	{
		dir->list_dir_begin();
		String file_name = dir->get_next();

		while (file_name != "")
		{
			if (file_name.ends_with(".bank"))
			{
				Dictionary bank_info;

				bank_info["file_path"] = bank_path + String("/") + file_name;

				Ref<FileAccess> file =
						FileAccess::open(bank_path + String("/") + file_name, FileAccess::ModeFlags::READ);

				bank_info["md5"] = file->get_md5(bank_path + String("/") + file_name);
				bank_info["modified_time"] = file->get_modified_time(bank_path + String("/") + file_name);

				files.push_back(bank_info);
			}

			file_name = dir->get_next();
		}
	}

	return files;
}

Ref<BankAsset> FMODStudioEditorModule::get_bank_reference(const Dictionary& file_bank_info) const
{
	Ref<BankAsset> bank_ref = create_ref<BankAsset>();
	bank_ref->set_bank_file_info(file_bank_info);

	return bank_ref;
}

FMODStudioEditorModule::FMODStudioEditorModule()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

FMODStudioEditorModule::~FMODStudioEditorModule()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

FMODStudioEditorModule* FMODStudioEditorModule::get_singleton()
{
	return singleton;
}

bool FMODStudioEditorModule::init()
{
	bool result = initialize_fmod();

	if (!result)
	{
		ERROR_CHECK(FMOD_ERR_INITIALIZATION);
		return result;
	}

	String message = "[FMOD] Initialized Editor System";
	UtilityFunctions::print(message);

	project_cache = get_project_cache();

	result = true;
	return result;
}

void FMODStudioEditorModule::shutdown()
{
	if (shutdown_fmod())
	{
		String message = "[FMOD] Shut down Editor System";
		UtilityFunctions::print(message);
	}
}

void FMODStudioEditorModule::load_all_banks()
{
	banks_loaded = false;

	const String settings_path = get_platform_setting_path(PlatformSettingsPath::FMOD_SETTINGS_PATH);
	String banks_path = ProjectSettings::get_singleton()->get(settings_path + String("banks_path"));
	Array bank_files_infos = get_bank_file_infos(banks_path);

	if (bank_files_infos.size() == 0)
	{
		first_run = false;
		UtilityFunctions::push_warning("[FMOD] No Banks found in the banks folder. Add a path to a folder containing Banks in the FMOD Project Settings and refresh the Project.");
		return;
	}

	int index = -1;
	for (int64_t i = 0; i < bank_files_infos.size(); i++)
	{
		Dictionary bank_file_info = bank_files_infos[i];
		String filePath = bank_file_info["file_path"];

		if (filePath.contains(".strings"))
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		first_run = false;
		UtilityFunctions::push_warning(
				"[FMOD] No .strings bank found in the bank path. Check the Banks Path setting in the Project Settings and make sure to export a .strings Master Bank.");
		return;
	}

	Dictionary strings_bank_info = bank_files_infos[index];
	String strings_bank_path = strings_bank_info["file_path"];

	FMOD::Studio::Bank* strings_bank = nullptr;
	studio_system->loadBankFile(strings_bank_path.utf8().get_data(), FMOD_STUDIO_LOAD_BANK_NORMAL, &strings_bank);

	for (int64_t i = 0; i < bank_files_infos.size(); i++)
	{
		Dictionary bank_file_info = bank_files_infos[i];
		String file_path = bank_file_info["file_path"];

		// Note(alex): We already loaded the .strings bank, so skip it now
		if (file_path.contains(".strings"))
		{
			Ref<BankAsset> strings_bank_asset = get_bank_reference(bank_files_infos[i]);
			bank_loading_check.emplace(strings_bank, strings_bank_asset);
			continue;
		}

		FMOD::Studio::Bank* bank = nullptr;
		studio_system->loadBankFile(file_path.utf8().get_data(), FMOD_STUDIO_LOAD_BANK_NONBLOCKING, &bank);
		Ref<BankAsset> bank_asset = get_bank_reference(bank_files_infos[i]);
		bank_loading_check.emplace(bank, bank_asset);
	}

	emit_signal("banks_loading");
}

void FMODStudioEditorModule::unload_all_banks()
{
	studio_system->unloadAll();
	studio_system->update();
	studio_system->flushCommands();
	bank_refs.clear();
}

void FMODStudioEditorModule::create_icon(const String& icon_path, FMODIconType icon_type)
{
	Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(icon_path);

	if (texture.is_valid())
	{
		icons.try_emplace(icon_type, texture);
	}
}

Ref<Texture2D> FMODStudioEditorModule::get_icon(FMODIconType icon_type)
{
	if (icons.count(icon_type) > 0)
	{
		Ref<Texture2D> texture = icons[icon_type];
		return texture;
	}

	UtilityFunctions::push_warning(String("[FMOD] Failed to get icon: ") + itos(icon_type));

	return Ref<Texture2D>();
}

Ref<ProjectCache> FMODStudioEditorModule::generate_cache(const Dictionary& project_info)
{
	Ref<ProjectCache> project_cache = create_ref<ProjectCache>();
	project_cache->initialize_cache(project_info);
	emit_signal("cache_generated");
	return project_cache;
}

void FMODStudioEditorModule::reload_cache_file()
{
	project_cache =
			ResourceLoader::get_singleton()->load("res://addons/FMOD/editor/cache/fmod_project_cache.tres", "ProjectCache",
					ResourceLoader::CacheMode::CACHE_MODE_REPLACE);
}

Ref<ProjectCache> FMODStudioEditorModule::get_project_cache()
{
	if (!FileAccess::file_exists("res://addons/FMOD/editor/cache/fmod_project_cache.tres"))
	{
		Dictionary project_info = get_project_info_from_banks();
		project_cache = generate_cache(project_info);
	}

	if (project_cache.is_null())
	{
		reload_cache_file();
	}

	return project_cache;
}

Dictionary FMODStudioEditorModule::get_project_info_from_banks()
{
	const String resources_path = "res://addons/FMOD/editor/resources";

	Dictionary resource_dirs;
	resource_dirs["events"] = "res://addons/FMOD/editor/resources/events/";
	resource_dirs["snapshots"] = "res://addons/FMOD/editor/resources/snapshots/";
	resource_dirs["busses"] = "res://addons/FMOD/editor/resources/busses/";
	resource_dirs["vcas"] = "res://addons/FMOD/editor/resources/vcas/";
	resource_dirs["banks"] = "res://addons/FMOD/editor/resources/banks/";
	resource_dirs["parameters"] = "res://addons/FMOD/editor/resources/parameters/";

	Ref<DirAccess> dir = DirAccess::open(resources_path);
	if (dir.is_null())
	{
		DirAccess::make_dir_absolute(resources_path);
		dir = DirAccess::open(resources_path);
	}

	for (int64_t i = 0; i < resource_dirs.size(); i++)
	{
		const String path = resource_dirs.values()[i];
		if (!dir->dir_exists(path))
		{
			dir->make_dir(path);
		}
	}

	Dictionary events, snapshots, busses, vcas, parameters, banks;

	for (int64_t i = 0; i < bank_refs.size(); i++)
	{
		Ref<BankAsset> bank_asset = bank_refs[i];
		FMOD::Studio::Bank* bank = nullptr;
		String guid = bank_asset->get_guid();
		FMOD_GUID fmod_guid{};
		FMOD_Studio_ParseID(guid.utf8().get_data(), &fmod_guid);
		studio_system->getBankByID(&fmod_guid, &bank);

		if (!FileAccess::file_exists(resource_dirs["banks"].operator godot::String() + guid + ".tres"))
		{
			ResourceSaver::get_singleton()->save(bank_asset, resource_dirs["banks"].operator godot::String() + guid + ".tres");
			banks[guid] = bank_asset;
		}
		else
		{
			Ref<BankAsset> asset = ResourceLoader::get_singleton()->load(resource_dirs["banks"].operator godot::String() + guid + ".tres", "BankAsset");
			asset->set_name(bank_asset->get_name());
			asset->set_path(bank_asset->get_path());
			asset->set_modified_time(bank_asset->get_modified_time());
			ResourceSaver::get_singleton()->save(asset, resource_dirs["banks"].operator godot::String() + guid + ".tres");
			banks[guid] = asset;
		}

		int event_count = 0;
		bank->getEventCount(&event_count);

		if (event_count > 0)
		{
			Dictionary event_infos = get_event_list(bank, event_count);
			Array events_array = event_infos["events"];
			Array snapshots_array = event_infos["snapshots"];

			for (int64_t i = 0; i < events_array.size(); i++)
			{
				Ref<EventAsset> event = events_array[i];
				String guid = event->get_guid();

				if (!FileAccess::file_exists(resource_dirs["events"].operator godot::String() + guid + ".tres"))
				{
					ResourceSaver::get_singleton()->save(event, resource_dirs["events"].operator godot::String() + guid + ".tres");
					events[guid] = event;
				}
				else
				{
					Ref<EventAsset> asset = ResourceLoader::get_singleton()->load(resource_dirs["events"].operator godot::String() + guid + ".tres", "EventAsset");

					if (has_event_changed(asset, event))
					{
						asset->set_name(event->get_name());
						asset->set_path(event->get_path());
						asset->set_3d(event->get_3d());
						asset->set_oneshot(event->get_oneshot());
						asset->set_is_snapshot(event->get_is_snapshot());
						asset->set_min_distance(event->get_min_distance());
						asset->set_max_distance(event->get_max_distance());
						asset->set_parameters(event->get_parameters());
						ResourceSaver::get_singleton()->save(asset, resource_dirs["events"].operator godot::String() + guid + ".tres");
					}
					events[guid] = asset;
				}
			}

			for (int64_t i = 0; i < snapshots_array.size(); i++)
			{
				Ref<EventAsset> snapshot = snapshots_array[i];
				String guid = snapshot->get_guid();

				if (!FileAccess::file_exists(resource_dirs["snapshots"].operator godot::String() + guid + ".tres"))
				{
					ResourceSaver::get_singleton()->save(snapshot, resource_dirs["snapshots"].operator godot::String() + guid + ".tres");
					snapshots[guid] = ResourceSaver::get_singleton()->save(snapshot, resource_dirs["snapshots"].operator godot::String() + guid + ".tres");
				}
				else
				{
					Ref<EventAsset> asset = ResourceLoader::get_singleton()->load(resource_dirs["snapshots"].operator godot::String() + guid + ".tres", "EventAsset");
					if (has_event_changed(asset, snapshot))
					{
						asset->set_name(snapshot->get_name());
						asset->set_path(snapshot->get_path());
						asset->set_3d(snapshot->get_3d());
						asset->set_oneshot(snapshot->get_oneshot());
						asset->set_is_snapshot(snapshot->get_is_snapshot());
						asset->set_min_distance(snapshot->get_min_distance());
						asset->set_max_distance(snapshot->get_max_distance());
						asset->set_parameters(snapshot->get_parameters());
						ResourceSaver::get_singleton()->save(asset, resource_dirs["snapshots"].operator godot::String() + guid + ".tres");
					}
					snapshots[guid] = asset;
				}
			}
		}

		int bus_count = 0;
		bank->getBusCount(&bus_count);

		if (bus_count > 0)
		{
			Array bus_infos = get_bus_list(bank, bus_count);

			for (int64_t i = 0; i < bus_infos.size(); i++)
			{
				Ref<BusAsset> bus = bus_infos[i];
				String guid = bus->get_guid();

				if (!FileAccess::file_exists(resource_dirs["busses"].operator godot::String() + guid + ".tres"))
				{
					ResourceSaver::get_singleton()->save(bus, resource_dirs["busses"].operator godot::String() + guid + ".tres");
					busses[guid] = bus;
				}
				else
				{
					Ref<BusAsset> asset = ResourceLoader::get_singleton()->load(resource_dirs["busses"].operator godot::String() + guid + ".tres", "BusAsset");
					asset->set_name(bus->get_name());
					asset->set_path(bus->get_path());
					ResourceSaver::get_singleton()->save(asset, resource_dirs["busses"].operator godot::String() + guid + ".tres");
					busses[guid] = asset;
				}
			}
		}

		int vca_count = 0;
		bank->getVCACount(&vca_count);

		if (vca_count > 0)
		{
			Array vca_infos = get_vca_list(bank, vca_count);
			for (int64_t i = 0; i < vca_infos.size(); i++)
			{
				Ref<VCAAsset> vca = vca_infos[i];
				String guid = vca->get_guid();

				if (!FileAccess::file_exists(resource_dirs["vcas"].operator godot::String() + guid + ".tres"))
				{
					ResourceSaver::get_singleton()->save(vca, resource_dirs["vcas"].operator godot::String() + guid + ".tres");
					vcas[guid] = vca;
				}
				else
				{
					Ref<VCAAsset> asset = ResourceLoader::get_singleton()->load(resource_dirs["vcas"].operator godot::String() + guid + ".tres", "VCAAsset");
					asset->set_name(vca->get_name());
					asset->set_path(vca->get_path());
					ResourceSaver::get_singleton()->save(asset, resource_dirs["vcas"].operator godot::String() + guid + ".tres");
					vcas[guid] = asset;
				}
			}
		}
	}

	int parameter_count = 0;
	studio_system->getParameterDescriptionCount(&parameter_count);

	if (parameter_count > 0)
	{
		Array parameter_infos = get_global_parameter_list(parameter_count);
		for (int64_t i = 0; i < parameter_infos.size(); i++)
		{
			Ref<ParameterAsset> parameter = parameter_infos[i];
			String guid = parameter->get_guid();

			if (!FileAccess::file_exists(resource_dirs["parameters"].operator godot::String() + guid + ".tres"))
			{
				ResourceSaver::get_singleton()->save(parameter, resource_dirs["parameters"].operator godot::String() + guid + ".tres");
				parameters[guid] = parameter;
			}
			else
			{
				Ref<ParameterAsset> asset = ResourceLoader::get_singleton()->load(resource_dirs["parameters"].operator godot::String() + guid + ".tres", "ParameterAsset");
				asset->set_name(parameter->get_name());
				asset->set_path(parameter->get_path());
				asset->set_parameter_description(parameter->get_parameter_description());
				FMOD_STUDIO_PARAMETER_DESCRIPTION desc;
				parameter->get_parameter_description()->get_parameter_description(desc);
				asset->set_parameter_ref(desc);
				ResourceSaver::get_singleton()->save(asset, resource_dirs["parameters"].operator godot::String() + guid + ".tres");
				parameters[guid] = asset;
			}
		}
	}

	Dictionary result;
	result["events"] = events;
	result["snapshots"] = snapshots;
	result["busses"] = busses;
	result["vcas"] = vcas;
	result["parameters"] = parameters;
	result["banks"] = banks;

	return result;
}

Array FMODStudioEditorModule::create_tree(const Dictionary& list, const FMODAssetType asset_type)
{
	Dictionary tree;
	tree["item_type"] = FMODAssetType::FMOD_ASSETTYPE_NONE;
	tree["node_name"] = "root";
	tree["path"] = "";
	tree["resource"] = Variant();
	tree["children"] = Array();

	Array assets = list.values();
	assets.sort_custom(Callable(FMODStudioEditorModule::get_singleton(), "sort_items_by_path"));

	for (int64_t i = 0; i < assets.size(); i++)
	{
		Variant asset = assets[i];
		String path = asset.get("path");

		PackedStringArray path_array = path.split("/", false);

		Dictionary current_node = tree;

		FMODAssetType item_type = asset_type;

		const int64_t path_array_size = path_array.size();
		for (int64_t j = 0; j < path_array_size; j++)
		{
			if (j == 0 && item_type != FMODAssetType::FMOD_ASSETTYPE_BUS)
			{
				continue;
			}

			String node_name = path_array[j];

			Array children = current_node["children"];
			Dictionary child;
			for (int c = 0; c < children.size(); c++)
			{
				Dictionary temp_child = children[c];
				String temp_node_name = temp_child["node_name"];
				if (temp_node_name == node_name)
				{
					child = children[c];
				}
			}
			if (!child.is_empty())
			{
				current_node = child;
			}
			else
			{
				int64_t to_check = path_array.size() - 1;

				if (j == to_check)
				{
					Ref<Resource> resource;
					resource = assets[i];
					child = Dictionary();
					child["item_type"] = item_type;
					child["node_name"] = node_name;
					child["path"] = path;
					child["resource"] = resource;
					child["children"] = Array();
				}
				else
				{
					if (item_type == FMODAssetType::FMOD_ASSETTYPE_BUS)
					{
						Ref<Resource> resource;
						PackedStringArray sliced_bus = path_array.slice(0, j + 1);
						String finalpath = "bus:";
						for (int l = 0; l < sliced_bus.size(); l++)
						{
							if (l == 0)
							{
								continue;
							}
							finalpath = finalpath + "/" + sliced_bus[l];
						}
						for (int k = 0; k < assets.size(); k++)
						{
							String path_k = assets[k].get("path");
							if (path_k == finalpath)
							{
								resource = assets[k];
							}
						}
						child = Dictionary();
						child["item_type"] = FMODAssetType::FMOD_ASSETTYPE_BUS;
						child["node_name"] = node_name;
						child["path"] = finalpath;
						child["resource"] = resource;
						child["children"] = Array();
					}
					else
					{
						child = Dictionary();
						child["item_type"] = FMODAssetType::FMOD_ASSETTYPE_FOLDER;
						child["node_name"] = node_name;
						child["path"] = "";
						child["resource"] = Variant();
						child["children"] = Array();
					}
				}
				children.push_back(child);
				current_node = child;
			}
		}
	}
	return tree["children"];
}

void FMODStudioEditorModule::create_tree_items(Object* root, const Array& items, Object* parent)
{
	Tree* tree = Object::cast_to<Tree>(root);
	const int64_t item_count = items.size();

	for (int64_t i = 0; i < item_count; i++)
	{
		const Dictionary& item = items[i];
		const int type = item["item_type"];
		const FMODAssetType item_type = static_cast<FMODAssetType>(type);
		String node_name = item["node_name"];

		if (item_type == FMODAssetType::FMOD_ASSETTYPE_FOLDER || item_type == FMODAssetType::FMOD_ASSETTYPE_BUS)
		{
			if (node_name == "bus:")
			{
				node_name = "Master";
			}

			TreeItem* new_item = tree->create_item(Object::cast_to<TreeItem>(parent));
			new_item->set_text(0, node_name);
			new_item->set_tooltip_text(0, " ");

			if (item_type == FMODAssetType::FMOD_ASSETTYPE_FOLDER)
			{
				Ref<Texture2D> const icon = get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_FOLDER_CLOSED);
				new_item->set_icon(0, icon);
				new_item->set_meta("Type", "folder");
			}
			else if (item_type == FMODAssetType::FMOD_ASSETTYPE_BUS)
			{
				Ref<Texture2D> const icon = get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_BUS);
				new_item->set_icon(0, icon);
				new_item->set_meta("Type", "bus");
				new_item->set_meta("Path", item["path"]);
				new_item->set_meta("Resource", item["resource"]);
				new_item->set_text(0, node_name);
			}

			new_item->set_collapsed(true);

			const Array& children = item["children"];

			if (children.size() > 0)
			{
				create_tree_items(root, children, new_item);
			}
		}
		else
		{
			if (!parent)
			{
				return;
			}

			TreeItem* new_asset_item = tree->create_item(Object::cast_to<TreeItem>(parent));

			Ref<Texture2D> icon;
			String meta_type;

			switch (item_type)
			{
				case FMODAssetType::FMOD_ASSETTYPE_EVENT:
					icon = get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_EVENT);
					meta_type = "event";
					break;
				case FMODAssetType::FMOD_ASSETTYPE_SNAPSHOT:
					icon = get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_SNAPSHOT);
					meta_type = "event";
					break;
				case FMODAssetType::FMOD_ASSETTYPE_BANK:
					icon = get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_BANK);
					meta_type = "bank";
					break;
				case FMODAssetType::FMOD_ASSETTYPE_BUS:
					icon = get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_BUS);
					meta_type = "bus";
					break;
				case FMODAssetType::FMOD_ASSETTYPE_VCA:
					icon = get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_VCA);
					meta_type = "vca";
					break;
				case FMODAssetType::FMOD_ASSETTYPE_GLOBAL_PARAMETER:
					icon = get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_C_PARAMETER);
					meta_type = "parameter";
					break;
				default:
					break;
			}

			if (icon.is_valid())
			{
				new_asset_item->set_icon(0, icon);
			}

			new_asset_item->set_meta("Resource", item["resource"]);

			String meta_guid;
			Ref<FMODAsset> asset = item["resource"];

			if (asset.is_valid())
			{
				meta_guid = asset->get_guid();
				new_asset_item->set_meta("Guid", meta_guid);
			}

			new_asset_item->set_tooltip_text(0, " ");
			new_asset_item->set_text(0, node_name);
			new_asset_item->set_meta("Type", meta_type);
			new_asset_item->set_meta("Path", item["path"]);
		}
	}
}

bool FMODStudioEditorModule::sort_items_by_path(const Variant& a, const Variant& b)
{
	Ref<FMODAsset> a_asset = a;
	Ref<FMODAsset> b_asset = b;

	if (a_asset.is_null() || b_asset.is_null())
	{
		return false;
	}

	String path_a = a_asset->get_path();
	String path_b = b_asset->get_path();

	return path_a.naturalnocasecmp_to(path_b) < 0;
}

bool FMODStudioEditorModule::sort_parameters_by_name(const Variant& a, const Variant& b)
{
	Ref<ParameterAsset> param_a = a;
	Ref<ParameterAsset> param_b = b;

	if (param_a.is_null() || param_b.is_null())
	{
		return false;
	}

	return param_a->get_name().naturalnocasecmp_to(param_b->get_name()) < 0;
}

bool FMODStudioEditorModule::has_event_changed(const Ref<EventAsset>& old_event, const Ref<EventAsset>& to_check)
{
	String old_name = old_event->get_name();
	bool old_3d = old_event->get_3d();
	bool old_oneshot = old_event->get_oneshot();
	float old_max_distance = old_event->get_max_distance();
	float old_min_distance = old_event->get_min_distance();

	if (old_name != to_check->get_name() ||
			old_3d != to_check->get_3d() ||
			old_oneshot != to_check->get_oneshot() ||
			old_max_distance != to_check->get_max_distance() ||
			old_min_distance != to_check->get_min_distance())
	{
		return true;
	}

	Dictionary new_parameters = to_check->get_parameters();
	Dictionary old_parameters = old_event->get_parameters();

	if (new_parameters.size() != old_parameters.size())
	{
		return true;
	}

	for (int j = 0; j < new_parameters.keys().size(); j++)
	{
		if (!old_event->has_parameter(new_parameters.keys()[j]))
		{
			return true;
		}

		Ref<ParameterAsset> new_parameter = new_parameters.values()[j];
		Ref<ParameterAsset> old_parameter = old_parameters[new_parameter->get_guid()];

		if (new_parameter->get_name() != old_parameter->get_name() ||
				new_parameter->get_parameter_description()->get_maximum() != old_parameter->get_parameter_description()->get_maximum() ||
				new_parameter->get_parameter_description()->get_minimum() != old_parameter->get_parameter_description()->get_minimum() ||
				new_parameter->get_parameter_description()->get_flags() != old_parameter->get_parameter_description()->get_flags())
		{
			return true;
		}
	}

	return false;
}

void FMODStudioEditorModule::play_event(const String& guid)
{
	FMOD::Studio::EventDescription* event_description = nullptr;
	FMOD_GUID fmod_guid;
	FMOD_Studio_ParseID(guid.utf8().get_data(), &fmod_guid);
	studio_system->getEventByID(&fmod_guid, &event_description);
	FMOD::Studio::EventInstance* event_instance = nullptr;

	event_description->createInstance(&event_instance);
	event_instance->start();

	studio_system->update();

	preview_events.push_back(event_instance);
}

void FMODStudioEditorModule::stop_events(bool allow_fadeout)
{
	FMOD_STUDIO_STOP_MODE stop_mode = allow_fadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;

	for (int i = 0; i < preview_events.size(); i++)
	{
		preview_events[i]->stop(stop_mode);
		preview_events[i]->release();
		studio_system->update();
	}
	preview_events.clear();
}

void FMODStudioEditorModule::set_preview_parameter(const String& parameter_name, float value)
{
	// note(alex): make this better in the future
	for (int i = 0; i < preview_events.size(); i++)
	{
		preview_events[i]->setParameterByName(parameter_name.utf8().get_data(), value);
	}

	studio_system->update();
}

FMOD::Studio::System* FMODStudioEditorModule::get_studio_system()
{
	return studio_system;
}

void FMODStudioEditorModule::set_is_initialized(bool initialized)
{
	is_initialized = initialized;
}

bool FMODStudioEditorModule::get_is_initialized() const
{
	return is_initialized;
}

void FMODStudioEditorModule::poll_banks_loading_state(Timer* timer)
{
	static int retries = 0;
	const int max_retries = 10;

	if (!banks_loaded)
	{
		for (auto it = bank_loading_check.cbegin(); it != bank_loading_check.cend();)
		{
			FMOD_STUDIO_LOADING_STATE state = FMOD_STUDIO_LOADING_STATE_UNLOADED;
			ERROR_CHECK(it->first->getLoadingState(&state));
			if (state == FMOD_STUDIO_LOADING_STATE_LOADED)
			{
				Ref<BankAsset> asset = it->second;
				asset->set_bank_ref(it->first);
				bank_refs.append(asset);
				bank_loading_check.erase(it++);
			}
			else
			{
				studio_system->update();
				++it;
			}
		}

		if (bank_loading_check.size() == 0)
		{
			banks_loaded = true;

			if (timer)
			{
				timer->stop();
				timer->queue_free();
			}

			emit_signal("banks_loaded");
			UtilityFunctions::print("[FMOD] Loaded Editor Banks");
		}

		if (retries == max_retries)
		{
			if (timer)
			{
				timer->stop();
				timer->queue_free();
				retries = 0;

				UtilityFunctions::push_warning("[FMOD] Failed to load editor banks. Please verify that the banks path location in the Project Settings is correct. Refresh the project afterwards.");
				return;
			}
		}

		++retries;
	}
}

bool FMODStudioEditorModule::get_all_banks_loaded() const
{
	return banks_loaded;
}

void ProjectCache::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_banks", "banks"), &ProjectCache::set_banks);
	ClassDB::bind_method(D_METHOD("get_banks"), &ProjectCache::get_banks);
	ClassDB::bind_method(D_METHOD("set_events", "events"), &ProjectCache::set_events);
	ClassDB::bind_method(D_METHOD("get_events"), &ProjectCache::get_events);
	ClassDB::bind_method(D_METHOD("set_snapshots", "snapshots"), &ProjectCache::set_snapshots);
	ClassDB::bind_method(D_METHOD("get_snapshots"), &ProjectCache::get_snapshots);
	ClassDB::bind_method(D_METHOD("set_busses", "busses"), &ProjectCache::set_busses);
	ClassDB::bind_method(D_METHOD("get_busses"), &ProjectCache::get_busses);
	ClassDB::bind_method(D_METHOD("set_vcas", "vcas"), &ProjectCache::set_vcas);
	ClassDB::bind_method(D_METHOD("get_vcas"), &ProjectCache::get_vcas);
	ClassDB::bind_method(D_METHOD("set_parameters", "parameters"), &ProjectCache::set_parameters);
	ClassDB::bind_method(D_METHOD("get_parameters"), &ProjectCache::get_parameters);
	ClassDB::bind_method(D_METHOD("set_bank_tree", "bank_tree"), &ProjectCache::set_bank_tree);
	ClassDB::bind_method(D_METHOD("get_bank_tree"), &ProjectCache::get_bank_tree);
	ClassDB::bind_method(D_METHOD("set_event_tree", "event_tree"), &ProjectCache::set_event_tree);
	ClassDB::bind_method(D_METHOD("get_event_tree"), &ProjectCache::get_event_tree);
	ClassDB::bind_method(D_METHOD("set_snapshot_tree", "snapshot_tree"), &ProjectCache::set_snapshot_tree);
	ClassDB::bind_method(D_METHOD("get_snapshot_tree"), &ProjectCache::get_snapshot_tree);
	ClassDB::bind_method(D_METHOD("set_bus_tree", "bus_tree"), &ProjectCache::set_bus_tree);
	ClassDB::bind_method(D_METHOD("get_bus_tree"), &ProjectCache::get_bus_tree);
	ClassDB::bind_method(D_METHOD("set_vca_tree", "vca_tree"), &ProjectCache::set_vca_tree);
	ClassDB::bind_method(D_METHOD("get_vca_tree"), &ProjectCache::get_vca_tree);
	ClassDB::bind_method(D_METHOD("set_parameter_tree", "parameter_tree"), &ProjectCache::set_parameter_tree);
	ClassDB::bind_method(D_METHOD("get_parameter_tree"), &ProjectCache::get_parameter_tree);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "banks", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_banks", "get_banks");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "events", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_events", "get_events");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "snapshots", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_snapshots", "get_snapshots");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "vcas", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_vcas",
			"get_vcas");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "busses", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_busses", "get_busses");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "parameters", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_parameters", "get_parameters");

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "bank_tree", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE),
			"set_bank_tree", "get_bank_tree");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "event_tree", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE),
			"set_event_tree", "get_event_tree");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "snapshot_tree", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE),
			"set_snapshot_tree", "get_snapshot_tree");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "vca_tree", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE),
			"set_vca_tree", "get_vca_tree");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "bus_tree", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE),
			"set_bus_tree", "get_bus_tree");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "parameter_tree", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE),
			"set_parameter_tree", "get_parameter_tree");
}

void ProjectCache::initialize_cache(const Dictionary& project_info)
{
	set_banks(project_info["banks"]);
	set_events(project_info["events"]);
	set_snapshots(project_info["snapshots"]);
	set_vcas(project_info["vcas"]);
	set_busses(project_info["busses"]);
	set_parameters(project_info["parameters"]);
	set_bank_tree(FMODStudioEditorModule::get_singleton()->create_tree(banks, FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BANK));
	set_event_tree(FMODStudioEditorModule::get_singleton()->create_tree(events, FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_EVENT));
	set_snapshot_tree(FMODStudioEditorModule::get_singleton()->create_tree(snapshots, FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_SNAPSHOT));
	set_vca_tree(FMODStudioEditorModule::get_singleton()->create_tree(vcas, FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_VCA));
	set_bus_tree(FMODStudioEditorModule::get_singleton()->create_tree(busses, FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BUS));
	set_parameter_tree(FMODStudioEditorModule::get_singleton()->create_tree(parameters, FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_GLOBAL_PARAMETER));

	const String cache_dir = "res://addons/FMOD/editor/cache";
	Ref<DirAccess> dir = DirAccess::open(cache_dir);
	if (dir.is_null())
	{
		DirAccess::make_dir_absolute(cache_dir);
		dir = DirAccess::open(cache_dir);
	}

	const String cache_path = "res://addons/FMOD/editor/cache/fmod_project_cache.tres";
	take_over_path(cache_path);
	ResourceSaver::get_singleton()->save(this, cache_path);
	String message = "[FMOD] Cache created in {0}";
	UtilityFunctions::print(message.format(Array::make(cache_path)));
	emit_changed();
}

void ProjectCache::set_banks(const Dictionary& banks)
{
	this->banks = banks;
}

Dictionary ProjectCache::get_banks()
{
	return banks;
}

void ProjectCache::set_events(const Dictionary& events)
{
	this->events = events;
}

Dictionary ProjectCache::get_events()
{
	return events;
}

void ProjectCache::set_snapshots(const Dictionary& snapshots)
{
	this->snapshots = snapshots;
}

Dictionary ProjectCache::get_snapshots()
{
	return snapshots;
}

void ProjectCache::set_busses(const Dictionary& busses)
{
	this->busses = busses;
}

Dictionary ProjectCache::get_busses()
{
	return busses;
}

void ProjectCache::set_vcas(const Dictionary& vcas)
{
	this->vcas = vcas;
}

Dictionary ProjectCache::get_vcas()
{
	return vcas;
}

void ProjectCache::set_parameters(const Dictionary& parameters)
{
	this->parameters = parameters;
}

Dictionary ProjectCache::get_parameters()
{
	return parameters;
}

void ProjectCache::set_bank_tree(const Array& bank_tree)
{
	this->bank_tree = bank_tree;
}

Array ProjectCache::get_bank_tree()
{
	return bank_tree;
}

void ProjectCache::set_event_tree(const Array& event_tree)
{
	this->event_tree = event_tree;
}

Array ProjectCache::get_event_tree()
{
	return event_tree;
}

void ProjectCache::set_snapshot_tree(const Array& snapshot_tree)
{
	this->snapshot_tree = snapshot_tree;
}

Array ProjectCache::get_snapshot_tree()
{
	return snapshot_tree;
}

void ProjectCache::set_bus_tree(const Array& bus_tree)
{
	this->bus_tree = bus_tree;
}

Array ProjectCache::get_bus_tree()
{
	return bus_tree;
}

void ProjectCache::set_vca_tree(const Array& vca_tree)
{
	this->vca_tree = vca_tree;
}

Array ProjectCache::get_vca_tree()
{
	return vca_tree;
}

void ProjectCache::set_parameter_tree(const Array& parameter_tree)
{
	this->parameter_tree = parameter_tree;
}

Array ProjectCache::get_parameter_tree()
{
	return parameter_tree;
}