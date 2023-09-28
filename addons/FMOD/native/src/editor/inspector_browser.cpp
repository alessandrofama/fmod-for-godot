#include "inspector_browser.h"
#include <functional>

using namespace godot;

const int BASE_DPI = 96;

void FMODEditorInspectorTree::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("on_size_changed"), &FMODEditorInspectorTree::on_size_changed);
	ClassDB::bind_method(D_METHOD("on_item_collapsed"), &FMODEditorInspectorTree::on_item_collapsed);
	ClassDB::bind_method(D_METHOD("on_text_changed"), &FMODEditorInspectorTree::on_text_changed);
}

void FMODEditorInspectorTree::initialize(FMODStudioEditorModule::FMODAssetType item_type)
{
	type = item_type;
	window = Object::cast_to<FMODEditorInspector>(get_parent()->get_parent());
	window->connect("size_changed", Callable(this, "on_size_changed"));

	search_text = window->search_text;

	connect("item_collapsed", Callable(this, "on_item_collapsed"));

	search_text->connect("text_changed", Callable(this, "on_text_changed"));

	project_cache = FMODStudioEditorModule::get_singleton()->get_project_cache();

	if (project_cache.is_valid())
	{
		populate_browser();
	}
}

void FMODEditorInspectorTree::populate_browser()
{
	if (root_item)
	{
		clear();
	}

	root_item = create_item(nullptr);
	root_item->set_text(0, "Root");
	set_hide_root(true);
	set_hide_folding(false);
	set_allow_reselect(false);

	if (filter == "")
	{
		switch (type)
		{
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_EVENT:
			{
				events_root = create_item(root_item);
				events_root->set_text(0, "Events");
				events_root->set_icon(
						0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_CLOSED));
				events_root->set_meta("Type", "folder");
				snapshots_root = create_item(root_item);
				snapshots_root->set_text(0, "Snapshots");
				snapshots_root->set_icon(
						0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_CLOSED));
				window->search_text->set_placeholder("Search Events...");
				FMODStudioEditorModule::get_singleton()->create_tree_items(this, project_cache->get_event_tree(),
						events_root);
				FMODStudioEditorModule::get_singleton()->create_tree_items(this, project_cache->get_snapshot_tree(),
						snapshots_root);
				events_root->set_visible(true);
				snapshots_root->set_visible(true);
			}
			break;
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BANK:
			{
				banks_root = create_item(root_item);
				banks_root->set_text(0, "Banks");
				banks_root->set_icon(
						0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_CLOSED));
				window->search_text->set_placeholder("Search Banks...");
				FMODStudioEditorModule::get_singleton()->create_tree_items(this, project_cache->get_bank_tree(),
						banks_root);
				banks_root->set_visible(true);
			}
			break;
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BUS:
			{
				busses_root = create_item(root_item);
				busses_root->set_text(0, "Busses");
				busses_root->set_icon(
						0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_CLOSED));
				window->search_text->set_placeholder("Search Busses...");
				FMODStudioEditorModule::get_singleton()->create_tree_items(this, project_cache->get_bus_tree(),
						busses_root);
				busses_root->set_visible(true);
			}
			break;
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_VCA:
			{
				vcas_root = create_item(root_item);
				vcas_root->set_text(0, "VCAs");
				vcas_root->set_icon(
						0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_CLOSED));
				window->search_text->set_placeholder("Search VCAs...");
				FMODStudioEditorModule::get_singleton()->create_tree_items(this, project_cache->get_vca_tree(), vcas_root);
				vcas_root->set_visible(true);
			}
			break;
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_GLOBAL_PARAMETER:
			{
				parameters_root = create_item(root_item);
				parameters_root->set_text(0, "Global Parameters");
				parameters_root->set_icon(
						0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_CLOSED));
				window->search_text->set_placeholder("Search Parameters...");
				FMODStudioEditorModule::get_singleton()->create_tree_items(this, project_cache->get_parameter_tree(),
						parameters_root);
				parameters_root->set_visible(true);

				// Setup local parameters
				{
					local_parameters_root = create_item(root_item);
					local_parameters_root->set_text(0, "Local Parameters");
					local_parameters_root->set_icon(
							0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_CLOSED));
					FMODStudioEditorModule::get_singleton()->create_tree_items(this, project_cache->get_event_tree(),
							local_parameters_root);
					local_parameters_root->set_visible(true);

					// note(alex): iterating through the Event Tree to get local Event Parameters and adding them as children to the respective Events
					std::function<void(TreeItem*)> get_event_parameters = [&get_event_parameters](TreeItem* tree_item)
					{
						while (tree_item)
						{
							if (tree_item->has_meta("Type"))
							{
								String type = tree_item->get_meta("Type");
								if (type == String("event"))
								{
									Ref<EventAsset> event_asset = tree_item->get_meta("Resource");

									if (event_asset.is_valid())
									{
										// note(alex): setting the Resource meta for the Event TreeItem to null, so it can't be selected
										tree_item->set_meta("Resource", Variant());

										Dictionary parameters = event_asset->get_parameters();

										for (size_t i = 0; i < parameters.size(); i++)
										{
											Ref<ParameterAsset> param_asset = parameters.values()[i];

											if (param_asset.is_valid())
											{
												String name = param_asset->get_name();
												String path = param_asset->get_path();
												TreeItem* child = tree_item->create_child();
												child->set_text(0, name);
												child->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_C_PARAMETER));
												child->set_meta("Type", "parameter");
												child->set_meta("Path", path);
												child->set_meta("Resource", param_asset);
											}
										}
									}
								}
							}

							if (tree_item->get_child_count() > 0)
							{
								get_event_parameters(tree_item->get_first_child());
							}

							tree_item = tree_item->get_next();
						}
					};

					TreeItem* item = local_parameters_root->get_first_child();
					get_event_parameters(item);
				}
			}
			break;
			default:
				break;
		}
	}
}

void FMODEditorInspectorTree::on_text_changed(const String& text_filter)
{
	filter = text_filter;
	update_filter(root_item, false);
}

bool FMODEditorInspectorTree::update_filter(TreeItem* p_parent, bool p_scroll_to_selected)
{
	if (!p_parent)
	{
		p_parent = root_item;
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

	TreeItem* root = root_item;
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
			scroll_to_item(p_parent);
		}
		else if (p_parent->is_selected(0))
		{
			p_parent->deselect(0);
		}
	}

	return keep;
}

void FMODEditorInspectorTree::collapse_all(TreeItem* p_parent)
{
	if (!p_parent)
	{
		return;
	}

	switch (type)
	{
		case FMODStudioEditorModule::FMOD_ASSETTYPE_EVENT:
		{
			if (p_parent->get_text(0) == String("Events"))
			{
				p_parent->set_collapsed(false);
			}
		}
		break;
		case FMODStudioEditorModule::FMOD_ASSETTYPE_SNAPSHOT:
		{
			if (p_parent->get_text(0) == String("Snapshots"))
			{
				p_parent->set_collapsed(false);
			}
		}
		break;
		case FMODStudioEditorModule::FMOD_ASSETTYPE_BANK:
		{
			if (p_parent->get_text(0) == String("Banks"))
			{
				p_parent->set_collapsed(false);
			}
		}
		break;
		case FMODStudioEditorModule::FMOD_ASSETTYPE_BUS:
		{
			if (p_parent->get_text(0) == String("Busses"))
			{
				p_parent->set_collapsed(false);
			}
		}
		break;
		case FMODStudioEditorModule::FMOD_ASSETTYPE_VCA:
		{
			if (p_parent->get_text(0) == String("VCAs"))
			{
				p_parent->set_collapsed(false);
			}
		}
		break;
		case FMODStudioEditorModule::FMOD_ASSETTYPE_GLOBAL_PARAMETER:
		{
			if (p_parent->get_text(0) == String("Global Parameters"))
			{
				p_parent->set_collapsed(false);
			}
		}
		break;
		case FMODStudioEditorModule::FMOD_ASSETTYPE_FOLDER:
			break;
		default:
			break;
	}

	for (TreeItem* child = p_parent->get_first_child(); child; child = child->get_next())
	{
		child->set_collapsed(true);
		collapse_all(child);
	}
}

void FMODEditorInspectorTree::on_size_changed()
{
	window->root_vbox->set_size(window->get_size());
}

void FMODEditorInspectorTree::on_item_collapsed(Object* item)
{
	TreeItem* tree_item = Object::cast_to<TreeItem>(item);

	if (!tree_item)
	{
		return;
	}

	if (tree_item->has_meta("Type"))
	{
		String type = tree_item->get_meta("Type");

		if (type == String("folder"))
		{
			if (tree_item->is_collapsed())
			{
				tree_item->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_CLOSED));
			}
			else
			{
				tree_item->set_icon(0, FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_FOLDER_OPENED));
			}
		}
	}
}

void FMODEditorInspector::_bind_methods()
{
}

void FMODEditorInspector::initialize()
{
	set_name("Window");
	set_title("FMOD Browser");
	set_disable_3d(true);
	set_exclusive(true);

	root_vbox = memnew(VBoxContainer);
	root_vbox->set_name("ParentVBoxContainer");

	Size2 window_size = Size2(400, 680);
	DisplayServer* display_server = DisplayServer::get_singleton();

	if (display_server)
	{
		int32_t dpi = display_server->screen_get_dpi();

		if (dpi != 72)
		{
			int dpi_scaling_factor = 1;
			dpi_scaling_factor = dpi / BASE_DPI;
			window_size *= dpi_scaling_factor;
		}

		window_size *= editor_scale;
	}

	root_vbox->set_size(window_size);
	root_vbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	root_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	add_child(root_vbox);

	search_vbox = memnew(VBoxContainer);
	search_vbox->set_name("TopVBoxContainer");
	root_vbox->add_child(search_vbox);

	search_text = memnew(LineEdit);
	search_text->set_name("SearchText");
	search_vbox->add_child(search_text);

	tree = memnew(FMODEditorInspectorTree);
	tree->set_name("ProjectObjectsTree");
	tree->set_allow_reselect(true);
	tree->set_hide_folding(true);
	tree->set_hide_root(true);
	tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	root_vbox->add_child(tree);
}

void FMODEditorInspector::set_editor_scale(float editor_scale)
{
	this->editor_scale = editor_scale;
}

void FMODEditorInspectorProperty::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("init", "asset_type"), &FMODEditorInspectorProperty::init);
	ClassDB::bind_method(D_METHOD("on_button_pressed"), &FMODEditorInspectorProperty::on_button_pressed);
	ClassDB::bind_method(D_METHOD("on_item_selected"), &FMODEditorInspectorProperty::on_item_selected);
	ClassDB::bind_method(D_METHOD("reset"), &FMODEditorInspectorProperty::reset);
	ClassDB::bind_method(D_METHOD("on_event_popup_id_pressed", "id"), &FMODEditorInspectorProperty::on_event_popup_id_pressed);
}

void FMODEditorInspectorProperty::popup_menu(PopupType type, Vector2 pos)
{
	switch (type)
	{
		case FMODEditorInspectorProperty::POPUP_EVENT:
		{
			event_popup->popup_on_parent(Rect2(pos, Vector2(0, 0)));
		}
		break;
		default:
			break;
	}
}

void FMODEditorInspectorProperty::init(FMODStudioEditorModule::FMODAssetType asset_type)
{
	type = asset_type;
	property_control = memnew(Button);
	inspector_browser = memnew(FMODEditorInspector);
	inspector_browser->set_editor_scale(editor_scale);
	inspector_browser->initialize();

	add_child(property_control);
	property_control->set_clip_text(true);

	inspector_browser->connect("close_requested", Callable(this, "reset"));
	inspector_browser->connect("confirmed", Callable(this, "reset"));
	FMODEditorInspectorTree* tree = inspector_browser->tree;
	tree->initialize(type);

	switch (type)
	{
		case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_EVENT:
			property_control->set_text("Select Event...");
			icon = FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_EVENT);
			break;
		case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BANK:
			property_control->set_text("Select Bank...");
			icon = FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_BANK);
			break;
		case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BUS:
			property_control->set_text("Select Bus...");
			icon = FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_BUS);
			break;
		case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_VCA:
			property_control->set_text("Select VCA...");
			icon = FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_VCA);
			break;
		case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_GLOBAL_PARAMETER:
			property_control->set_text("Select Parameter...");
			icon = FMODStudioEditorModule::get_singleton()->get_icon(FMODStudioEditorModule::FMOD_ICONTYPE_C_PARAMETER);
			break;
		default:
			break;
	}

	tree->connect("item_selected", Callable(this, "on_item_selected"));
	property_control->connect("pressed", Callable(this, "on_button_pressed"));

	event_popup = memnew(PopupMenu);
	event_popup->add_item("Copy Path", EventPopupItems::EVENT_POPUP_COPY_PATH);
	event_popup->add_item("Copy GUID", EventPopupItems::EVENT_POPUP_COPY_GUID);
	event_popup->add_item("Open in Studio", EventPopupItems::EVENT_POPUP_OPEN_IN_STUDIO);
	event_popup->connect("id_pressed", Callable(this, "on_event_popup_id_pressed"));
	add_child(event_popup);
	event_popup->set_owner(this);
}

void FMODEditorInspectorProperty::_input(const Ref<InputEvent>& event)
{
	Ref<InputEventMouseButton> mouse_button_event = event;
	if (mouse_button_event.is_valid())
	{
		if (mouse_button_event->get_button_index() == MouseButton::MOUSE_BUTTON_RIGHT && mouse_button_event->is_pressed())
		{
			if (Rect2(get_global_position(), get_size()).has_point(mouse_button_event->get_position()))
			{
				if (type == FMODStudioEditorModule::FMOD_ASSETTYPE_EVENT || type == FMODStudioEditorModule::FMOD_ASSETTYPE_SNAPSHOT)
				{
					if (current_value.is_valid())
					{
						popup_menu(PopupType::POPUP_EVENT, mouse_button_event->get_position());
					}
				}
			}
		}
	}
}

void FMODEditorInspectorProperty::_update_property()
{
	Variant new_value = get_edited_object()->get(get_edited_property());

	if (new_value.get_type() == Variant::NIL)
	{
		close_popup();
		get_edited_object()->notify_property_list_changed();
		return;
	}
	if (new_value.get_type() == Variant::OBJECT)
	{
		Object* obj = new_value;
		if (obj == nullptr)
		{
			close_popup();
			get_edited_object()->notify_property_list_changed();
			return;
		}
	}

	updating = true;

	current_value = new_value;

	if (new_value.get("guid").get_type() == Variant::NIL)
	{
		updating = false;
		return;
	}

	if (!new_value.get("name").operator String().is_empty())
	{
		property_control->set_text(new_value.get("name").operator String());
		property_control->set_button_icon(icon);
	}
	else
	{
		switch (type)
		{
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_EVENT:
				property_control->set_text("Select Event...");
				break;
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BANK:
				property_control->set_text("Select Bank...");
				break;
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BUS:
				property_control->set_text("Select Bus...");
				break;
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_VCA:
				property_control->set_text("Select VCA...");
				break;
			case FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_GLOBAL_PARAMETER:
				property_control->set_text("Select Parameter...");
				break;
			default:
				break;
		}
		property_control->set_button_icon(Ref<Texture2D>());
	}

	close_popup();
	get_edited_object()->notify_property_list_changed();
	updating = false;
}

void FMODEditorInspectorProperty::on_button_pressed()
{
	if (updating)
	{
		return;
	}
	inspector_browser->set_min_size(
			Vector2(inspector_browser->root_vbox->get_size().x, inspector_browser->root_vbox->get_size().y));
	open_popup();
}

void FMODEditorInspectorProperty::on_item_selected()
{
	FMODEditorInspectorTree* tree = inspector_browser->tree;
	TreeItem* selected_item = tree->get_selected();
	if (selected_item)
	{
		if (!selected_item->has_meta("Type"))
		{
			return;
		}

		String type = selected_item->get_meta("Type");

		if (type == String("folder"))
		{
			return;
		}

		if (!selected_item->has_meta("Resource"))
		{
			return;
		}

		current_value = selected_item->get_meta("Resource");
		emit_changed(get_edited_property(), current_value);
	}
}

void FMODEditorInspectorProperty::open_popup()
{
	add_child(inspector_browser);

	inspector_browser->popup(
			Rect2i(Vector2i(get_global_mouse_position().x - (inspector_browser->get_size().x), get_global_mouse_position().y - (100.0f * editor_scale)), Vector2(1, 1)));
}

void FMODEditorInspectorProperty::close_popup()
{
	for (int i = 0; i < get_child_count(); i++)
	{
		if (get_child(i)->get_class() == "FMODEditorInspector")
		{
			inspector_browser->hide();
			remove_child(inspector_browser);
		}
	}

	inspector_browser->search_text->set_text("");
}

void FMODEditorInspectorProperty::reset()
{
	emit_changed(get_edited_property(), current_value);
}

void FMODEditorInspectorProperty::on_event_popup_id_pressed(int32_t id)
{
	if (!current_value.is_valid())
	{
		return;
	}

	Ref<EventAsset> event = current_value;
	if (!event.is_valid())
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
			String path = event->get_path();
			DisplayServer::get_singleton()->clipboard_set(path);
		}
		break;
		case EventPopupItems::EVENT_POPUP_COPY_GUID:
		{
			String guid = event->get_guid();
			DisplayServer::get_singleton()->clipboard_set(guid);
		}
		break;
		case EventPopupItems::EVENT_POPUP_OPEN_IN_STUDIO:
		{
			String path = event->get_path();
			String result;
			client->get_command("studio.window.navigateTo(studio.project.lookup('" + path + "'))", result);
		}
		break;
		default:
			break;
	}
}

void FMODEditorInspectorProperty::set_editor_scale(float editor_scale)
{
	this->editor_scale = editor_scale;
}

float FMODEditorInspectorProperty::get_editor_scale() const
{
	return editor_scale;
}
