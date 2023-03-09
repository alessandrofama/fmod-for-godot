# note(alex): Due to a godot-cpp bug we need to create the InspectorPlugin here
# in GDScript. See godot-cpp#886

extends EditorInspectorPlugin


func _can_handle(object):
	return object != null


func _parse_property(object, type, name, hint_type, hint_string, usage_flags, wide):
	if object != null && type == TYPE_OBJECT:
		if hint_string == "BankAsset":
			var inspector_property: InspectorBrowserProperty = InspectorBrowserProperty.new()
			inspector_property.init(FMODStudioEditorModule.FMOD_ASSETTYPE_BANK)
			add_property_editor(name, inspector_property)
			return true
		elif hint_string == "EventAsset":
			var inspector_property: InspectorBrowserProperty = InspectorBrowserProperty.new()
			inspector_property.init(FMODStudioEditorModule.FMOD_ASSETTYPE_EVENT)
			add_property_editor(name, inspector_property)
			return true
		elif hint_string == "BusAsset":
			var inspector_property: InspectorBrowserProperty = InspectorBrowserProperty.new()
			inspector_property.init(FMODStudioEditorModule.FMOD_ASSETTYPE_BUS)
			add_property_editor(name, inspector_property)
			return true
		elif hint_string == "VCAAsset":
			var inspector_property: InspectorBrowserProperty = InspectorBrowserProperty.new()
			inspector_property.init(FMODStudioEditorModule.FMOD_ASSETTYPE_VCA)
			add_property_editor(name, inspector_property)
			return true
		elif hint_string == "ParameterAsset":
			var inspector_property: InspectorBrowserProperty = InspectorBrowserProperty.new()
			inspector_property.init(FMODStudioEditorModule.FMOD_ASSETTYPE_GLOBAL_PARAMETER)
			add_property_editor(name, inspector_property)
			return true
		return false
	return false
