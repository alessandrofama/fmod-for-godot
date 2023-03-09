extends "res://addons/FMOD/tests/fmod_test_framework.gd"

var event_description: EventDescription
const ed_path = "event:/TestEvent"
const ed_guid = "{9ccfe520-0b85-4bb2-9325-d0919729aa54}"


func before_all():
	super.before_all()
	event_description = FMODStudioModule.get_studio_system().get_event(ed_path)


func after_all():
	super.after_all()


func run(randomize = false):
	tests = get_method_names()
	return run_tests(tests, randomize)


func test_is_valid() -> bool:
	var result = event_description.is_valid()
	return fmod_assert(result)


func test_get_id() -> bool:
	var guid = event_description.get_id()
	return fmod_assert(guid == ed_guid)


func test_get_path() -> bool:
	var path = event_description.get_path()
	return fmod_assert(path == ed_path)


func test_get_parameter_description_count() -> bool:
	var count = event_description.get_parameter_description_count()
	return fmod_assert(count > 0)


func test_get_parameter_description_by_index() -> bool:
	var parameter_description: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		event_description.get_parameter_description_by_index(0)
	)
	return fmod_assert(parameter_description.name == "TestContinuousParameter")  #


func test_get_parameter_description_by_name() -> bool:
	var parameter_description: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		event_description.get_parameter_description_by_name("TestContinuousParameter")
	)
	return fmod_assert(parameter_description.name == "TestContinuousParameter")


func test_get_parameter_description_by_id() -> bool:
	var parameter_description_name = event_description.get_parameter_description_by_name(
		"TestContinuousParameter"
	)
	var parameter_description_id: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		event_description.get_parameter_description_by_id(parameter_description_name.id)
	)
	return fmod_assert(parameter_description_id.name == parameter_description_name.name)


func test_get_parameter_label_by_index() -> bool:
	var count = event_description.get_parameter_description_count()
	var parameter_index = 0
	for i in count:
		var parameter_desc: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
			event_description.get_parameter_description_by_index(i)
		)
		if parameter_desc.flags & FMODStudioModule.FMOD_STUDIO_PARAMETER_LABELED:
			parameter_index = i
			break
	var parameter_label: String = event_description.get_parameter_label_by_index(parameter_index, 2)
	return fmod_assert(parameter_label == "LabelC")


func test_get_parameter_label_by_name() -> bool:
	var parameter_label: String = event_description.get_parameter_label_by_name(
		"TestLabeledParameter", 1
	)
	return fmod_assert(parameter_label == "LabelB")


func test_get_parameter_label_by_id() -> bool:
	var parameter_description = event_description.get_parameter_description_by_name(
		"TestLabeledParameter"
	)
	var parameter_label: String = event_description.get_parameter_label_by_id(
		parameter_description.id, 2
	)
	return fmod_assert(parameter_label == "LabelC")


func test_get_user_property_count() -> bool:
	var count = event_description.get_user_property_count()
	return fmod_assert(count == 3)


func test_get_user_property_by_index() -> bool:
	var count = event_description.get_user_property_count()
	var user_property: FMOD_STUDIO_USER_PROPERTY
	for i in count:
		user_property = event_description.get_user_property_by_index(i)
		if user_property.type == FMODStudioModule.FMOD_STUDIO_USER_PROPERTY_TYPE_STRING:
			break
	return fmod_assert(user_property.stringvalue == "This is a String!")


func test_get_user_property() -> bool:
	var user_property: FMOD_STUDIO_USER_PROPERTY = event_description.get_user_property(
		"test_property_3"
	)
	return fmod_assert(user_property.stringvalue == "This is a String!")


func test_get_length() -> bool:
	var length = event_description.get_length()
	return fmod_assert(length > 0)


func test_get_min_max_distance() -> bool:
	var minmax = event_description.get_min_max_distance()
	return fmod_assert(minmax.has("min") and minmax.has("max"))


func test_get_sound_size() -> bool:
	var sound_size = event_description.get_sound_size()
	return fmod_assert(sound_size == 2.0)


func test_is_snapshot() -> bool:
	var result = event_description.is_snapshot()
	return fmod_assert(!result)


func test_is_oneshot() -> bool:
	var result = event_description.is_one_shot()
	return fmod_assert(!result)


func test_is_stream() -> bool:
	var result = event_description.is_stream()
	return fmod_assert(!result)


func test_is_3d() -> bool:
	var result = event_description.is_3d()
	return fmod_assert(result)


func test_is_doppler_enabled() -> bool:
	var result = event_description.is_doppler_enabled()
	return fmod_assert(result)


func test_has_sustain_point() -> bool:
	var result = event_description.has_sustain_point()
	return fmod_assert(result)


func test_create_instance() -> bool:
	var result = false
	var instance = event_description.create_instance()
	if instance.is_valid():
		result = true
		instance.release()
	return fmod_assert(result)


func test_get_instance_count() -> bool:
	var instance_1 = event_description.create_instance()
	var instance_2 = event_description.create_instance()
	FMODStudioModule.get_studio_system().flush_commands()
	var count = event_description.get_instance_count()
	var result = count == 2
	instance_1.release()
	instance_2.release()
	return fmod_assert(result)


func test_get_instance_list() -> bool:
	var instance_1 = event_description.create_instance()
	var instance_2 = event_description.create_instance()
	FMODStudioModule.get_studio_system().flush_commands()
	var count = event_description.get_instance_count()
	var list = event_description.get_instance_list(count)
	var result = list.size() == 2
	instance_1.release()
	instance_2.release()
	return fmod_assert(result)


func test_load_sample_data() -> bool:
	FMODStudioModule.get_studio_system().flush_commands()
	if (
		event_description.get_sample_loading_state()
		== FMODStudioModule.FMOD_STUDIO_LOADING_STATE_LOADED
	):
		event_description.unload_sample_data()
		FMODStudioModule.get_studio_system().flush_commands()
	var result = event_description.load_sample_data()
	event_description.unload_sample_data()
	return fmod_assert(result)


func test_unload_sample_data() -> bool:
	if (
		event_description.get_sample_loading_state()
		== FMODStudioModule.FMOD_STUDIO_LOADING_STATE_UNLOADED
	):
		event_description.load_sample_data()
		FMODStudioModule.get_studio_system().flush_commands()
	var result = event_description.unload_sample_data()
	return fmod_assert(result)


func test_get_sample_loading_state() -> bool:
	FMODStudioModule.get_studio_system().flush_commands()
	var result = (
		event_description.get_sample_loading_state()
		== FMODStudioModule.FMOD_STUDIO_LOADING_STATE_UNLOADED
	)
	return fmod_assert(result)


func test_release_all_instances() -> bool:
	var result = event_description.release_all_instances()
	return fmod_assert(result)
