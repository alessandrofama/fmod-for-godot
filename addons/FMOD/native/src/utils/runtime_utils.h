#ifndef RUNTIME_UTILS_H
#define RUNTIME_UTILS_H

#include "fmod_studio_module.h"
#include "fmod_types.h"
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

class RuntimeUtils : public RefCounted
{
	GDCLASS(RuntimeUtils, RefCounted);

protected:
	static void _bind_methods();

public:
	enum GameEvent
	{
		GAMEEVENT_NONE,
		GAMEEVENT_ENTER_TREE,
		GAMEEVENT_READY,
		GAMEEVENT_EXIT_TREE
	};

	static void to_3d_attributes_node(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes, Object* node, Object* physicsbody);
	static void to_3d_attributes_transform_physicsbody(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes,
			const Variant& transform, Object* physicsbody);
	static void to_3d_attributes(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes, const Variant& position);
};

VARIANT_ENUM_CAST(RuntimeUtils::GameEvent);

#endif // RUNTIME_UTILS_H