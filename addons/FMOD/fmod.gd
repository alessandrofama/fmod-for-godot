@tool
extends EditorPlugin

var editor_addon: EditorPlugin


func _enter_tree():
	if !ProjectSettings.has_setting("autoload/RuntimeManager"):
		add_autoload_singleton("RuntimeManager", "res://addons/FMOD/runtime/runtime_manager.gd")

	# note(alex): _enter_tree is called twice here (bug?) so we check if we are already initialized
	if !FMODStudioEditorModule.get_is_initialized():
		editor_addon = load("res://addons/FMOD/editor/editor_addon.gd").new()
		get_editor_interface().get_base_control().add_child(editor_addon)

		FMODStudioEditorModule.set_is_initialized(true)


func _exit_tree():
	if FMODStudioEditorModule.get_is_initialized():
		get_editor_interface().get_base_control().remove_child(editor_addon)
		editor_addon.queue_free()
	remove_autoload_singleton("RuntimeManager")
