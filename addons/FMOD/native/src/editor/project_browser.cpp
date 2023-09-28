#include "project_browser.h"

using namespace godot;

const Size2i BASE_WINDOW_SIZE = Size2i(750, 550);
const int BASE_DPI = 96;
const int STANDARD_MARGIN = 10;

void FMODProjectBrowserTree::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("on_connection_finished"), &FMODProjectBrowserTree::on_connection_finished);
}

void FMODProjectBrowserTree::initialize()
{
	set_allow_reselect(true);
	set_allow_rmb_select(true);
	set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SizeFlags::SIZE_FILL);

	populate_browser();
}

void FMODProjectBrowserTree::populate_browser()
{
	clear();
	root = create_item();
	root->set_text(0, "FMOD Project");
	root->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_PROJECT));

	banks_root = create_item(root);
	banks_root->set_text(0, "Banks");
	banks_root->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_BANK));
	banks_root->set_collapsed(true);

	events_root = create_item(root);
	events_root->set_text(0, "Events");
	events_root->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_EVENT));
	events_root->set_collapsed(true);

	snapshots_root = create_item(root);
	snapshots_root->set_text(0, "Snapshots");
	snapshots_root->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_SNAPSHOT));
	snapshots_root->set_collapsed(true);

	parameters_root = create_item(root);
	parameters_root->set_text(0, "Parameters");
	parameters_root->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_C_PARAMETER));
	parameters_root->set_collapsed(true);

	busses_root = create_item(root);
	busses_root->set_text(0, "Busses");
	busses_root->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_BUS));
	busses_root->set_collapsed(true);

	vcas_root = create_item(root);
	vcas_root->set_text(0, "VCAs");
	vcas_root->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMODIconType::FMOD_ICONTYPE_VCA));
	vcas_root->set_collapsed(true);

	if (client.is_valid())
	{
		if (!client->is_connected("connection_finished", Callable(this, "on_connection_finished")))
		{
			client->connect("connection_finished", Callable(this, "on_connection_finished"));
		}

		if (!client->client_is_connected)
		{
			client->connect_client(this);
		}
	}

	Ref<ProjectCache> cache = FMODStudioEditorModule::get_singleton()->get_project_cache();

	if (cache.is_valid())
	{
		FMODStudioEditorModule::get_singleton()->create_tree_items(this, cache->get_event_tree(), events_root);
		FMODStudioEditorModule::get_singleton()->create_tree_items(this, cache->get_snapshot_tree(), snapshots_root);
		FMODStudioEditorModule::get_singleton()->create_tree_items(this, cache->get_bank_tree(), banks_root);
		FMODStudioEditorModule::get_singleton()->create_tree_items(this, cache->get_bus_tree(), busses_root);
		FMODStudioEditorModule::get_singleton()->create_tree_items(this, cache->get_vca_tree(), vcas_root);
		FMODStudioEditorModule::get_singleton()->create_tree_items(this, cache->get_parameter_tree(), parameters_root);
	}
}

void FMODProjectBrowserTree::update_project_text()
{
	String project_message = "studio.project.filePath.replace(/^.*[\\\\\\/]/, '')";
	String result;
	Error err = client->get_command(project_message, result);
	if (err != Error::FAILED || !result.is_empty())
	{
		root->set_text(0, result);
	}
}

void FMODProjectBrowserTree::on_connection_finished()
{
	if (client->client_is_connected)
	{
		update_project_text();
	}
}

void FMODEditorDiscreteParameter::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("on_value_changed", "value"), &FMODEditorDiscreteParameter::on_value_changed);
}

void FMODEditorDiscreteParameter::initialize(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>& desc, Size2 size)
{
	set_min(desc->get_minimum());
	set_max(desc->get_maximum());
	double value = desc->get_default_value();
	set_value(value);
	set_step(1.0);
	set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);

	set_custom_minimum_size(size);
	parameter_name = desc->get_name();

	connect("value_changed", Callable(this, "on_value_changed"));
}

void FMODEditorDiscreteParameter::on_value_changed(float value)
{
	FMODStudioEditorModule::get_singleton()->set_preview_parameter(parameter_name, value);
}

void FMODEditorLabeledParameter::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("on_item_selected", "value"), &FMODEditorLabeledParameter::on_item_selected);
}

void FMODEditorLabeledParameter::initialize(const Ref<ParameterAsset>& parameter, Size2 size)
{
	set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
	set_custom_minimum_size(size);
	for (int64_t j = 0; j < parameter->get_labels().size(); j++)
	{
		String parameter_label = parameter->get_labels()[j];
		add_item(parameter_label, j);
	}

	select(static_cast<int32_t>(parameter->get_parameter_description()->get_default_value()));
	parameter_name = parameter->get_parameter_description()->get_name();
	connect("item_selected", Callable(this, "on_item_selected"));
}

void FMODEditorLabeledParameter::on_item_selected(int value)
{
	FMODStudioEditorModule::get_singleton()->set_preview_parameter(parameter_name, static_cast<float>(value));
}

void FMODEditorContinuousParameter::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("on_value_changed", "value"), &FMODEditorContinuousParameter::on_value_changed);
}

void FMODEditorContinuousParameter::initialize(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>& desc, Size2 size)
{
	set_min(desc->get_minimum());
	set_max(desc->get_maximum());
	double value = desc->get_default_value();
	set_value(Math::snapped(value, 0.1));
	set_step(0.01);
	set_h_size_flags(Control::SizeFlags::SIZE_FILL | Control::SizeFlags::SIZE_EXPAND);
	set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
	set_custom_minimum_size(size);
	parameter_name = desc->get_name();
	connect("value_changed", Callable(this, "on_value_changed"));
}

void FMODEditorContinuousParameter::on_value_changed(float value)
{
	FMODStudioEditorModule::get_singleton()->set_preview_parameter(parameter_name, value);
}

void FMODProjectBrowserWindow::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("on_about_to_popup"), &FMODProjectBrowserWindow::on_about_to_popup);
	ClassDB::bind_method(D_METHOD("on_close_requested"), &FMODProjectBrowserWindow::on_close_requested);
	ClassDB::bind_method(D_METHOD("on_size_changed"), &FMODProjectBrowserWindow::on_size_changed);
	ClassDB::bind_method(D_METHOD("on_github_link_pressed"), &FMODProjectBrowserWindow::on_github_link_pressed);
	ClassDB::bind_method(D_METHOD("on_tutorials_link_pressed"), &FMODProjectBrowserWindow::on_tutorials_link_pressed);
	ClassDB::bind_method(D_METHOD("on_contact_link_pressed"), &FMODProjectBrowserWindow::on_contact_link_pressed);
	ClassDB::bind_method(D_METHOD("on_search_text_changed", "text"), &FMODProjectBrowserWindow::on_search_text_changed);
	ClassDB::bind_method(D_METHOD("on_cell_selected"), &FMODProjectBrowserWindow::on_cell_selected);
	ClassDB::bind_method(D_METHOD("on_refresh_button_pressed"), &FMODProjectBrowserWindow::on_refresh_button_pressed);
	ClassDB::bind_method(D_METHOD("on_generate_guids_button_pressed"),
			&FMODProjectBrowserWindow::on_generate_guids_button_pressed);
	ClassDB::bind_method(D_METHOD("on_play_button_pressed"), &FMODProjectBrowserWindow::on_play_button_pressed);
	ClassDB::bind_method(D_METHOD("on_stop_button_pressed"), &FMODProjectBrowserWindow::on_stop_button_pressed);
	ClassDB::bind_method(D_METHOD("on_checkbox_toggled", "button_pressed"), &FMODProjectBrowserWindow::on_checkbox_toggled);
	ClassDB::bind_method(D_METHOD("on_event_popup_id_pressed", "id"), &FMODProjectBrowserWindow::on_event_popup_id_pressed);
	ClassDB::bind_method(D_METHOD("on_bank_popup_id_pressed", "id"), &FMODProjectBrowserWindow::on_bank_popup_id_pressed);
	ClassDB::bind_method(D_METHOD("set_editor_scale", "scale"), &FMODProjectBrowserWindow::set_editor_scale);
	ClassDB::bind_method(D_METHOD("initialize"), &FMODProjectBrowserWindow::initialize);
}

void FMODProjectBrowserWindow::popup_menu(PopupType type, Vector2 pos)
{
	switch (type)
	{
		case FMODProjectBrowserWindow::POPUP_EVENT:
		{
			event_popup->popup_on_parent(Rect2(pos, Vector2(0, 0)));
			break;
		}
		case FMODProjectBrowserWindow::POPUP_BANK:
			bank_popup->popup_on_parent(Rect2(pos, Vector2(0, 0)));
			break;
		default:
			break;
	}
}

bool FMODProjectBrowserWindow::update_filter(TreeItem* p_parent, bool p_scroll_to_selected)
{
	if (!p_parent)
	{
		p_parent = project_tree->root;
	}

	if (!p_parent)
	{
		return false;
	}

	bool keep = false;
	for (TreeItem* child = p_parent->get_first_child(); child; child = child->get_next())
	{
		keep = update_filter(child, p_scroll_to_selected) || keep;
	}

	if (!keep)
	{
		keep = filter.is_subsequence_ofn(p_parent->get_text(0));
	}

	p_parent->set_visible(keep);

	p_parent->set_collapsed(!keep && filter != "");

	TreeItem* root = project_tree->root;
	TypedArray<TreeItem> children = root->get_children();

	for (int i = 0; i < children.size(); i++)
	{
		TreeItem* child = Object::cast_to<TreeItem>(children[i].operator godot::Object*());
		if (filter == "")
		{
			collapse_all(child);
		}
		else
		{
			child->set_collapsed(false);
		}
	}

	if (keep)
	{
		if (p_scroll_to_selected)
		{
			project_tree->scroll_to_item(p_parent);
		}
		else if (p_parent->is_selected(0))
		{
			p_parent->deselect(0);
		}
	}

	return keep;
}

void FMODProjectBrowserWindow::collapse_all(TreeItem* p_parent)
{
	if (!p_parent)
	{
		return;
	}

	p_parent->set_collapsed(true);

	for (TreeItem* child = p_parent->get_first_child(); child; child = child->get_next())
	{
		child->set_collapsed(true);
		collapse_all(child);
	}
}

void FMODProjectBrowserWindow::on_about_to_popup()
{
	Ref<FmodTCPClient> client = FMODStudioEditorModule::get_singleton()->client;

	if (client.is_valid())
	{
		if (!client->client_is_connected)
		{
			client->connect_client(this);
		}
	}

	FMODStudioEditorModule::get_singleton()->init();
	FMODStudioEditorModule::get_singleton()->load_all_banks();
	project_tree->populate_browser();
}

void FMODProjectBrowserWindow::on_close_requested()
{
	set_visible(false);
	FMODStudioEditorModule::get_singleton()->unload_all_banks();
	FMODStudioEditorModule::get_singleton()->shutdown();
}

void FMODProjectBrowserWindow::on_size_changed()
{
	parent_vbox_container->set_size(get_size());
}

void FMODProjectBrowserWindow::on_github_link_pressed()
{
	OS::get_singleton()->shell_open(github_url);
}

void FMODProjectBrowserWindow::on_tutorials_link_pressed()
{
	OS::get_singleton()->shell_open(tutorials_url);
}

void FMODProjectBrowserWindow::on_contact_link_pressed()
{
	OS::get_singleton()->shell_open(email_url);
}

void FMODProjectBrowserWindow::on_search_text_changed(const String& text)
{
	filter = text;
	update_filter(project_tree->root);
}

void FMODProjectBrowserWindow::on_cell_selected()
{
	parameters_margin_container->set_visible(false);

	int32_t child_count = parameters_grid_container->get_child_count();

	for (int i = 0; i < child_count; i++)
	{
		parameters_grid_container->get_child(i)->queue_free();
	}

	selected_item = project_tree->get_selected();

	if (!selected_item)
	{
		return;
	}
	if (!selected_item->has_meta("Type"))
	{
		return;
	}

	Ref<EventAsset> current_event;
	if (selected_item->get_meta("Type").operator godot::String() == "event")
	{
		current_event = selected_item->get_meta("Resource");
	}
	else
	{
		return;
	}

	Dictionary parameters = current_event->get_parameters();

	if (parameters.size() == 0)
	{
		return;
	}

	parameters_margin_container->set_visible(true);

	parameters_title_margin_container = memnew(MarginContainer);
	parameters_title_margin_container->add_theme_constant_override("margin_top", 10);
	parameters_title_margin_container->add_theme_constant_override("margin_bottom", 20);
	parameters_grid_container->add_child(parameters_title_margin_container);
	parameters_title_margin_container->set_owner(parameters_grid_container);

	parameters_title_label = memnew(Label);
	parameters_title_label->set_text("Parameters Preview");
	parameters_title_label->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
	parameters_title_margin_container->add_child(parameters_title_label);
	parameters_title_label->set_owner(parameters_title_margin_container);

	for (int i = 0; i < parameters.size(); i++)
	{
		Ref<ParameterAsset> parameter = parameters.values()[i];
		Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> parameter_description =
				parameter->get_parameter_description();
		int flags = parameter_description->get_flags();
		String name = parameter_description->get_name();

		HBoxContainer* parameter_hbox_container = memnew(HBoxContainer);
		parameter_hbox_container->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		parameter_hbox_container->add_theme_constant_override("separation", 50);
		parameters_grid_container->add_child(parameter_hbox_container);
		parameter_hbox_container->set_owner(parameters_grid_container);

		Label* parameter_label = memnew(Label);
		parameter_label->set_text(name);
		parameter_label->set_vertical_alignment(VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
		parameter_label->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_BEGIN);
		parameter_label->set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);

		Size2 size(get_size().x / 5, 0);
		parameter_label->set_custom_minimum_size(size);
		parameter_hbox_container->add_child(parameter_label);
		parameter_label->set_owner(parameter_hbox_container);

		if ((flags & FMOD_STUDIO_PARAMETER_DISCRETE) && !(flags & FMOD_STUDIO_PARAMETER_LABELED))
		{
			FMODEditorDiscreteParameter* spinbox = memnew(FMODEditorDiscreteParameter);
			spinbox->initialize(parameter_description, Size2(get_size().x / 5, 0));

			parameter_hbox_container->add_child(spinbox);
			spinbox->set_owner(parameter_hbox_container);
		}
		else if (flags & FMOD_STUDIO_PARAMETER_LABELED)
		{
			FMODEditorLabeledParameter* option_button = memnew(FMODEditorLabeledParameter);
			option_button->initialize(parameter, Size2(get_size().x / 5, 0));

			parameter_hbox_container->add_child(option_button);
			option_button->set_owner(parameter_hbox_container);
		}
		else
		{
			FMODEditorContinuousParameter* spin_slider = memnew(FMODEditorContinuousParameter);
			spin_slider->initialize(parameter_description, Size2(get_size().x / 5, 0));

			parameter_hbox_container->add_child(spin_slider);
			spin_slider->set_owner(parameter_hbox_container);
		}
	}
}

void FMODProjectBrowserWindow::on_refresh_button_pressed()
{
	FMODStudioEditorModule::get_singleton()->unload_all_banks();
	FMODStudioEditorModule::get_singleton()->load_all_banks();
	Dictionary project_info = FMODStudioEditorModule::get_singleton()->get_project_info_from_banks();
	FMODStudioEditorModule::get_singleton()->generate_cache(project_info);
	FMODStudioEditorModule::get_singleton()->reload_cache_file();
	project_tree->populate_browser();
}

void FMODProjectBrowserWindow::on_generate_guids_button_pressed()
{
	Ref<FmodTCPClient> client = FMODStudioEditorModule::get_singleton()->client;

	if (!client.is_valid())
	{
		return;
	}

	if (!client->client_is_connected)
	{
		return;
	}

	const String script_path =
			ProjectSettings::get_singleton()->globalize_path("res://addons/FMOD/editor/tools/export_guids.js");
	String result;
	client->get_command("var fmodModule = studio.system.require('" + script_path + "');", result);

	const String guids_path = "res://addons/FMOD/editor/fmod_guids.gd";

	Error err = client->get_command(
			"fmodModule.writeHeaderFile('" +
					ProjectSettings::get_singleton()->globalize_path(guids_path) + "');",
			result);

	if (err == Error::FAILED)
	{
		UtilityFunctions::push_warning("[FMOD] Failed to generate GUIDs. Please verify that an FMOD Studio project is open and the path to the Banks in the Project Settings is correct.");
	}
	else
	{
		ResourceLoader::get_singleton()->load(guids_path, "",
				ResourceLoader::CacheMode::CACHE_MODE_REPLACE);
		UtilityFunctions::print(String("[FMOD] GUIDs generated in {0}").format(Array::make(guids_path)));
	}
}

void FMODProjectBrowserWindow::on_play_button_pressed()
{
	if (!selected_item)
	{
		return;
	}

	if (selected_item->has_meta("Type"))
	{
		if (selected_item->get_meta("Type").operator godot::String() == "event")
		{
			if (selected_item->has_meta("Resource"))
			{
				Ref<EventAsset> event = selected_item->get_meta("Resource");
				FMODStudioEditorModule::get_singleton()->play_event(event->get_guid());
			}
		}
	}
}

void FMODProjectBrowserWindow::on_stop_button_pressed()
{
	FMODStudioEditorModule::get_singleton()->stop_events(allow_fadeout);
}

void FMODProjectBrowserWindow::on_checkbox_toggled(bool button_pressed)
{
	allow_fadeout = button_pressed;
}

void FMODProjectBrowserWindow::on_event_popup_id_pressed(int32_t id)
{
	if (!selected_item)
	{
		return;
	}

	Ref<FmodTCPClient> client = FMODStudioEditorModule::get_singleton()->client;

	if (!client.is_valid())
	{
		return;
	}

	switch (id)
	{
		case EventPopupItems::EVENT_POPUP_COPY_PATH:
		{
			if (selected_item->has_meta("Path"))
			{
				String path = selected_item->get_meta("Path");
				DisplayServer::get_singleton()->clipboard_set(path);
			}
		}
		break;
		case EventPopupItems::EVENT_POPUP_COPY_GUID:
		{
			if (selected_item->has_meta("Guid"))
			{
				String guid = selected_item->get_meta("Guid");
				DisplayServer::get_singleton()->clipboard_set(guid);
			}
		}
		break;
		case EventPopupItems::EVENT_POPUP_OPEN_IN_STUDIO:
		{
			if (selected_item->has_meta("Path"))
			{
				String path = selected_item->get_meta("Path");
				String result;
				client->get_command("studio.window.navigateTo(studio.project.lookup('" + path + "'))", result);
			}
		}
		break;
		default:
			break;
	}
}

void FMODProjectBrowserWindow::on_bank_popup_id_pressed(int32_t id)
{
	if (!selected_item)
	{
		return;
	}

	Ref<FmodTCPClient> client = FMODStudioEditorModule::get_singleton()->client;

	if (!client.is_valid())
	{
		return;
	}

	switch (id)
	{
		case BankPopupItems::BANK_POPUP_COPY_PATH:
		{
			if (selected_item->has_meta("Path"))
			{
				String path = selected_item->get_meta("Path");
				DisplayServer::get_singleton()->clipboard_set(path);
			}
		}
		break;
		case BankPopupItems::BANK_POPUP_COPY_GUID:
		{
			if (selected_item->has_meta("Guid"))
			{
				String guid = selected_item->get_meta("Guid");
				DisplayServer::get_singleton()->clipboard_set(guid);
			}
		}
		break;
		case BankPopupItems::BANK_POPUP_BUILD:
		{
			String name = selected_item->get_text(0);
			String result;
			client->get_command(String("studio.project.build({ banks: '{0}' });").format(Array::make(name)), result);
		}
		break;
		default:
			break;
	}
}

void FMODProjectBrowserWindow::_input(const Ref<InputEvent>& event)
{
	if (is_visible())
	{
		Ref<InputEventMouseButton> mouse_button_event = event;
		if (mouse_button_event.is_valid())
		{
			if (mouse_button_event->get_button_index() == MouseButton::MOUSE_BUTTON_RIGHT && mouse_button_event->is_pressed())
			{
				if (selected_item)
				{
					if (selected_item->has_meta("Type"))
					{
						String meta = selected_item->get_meta("Type");
						if (meta == String("event"))
						{
							if (Rect2(project_tree->get_global_position(), project_tree->get_size()).has_point(mouse_button_event->get_global_position()))
							{
								popup_menu(PopupType::POPUP_EVENT, mouse_button_event->get_position());
							}
						}
						else if (meta == String("bank"))
						{
							if (Rect2(project_tree->get_global_position(), project_tree->get_size()).has_point(mouse_button_event->get_global_position()))
							{
								popup_menu(PopupType::POPUP_BANK, mouse_button_event->get_position());
							}
						}
					}
				}
			}
		}
	}
}

void FMODProjectBrowserWindow::set_editor_scale(float scale)
{
	editor_scale = scale;
}

void FMODProjectBrowserWindow::initialize()
{
	set_initial_position(Window::WindowInitialPosition::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
	set_visible(false);
	set_wrap_controls(true);
	set_title("FMOD Project Browser");
	set_exclusive(true);
	connect("about_to_popup", Callable(this, "on_about_to_popup"));
	connect("close_requested", Callable(this, "on_close_requested"));
	connect("confirmed", Callable(this, "on_close_requested"));
	connect("size_changed", Callable(this, "on_size_changed"));

	Size2 window_size = BASE_WINDOW_SIZE;
	DisplayServer* display_server = DisplayServer::get_singleton();

	if (display_server)
	{
		int32_t dpi = display_server->screen_get_dpi();

		if (dpi != 72)
		{
			dpi_scaling_factor = dpi / BASE_DPI;
			window_size *= dpi_scaling_factor;
		}

		window_size *= editor_scale;
	}

	parent_vbox_container = memnew(VBoxContainer);
	parent_vbox_container->set_size(window_size);
	parent_vbox_container->set_custom_minimum_size(window_size);
	this->add_child(parent_vbox_container);
	parent_vbox_container->set_owner(this);

	top_margin_container = memnew(MarginContainer);
	top_margin_container->add_theme_constant_override("margin_left", STANDARD_MARGIN);
	top_margin_container->add_theme_constant_override("margin_right", STANDARD_MARGIN);
	top_margin_container->add_theme_constant_override("margin_top", STANDARD_MARGIN);
	top_margin_container->add_theme_constant_override("margin_bottom", STANDARD_MARGIN - 10);
	parent_vbox_container->add_child(top_margin_container);
	top_margin_container->set_owner(parent_vbox_container);

	top_vbox_container = memnew(VBoxContainer);
	top_margin_container->add_child(top_vbox_container);
	top_vbox_container->set_owner(top_margin_container);

	top_hbox_container = memnew(HBoxContainer);
	top_vbox_container->add_child(top_hbox_container);
	top_hbox_container->set_owner(top_vbox_container);

	button_container = memnew(HBoxContainer);
	button_container->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	top_hbox_container->add_child(button_container);
	button_container->set_owner(top_hbox_container);

	refresh_button = memnew(Button);
	refresh_button->set_text("Refresh Project");
	refresh_button->connect("pressed", Callable(this, "on_refresh_button_pressed"));
	generate_guids_button = memnew(Button);
	generate_guids_button->set_text("Generate GUIDs");
	generate_guids_button->connect("pressed", Callable(this, "on_generate_guids_button_pressed"));

	button_container->add_child(refresh_button);
	button_container->add_child(generate_guids_button);
	refresh_button->set_owner(button_container);
	generate_guids_button->set_owner(button_container);

	links_margin_container = memnew(MarginContainer);
	links_margin_container->add_theme_constant_override("margin_right", 10);
	top_hbox_container->add_child(links_margin_container);
	links_margin_container->set_owner(top_hbox_container);

	links_hbox_container = memnew(HBoxContainer);
	links_hbox_container->set_h_size_flags(Control::SIZE_FILL);
	links_hbox_container->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
	links_hbox_container->set_alignment(BoxContainer::AlignmentMode::ALIGNMENT_END);
	links_margin_container->add_child(links_hbox_container);
	links_hbox_container->set_owner(links_margin_container);

	link_github = memnew(LinkButton);
	link_github->set_text("GitHub");
	link_github->connect("pressed", Callable(this, "on_github_link_pressed"));
	link_tutorials = memnew(LinkButton);
	link_tutorials->set_text("Tutorials");
	link_tutorials->connect("pressed", Callable(this, "on_tutorials_link_pressed"));
	link_contact = memnew(LinkButton);
	link_contact->set_text("Contact");
	link_contact->connect("pressed", Callable(this, "on_contact_link_pressed"));

	VSeparator* separator[2] = { memnew(VSeparator), memnew(VSeparator) };
	links_hbox_container->add_child(link_github);
	links_hbox_container->add_child(separator[0]);
	links_hbox_container->add_child(link_tutorials);
	links_hbox_container->add_child(separator[1]);
	links_hbox_container->add_child(link_contact);
	link_github->set_owner(links_hbox_container);
	separator[0]->set_owner(links_hbox_container);
	link_tutorials->set_owner(links_hbox_container);
	separator[1]->set_owner(links_hbox_container);
	link_contact->set_owner(links_hbox_container);

	search_text = memnew(LineEdit);
	search_text->set_placeholder("Search FMOD Project...");
	search_text->set_caret_blink_enabled(true);
	search_text->set_caret_blink_interval(0.5);
	search_text->connect("text_changed", Callable(this, "on_search_text_changed"));
	top_vbox_container->add_child(search_text);
	search_text->set_owner(top_vbox_container);

	tree_margin_container = memnew(MarginContainer);
	tree_margin_container->set_h_size_flags(Control::SizeFlags::SIZE_FILL);
	tree_margin_container->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	tree_margin_container->add_theme_constant_override("margin_left", STANDARD_MARGIN);
	tree_margin_container->add_theme_constant_override("margin_right", STANDARD_MARGIN);
	parent_vbox_container->add_child(tree_margin_container);
	tree_margin_container->set_owner(parent_vbox_container);

	tree_hbox_container = memnew(HBoxContainer);
	tree_hbox_container->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	tree_margin_container->add_child(tree_hbox_container);
	tree_hbox_container->set_owner(tree_margin_container);

	project_tree = memnew(FMODProjectBrowserTree);
	project_tree->client = FMODStudioEditorModule::get_singleton()->client;
	project_tree->initialize();
	project_tree->connect("cell_selected", Callable(this, "on_cell_selected"));
	tree_hbox_container->add_child(project_tree);
	project_tree->set_owner(tree_hbox_container);

	event_popup = memnew(PopupMenu);
	event_popup->add_item("Copy Path", EventPopupItems::EVENT_POPUP_COPY_PATH);
	event_popup->add_item("Copy GUID", EventPopupItems::EVENT_POPUP_COPY_GUID);
	event_popup->add_item("Open in Studio", EventPopupItems::EVENT_POPUP_OPEN_IN_STUDIO);
	event_popup->connect("id_pressed", Callable(this, "on_event_popup_id_pressed"));
	project_tree->add_child(event_popup);
	event_popup->set_owner(project_tree);

	bank_popup = memnew(PopupMenu);
	bank_popup->add_item("Copy Path", BankPopupItems::BANK_POPUP_COPY_PATH);
	bank_popup->add_item("Copy GUID", BankPopupItems::BANK_POPUP_COPY_GUID);
	bank_popup->add_item("Build Bank", BankPopupItems::BANK_POPUP_BUILD);
	bank_popup->connect("id_pressed", Callable(this, "on_bank_popup_id_pressed"));
	project_tree->add_child(bank_popup);
	bank_popup->set_owner(project_tree);

	parameters_margin_container = memnew(MarginContainer);
	parameters_margin_container->add_theme_constant_override("margin_left", STANDARD_MARGIN);
	parameters_margin_container->add_theme_constant_override("margin_top", STANDARD_MARGIN);
	parameters_margin_container->add_theme_constant_override("margin_bottom", STANDARD_MARGIN);
	parameters_margin_container->set_visible(false);

	tree_hbox_container->add_child(parameters_margin_container);
	parameters_margin_container->set_owner(tree_hbox_container);

	parameters_grid_container = memnew(VBoxContainer);
	parameters_grid_container->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_BEGIN);
	parameters_grid_container->add_theme_constant_override("h_separation", 20);
	parameters_grid_container->add_theme_constant_override("v_separation", 10);
	parameters_margin_container->add_child(parameters_grid_container);
	parameters_grid_container->set_owner(parameters_margin_container);

	bottom_vbox_container = memnew(VBoxContainer);
	bottom_vbox_container->set_h_size_flags(Control::SizeFlags::SIZE_FILL);
	bottom_vbox_container->set_v_size_flags(Control::SizeFlags::SIZE_FILL);
	parent_vbox_container->add_child(bottom_vbox_container);
	bottom_vbox_container->set_owner(parent_vbox_container);

	bottom_hbox_container = memnew(HBoxContainer);
	bottom_vbox_container->add_child(bottom_hbox_container);
	bottom_hbox_container->set_owner(bottom_vbox_container);

	bottom_margin_container = memnew(MarginContainer);
	bottom_margin_container->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	bottom_margin_container->add_theme_constant_override("margin_left", 15);
	bottom_margin_container->add_theme_constant_override("margin_right", 15);
	bottom_margin_container->add_theme_constant_override("margin_top", 15);
	bottom_hbox_container->add_child(bottom_margin_container);
	bottom_margin_container->set_owner(bottom_hbox_container);

	buttons_parameters_vbox = memnew(VBoxContainer);
	buttons_parameters_vbox->add_theme_constant_override("separation", 20);
	bottom_margin_container->add_child(buttons_parameters_vbox);
	buttons_parameters_vbox->set_owner(bottom_margin_container);

	buttons_hbox = memnew(HBoxContainer);
	buttons_hbox->add_theme_constant_override("separation", 10);
	buttons_parameters_vbox->add_child(buttons_hbox);
	buttons_hbox->set_owner(buttons_parameters_vbox);

	play_button = memnew(Button);
	play_button->set_text("Play Event");
	play_button->connect("pressed", Callable(this, "on_play_button_pressed"));
	buttons_hbox->add_child(play_button);
	play_button->set_owner(buttons_hbox);

	stop_button = memnew(Button);
	stop_button->set_text("Stop");
	stop_button->connect("pressed", Callable(this, "on_stop_button_pressed"));
	buttons_hbox->add_child(stop_button);
	stop_button->set_owner(buttons_hbox);

	allow_fadeout_checkbox = memnew(CheckBox);
	allow_fadeout_checkbox->set_text("Allow Fadeout");
	allow_fadeout_checkbox->connect("toggled", Callable(this, "on_checkbox_toggled"));
	buttons_hbox->add_child(allow_fadeout_checkbox);
	allow_fadeout_checkbox->set_owner(buttons_hbox);

	set_min_size(parent_vbox_container->get_custom_minimum_size());
	child_controls_changed();
}