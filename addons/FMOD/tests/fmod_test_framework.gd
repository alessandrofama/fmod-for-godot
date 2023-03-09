extends Node

var banks
var banks_loaded = false

var tests = []

var studio_tests = {
	"StudioSystem": "res://addons/FMOD/tests/test_studiosystem.gd",
	"EventDescription": "res://addons/FMOD/tests/test_eventdescription.gd",
	"EventInstance": "res://addons/FMOD/tests/test_eventinstance.gd",
	"Bus": "res://addons/FMOD/tests/test_bus.gd",
	"VCA": "res://addons/FMOD/tests/test_vca.gd",
	"Bank": "res://addons/FMOD/tests/test_bank.gd",
	"CommandReplay": "res://addons/FMOD/tests/test_commandreplay.gd",
}


func get_method_names() -> Array:
	var methods = []
	for fun in get_method_list():
		if fun["flags"] == METHOD_FLAG_NORMAL:
			var func_name: String = fun["name"]
			if func_name.begins_with("test_"):
				var dict = {"name": func_name, "func": Callable(self, func_name)}
				methods.append(dict)
	return methods


func load_test_banks():
	banks = [
		FMODStudioModule.get_studio_system().load_bank_file(
			"res://addons/FMOD/tests/fmod_test_project/Build/Desktop/Master.bank",
			FMODStudioModule.FMOD_STUDIO_LOAD_BANK_NORMAL,
			false
		),
		FMODStudioModule.get_studio_system().load_bank_file(
			"res://addons/FMOD/tests/fmod_test_project/Build/Desktop/Master.strings.bank",
			FMODStudioModule.FMOD_STUDIO_LOAD_BANK_NORMAL,
			false
		),
	]
	banks_loaded = true


func unload_test_banks():
	for bank in banks:
		(bank as Bank).unload()
	banks.clear()
	banks_loaded = false


func fmod_assert(condition, message: String = ""):
	if not condition:
		if not message.is_empty():
			print("Assertion Error: ", message)
		return false
	return true


func before_all():
	load_test_banks()


func after_all():
	unload_test_banks()


func run_tests(tests, randomize) -> Dictionary:
	before_all()
	var passed = 0
	var failed = 0
	var failed_functions = Array()
	if randomize:
		tests.shuffle()
	for test in tests:
		RuntimeManager.studio_system.update()
		if test["func"].call():
			passed += 1
			print_rich("[color=green]PASS: ", test["name"] + "[/color]")
		else:
			failed += 1
			failed_functions.append(test["name"])
			print_rich("[color=red]FAIL: ", test["name"] + "[/color]")
	after_all()
	print_rich("[color=green]Passed: ", str(passed) + "[/color]")
	print_rich("[color=red]Failed: ", str(failed) + "[/color]")
	return {"passed": passed, "failed": failed, "failed_functions": failed_functions}


func _ready():
	print()
	print_rich(
		"[wave amp=50 freq=2]≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈ ≈ 🆁🆄🅽🅽🅸🅽🅶 ​ 🆃🅴🆂🆃🆂 ≈ ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈[/wave]\n"
	)
	var completed_tests = Array()
	var infos = Array()
	for test in studio_tests:
		print_rich("[color=white]" + test + ":[/color]")
		var node = load(studio_tests[test]).new()
		var info = node.run(false)
		completed_tests.append(test)
		infos.append(info)
		node.free()
		print()
	print_rich("[wave amp=50 freq=2]≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈ ≈ 🆃🅴🆂🆃 🆁🅴🆂🆄🅻🆃🆂 ≈ ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈[/wave]\n")

	for i in completed_tests.size():
		print_rich("[color=white]" + completed_tests[i] + ":[/color]")
		print_rich("[color=green]Passed: " + str(infos[i]["passed"]) + "[/color]")
		print_rich("[color=red]Failed: " + str(infos[i]["failed"]) + "[/color]")
		for function in infos[i]["failed_functions"]:
			print_rich("[color=red]" + function + "[/color]")
		if i != completed_tests.size() - 1:
			print()
