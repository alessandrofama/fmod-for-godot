#ifndef STUDIO_API
#define STUDIO_API

#include "fmod_types.h"
#include <memory>

namespace StudioApi
{

class EventDescription;
class EventInstance;
class Bus;
class VCA;
class Bank;
class CommandReplay;

class StudioSystem : public RefCounted
{
	GDCLASS(StudioSystem, RefCounted);

protected:
	static void _bind_methods();

private:
	FMOD::Studio::System* studio_system = nullptr;

public:
	void set_instance(FMOD::Studio::System* studio_system);

	bool is_valid() const;

	bool update() const;
	bool flush_commands() const;
	bool flush_sample_loading() const;

	Ref<EventDescription> get_event(const String& event_path) const;
	Ref<Bus> get_bus(const String& bus_path) const;
	Ref<VCA> get_vca(const String& vca_path) const;
	Ref<Bank> get_bank(const String& bank_path) const;
	Ref<EventDescription> get_event_by_id(const String& event_guid) const;
	Ref<Bus> get_bus_by_id(const String& bus_guid) const;
	Ref<VCA> get_vca_by_id(const String& vca_guid) const;
	Ref<Bank> get_bank_by_id(const String& bank_guid) const;

	// note(alex): StudioSystem::getSoundInfo is not implemented due to missing Core API implementation.
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> get_parameter_description_by_name(const String& name) const;
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> get_parameter_description_by_id(
			const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id) const;
	String get_parameter_label_by_name(const String& name, int label_index) const;
	String get_parameter_label_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id, int label_index) const;

	Dictionary get_parameter_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id) const;
	bool set_parameter_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id, float value,
			bool ignore_seek_speed = false) const;
	bool set_parameter_by_id_with_label(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id, const String& label,
			bool ignore_seek_speed = false) const;
	bool set_parameters_by_ids(const Array& parameter_ids, const Array& values, int count,
			bool ignore_seek_speed = false) const;
	Dictionary get_parameter_by_name(const String& name) const;
	bool set_parameter_by_name(const String& name, float value, bool ignore_seek_speed = false) const;
	bool set_parameter_by_name_with_label(const String& name, const String& label, bool ignore_seek_speed = false) const;

	String lookup_id(const String& path) const;
	String lookup_path(const String& guid) const;

	int get_num_listeners() const;
	bool set_num_listeners(int num_listeners) const;
	Dictionary get_listener_attributes(int listener) const;
	bool set_listener_attributes(int listener, Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes,
			const Variant& attenuation_position) const;
	float get_listener_weight(int listener) const;
	bool set_listener_weight(int listener, float weight) const;

	Ref<Bank> load_bank_file(const String& bankPath, FMOD_STUDIO_LOAD_BANK_FLAGS load_bank_flags,
			bool load_sample_data) const;
	bool unload_all() const;

	Ref<FmodTypes::FMOD_STUDIO_BUFFER_USAGE> get_buffer_usage() const;
	bool reset_buffer_usage() const;

	int get_bank_count() const;
	TypedArray<Bank> get_bank_list(int capacity) const;
	int get_parameter_description_count() const;
	TypedArray<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> get_parameter_description_list(int capacity) const;

	bool start_command_capture(const String& file_name, const FMOD_STUDIO_COMMANDCAPTURE_FLAGS flags) const;
	bool stop_command_capture() const;
	Ref<CommandReplay> load_command_replay(const String& file_name, FMOD_STUDIO_COMMANDREPLAY_FLAGS flags) const;

	bool set_callback(Variant callable, FMOD_STUDIO_SYSTEM_CALLBACK_TYPE callback_mask) const;

	void get_cpu_usage(Ref<FmodTypes::FMOD_STUDIO_CPU_USAGE> studio, Ref<FmodTypes::FMOD_CPU_USAGE> core) const;
	void get_memory_usage(
			Ref<FmodTypes::FMOD_STUDIO_MEMORY_USAGE> memory_usage) const;
};

class EventDescription : public RefCounted
{
	GDCLASS(EventDescription, RefCounted);

protected:
	static void _bind_methods();

private:
	FMOD::Studio::EventDescription* event_description = nullptr;

public:
	void set_instance(FMOD::Studio::EventDescription* event_description);
	FMOD::Studio::EventDescription* get_instance() const;

	bool is_valid() const;

	String get_id() const;
	String get_path() const;
	int get_parameter_description_count() const;
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> get_parameter_description_by_index(int index) const;
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> get_parameter_description_by_name(const String& name) const;
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> get_parameter_description_by_id(
			const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id) const;
	String get_parameter_label_by_index(int index, int label_index);
	String get_parameter_label_by_name(const String& name, int label_index);
	String get_parameter_label_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameterID,
			int label_index) const;
	int get_user_property_count() const;
	Ref<FmodTypes::FMOD_STUDIO_USER_PROPERTY> get_user_property_by_index(int index) const;
	Ref<FmodTypes::FMOD_STUDIO_USER_PROPERTY> get_user_property(const String& name) const;
	int get_length() const;
	Dictionary get_min_max_distance() const;
	float get_sound_size() const;

	bool is_snapshot() const;
	bool is_oneshot() const;
	bool is_stream() const;
	bool is_3d() const;
	bool is_doppler_enabled() const;
	bool has_sustain_point() const;

	Ref<EventInstance> create_instance();
	int get_instance_count() const;
	TypedArray<EventInstance> get_instance_list(int capacity);

	bool load_sample_data() const;
	bool unload_sample_data() const;
	FMOD_STUDIO_LOADING_STATE get_sample_loading_state() const;

	bool release_all_instances() const;

	bool set_callback(Variant callable, FMOD_STUDIO_EVENT_CALLBACK_TYPE callback_mask) const;
};

class EventInstance : public RefCounted
{
	GDCLASS(EventInstance, RefCounted);

protected:
	static void _bind_methods();

private:
	FMOD::Studio::EventInstance* event_instance = nullptr;

public:
	void set_instance(FMOD::Studio::EventInstance* event_instance);

	bool is_valid() const;

	Ref<EventDescription> get_description();

	Dictionary get_volume() const;
	bool set_volume(float volume) const;

	Dictionary get_pitch() const;
	bool set_pitch(float pitch) const;

	void get_3d_attributes(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes_ref) const;
	bool set_3d_attributes(const Ref<FmodTypes::FMOD_3D_ATTRIBUTES>& attributes_ref) const;

	unsigned int get_listener_mask() const;
	bool set_listener_mask(const unsigned int mask);

	float get_property(int event_property) const;
	bool set_property(int event_property, float property_value);

	float get_reverb_level(int index) const;
	bool set_reverb_level(int index, float level);

	bool get_paused() const;
	bool set_paused(bool paused) const;

	bool start() const;
	bool stop(FMOD_STUDIO_STOP_MODE stop_mode) const;

	int get_timeline_position() const;
	bool set_timeline_position(int position) const;

	FMOD_STUDIO_PLAYBACK_STATE get_playback_state() const;

	// note(alex): EventInstance::getChannelGroup not implemented due to missing Core API implementation.
	Dictionary get_min_max_distance() const;

	bool release();

	bool is_virtual() const;

	Dictionary get_parameter_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id) const;
	bool set_parameter_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id, float value,
			bool ignore_seek_speed = false) const;
	bool set_parameter_by_id_with_label(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id,
			const String& label, bool ignore_seek_speed = false) const;
	bool set_parameters_by_ids(const Array ids, const Array values, int count, bool ignore_seek_speed = false) const;

	Dictionary get_parameter_by_name(const String& name) const;
	bool set_parameter_by_name(const String& name, float value, bool ignore_seek_speed = false) const;
	bool set_parameter_by_name_with_label(const String& name, const String& label, bool ignore_seek_speed = false) const;

	bool key_off() const;

	void get_cpu_usage(Dictionary cpu_usage);
	void get_memory_usage(Ref<FmodTypes::FMOD_STUDIO_MEMORY_USAGE> memory_usage_ref);

	bool set_callback(Variant callable, FMOD_STUDIO_EVENT_CALLBACK_TYPE callback_mask) const;
};

class Bus : public RefCounted
{
	GDCLASS(Bus, RefCounted);

protected:
	static void _bind_methods();

private:
	FMOD::Studio::Bus* bus = nullptr;

public:
	void set_instance(FMOD::Studio::Bus* bus);

	bool is_valid() const;

	String get_id() const;
	String get_path() const;

	Dictionary get_volume() const;
	bool set_volume(float volume) const;

	bool get_paused() const;
	bool set_paused(bool paused) const;

	bool get_mute() const;
	bool set_mute(bool mute) const;

	bool stop_all_events(int stop_mode) const;

	bool lock_channel_group() const;
	bool unlock_channel_group() const;

	// note(alex): Bus::getChannelGroup not implemented due to missing Core API implementation.

	void get_cpu_usage(Dictionary cpu_usage) const;
	void get_memory_usage(Ref<FmodTypes::FMOD_STUDIO_MEMORY_USAGE> memory_usage_ref) const;
};

class VCA : public RefCounted
{
	GDCLASS(VCA, RefCounted);

protected:
	static void _bind_methods();

private:
	FMOD::Studio::VCA* vca = nullptr;

public:
	void set_instance(FMOD::Studio::VCA* vca);

	bool is_valid() const;

	String get_id() const;
	String get_path() const;

	Dictionary get_volume() const;
	bool set_volume(float volume) const;
};

class Bank : public RefCounted
{
	GDCLASS(Bank, RefCounted);

protected:
	static void _bind_methods();

private:
	FMOD::Studio::Bank* bank = nullptr;

public:
	void set_instance(FMOD::Studio::Bank* bank);

	bool is_valid() const;

	String get_id() const;
	String get_path() const;

	bool unload();
	bool load_sample_data() const;
	bool unload_sample_data() const;

	FMOD_STUDIO_LOADING_STATE get_loading_state() const;
	FMOD_STUDIO_LOADING_STATE get_sample_loading_state() const;

	int get_event_count() const;
	TypedArray<EventDescription> get_event_list(int capacity);
	int get_bus_count() const;
	TypedArray<Bus> get_bus_list(int capacity);
	int get_vca_count() const;
	TypedArray<VCA> get_vca_list(int capacity);
};

class CommandReplay : public RefCounted
{
	GDCLASS(CommandReplay, RefCounted);

protected:
	static void _bind_methods();

private:
	FMOD::Studio::CommandReplay* command_replay = nullptr;

public:
	void set_instance(FMOD::Studio::CommandReplay* command_replay);

	bool is_valid() const;

	float get_length() const;

	int get_command_count() const;
	Dictionary get_command_info(int commandIndex) const;
	String get_command_string(int commandIndex, int length) const;
	int get_command_at_time(float time) const;

	bool set_bank_path(const String& bankPath);
	bool start();
	bool stop();
	bool seek_to_time(float time);
	bool seek_to_command(int commandIndex);
	bool get_paused() const;
	bool set_paused(bool paused);
	FMOD_STUDIO_PLAYBACK_STATE get_playback_state() const;
	Dictionary get_current_command() const;
	bool release();
};

} // namespace StudioApi

#endif // STUDIO_API