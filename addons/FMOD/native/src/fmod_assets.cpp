#include "fmod_assets.h"
#include "fmod_studio_editor_module.h"

using namespace godot;

void FMODAsset::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_path", "path"), &FMODAsset::set_path);
	ClassDB::bind_method(D_METHOD("get_path"), &FMODAsset::get_path);
	ClassDB::bind_method(D_METHOD("set_guid", "guid"), &FMODAsset::set_guid);
	ClassDB::bind_method(D_METHOD("get_guid"), &FMODAsset::get_guid);
	ClassDB::bind_method(D_METHOD("set_name", "name"), &FMODAsset::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &FMODAsset::get_name);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "path", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_path", "get_path");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "guid", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),

			"set_guid", "get_guid");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_name", "get_name");
}

void FMODAsset::set_path(const String& path)
{
	this->path = path;
}

String FMODAsset::get_path() const
{
	return path;
}

void FMODAsset::set_guid(const String& guid)
{
	this->guid = guid;
}

String FMODAsset::get_guid() const
{
	return guid;
}

void FMODAsset::set_name(const String& name)
{
	this->name = name;
}

String FMODAsset::get_name() const
{
	return name;
}

void BankAsset::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_file_path", "file_path"), &BankAsset::set_file_path);
	ClassDB::bind_method(D_METHOD("get_file_path"), &BankAsset::get_file_path);
	ClassDB::bind_method(D_METHOD("set_modified_time", "modified_time"), &BankAsset::set_modified_time);
	ClassDB::bind_method(D_METHOD("get_modified_time"), &BankAsset::get_modified_time);

	ADD_PROPERTY(
			PropertyInfo(Variant::STRING, "file_path", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_file_path", "get_file_path");

	ADD_PROPERTY(
			PropertyInfo(Variant::STRING, "modified_time", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_modified_time", "get_modified_time");
}

void BankAsset::set_bank_file_info(const Dictionary& file_bank_info)
{
	this->file_path = file_bank_info["file_path"];
	this->modified_time = file_bank_info["modified_time"];
}

void BankAsset::set_bank_ref(const FMOD::Studio::Bank* bank)
{
	String path;
	fmod_obj_to_path(bank, path);
	set_path(path);

	PackedStringArray arr = path.rsplit("/", true, 1);
	String name{};

	if (arr.size() > 0)
	{
		name = arr[arr.size() - 1];
	}
	else
	{
		name = "";
	}

	set_name(name);

	FMOD_GUID guid{};
	bank->getID(&guid);
	String string_guid = guid_to_string(guid);
	set_guid(string_guid);
}

void BankAsset::set_file_path(const String& file_path)
{
	this->file_path = file_path;
}

String BankAsset::get_file_path() const
{
	return file_path;
}

void BankAsset::set_modified_time(int modified_time)
{
	this->modified_time = modified_time;
}

int BankAsset::get_modified_time() const
{
	return modified_time;
}

void EventAsset::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_3d", "is_3d"), &EventAsset::set_3d);
	ClassDB::bind_method(D_METHOD("get_3d"), &EventAsset::get_3d);
	ClassDB::bind_method(D_METHOD("set_oneshot", "is_oneshot"), &EventAsset::set_oneshot);
	ClassDB::bind_method(D_METHOD("get_oneshot"), &EventAsset::get_oneshot);
	ClassDB::bind_method(D_METHOD("set_is_snapshot", "is_snapshot"), &EventAsset::set_is_snapshot);
	ClassDB::bind_method(D_METHOD("get_is_snapshot"), &EventAsset::get_is_snapshot);
	ClassDB::bind_method(D_METHOD("set_min_distance", "min_distance"), &EventAsset::set_min_distance);
	ClassDB::bind_method(D_METHOD("get_min_distance"), &EventAsset::get_min_distance);
	ClassDB::bind_method(D_METHOD("set_max_distance", "max_distance"), &EventAsset::set_max_distance);
	ClassDB::bind_method(D_METHOD("get_max_distance"), &EventAsset::get_max_distance);
	ClassDB::bind_method(D_METHOD("set_parameters", "parameters"), &EventAsset::set_parameters);
	ClassDB::bind_method(D_METHOD("get_parameters"), &EventAsset::get_parameters);
	ClassDB::bind_method(D_METHOD("set_event_ref_from_description", "description"),
			&EventAsset::set_event_ref_from_description_ref);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_3d", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_3d", "get_3d");
	ADD_PROPERTY(
			PropertyInfo(Variant::BOOL, "is_oneshot", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_oneshot", "get_oneshot");
	ADD_PROPERTY(
			PropertyInfo(Variant::BOOL, "is_snapshot", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_is_snapshot", "get_is_snapshot");
	ADD_PROPERTY(
			PropertyInfo(Variant::FLOAT, "min_distance", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_min_distance", "get_min_distance");
	ADD_PROPERTY(
			PropertyInfo(Variant::FLOAT, "max_distance", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT),
			"set_max_distance", "get_max_distance");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "parameters", PropertyHint::PROPERTY_HINT_ARRAY_TYPE,
						 "24/17:ParameterAsset", PROPERTY_USAGE_DEFAULT),
			"set_parameters", "get_parameters");
}

void EventAsset::set_event_ref(FMOD::Studio::EventDescription* event_description)
{
	bool is_3d{};
	event_description->is3D(&is_3d);
	set_3d(is_3d);
	bool is_oneshot{};
	event_description->isOneshot(&is_oneshot);
	set_oneshot(is_oneshot);

	float min_distance{}, max_distance{};
	event_description->getMinMaxDistance(&min_distance, &max_distance);
	set_min_distance(min_distance);
	set_max_distance(max_distance);

	FMOD_GUID guid{};
	event_description->getID(&guid);
	String string_guid = guid_to_string(guid);
	set_guid(string_guid);

	String path{};
	fmod_obj_to_path(event_description, path);
	set_path(path);

	PackedStringArray arr = path.rsplit("/", true, 1);
	String name;
	if (arr.size() > 0)
	{
		name = arr[arr.size() - 1];
	}
	else
	{
		name = "";
	}

	set_name(name);

	bool is_snapshot{};
	event_description->isSnapshot(&is_snapshot);
	set_is_snapshot(is_snapshot);

	if (is_snapshot)
	{
		return;
	}

	int parameter_count{};
	event_description->getParameterDescriptionCount(&parameter_count);

	for (int i = 0; i < parameter_count; i++)
	{
		FMOD_STUDIO_PARAMETER_DESCRIPTION parameter_description = {};
		if (event_description->getParameterDescriptionByIndex(i, &parameter_description) == FMOD_OK)
		{
			bool global = parameter_description.flags & FMOD_STUDIO_PARAMETER_GLOBAL;
			bool read_only = parameter_description.flags & FMOD_STUDIO_PARAMETER_READONLY;

			if (global || read_only)
			{
				continue;
			}

			Ref<ParameterAsset> param = create_ref<ParameterAsset>();
			param->set_parameter_ref(parameter_description);

			if (parameter_description.flags & FMOD_STUDIO_PARAMETER_LABELED)
			{
				int maximum = static_cast<int>(parameter_description.maximum);

				for (int i = 0; i <= maximum; i++)
				{
					int actual_size{ 512 };
					auto raw_buffer = std::string(actual_size, ' ');
					FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;
					do
					{
						raw_buffer.resize(actual_size);

						result = event_description->getParameterLabelByID(parameter_description.id, i, &raw_buffer[0],
								actual_size, &actual_size);

						if (result == FMOD_ERR_EVENT_NOTFOUND || result == FMOD_ERR_INVALID_PARAM)
						{
							UtilityFunctions::printerr("[FMOD] Failed to get Parameter Label", __FUNCTION__, __FILE__,
									__LINE__);
							break;
						}
					} while (result == FMOD_ERR_TRUNCATED);

					if (result == FMOD_OK)
					{
						param->get_labels().push_back(String(raw_buffer.c_str()));
					}
				}
			}
			String param_guid = guid_to_string(parameter_description.guid);
			parameters[param_guid] = param;
		}
	}
}

FMOD::Studio::EventDescription* EventAsset::get_event_description() const
{
	FMOD::Studio::EventDescription* event_description = nullptr;
	FMOD::Studio::System* studio_system = FMODStudioEditorModule::get_singleton()->get_studio_system();

	if (studio_system)
	{
		FMODStudioEditorModule::get_singleton()->get_studio_system()->getEvent(get_path().utf8().get_data(),
				&event_description);
	}

	return event_description;
}

void EventAsset::set_event_ref_from_description_ref(const Ref<StudioApi::EventDescription>& event_description)
{
	set_3d(event_description->is_3d());
	set_oneshot(event_description->is_oneshot());

	Dictionary min_max_distance = event_description->get_min_max_distance();
	set_min_distance(min_max_distance["min"]);
	set_max_distance(min_max_distance["max"]);

	set_guid(event_description->get_id());

	String path = event_description->get_path();
	set_path(path);

	PackedStringArray arr = path.rsplit("/", true, 1);
	String name = "";

	if (arr.size() > 0)
	{
		name = arr[arr.size() - 1];
	}
	else
	{
		name = "";
	}

	set_name(name);

	set_is_snapshot(event_description->is_snapshot());

	if (is_snapshot)
	{
		return;
	}

	int parameter_count = event_description->get_parameter_description_count();
	for (int i = 0; i < parameter_count; i++)
	{
		Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> parameter_description =
				event_description->get_parameter_description_by_index(i);

		bool global = parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_GLOBAL;
		bool read_only = parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_READONLY;

		if (global || read_only)
		{
			continue;
		}

		Ref<ParameterAsset> param = create_ref<ParameterAsset>();
		param->set_parameter_description(parameter_description);

		if (parameter_description->get_flags() & FMOD_STUDIO_PARAMETER_LABELED)
		{
			int maximum = static_cast<int>(parameter_description->get_maximum());

			for (int i = 0; i <= maximum; i++)
			{
				const String label = event_description->get_parameter_label_by_id(parameter_description->get_id(), i);

				if (!label.is_empty())
				{
					param->get_labels().push_back(label);
				}
			}
		}
		String guid = param->get_guid();
		parameters[guid] = param;
	}
}

void EventAsset::set_3d(bool is_3d)
{
	this->is_3d = is_3d;
}

bool EventAsset::get_3d() const
{
	return is_3d;
}

void EventAsset::set_oneshot(bool is_oneshot)
{
	this->is_oneshot = is_oneshot;
}

bool EventAsset::get_oneshot() const
{
	return is_oneshot;
}

void EventAsset::set_is_snapshot(bool is_snapshot)
{
	this->is_snapshot = is_snapshot;
}

bool EventAsset::get_is_snapshot() const
{
	return is_snapshot;
}

void EventAsset::set_min_distance(float min_distance)
{
	this->min_distance = min_distance;
}

float EventAsset::get_min_distance() const
{
	return min_distance;
}

void EventAsset::set_max_distance(float max_distance)
{
	this->max_distance = max_distance;
}

float EventAsset::get_max_distance() const
{
	return max_distance;
}

void EventAsset::set_parameters(const Dictionary& parameters)
{
	this->parameters = parameters;
}

Dictionary EventAsset::get_parameters() const
{
	return parameters;
}

bool EventAsset::has_parameter(const String& parameter_guid) const
{
	return parameters.has(parameter_guid);
}

void BusAsset::_bind_methods()
{
}

void BusAsset::set_bus_ref(const FMOD::Studio::Bus* bus)
{
	String path;
	fmod_obj_to_path(bus, path);
	set_path(path);

	PackedStringArray arr = path.rsplit("/", true, 1);
	String name;

	if (arr.size() > 0)
	{
		name = arr[arr.size() - 1];
	}
	else
	{
		name = "";
	}

	if (path == "bus:/")
	{
		name = "Master";
	}

	set_name(name);

	FMOD_GUID guid{};
	bus->getID(&guid);
	String string_guid = guid_to_string(guid);
	set_guid(string_guid);
}

void VCAAsset::_bind_methods()
{
}

void VCAAsset::set_vca_ref(const FMOD::Studio::VCA* vca)
{
	String path{};
	fmod_obj_to_path(vca, path);
	set_path(path);

	PackedStringArray arr = path.rsplit("/", true, 1);

	String name{};
	if (arr.size() > 0)
	{
		name = arr[arr.size() - 1];
	}
	else
	{
		name = "";
	}

	set_name(name);

	FMOD_GUID guid{};
	vca->getID(&guid);
	String string_guid = guid_to_string(guid);
	set_guid(string_guid);
}

void ParameterAsset::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_parameter_description", "parameter_description"),
			&ParameterAsset::set_parameter_description);
	ClassDB::bind_method(D_METHOD("get_parameter_description"), &ParameterAsset::get_parameter_description);
	ClassDB::bind_method(D_METHOD("set_labels", "labels"), &ParameterAsset::set_labels);
	ClassDB::bind_method(D_METHOD("get_labels"), &ParameterAsset::get_labels);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "parameter_description", PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
						 "FMOD_STUDIO_PARAMETER_DESCRIPTION", PROPERTY_USAGE_DEFAULT),
			"set_parameter_description", "get_parameter_description");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "labels"), "set_labels", "get_labels");
}

void ParameterAsset::set_parameter_ref(const FMOD_STUDIO_PARAMETER_DESCRIPTION& parameter_description)
{
	FMOD_GUID* guid = (FMOD_GUID*)&parameter_description.guid;
	String string_guid = guid_to_string(*guid);
	set_guid(string_guid);

	int actual_size{ 512 };
	auto raw_buffer = std::string(actual_size, ' ');
	FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;
	do
	{
		raw_buffer.resize(actual_size);

		FMOD::Studio::System* studio_system = FMODStudioEditorModule::get_singleton()->get_studio_system();
		if (studio_system)
		{
			result = studio_system->lookupPath(std::as_const(guid), &raw_buffer[0], actual_size, &actual_size);
		}

		if (result == FMOD_ERR_EVENT_NOTFOUND)
		{
			UtilityFunctions::printerr("Failed to get Path", __FUNCTION__, __FILE__, __LINE__);
			break;
		}
	} while (result == FMOD_ERR_TRUNCATED);

	if (result == FMOD_OK)
	{
		set_path(String(raw_buffer.c_str()));
	}
	else
	{
		set_path("");
	}

	set_name(parameter_description.name);

	this->parameter_description = create_ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>();
	this->parameter_description->set_parameter_description(parameter_description);

	if ((parameter_description.flags & FMOD_STUDIO_PARAMETER_LABELED) &&
			(parameter_description.flags & FMOD_STUDIO_PARAMETER_GLOBAL))
	{
		labels.clear();
		int maximum = static_cast<int>(parameter_description.maximum);

		for (int i = 0; i <= maximum; i++)
		{
			int actual_size{ 512 };
			auto raw_buffer = std::string(actual_size, ' ');
			FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;
			do
			{
				raw_buffer.resize(actual_size);

				FMOD::Studio::System* studio_system = FMODStudioEditorModule::get_singleton()->get_studio_system();
				if (studio_system)
				{
					result = studio_system->getParameterLabelByID(parameter_description.id, i, &raw_buffer[0],
							actual_size, &actual_size);
				}

				if (result == FMOD_ERR_EVENT_NOTFOUND || result == FMOD_ERR_INVALID_PARAM)
				{
					UtilityFunctions::printerr("[FMOD] Failed to get Parameter Label", __FUNCTION__, __FILE__,
							__LINE__);
					break;
				}
			} while (result == FMOD_ERR_TRUNCATED);

			if (result == FMOD_OK)
			{
				labels.push_back(String(raw_buffer.c_str()));
			}
		}
	}
}

void ParameterAsset::set_parameter_description(
		const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>& parameter_description)
{
	this->parameter_description = parameter_description;
}

Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> ParameterAsset::get_parameter_description() const
{
	return parameter_description;
}

void ParameterAsset::set_labels(const Array& labels)
{
	this->labels = labels;
}

Array ParameterAsset::get_labels() const
{
	return labels;
}