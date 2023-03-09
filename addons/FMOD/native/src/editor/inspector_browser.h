#ifndef INSPECTOR_BROWSER_H
#define INSPECTOR_BROWSER_H

#include "fmod_assets.h"
#include "fmod_studio_editor_module.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/tree_item.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/window.hpp>

using namespace godot;

class InspectorBrowser;

class InspectorBrowserTree : public Tree
{
	GDCLASS(InspectorBrowserTree, Tree);

protected:
	static void _bind_methods();

private:
	// TreeItem* selected_item = nullptr;
	TreeItem* root_item = nullptr;
	LineEdit* search_text = nullptr;
	FMODStudioEditorModule::FMODAssetType type = FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_NONE;
	TreeItem* events_root = nullptr;
	TreeItem* snapshots_root = nullptr;
	TreeItem* banks_root = nullptr;
	TreeItem* busses_root = nullptr;
	TreeItem* vcas_root = nullptr;
	TreeItem* parameters_root = nullptr;
	Ref<Texture2D> icon;
	Ref<ProjectCache> project_cache;
	InspectorBrowser* window = nullptr;
	String filter = "";

public:
	void initialize(FMODStudioEditorModule::FMODAssetType item_type);
	void populate_browser();
	void on_text_changed(const String& text_filter);
	bool update_filter(TreeItem* p_parent, bool p_scroll_to_selected);
	void collapse_all(TreeItem* p_parent = nullptr);
	void on_size_changed();
	void on_item_collapsed(Object* item);
};

class InspectorBrowser : public Window
{
	GDCLASS(InspectorBrowser, Window);

protected:
	static void _bind_methods();

public:
	VBoxContainer* root_vbox = nullptr;
	VBoxContainer* search_vbox = nullptr;
	LineEdit* search_text = nullptr;
	InspectorBrowserTree* tree = nullptr;
	void initialize();
};

class InspectorBrowserProperty : public EditorProperty
{
	GDCLASS(InspectorBrowserProperty, EditorProperty);

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

protected:
	static void _bind_methods();

private:
	Button* property_control;
	Ref<Texture2D> icon;
	Ref<Resource> current_value;
	bool updating = false;
	InspectorBrowser* inspector_browser;
	FMODStudioEditorModule::FMODAssetType type = FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_NONE;
	PopupMenu* event_popup;
	void popup_menu(PopupType type, Vector2 pos);

public:
	void init(FMODStudioEditorModule::FMODAssetType asset_type);
	void _input(const Ref<InputEvent>& event) override;
	virtual void _update_property() override;
	void on_button_pressed();
	void on_item_selected();
	void open_popup();
	void close_popup();
	void reset();
	void on_event_popup_id_pressed(int32_t id);
};

//class InspectorBrowserPlugin : public EditorInspectorPlugin
//{
//	GDCLASS(InspectorBrowserPlugin, EditorInspectorPlugin);
//
//protected:
//	static void _bind_methods()
//	{
//	}
//
//public:
//	virtual bool _can_handle(const Variant& object) const override
//	{
//		Object* obj = object;
//		return object.get_type() != Variant::NIL && obj != nullptr;
//	}
//
//	virtual bool _parse_property(Object* object, int64_t type, const String& name, int64_t hint_type,
//			const String& hint_string, int64_t usage_flags, bool wide) override
//	{
//		if (object != nullptr && type == Variant::Type::OBJECT)
//		{
//			if (hint_string == "EventAsset")
//			{
//				InspectorBrowserProperty* control = memnew(InspectorBrowserProperty);
//				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_EVENT);
//				add_property_editor(name, control);
//				return true;
//			}
//			else if (hint_string == "BankAsset")
//			{
//				InspectorBrowserProperty* control = memnew(InspectorBrowserProperty);
//				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BANK);
//				add_property_editor(name, control);
//				return true;
//			}
//			else if (hint_string == "BusAsset")
//			{
//				InspectorBrowserProperty* control = memnew(InspectorBrowserProperty);
//				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BUS);
//				add_property_editor(name, control);
//				return true;
//			}
//			else if (hint_string == "VCAAsset")
//			{
//				InspectorBrowserProperty* control = memnew(InspectorBrowserProperty);
//				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_VCA);
//				add_property_editor(name, control);
//				return true;
//			}
//			else if (hint_string == "ParameterAsset")
//			{
//				InspectorBrowserProperty* control = memnew(InspectorBrowserProperty);
//				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_GLOBAL_PARAMETER);
//				add_property_editor(name, control);
//				return true;
//			}
//
//			return false;
//		}
//
//		return false;
//	}
//};

#endif // INSPECTOR_BROWSER_H