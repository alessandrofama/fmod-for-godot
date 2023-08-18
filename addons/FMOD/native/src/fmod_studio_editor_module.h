#ifndef FMOD_EDITOR_H
#define FMOD_EDITOR_H

#include "api/studio_api.h"
#include "fmod_assets.h"
#include "fmod_types.h"
#include "misc/fmod_io.h"
#include "misc/fmod_memory.h"
#include "utils/tcp_client.h"
#include <algorithm>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/tree_item.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <map>
#include <unordered_map>

#ifdef FMOD_OSX
#include <AudioToolbox/AudioToolbox.h>
#endif

class ProjectCache;

class FMODSettings : public RefCounted
{
	GDCLASS(FMODSettings, RefCounted);

protected:
	static void _bind_methods();

public:
	FMODSettings();

private:
	void add_fmod_settings();
	void add_setting(const String& name, const Variant& default_value, Variant::Type type,
			PropertyHint hint = PROPERTY_HINT_NONE, const String& hint_string = "",
			int usage = PROPERTY_USAGE_DEFAULT);
};

class FMODStudioEditorModule : public Object
{
	GDCLASS(FMODStudioEditorModule, Object);

	enum FMODIconType
	{
		FMOD_ICONTYPE_PROJECT,
		FMOD_ICONTYPE_FOLDER_CLOSED,
		FMOD_ICONTYPE_FOLDER_OPENED,
		FMOD_ICONTYPE_EVENT,
		FMOD_ICONTYPE_BANK,
		FMOD_ICONTYPE_SNAPSHOT,
		FMOD_ICONTYPE_BUS,
		FMOD_ICONTYPE_VCA,
		FMOD_ICONTYPE_C_PARAMETER
	};

	enum FMODAssetType
	{
		FMOD_ASSETTYPE_NONE,
		FMOD_ASSETTYPE_EVENT,
		FMOD_ASSETTYPE_SNAPSHOT,
		FMOD_ASSETTYPE_BANK,
		FMOD_ASSETTYPE_BUS,
		FMOD_ASSETTYPE_VCA,
		FMOD_ASSETTYPE_GLOBAL_PARAMETER,
		FMOD_ASSETTYPE_FOLDER
	};

protected:
	static FMODStudioEditorModule* singleton;
	static void _bind_methods();

private:
	FMOD::Studio::System* studio_system;
	FMOD::System* core_system;
	Ref<ProjectCache> project_cache;
	std::unordered_map<FMODIconType, Ref<Texture2D>> icons;
	Array bank_refs;
	std::map<FMOD::Studio::Bank*, Ref<BankAsset>> bank_loading_check;
	std::vector<FMOD::Studio::EventInstance*> preview_events;
	bool banks_loaded = false;
	bool is_initialized = false;

	bool initialize_fmod();
	bool shutdown_fmod();

	Dictionary get_event_list(const FMOD::Studio::Bank* bank, int count) const;
	Array get_bus_list(const FMOD::Studio::Bank* bank, int count) const;
	Array get_vca_list(const FMOD::Studio::Bank* bank, int count) const;
	Array get_global_parameter_list(int count) const;
	Array get_bank_file_infos(const String& bankPath) const;
	Ref<BankAsset> get_bank_reference(const Dictionary& file_bank_info) const;

public:
	Ref<FmodTCPClient> client = create_ref<FmodTCPClient>();
	bool first_run = true;

	FMODStudioEditorModule();
	~FMODStudioEditorModule();
	static FMODStudioEditorModule* get_singleton();

	bool init();
	void shutdown();

	void load_all_banks();
	void unload_all_banks();
	void create_icon(const String& icon_path, FMODIconType icon_type);
	Ref<Texture2D> get_icon(FMODIconType icon_type);

	Ref<ProjectCache> generate_cache(const Dictionary& project_info);
	void reload_cache_file();
	Ref<ProjectCache> get_project_cache();
	Dictionary get_project_info_from_banks();
	Array create_tree(const Dictionary& list, const FMODAssetType asset_type);
	void create_tree_items(Object* root, const Array& items, Object* parent);

	bool sort_items_by_path(const Variant& a, const Variant& b);
	bool sort_parameters_by_name(const Variant& a, const Variant& b);

	bool has_event_changed(const Ref<EventAsset>& old_event, const Ref<EventAsset>& to_check);

	void play_event(const String& guid);
	void stop_events(bool allow_fadeout);
	void set_preview_parameter(const String& parameter_name, float value);

	FMOD::Studio::System* get_studio_system();

	void set_is_initialized(bool initialized);
	bool get_is_initialized() const;

	bool get_all_banks_loaded() const;
};

class ProjectCache : public Resource
{
	GDCLASS(ProjectCache, Resource);

protected:
	static void _bind_methods();

private:
	Array bank_tree;
	Array event_tree;
	Array snapshot_tree;
	Array bus_tree;
	Array vca_tree;
	Array parameter_tree;

public:
	Dictionary banks;
	Dictionary events;
	Dictionary snapshots;
	Dictionary busses;
	Dictionary vcas;
	Dictionary parameters;

	void initialize_cache(const Dictionary& project_info);

	void set_banks(const Dictionary& banks);
	Dictionary get_banks();
	void set_events(const Dictionary& events);
	Dictionary get_events();
	void set_snapshots(const Dictionary& snapshots);
	Dictionary get_snapshots();
	void set_busses(const Dictionary& busses);
	Dictionary get_busses();
	void set_vcas(const Dictionary& vcas);
	Dictionary get_vcas();
	void set_parameters(const Dictionary& parameters);
	Dictionary get_parameters();
	void set_bank_tree(const Array& bank_tree);
	Array get_bank_tree();
	void set_event_tree(const Array& event_tree);
	Array get_event_tree();
	void set_snapshot_tree(const Array& snapshot_tree);
	Array get_snapshot_tree();
	void set_bus_tree(const Array& bus_tree);
	Array get_bus_tree();
	void set_vca_tree(const Array& vca_tree);
	Array get_vca_tree();
	void set_parameter_tree(const Array& parameter_tree);
	Array get_parameter_tree();
};

VARIANT_ENUM_CAST(FMODStudioEditorModule::FMODAssetType);
VARIANT_ENUM_CAST(FMODStudioEditorModule::FMODIconType);

#endif // FMOD_STUDIO_EDITOR_MODULE_H