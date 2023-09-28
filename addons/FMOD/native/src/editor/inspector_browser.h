#ifndef INSPECTOR_BROWSER_H
#define INSPECTOR_BROWSER_H

#include "fmod_assets.h"
#include "fmod_studio_editor_module.h"
#include <godot_cpp/classes/accept_dialog.hpp>
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

using namespace godot;

class FMODEditorInspector;

class FMODEditorInspectorTree : public Tree
{
	GDCLASS(FMODEditorInspectorTree, Tree);

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
	TreeItem* local_parameters_root = nullptr;
	Ref<Texture2D> icon;
	Ref<ProjectCache> project_cache;
	FMODEditorInspector* window = nullptr;
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

class FMODEditorInspector : public AcceptDialog
{
	GDCLASS(FMODEditorInspector, AcceptDialog);

protected:
	static void _bind_methods();

private:
	float editor_scale;

public:
	VBoxContainer* root_vbox = nullptr;
	VBoxContainer* search_vbox = nullptr;
	LineEdit* search_text = nullptr;
	FMODEditorInspectorTree* tree = nullptr;
	void initialize();
	void set_editor_scale(float editor_scale);
};

class FMODEditorInspectorProperty : public EditorProperty
{
	GDCLASS(FMODEditorInspectorProperty, EditorProperty);

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
	FMODEditorInspector* inspector_browser;
	FMODStudioEditorModule::FMODAssetType type = FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_NONE;
	PopupMenu* event_popup;
	float editor_scale;
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
	void set_editor_scale(float editor_scale);
	float get_editor_scale() const;
};

class FMODEditorInspectorPlugin : public EditorInspectorPlugin
{
	GDCLASS(FMODEditorInspectorPlugin, EditorInspectorPlugin);

protected:
	static void _bind_methods() {}

private:
	float editor_scale = 1.0f;

public:
	virtual bool _can_handle(Object* object) const override
	{
		return object != nullptr;
	}

	virtual bool _parse_property(Object* object, Variant::Type type, const String& name, PropertyHint hint_type,
			const String& hint_string, BitField<PropertyUsageFlags> usage_flags, bool wide) override
	{
		if (object != nullptr && type == Variant::Type::OBJECT)
		{
			if (hint_string == "EventAsset")
			{
				FMODEditorInspectorProperty* control = memnew(FMODEditorInspectorProperty);
				control->set_editor_scale(editor_scale);
				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_EVENT);
				add_property_editor(name, control);
				return true;
			}
			else if (hint_string == "BankAsset")
			{
				FMODEditorInspectorProperty* control = memnew(FMODEditorInspectorProperty);
				control->set_editor_scale(editor_scale);
				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BANK);
				add_property_editor(name, control);
				return true;
			}
			else if (hint_string == "BusAsset")
			{
				FMODEditorInspectorProperty* control = memnew(FMODEditorInspectorProperty);
				control->set_editor_scale(editor_scale);
				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_BUS);
				add_property_editor(name, control);
				return true;
			}
			else if (hint_string == "VCAAsset")
			{
				FMODEditorInspectorProperty* control = memnew(FMODEditorInspectorProperty);
				control->set_editor_scale(editor_scale);
				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_VCA);
				add_property_editor(name, control);
				return true;
			}
			else if (hint_string == "ParameterAsset")
			{
				FMODEditorInspectorProperty* control = memnew(FMODEditorInspectorProperty);
				control->set_editor_scale(editor_scale);
				control->init(FMODStudioEditorModule::FMODAssetType::FMOD_ASSETTYPE_GLOBAL_PARAMETER);
				add_property_editor(name, control);
				return true;
			}

			return false;
		}

		return false;
	}

	void set_editor_scale(float editor_scale)
	{
		this->editor_scale = editor_scale;
	}
};

#endif // INSPECTOR_BROWSER_H