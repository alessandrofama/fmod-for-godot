extends "res://addons/FMOD/tests/fmod_test_framework.gd"

var command_capture_started = false


func run(randomize = false):
	tests = get_method_names()
	return run_tests(tests, randomize)


func test_is_valid() -> bool:
	var result = FMODStudioModule.get_studio_system().is_valid()
	return fmod_assert(result)


func test_update() -> bool:
	var result = FMODStudioModule.get_studio_system().update()
	return fmod_assert(result)


func test_flush_commands() -> bool:
	var result = FMODStudioModule.get_studio_system().flush_commands()
	return fmod_assert(result)


func test_flush_sample_loading() -> bool:
	var result = FMODStudioModule.get_studio_system().flush_sample_loading()
	return fmod_assert(result)


func test_get_event() -> bool:
	var result_1 = FMODStudioModule.get_studio_system().get_event("event:/TestEvent")
	var result_2 = fmod_assert(result_1 is EventDescription)
	if result_1 and result_2:
		var path = (result_1 as EventDescription).get_path()
		return fmod_assert(path == "event:/TestEvent")
	return false


func test_get_bus() -> bool:
	var result_1 = FMODStudioModule.get_studio_system().get_bus("bus:/TestBus")
	var result_2 = fmod_assert(result_1 is Bus)
	if result_1 and result_2:
		var path = (result_1 as Bus).get_path()
		return fmod_assert(path == "bus:/TestBus")
	return false


func test_get_vca() -> bool:
	var result_1 = FMODStudioModule.get_studio_system().get_vca("vca:/TestVCA")
	var result_2 = fmod_assert(result_1 is VCA)
	if result_1 and result_2:
		var path = (result_1 as VCA).get_path()
		return fmod_assert(path == "vca:/TestVCA")
	return false


func test_get_bank() -> bool:
	var result_1 = FMODStudioModule.get_studio_system().get_bank("bank:/Master")
	var result_2 = fmod_assert(result_1 is Bank)
	if result_1 and result_2:
		var path = (result_1 as Bank).get_path()
		return fmod_assert(path == "bank:/Master")
	return false


func test_get_event_by_id() -> bool:
	var guid = RuntimeManager.path_to_guid("event:/TestEvent")
	var result_1 = FMODStudioModule.get_studio_system().get_event_by_id(guid)
	var result_2 = fmod_assert(result_1 is EventDescription)
	if result_1 and result_2:
		var id = (result_1 as EventDescription).get_id()
		var path = (result_1 as EventDescription).get_path()
		return fmod_assert(id == guid) and fmod_assert(path == "event:/TestEvent")
	return false


func test_get_bus_by_id() -> bool:
	var guid = RuntimeManager.path_to_guid("bus:/TestBus")
	var result_1 = FMODStudioModule.get_studio_system().get_bus_by_id(guid)
	var result_2 = fmod_assert(result_1 is Bus)
	if result_1 and result_2:
		var id = (result_1 as Bus).get_id()
		var path = (result_1 as Bus).get_path()
		return fmod_assert(id == guid) and fmod_assert(path == "bus:/TestBus")
	return false


func test_get_vca_by_id() -> bool:
	var guid = RuntimeManager.path_to_guid("vca:/TestVCA")
	var result_1 = FMODStudioModule.get_studio_system().get_vca_by_id(guid)
	var result_2 = fmod_assert(result_1 is VCA)
	if result_1 and result_2:
		var id = (result_1 as VCA).get_id()
		var path = (result_1 as VCA).get_path()
		return fmod_assert(id == guid) and fmod_assert(path == "vca:/TestVCA")
	return false


func test_get_bank_by_id() -> bool:
	var guid = RuntimeManager.path_to_guid("bank:/Master")
	var result_1 = FMODStudioModule.get_studio_system().get_bank_by_id(guid)
	var result_2 = fmod_assert(result_1 is Bank)
	if result_1 and result_2:
		var id = (result_1 as Bank).get_id()
		var path = (result_1 as Bank).get_path()
		return fmod_assert(id == guid) and fmod_assert(path == "bank:/Master")
	return false


func test_get_parameter_description_by_name() -> bool:
	var result_1 = FMODStudioModule.get_studio_system().get_parameter_description_by_name(
		"GlobalTestDiscreteParameter"
	)
	var result_2 = fmod_assert(result_1 is FMOD_STUDIO_PARAMETER_DESCRIPTION)
	return fmod_assert(
		(result_1 as FMOD_STUDIO_PARAMETER_DESCRIPTION).get_name() == "GlobalTestDiscreteParameter"
	)


func test_get_parameter_description_by_id() -> bool:
	var parameter_description: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		FMODStudioModule
		. get_studio_system()
		. get_parameter_description_by_name("GlobalTestDiscreteParameter")
	)
	var id = parameter_description.get_id()
	var result = FMODStudioModule.get_studio_system().get_parameter_description_by_id(id)  #todo(alex): if the id is typed it crashes, issue on github
	return (
		fmod_assert(result is FMOD_STUDIO_PARAMETER_DESCRIPTION)
		and fmod_assert(
			(
				(result as FMOD_STUDIO_PARAMETER_DESCRIPTION).get_name()
				== "GlobalTestDiscreteParameter"
			)
		)
	)


func test_get_parameter_label_by_name() -> bool:
	var label = FMODStudioModule.get_studio_system().get_parameter_label_by_name(
		"GlobalTestLabeledParameter", 1
	)

	return fmod_assert(label == "LabelB")


func test_get_parameter_label_by_id() -> bool:
	var parameter_description: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		FMODStudioModule
		. get_studio_system()
		. get_parameter_description_by_name("GlobalTestLabeledParameter")
	)
	var id = parameter_description.get_id()
	var label = FMODStudioModule.get_studio_system().get_parameter_label_by_id(id, 1)
	return fmod_assert(label == "LabelB")


func test_get_parameter_by_id() -> bool:
	var parameter_description: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		FMODStudioModule
		. get_studio_system()
		. get_parameter_description_by_name("GlobalTestDiscreteParameter")
	)
	var id = parameter_description.get_id()
	var parameter = FMODStudioModule.get_studio_system().get_parameter_by_id(id)
	return fmod_assert(parameter.has("value") and parameter.has("final_value"))


func test_set_parameter_by_id() -> bool:
	var parameter_description: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		FMODStudioModule
		. get_studio_system()
		. get_parameter_description_by_name("GlobalTestDiscreteParameter")
	)
	var id = parameter_description.get_id()
	var result = FMODStudioModule.get_studio_system().set_parameter_by_id(id, 4.0, false)
	return fmod_assert(result)


func test_set_parameter_by_id_with_label() -> bool:
	var parameter_description: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		FMODStudioModule
		. get_studio_system()
		. get_parameter_description_by_name("GlobalTestLabeledParameter")
	)
	var id = parameter_description.get_id()
	var result = FMODStudioModule.get_studio_system().set_parameter_by_id_with_label(
		id, "LabelC", false
	)
	return fmod_assert(result)


func test_set_parameters_by_ids() -> bool:
	var ids: Array = Array()
	var values = [2.0, 3.0]
	var parameter_description_1: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		FMODStudioModule
		. get_studio_system()
		. get_parameter_description_by_name("GlobalTestLabeledParameter")
	)
	var id_1 = parameter_description_1.get_id()
	var parameter_description_2: FMOD_STUDIO_PARAMETER_DESCRIPTION = (
		FMODStudioModule
		. get_studio_system()
		. get_parameter_description_by_name("GlobalTestDiscreteParameter")
	)
	var id_2 = parameter_description_1.get_id()
	ids.push_back(id_1)
	ids.push_back(id_2)
	var result = FMODStudioModule.get_studio_system().set_parameters_by_ids(ids, values, 2, false)
	return fmod_assert(result)


func test_get_parameter_by_name() -> bool:
	FMODStudioModule.get_studio_system().set_parameter_by_name(
		"GlobalTestDiscreteParameter", 3.0, false
	)
	FMODStudioModule.get_studio_system().flush_commands()
	var param = FMODStudioModule.get_studio_system().get_parameter_by_name(
		"GlobalTestDiscreteParameter"
	)
	var result = false
	if param.has("final_value"):
		result = fmod_assert(param["final_value"] == 3.0)
	return result


func test_set_parameter_by_name() -> bool:
	FMODStudioModule.get_studio_system().set_parameter_by_name(
		"GlobalTestDiscreteParameter", 5.0, false
	)
	FMODStudioModule.get_studio_system().flush_commands()
	var param = FMODStudioModule.get_studio_system().get_parameter_by_name(
		"GlobalTestDiscreteParameter"
	)
	var result = false
	if param.has("final_value"):
		result = fmod_assert(param["final_value"] == 5.0)
	return result


func test_set_parameter_by_name_with_label() -> bool:
	var result_1 = FMODStudioModule.get_studio_system().set_parameter_by_name_with_label(
		"GlobalTestLabeledParameter", "LabelC", false
	)
	FMODStudioModule.get_studio_system().flush_commands()
	var param = FMODStudioModule.get_studio_system().get_parameter_by_name(
		"GlobalTestLabeledParameter"
	)
	var result_2 = false
	if param.has("final_value"):
		result_2 = fmod_assert(param["final_value"] == 2.0)
	return result_1 and result_2


func test_lookup_id() -> bool:
	var path = "event:/TestEvent"
	var guid = FMODStudioModule.get_studio_system().lookup_id(path)
	return fmod_assert(guid == "{9ccfe520-0b85-4bb2-9325-d0919729aa54}")


func test_lookup_path() -> bool:
	var guid = "{9ccfe520-0b85-4bb2-9325-d0919729aa54}"
	var path = FMODStudioModule.get_studio_system().lookup_path(guid)
	return fmod_assert(path == "event:/TestEvent")


func test_get_num_listeners() -> bool:
	FMODStudioModule.get_studio_system().set_num_listeners(4)
	var listeners = FMODStudioModule.get_studio_system().get_num_listeners()
	var result = fmod_assert(listeners == 4)
	FMODStudioModule.get_studio_system().set_num_listeners(1)
	return result


func test_set_num_listeners() -> bool:
	FMODStudioModule.get_studio_system().set_num_listeners(6)
	var listeners = FMODStudioModule.get_studio_system().get_num_listeners()
	var result = fmod_assert(listeners == 6)
	FMODStudioModule.get_studio_system().set_num_listeners(1)
	return result


func test_get_listener_attributes() -> bool:
	FMODStudioModule.get_studio_system().set_num_listeners(1)
	var attributes = FMOD_3D_ATTRIBUTES.new()
	var position = Vector3(2, 3, 5)
	var up = Vector3.UP
	var forward = Vector3.FORWARD
	var velocity = Vector3(9, 8, 7)
	var attenuation = Vector3(14, 15, 16)
	attributes.position = position
	attributes.up = up
	attributes.forward = forward
	attributes.velocity = velocity

	FMODStudioModule.get_studio_system().set_listener_attributes(0, attributes, attenuation)

	var new_attributes_dict = FMODStudioModule.get_studio_system().get_listener_attributes(0)
	var new_attributes: FMOD_3D_ATTRIBUTES = new_attributes_dict["attributes"]
	var result_1 = fmod_assert(new_attributes.position == position)
	var result_2 = fmod_assert(new_attributes.up == up)
	var result_3 = fmod_assert(new_attributes.forward == forward)
	var result_4 = fmod_assert(new_attributes.velocity == velocity)
	var result_5 = fmod_assert(new_attributes_dict["attenuation"] == attenuation)
	FMODStudioModule.get_studio_system().set_num_listeners(1)
	return result_1 and result_2 and result_3 and result_4 and result_5


func test_set_listener_attributes() -> bool:
	FMODStudioModule.get_studio_system().set_num_listeners(1)
	var attributes = FMOD_3D_ATTRIBUTES.new()
	var position = Vector3(2, 3, 5)
	var up = Vector3.UP
	var forward = Vector3.FORWARD
	var velocity = Vector3(9, 8, 7)
	var attenuation = Vector3(14, 15, 16)
	attributes.position = position
	attributes.up = up
	attributes.forward = forward
	attributes.velocity = velocity

	var result = FMODStudioModule.get_studio_system().set_listener_attributes(
		0, attributes, attenuation
	)

	var new_attributes_dict = FMODStudioModule.get_studio_system().get_listener_attributes(0)
	var new_attributes: FMOD_3D_ATTRIBUTES = new_attributes_dict["attributes"]
	var result_1 = fmod_assert(new_attributes.position == position)
	var result_2 = fmod_assert(new_attributes.up == up)
	var result_3 = fmod_assert(new_attributes.forward == forward)
	var result_4 = fmod_assert(new_attributes.velocity == velocity)
	var result_5 = fmod_assert(new_attributes_dict["attenuation"] == attenuation)
	FMODStudioModule.get_studio_system().set_num_listeners(1)
	return result and result_1 and result_2 and result_3 and result_4 and result_5


func test_get_listener_weight() -> bool:
	FMODStudioModule.get_studio_system().set_num_listeners(1)
	FMODStudioModule.get_studio_system().set_listener_weight(0, 0.4)
	FMODStudioModule.get_studio_system().flush_commands()
	var weight: float = FMODStudioModule.get_studio_system().get_listener_weight(0)
	return fmod_assert(is_equal_approx(weight, 0.4))


func test_set_listener_weight() -> bool:
	FMODStudioModule.get_studio_system().set_num_listeners(1)
	var result = FMODStudioModule.get_studio_system().set_listener_weight(0, 0.9)
	var weight = FMODStudioModule.get_studio_system().get_listener_weight(0)
	FMODStudioModule.get_studio_system().set_num_listeners(1)
	return fmod_assert(is_equal_approx(weight, 0.9) and result)


func test_load_bank_file() -> bool:
	var path = "res://addons/FMOD/tests/fmod_test_project/Build/Desktop/TestBank.bank"
	var bank = FMODStudioModule.get_studio_system().load_bank_file(
		path, FMODStudioModule.FMOD_STUDIO_LOAD_BANK_NORMAL, false
	)
	var result_1 = fmod_assert(bank is Bank)
	var result_2 = fmod_assert(bank.get_id() == "{16897df0-ee8c-4426-b3fe-0cde11692360}")
	bank.unload()
	return result_1 and result_2


func test_get_buffer_usage() -> bool:
	var buffer_usage = FMODStudioModule.get_studio_system().get_buffer_usage()
	var result = fmod_assert(buffer_usage is FMOD_STUDIO_BUFFER_USAGE)
	return result


func test_reset_buffer_usage() -> bool:
	return fmod_assert(FMODStudioModule.get_studio_system().reset_buffer_usage())


func test_get_bank_count() -> bool:
	var bank_count = FMODStudioModule.get_studio_system().get_bank_count()
	return fmod_assert(bank_count > 0)


func test_get_bank_list() -> bool:
	var bank_count = FMODStudioModule.get_studio_system().get_bank_count()
	var bank_list = FMODStudioModule.get_studio_system().get_bank_list(bank_count)
	return fmod_assert(bank_list.size() > 0)


func test_get_parameter_description_count() -> bool:
	var count = FMODStudioModule.get_studio_system().get_parameter_description_count()
	return fmod_assert(count > 0)


func test_get_parameter_description_list() -> bool:
	var count = FMODStudioModule.get_studio_system().get_parameter_description_count()
	var list = FMODStudioModule.get_studio_system().get_parameter_description_list(count)
	return fmod_assert(list.size() > 0)
	return false


func test_start_command_capture() -> bool:
	var result = false
	if not command_capture_started:
		result = FMODStudioModule.get_studio_system().start_command_capture(
			"command_capture.txt", FMODStudioModule.FMOD_STUDIO_COMMANDCAPTURE_NORMAL
		)
		command_capture_started = true
	else:
		FMODStudioModule.get_studio_system().stop_command_capture()
		result = FMODStudioModule.get_studio_system().start_command_capture(
			"command_capture.txt", FMODStudioModule.FMOD_STUDIO_COMMANDCAPTURE_NORMAL
		)
		command_capture_started = true

	return fmod_assert(result)


func test_stop_command_capture() -> bool:
	var result = false
	if not command_capture_started:
		FMODStudioModule.get_studio_system().start_command_capture(
			"command_capture.txt", FMODStudioModule.FMOD_STUDIO_COMMANDCAPTURE_NORMAL
		)
		command_capture_started = true

	result = FMODStudioModule.get_studio_system().stop_command_capture()
	command_capture_started = false
	return fmod_assert(result)


func test_load_command_replay() -> bool:
	if not command_capture_started:
		test_start_command_capture()
		test_stop_command_capture()
	var command_replay = FMODStudioModule.get_studio_system().load_command_replay(
		"res://command_capture.txt", FMODStudioModule.FMOD_STUDIO_COMMANDREPLAY_NORMAL
	)
	return fmod_assert(command_replay is CommandReplay)


func test_get_cpu_usage() -> bool:
	var studio_cpu_usage = FMOD_STUDIO_CPU_USAGE.new()
	var cpu_usage = FMOD_CPU_USAGE.new()
	FMODStudioModule.get_studio_system().get_cpu_usage(studio_cpu_usage, cpu_usage)
	return fmod_assert(studio_cpu_usage.update > 0.0)


func test_get_memory_usage() -> bool:
	var memory_usage = FMOD_STUDIO_MEMORY_USAGE.new()
	FMODStudioModule.get_studio_system().get_memory_usage(memory_usage)
	return fmod_assert(memory_usage is FMOD_STUDIO_MEMORY_USAGE)
