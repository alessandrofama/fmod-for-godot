#include "studio_event_emitter_3d_gizmo_plugin.h"

using namespace godot;

void StudioEventEmitter3DGizmoPlugin::_bind_methods()
{
}

bool StudioEventEmitter3DGizmoPlugin::_has_gizmo(Node3D* for_node_3d) const
{
	if (for_node_3d)
	{
		return for_node_3d->get_class() == "StudioEventEmitter3D";
	}
	return false;
}

String StudioEventEmitter3DGizmoPlugin::_get_gizmo_name() const
{
	return "StudioEventEmitter3D";
}

int32_t StudioEventEmitter3DGizmoPlugin::_get_priority() const
{
	return -1;
}

void StudioEventEmitter3DGizmoPlugin::_redraw(const Ref<EditorNode3DGizmo>& gizmo)
{
	EditorNode3DGizmo* gizmo_ref = const_cast<EditorNode3DGizmo*>(gizmo.ptr());
	gizmo_ref->clear();

	StudioEventEmitter3D* emitter = Object::cast_to<StudioEventEmitter3D>(gizmo->get_node_3d());

	if (!emitter)
	{
		return;
	}

	if (emitter->get("event").get_type() == Variant::Type::NIL)
	{
		return;
	}

	Ref<StandardMaterial3D> icon = get_material("studio_event_emitter_3d_icon", gizmo_ref);
	gizmo_ref->add_unscaled_billboard(icon, 0.03);

	Ref<StandardMaterial3D> lines_billboard_material =
			get_material("studio_event_emitter_3d_material_billboard", gizmo_ref);

	float soft_multiplier = 10000.0f;

	if (emitter->get_event().is_null())
	{
		return;
	}

	Ref<EventAsset> event_asset = emitter->get_event();

	if (!event_asset.is_valid())
	{
		return;
	}

	

	bool is_3d = event_asset->get_3d();

	if (!is_3d)
	{
		return;
	}

	float r_max{};	
	float r_min{};
	float max_distance = event_asset->get_max_distance();
	float min_distance = event_asset->get_min_distance();


	if (max_distance > CMP_EPSILON)
	{
		r_max = MIN(1 * soft_multiplier, max_distance);
		r_min = MIN(1 * soft_multiplier, min_distance);
	}
	else
	{
		r_max = 1 * soft_multiplier;
		r_min = 1 * soft_multiplier;
	}

	PackedVector3Array points_billboard;

	for (int i = 0; i < 120; i++)
	{
		const float ra = Math::deg_to_rad((float)(i * 3));
		const float rb = Math::deg_to_rad((float)((i + 1) * 3));
		const Point2 a = Vector2(Math::sin(ra), Math::cos(ra)) * r_max;
		const Point2 b = Vector2(Math::sin(rb), Math::cos(rb)) * r_max;

		points_billboard.push_back(Vector3(a.x, a.y, 0));
		points_billboard.push_back(Vector3(b.x, b.y, 0));
	}

	for (int i = 0; i < 120; i++)
	{
		const float ra = Math::deg_to_rad((float)(i * 3));
		const float rb = Math::deg_to_rad((float)((i + 1) * 3));
		const Point2 a = Vector2(Math::sin(ra), Math::cos(ra)) * r_min;
		const Point2 b = Vector2(Math::sin(rb), Math::cos(rb)) * r_min;

		points_billboard.push_back(Vector3(a.x, a.y, 0));
		points_billboard.push_back(Vector3(b.x, b.y, 0));
	}

	Color color = Color(1, 1, 1);

	if (max_distance > CMP_EPSILON)
	{
		color.set_h(color.get_h() + 0.5);
	}

	Ref<Material> material = Ref<Material>(Object::cast_to<Material>(lines_billboard_material.ptr()));
	gizmo_ref->add_lines(points_billboard, material, true, color);
}