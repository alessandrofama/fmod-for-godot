#include "studio_listener.h"

using namespace godot;

static std::vector<ListenerImpl*> listeners;
static const int MAX_LISTENERS = 8;

int ListenerImpl::get_num_listener()
{
	int index = -1;
	auto it = std::find(listeners.begin(), listeners.end(), this);
	if (it != listeners.end())
	{
		index = std::distance(listeners.begin(), it);
	}

	return index;
}

void ListenerImpl::add_listener()
{
	for (size_t i = 0; i < listeners.size(); i++)
	{
		if (listeners[i] == this)
		{
			UtilityFunctions::push_warning("[FMOD] Listener has already been added.");
			return;
		}
	}

	if (listeners.size() >= MAX_LISTENERS)
	{
		UtilityFunctions::push_warning("[FMOD] Max number of listeners reached.");
	}

	listeners.push_back(this);

	FMODStudioModule* fmod_module = FMODStudioModule::get_singleton();

	if (fmod_module)
	{
		fmod_module->get_studio_system_ref()
				->set_num_listeners(
						Math::clamp(static_cast<int>(listeners.size()), 1, MAX_LISTENERS));
	}
}

void ListenerImpl::remove_listener()
{
	for (size_t i = 0; i < listeners.size(); i++)
	{
		if (listeners[i] == this)
		{
			listeners.erase(listeners.begin() + i);
			break;
		}
	}

	FMODStudioModule* fmod_module = FMODStudioModule::get_singleton();

	if (fmod_module)
	{
		fmod_module->get_studio_system_ref()
				->set_num_listeners(
						Math::clamp(static_cast<int>(listeners.size()), 1, MAX_LISTENERS));
	}
}

int ListenerImpl::get_listener_count()
{
	return listeners.size();
}

void ListenerImpl::set_listener_location()
{
	int listener_index = get_num_listener();

	if (node_2d)
	{
		Node2D* attenuation_node = Object::cast_to<Node2D>(attenuation_object);
		Variant transform = node_2d->get_global_transform();

		if (attenuation_node)
		{
			set_listener_attributes(attenuation_node->get_global_transform().get_origin(), listener_index, transform, rigidbody);
		}
		else
		{
			set_listener_attributes(Variant(), listener_index, transform, rigidbody);
		}
	}
	else if (node_3d)
	{
		Node3D* attenuation_node = Object::cast_to<Node3D>(attenuation_object);
		Variant transform = node_3d->get_global_transform();
		if (attenuation_node)
		{
			set_listener_attributes(attenuation_node->get_global_transform().get_origin(), listener_index, transform, rigidbody);
		}
		else
		{
			set_listener_attributes(Variant(), listener_index, transform, rigidbody);
		}
	}
}

void ListenerImpl::set_listener_attributes(const Variant& attenuation, int num_listener, const Variant& transform, Object* rigidbody)
{
	RuntimeUtils::to_3d_attributes_transform_physicsbody(attributes, transform, rigidbody);

	FMODStudioModule* fmod_module = FMODStudioModule::get_singleton();
	if (fmod_module)
	{
		fmod_module->get_studio_system_ref()->set_listener_attributes(num_listener, attributes, attenuation);
	}
}

float ListenerImpl::distance_to_nearest_listener(const Variant& position)
{
	float result = FLT_MAX;

	for (size_t i = 0; i < listeners.size(); i++)
	{
		ListenerImpl* listener_impl = listeners[i];

		if (listener_impl)
		{
			if (listener_impl->node_2d)
			{
				Transform2D t2d = listener_impl->node_2d->get_global_transform();
				Vector2 origin = t2d.get_origin() / FMODStudioModule::get_singleton()->distance_scale_2d;
				result = MIN(result, origin.distance_to(position));
			}
			else if (listener_impl->node_3d)
			{
				Transform3D t3d = listener_impl->node_3d->get_global_transform();
				result = MIN(result, t3d.get_origin().distance_to(position));
			}
		}
	}

	return result;
}

void ListenerImpl::_enter_tree()
{
	add_listener();
}

void ListenerImpl::_exit_tree()
{
	remove_listener();
}

void ListenerImpl::_process(double p_delta)
{
	if (get_num_listener() >= 0 && get_num_listener() < MAX_LISTENERS)
	{
		set_listener_location();
	}
}

void StudioListener2D::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_attenuation_object", "object"), &StudioListener2D::set_attenuation_object);
	ClassDB::bind_method(D_METHOD("get_attenuation_object"), &StudioListener2D::get_attenuation_object);
	ClassDB::bind_method(D_METHOD("set_rigidbody", "object"), &StudioListener2D::set_rigidbody);
	ClassDB::bind_method(D_METHOD("get_rigidbody"), &StudioListener2D::get_rigidbody);
	ClassDB::bind_method(D_METHOD("set_num_listener", "listener"), &StudioListener2D::set_num_listener);
	ClassDB::bind_method(D_METHOD("get_num_listener"), &StudioListener2D::get_num_listener);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "attenuation_object", PROPERTY_HINT_NODE_TYPE, "Node2D"),
			"set_attenuation_object", "get_attenuation_object");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rigidbody", PROPERTY_HINT_NODE_TYPE, "PhysicsBody2D"), "set_rigidbody",
			"get_rigidbody");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_listener", PROPERTY_HINT_NONE, "",
						 PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_READ_ONLY),
			"set_num_listener", "get_num_listener");
}

void StudioListener2D::_enter_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	implementation.node_2d = this;
	implementation._enter_tree();
}

void StudioListener2D::_exit_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	implementation._exit_tree();
}

void StudioListener2D::_process(double p_delta)
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	implementation._process(p_delta);
}

float StudioListener2D::distance_to_nearest_listener(const Vector2& position)
{
	return ListenerImpl::distance_to_nearest_listener(position);
}

void StudioListener2D::set_attenuation_object(Object* object)
{
	implementation.attenuation_object = object;
}

Object* StudioListener2D::get_attenuation_object() const
{
	return implementation.attenuation_object;
}

void StudioListener2D::set_rigidbody(Object* object)
{
	implementation.rigidbody = object;
}

Object* StudioListener2D::get_rigidbody() const
{
	return implementation.rigidbody;
}

void StudioListener2D::set_num_listener(int num)
{
	notify_property_list_changed();
}

int StudioListener2D::get_num_listener()
{
	return implementation.get_num_listener();
}

void StudioListener3D::_bind_methods()
{
	ClassDB::bind_static_method("StudioListener3D", D_METHOD("get_listener_count"), &StudioListener3D::get_listener_count);
	ClassDB::bind_method(D_METHOD("set_attenuation_object", "object"), &StudioListener3D::set_attenuation_object);
	ClassDB::bind_method(D_METHOD("get_attenuation_object"), &StudioListener3D::get_attenuation_object);
	ClassDB::bind_method(D_METHOD("set_rigidbody", "object"), &StudioListener3D::set_rigidbody);
	ClassDB::bind_method(D_METHOD("get_rigidbody"), &StudioListener3D::get_rigidbody);
	ClassDB::bind_method(D_METHOD("set_num_listener", "listener"), &StudioListener3D::set_num_listener);
	ClassDB::bind_method(D_METHOD("get_num_listener"), &StudioListener3D::get_num_listener);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "attenuation_object", PROPERTY_HINT_NODE_TYPE, "Node3D"),
			"set_attenuation_object", "get_attenuation_object");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rigidbody", PROPERTY_HINT_NODE_TYPE, "PhysicsBody3D"), "set_rigidbody",
			"get_rigidbody");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_listener", PROPERTY_HINT_NONE, "",
						 PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_READ_ONLY),
			"set_num_listener", "get_num_listener");
}

void StudioListener3D::_enter_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	implementation.node_3d = this;
	implementation._enter_tree();
}

void StudioListener3D::_exit_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	implementation._exit_tree();
}

void StudioListener3D::_process(double p_delta)
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	implementation._process(p_delta);
}

float StudioListener3D::distance_to_nearest_listener(const Vector3& position)
{
	return ListenerImpl::distance_to_nearest_listener(position);
}

int StudioListener3D::get_listener_count()
{
	return ListenerImpl::get_listener_count();
}

void StudioListener3D::set_attenuation_object(Object* object)
{
	implementation.attenuation_object = object;
}

Object* StudioListener3D::get_attenuation_object() const
{
	return implementation.attenuation_object;
}

void StudioListener3D::set_rigidbody(Object* object)
{
	implementation.rigidbody = object;
}

Object* StudioListener3D::get_rigidbody() const
{
	return implementation.rigidbody;
}

void StudioListener3D::set_num_listener(int num)
{
	notify_property_list_changed();
}

int StudioListener3D::get_num_listener()
{
	return implementation.get_num_listener();
}