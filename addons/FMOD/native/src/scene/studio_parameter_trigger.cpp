#include "studio_parameter_trigger.h"

using namespace godot;

void StudioParameterTrigger::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &StudioParameterTrigger::handle_game_event);
	ClassDB::bind_method(D_METHOD("trigger"), &StudioParameterTrigger::trigger);
	ClassDB::bind_method(D_METHOD("set_studio_event_emitter", "path"),
			&StudioParameterTrigger::set_studio_event_emitter);
	ClassDB::bind_method(D_METHOD("get_studio_event_emitter"), &StudioParameterTrigger::get_studio_event_emitter);
	ClassDB::bind_method(D_METHOD("set_trigger_on", "trigger_on"), &StudioParameterTrigger::set_trigger_on);
	ClassDB::bind_method(D_METHOD("get_trigger_on"), &StudioParameterTrigger::get_trigger_on);
	ClassDB::bind_method(D_METHOD("set_event"), &StudioParameterTrigger::set_event);
	ClassDB::bind_method(D_METHOD("get_event"), &StudioParameterTrigger::get_event);
	ClassDB::bind_method(D_METHOD("on_event_changed", "value"), &StudioParameterTrigger::on_event_changed);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "trigger_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_trigger_on", "get_trigger_on");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "studio_event_emitter", PROPERTY_HINT_NODE_PATH_VALID_TYPES,
						 "StudioEventEmitter3D,StudioEventEmitter2D", PROPERTY_USAGE_DEFAULT),
			"set_studio_event_emitter", "get_studio_event_emitter");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "EventAsset", PROPERTY_USAGE_STORAGE), "set_event",
			"get_event");
}

bool StudioParameterTrigger::_set(const StringName& p_name, const Variant& p_value)
{
	String name = String(p_name);

	if (name.begins_with("parameter_"))
	{
		if (event.is_valid())
		{
			Ref<ParameterAsset> parameter;
			Dictionary parameters = event->get_parameters();

			String parameter_name = name.replace("parameter_", "");

			for (int64_t i = 0; i < parameters.size(); i++)
			{
				Ref<ParameterAsset> p = parameters.values()[i];

				if (p->get_name() == parameter_name)
				{
					parameter = parameters.values()[i];
					break;
				}
			}

			if (p_value.get_type() == Variant::NIL)
			{
				if (name.begins_with("parameter_"))
				{
					if (parameter.is_valid())
					{
						String guid = parameter->get_guid();
						overridden_parameters.erase(guid);
						notify_property_list_changed();
					}
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (parameter.is_valid())
				{
					overridden_parameters[parameter->get_guid()] = p_value;
					return true;
				}
			}
		}
	}

	return false;
}

bool StudioParameterTrigger::_get(const StringName& p_name, Variant& r_ret) const
{
	String name = String(p_name);

	if (name.begins_with("parameter_"))
	{
		if (event.is_null())
		{
			return false;
		}

		Dictionary parameters = event->get_parameters();

		String parameter_name = name.replace("parameter_", "");
		Ref<ParameterAsset> parameter;

		for (int64_t i = 0; i < parameters.size(); i++)
		{
			Ref<ParameterAsset> p = parameters.values()[i];

			if (p->get_name() == parameter_name)
			{
				parameter = parameters.values()[i];
				break;
			}
		}

		if (!parameter.is_valid())
		{
			return false;
		}

		if (overridden_parameters.has(parameter->get_guid()))
		{
			r_ret = overridden_parameters[parameter->get_guid()];
		}
		else
		{
			r_ret = parameter->get_parameter_description()->get_default_value();
		}

		return true;
	}

	return false;
}

void StudioParameterTrigger::_get_property_list(List<PropertyInfo>* p_list) const
{
	if (event.is_null())
	{
		return;
	}

	Dictionary parameters = event->get_parameters();

	if (parameters.is_empty())
	{
		return;
	}

	p_list->push_back(PropertyInfo(Variant::NIL, "Parameters", PROPERTY_HINT_NONE, "parameter_",
			PROPERTY_USAGE_GROUP | PROPERTY_USAGE_SCRIPT_VARIABLE));

	if (parameters.size() > 0)
	{
		Array values = parameters.values();
		if (Engine::get_singleton()->is_editor_hint())
		{
			values.sort_custom(Callable(FMODStudioEditorModule::get_singleton(), "sort_parameters_by_name"));
		}

		for (int64_t i = 0; i < values.size(); i++)
		{
			Variant::Type flags;
			float step{};

			Ref<ParameterAsset> parameter = values[i];
			Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> parameter_description =
					parameter->get_parameter_description();

			if (parameter_description.is_null())
			{
				return;
			}

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

			if (overridden_parameters.has(parameter->get_guid()))
			{
				usage |= PROPERTY_USAGE_STORAGE;
			}

			String hint_string;

			if (parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_LABELED)
			{
				if (parameter->get_labels().size() > 0)
				{
					for (int j = 0; j < parameter->get_labels().size(); j++)
					{
						hint_string = hint_string + String(parameter->get_labels()[j]);

						if (j < parameter->get_labels().size() - 1)
						{
							hint_string = hint_string + ",";
						}
					}
				}
			}
			else
			{
				hint_string =
						String(String::num_real(parameter_description->get_minimum()) + "," +
								String::num_real(parameter_description->get_maximum()) + "," + String::num_real(step));
			}

			String name = String("parameter_") + parameter_description->get_name();

			if (parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_LABELED)
			{
				p_list->push_back(PropertyInfo(flags, name, PROPERTY_HINT_ENUM, hint_string, usage));
			}
			else
			{
				p_list->push_back(PropertyInfo(flags, name, PROPERTY_HINT_RANGE, hint_string, usage));
			}
		}
	}
}

bool StudioParameterTrigger::_property_can_revert(const StringName& p_name) const
{
	String name = String(p_name);
	if (name.begins_with("parameter_"))
	{
		return true;
	}
	return false;
}

bool StudioParameterTrigger::_property_get_revert(const StringName& p_name, Variant& r_property) const
{
	String name = String(p_name);

	if (name.begins_with("parameter_"))
	{
		if (event.is_null())
		{
			return false;
		}

		Dictionary parameters = event->get_parameters();

		String parameter_name = name.replace("parameter_", "");
		Ref<ParameterAsset> parameter;

		for (int64_t i = 0; i < parameters.size(); i++)
		{
			Ref<ParameterAsset> p = parameters.values()[i];

			if (p->get_name() == parameter_name)
			{
				parameter = parameters.values()[i];
				break;
			}
		}

		if (parameter.is_valid())
		{
			Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>
					id = parameter->get_parameter_description()->get_id();
			float default_parameter_value = parameter->get_parameter_description()->get_default_value();
			r_property = default_parameter_value;
			return true;
		}
	}

	return false;
}

void StudioParameterTrigger::reset()
{
	overridden_parameters = Dictionary();
	event = Ref<EventAsset>();
}

void StudioParameterTrigger::_enter_tree()
{
	handle_game_event(RuntimeUtils::GameEvent::GAMEEVENT_ENTER_TREE);
}

void StudioParameterTrigger::_ready()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		if (is_inside_tree())
		{
			Node* node = get_node_or_null(studio_event_emitter);

			if (!node)
			{
				return;
			}

			if (node->has_signal("event_changed"))
			{
				if (!node->is_connected("event_changed", Callable(this, "on_event_changed")))
				{
					node->connect("event_changed", Callable(this, "on_event_changed"));
				}
			}
		}
	}

	handle_game_event(RuntimeUtils::GameEvent::GAMEEVENT_READY);
}

void StudioParameterTrigger::_exit_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		if (is_inside_tree())
		{
			Node* node = get_node_or_null(studio_event_emitter);

			if (!node)
			{
				return;
			}

			if (node->has_signal("event_changed"))
			{
				if (node->is_connected("event_changed", Callable(this, "on_event_changed")))
				{
					node->disconnect("event_changed", Callable(this, "on_event_changed"));
				}
			}
		}
	}

	handle_game_event(RuntimeUtils::GameEvent::GAMEEVENT_EXIT_TREE);
}

void StudioParameterTrigger::handle_game_event(RuntimeUtils::GameEvent game_event)
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

void StudioParameterTrigger::trigger()
{
	Node* node = this->get_node_or_null(studio_event_emitter);

	StudioEventEmitter3D* emitter_3d = nullptr;
	StudioEventEmitter2D* emitter_2d = nullptr;

	if (node)
	{
		if (node->get_class() == "StudioEventEmitter3D")
		{
			emitter_3d = Object::cast_to<StudioEventEmitter3D>(node);
		}
		else if (node->get_class() == "StudioEventEmitter2D")
		{
			emitter_2d = Object::cast_to<StudioEventEmitter2D>(node);
		}

		if (emitter_3d)
		{
			if (emitter_3d->implementation.event_instance.is_valid())
			{
				for (int64_t i = 0; i < overridden_parameters.size(); i++)
				{
					Variant parameter_name = overridden_parameters.keys()[i];
					Variant parameter_value = overridden_parameters.values()[i];

					emitter_3d->implementation.event_instance->set_parameter_by_name(parameter_name, parameter_value,
							false);
				}
			}
		}
		else if (emitter_2d)
		{
			if (emitter_2d->implementation.event_instance.is_valid())
			{
				for (int64_t i = 0; i < overridden_parameters.size(); i++)
				{
					Variant parameter_name = overridden_parameters.keys()[i];
					Variant parameter_value = overridden_parameters.values()[i];

					emitter_2d->implementation.event_instance->set_parameter_by_name(parameter_name, parameter_value,
							false);
				}
			}
		}
	}
}

void StudioParameterTrigger::set_studio_event_emitter(const Variant& path)
{
	StudioEventEmitter3D* emitter_3d = nullptr;
	StudioEventEmitter2D* emitter_2d = nullptr;

	if (is_inside_tree())
	{
		NodePath n_path = path;
		Node* node = get_node_or_null(n_path);
		emitter_3d = Object::cast_to<StudioEventEmitter3D>(node);
		emitter_2d = Object::cast_to<StudioEventEmitter2D>(node);

		if (emitter_3d || emitter_2d)
		{
			if (studio_event_emitter != n_path || event_changed)
			{
				if (emitter_3d)
				{
					if (emitter_3d->get_event().is_valid())
					{
						reset();
						event = emitter_3d->get_event();
					}
					else
					{
						reset();
					}
				}
				else if (emitter_2d)
				{
					if (emitter_2d->get_event().is_valid())
					{
						reset();
						event = emitter_2d->get_event();
					}
					else
					{
						reset();
					}
				}
			}
			if (event_changed)
			{
				event_changed = false;
			}
		}
		else
		{
			reset();
		}
	}

	studio_event_emitter = path;

	if (Engine::get_singleton()->is_editor_hint())
	{
		if (_owner == nullptr)
		{
			return;
		}

		if (is_inside_tree())
		{
			if (event.is_valid())
			{
				Dictionary parameters = event->get_parameters();
				for (int64_t i = 0; i < parameters.size(); i++)
				{
					Ref<ParameterAsset> parameter = parameters.values()[i];
					if (parameter.is_null())
					{
						break;
					}

					if (!overridden_parameters.has(parameter->get_guid()))
					{
						overridden_parameters[parameter->get_guid()] = parameter->get_parameter_description()->get_default_value();
					}
				}
			}

			Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.001);
			timer->connect("timeout", Callable(this, "notify_property_list_changed"));
		}

		return;
	}
}

NodePath StudioParameterTrigger::get_studio_event_emitter() const
{
	return studio_event_emitter;
}

void StudioParameterTrigger::set_trigger_on(RuntimeUtils::GameEvent trigger_on)
{
	this->trigger_on = trigger_on;
}

RuntimeUtils::GameEvent StudioParameterTrigger::get_trigger_on() const
{
	return trigger_on;
}

void StudioParameterTrigger::set_event(const Ref<EventAsset>& event_asset)
{
	event = event_asset;
}

Ref<EventAsset> StudioParameterTrigger::get_event()
{
	return event;
}

void StudioParameterTrigger::on_event_changed(Object* value)
{
	Node* emitter = Object::cast_to<Node>(value);
	NodePath path;
	if (emitter)
	{
		path = get_path_to(emitter);
	}
	event_changed = true;
	reset();

	if (Engine::get_singleton()->is_editor_hint())
	{
		if (_owner == nullptr)
		{
			return;
		}

		Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.001);
		timer->connect("timeout", Callable(this, "notify_property_list_changed"));
	}

	if (!path.is_empty())
	{
		set_studio_event_emitter(path);
	}
	else
	{
		set_studio_event_emitter(NodePath());
	}
}