#ifndef FMOD_DEBUG_MONITOR_H
#define FMOD_DEBUG_MONITOR_H

#include "fmod_studio_module.h"
#include <godot_cpp/classes/performance.hpp>
#include <godot_cpp/classes/time.hpp>

class FMODDebugMonitor : public Node
{
	GDCLASS(FMODDebugMonitor, Node);

protected:
	static void _bind_methods();

private:
	uint64_t last_debug_update = 0;
	float dsp_cpu_info = 0.0f;
	float studio_cpu_info = 0.0f;
	int current_alloc_info = 0;
	int max_alloc_info = 0;
	int channels_info = 0;
	int real_channels_info = 0;
	FMOD::Studio::System* studio_system = nullptr;
	FMOD::System* core_system = nullptr;

public:
	virtual void _enter_tree() override;
	virtual void _exit_tree() override;
	virtual void _process(double delta) override;

	void process_debug_info();
	float get_cpu_dsp_usage();
	float get_cpu_studio_update();
	int get_current_memory_alloc();
	int get_max_memory_alloc();
	int get_channels();
	int get_real_channels();
};

#endif // FMOD_DEBUG_MONITOR_H