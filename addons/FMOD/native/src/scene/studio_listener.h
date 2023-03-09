#ifndef STUDIO_LISTENER_H
#define STUDIO_LISTENER_H

#include "fmod_studio_module.h"
#include "utils/runtime_utils.h"
#include <float.h>
#include <algorithm>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>

struct ListenerImpl
{
	Node2D* node_2d = nullptr;
	Node3D* node_3d = nullptr;
	Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes = create_ref<FmodTypes::FMOD_3D_ATTRIBUTES>();
	Object* attenuation_object = nullptr;
	Object* rigidbody = nullptr;

	int get_num_listener();

	void add_listener();
	void remove_listener();
	static int get_listener_count();
	void set_listener_location();
	void set_listener_attributes(const Variant& attenuation, int num_listener, const Variant& transform, Object* rigidbody);
	static float distance_to_nearest_listener(const Variant& position);

	void _enter_tree();
	void _exit_tree();
	void _process(double p_delta);
};

class StudioListener2D : public Node2D
{
	GDCLASS(StudioListener2D, Node2D);

protected:
	static void _bind_methods();

private:
	ListenerImpl implementation;

public:
	static float distance_to_nearest_listener(const Vector2& position);

	virtual void _enter_tree() override;
	virtual void _exit_tree() override;
	virtual void _process(double p_delta) override;

	void set_attenuation_object(Object* object);
	Object* get_attenuation_object() const;

	void set_rigidbody(Object* object);
	Object* get_rigidbody() const;

	void set_num_listener(int num);
	int get_num_listener();
};

class StudioListener3D : public Node3D
{
	GDCLASS(StudioListener3D, Node3D);

protected:
	static void _bind_methods();

private:
	ListenerImpl implementation;

public:
	static float distance_to_nearest_listener(const Vector3& position);
	static int get_listener_count();

	virtual void _enter_tree() override;
	virtual void _exit_tree() override;
	virtual void _process(double p_delta) override;

	void set_attenuation_object(Object* object);
	Object* get_attenuation_object() const;

	void set_rigidbody(Object* object);
	Object* get_rigidbody() const;

	void set_num_listener(int num);
	int get_num_listener();
};

#endif // STUDIO_LISTENER_H