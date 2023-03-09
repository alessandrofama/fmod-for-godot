#ifndef STUDIO_PARAMETER_TRIGGER_H
#define STUDIO_PARAMETER_TRIGGER_H

#include "fmod_studio_module.h"
#include "studio_event_emitter.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/node_path.hpp>

class StudioParameterTrigger : public Node
{
	GDCLASS(StudioParameterTrigger, Node);

protected:
	static void _bind_methods();
	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name, Variant& r_ret) const;
	void _get_property_list(List<PropertyInfo>* p_list) const;
	bool _property_can_revert(const StringName& p_name) const;
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const;

private:
	NodePath studio_event_emitter;
	Ref<EventAsset> event;
	RuntimeUtils::GameEvent trigger_on = RuntimeUtils::GameEvent::GAMEEVENT_NONE;
	bool event_changed = false;
	mutable Dictionary overridden_parameters;
	void reset();

public:
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void handle_game_event(RuntimeUtils::GameEvent game_event);
	void trigger();

	void set_studio_event_emitter(const Variant& path);
	NodePath get_studio_event_emitter() const;

	void set_trigger_on(RuntimeUtils::GameEvent trigger_on);
	RuntimeUtils::GameEvent get_trigger_on() const;

	void set_event(const Ref<EventAsset>& event_asset);
	Ref<EventAsset> get_event();

	void on_event_changed(Object* value);
};

#endif // STUDIO_PARAMETER_TRIGGER_H