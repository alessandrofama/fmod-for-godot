extends "res://addons/FMOD/tests/fmod_test_framework.gd"

var bank: Bank
const bank_path = "bank:/TestBank"
const bank_guid = "{16897df0-ee8c-4426-b3fe-0cde11692360}"


func before_all():
	super.before_all()
	FMODStudioModule.get_studio_system().load_bank_file(
		"res://addons/FMOD/tests/fmod_test_project/Build/Desktop/TestBank.bank",
		FMODStudioModule.FMOD_STUDIO_LOAD_BANK_NORMAL,
		false
	)
	bank = FMODStudioModule.get_studio_system().get_bank(bank_path)


func after_all():
	super.after_all()
	bank.unload()


func run(randomize = false):
	tests = get_method_names()
	return run_tests(tests, randomize)


func test_is_valid() -> bool:
	return fmod_assert(bank.is_valid())


func test_get_id() -> bool:
	var id = bank.get_id()
	return fmod_assert(id == bank_guid)


func test_get_path() -> bool:
	var path = bank.get_path()
	return fmod_assert(path == bank_path)


func test_load_sample_data() -> bool:
	if bank.get_sample_loading_state() == FMODStudioModule.FMOD_STUDIO_LOADING_STATE_LOADED:
		bank.unload_sample_data()
		FMODStudioModule.get_studio_system().flush_commands()
	var result = bank.load_sample_data()
	FMODStudioModule.get_studio_system().flush_commands()
	bank.unload_sample_data()
	FMODStudioModule.get_studio_system().flush_commands()
	return fmod_assert(result)


func test_unload_sample_data() -> bool:
	var result = false
	if bank.get_sample_loading_state() == FMODStudioModule.FMOD_STUDIO_LOADING_STATE_UNLOADED:
		result = bank.load_sample_data()
		FMODStudioModule.get_studio_system().flush_commands()
	bank.unload_sample_data()
	FMODStudioModule.get_studio_system().flush_commands()
	return fmod_assert(result)


func test_get_loading_state() -> bool:
	var result = false
	var state = bank.get_loading_state()
	if state == FMODStudioModule.FMOD_STUDIO_LOADING_STATE_LOADED:
		result = true
	return fmod_assert(result)


func test_get_sample_loading_state() -> bool:
	var result = false
	bank.load_sample_data()
	FMODStudioModule.get_studio_system().flush_commands()
	if bank.get_sample_loading_state() == FMODStudioModule.FMOD_STUDIO_LOADING_STATE_LOADED:
		result = true
	bank.unload_sample_data()
	FMODStudioModule.get_studio_system().flush_commands()
	return fmod_assert(result)


func test_get_event_count() -> bool:
	var count = bank.get_event_count()
	return fmod_assert(count == 1)


func test_get_event_list() -> bool:
	var event_list: Array[EventDescription] = bank.get_event_list(1624)
	return fmod_assert(event_list.size() == 1)


func test_get_bus_count() -> bool:
	var count = bank.get_bus_count()
	return fmod_assert(count == 0)


func test_get_bus_list() -> bool:
	var bus_list: Array[Bus] = bank.get_bus_list(3543)
	return fmod_assert(bus_list.size() == 0)


func test_get_vca_count() -> bool:
	var count = bank.get_vca_count()
	return fmod_assert(count == 0)


func test_get_vca_list() -> bool:
	var vca_list: Array[VCA] = bank.get_vca_list(462)
	return fmod_assert(vca_list.size() == 0)
