extends "res://addons/FMOD/tests/fmod_test_framework.gd"

var bus: Bus
const bus_path = "bus:/TestBus"
const bus_guid = "{52187cb8-b8e1-4347-8012-00ed9bacdb47}"


func before_all():
	super.before_all()
	bus = FMODStudioModule.get_studio_system().get_bus(bus_path)


func after_all():
	super.after_all()


func run(randomize = false):
	tests = get_method_names()
	return run_tests(tests, randomize)


func test_is_valid() -> bool:
	return fmod_assert(bus.is_valid())


func test_get_id() -> bool:
	var id = bus.get_id()
	return fmod_assert(id == bus_guid)


func test_get_path() -> bool:
	var path = bus.get_path()
	return fmod_assert(path == bus_path)


func test_get_volume() -> bool:
	var volume = bus.get_volume()
	return fmod_assert(volume["final_volume"] == 1.0)


func test_set_volume() -> bool:
	var result = bus.set_volume(0.5)
	bus.set_volume(1.0)
	return fmod_assert(result)


func test_get_paused() -> bool:
	bus.set_paused(true)
	FMODStudioModule.get_studio_system().flush_commands()
	var paused = bus.get_paused()
	bus.set_paused(false)
	return fmod_assert(paused)


func test_set_paused() -> bool:
	var result = bus.set_paused(true)
	FMODStudioModule.get_studio_system().flush_commands()
	var paused = bus.get_paused()
	bus.set_paused(false)
	return fmod_assert(result and paused)


func test_get_mute() -> bool:
	bus.set_mute(true)
	FMODStudioModule.get_studio_system().flush_commands()
	var mute = bus.get_mute()
	bus.set_mute(false)
	return fmod_assert(mute)


func test_set_mute() -> bool:
	var result = bus.set_mute(true)
	FMODStudioModule.get_studio_system().flush_commands()
	var mute = bus.get_mute()
	bus.set_mute(false)
	return fmod_assert(result and mute)


func test_stop_all_events() -> bool:
	var result = bus.stop_all_events(FMODStudioModule.FMOD_STUDIO_STOP_IMMEDIATE)
	return fmod_assert(result)


func test_lock_channel_group() -> bool:
	var result = bus.lock_channel_group()
	bus.unlock_channel_group()
	return fmod_assert(result)


func test_unlock_channel_group() -> bool:
	bus.lock_channel_group()
	var result = bus.unlock_channel_group()
	return fmod_assert(result)


func test_get_cpu_usage() -> bool:
	var cpu_usage = {}
	bus.get_cpu_usage(cpu_usage)
	return fmod_assert(cpu_usage.has("inclusive") and cpu_usage.has("exclusive"))


func test_get_memory_usage() -> bool:
	var memory_usage = FMOD_STUDIO_MEMORY_USAGE.new()
	bus.get_memory_usage(memory_usage)
	return fmod_assert(memory_usage is FMOD_STUDIO_MEMORY_USAGE)
