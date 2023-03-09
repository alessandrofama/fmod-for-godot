extends "res://addons/FMOD/tests/fmod_test_framework.gd"

var command_replay: CommandReplay


func before_all():
	super.before_all()
	command_replay = FMODStudioModule.get_studio_system().load_command_replay(
		"res://addons/FMOD/tests/command_capture.txt",
		FMODStudioModule.FMOD_STUDIO_COMMANDCAPTURE_NORMAL
	)


func after_all():
	super.after_all()
	command_replay.release()


func run(randomize = false):
	tests = get_method_names()
	return run_tests(tests, randomize)


func test_is_valid() -> bool:
	var result = command_replay.is_valid()
	return fmod_assert(result)


func test_get_length() -> bool:
	var length = command_replay.get_length()
	return fmod_assert(length > 0)


func test_get_command_count() -> bool:
	var count = command_replay.get_command_count()
	return fmod_assert(count > 0)


func test_get_command_info() -> bool:
	var result = false
	var info = command_replay.get_command_info(10)
	if info.has("command_name"):
		if info.get("command_name") == "System::setListenerAttributes":
			result = true
	return fmod_assert(result)


func test_get_command_string() -> bool:
	var result = false
	var command_string = command_replay.get_command_string(10, 100)
	if (
		command_string
		== "System::setListenerAttributes, 0, {{2,3,5},{9,8,7},{0,0,-1},{0,1,0}}, {14,15,16}, false"
	):
		result = true
	return fmod_assert(result)


func test_get_command_at_time() -> bool:
	var result = false
	var command_at_time = command_replay.get_command_at_time(0.01)
	var command_info = command_replay.get_command_info(command_at_time)
	if command_info.has("command_name"):
		result = command_info.get("command_name") == "Playback::frame"
	return fmod_assert(result)


func test_start() -> bool:
	var result = false
	if command_replay.get_playback_state() == FMODStudioModule.FMOD_STUDIO_PLAYBACK_STOPPED:
		result = command_replay.start()
		FMODStudioModule.get_studio_system().flush_commands()
	else:
		command_replay.stop()
		FMODStudioModule.get_studio_system().flush_commands()
		result = command_replay.start()
		command_replay.stop()
		FMODStudioModule.get_studio_system().flush_commands()
	return fmod_assert(result)


func test_stop() -> bool:
	var result = false
	if command_replay.get_playback_state() == FMODStudioModule.FMOD_STUDIO_PLAYBACK_PLAYING:
		result = command_replay.stop()
		FMODStudioModule.get_studio_system().flush_commands()
	else:
		command_replay.start()
		FMODStudioModule.get_studio_system().flush_commands()
		result = command_replay.stop()
	return fmod_assert(result)


func test_seek_to_time() -> bool:
	var result = command_replay.seek_to_time(0.08)
	return fmod_assert(result)


func test_seek_to_command() -> bool:
	var result = command_replay.seek_to_command(10)
	return fmod_assert(result)


func test_get_paused() -> bool:
	var paused = command_replay.get_paused()
	return fmod_assert(!paused)


func test_set_paused() -> bool:
	var result = command_replay.set_paused(false)
	return fmod_assert(result)


func test_get_playback_state() -> bool:
	var playback_state = command_replay.get_playback_state()
	return fmod_assert(playback_state == FMODStudioModule.FMOD_STUDIO_PLAYBACK_STOPPED)


func test_get_current_command() -> bool:
	var command = command_replay.get_current_command()
	return fmod_assert(command.has("command_index"))
