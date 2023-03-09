#include "studio_global_parameter_trigger.h"

void StudioGlobalParameterTrigger::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &StudioGlobalParameterTrigger::handle_game_event);
	ClassDB::bind_method(D_METHOD("trigger"), &StudioGlobalParameterTrigger::trigger);
	ClassDB::bind_method(D_METHOD("set_parameter", "parameter"),
			&StudioGlobalParameterTrigger::set_parameter);
	ClassDB::bind_method(D_METHOD("get_parameter"), &StudioGlobalParameterTrigger::get_parameter);
	ClassDB::bind_method(D_METHOD("set_trigger_on", "trigger_on"), &StudioGlobalParameterTrigger::set_trigger_on);
	ClassDB::bind_method(D_METHOD("get_trigger_on"), &StudioGlobalParameterTrigger::get_trigger_on);
	ClassDB::bind_method(D_METHOD("get_overridden_parameter"), &StudioGlobalParameterTrigger::get_overridden_parameter);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "trigger_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_trigger_on", "get_trigger_on");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "parameter", PROPERTY_HINT_RESOURCE_TYPE, "ParameterAsset",
						 PROPERTY_USAGE_DEFAULT),
			"set_parameter", "get_parameter");
}

bool StudioGlobalParameterTrigger::_set(const StringName& p_name, const Variant& p_value)
{
	String name = String(p_name);

	if (parameter.is_valid())
	{
		if (p_value.get_type() == Variant::NIL)
		{
			if (name == "value")
			{
				overridden_parameter.erase("value");
				notify_property_list_changed();
				return true;
			}
		}
		else
		{
			if (name == "value")
			{
				overridden_parameter["value"] = p_value;
				return true;
			}
		}
	}

	return false;
}

bool StudioGlobalParameterTrigger::_get(const StringName& p_name, Variant& r_ret) const
{
	String name = String(p_name);

	if (parameter.is_valid())
	{
		if (name == "value")
		{
			if (overridden_parameter.has("value"))
			{
				r_ret = overridden_parameter["value"];
				return true;
			}
			else
			{
				Ref<ParameterAsset> param = parameter;
				r_ret = param->get_parameter_description()->get_default_value();
				return true;
			}
		}
	}

	if (name == "parameter")
	{
		r_ret = parameter;
		return true;
	}

	return false;
}

void StudioGlobalParameterTrigger::_get_property_list(List<PropertyInfo>* p_list) const
{
	if (parameter.is_valid())
	{
		Variant::Type flags;
		float step{};
		Ref<ParameterAsset> param = parameter;
		Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> parameter_description =
				param->get_parameter_description();

		if ((parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_DISCRETE) &&
				!(parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_LABELED))
		{
			flags = Variant::FLOAT;
			step = 1.0f;
		}
		else if (parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_LABELED)
		{
			flags = Variant::INT;
		}
		else
		{
			flags = Variant::FLOAT;
			step = 0.01f;
		}

		uint32_t usage = PROPERTY_USAGE_EDITOR;

		if (overridden_parameter.has("value"))
		{
			usage |= PROPERTY_USAGE_STORAGE;
		}

		String hint_string;

		if (parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_LABELED)
		{
			if (param->get_labels().size() > 0)
			{
				for (int64_t i = 0; i < param->get_labels().size(); i++)
				{
					hint_string = hint_string + String(param->get_labels()[i]);

					if (i < param->get_labels().size() - 1)
					{
						hint_string = hint_string + ",";
					}
				}
			}
		}
		else
		{
			hint_string = String(String::num_real(parameter_description->get_minimum()) + "," +
					String::num_real(parameter_description->get_maximum()) + "," + String::num_real(step));
		}

		if (parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_LABELED)
		{
			p_list->push_back(PropertyInfo(flags, "value", PROPERTY_HINT_ENUM, hint_string, usage));
		}
		else
		{
			p_list->push_back(PropertyInfo(flags, "value", PROPERTY_HINT_RANGE, hint_string, usage));
		}
	}
}

bool StudioGlobalParameterTrigger::_property_can_revert(const StringName& p_name) const
{
	String name = String(p_name);

	if (parameter.is_valid())
	{
		if (name == "value")
		{
			return overridden_parameter.has("value");
		}
	}

	return false;
}

bool StudioGlobalParameterTrigger::_property_get_revert(const StringName& p_name, Variant& r_property) const
{
	String name = String(p_name);

	if (parameter.is_valid())
	{
		if (name == "value")
		{
			r_property = parameter->get_parameter_description()->get_default_value();
			return true;
		}
	}

	return false;
}

void StudioGlobalParameterTrigger::trigger()
{
	FMODStudioModule* fmod_module = FMODStudioModule::get_singleton();

	if (fmod_module)
	{
		if (parameter.is_valid())
		{
			String parameter_name = parameter->get_name();
			Variant parameter_value = overridden_parameter["value"];

			fmod_module->get_studio_system_ref()
					->set_parameter_by_name(parameter_name, parameter_value,
							false);
		}
	}
}

void StudioGlobalParameterTrigger::_enter_tree()
{
	handle_game_event(RuntimeUtils::GameEvent::GAMEEVENT_ENTER_TREE);
}

void StudioGlobalParameterTrigger::_ready()
{
	handle_game_event(RuntimeUtils::GameEvent::GAMEEVENT_READY);
}

void StudioGlobalParameterTrigger::_exit_tree()
{
	handle_game_event(RuntimeUtils::GameEvent::GAMEEVENT_EXIT_TREE);
}

void StudioGlobalParameterTrigger::handle_game_event(RuntimeUtils::GameEvent game_event)
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	if (trigger_on == game_event)
	{
		trigger();
	}
}

void StudioGlobalParameterTrigger::set_trigger_on(RuntimeUtils::GameEvent trigger_on)
{
	this->trigger_on = trigger_on;
}

RuntimeUtils::GameEvent StudioGlobalParameterTrigger::get_trigger_on() const
{
	return trigger_on;
}

void StudioGlobalParameterTrigger::set_parameter(const Ref<ParameterAsset>& parameter)
{
	this->parameter = parameter;

	if (Engine::get_singleton()->is_editor_hint())
	{
		if (_owner == nullptr)
		{
			return;
		}

		if (is_inside_tree())
		{
			if (parameter.is_valid())
			{
				if (!overridden_parameter.has("guid"))
				{
					overridden_parameter["guid"] = parameter->get_guid();
					overridden_parameter["value"] = parameter->get_parameter_description()->get_default_value();
				}

				if (overridden_parameter["guid"] != parameter->get_guid())
				{
					overridden_parameter.clear();
					overridden_parameter["guid"] = parameter->get_guid();
					overridden_parameter["value"] = parameter->get_parameter_description()->get_default_value();
				}
			}

			Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.001);
			timer->connect("timeout", Callable(this, "notify_property_list_changed"));
		}
	}
}

Ref<ParameterAsset> StudioGlobalParameterTrigger::get_parameter() const
{
	return parameter;
}

Dictionary StudioGlobalParameterTrigger::get_overridden_parameter() const
{
	return overridden_parameter;
}