#ifndef STUDIO_BANK_LOADER_H
#define STUDIO_BANK_LOADER_H

#include "fmod_assets.h"
#include "fmod_studio_module.h"
#include "utils/runtime_utils.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>

class StudioBankLoader : public Node
{
	GDCLASS(StudioBankLoader, Node);

protected:
	static void _bind_methods();
	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name, Variant& r_ret) const;
	void _get_property_list(List<PropertyInfo>* p_list) const;

private:
	RuntimeUtils::GameEvent load_on = RuntimeUtils::GameEvent::GAMEEVENT_NONE;
	RuntimeUtils::GameEvent unload_on = RuntimeUtils::GameEvent::GAMEEVENT_NONE;
	int num_banks = 0;
	Array banks;
	bool preload_sample_data = false;

	void set_bank_ref(int index, const Variant& value);

public:
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void handle_game_event(RuntimeUtils::GameEvent game_event);

	void load();
	void unload();

	void set_load_on(RuntimeUtils::GameEvent load_on);
	RuntimeUtils::GameEvent get_load_on() const;

	void set_unload_on(RuntimeUtils::GameEvent unload_on);
	RuntimeUtils::GameEvent get_unload_on() const;

	void set_num_banks(int num_banks);
	int get_num_banks() const;

	void set_preload_sample_data(bool preload_sample_data);
	bool get_preload_sample_data() const;
};

#endif // STUDIO_BANK_LOADER_H