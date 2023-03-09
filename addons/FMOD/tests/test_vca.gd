extends "res://addons/FMOD/tests/fmod_test_framework.gd"

var vca: VCA
const vca_path = "vca:/TestVCA"
const vca_guid = "{bc51c1c2-2e0a-470d-9c46-60a47dc3de90}"


func before_all():
	super.before_all()
	vca = FMODStudioModule.get_studio_system().get_vca(vca_path)


func after_all():
	super.after_all()


func run(randomize = false):
	tests = get_method_names()
	return run_tests(tests, randomize)


func test_is_valid() -> bool:
	return fmod_assert(vca.is_valid())


func test_get_id() -> bool:
	var id = vca.get_id()
	return fmod_assert(id == vca_guid)


func test_get_path() -> bool:
	var path = vca.get_path()
	return fmod_assert(path == vca_path)


func test_get_volume() -> bool:
	var volume = vca.get_volume()
	return fmod_assert(volume["final_volume"] == 1.0)


func test_set_volume() -> bool:
	var result = vca.set_volume(0.5)
	vca.set_volume(1.0)
	return fmod_assert(result)
