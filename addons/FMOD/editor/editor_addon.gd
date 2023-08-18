@tool
extends EditorPlugin

# Plugins
var inspector_browser: EditorInspectorPlugin
var studio_event_emitter_3d_gizmo_plugin: EditorNode3DGizmoPlugin

# Icon for the custom nodes
var fmod_icon: Texture2D

# Project Browser in the Toolbar
var project_preview_browser: Window
var project_preview_button: Button

var banks_loaded_timer: Timer


func _enter_tree():
	# Create all icons
	create_icons()

	fmod_icon = FMODStudioEditorModule.get_icon(FMODStudioEditorModule.FMOD_ICONTYPE_PROJECT)

	# Set up the icons for the custom nodes
	# note(alex): This is very slow and results in a slower editor start
	setup_custom_nodes_icon()

	# Add the inspector browser plugin to Godot. Until godot-cpp#886 is fixed
	# we need to implement this in GDScript
	inspector_browser = load("res://addons/FMOD/editor/inspector_browser_plugin.gd").new()
	add_inspector_plugin(inspector_browser)

	# Add the Gizmo plugin for the StudioEventEmitter3D Node to Godot. Not possible
	# from godot-cpp directly
	studio_event_emitter_3d_gizmo_plugin = StudioEventEmitter3DGizmoPlugin.new()
	add_node_3d_gizmo_plugin(studio_event_emitter_3d_gizmo_plugin)

	# Add the Project Browser Button to the Container Toolbar
	project_preview_button = ProjectBrowserPreviewButton.new()
	project_preview_browser = ProjectBrowserWindow.new()
	var editor_scale = get_editor_interface().get_editor_scale()
	project_preview_browser.set_editor_scale(editor_scale)
	project_preview_browser.initialize()
	add_control_to_container(EditorPlugin.CONTAINER_TOOLBAR, project_preview_button)
	
	if project_preview_button.get_child_count() == 0:
		project_preview_button.add_child(project_preview_browser)
		project_preview_browser.set_visible(false)

	# Set the Project Browser visible when pressed
	project_preview_button.connect("pressed",
	func():
		if project_preview_browser.visible == false:
			project_preview_browser.popup_centered()
		else:
			project_preview_browser.set_visible(false))

func _exit_tree():
	remove_control_from_container(EditorPlugin.CONTAINER_TOOLBAR, project_preview_button)
	remove_inspector_plugin(inspector_browser)
	remove_node_3d_gizmo_plugin(studio_event_emitter_3d_gizmo_plugin)


func create_icons():
	FMODStudioEditorModule.create_icon(
		"res://addons/FMOD/editor/images/fmod_icon.svg",
		FMODStudioEditorModule.FMOD_ICONTYPE_PROJECT
	)
	FMODStudioEditorModule.create_icon(
		"res://addons/FMOD/editor/images/folder_closed.svg",
		FMODStudioEditorModule.FMOD_ICONTYPE_FOLDER_CLOSED
	)
	FMODStudioEditorModule.create_icon(
		"res://addons/FMOD/editor/images/folder_opened.svg",
		FMODStudioEditorModule.FMOD_ICONTYPE_FOLDER_OPENED
	)
	FMODStudioEditorModule.create_icon(
		"res://addons/FMOD/editor/images/event_icon.svg", FMODStudioEditorModule.FMOD_ICONTYPE_EVENT
	)
	FMODStudioEditorModule.create_icon(
		"res://addons/FMOD/editor/images/bank_icon.svg", FMODStudioEditorModule.FMOD_ICONTYPE_BANK
	)
	FMODStudioEditorModule.create_icon(
		"res://addons/FMOD/editor/images/snapshot_icon.svg",
		FMODStudioEditorModule.FMOD_ICONTYPE_SNAPSHOT
	)
	FMODStudioEditorModule.create_icon(
		"res://addons/FMOD/editor/images/bus_icon.svg", FMODStudioEditorModule.FMOD_ICONTYPE_BUS
	)
	FMODStudioEditorModule.create_icon(
		"res://addons/FMOD/editor/images/vca_icon.svg", FMODStudioEditorModule.FMOD_ICONTYPE_VCA
	)
	FMODStudioEditorModule.create_icon(
		"res://addons/FMOD/editor/images/c_parameter_icon.svg",
		FMODStudioEditorModule.FMOD_ICONTYPE_C_PARAMETER
	)


func setup_custom_nodes_icon():
	# note(alex): Directly setting the icons on the theme slows down startup time
	# considerably. We duplicate the theme to avoid this here.
	var theme := get_editor_interface().get_base_control().get_theme().duplicate()
	if theme:
		theme.set_icon(&"StudioEventEmitter3D", &"EditorIcons", fmod_icon)
		theme.set_icon(&"StudioEventEmitter2D", &"EditorIcons", fmod_icon)
		theme.set_icon(&"StudioParameterTrigger", &"EditorIcons", fmod_icon)
		theme.set_icon(&"StudioGlobalParameterTrigger", &"EditorIcons", fmod_icon)
		theme.set_icon(&"StudioBankLoader", &"EditorIcons", fmod_icon)
		theme.set_icon(&"StudioListener3D", &"EditorIcons", fmod_icon)
		theme.set_icon(&"StudioListener2D", &"EditorIcons", fmod_icon)
		get_editor_interface().get_base_control().set_theme(theme)
