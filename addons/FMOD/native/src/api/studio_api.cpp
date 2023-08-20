#include "studio_api.h"
#include "../fmod_studio_module.h"

using namespace StudioApi;

void StudioSystem::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("is_valid"), &StudioSystem::is_valid);
	ClassDB::bind_method(D_METHOD("update"), &StudioSystem::update);
	ClassDB::bind_method(D_METHOD("flush_commands"), &StudioSystem::flush_commands);
	ClassDB::bind_method(D_METHOD("flush_sample_loading"), &StudioSystem::flush_sample_loading);
	ClassDB::bind_method(D_METHOD("get_event", "event_path"), &StudioSystem::get_event);
	ClassDB::bind_method(D_METHOD("get_bus", "bus_path"), &StudioSystem::get_bus);
	ClassDB::bind_method(D_METHOD("get_vca", "vca_path"), &StudioSystem::get_vca);
	ClassDB::bind_method(D_METHOD("get_bank", "bank_path"), &StudioSystem::get_bank);
	ClassDB::bind_method(D_METHOD("get_event_by_id", "event_guid"), &StudioSystem::get_event_by_id);
	ClassDB::bind_method(D_METHOD("get_bus_by_id", "bus_guid"), &StudioSystem::get_bus_by_id);
	ClassDB::bind_method(D_METHOD("get_vca_by_id", "vca_guid"), &StudioSystem::get_vca_by_id);
	ClassDB::bind_method(D_METHOD("get_bank_by_id", "bank_guid"), &StudioSystem::get_bank_by_id);
	ClassDB::bind_method(D_METHOD("get_parameter_description_by_name", "name"),
			&StudioSystem::get_parameter_description_by_name);
	ClassDB::bind_method(D_METHOD("get_parameter_description_by_id", "parameter_id"),
			&StudioSystem::get_parameter_description_by_id);
	ClassDB::bind_method(D_METHOD("get_parameter_label_by_name", "name", "label_index"),
			&StudioSystem::get_parameter_label_by_name);
	ClassDB::bind_method(D_METHOD("get_parameter_label_by_id", "parameter_id", "label_index"),
			&StudioSystem::get_parameter_label_by_id);
	ClassDB::bind_method(D_METHOD("get_parameter_by_id", "parameter_id"), &StudioSystem::get_parameter_by_id);
	ClassDB::bind_method(D_METHOD("set_parameter_by_id", "parameter_id", "value", "ignore_seek_speed"),
			&StudioSystem::set_parameter_by_id, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("set_parameter_by_id_with_label", "parameter_id", "label", "ignore_seek_speed"),
			&StudioSystem::set_parameter_by_id_with_label, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("set_parameters_by_ids", "parameter_ids", "values", "count", "ignore_seek_speed"),
			&StudioSystem::set_parameters_by_ids, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("get_parameter_by_name", "name"), &StudioSystem::get_parameter_by_name);
	ClassDB::bind_method(D_METHOD("set_parameter_by_name", "name", "value", "ignore_seek_speed"),
			&StudioSystem::set_parameter_by_name, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("set_parameter_by_name_with_label", "name", "label", "ignore_seek_speed"),
			&StudioSystem::set_parameter_by_name_with_label, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("lookup_id", "path"), &StudioSystem::lookup_id);
	ClassDB::bind_method(D_METHOD("lookup_path", "guid"), &StudioSystem::lookup_path);
	ClassDB::bind_method(D_METHOD("get_num_listeners"), &StudioSystem::get_num_listeners);
	ClassDB::bind_method(D_METHOD("set_num_listeners", "num_listeners"), &StudioSystem::set_num_listeners);
	ClassDB::bind_method(D_METHOD("get_listener_attributes", "listener"), &StudioSystem::get_listener_attributes);
	ClassDB::bind_method(D_METHOD("set_listener_attributes", "listener", "attributes", "attenuation_position"),
			&StudioSystem::set_listener_attributes);
	ClassDB::bind_method(D_METHOD("get_listener_weight", "listener"), &StudioSystem::get_listener_weight);
	ClassDB::bind_method(D_METHOD("set_listener_weight", "listener", "weight"), &StudioSystem::set_listener_weight);
	ClassDB::bind_method(D_METHOD("load_bank_file", "bank_path", "load_bank_flags", "load_sample_data"),
			&StudioSystem::load_bank_file);
	ClassDB::bind_method(D_METHOD("unload_all"), &StudioSystem::unload_all);
	ClassDB::bind_method(D_METHOD("get_buffer_usage"), &StudioSystem::get_buffer_usage);
	ClassDB::bind_method(D_METHOD("reset_buffer_usage"), &StudioSystem::reset_buffer_usage);
	ClassDB::bind_method(D_METHOD("get_bank_count"), &StudioSystem::get_bank_count);
	ClassDB::bind_method(D_METHOD("get_bank_list", "capacity"), &StudioSystem::get_bank_list);
	ClassDB::bind_method(D_METHOD("get_parameter_description_count"), &StudioSystem::get_parameter_description_count);
	ClassDB::bind_method(D_METHOD("get_parameter_description_list", "capacity"),
			&StudioSystem::get_parameter_description_list);
	ClassDB::bind_method(D_METHOD("start_command_capture", "file_name", "flags"), &StudioSystem::start_command_capture);
	ClassDB::bind_method(D_METHOD("stop_command_capture"), &StudioSystem::stop_command_capture);
	ClassDB::bind_method(D_METHOD("load_command_replay", "file_name", "flags"), &StudioSystem::load_command_replay);
	ClassDB::bind_method(D_METHOD("set_callback", "callable", "callback_mask"), &StudioSystem::set_callback);
	ClassDB::bind_method(D_METHOD("get_cpu_usage", "studio_usage", "cpu_usage"), &StudioSystem::get_cpu_usage);
	ClassDB::bind_method(D_METHOD("get_memory_usage", "memory_usage"), &StudioSystem::get_memory_usage);
}

void StudioSystem::set_instance(FMOD::Studio::System* studio_system)
{
	this->studio_system = studio_system;
}

bool StudioSystem::is_valid() const
{
	return studio_system->isValid();
}

bool StudioSystem::flush_commands() const
{
	return ERROR_CHECK(studio_system->flushCommands());
}

bool StudioSystem::flush_sample_loading() const
{
	return ERROR_CHECK(studio_system->flushSampleLoading());
}

Ref<EventDescription> StudioSystem::get_event(const String& event_path) const
{
	FMOD::Studio::EventDescription* event_description = nullptr;
	Ref<EventDescription> ref = create_ref<EventDescription>();

	if (ERROR_CHECK(studio_system->getEvent(event_path.utf8().get_data(), &event_description)))
	{
		ref->set_instance(event_description);
	}

	return ref;
}

Ref<Bus> StudioSystem::get_bus(const String& bus_path) const
{
	FMOD::Studio::Bus* bus = nullptr;
	Ref<Bus> ref = create_ref<Bus>();

	if (ERROR_CHECK(studio_system->getBus(bus_path.utf8().get_data(), &bus)))
	{
		ref->set_instance(bus);
	}

	return ref;
}

Ref<VCA> StudioSystem::get_vca(const String& vca_path) const
{
	FMOD::Studio::VCA* vca = nullptr;

	Ref<VCA> ref = create_ref<VCA>();

	if (ERROR_CHECK(studio_system->getVCA(vca_path.utf8().get_data(), &vca)))
	{
		ref->set_instance(vca);
	}

	return ref;
}

Ref<Bank> StudioSystem::get_bank(const String& bank_path) const
{
	FMOD::Studio::Bank* bank = nullptr;
	Ref<Bank> ref = create_ref<Bank>();

	if (ERROR_CHECK(studio_system->getBank(bank_path.utf8().get_data(), &bank)))
	{
		ref->set_instance(bank);
	}

	return ref;
}

Ref<EventDescription> StudioSystem::get_event_by_id(const String& event_guid) const
{
	FMOD::Studio::EventDescription* event_description = nullptr;

	FMOD_GUID guid{};
	FMOD::Studio::parseID(event_guid.utf8().get_data(), &guid);
	Ref<EventDescription> ref = create_ref<EventDescription>();

	if (ERROR_CHECK(studio_system->getEventByID(&guid, &event_description)))
	{
		ref->set_instance(event_description);
	}

	return ref;
}

Ref<Bus> StudioSystem::get_bus_by_id(const String& bus_guid) const
{
	FMOD::Studio::Bus* bus = nullptr;

	FMOD_GUID guid{};
	FMOD::Studio::parseID(bus_guid.utf8().get_data(), &guid);
	Ref<Bus> ref = create_ref<Bus>();

	if (ERROR_CHECK(studio_system->getBusByID(&guid, &bus)))
	{
		ref->set_instance(bus);
	}

	return ref;
}

Ref<VCA> StudioSystem::get_vca_by_id(const String& vca_guid) const
{
	FMOD::Studio::VCA* vca = nullptr;

	FMOD_GUID guid{};
	FMOD::Studio::parseID(vca_guid.utf8().get_data(), &guid);
	Ref<VCA> ref = create_ref<VCA>();

	if (ERROR_CHECK(studio_system->getVCAByID(&guid, &vca)))
	{
		ref->set_instance(vca);
	}

	return ref;
}

Ref<Bank> StudioSystem::get_bank_by_id(const String& bank_guid) const
{
	FMOD::Studio::Bank* bank = nullptr;

	FMOD_GUID guid{};
	FMOD::Studio::parseID(bank_guid.utf8().get_data(), &guid);
	Ref<Bank> ref = create_ref<Bank>();

	if (ERROR_CHECK(studio_system->getBankByID(&guid, &bank)))
	{
		ref->set_instance(bank);
	}

	return ref;
}

Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> StudioSystem::get_parameter_description_by_name(
		const String& name) const
{
	FMOD_STUDIO_PARAMETER_DESCRIPTION description{};
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> ref = create_ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>();

	if (ERROR_CHECK(studio_system->getParameterDescriptionByName(name.utf8().get_data(), &description)))
	{
		ref->set_parameter_description(description);
	}

	return ref;
}

Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> StudioSystem::get_parameter_description_by_id(
		const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id) const
{
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> ref = create_ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>();

	if (parameter_id.is_null())
	{
		UtilityFunctions::push_warning("The Parameter ID is invalid");
		return ref;
	}

	FMOD_STUDIO_PARAMETER_ID id{};
	parameter_id->get_parameter_id(id);

	FMOD_STUDIO_PARAMETER_DESCRIPTION description;
	if (ERROR_CHECK(studio_system->getParameterDescriptionByID(id, &description)))
	{
		ref->set_parameter_description(description);
	}

	return ref;
}

String StudioSystem::get_parameter_label_by_name(const String& name, int label_index) const
{
	int actual_size{ 512 };
	auto raw_buffer = std::string(actual_size, ' ');
	FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;

	do
	{
		raw_buffer.resize(actual_size);
		result = studio_system->getParameterLabelByName(name.utf8().get_data(), label_index, &raw_buffer[0],
				actual_size, &actual_size);

		if (result == FMOD_ERR_INVALID_PARAM || result == FMOD_ERR_EVENT_NOTFOUND)
		{
			ERROR_CHECK(FMOD_ERR_INVALID_PARAM);
			break;
		}
	} while (result == FMOD_ERR_TRUNCATED);

	if (result == FMOD_OK)
	{
		return String(raw_buffer.c_str());
	}

	return String();
}

String StudioSystem::get_parameter_label_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id,
		int label_index) const
{
	FMOD_STUDIO_PARAMETER_ID id{};
	parameter_id->get_parameter_id(id);

	int actual_size{ 512 };
	auto raw_buffer = std::string(actual_size, ' ');
	FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;
	do
	{
		raw_buffer.resize(actual_size);
		result = studio_system->getParameterLabelByID(id, label_index, &raw_buffer[0], actual_size, &actual_size);

		if (result == FMOD_ERR_INVALID_PARAM || result == FMOD_ERR_EVENT_NOTFOUND)
		{
			ERROR_CHECK(FMOD_ERR_INVALID_PARAM);
			break;
		}
	} while (result == FMOD_ERR_TRUNCATED);

	if (result == FMOD_OK)
	{
		return String(raw_buffer.c_str());
	}

	return String();
}

Dictionary StudioSystem::get_parameter_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id) const
{
	float value{}, final_value{};
	Dictionary result;

	FMOD_STUDIO_PARAMETER_ID id{};
	parameter_id->get_parameter_id(id);

	if (ERROR_CHECK(studio_system->getParameterByID(id, &value, &final_value)))
	{
		result["value"] = value;
		result["final_value"] = final_value;
		return result;
	}

	return result;
}

bool StudioSystem::set_parameter_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id, float value,
		bool ignore_seek_speed) const
{
	FMOD_STUDIO_PARAMETER_ID id{};
	parameter_id->get_parameter_id(id);

	return ERROR_CHECK(studio_system->setParameterByID(id, value, ignore_seek_speed));
}

bool StudioSystem::set_parameter_by_id_with_label(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id,
		const String& label, bool ignore_seek_speed) const
{
	FMOD_STUDIO_PARAMETER_ID id{};
	parameter_id->get_parameter_id(id);

	return ERROR_CHECK(studio_system->setParameterByIDWithLabel(id, label.utf8().get_data(), ignore_seek_speed));
}

bool StudioSystem::set_parameters_by_ids(const Array& parameter_ids, const Array& values, int count,
		bool ignore_seek_speed) const
{
	std::unique_ptr<FMOD_STUDIO_PARAMETER_ID[]> ids = std::make_unique<FMOD_STUDIO_PARAMETER_ID[]>(count);
	std::unique_ptr<float[]> parameter_values = std::make_unique<float[]>(count);

	for (int i = 0; i < count; i++)
	{
		FMOD_STUDIO_PARAMETER_ID id{};
		Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID> parameter_id = parameter_ids[i];
		parameter_id->get_parameter_id(id);

		ids[i] = id;
		parameter_values[i] = values[i];
	}

	return ERROR_CHECK(studio_system->setParametersByIDs(ids.get(), parameter_values.get(), count, ignore_seek_speed));
}

Dictionary StudioSystem::get_parameter_by_name(const String& name) const
{
	float value{}, final_value{};
	Dictionary result;

	if (ERROR_CHECK(studio_system->getParameterByName(name.utf8().get_data(), &value, &final_value)))
	{
		result["value"] = value;
		result["final_value"] = final_value;
		return result;
	}

	return result;
}

bool StudioSystem::set_parameter_by_name(const String& name, float value, bool ignore_seek_speed) const
{
	return ERROR_CHECK(studio_system->setParameterByName(name.utf8().get_data(), value, ignore_seek_speed));
}

bool StudioSystem::set_parameter_by_name_with_label(const String& name, const String& label,
		bool ignore_seek_speed) const
{
	return ERROR_CHECK(
			studio_system->setParameterByNameWithLabel(name.utf8().get_data(), label.utf8().get_data(), ignore_seek_speed));
}

String StudioSystem::lookup_id(const String& path) const
{
	FMOD_GUID guid{};

	if (ERROR_CHECK(studio_system->lookupID(path.utf8().get_data(), &guid)))
	{
		return guid_to_string(guid);
	}

	return String();
}

String StudioSystem::lookup_path(const String& guid) const
{
	FMOD_GUID fmod_guid{};
	FMOD::Studio::parseID(guid.utf8().get_data(), &fmod_guid);

	String path;
	int actual_size{ 512 };
	auto raw_buffer = std::string(actual_size, ' ');
	FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;
	do
	{
		raw_buffer.resize(actual_size);
		result = studio_system->lookupPath(&fmod_guid, &raw_buffer[0], actual_size, &actual_size);
	} while (result == FMOD_ERR_TRUNCATED);

	return String(raw_buffer.c_str());
}

int StudioSystem::get_num_listeners() const
{
	int num_listeners{};

	if (ERROR_CHECK(studio_system->getNumListeners(&num_listeners)))
	{
		return num_listeners;
	}

	return num_listeners;
}

bool StudioSystem::set_num_listeners(int num_listeners) const
{
	return ERROR_CHECK(studio_system->setNumListeners(num_listeners));
}

Dictionary StudioSystem::get_listener_attributes(int listener) const
{
	FMOD_3D_ATTRIBUTES attributes{};
	FMOD_VECTOR attenuation{};

	ERROR_CHECK(studio_system->getListenerAttributes(listener, &attributes, &attenuation));

	Ref<FmodTypes::FMOD_3D_ATTRIBUTES> fmod_attributes = create_ref<FmodTypes::FMOD_3D_ATTRIBUTES>();
	fmod_attributes->set_3d_attributes(attributes);

	Vector3 attenuation_vector;
	fmod_vector_to_vector3(attenuation, attenuation_vector);

	Dictionary attributesDict;
	attributesDict["attributes"] = fmod_attributes;
	attributesDict["attenuation"] = attenuation_vector;

	return attributesDict;
}

bool StudioSystem::set_listener_attributes(int listener, Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes,
		const Variant& attenuation_position) const
{
	FMOD_3D_ATTRIBUTES fmod_attributes{};
	attributes->get_3d_attributes(fmod_attributes);

	Variant::Type type = attenuation_position.get_type();

	switch (type)
	{
		case Variant::Type::VECTOR3:
		{
			FMOD_VECTOR attenuation{};
			Vector3 attenuation_vector = attenuation_position;
			vector3_to_fmod_vector(attenuation_vector, attenuation);

			return ERROR_CHECK(studio_system->setListenerAttributes(listener, &fmod_attributes, &attenuation));
		}
		case Variant::Type::VECTOR2:
		{
			FMOD_VECTOR attenuation{};
			Vector2 attenuation_vector = attenuation_position;
			vector2_to_fmod_vector(attenuation_vector, attenuation);

			return ERROR_CHECK(studio_system->setListenerAttributes(listener, &fmod_attributes, &attenuation));
		}
		default:
		{
			return ERROR_CHECK(studio_system->setListenerAttributes(listener, &fmod_attributes, nullptr));
		}
	}
}

float StudioSystem::get_listener_weight(int listener) const
{
	float weight{};
	if (ERROR_CHECK(studio_system->getListenerWeight(listener, &weight)))
	{
		return weight;
	}

	return weight;
}

bool StudioSystem::set_listener_weight(int listener, float weight) const
{
	return ERROR_CHECK(studio_system->setListenerWeight(listener, weight));
}

bool StudioSystem::update() const
{
	return ERROR_CHECK(studio_system->update());
}

Ref<Bank> StudioSystem::load_bank_file(const String& bank_path, FMOD_STUDIO_LOAD_BANK_FLAGS load_bank_flags,
		bool load_sample_data) const
{
	FMOD::Studio::Bank* bank = nullptr;

	ERROR_CHECK(studio_system->loadBankFile(bank_path.utf8().get_data(), load_bank_flags, &bank));

	Ref<Bank> ref = create_ref<Bank>();
	ref->set_instance(bank);

	return ref;
}

bool StudioSystem::unload_all() const
{
	return ERROR_CHECK(studio_system->unloadAll());
}

Ref<FmodTypes::FMOD_STUDIO_BUFFER_USAGE> StudioSystem::get_buffer_usage() const
{
	FMOD_STUDIO_BUFFER_USAGE usage{};
	ERROR_CHECK(studio_system->getBufferUsage(&usage));

	Ref<FmodTypes::FMOD_STUDIO_BUFFER_USAGE> ref = create_ref<FmodTypes::FMOD_STUDIO_BUFFER_USAGE>();
	ref->set_buffer_usage(usage);
	return ref;
}

bool StudioSystem::reset_buffer_usage() const
{
	return ERROR_CHECK(studio_system->resetBufferUsage());
}

int StudioSystem::get_bank_count() const
{
	int bank_count{};
	if (ERROR_CHECK(studio_system->getBankCount(&bank_count)))
	{
		return bank_count;
	}

	return bank_count;
}

TypedArray<Bank> StudioSystem::get_bank_list(int capacity) const
{
	TypedArray<Bank> bank_list;
	std::unique_ptr<FMOD::Studio::Bank*[]> banks = std::make_unique<FMOD::Studio::Bank*[]>(capacity);

	int bank_count{};
	ERROR_CHECK(studio_system->getBankList(banks.get(), capacity, &bank_count));

	for (int i = 0; i < bank_count; i++)
	{
		Ref<Bank> ref = create_ref<Bank>();
		ref->set_instance(banks[i]);
		bank_list.append(ref);
	}

	bank_list.resize(bank_count);

	return bank_list;
}

int StudioSystem::get_parameter_description_count() const
{
	int parameter_description_count{};
	if (ERROR_CHECK(studio_system->getParameterDescriptionCount(&parameter_description_count)))
	{
		return parameter_description_count;
	}

	return parameter_description_count;
}

TypedArray<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> StudioSystem::get_parameter_description_list(int capacity) const
{
	TypedArray<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> parameter_description_list;

	std::unique_ptr<FMOD_STUDIO_PARAMETER_DESCRIPTION[]> parameter_descriptions =
			std::make_unique<FMOD_STUDIO_PARAMETER_DESCRIPTION[]>(capacity);

	int parameter_description_count{};
	ERROR_CHECK(studio_system->getParameterDescriptionList(parameter_descriptions.get(), capacity,
			&parameter_description_count));

	for (int i = 0; i < parameter_description_count; i++)
	{
		Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> ref =
				create_ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>();
		ref->set_parameter_description(parameter_descriptions[i]);
		parameter_description_list.append(ref);
	}

	parameter_description_list.resize(parameter_description_count);

	return parameter_description_list;
}

bool StudioSystem::start_command_capture(const String& file_name, const FMOD_STUDIO_COMMANDCAPTURE_FLAGS flags) const
{
	return ERROR_CHECK(studio_system->startCommandCapture(file_name.utf8().get_data(), flags));
}

bool StudioSystem::stop_command_capture() const
{
	return ERROR_CHECK(studio_system->stopCommandCapture());
}

Ref<CommandReplay> StudioSystem::load_command_replay(const String& file_name,
		FMOD_STUDIO_COMMANDREPLAY_FLAGS flags) const
{
	FMOD::Studio::CommandReplay* replay = nullptr;
	Ref<CommandReplay> ref = create_ref<CommandReplay>();

	if (ERROR_CHECK(studio_system->loadCommandReplay(file_name.utf8().get_data(), flags, &replay)))
	{
		ref->set_instance(replay);
	}

	return ref;
}

bool StudioSystem::set_callback(Variant callable, FMOD_STUDIO_SYSTEM_CALLBACK_TYPE callback_mask) const
{
	Callable* callable_ptr = nullptr;

	void* user_data;
	studio_system->getUserData(&user_data);
	callable_ptr = static_cast<Callable*>(user_data);

	if (callable.get_type() == Variant::Type::CALLABLE)
	{
		if (callable_ptr == nullptr)
		{
			callable_ptr = new Callable(callable);
			void* data = static_cast<void*>(callable_ptr);
			studio_system->setUserData(data);

			return ERROR_CHECK(studio_system->setCallback(
					[](FMOD_STUDIO_SYSTEM* system, FMOD_STUDIO_SYSTEM_CALLBACK_TYPE type, void* commanddata,
							void* userdata) -> FMOD_RESULT
					{
						Callable* callable = static_cast<Callable*>(userdata);
						Dictionary callback_info;
						callback_info["type"] = type;
						Array args = Array();
						args.append(callback_info);
						callable->callv(args);
						return FMOD_OK;
					},
					callback_mask));
		}
	}
	else if (callable.get_type() == Variant::Type::NIL)
	{
		if (callable_ptr)
		{
			FMOD_RESULT result = studio_system->setCallback(nullptr, FMOD_STUDIO_SYSTEM_CALLBACK_ALL);
			studio_system->setUserData(nullptr);
			delete callable_ptr;
			return result;
		}
	}

	return false;
}

void StudioSystem::get_cpu_usage(Ref<FmodTypes::FMOD_STUDIO_CPU_USAGE> studio,
		Ref<FmodTypes::FMOD_CPU_USAGE> core) const
{
	FMOD_STUDIO_CPU_USAGE studio_usage{};
	FMOD_CPU_USAGE core_usage{};

	if (ERROR_CHECK(studio_system->getCPUUsage(&studio_usage, &core_usage)))
	{
		core->set_cpu_usage(core_usage);
		studio->set_cpu_usage(studio_usage);
	}
}

void StudioSystem::get_memory_usage(
		Ref<FmodTypes::FMOD_STUDIO_MEMORY_USAGE> memory_usage) const
{
	FMOD_STUDIO_MEMORY_USAGE usage{};

	if (ERROR_CHECK(studio_system->getMemoryUsage(&usage)))
	{
		memory_usage->set_memory_usage(usage);
	}
}

void EventDescription::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("is_valid"), &EventDescription::is_valid);
	ClassDB::bind_method(D_METHOD("get_id"), &EventDescription::get_id);
	ClassDB::bind_method(D_METHOD("get_path"), &EventDescription::get_path);
	ClassDB::bind_method(D_METHOD("get_parameter_description_count"),
			&EventDescription::get_parameter_description_count);
	ClassDB::bind_method(D_METHOD("get_parameter_description_by_index", "index"),
			&EventDescription::get_parameter_description_by_index);
	ClassDB::bind_method(D_METHOD("get_parameter_description_by_name", "name"),
			&EventDescription::get_parameter_description_by_name);
	ClassDB::bind_method(D_METHOD("get_parameter_description_by_id", "parameter_id"),
			&EventDescription::get_parameter_description_by_id);
	ClassDB::bind_method(D_METHOD("get_parameter_label_by_index", "index", "label_index"),
			&EventDescription::get_parameter_label_by_index);
	ClassDB::bind_method(D_METHOD("get_parameter_label_by_name", "name", "label_index"),
			&EventDescription::get_parameter_label_by_name);
	ClassDB::bind_method(D_METHOD("get_parameter_label_by_id", "parameter_id", "label_index"),
			&EventDescription::get_parameter_label_by_id);
	ClassDB::bind_method(D_METHOD("get_user_property_count"), &EventDescription::get_user_property_count);
	ClassDB::bind_method(D_METHOD("get_user_property_by_index", "index"),
			&EventDescription::get_user_property_by_index);
	ClassDB::bind_method(D_METHOD("get_user_property", "name"), &EventDescription::get_user_property);
	ClassDB::bind_method(D_METHOD("get_length"), &EventDescription::get_length);
	ClassDB::bind_method(D_METHOD("get_min_max_distance"), &EventDescription::get_min_max_distance);
	ClassDB::bind_method(D_METHOD("get_sound_size"), &EventDescription::get_sound_size);
	ClassDB::bind_method(D_METHOD("is_snapshot"), &EventDescription::is_snapshot);
	ClassDB::bind_method(D_METHOD("is_one_shot"), &EventDescription::is_oneshot);
	ClassDB::bind_method(D_METHOD("is_stream"), &EventDescription::is_stream);
	ClassDB::bind_method(D_METHOD("is_3d"), &EventDescription::is_3d);
	ClassDB::bind_method(D_METHOD("is_doppler_enabled"), &EventDescription::is_doppler_enabled);
	ClassDB::bind_method(D_METHOD("has_sustain_point"), &EventDescription::has_sustain_point);
	ClassDB::bind_method(D_METHOD("create_instance"), &EventDescription::create_instance);
	ClassDB::bind_method(D_METHOD("get_instance_count"), &EventDescription::get_instance_count);
	ClassDB::bind_method(D_METHOD("get_instance_list", "capacity"), &EventDescription::get_instance_list);
	ClassDB::bind_method(D_METHOD("load_sample_data"), &EventDescription::load_sample_data);
	ClassDB::bind_method(D_METHOD("unload_sample_data"), &EventDescription::unload_sample_data);
	ClassDB::bind_method(D_METHOD("get_sample_loading_state"), &EventDescription::get_sample_loading_state);
	ClassDB::bind_method(D_METHOD("release_all_instances"), &EventDescription::release_all_instances);
	ClassDB::bind_method(D_METHOD("set_callback", "callable", "callback_mask"), &EventDescription::set_callback);
}

void EventDescription::set_instance(FMOD::Studio::EventDescription* event_description)
{
	this->event_description = event_description;
}

FMOD::Studio::EventDescription* EventDescription::get_instance() const
{
	return event_description;
}

bool EventDescription::is_valid() const
{
	return event_description->isValid();
}

String EventDescription::get_id() const
{
	String id;
	FMOD_GUID guid{};
	if (ERROR_CHECK(event_description->getID(&guid)))
	{
		id = guid_to_string(guid);
		return id;
	}

	return id;
}

String EventDescription::get_path() const
{
	String path;
	fmod_obj_to_path(event_description, path);

	return path;
}

int EventDescription::get_parameter_description_count() const
{
	int count{};

	if (ERROR_CHECK(event_description->getParameterDescriptionCount(&count)))
	{
		return count;
	}

	return count;
}

Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> EventDescription::get_parameter_description_by_index(int index) const
{
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> ref = create_ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>();
	FMOD_STUDIO_PARAMETER_DESCRIPTION description{};

	if (ERROR_CHECK(event_description->getParameterDescriptionByIndex(index, &description)))
	{
		ref->set_parameter_description(description);
		return ref;
	}

	return ref;
}

Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> EventDescription::get_parameter_description_by_name(
		const String& name) const
{
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> ref = create_ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>();
	FMOD_STUDIO_PARAMETER_DESCRIPTION description{};

	if (ERROR_CHECK(event_description->getParameterDescriptionByName(name.utf8().get_data(), &description)))
	{
		ref->set_parameter_description(description);
		return ref;
	}

	return ref;
}

Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> EventDescription::get_parameter_description_by_id(
		const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id) const
{
	Ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION> ref = create_ref<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>();

	FMOD_STUDIO_PARAMETER_ID id{};
	id.data1 = parameter_id->get_data1();
	id.data2 = parameter_id->get_data2();

	FMOD_STUDIO_PARAMETER_DESCRIPTION description{};

	if (ERROR_CHECK(event_description->getParameterDescriptionByID(id, &description)))
	{
		ref->set_parameter_description(description);
		return ref;
	}

	return ref;
}

String EventDescription::get_parameter_label_by_index(int index, int label_index)
{
	int actual_size{ 512 };
	auto raw_buffer = std::string(actual_size, ' ');
	FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;

	do
	{
		raw_buffer.resize(actual_size);
		result =
				event_description->getParameterLabelByIndex(index, label_index, &raw_buffer[0], actual_size, &actual_size);

		if (result == FMOD_ERR_INVALID_PARAM)
		{
			ERROR_CHECK(FMOD_ERR_INVALID_PARAM);
			break;
		}
	} while (result == FMOD_ERR_TRUNCATED);

	if (result == FMOD_OK)
	{
		return String(raw_buffer.c_str());
	}

	return String();
}

String EventDescription::get_parameter_label_by_name(const String& name, int label_index)
{
	int actual_size{ 512 };
	auto raw_buffer = std::string(actual_size, ' ');
	FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;

	do
	{
		raw_buffer.resize(actual_size);
		result = event_description->getParameterLabelByName(name.utf8().get_data(), label_index, &raw_buffer[0],
				actual_size, &actual_size);

		if (result == FMOD_ERR_INVALID_PARAM || result == FMOD_ERR_EVENT_NOTFOUND)
		{
			UtilityFunctions::printerr("Failed to get Parameter Label By Name", __FUNCTION__, __FILE__, __LINE__);
			break;
		}
	} while (result == FMOD_ERR_TRUNCATED);

	if (result == FMOD_OK)
	{
		return String(raw_buffer.c_str());
	}

	return String();
}

String EventDescription::get_parameter_label_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id,
		int label_index) const
{
	FMOD_STUDIO_PARAMETER_ID id{};
	id.data1 = parameter_id->get_data1();
	id.data2 = parameter_id->get_data2();

	int actual_size{ 512 };
	auto raw_buffer = std::string(actual_size, ' ');
	FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;

	do
	{
		raw_buffer.resize(actual_size);
		result = event_description->getParameterLabelByID(id, label_index, &raw_buffer[0], actual_size, &actual_size);

		if (result == FMOD_ERR_INVALID_PARAM || result == FMOD_ERR_EVENT_NOTFOUND)
		{
			UtilityFunctions::printerr("Failed to get Parameter Label By ID", __FUNCTION__, __FILE__, __LINE__);
			break;
		}
	} while (result == FMOD_ERR_TRUNCATED);

	if (result == FMOD_OK)
	{
		return String(raw_buffer.c_str());
	}

	return String();
}

int EventDescription::get_user_property_count() const
{
	int count{};

	if (ERROR_CHECK(event_description->getUserPropertyCount(&count)))
	{
		return count;
	}

	return count;
}

Ref<FmodTypes::FMOD_STUDIO_USER_PROPERTY> EventDescription::get_user_property_by_index(int index) const
{
	Ref<FmodTypes::FMOD_STUDIO_USER_PROPERTY> ref = create_ref<FmodTypes::FMOD_STUDIO_USER_PROPERTY>();

	FMOD_STUDIO_USER_PROPERTY property{};
	if (ERROR_CHECK(event_description->getUserPropertyByIndex(index, &property)))
	{
		ref->set_user_property(property);
		return ref;
	}

	return ref;
}

Ref<FmodTypes::FMOD_STUDIO_USER_PROPERTY> EventDescription::get_user_property(const String& name) const
{
	Ref<FmodTypes::FMOD_STUDIO_USER_PROPERTY> ref = create_ref<FmodTypes::FMOD_STUDIO_USER_PROPERTY>();

	FMOD_STUDIO_USER_PROPERTY property{};

	if (ERROR_CHECK(event_description->getUserProperty(name.utf8().get_data(), &property)))
	{
		ref->set_user_property(property);
		return ref;
	}

	return ref;
}

int EventDescription::get_length() const
{
	int length{};

	if (ERROR_CHECK(event_description->getLength(&length)))
	{
		return length;
	}

	return length;
}

Dictionary EventDescription::get_min_max_distance() const
{
	Dictionary min_max_distance;

	float min_distance{}, max_distance{};
	ERROR_CHECK(event_description->getMinMaxDistance(&min_distance, &max_distance));
	min_max_distance["min"] = min_distance;
	min_max_distance["max"] = max_distance;

	return min_max_distance;
}

float EventDescription::get_sound_size() const
{
	float sound_size{};

	if (ERROR_CHECK(event_description->getSoundSize(&sound_size)))
	{
		return sound_size;
	}

	return sound_size;
}

bool EventDescription::is_snapshot() const
{
	bool is_snapshot{};

	if (ERROR_CHECK(event_description->isSnapshot(&is_snapshot)))
	{
		return is_snapshot;
	}

	return is_snapshot;
}

bool EventDescription::is_oneshot() const
{
	bool is_oneshot{};

	if (ERROR_CHECK(event_description->isOneshot(&is_oneshot)))
	{
		return is_oneshot;
	}

	return is_oneshot;
}

bool EventDescription::is_stream() const
{
	bool is_stream{};

	if (ERROR_CHECK(event_description->isStream(&is_stream)))
	{
		return is_stream;
	}

	return is_stream;
}

bool EventDescription::is_3d() const
{
	bool is_3d{};

	if (ERROR_CHECK(event_description->is3D(&is_3d)))
	{
		return is_3d;
	}

	return is_3d;
}

bool EventDescription::is_doppler_enabled() const
{
	bool is_doppler_enabled{};

	if (ERROR_CHECK(event_description->isDopplerEnabled(&is_doppler_enabled)))
	{
		return is_doppler_enabled;
	}

	return is_doppler_enabled;
}

bool EventDescription::has_sustain_point() const
{
	bool has_sustain_point{};

	if (ERROR_CHECK(event_description->hasSustainPoint(&has_sustain_point)))
	{
		return has_sustain_point;
	}

	return has_sustain_point;
}

Ref<EventInstance> EventDescription::create_instance()
{
	Ref<EventInstance> ref = create_ref<EventInstance>();
	FMOD::Studio::EventInstance* instance = nullptr;

	if (ERROR_CHECK(event_description->createInstance(&instance)))
	{
		ref->set_instance(instance);
		return ref;
	}

	return ref;
}

int EventDescription::get_instance_count() const
{
	int count{};

	if (ERROR_CHECK(event_description->getInstanceCount(&count)))
	{
		return count;
	}

	return count;
}

TypedArray<EventInstance> EventDescription::get_instance_list(int capacity)
{
	TypedArray<EventInstance> instance_list;

	std::unique_ptr<FMOD::Studio::EventInstance*[]> instances =
			std::make_unique<FMOD::Studio::EventInstance*[]>(capacity);

	int event_instance_count{};
	if (ERROR_CHECK(event_description->getInstanceList(instances.get(), capacity, &event_instance_count)))
	{
		for (int i = 0; i < event_instance_count; i++)
		{
			Ref<EventInstance> ref = create_ref<EventInstance>();
			ref->set_instance(instances[i]);
			instance_list.append(ref);
		}
		return instance_list;
	}

	instance_list.resize(event_instance_count);

	return instance_list;
}

bool EventDescription::load_sample_data() const
{
	if (ERROR_CHECK(event_description->loadSampleData()))
	{
		return true;
	}

	return false;
}

bool EventDescription::unload_sample_data() const
{
	if (ERROR_CHECK(event_description->unloadSampleData()))
	{
		return true;
	}

	return false;
}

FMOD_STUDIO_LOADING_STATE EventDescription::get_sample_loading_state() const
{
	FMOD_STUDIO_LOADING_STATE loading_state = FMOD_STUDIO_LOADING_STATE::FMOD_STUDIO_LOADING_STATE_UNLOADED;

	if (ERROR_CHECK(event_description->getSampleLoadingState(&loading_state)))
	{
		return loading_state;
	}

	return loading_state;
}

bool EventDescription::release_all_instances() const
{
	if (ERROR_CHECK(event_description->releaseAllInstances()))
	{
		return true;
	}

	return false;
}

bool EventDescription::set_callback(Variant callable, FMOD_STUDIO_EVENT_CALLBACK_TYPE callback_mask) const
{
	Callable* callable_ptr = nullptr;

	void* user_data;
	event_description->getUserData(&user_data);
	callable_ptr = static_cast<Callable*>(user_data);

	if (callable.get_type() == Variant::Type::CALLABLE)
	{
		if (callable_ptr == nullptr)
		{
			callable_ptr = new Callable(callable);
			void* data = static_cast<void*>(callable_ptr);
			event_description->setUserData(data);

			return ERROR_CHECK(event_description->setCallback(
					[](FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE* event, void* parameters) -> FMOD_RESULT
					{
						FMOD::Studio::EventInstance* event_instance = reinterpret_cast<FMOD::Studio::EventInstance*>(event);
						void* data;
						event_instance->getUserData(&data);
						Callable* callable = static_cast<Callable*>(data);

						Dictionary callback_info;
						callback_info["type"] = type;
						Dictionary props;

						switch (type)
						{
							case FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER:
							{
								FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES* properties =
										static_cast<FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES*>(parameters);

								props["name"] = properties->name;
								props["position"] = properties->position;
								break;
							}
							case FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT:
							{
								FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES* properties =
										static_cast<FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES*>(parameters);

								props["bar"] = properties->bar;
								props["beat"] = properties->beat;
								props["position"] = properties->position;
								props["tempo"] = properties->tempo;
								props["timesignatureupper"] = properties->timesignatureupper;
								props["timesignaturelower"] = properties->timesignaturelower;
								break;
							}
							default:
								break;
						}

						callback_info["properties"] = props;
						Array args = Array();
						args.append(callback_info);
						callable->callv(args);
						return FMOD_OK;
					},
					callback_mask));
		}
	}
	else if (callable.get_type() == Variant::Type::NIL)
	{
		if (callable_ptr)
		{
			FMOD_RESULT result = event_description->setCallback(nullptr, FMOD_STUDIO_EVENT_CALLBACK_ALL);
			event_description->setUserData(nullptr);
			delete callable_ptr;
			return result;
		}
	}

	return false;
}

void EventInstance::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("is_valid"), &EventInstance::is_valid);
	ClassDB::bind_method(D_METHOD("get_description"), &EventInstance::get_description);
	ClassDB::bind_method(D_METHOD("get_volume"), &EventInstance::get_volume);
	ClassDB::bind_method(D_METHOD("set_volume", "volume"), &EventInstance::set_volume);
	ClassDB::bind_method(D_METHOD("get_pitch"), &EventInstance::get_pitch);
	ClassDB::bind_method(D_METHOD("set_pitch", "pitch"), &EventInstance::set_pitch);
	ClassDB::bind_method(D_METHOD("get_3d_attributes", "fmod_3d_attributes"), &EventInstance::get_3d_attributes);
	ClassDB::bind_method(D_METHOD("set_3d_attributes", "fmod_3d_attributes"), &EventInstance::set_3d_attributes);
	ClassDB::bind_method(D_METHOD("get_listener_mask"), &EventInstance::get_listener_mask);
	ClassDB::bind_method(D_METHOD("set_listener_mask", "mask"), &EventInstance::set_listener_mask);
	ClassDB::bind_method(D_METHOD("get_property", "event_property"), &EventInstance::get_property);
	ClassDB::bind_method(D_METHOD("set_property", "event_property", "property_value"), &EventInstance::set_property);
	ClassDB::bind_method(D_METHOD("get_reverb_level", "index"), &EventInstance::get_reverb_level);
	ClassDB::bind_method(D_METHOD("set_reverb_level", "index", "level"), &EventInstance::set_reverb_level);
	ClassDB::bind_method(D_METHOD("get_paused"), &EventInstance::get_paused);
	ClassDB::bind_method(D_METHOD("set_paused", "paused"), &EventInstance::set_paused);
	ClassDB::bind_method(D_METHOD("start"), &EventInstance::start);
	ClassDB::bind_method(D_METHOD("stop", "stop_mode"), &EventInstance::stop);
	ClassDB::bind_method(D_METHOD("get_timeline_position"), &EventInstance::get_timeline_position);
	ClassDB::bind_method(D_METHOD("set_timeline_position", "position"), &EventInstance::set_timeline_position);
	ClassDB::bind_method(D_METHOD("get_playback_state"), &EventInstance::get_playback_state);
	ClassDB::bind_method(D_METHOD("get_min_max_distance"), &EventInstance::get_min_max_distance);
	ClassDB::bind_method(D_METHOD("release"), &EventInstance::release);
	ClassDB::bind_method(D_METHOD("is_virtual"), &EventInstance::is_virtual);
	ClassDB::bind_method(D_METHOD("get_parameter_by_id", "parameter_id"), &EventInstance::get_parameter_by_id);
	ClassDB::bind_method(D_METHOD("set_parameter_by_id", "parameter_id", "parameter_value", "ignore_seek_speed"),
			&EventInstance::set_parameter_by_id);
	ClassDB::bind_method(D_METHOD("set_parameter_by_id_with_label", "parameter_id", "label", "ignore_seek_speed"),
			&EventInstance::set_parameter_by_id_with_label);
	ClassDB::bind_method(D_METHOD("set_parameters_by_ids", "parameter_ids", "values", "count", "ignore_seek_speed"),
			&EventInstance::set_parameters_by_ids);
	ClassDB::bind_method(D_METHOD("get_parameter_by_name", "name"), &EventInstance::get_parameter_by_name);
	ClassDB::bind_method(D_METHOD("set_parameter_by_name", "name", "parameter_value", "ignore_seek_speed"),
			&EventInstance::set_parameter_by_name);
	ClassDB::bind_method(D_METHOD("set_parameter_by_name_with_label", "name", "label", "ignore_seek_speed"),
			&EventInstance::set_parameter_by_name_with_label);
	ClassDB::bind_method(D_METHOD("key_off"), &EventInstance::key_off);
	ClassDB::bind_method(D_METHOD("get_cpu_usage", "cpu_usage"), &EventInstance::get_cpu_usage);
	ClassDB::bind_method(D_METHOD("get_memory_usage", "memory_usage_ref"), &EventInstance::get_memory_usage);
	ClassDB::bind_method(D_METHOD("set_callback", "callable", "callback_mask"), &EventInstance::set_callback);
}

void EventInstance::set_instance(FMOD::Studio::EventInstance* event_instance)
{
	this->event_instance = event_instance;
}

bool EventInstance::is_valid() const
{
	return event_instance->isValid();
}

Ref<EventDescription> EventInstance::get_description()
{
	Ref<EventDescription> ref = create_ref<EventDescription>();
	FMOD::Studio::EventDescription* description;

	if (ERROR_CHECK(event_instance->getDescription(&description)))
	{
		ref->set_instance(description);

		return ref;
	}

	return ref;
}

Dictionary EventInstance::get_volume() const
{
	float volume{}, final_volume{};
	Dictionary result;

	if (ERROR_CHECK(event_instance->getVolume(&volume, &final_volume)))
	{
		result["volume"] = volume;
		result["final_volume"] = final_volume;
		return result;
	}

	return result;
}

bool EventInstance::set_volume(float volume) const
{
	return ERROR_CHECK(event_instance->setVolume(volume));
}

Dictionary EventInstance::get_pitch() const
{
	float pitch{}, final_pitch{};
	Dictionary result;

	if (ERROR_CHECK(event_instance->getPitch(&pitch, &final_pitch)))
	{
		result["pitch"] = pitch;
		result["final_pitch"] = final_pitch;
		return result;
	}

	return result;
}

bool EventInstance::set_pitch(float pitch) const
{
	return ERROR_CHECK(event_instance->setPitch(pitch));
}

void EventInstance::get_3d_attributes(Ref<FmodTypes::FMOD_3D_ATTRIBUTES> attributes_ref) const
{
	FMOD_3D_ATTRIBUTES attributes{};

	if (ERROR_CHECK(event_instance->get3DAttributes(&attributes)))
	{
		attributes_ref->set_3d_attributes(attributes);
	}
}

bool EventInstance::set_3d_attributes(const Ref<FmodTypes::FMOD_3D_ATTRIBUTES>& attributes_ref) const
{
	FMOD_3D_ATTRIBUTES attributes{};
	attributes_ref->get_3d_attributes(attributes);

	return ERROR_CHECK(event_instance->set3DAttributes(&attributes));
}

unsigned int EventInstance::get_listener_mask() const
{
	unsigned int listener_mask{};

	if (ERROR_CHECK(event_instance->getListenerMask(&listener_mask)))
	{
		return listener_mask;
	}

	return listener_mask;
}

bool EventInstance::set_listener_mask(const unsigned int mask)
{
	if (ERROR_CHECK(event_instance->setListenerMask(mask)))
	{
		return true;
	}

	return false;
}

float EventInstance::get_property(int event_property) const
{
	float property{};

	if (ERROR_CHECK(event_instance->getProperty(static_cast<FMOD_STUDIO_EVENT_PROPERTY>(event_property), &property)))
	{
		return property;
	}

	return property;
}

bool EventInstance::set_property(int event_property, float property_value)
{
	if (ERROR_CHECK(
				event_instance->setProperty(static_cast<FMOD_STUDIO_EVENT_PROPERTY>(event_property), property_value)))
	{
		return true;
	}

	return false;
}

float EventInstance::get_reverb_level(int index) const
{
	float reverb_level{};

	if (ERROR_CHECK(event_instance->getReverbLevel(index, &reverb_level)))
	{
		return reverb_level;
	}

	return reverb_level;
}

bool EventInstance::set_reverb_level(int index, float level)
{
	if (ERROR_CHECK(event_instance->setReverbLevel(index, level)))
	{
		return true;
	}

	return false;
}

bool EventInstance::get_paused() const
{
	bool paused{};

	if (ERROR_CHECK(event_instance->getPaused(&paused)))
	{
		return paused;
	}

	return paused;
}

bool EventInstance::set_paused(bool paused) const
{
	return ERROR_CHECK(event_instance->setPaused(paused));
}

bool EventInstance::start() const
{
	return ERROR_CHECK(event_instance->start());
}

bool EventInstance::stop(int stop_mode) const
{
	return ERROR_CHECK(event_instance->stop(static_cast<FMOD_STUDIO_STOP_MODE>(stop_mode)));
}

int EventInstance::get_timeline_position() const
{
	int timeline_position{};

	if (ERROR_CHECK(event_instance->getTimelinePosition(&timeline_position)))
	{
		return timeline_position;
	}

	return timeline_position;
}

bool EventInstance::set_timeline_position(int position) const
{
	return ERROR_CHECK(event_instance->setTimelinePosition(position));
}

FMOD_STUDIO_PLAYBACK_STATE EventInstance::get_playback_state() const
{
	FMOD_STUDIO_PLAYBACK_STATE playback_state = FMOD_STUDIO_PLAYBACK_STATE::FMOD_STUDIO_PLAYBACK_STOPPED;

	if (ERROR_CHECK(event_instance->getPlaybackState(&playback_state)))
	{
		return playback_state;
	}

	return playback_state;
}

Dictionary EventInstance::get_min_max_distance() const
{
	Dictionary min_max_distance;

	float min_distance{}, max_distance{};

	if (ERROR_CHECK(event_instance->getMinMaxDistance(&min_distance, &max_distance)))
	{
		min_max_distance["min"] = min_distance;
		min_max_distance["max"] = max_distance;

		return min_max_distance;
	}

	return min_max_distance;
}

bool EventInstance::release()
{
	if (ERROR_CHECK(event_instance->release()))
	{
		return true;
	}

	return false;
}

bool EventInstance::is_virtual() const
{
	bool is_virtual{};

	if (ERROR_CHECK(event_instance->isVirtual(&is_virtual)))
	{
		return is_virtual;
	}

	return is_virtual;
}

Dictionary EventInstance::get_parameter_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id) const
{
	Dictionary result;
	float value{}, final_value{};

	FMOD_STUDIO_PARAMETER_ID id{};
	parameter_id->get_parameter_id(id);

	if (ERROR_CHECK(event_instance->getParameterByID(id, &value, &final_value)))
	{
		result["value"] = value;
		result["final_value"] = final_value;
		return result;
	}

	return result;
}

bool EventInstance::set_parameter_by_id(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id, float value,
		bool ignore_seek_speed) const
{
	FMOD_STUDIO_PARAMETER_ID id;
	parameter_id->get_parameter_id(id);

	return ERROR_CHECK(event_instance->setParameterByID(id, value, ignore_seek_speed));
}

bool EventInstance::set_parameter_by_id_with_label(const Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>& parameter_id,
		const String& label, bool ignore_seek_speed)
{
	FMOD_STUDIO_PARAMETER_ID id;
	parameter_id->get_parameter_id(id);

	return ERROR_CHECK(event_instance->setParameterByIDWithLabel(id, label.utf8().get_data(), ignore_seek_speed));
}

bool EventInstance::set_parameters_by_ids(const Array ids, const Array values, int count, bool ignore_seek_speed) const
{
	std::unique_ptr<FMOD_STUDIO_PARAMETER_ID[]> parameter_ids = std::make_unique<FMOD_STUDIO_PARAMETER_ID[]>(count);
	std::unique_ptr<float[]> parameter_values = std::make_unique<float[]>(count);

	for (int i = 0; i < count; i++)
	{
		Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID> ref = static_cast<Ref<FmodTypes::FMOD_STUDIO_PARAMETER_ID>>(ids[i]);
		ref->get_parameter_id(parameter_ids[i]);
		parameter_values[i] = values[i];
	}

	if (ERROR_CHECK(
				event_instance->setParametersByIDs(parameter_ids.get(), parameter_values.get(), count, ignore_seek_speed)))
	{
		return true;
	}

	return false;
}

Dictionary EventInstance::get_parameter_by_name(const String& name) const
{
	Dictionary result;
	float value{}, final_value{};

	if (ERROR_CHECK(event_instance->getParameterByName(name.utf8().get_data(), &value, &final_value)))
	{
		result["value"] = value;
		result["final_value"] = final_value;
		return result;
	}

	return result;
}

bool EventInstance::set_parameter_by_name(const String& name, float value, bool ignore_seek_speed) const
{
	return ERROR_CHECK(event_instance->setParameterByName(name.utf8().get_data(), value, ignore_seek_speed));
}

bool EventInstance::set_parameter_by_name_with_label(const String& name, const String& label, bool ignore_seek_speed)
{
	return ERROR_CHECK(event_instance->setParameterByNameWithLabel(name.utf8().get_data(), label.utf8().get_data(),
			ignore_seek_speed));
}

bool EventInstance::key_off() const
{
	return ERROR_CHECK(event_instance->keyOff());
}

void EventInstance::get_cpu_usage(Dictionary cpu_usage)
{
	unsigned int exclusive{}, inclusive{};

	if (ERROR_CHECK(event_instance->getCPUUsage(&exclusive, &inclusive)))
	{
		cpu_usage["exclusive"] = exclusive;
		cpu_usage["inclusive"] = inclusive;
	}
}

void EventInstance::get_memory_usage(Ref<FmodTypes::FMOD_STUDIO_MEMORY_USAGE> memory_usage_ref)
{
	FMOD_STUDIO_MEMORY_USAGE memory_usage{};

	if (ERROR_CHECK(event_instance->getMemoryUsage(&memory_usage)))
	{
		memory_usage_ref->set_memory_usage(memory_usage);
	}
}

bool EventInstance::set_callback(Variant callable, FMOD_STUDIO_EVENT_CALLBACK_TYPE callback_mask) const
{
	Callable* callable_ptr = nullptr;

	void* user_data;
	event_instance->getUserData(&user_data);
	callable_ptr = static_cast<Callable*>(user_data);

	if (callable.get_type() == Variant::Type::CALLABLE)
	{
		if (callable_ptr == nullptr)
		{
			callable_ptr = new Callable(callable);
			void* data = static_cast<void*>(callable_ptr);
			event_instance->setUserData(data);

			return ERROR_CHECK(event_instance->setCallback(
					[](FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE* event, void* parameters) -> FMOD_RESULT
					{
						FMOD::Studio::EventInstance* event_instance = reinterpret_cast<FMOD::Studio::EventInstance*>(event);
						void* data;
						event_instance->getUserData(&data);
						Callable* callable = static_cast<Callable*>(data);

						Dictionary callback_info;
						callback_info["type"] = type;
						Dictionary props;

						switch (type)
						{
							case FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER:
							{
								FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES* properties =
										static_cast<FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES*>(parameters);

								props["name"] = properties->name;
								props["position"] = properties->position;
								break;
							}
							case FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT:
							{
								FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES* properties =
										static_cast<FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES*>(parameters);

								props["bar"] = properties->bar;
								props["beat"] = properties->beat;
								props["position"] = properties->position;
								props["tempo"] = properties->tempo;
								props["timesignatureupper"] = properties->timesignatureupper;
								props["timesignaturelower"] = properties->timesignaturelower;
								break;
							}
							default:
								break;
						}

						callback_info["properties"] = props;
						Array args = Array();
						args.append(callback_info);
						callable->callv(args);
						return FMOD_OK;
					},
					callback_mask));
		}
	}
	else if (callable.get_type() == Variant::Type::NIL)
	{
		if (callable_ptr)
		{
			FMOD_RESULT result = event_instance->setCallback(nullptr, FMOD_STUDIO_EVENT_CALLBACK_ALL);
			event_instance->setUserData(nullptr);
			delete callable_ptr;
			return result;
		}
	}

	return false;
}

void Bus::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("is_valid"), &Bus::is_valid);
	ClassDB::bind_method(D_METHOD("get_id"), &Bus::get_id);
	ClassDB::bind_method(D_METHOD("get_path"), &Bus::get_path);
	ClassDB::bind_method(D_METHOD("get_volume"), &Bus::get_volume);
	ClassDB::bind_method(D_METHOD("set_volume", "volume"), &Bus::set_volume);
	ClassDB::bind_method(D_METHOD("get_paused"), &Bus::get_paused);
	ClassDB::bind_method(D_METHOD("set_paused", "paused"), &Bus::set_paused);
	ClassDB::bind_method(D_METHOD("get_mute"), &Bus::get_mute);
	ClassDB::bind_method(D_METHOD("set_mute", "mute"), &Bus::set_mute);
	ClassDB::bind_method(D_METHOD("stop_all_events", "stop_mode"), &Bus::stop_all_events);
	ClassDB::bind_method(D_METHOD("lock_channel_group"), &Bus::lock_channel_group);
	ClassDB::bind_method(D_METHOD("unlock_channel_group"), &Bus::unlock_channel_group);
	ClassDB::bind_method(D_METHOD("get_cpu_usage", "cpu_usage"), &Bus::get_cpu_usage);
	ClassDB::bind_method(D_METHOD("get_memory_usage", "memory_usage"), &Bus::get_memory_usage);
}

void Bus::set_instance(FMOD::Studio::Bus* bus)
{
	this->bus = bus;
}

bool Bus::is_valid() const
{
	return bus->isValid();
}

String Bus::get_id() const
{
	String id;
	FMOD_GUID guid{};

	if (ERROR_CHECK(bus->getID(&guid)))
	{
		id = guid_to_string(guid);
		return id;
	}

	return id;
}

String Bus::get_path() const
{
	String path;
	fmod_obj_to_path(bus, path);

	return path;
}

Dictionary Bus::get_volume() const
{
	Dictionary result;
	float volume{}, final_volume{};

	if (ERROR_CHECK(bus->getVolume(&volume, &final_volume)))
	{
		result["volume"] = volume;
		result["final_volume"] = final_volume;

		return result;
	}

	return result;
}

bool Bus::set_volume(float volume) const
{
	return ERROR_CHECK(bus->setVolume(volume));
}

bool Bus::get_paused() const
{
	bool paused{};

	if (ERROR_CHECK(bus->getPaused(&paused)))
	{
		return paused;
	}

	return paused;
}

bool Bus::set_paused(bool paused) const
{
	return ERROR_CHECK(bus->setPaused(paused));
}

bool Bus::get_mute() const
{
	bool muted{};

	if (ERROR_CHECK(bus->getMute(&muted)))
	{
		return muted;
	}

	return muted;
}

bool Bus::set_mute(bool mute) const
{
	return ERROR_CHECK(bus->setMute(mute));
}

bool Bus::stop_all_events(int stop_mode) const
{
	return ERROR_CHECK(bus->stopAllEvents(static_cast<FMOD_STUDIO_STOP_MODE>(stop_mode)));
}

bool Bus::lock_channel_group() const
{
	return ERROR_CHECK(bus->lockChannelGroup());
}

bool Bus::unlock_channel_group() const
{
	return ERROR_CHECK(bus->unlockChannelGroup());
}

void Bus::get_cpu_usage(Dictionary cpu_usage) const
{
	unsigned int exclusive{}, inclusive{};

	ERROR_CHECK(bus->getCPUUsage(&exclusive, &inclusive));
	cpu_usage["exclusive"] = exclusive;
	cpu_usage["inclusive"] = inclusive;
}

void Bus::get_memory_usage(Ref<FmodTypes::FMOD_STUDIO_MEMORY_USAGE> memory_usage_ref) const
{
	FMOD_STUDIO_MEMORY_USAGE memory_usage{};

	if (ERROR_CHECK(bus->getMemoryUsage(&memory_usage)))
	{
		memory_usage_ref->set_memory_usage(memory_usage);
	}
}

void VCA::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("is_valid"), &VCA::is_valid);
	ClassDB::bind_method(D_METHOD("get_id"), &VCA::get_id);
	ClassDB::bind_method(D_METHOD("get_path"), &VCA::get_path);
	ClassDB::bind_method(D_METHOD("get_volume"), &VCA::get_volume);
	ClassDB::bind_method(D_METHOD("set_volume", "volume"), &VCA::set_volume);
}

void VCA::set_instance(FMOD::Studio::VCA* vca)
{
	this->vca = vca;
}

bool VCA::is_valid() const
{
	return vca->isValid();
}

String VCA::get_id() const
{
	String id;
	FMOD_GUID guid{};

	if (ERROR_CHECK(vca->getID(&guid)))
	{
		id = guid_to_string(guid);
		return id;
	}

	return id;
}

String VCA::get_path() const
{
	String path;
	fmod_obj_to_path(vca, path);

	return path;
}

Dictionary VCA::get_volume() const
{
	Dictionary result;
	float volume{}, final_volume{};

	if (ERROR_CHECK(vca->getVolume(&volume, &final_volume)))
	{
		result["volume"] = volume;
		result["final_volume"] = final_volume;

		return result;
	}

	return result;
}

bool VCA::set_volume(float volume) const
{
	return ERROR_CHECK(vca->setVolume(volume));
}

void Bank::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("is_valid"), &Bank::is_valid);
	ClassDB::bind_method(D_METHOD("get_id"), &Bank::get_id);
	ClassDB::bind_method(D_METHOD("get_path"), &Bank::get_path);
	ClassDB::bind_method(D_METHOD("unload"), &Bank::unload);
	ClassDB::bind_method(D_METHOD("load_sample_data"), &Bank::load_sample_data);
	ClassDB::bind_method(D_METHOD("unload_sample_data"), &Bank::unload_sample_data);
	ClassDB::bind_method(D_METHOD("get_loading_state"), &Bank::get_loading_state);
	ClassDB::bind_method(D_METHOD("get_sample_loading_state"), &Bank::get_sample_loading_state);
	ClassDB::bind_method(D_METHOD("get_event_count"), &Bank::get_event_count);
	ClassDB::bind_method(D_METHOD("get_event_list", "capacity"), &Bank::get_event_list);
	ClassDB::bind_method(D_METHOD("get_bus_count"), &Bank::get_bus_count);
	ClassDB::bind_method(D_METHOD("get_bus_list", "capacity"), &Bank::get_bus_list);
	ClassDB::bind_method(D_METHOD("get_vca_count"), &Bank::get_vca_count);
	ClassDB::bind_method(D_METHOD("get_vca_list", "capacity"), &Bank::get_vca_list);
}

void Bank::set_instance(FMOD::Studio::Bank* bank)
{
	this->bank = bank;
}

bool Bank::is_valid() const
{
	return bank->isValid();
}

String Bank::get_id() const
{
	String id;
	FMOD_GUID guid{};

	if (ERROR_CHECK(bank->getID(&guid)))
	{
		id = guid_to_string(guid);
		return id;
	}

	return id;
}

String Bank::get_path() const
{
	String path;
	fmod_obj_to_path(bank, path);

	return path;
}

bool Bank::unload()
{
	if (ERROR_CHECK(bank->unload()))
	{
		return true;
	}

	return false;
}

bool Bank::load_sample_data() const
{
	if (ERROR_CHECK(bank->loadSampleData()))
	{
		return true;
	}

	return false;
}

bool Bank::unload_sample_data() const
{
	if (ERROR_CHECK(bank->unloadSampleData()))
	{
		return true;
	}

	return false;
}

FMOD_STUDIO_LOADING_STATE Bank::get_loading_state() const
{
	FMOD_STUDIO_LOADING_STATE loading_state = FMOD_STUDIO_LOADING_STATE_ERROR;

	if (ERROR_CHECK(bank->getLoadingState(&loading_state)))
	{
		return loading_state;
	}

	return loading_state;
}

FMOD_STUDIO_LOADING_STATE Bank::get_sample_loading_state() const
{
	FMOD_STUDIO_LOADING_STATE loading_state = FMOD_STUDIO_LOADING_STATE_ERROR;

	if (ERROR_CHECK(bank->getSampleLoadingState(&loading_state)))
	{
		return loading_state;
	}

	return loading_state;
}

int Bank::get_event_count() const
{
	int count{};

	if (bank->getEventCount(&count), "Failed to get Event Count for Bank")
	{
		return count;
	}

	return count;
}

TypedArray<EventDescription> Bank::get_event_list(int capacity)
{
	TypedArray<EventDescription> event_list;
	std::unique_ptr<FMOD::Studio::EventDescription*[]> descriptions =
			std::make_unique<FMOD::Studio::EventDescription*[]>(capacity);

	int count{};
	if (ERROR_CHECK(bank->getEventList(descriptions.get(), capacity, &count)))
	{
		for (int i = 0; i < count; i++)
		{
			Ref<EventDescription> ref = create_ref<EventDescription>();
			ref->set_instance(descriptions[i]);
			event_list.push_back(ref);
		}

		return event_list;
	}

	event_list.resize(count);

	return event_list;
}

int Bank::get_bus_count() const
{
	int count{};

	if (bank->getBusCount(&count), "Failed to get Bus Count for Bank")
	{
		return count;
	}

	return count;
}

TypedArray<Bus> Bank::get_bus_list(int capacity)
{
	TypedArray<Bus> bus_list;
	std::unique_ptr<FMOD::Studio::Bus*[]> busses = std::make_unique<FMOD::Studio::Bus*[]>(capacity);

	int count{};
	if (ERROR_CHECK(bank->getBusList(busses.get(), capacity, &count)))
	{
		for (int i = 0; i < count; i++)
		{
			Ref<Bus> ref = create_ref<Bus>();
			ref->set_instance(busses[i]);
			bus_list.push_back(ref);
		}

		return bus_list;
	}

	bus_list.resize(count);

	return bus_list;
}

int Bank::get_vca_count() const
{
	int count{};

	if (ERROR_CHECK(bank->getVCACount(&count)))
	{
		return count;
	}

	return count;
}

TypedArray<VCA> Bank::get_vca_list(int capacity)
{
	TypedArray<VCA> vca_list;
	std::unique_ptr<FMOD::Studio::VCA*[]> vcas = std::make_unique<FMOD::Studio::VCA*[]>(capacity);

	int count{};
	if (ERROR_CHECK(bank->getVCAList(vcas.get(), capacity, &count)))
	{
		for (int i = 0; i < count; i++)
		{
			Ref<VCA> vca_ref = create_ref<VCA>();
			vca_ref->set_instance(vcas[i]);
			vca_list.push_back(vca_ref);
		}

		return vca_list;
	}

	vca_list.resize(count);

	return vca_list;
}

void CommandReplay::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("is_valid"), &CommandReplay::is_valid);
	ClassDB::bind_method(D_METHOD("get_length"), &CommandReplay::get_length);
	ClassDB::bind_method(D_METHOD("get_command_count"), &CommandReplay::get_command_count);
	ClassDB::bind_method(D_METHOD("get_command_info", "index"), &CommandReplay::get_command_info);
	ClassDB::bind_method(D_METHOD("get_command_string", "index", "length"), &CommandReplay::get_command_string);
	ClassDB::bind_method(D_METHOD("get_command_at_time", "time"), &CommandReplay::get_command_at_time);
	ClassDB::bind_method(D_METHOD("set_bank_path", "bank_path"), &CommandReplay::set_bank_path);
	ClassDB::bind_method(D_METHOD("start"), &CommandReplay::start);
	ClassDB::bind_method(D_METHOD("stop"), &CommandReplay::stop);
	ClassDB::bind_method(D_METHOD("seek_to_time", "time"), &CommandReplay::seek_to_time);
	ClassDB::bind_method(D_METHOD("seek_to_command", "index"), &CommandReplay::seek_to_command);
	ClassDB::bind_method(D_METHOD("get_paused"), &CommandReplay::get_paused);
	ClassDB::bind_method(D_METHOD("set_paused", "paused"), &CommandReplay::set_paused);
	ClassDB::bind_method(D_METHOD("get_playback_state"), &CommandReplay::get_playback_state);
	ClassDB::bind_method(D_METHOD("get_current_command"), &CommandReplay::get_current_command);
	ClassDB::bind_method(D_METHOD("release"), &CommandReplay::release);
}

void CommandReplay::set_instance(FMOD::Studio::CommandReplay* command_replay)
{
	this->command_replay = command_replay;
}

bool CommandReplay::is_valid() const
{
	return command_replay->isValid();
}

float CommandReplay::get_length() const
{
	float length{};

	if (ERROR_CHECK(command_replay->getLength(&length)))
	{
		return length;
	}

	return length;
}

int CommandReplay::get_command_count() const
{
	int command_count{};

	if (ERROR_CHECK(command_replay->getCommandCount(&command_count)))
	{
		return command_count;
	}

	return command_count;
}

Dictionary CommandReplay::get_command_info(int index) const
{
	Dictionary command_info;
	FMOD_STUDIO_COMMAND_INFO info{};

	if (ERROR_CHECK(command_replay->getCommandInfo(index, &info)))
	{
		command_info["command_name"] = String(info.commandname);
		command_info["parent_command_index"] = info.parentcommandindex;
		command_info["frame_number"] = info.framenumber;
		command_info["frame_time"] = info.frametime;
		command_info["instance_type"] = static_cast<int>(info.instancetype);
		command_info["output_type"] = static_cast<int>(info.outputtype);
		command_info["instance_handle"] = info.instancehandle;
		command_info["output_handle"] = info.outputhandle;

		return command_info;
	}

	return command_info;
}

String CommandReplay::get_command_string(int index, int length) const
{
	String command_string;

	auto buffer = std::string(length, ' ');

	if (ERROR_CHECK(command_replay->getCommandString(index, buffer.data(), length)))
	{
		command_string = String(buffer.c_str());
		return command_string;
	}

	return command_string;
}

int CommandReplay::get_command_at_time(float time) const
{
	int commandAtTime{};

	if (ERROR_CHECK(command_replay->getCommandAtTime(time, &commandAtTime)))
	{
		return commandAtTime;
	}

	return commandAtTime;
}

bool CommandReplay::set_bank_path(const String& bank_path)
{
	return ERROR_CHECK(command_replay->setBankPath(bank_path.utf8().get_data()));
}

bool CommandReplay::start()
{
	return ERROR_CHECK(command_replay->start());
}

bool CommandReplay::stop()
{
	return ERROR_CHECK(command_replay->stop());
}

bool CommandReplay::seek_to_time(float time)
{
	return ERROR_CHECK(command_replay->seekToTime(time));
}

bool CommandReplay::seek_to_command(int index)
{
	return ERROR_CHECK(command_replay->seekToCommand(index));
}

bool CommandReplay::get_paused() const
{
	bool paused{};

	if (ERROR_CHECK(command_replay->getPaused(&paused)))
	{
		return paused;
	}

	return paused;
}

bool CommandReplay::set_paused(bool paused)
{
	return ERROR_CHECK(command_replay->setPaused(paused));
}

FMOD_STUDIO_PLAYBACK_STATE CommandReplay::get_playback_state() const
{
	FMOD_STUDIO_PLAYBACK_STATE playback_state = FMOD_STUDIO_PLAYBACK_STATE::FMOD_STUDIO_PLAYBACK_STOPPED;

	if (ERROR_CHECK(command_replay->getPlaybackState(&playback_state)))
	{
		return playback_state;
	}

	return playback_state;
}

Dictionary CommandReplay::get_current_command() const
{
	Dictionary current_command;

	int index{};
	float current_time{};

	if (ERROR_CHECK(command_replay->getCurrentCommand(&index, &current_time)))
	{
		current_command["command_index"] = index;
		current_command["current_time"] = current_time;
		return current_command;
	}

	return current_command;
}

bool CommandReplay::release()
{
	return ERROR_CHECK(command_replay->release());
}
