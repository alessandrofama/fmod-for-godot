#ifndef FMOD_PLUGIN
#define FMOD_PLUGIN

#include "fmod_studio_editor_module.h"
#include "inspector_browser.h"
#include "project_browser.h"
#include "studio_event_emitter_3d_gizmo_plugin.h"
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>

class FMODEditorPlugin : public EditorPlugin
{
	GDCLASS(FMODEditorPlugin, EditorPlugin);

protected:
	static void _bind_methods()
	{
		ClassDB::bind_method(D_METHOD("on_project_browser_button_pressed"), &FMODEditorPlugin::on_project_browser_button_pressed);
	}

private:
	Ref<StudioEventEmitter3DGizmoPlugin> gizmo_plugin;
	Ref<FMODEditorInspectorPlugin> inspector_plugin;
	FMODProjectBrowserWindow* project_preview_browser;
	FMODProjectBrowserPreviewButton* project_preview_button;

public:
	virtual void _enter_tree() override
	{
		if (!ProjectSettings::get_singleton()->has_setting("autoload/FMODRuntime"))
		{
			add_autoload_singleton("FMODRuntime", "res://addons/FMOD/runtime/fmod_runtime_manager.gd");
		}

		FMODSettings* fmod_settings = FMODSettings::get_singleton();
		fmod_settings->initialize();

		FMODStudioEditorModule* fmod_editor = FMODStudioEditorModule::get_singleton();

		// Create Icons
		if (fmod_editor)
		{
			fmod_editor->create_icon("res://addons/FMOD/editor/images/fmod_icon.svg", FMODStudioEditorModule::FMOD_ICONTYPE_PROJECT);
			fmod_editor->create_icon("res://addons/FMOD/editor/images/folder_closed.svg", FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_CLOSED);
			fmod_editor->create_icon("res://addons/FMOD/editor/images/folder_opened.svg", FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_OPENED);
			fmod_editor->create_icon("res://addons/FMOD/editor/images/event_icon.svg", FMODStudioEditorModule::FMOD_ICONTYPE_EVENT);
			fmod_editor->create_icon("res://addons/FMOD/editor/images/bank_icon.svg", FMODStudioEditorModule::FMOD_ICONTYPE_BANK);
			fmod_editor->create_icon("res://addons/FMOD/editor/images/snapshot_icon.svg", FMODStudioEditorModule::FMOD_ICONTYPE_SNAPSHOT);
			fmod_editor->create_icon("res://addons/FMOD/editor/images/bus_icon.svg", FMODStudioEditorModule::FMOD_ICONTYPE_BUS);
			fmod_editor->create_icon("res://addons/FMOD/editor/images/vca_icon.svg", FMODStudioEditorModule::FMOD_ICONTYPE_VCA);
			fmod_editor->create_icon("res://addons/FMOD/editor/images/c_parameter_icon.svg", FMODStudioEditorModule::FMOD_ICONTYPE_C_PARAMETER);
		}

		// Init Project Browser
		project_preview_browser = memnew(FMODProjectBrowserWindow);
		float editor_scale = get_editor_interface()->get_editor_scale();
		project_preview_browser->set_editor_scale(editor_scale);
		project_preview_browser->initialize();

		// Add the Project Browser Button to the Container Toolbar
		project_preview_button = memnew(FMODProjectBrowserPreviewButton);
		project_preview_button->initialize();
		add_control_to_container(EditorPlugin::CONTAINER_TOOLBAR, project_preview_button);

		if (project_preview_button->get_child_count() == 0)
		{
			project_preview_button->add_child(project_preview_browser);
			project_preview_browser->set_visible(false);
		}

		// Set the Project Browser visible when pressed
		project_preview_button->connect("pressed", Callable(this, "on_project_browser_button_pressed"));

		gizmo_plugin.instantiate();
		add_node_3d_gizmo_plugin(gizmo_plugin);

		inspector_plugin.instantiate();
		inspector_plugin->set_editor_scale(editor_scale);
		add_inspector_plugin(inspector_plugin);
	}

	virtual void _exit_tree() override
	{
		remove_autoload_singleton("FMODRuntime");
		remove_control_from_container(EditorPlugin::CONTAINER_TOOLBAR, project_preview_button);
		project_preview_button->queue_free();
		remove_node_3d_gizmo_plugin(gizmo_plugin);
		remove_inspector_plugin(inspector_plugin);
		project_preview_browser->queue_free();
	}

	void on_project_browser_button_pressed()
	{
		if (!project_preview_browser->is_visible())
		{
			project_preview_browser->popup_centered();
		}
		else
		{
			project_preview_browser->set_visible(false);
		}
	}
};

#endif