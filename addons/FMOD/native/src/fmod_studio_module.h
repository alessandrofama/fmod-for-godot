#ifndef FMOD_STUDIO_MODULE_H
#define FMOD_STUDIO_MODULE_H

#include "api/studio_api.h"
#include "misc/fmod_io.h"
#include "misc/fmod_memory.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/os.hpp>

#ifdef FMOD_OSX
#include <AudioToolbox/AudioToolbox.h>
#endif

class FMODStudioModule : public Object
{
	GDCLASS(FMODStudioModule, Object);

protected:
	static FMODStudioModule* singleton;
	static void _bind_methods();

private:
	FMOD::Studio::System* studio_system;
	Ref<StudioApi::StudioSystem> studio_system_ref;

	bool initialize_fmod();
	void initialize_debug_functions();
	bool shutdown_fmod();

public:
	static FMODStudioModule* get_singleton();
	FMODStudioModule();
	~FMODStudioModule();

	float distance_scale_2d{ 1.0f };

	bool init();
	void shutdown();

	Ref<StudioApi::StudioSystem> get_studio_system_ref();
	FMOD::Studio::System* get_studio_system() const;
	FMOD::System* get_core_system() const;

	using FMOD_STUDIO_LOADING_STATE = ::FMOD_STUDIO_LOADING_STATE;
	using FMOD_STUDIO_LOAD_MEMORY_MODE = ::FMOD_STUDIO_LOAD_MEMORY_MODE;
	using FMOD_STUDIO_PARAMETER_TYPE = ::FMOD_STUDIO_PARAMETER_TYPE;
	using FMOD_STUDIO_USER_PROPERTY_TYPE = ::FMOD_STUDIO_USER_PROPERTY_TYPE;
	using FMOD_STUDIO_EVENT_PROPERTY = ::FMOD_STUDIO_EVENT_PROPERTY;
	using FMOD_STUDIO_PLAYBACK_STATE = ::FMOD_STUDIO_PLAYBACK_STATE;
	using FMOD_STUDIO_STOP_MODE = ::FMOD_STUDIO_STOP_MODE;
	using FMOD_STUDIO_INSTANCETYPE = ::FMOD_STUDIO_INSTANCETYPE;
};

VARIANT_ENUM_CAST(FMODStudioModule::FMOD_STUDIO_LOADING_STATE);
VARIANT_ENUM_CAST(FMODStudioModule::FMOD_STUDIO_LOAD_MEMORY_MODE);
VARIANT_ENUM_CAST(FMODStudioModule::FMOD_STUDIO_PARAMETER_TYPE);
VARIANT_ENUM_CAST(FMODStudioModule::FMOD_STUDIO_USER_PROPERTY_TYPE);
VARIANT_ENUM_CAST(FMODStudioModule::FMOD_STUDIO_EVENT_PROPERTY);
VARIANT_ENUM_CAST(FMODStudioModule::FMOD_STUDIO_PLAYBACK_STATE);
VARIANT_ENUM_CAST(FMODStudioModule::FMOD_STUDIO_STOP_MODE);
VARIANT_ENUM_CAST(FMODStudioModule::FMOD_STUDIO_INSTANCETYPE);

#endif // FMOD_STUDIO_MODULE_H