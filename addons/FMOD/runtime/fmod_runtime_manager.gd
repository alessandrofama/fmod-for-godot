extends Node

var studio_system: StudioSystem
var debug_scene


class AttachedInstance:
	var instance: EventInstance = null
	var node = null
	var physicsbody = null


var attached_instances: Array = []


func _enter_tree() -> void:
	FMODStudioModule.init()
	studio_system = FMODStudioModule.get_studio_system()
	if OS.has_feature("editor"):
		debug_scene = FMODDebugMonitor.new()
		add_child(debug_scene)
		debug_scene.set_owner(self)


func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		if get_child(0) && debug_scene:
			debug_scene.free()
		FMODStudioModule.shutdown()


func path_to_guid(path: String) -> String:
	return FMODRuntime.studio_system.lookup_id(path)


func get_event_description(event_asset: EventAsset) -> EventDescription:
	var description: EventDescription
	description = FMODRuntime.studio_system.get_event_by_id(event_asset.guid)
	return description


func get_event_description_path(event_path: String) -> EventDescription:
	var guid = path_to_guid(event_path)
	var description: EventDescription = get_event_description_id(guid)
	return description


func get_event_description_id(guid: String) -> EventDescription:
	var description: EventDescription
	description = FMODRuntime.studio_system.get_event_by_id(guid)
	return description


func create_instance(event_asset: EventAsset) -> EventInstance:
	var description: EventDescription
	var guid = event_asset.guid if event_asset else ""
	description = get_event_description_id(guid)
	var instance: EventInstance
	instance = description.create_instance()
	return instance


func create_instance_path(event_path: String) -> EventInstance:
	var description: EventDescription
	description = get_event_description_path(event_path)
	var instance: EventInstance
	instance = description.create_instance()
	return instance


func create_instance_id(guid: String) -> EventInstance:
	var description: EventDescription
	description = get_event_description_id(guid)
	var instance: EventInstance
	instance = description.create_instance()
	return instance


func play_one_shot_attached(event_asset: EventAsset, node) -> void:
	var guid = event_asset.guid
	play_one_shot_attached_id(guid, node)


func play_one_shot_attached_path(event_path: String, node) -> void:
	var guid = path_to_guid(event_path)
	play_one_shot_attached_id(guid, node)


func play_one_shot_attached_id(guid: String, node) -> void:
	var instance = create_instance_id(guid)
	if node is Node2D or node is Node3D:
		attach_instance_to_node(instance, node)
	else:
		push_warning(
			"[FMOD] Trying to attach an instance to an invalid node. The node should inherit Node3D or Node2D."
		)
	instance.start()
	instance.release()


func play_one_shot(event_asset: EventAsset, position = null) -> void:
	var guid = event_asset.guid
	play_one_shot_id(guid, position)


func play_one_shot_path(event_path: String, position = null) -> void:
	var guid = path_to_guid(event_path)
	play_one_shot_id(guid, position)


func play_one_shot_id(guid: String, position = null) -> void:
	var instance = create_instance_id(guid)
	var attributes = FMOD_3D_ATTRIBUTES.new()
	var type = typeof(position)
	if type == TYPE_OBJECT:
		RuntimeUtils.to_3d_attributes_node(attributes, position, null)
	elif type in [TYPE_VECTOR3, TYPE_VECTOR2, TYPE_TRANSFORM2D, TYPE_TRANSFORM3D]:
		RuntimeUtils.to_3d_attributes(attributes, position)
	else:
		RuntimeUtils.to_3d_attributes(attributes, Vector3(0, 0, 0))
	instance.set_3d_attributes(attributes)
	instance.start()
	instance.release()


func attach_instance_to_node(instance: EventInstance, node, physicsbody = null) -> void:
	var runtime_manager = FMODRuntime
	var attached_instance: AttachedInstance
	for i in runtime_manager.attached_instances.size():
		if runtime_manager.attached_instances[i].instance == instance:
			attached_instance = runtime_manager.attached_instances[i]
	if attached_instance == null:
		attached_instance = AttachedInstance.new()
		runtime_manager.attached_instances.append(attached_instance)
	var attributes = FMOD_3D_ATTRIBUTES.new()
	RuntimeUtils.to_3d_attributes_node(attributes, node, physicsbody)
	instance.set_3d_attributes(attributes)
	attached_instance.node = node
	attached_instance.instance = instance
	attached_instance.physicsbody = physicsbody


func detach_instance_from_node(instance: EventInstance) -> void:
	var runtime_manager = FMODRuntime
	for i in runtime_manager.attached_instances.size():
		if runtime_manager.attached_instances[i].instance == instance:
			runtime_manager.attached_instances[i] = runtime_manager.attached_instances[
				runtime_manager.attached_instances.size() - 1
			]
			runtime_manager.attached_instances.remove_at(
				runtime_manager.attached_instances.size() - 1
			)


func _process(_delta: float) -> void:
	if studio_system.is_valid():
		StudioEventEmitter3D.update_active_emitters()

		var i = 0
		while i < attached_instances.size() && i >= 0:
			var playback_state: FMODStudioModule.FMOD_STUDIO_PLAYBACK_STATE = (
				FMODStudioModule.FMOD_STUDIO_PLAYBACK_STATE.FMOD_STUDIO_PLAYBACK_STOPPED
			)

			if attached_instances[i].instance.is_valid():
				playback_state = attached_instances[i].instance.get_playback_state()

			if (
				(
					playback_state
					== FMODStudioModule.FMOD_STUDIO_PLAYBACK_STATE.FMOD_STUDIO_PLAYBACK_STOPPED
				)
				|| attached_instances[i].node == null
			):
				attached_instances[i] = attached_instances[attached_instances.size() - 1]
				attached_instances.remove_at(attached_instances.size() - 1)
				i -= 1
				continue
			var attributes = FMOD_3D_ATTRIBUTES.new()
			if attached_instances[i].node.is_inside_tree():
				RuntimeUtils.to_3d_attributes_node(
					attributes, attached_instances[i].node, attached_instances[i].physicsbody
				)
				attached_instances[i].instance.set_3d_attributes(attributes)
			else:
				push_warning(
					"[FMOD] Trying to set FMOD_3D_ATTRIBUTES for an attached instance, but the instance is not inside the scene tree anymore. Detach the instance before removing the node from the tree."
				)
			i = i + 1
		studio_system.update()
