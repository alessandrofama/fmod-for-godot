#ifndef STUDIO_EVENT_EMITTER_H
#define STUDIO_EVENT_EMITTER_H

#include "fmod_assets.h"
#include "fmod_studio_editor_module.h"
#include "scene/studio_listener.h"
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>

template <typename T>
struct StudioEventEmitterImpl
{
	T* node;
	RuntimeUtils::GameEvent play_event = RuntimeUtils::GameEvent::GAMEEVENT_NONE;
	RuntimeUtils::GameEvent stop_event = RuntimeUtils::GameEvent::GAMEEVENT_NONE;
	Ref<StudioApi::EventDescription> event_description;
	Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes = create_ref<FmodTypes::FMOD_3D_ATTRIBUTES>();
	Ref<EventAsset> event;
	bool has_triggered{};
	bool is_active{};
	bool preload_samples{};
	bool allow_fadeout = true;
	bool trigger_once{};
	Object* rigidbody = nullptr;
	Dictionary overridden_parameters;
	Ref<StudioApi::EventInstance> event_instance;

	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name, Variant& r_ret) const;
	void _get_property_list(List<PropertyInfo>* p_list) const;
	bool _property_can_revert(const StringName& p_name) const;
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const;

	void play_instance();
	void stop_instance();
	bool is_playing();

	void register_active_emitter(T* emitter);
	void deregister_active_emitter(T* emitter);
	void update_playing_status(bool force = false);

	void _enter_tree();
	void _ready();
	void _exit_tree();
	void _process(double p_delta);

	void handle_game_event(RuntimeUtils::GameEvent game_event);

	void play();
	void stop();
	void lookup();
	float get_max_distance();
};

class StudioEventEmitter2D : public Node2D
{
	GDCLASS(StudioEventEmitter2D, Node2D);

protected:
	static void _bind_methods();
	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name, Variant& r_ret) const;
	void _get_property_list(List<PropertyInfo>* p_list) const;
	bool _property_can_revert(const StringName& p_name) const;
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const;

public:
	StudioEventEmitterImpl<StudioEventEmitter2D> implementation;

	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;
	virtual void _process(double p_delta) override;

	void handle_game_event(RuntimeUtils::GameEvent game_event);

	void play();
	void stop();
	void lookup();
	float get_max_distance();

	void set_play_event(RuntimeUtils::GameEvent play_event);
	RuntimeUtils::GameEvent get_play_event() const;

	void set_stop_event(RuntimeUtils::GameEvent stop_event);
	RuntimeUtils::GameEvent get_stop_event() const;

	void set_event(const Ref<EventAsset>& event);
	Ref<EventAsset> get_event() const;

	void set_preload_samples(bool preload_samples);
	bool get_preload_samples() const;

	void set_allow_fadeout(bool allow_fadeout);
	bool get_allow_fadeout() const;

	void set_trigger_once(bool trigger_once);
	bool get_trigger_once() const;

	void set_rigidbody(Object* rigidbody);
	Object* get_rigidbody() const;

	void set_overridden_parameters(const Dictionary& overridden_parameters);
	Dictionary get_overridden_parameters() const;
};

class StudioEventEmitter3D : public Node3D
{
	GDCLASS(StudioEventEmitter3D, Node3D);

protected:
	static void _bind_methods();
	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name, Variant& r_ret) const;
	void _get_property_list(List<PropertyInfo>* p_list) const;
	bool _property_can_revert(const StringName& p_name) const;
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const;

public:
	StudioEventEmitterImpl<StudioEventEmitter3D> implementation;

	static void update_active_emitters();

	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;
	virtual void _process(double p_delta) override;

	void handle_game_event(RuntimeUtils::GameEvent game_event);

	void play();
	void stop();
	void lookup();
	float get_max_distance();

	void set_play_event(RuntimeUtils::GameEvent play_event);
	RuntimeUtils::GameEvent get_play_event() const;

	void set_stop_event(RuntimeUtils::GameEvent stop_event);
	RuntimeUtils::GameEvent get_stop_event() const;

	void set_event(const Ref<EventAsset>& event);
	Ref<EventAsset> get_event() const;

	void set_preload_samples(bool preload_samples);
	bool get_preload_samples() const;

	void set_allow_fadeout(bool allow_fadeout);
	bool get_allow_fadeout() const;

	void set_trigger_once(bool trigger_once);
	bool get_trigger_once() const;

	void set_rigidbody(Object* rigidbody);
	Object* get_rigidbody() const;

	void set_overridden_parameters(const Dictionary& overridden_parameters);
	Dictionary get_overridden_parameters() const;
};

#endif // STUDIO_EVENT_EMITTER_H