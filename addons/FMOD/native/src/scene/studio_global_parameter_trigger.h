#ifndef STUDIO_GLOBAL_PARAMETER_TRIGGER_H
#define STUDIO_GLOBAL_PARAMETER_TRIGGER_H

#include "fmod_assets.h"
#include "fmod_studio_module.h"
#include "utils/runtime_utils.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>

class StudioGlobalParameterTrigger : public Node
{
	GDCLASS(StudioGlobalParameterTrigger, Node);

protected:
	static void _bind_methods();
	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name, Variant& r_ret) const;
	void _get_property_list(List<PropertyInfo>* p_list) const;
	bool _property_can_revert(const StringName& p_name) const;
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const;

private:
	RuntimeUtils::GameEvent trigger_on = RuntimeUtils::GameEvent::GAMEEVENT_NONE;
	Dictionary overridden_parameter;
	Ref<ParameterAsset> parameter;
	float parameter_value = 0.0f;

public:
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void handle_game_event(RuntimeUtils::GameEvent game_event);
	void trigger();

	void set_trigger_on(RuntimeUtils::GameEvent trigger_on);
	RuntimeUtils::GameEvent get_trigger_on() const;

	void set_parameter(const Ref<ParameterAsset>& parameter);
	Ref<ParameterAsset> get_parameter() const;

	Dictionary get_overridden_parameter() const;
};

#endif // STUDIO_GLOBAL_PARAMETER_TRIGGER_H