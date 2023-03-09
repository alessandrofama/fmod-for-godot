#include "runtime_utils.h"

using namespace godot;

void RuntimeUtils::_bind_methods()
{
	ClassDB::bind_static_method("RuntimeUtils", D_METHOD("to_3d_attributes_node", "attributes", "node", "physicsbody"), &RuntimeUtils::to_3d_attributes_node);

	ClassDB::bind_static_method("RuntimeUtils", D_METHOD("to_3d_attributes_transform_physicsbody", "attributes", "transform", "physicsbody"),
			&RuntimeUtils::to_3d_attributes_transform_physicsbody);
	ClassDB::bind_static_method("RuntimeUtils", D_METHOD("to_3d_attributes", "attributes", "position"), &RuntimeUtils::to_3d_attributes);

	BIND_ENUM_CONSTANT(GAMEEVENT_NONE);
	BIND_ENUM_CONSTANT(GAMEEVENT_ENTER_TREE);
	BIND_ENUM_CONSTANT(GAMEEVENT_READY);
	BIND_ENUM_CONSTANT(GAMEEVENT_EXIT_TREE);
}

void RuntimeUtils::to_3d_attributes_node(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes, Object* node,
		Object* physicsbody)
{
	const Node3D* node3d = Object::cast_to<Node3D>(node);
	if (node3d)
	{
		to_3d_attributes_transform_physicsbody(attributes, node3d->get_global_transform(), physicsbody);
	}
	else
	{
		const Node2D* node2d = Object::cast_to<Node2D>(node);
		if (node2d)
		{
			to_3d_attributes_transform_physicsbody(attributes, node2d->get_global_transform(), physicsbody);
		}
	}
}

void RuntimeUtils::to_3d_attributes_transform_physicsbody(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes,
		const Variant& transform, Object* physicsbody)
{
	FMOD_3D_ATTRIBUTES fmod_attributes{};
	float distance_scale_2d = FMODStudioModule::get_singleton()->distance_scale_2d;

	const Variant::Type transform_type = transform.get_type();
	if (transform_type == Variant::TRANSFORM3D)
	{
		const Transform3D transform3d = transform;

		transform3d_to_3dattributes(transform3d, fmod_attributes, physicsbody);
		attributes->set_3d_attributes(fmod_attributes);
	}
	else if (transform_type == Variant::TRANSFORM2D)
	{
		Transform2D transform2d = transform;
		transform2d_to_3dattributes(transform2d, fmod_attributes, physicsbody, distance_scale_2d);
		attributes->set_3d_attributes(fmod_attributes);
	}
	else
	{
		UtilityFunctions::push_warning(
				"[FMOD] Converting to 3D Attributes is only available for Transform3D and Transfom2D. ", __FUNCTION__,
				__FILE__, __LINE__);
	}
}

void RuntimeUtils::to_3d_attributes(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes, const Variant& position)
{
	FMOD_3D_ATTRIBUTES fmod_attributes{};
	float distance_scale_2d = FMODStudioModule::get_singleton()->distance_scale_2d;

	switch (position.get_type())
	{
		case Variant::TRANSFORM3D:
		{
			Transform3D transform3d = position;
			transform3d_to_3dattributes(transform3d, fmod_attributes);
			attributes->set_3d_attributes(fmod_attributes);
			break;
		}
		case Variant::TRANSFORM2D:
		{
			Transform2D transform2d = position;
			transform2d_to_3dattributes(transform2d, fmod_attributes, nullptr, distance_scale_2d);
			attributes->set_3d_attributes(fmod_attributes);
			break;
		}
		case Variant::VECTOR3:
		{
			Vector3 vector3 = position;
			vector3_to_fmod_vector(vector3, fmod_attributes.position);
			vector3_to_fmod_vector(Vector3(0, 0, 1), fmod_attributes.forward);
			vector3_to_fmod_vector(Vector3(0, 1, 0), fmod_attributes.up);
			attributes->set_3d_attributes(fmod_attributes);
			break;
		}
		case Variant::VECTOR2:
		{
			Vector2 vector2 = position;
			vector2_to_fmod_vector(vector2, fmod_attributes.position, distance_scale_2d);
			vector3_to_fmod_vector(Vector3(0, 0, 1), fmod_attributes.forward);
			vector3_to_fmod_vector(Vector3(0, 1, 0), fmod_attributes.up);
			attributes->set_3d_attributes(fmod_attributes);
			break;
		}
		default:
		{
			UtilityFunctions::push_warning(
					"[FMOD] Converting to 3D Attributes is only available for Transform3D, Transform2D, Vector3 and Vector2.",
					__FUNCTION__, __FILE__, __LINE__);
			break;
		}
	}
}