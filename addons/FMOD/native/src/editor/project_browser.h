#ifndef PROJECT_BROWSER_H
#define PROJECT_BROWSER_H

#include "fmod_assets.h"
#include "fmod_studio_editor_module.h"
#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/editor_spin_slider.hpp>
#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/link_button.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/tree_item.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/v_separator.hpp>

using namespace godot;

class FMODProjectBrowserPreviewButton : public Button
{
	GDCLASS(FMODProjectBrowserPreviewButton, Button);

protected:
	static void _bind_methods(){};

public:
	FMODProjectBrowserPreviewButton() = default;
	~FMODProjectBrowserPreviewButton() = default;

	void initialize()
	{
		set_text("FMOD");
		set_button_icon(FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_PROJECT));
	}
};

class FMODProjectBrowserTree : public Tree
{
	GDCLASS(FMODProjectBrowserTree, Tree);

protected:
	static void _bind_methods();

private:
public:
	TreeItem* root = nullptr;
	TreeItem* events_root = nullptr;
	TreeItem* banks_root = nullptr;
	TreeItem* snapshots_root = nullptr;
	TreeItem* busses_root = nullptr;
	TreeItem* vcas_root = nullptr;
	TreeItem* parameters_root = nullptr;
	Ref<FmodTCPClient> client;

	void initialize();
	void populate_browser();
	void update_project_text();
	void on_connection_finished();
};

class FMODEditorDiscreteParameter : public SpinBox
{
	GDCLASS(FMODEditorDiscreteParameter, SpinBox);

protected:
	static void _bind_methods();

public:
	String parameter_name;
	void initialize(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>& desc, Size2 size);
	void on_value_changed(float value);
};

class FMODEditorLabeledParameter : public OptionButton
{
	GDCLASS(FMODEditorLabeledParameter, OptionButton);

protected:
	static void _bind_methods();

public:
	String parameter_name;
	void initialize(const Ref<ParameterAsset>& parameter, Size2 size);
	void on_item_selected(int value);
};

class FMODEditorContinuousParameter : public EditorSpinSlider
{
	GDCLASS(FMODEditorContinuousParameter, EditorSpinSlider);

protected:
	static void _bind_methods();

public:
	String parameter_name;
	void initialize(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>& desc, Size2 size);
	void on_value_changed(float value);
};

class FMODProjectBrowserWindow : public AcceptDialog
{
	GDCLASS(FMODProjectBrowserWindow, AcceptDialog);

	enum PopupType
	{
		POPUP_EVENT,
		POPUP_BANK
	};

	enum EventPopupItems
	{
		EVENT_POPUP_COPY_PATH,
		EVENT_POPUP_COPY_GUID,
		EVENT_POPUP_OPEN_IN_STUDIO
	};

	enum BankPopupItems
	{
		BANK_POPUP_COPY_PATH,
		BANK_POPUP_COPY_GUID,
		BANK_POPUP_BUILD
	};

protected:
	static void _bind_methods();

private:
	VBoxContainer* parent_vbox_container = nullptr;
	MarginContainer* top_margin_container = nullptr;
	VBoxContainer* top_vbox_container = nullptr;
	HBoxContainer* top_hbox_container = nullptr;
	HBoxContainer* button_container = nullptr;
	Button* refresh_button = nullptr;
	Button* generate_guids_button = nullptr;
	MarginContainer* links_margin_container = nullptr;
	HBoxContainer* links_hbox_container = nullptr;
	LinkButton* link_github = nullptr;
	LinkButton* link_tutorials = nullptr;
	LinkButton* link_contact = nullptr;
	LineEdit* search_text = nullptr;
	MarginContainer* tree_margin_container = nullptr;
	HBoxContainer* tree_hbox_container = nullptr;
	FMODProjectBrowserTree* project_tree = nullptr;
	MarginContainer* parameters_margin_container = nullptr;
	VBoxContainer* parameters_grid_container = nullptr;
	MarginContainer* parameters_title_margin_container = nullptr;
	Label* parameters_title_label = nullptr;
	VBoxContainer* bottom_vbox_container = nullptr;
	HBoxContainer* bottom_hbox_container = nullptr;
	MarginContainer* bottom_margin_container = nullptr;
	VBoxContainer* buttons_parameters_vbox = nullptr;
	HBoxContainer* buttons_hbox = nullptr;
	Button* play_button = nullptr;
	Button* stop_button = nullptr;
	CheckBox* allow_fadeout_checkbox = nullptr;
	TreeItem* selected_item = nullptr;
	PopupMenu* event_popup = nullptr;
	PopupMenu* bank_popup = nullptr;
	String filter = "";
	bool allow_fadeout = false;
	const String github_url = "https://github.com/alessandrofama/fmod-for-godot/";
	const String tutorials_url = "https://alessandrofama.com/tutorials/fmod/godot/";
	const String email_url = "mailto:me@alessandrofama.com?subject=FMOD%20for%20Godot%20Integration";
	int dpi_scaling_factor{ 1 };
	float editor_scale{ 1.0f };

	void popup_menu(PopupType type, Vector2 pos);
	bool update_filter(TreeItem* p_parent = nullptr, bool p_scroll_to_selected = false);
	void collapse_all(TreeItem* p_parent = nullptr);
	void on_about_to_popup();
	void on_close_requested();
	void on_size_changed();
	void on_github_link_pressed();
	void on_tutorials_link_pressed();
	void on_contact_link_pressed();
	void on_search_text_changed(const String& text);
	void on_cell_selected();
	void on_refresh_button_pressed();
	void on_generate_guids_button_pressed();
	void on_play_button_pressed();
	void on_stop_button_pressed();
	void on_checkbox_toggled(bool button_pressed);
	void on_event_popup_id_pressed(int32_t id);
	void on_bank_popup_id_pressed(int32_t id);

public:
	void _input(const Ref<InputEvent>& event) override;
	void set_editor_scale(float scale);
	void initialize();
};

#endif // PROJECT_BROWSER_H