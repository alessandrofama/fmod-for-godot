extends "res://addons/FMOD/tests/fmod_test_framework.gd"

var event_instance: EventInstance
const ed_path = "event:/TestEvent"


func before_all():
	super.before_all()
	event_instance = FMODStudioModule.get_studio_system().get_event(ed_path).create_instance()


func after_all():
	super.after_all()
	event_instance.release()


func run(randomize = false):
	tests = get_method_names()
	return run_tests(tests, randomize)


func test_is_valid() -> bool:
	return fmod_assert(event_instance.is_valid())


func test_get_description() -> bool:
	var description = event_instance.get_description()
	return fmod_assert(description.is_valid())


func test_get_volume() -> bool:
	event_instance.set_volume(0.5)
	FMODStudioModule.get_studio_system().flush_commands()
	var volume = event_instance.get_volume()
	event_instance.set_volume(1.0)
	return fmod_assert(volume["final_volume"] == 0.5)


func test_set_volume() -> bool:
	var result = event_instance.set_volume(0.2)
	event_instance.set_volume(1.0)
	return fmod_assert(result)


func test_get_pitch() -> bool:
	event_instance.set_pitch(0.5)
	FMODStudioModule.get_studio_system().flush_commands()
	var pitch = event_instance.get_pitch()
	event_instance.set_pitch(1.0)
	return fmod_assert(pitch["final_pitch"] == 0.5)


func test_set_pitch() -> bool:
	var result = event_instance.set_pitch(0.8)
	event_instance.set_pitch(1)
	return fmod_assert(result)


func test_get_3d_attributes() -> bool:
	var attributes = FMOD_3D_ATTRIBUTES.new()
	var position = Vector3(2, 3, 5)
	var up = Vector3.UP
	var forward = Vector3.FORWARD
	var velocity = Vector3(9, 8, 7)
	attributes.position = position
	attributes.up = up
	attributes.forward = forward
	attributes.velocity = velocity
	event_instance.set_3d_attributes(attributes)

	var new_attributes = FMOD_3D_ATTRIBUTES.new()
	event_instance.get_3d_attributes(new_attributes)
	var result_1 = fmod_assert(new_attributes.position == position)
	var result_2 = fmod_assert(new_attributes.up == up)
	var result_3 = fmod_assert(new_attributes.forward == forward)
	var result_4 = fmod_assert(new_attributes.velocity == velocity)
	return result_1 and result_2 and result_3 and result_4


func test_set_3d_attributes() -> bool:
	var attributes = FMOD_3D_ATTRIBUTES.new()
	var position = Vector3(6, 8, 4)
	var up = Vector3.UP
	var forward = Vector3.FORWARD
	var velocity = Vector3(1, 1, 2)
	attributes.position = position
	attributes.up = up
	attributes.forward = forward
	attributes.velocity = velocity
	var result = event_instance.set_3d_attributes(attributes)
	return fmod_assert(result)


func test_get_listener_mask() -> bool:
	var result = event_instance.get_listener_mask()
	return fmod_assert(result)


func test_set_listener_mask() -> bool:
	var result = event_instance.set_listener_mask(1)
	return fmod_assert(result)


func test_get_property() -> bool:
	event_instance.set_property(FMODStudioModule.FMOD_STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE, 12.0)
	FMODStudioModule.get_studio_system().flush_commands()
	var property = event_instance.get_property(
		FMODStudioModule.FMOD_STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE
	)
	event_instance.set_property(FMODStudioModule.FMOD_STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE, -1.0)  # note(alex): default set in Studio
	return fmod_assert(property == 12.0)


func test_set_property() -> bool:
	var result = event_instance.set_property(
		FMODStudioModule.FMOD_STUDIO_EVENT_PROPERTY_MINIMUM_DISTANCE, 3.0
	)
	FMODStudioModule.get_studio_system().flush_commands()
	var property = event_instance.get_property(
		FMODStudioModule.FMOD_STUDIO_EVENT_PROPERTY_MINIMUM_DISTANCE
	)
	event_instance.set_property(FMODStudioModule.FMOD_STUDIO_EVENT_PROPERTY_MINIMUM_DISTANCE, -1.0)  # note(alex): default set in Studio
	return fmod_assert(result and property == 3.0)


func test_get_reverb_level() -> bool:
	event_instance.set_reverb_level(0, 0.5)
	FMODStudioModule.get_studio_system().flush_commands()
	var reverb_level = event_instance.get_reverb_level(0)
	return fmod_assert(reverb_level == 0.5)


func test_set_reverb_level() -> bool:
	var result = event_instance.set_reverb_level(1, 0.3)
	return fmod_assert(result)


func test_get_paused() -> bool:
	var paused = event_instance.get_paused()
	return fmod_assert(!paused)


func test_set_paused() -> bool:
	var result = event_instance.set_paused(false)
	return fmod_assert(result)


func test_start() -> bool:
	var result = false
	if event_instance.get_playback_state() == FMODStudioModule.FMOD_STUDIO_PLAYBACK_STOPPED:
		result = event_instance.start()
		event_instance.stop(FMODStudioModule.FMOD_STUDIO_STOP_IMMEDIATE)
		FMODStudioModule.get_studio_system().flush_commands()
	return fmod_assert(result)


func test_stop() -> bool:
	var result = false
	if event_instance.get_playback_state() == FMODStudioModule.FMOD_STUDIO_PLAYBACK_STOPPED:
		event_instance.start()
		result = event_instance.stop(FMODStudioModule.FMOD_STUDIO_STOP_IMMEDIATE)
	return fmod_assert(result)


func test_get_timeline_position() -> bool:
	event_instance.set_timeline_position(100)
	FMODStudioModule.get_studio_system().flush_commands()
	var position = event_instance.get_timeline_position()
	return fmod_assert(position == 100)


func test_set_timeline_position() -> bool:
	var result = event_instance.set_timeline_position(200)
	FMODStudioModule.get_studio_system().flush_commands()
	var position = event_instance.get_timeline_position()
	return fmod_assert(result and position == 200)


func test_get_playback_state() -> bool:
	event_instance.start()
	FMODStudioModule.get_studio_system().flush_commands()
	var playback_state = event_instance.get_playback_state()
	event_instance.stop(FMODStudioModule.FMOD_STUDIO_STOP_IMMEDIATE)
	return fmod_assert(playback_state == FMODStudioModule.FMOD_STUDIO_PLAYBACK_PLAYING)


func test_get_min_max_distance() -> bool:
	var minmax = event_instance.get_min_max_distance()
	return fmod_assert(minmax.has("min") and minmax.has("max"))


func test_is_virtual() -> bool:
	var result = event_instance.is_virtual()
	return fmod_assert(!result)


func test_get_parameter_by_id() -> bool:
	var description = event_instance.get_description()
	var parameter_description = description.get_parameter_description_by_name(
		"TestContinuousParameter"
	)
	var parameter = event_instance.get_parameter_by_id(parameter_description.id)
	return fmod_assert(parameter.has("final_value"))


func test_set_parameter_by_id() -> bool:
	var description = event_instance.get_description()
	var parameter_description = description.get_parameter_description_by_name(
		"TestContinuousParameter"
	)
	var result = event_instance.set_parameter_by_id(parameter_description.id, 1.0, false)
	return fmod_assert(result)


func test_set_parameter_by_id_with_label() -> bool:
	var description = event_instance.get_description()
	var parameter_description = description.get_parameter_description_by_name(
		"TestLabeledParameter"
	)
	var result = event_instance.set_parameter_by_id_with_label(
		parameter_description.id, "LabelA", false
	)
	return fmod_assert(result)


func test_set_parameters_by_ids() -> bool:
	var ids: Array = Array()
	var values = [0.8, 3.0]
	var description = event_instance.get_description()
	var parameter_description_1: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		description.get_parameter_description_by_name("TestContinuousParameter")
	)
	var id_1 = parameter_description_1.get_id()
	var parameter_description_2: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		description.get_parameter_description_by_name("TestDiscreteParameter")
	)
	var id_2 = parameter_description_1.get_id()
	ids.push_back(id_1)
	ids.push_back(id_2)
	var result = event_instance.set_parameters_by_ids(ids, values, 2, false)
	return fmod_assert(result)


func test_get_parameter_by_name() -> bool:
	var parameter = event_instance.get_parameter_by_name("TestLabeledParameter")
	return fmod_assert(parameter.get("final_value") == 0.0)


func test_set_parameter_by_name() -> bool:
	var result = event_instance.set_parameter_by_name("TestContinuousParameter", 0.3, false)
	return fmod_assert(result)


func test_set_parameter_by_name_with_label() -> bool:
	var result = event_instance.set_parameter_by_name_with_label(
		"TestLabeledParameter", "LabelA", false
	)
	return fmod_assert(result)


func test_key_off() -> bool:
	event_instance.start()
	FMODStudioModule.get_studio_system().flush_commands()
	var result = event_instance.key_off()
	event_instance.stop(0)
	return fmod_assert(result)


func test_get_cpu_usage() -> bool:
	var cpu_usage = {}
	event_instance.get_cpu_usage(cpu_usage)
	return fmod_assert(cpu_usage.has("inclusive") and cpu_usage.has("exclusive"))


func test_get_memory_usage() -> bool:
	var result = false
	var memory_usage = FMOD_STUDIO_MEMORY_USAGE.new()
	event_instance.get_memory_usage(memory_usage)
	return fmod_assert(memory_usage is FMOD_STUDIO_MEMORY_USAGE)
