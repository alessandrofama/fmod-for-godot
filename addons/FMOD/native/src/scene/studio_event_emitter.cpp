#include "studio_event_emitter.h"

using namespace godot;

template <typename T>
static inline std::vector<T*>& get_active_emitters()
{
	static std::vector<T*> active_emitters;
	return active_emitters;
}

template <typename T>
bool StudioEventEmitterImpl<T>::_set(const StringName& p_name, const Variant& p_value)
{
	String name = String(p_name);

	if (name.begins_with("parameter_"))
	{
		Ref<ParameterAsset> default_parameter;

		if (event.is_valid())
		{
			Dictionary parameters = event->get_parameters();

			String parameter_name = name.replace("parameter_", "");

			for (int i = 0; i < parameters.size(); i++)
			{
				Ref<ParameterAsset> p = parameters.values()[i];

				if (p->get_name() == parameter_name)
				{
					default_parameter = parameters.values()[i];
					break;
				}
			}

			if (p_value.get_type() == Variant::NIL)
			{
				if (default_parameter.is_valid())
				{
					overridden_parameters.erase(default_parameter->get_guid());
					if (node)
					{
						node->notify_property_list_changed();
					}
					return true;
				}
			}
			else
			{
				if (default_parameter.is_valid())
				{
					Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>
							id = default_parameter->get_parameter_description()->get_id();

					if (p_value == Variant(default_parameter->get_parameter_description()->get_default_value()))
					{
						if (overridden_parameters.has(default_parameter->get_guid()))
						{
							overridden_parameters.erase(default_parameter->get_guid());
						}

						if (event_instance.is_valid())
						{
							event_instance->set_parameter_by_id(id, p_value, false);
						}

						return true;
					}

					overridden_parameters[default_parameter->get_guid()] = p_value;

					if (event_instance.is_valid())
					{
						event_instance->set_parameter_by_id(id, p_value, false);
					}

					return true;
				}
			}
		}
	}
	return false;
}

template <typename T>
bool StudioEventEmitterImpl<T>::_get(const StringName& p_name, Variant& r_ret) const
{
	r_ret = Variant();
	String name = String(p_name);

	if (name.begins_with("parameter_"))
	{
		if (event.is_null())
		{
			return false;
		}

		Dictionary parameters = event->get_parameters();

		String parameter_name = name.replace("parameter_", "");
		Ref<ParameterAsset> default_parameter;

		for (int i = 0; i < parameters.size(); i++)
		{
			Ref<ParameterAsset> p = parameters.values()[i];

			if (p->get_name() == parameter_name)
			{
				default_parameter = parameters.values()[i];
				break;
			}
		}

		if (!default_parameter.is_valid())
		{
			return false;
		}

		if (overridden_parameters.has(default_parameter->get_guid()))
		{
			r_ret = overridden_parameters[default_parameter->get_guid()];
		}
		else
		{
			r_ret = default_parameter->get_parameter_description()->get_default_value();
		}

		return true;
	}

	return false;
}

template <typename T>
void StudioEventEmitterImpl<T>::_get_property_list(List<PropertyInfo>* p_list) const
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

	p_list->push_back(PropertyInfo(Variant::NIL, "Initial Parameter Values", PROPERTY_HINT_NONE, "parameter_",
			PROPERTY_USAGE_GROUP | PROPERTY_USAGE_SCRIPT_VARIABLE));

	if (parameters.size() > 0)
	{
		Array values = parameters.values();
		if (Engine::get_singleton()->is_editor_hint())
		{
			values.sort_custom(Callable(FMODStudioEditorModule::get_singleton(), "sort_parameters_by_name"));
		}

		for (int i = 0; i < values.size(); i++)
		{
			Ref<ParameterAsset> parameter = values[i];
			Variant::Type flags;
			float step{};

			if ((parameter->get_parameter_description()->get_flags() & FMOD_STUDIO_PARAMETER_DISCRETE) &&
					!(parameter->get_parameter_description()->get_flags() & FMOD_STUDIO_PARAMETER_LABELED))
			{
				flags = Variant::FLOAT;
				step = 1.0f;
			}
			else if (parameter->get_parameter_description()->get_flags() & FMOD_STUDIO_PARAMETER_LABELED)
			{
				flags = Variant::INT;
			}
			else
			{
				flags = Variant::FLOAT;
				step = 0.01;
			}

			uint32_t usage = PROPERTY_USAGE_EDITOR;

			if (overridden_parameters.has(parameter->get_guid()))
			{
				usage |= PROPERTY_USAGE_STORAGE;
			}

			String hint_string;

			if (parameter->get_parameter_description()->get_flags() & FMOD_STUDIO_PARAMETER_LABELED)
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
				hint_string = String(String::num_real(parameter->get_parameter_description()->get_minimum()) + "," +
						String::num_real(parameter->get_parameter_description()->get_maximum()) + "," +
						String::num_real(step));
			}

			String name = String("parameter_") + parameter->get_parameter_description()->get_name();

			if (parameter->get_parameter_description()->get_flags() & FMOD_STUDIO_PARAMETER_LABELED)
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

template <typename T>
bool StudioEventEmitterImpl<T>::_property_can_revert(const StringName& p_name) const
{
	String name = String(p_name);

	if (name.begins_with("parameter_"))
	{
		return true;
	}

	return false;
}

template <typename T>
bool StudioEventEmitterImpl<T>::_property_get_revert(const StringName& p_name, Variant& r_property) const
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
		Ref<ParameterAsset> default_parameter;

		for (int i = 0; i < parameters.size(); i++)
		{
			Ref<ParameterAsset> p = parameters.values()[i];

			if (p->get_name() == parameter_name)
			{
				default_parameter = parameters.values()[i];
				break;
			}
		}

		if (default_parameter.is_valid())
		{
			Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>
					id = default_parameter->get_parameter_description()->get_id();
			float default_parameter_value = default_parameter->get_parameter_description()->get_default_value();
			r_property = default_parameter_value;

			if (event_instance.is_valid())
			{
				event_instance->set_parameter_by_id(id, default_parameter_value, false);
			}

			return true;
		}
	}

	return false;
}

template <typename T>
void StudioEventEmitterImpl<T>::play_instance()
{
	bool instance_is_valid = event_instance.is_valid();
	if (instance_is_valid)
	{
		instance_is_valid = event_instance->is_valid();
	}

	if (instance_is_valid)
	{
		if (event->get_oneshot())
		{
			event_instance->release();
		}
	}

	if (!instance_is_valid)
	{
		event_instance = event_description->create_instance();

		if (event_instance->is_valid())
		{
			if (event->get_3d())
			{
				RuntimeUtils::to_3d_attributes_transform_physicsbody(attributes, node->get_global_transform(), rigidbody);
			}
			else
			{
				RuntimeUtils::to_3d_attributes_transform_physicsbody(attributes, Transform3D(), rigidbody);
			}

			event_instance->set_3d_attributes(attributes);

			for (int i = 0; i < overridden_parameters.size(); i++)
			{
				String guid = overridden_parameters.keys()[i];
				Variant value = overridden_parameters.values()[i];

				Dictionary parameters = event->get_parameters();

				if (parameters.has(guid))
				{
					Ref<ParameterAsset> asset = parameters[guid];
					if (asset.is_valid())
					{
						event_instance->set_parameter_by_id(asset->get_parameter_description()->get_id(), value, false);
					}
				}
			}
		}
	}

	event_instance->start();
	has_triggered = true;
}

template <typename T>
void StudioEventEmitterImpl<T>::stop_instance()
{
	if (trigger_once && has_triggered)
	{
		deregister_active_emitter(node);
	}

	if (event_instance.is_valid())
	{
		if (event_instance->is_valid())
		{
			event_instance->stop(allow_fadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
			event_instance->release();
		}
	}
}

template <typename T>
bool StudioEventEmitterImpl<T>::is_playing()
{
	if (event_instance.is_valid())
	{
		if (event_instance->is_valid())
		{
			return event_instance->get_playback_state() != FMOD_STUDIO_PLAYBACK_STOPPED;
		}
	}

	return false;
}

template <typename T>
void StudioEventEmitterImpl<T>::register_active_emitter(T* emitter)
{
	auto& active_emitters = get_active_emitters<T>();
	auto it = std::find(active_emitters.begin(), active_emitters.end(), emitter);
	if (it == active_emitters.end())
	{
		active_emitters.push_back(emitter);
	}
}

template <typename T>
void StudioEventEmitterImpl<T>::deregister_active_emitter(T* emitter)
{
	auto& active_emitters = get_active_emitters<T>();
	auto it = std::find(active_emitters.begin(), active_emitters.end(), emitter);
	if (it != active_emitters.end())
	{
		active_emitters.erase(it);
	}
}

template <typename T>
void StudioEventEmitterImpl<T>::update_playing_status(bool force)
{
	float max_distance = get_max_distance();
	bool should_play_instance{};

	Variant transform = node->get_global_transform();

	const Variant::Type type = transform.get_type();
	if (type == Variant::Type::TRANSFORM2D)
	{
		Transform2D t2d = transform;
		should_play_instance = ListenerImpl::distance_to_nearest_listener(t2d.get_origin()) <= max_distance * max_distance;
	}
	else if (type == Variant::Type::TRANSFORM3D)
	{
		Transform3D t3d = transform;
		should_play_instance = ListenerImpl::distance_to_nearest_listener(t3d.get_origin()) <= max_distance * max_distance;
	}

	if (force || should_play_instance != is_playing())
	{
		if (should_play_instance)
		{
			play_instance();
		}
		else
		{
			stop_instance();
		}
	}
}

template <>
void StudioEventEmitterImpl<StudioEventEmitter2D>::update_playing_status(bool force)
{
	float max_distance = get_max_distance();
	bool should_play_instance{};

	const Transform2D transform = node->get_global_transform();
	should_play_instance = ListenerImpl::distance_to_nearest_listener(transform.get_origin() / FMODStudioModule::get_singleton()->distance_scale_2d) <= max_distance * max_distance;

	if (force || should_play_instance != is_playing())
	{
		if (should_play_instance)
		{
			play_instance();
		}
		else
		{
			stop_instance();
		}
	}
}

template <>
void StudioEventEmitterImpl<StudioEventEmitter3D>::update_playing_status(bool force)
{
	float max_distance = get_max_distance();
	bool should_play_instance{};

	const Transform3D transform = node->get_global_transform();
	should_play_instance = ListenerImpl::distance_to_nearest_listener(transform.get_origin()) <= max_distance * max_distance;

	if (force || should_play_instance != is_playing())
	{
		if (should_play_instance)
		{
			play_instance();
		}
		else
		{
			stop_instance();
		}
	}
}

template <typename T>
void StudioEventEmitterImpl<T>::_enter_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	if (preload_samples)
	{
		lookup();

		if (event_description.is_valid())
		{
			event_description->load_sample_data();
		}
	}

	handle_game_event(RuntimeUtils::GameEvent::GAMEEVENT_ENTER_TREE);
}

template <typename T>
void StudioEventEmitterImpl<T>::_ready()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	handle_game_event(RuntimeUtils::GameEvent::GAMEEVENT_READY);
}

template <typename T>
void StudioEventEmitterImpl<T>::_exit_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	handle_game_event(RuntimeUtils::GameEvent::GAMEEVENT_EXIT_TREE);
}

template <typename T>
void StudioEventEmitterImpl<T>::_process(double p_delta)
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	if (event.is_valid())
	{
		if (event->get_3d())
		{
			RuntimeUtils::to_3d_attributes_transform_physicsbody(attributes, node->get_global_transform(), rigidbody);
		}
		else
		{
			RuntimeUtils::to_3d_attributes_transform_physicsbody(attributes, Transform3D(), rigidbody);
		}

		if (event_instance.is_valid())
		{
			if (event_instance->is_valid())
			{
				event_instance->set_3d_attributes(attributes);
			}
		}
	}
}

template <typename T>
void StudioEventEmitterImpl<T>::handle_game_event(RuntimeUtils::GameEvent game_event)
{
	if (play_event == game_event)
	{
		play();
	}
	if (stop_event == game_event)
	{
		stop();
	}
}

template <typename T>
void StudioEventEmitterImpl<T>::play()
{
	if (trigger_once && has_triggered)
	{
		return;
	}

	if (event.is_null())
	{
		return;
	}

	if (!event_description.is_valid())
	{
		lookup();
	}

	is_active = true;

	if (event->get_3d() && !event->get_oneshot())
	{
		register_active_emitter(node);
		update_playing_status(true);
	}
	else
	{
		play_instance();
	}
}

template <typename T>
void StudioEventEmitterImpl<T>::stop()
{
	deregister_active_emitter(node);
	is_active = false;
	stop_instance();
}

template <typename T>
void StudioEventEmitterImpl<T>::lookup()
{
	Ref<StudioApi::StudioSystem> studio_system = FMODStudioModule::get_singleton()->get_studio_system_ref();

	String guid = event.is_valid() ? event->get_guid() : "";

	if (studio_system.is_valid())
	{
		event_description = studio_system->get_event_by_id(guid);
	}
}

template <typename T>
float StudioEventEmitterImpl<T>::get_max_distance()
{
	if (event_description.is_valid())
	{
		if (!event_description->is_valid())
		{
			lookup();
		}

		Dictionary min_max_distance = event_description->get_min_max_distance();

		if (min_max_distance.has("max"))
		{
			return min_max_distance["max"];
		}
	}

	return 0.0f;
}

void StudioEventEmitter2D::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &StudioEventEmitter2D::handle_game_event);
	ClassDB::bind_method(D_METHOD("play"), &StudioEventEmitter2D::play);
	ClassDB::bind_method(D_METHOD("stop"), &StudioEventEmitter2D::stop);
	ClassDB::bind_method(D_METHOD("lookup"), &StudioEventEmitter2D::lookup);
	ClassDB::bind_method(D_METHOD("set_play_event", "play_event"), &StudioEventEmitter2D::set_play_event);
	ClassDB::bind_method(D_METHOD("get_play_event"), &StudioEventEmitter2D::get_play_event);
	ClassDB::bind_method(D_METHOD("set_stop_event", "stop_event"), &StudioEventEmitter2D::set_stop_event);
	ClassDB::bind_method(D_METHOD("get_stop_event"), &StudioEventEmitter2D::get_stop_event);
	ClassDB::bind_method(D_METHOD("set_event", "event"), &StudioEventEmitter2D::set_event);
	ClassDB::bind_method(D_METHOD("get_event"), &StudioEventEmitter2D::get_event);
	ClassDB::bind_method(D_METHOD("set_preload_samples", "preload_samples"),
			&StudioEventEmitter2D::set_preload_samples);
	ClassDB::bind_method(D_METHOD("get_preload_samples"), &StudioEventEmitter2D::get_preload_samples);
	ClassDB::bind_method(D_METHOD("set_allow_fadeout", "allow_fadeout"), &StudioEventEmitter2D::set_allow_fadeout);
	ClassDB::bind_method(D_METHOD("get_allow_fadeout"), &StudioEventEmitter2D::get_allow_fadeout);
	ClassDB::bind_method(D_METHOD("set_trigger_once", "trigger_once"), &StudioEventEmitter2D::set_trigger_once);
	ClassDB::bind_method(D_METHOD("get_trigger_once"), &StudioEventEmitter2D::get_trigger_once);
	ClassDB::bind_method(D_METHOD("set_rigidbody", "rigidbody"), &StudioEventEmitter2D::set_rigidbody);
	ClassDB::bind_method(D_METHOD("get_rigidbody"), &StudioEventEmitter2D::get_rigidbody);
	ClassDB::bind_method(D_METHOD("set_overridden_parameters", "overridden_parameters"),
			&StudioEventEmitter2D::set_overridden_parameters);
	ClassDB::bind_method(D_METHOD("get_overridden_parameters"), &StudioEventEmitter2D::get_overridden_parameters);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "play_event", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_play_event", "get_play_event");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stop_event", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_stop_event", "get_stop_event");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "EventAsset"), "set_event",
			"get_event");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preload_samples"), "set_preload_samples", "get_preload_samples");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_fadeout"), "set_allow_fadeout", "get_allow_fadeout");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "trigger_once"), "set_trigger_once", "get_trigger_once");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rigidbody", PROPERTY_HINT_NODE_TYPE, "PhysicsBody2D"), "set_rigidbody",
			"get_rigidbody");
	ADD_SIGNAL(MethodInfo("event_changed", PropertyInfo(Variant::STRING, "value")));
}

bool StudioEventEmitter2D::_set(const StringName& p_name, const Variant& p_value)
{
	return implementation._set(p_name, p_value);
}

bool StudioEventEmitter2D::_get(const StringName& p_name, Variant& r_ret) const
{
	return implementation._get(p_name, r_ret);
}

void StudioEventEmitter2D::_get_property_list(List<PropertyInfo>* p_list) const
{
	implementation._get_property_list(p_list);
}

bool StudioEventEmitter2D::_property_can_revert(const StringName& p_name) const
{
	return implementation._property_can_revert(p_name);
}

bool StudioEventEmitter2D::_property_get_revert(const StringName& p_name, Variant& r_property) const
{
	return implementation._property_get_revert(p_name, r_property);
}

void StudioEventEmitter2D::_enter_tree()
{
	implementation.node = this;
	implementation._enter_tree();
}

void StudioEventEmitter2D::_ready()
{
	implementation._ready();
}

void StudioEventEmitter2D::_exit_tree()
{
	implementation._exit_tree();
}

void StudioEventEmitter2D::_process(double p_delta)
{
	implementation._process(p_delta);
}

void StudioEventEmitter2D::handle_game_event(RuntimeUtils::GameEvent game_event)
{
	implementation.handle_game_event(game_event);
}

void StudioEventEmitter2D::play()
{
	implementation.play();
}

void StudioEventEmitter2D::stop()
{
	implementation.stop();
}

void StudioEventEmitter2D::lookup()
{
	implementation.lookup();
}

float StudioEventEmitter2D::get_max_distance()
{
	return implementation.get_max_distance();
}

void StudioEventEmitter2D::set_play_event(RuntimeUtils::GameEvent play_event)
{
	implementation.play_event = play_event;
}

RuntimeUtils::GameEvent StudioEventEmitter2D::get_play_event() const
{
	return implementation.play_event;
}

void StudioEventEmitter2D::set_stop_event(RuntimeUtils::GameEvent stop_event)
{
	implementation.stop_event = stop_event;
}

RuntimeUtils::GameEvent StudioEventEmitter2D::get_stop_event() const
{
	return implementation.stop_event;
}

void StudioEventEmitter2D::set_event(const Ref<EventAsset>& event)
{
	Ref<EventAsset> new_event = event;

	if (implementation.event != new_event)
	{
		implementation.overridden_parameters.clear();

		implementation.event = new_event;

		if (is_inside_tree())
		{
			emit_signal("event_changed", this);
		}

		if (Engine::get_singleton()->is_editor_hint())
		{
			if (_owner == nullptr)
			{
				return;
			}

			if (!is_inside_tree())
			{
				return;
			}

			Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.001);
			timer->connect("timeout", Callable(this, "notify_property_list_changed"));
		}
	}
}

Ref<EventAsset> StudioEventEmitter2D::get_event() const
{
	return implementation.event;
}

void StudioEventEmitter2D::set_preload_samples(bool preload_samples)
{
	implementation.preload_samples = preload_samples;
}

bool StudioEventEmitter2D::get_preload_samples() const
{
	return implementation.preload_samples;
}

void StudioEventEmitter2D::set_allow_fadeout(bool allow_fadeout)
{
	implementation.allow_fadeout = allow_fadeout;
}

bool StudioEventEmitter2D::get_allow_fadeout() const
{
	return implementation.allow_fadeout;
}

void StudioEventEmitter2D::set_trigger_once(bool trigger_once)
{
	implementation.trigger_once = trigger_once;
}

bool StudioEventEmitter2D::get_trigger_once() const
{
	return implementation.trigger_once;
}

void StudioEventEmitter2D::set_rigidbody(Object* rigidbody)
{
	// todo(alex): check if this actually works
	implementation.rigidbody = Object::cast_to<PhysicsBody2D>(rigidbody);
}

Object* StudioEventEmitter2D::get_rigidbody() const
{
	return implementation.rigidbody;
}

void StudioEventEmitter2D::set_overridden_parameters(const Dictionary& overridden_parameters)
{
	implementation.overridden_parameters = overridden_parameters;
}

Dictionary StudioEventEmitter2D::get_overridden_parameters() const
{
	return implementation.overridden_parameters;
}

void StudioEventEmitter3D::_bind_methods()
{
	ClassDB::bind_static_method("StudioEventEmitter3D", D_METHOD("update_active_emitters"),
			&StudioEventEmitter3D::update_active_emitters);
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &StudioEventEmitter3D::handle_game_event);
	ClassDB::bind_method(D_METHOD("play"), &StudioEventEmitter3D::play);
	ClassDB::bind_method(D_METHOD("stop"), &StudioEventEmitter3D::stop);
	ClassDB::bind_method(D_METHOD("lookup"), &StudioEventEmitter3D::lookup);
	ClassDB::bind_method(D_METHOD("set_play_event", "play_event"), &StudioEventEmitter3D::set_play_event);
	ClassDB::bind_method(D_METHOD("get_play_event"), &StudioEventEmitter3D::get_play_event);
	ClassDB::bind_method(D_METHOD("set_stop_event", "stop_event"), &StudioEventEmitter3D::set_stop_event);
	ClassDB::bind_method(D_METHOD("get_stop_event"), &StudioEventEmitter3D::get_stop_event);
	ClassDB::bind_method(D_METHOD("set_event", "event"), &StudioEventEmitter3D::set_event);
	ClassDB::bind_method(D_METHOD("get_event"), &StudioEventEmitter3D::get_event);
	ClassDB::bind_method(D_METHOD("set_preload_samples", "preload_samples"),
			&StudioEventEmitter3D::set_preload_samples);
	ClassDB::bind_method(D_METHOD("get_preload_samples"), &StudioEventEmitter3D::get_preload_samples);
	ClassDB::bind_method(D_METHOD("set_allow_fadeout", "allow_fadeout"), &StudioEventEmitter3D::set_allow_fadeout);
	ClassDB::bind_method(D_METHOD("get_allow_fadeout"), &StudioEventEmitter3D::get_allow_fadeout);
	ClassDB::bind_method(D_METHOD("set_trigger_once", "trigger_once"), &StudioEventEmitter3D::set_trigger_once);
	ClassDB::bind_method(D_METHOD("get_trigger_once"), &StudioEventEmitter3D::get_trigger_once);
	ClassDB::bind_method(D_METHOD("set_rigidbody", "rigidbody"), &StudioEventEmitter3D::set_rigidbody);
	ClassDB::bind_method(D_METHOD("get_rigidbody"), &StudioEventEmitter3D::get_rigidbody);
	ClassDB::bind_method(D_METHOD("set_overridden_parameters", "overridden_parameters"),
			&StudioEventEmitter3D::set_overridden_parameters);
	ClassDB::bind_method(D_METHOD("get_overridden_parameters"), &StudioEventEmitter3D::get_overridden_parameters);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "play_event", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_play_event", "get_play_event");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stop_event", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_stop_event", "get_stop_event");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "EventAsset"), "set_event",
			"get_event");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preload_samples"), "set_preload_samples", "get_preload_samples");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_fadeout"), "set_allow_fadeout", "get_allow_fadeout");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "trigger_once"), "set_trigger_once", "get_trigger_once");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rigidbody", PROPERTY_HINT_NODE_TYPE, "PhysicsBody3D"), "set_rigidbody",
			"get_rigidbody");
	ADD_SIGNAL(MethodInfo("event_changed", PropertyInfo(Variant::STRING, "value")));
}

bool StudioEventEmitter3D::_set(const StringName& p_name, const Variant& p_value)
{
	return implementation._set(p_name, p_value);
}

bool StudioEventEmitter3D::_get(const StringName& p_name, Variant& r_ret) const
{
	return implementation._get(p_name, r_ret);
}

void StudioEventEmitter3D::_get_property_list(List<PropertyInfo>* p_list) const
{
	implementation._get_property_list(p_list);
}

bool StudioEventEmitter3D::_property_can_revert(const StringName& p_name) const
{
	return implementation._property_can_revert(p_name);
}

bool StudioEventEmitter3D::_property_get_revert(const StringName& p_name, Variant& r_property) const
{
	return implementation._property_get_revert(p_name, r_property);
}

void StudioEventEmitter3D::update_active_emitters()
{
	std::vector<StudioEventEmitter2D*>& emitters_2d = get_active_emitters<StudioEventEmitter2D>();
	for (size_t i = 0; i < emitters_2d.size(); i++)
	{
		StudioEventEmitter2D* emitter_2d = emitters_2d[i];
		if (emitter_2d)
		{
			emitter_2d->implementation.update_playing_status();
		}
	}

	std::vector<StudioEventEmitter3D*>& emitters_3d = get_active_emitters<StudioEventEmitter3D>();
	for (size_t i = 0; i < emitters_3d.size(); i++)
	{
		StudioEventEmitter3D* emitter_3d = emitters_3d[i];
		if (emitter_3d)
		{
			emitter_3d->implementation.update_playing_status();
		}
	}
}

void StudioEventEmitter3D::_enter_tree()
{
	implementation.node = this;
	implementation._enter_tree();

	if (Engine::get_singleton()->is_editor_hint())
	{
		update_gizmos();
	}
}

void StudioEventEmitter3D::_ready()
{
	implementation._ready();
}

void StudioEventEmitter3D::_exit_tree()
{
	implementation._exit_tree();
}

void StudioEventEmitter3D::_process(double p_delta)
{
	implementation._process(p_delta);
}

void StudioEventEmitter3D::handle_game_event(RuntimeUtils::GameEvent game_event)
{
	implementation.handle_game_event(game_event);
}

void StudioEventEmitter3D::play()
{
	implementation.play();
}

void StudioEventEmitter3D::stop()
{
	implementation.stop();
}

void StudioEventEmitter3D::lookup()
{
	implementation.lookup();
}

float StudioEventEmitter3D::get_max_distance()
{
	return implementation.get_max_distance();
}

void StudioEventEmitter3D::set_play_event(RuntimeUtils::GameEvent play_event)
{
	implementation.play_event = play_event;
}

RuntimeUtils::GameEvent StudioEventEmitter3D::get_play_event() const
{
	return implementation.play_event;
}

void StudioEventEmitter3D::set_stop_event(RuntimeUtils::GameEvent stop_event)
{
	implementation.stop_event = stop_event;
}

RuntimeUtils::GameEvent StudioEventEmitter3D::get_stop_event() const
{
	return implementation.stop_event;
}

void StudioEventEmitter3D::set_event(const Ref<EventAsset>& event)
{
	Ref<EventAsset> new_event = event;

	if (implementation.event != new_event)
	{
		implementation.overridden_parameters.clear();

		implementation.event = new_event;

		if (is_inside_tree())
		{
			emit_signal("event_changed", this);
		}

		if (Engine::get_singleton()->is_editor_hint())
		{
			if (_owner == nullptr)
			{
				return;
			}

			if (!is_inside_tree())
			{
				return;
			}

			Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.001);
			timer->connect("timeout", Callable(this, "notify_property_list_changed"));

			update_gizmos();
		}
	}
}

Ref<EventAsset> StudioEventEmitter3D::get_event() const
{
	return implementation.event;
}

void StudioEventEmitter3D::set_preload_samples(bool preload_samples)
{
	implementation.preload_samples = preload_samples;
}

bool StudioEventEmitter3D::get_preload_samples() const
{
	return implementation.preload_samples;
}

void StudioEventEmitter3D::set_allow_fadeout(bool allow_fadeout)
{
	implementation.allow_fadeout = allow_fadeout;
}

bool StudioEventEmitter3D::get_allow_fadeout() const
{
	return implementation.allow_fadeout;
}

void StudioEventEmitter3D::set_trigger_once(bool trigger_once)
{
	implementation.trigger_once = trigger_once;
}

bool StudioEventEmitter3D::get_trigger_once() const
{
	return implementation.trigger_once;
}

void StudioEventEmitter3D::set_rigidbody(Object* rigidbody)
{
	// todo(alex): check if this actually works
	implementation.rigidbody = Object::cast_to<PhysicsBody3D>(rigidbody);
}

Object* StudioEventEmitter3D::get_rigidbody() const
{
	return implementation.rigidbody;
}

void StudioEventEmitter3D::set_overridden_parameters(const Dictionary& overridden_parameters)
{
	implementation.overridden_parameters = overridden_parameters;
}

Dictionary StudioEventEmitter3D::get_overridden_parameters() const
{
	return implementation.overridden_parameters;
}