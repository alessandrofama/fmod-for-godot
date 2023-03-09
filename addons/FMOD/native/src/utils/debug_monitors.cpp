#include "debug_monitors.h"

using namespace godot;

void FMODDebugMonitor::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_cpu_dsp_usage"), &FMODDebugMonitor::get_cpu_dsp_usage);
	ClassDB::bind_method(D_METHOD("get_cpu_studio_update"), &FMODDebugMonitor::get_cpu_studio_update);
	ClassDB::bind_method(D_METHOD("get_current_memory_alloc"), &FMODDebugMonitor::get_current_memory_alloc);
	ClassDB::bind_method(D_METHOD("get_max_memory_alloc"), &FMODDebugMonitor::get_max_memory_alloc);
	ClassDB::bind_method(D_METHOD("get_channels"), &FMODDebugMonitor::get_channels);
	ClassDB::bind_method(D_METHOD("get_real_channels"), &FMODDebugMonitor::get_real_channels);
}

void FMODDebugMonitor::_enter_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	Performance* performance = Performance::get_singleton();

	if (performance == nullptr)
	{
		return;
	}

	studio_system = FMODStudioModule::get_singleton()->get_studio_system();

	if (studio_system == nullptr)
	{
		return;
	}

	core_system = FMODStudioModule::get_singleton()->get_core_system();

	if (core_system == nullptr)
	{
		return;
	}

	if (!performance->has_custom_monitor("FMOD/DSP CPU Usage (%)"))
	{
		performance->add_custom_monitor("FMOD/DSP CPU Usage (%)", Callable(this, "get_cpu_dsp_usage"));
	}
	if (!performance->has_custom_monitor("FMOD/Studio CPU Update (%)"))
	{
		performance->add_custom_monitor("FMOD/Studio CPU Update (%)",
				Callable(this, "get_cpu_studio_update"));
	}
	if (!performance->has_custom_monitor("FMOD/Memory Allocation (MB)"))
	{
		performance->add_custom_monitor("FMOD/Memory Allocation (MB)",
				Callable(this, "get_current_memory_alloc"));
	}
	if (!performance->has_custom_monitor("FMOD/Max Memory Allocation (MB)"))
	{
		performance->add_custom_monitor("FMOD/Max Memory Allocation (MB)",
				Callable(this, "get_max_memory_alloc"));
	}
	if (!performance->has_custom_monitor("FMOD/Channels"))
	{
		performance->add_custom_monitor("FMOD/Channels", Callable(this, "get_channels"));
	}
	if (!performance->has_custom_monitor("FMOD/Real Channels"))
	{
		performance->add_custom_monitor("FMOD/Real Channels", Callable(this, "get_real_channels"));
	}
}

void FMODDebugMonitor::_exit_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	if (!core_system)
	{
		return;
	}
}

void FMODDebugMonitor::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	if (last_debug_update + 250 < Time::get_singleton()->get_ticks_msec())
	{
		process_debug_info();

		last_debug_update = Time::get_singleton()->get_ticks_msec();
	}
}

void FMODDebugMonitor::process_debug_info()
{
	if (!core_system || !studio_system)
	{
		return;
	}

	FMOD_STUDIO_CPU_USAGE studioUsage{};
	FMOD_CPU_USAGE coreUsage{};
	ERROR_CHECK(studio_system->getCPUUsage(&studioUsage, &coreUsage));

	int currentAlloc{}, maxAlloc{};
	ERROR_CHECK(FMOD::Memory_GetStats(&currentAlloc, &maxAlloc));
	currentAlloc = currentAlloc >> 20;
	maxAlloc = maxAlloc >> 20;

	int channels{}, real_channels{};
	ERROR_CHECK(core_system->getChannelsPlaying(&channels, &real_channels));

	dsp_cpu_info = coreUsage.dsp;
	studio_cpu_info = studioUsage.update;
	current_alloc_info = currentAlloc;
	max_alloc_info = maxAlloc;
	channels_info = channels;
	real_channels_info = real_channels;
}

float FMODDebugMonitor::get_cpu_dsp_usage()
{
	return dsp_cpu_info;
}

float FMODDebugMonitor::get_cpu_studio_update()
{
	return studio_cpu_info;
}

int FMODDebugMonitor::get_current_memory_alloc()
{
	return current_alloc_info;
}

int FMODDebugMonitor::get_max_memory_alloc()
{
	return max_alloc_info;
}

int FMODDebugMonitor::get_channels()
{
	return channels_info;
}

int FMODDebugMonitor::get_real_channels()
{
	return real_channels_info;
}