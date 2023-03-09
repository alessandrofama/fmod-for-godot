#include "register_types.h"

using namespace godot;

static FMODStudioModule* fmod_module;
static FMODStudioEditorModule* fmod_editor_module;
static FMODSettings* fmod_settings;

void register_fmod_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
		ClassDB::register_class<FmodTCPClient>();
		ClassDB::register_class<FMODStudioEditorModule>();
		fmod_editor_module = memnew(FMODStudioEditorModule);
		Engine::get_singleton()->register_singleton("FMODStudioEditorModule", FMODStudioEditorModule::get_singleton());

		// Gizmo Plugins
		ClassDB::register_class<StudioEventEmitter3DGizmoPlugin>();

		// Editor Utilities
		ClassDB::register_class<ProjectCache>();

		ClassDB::register_class<ProjectBrowserPreviewButton>();
		ClassDB::register_class<InspectorBrowser>();
		ClassDB::register_class<InspectorBrowserTree>();
		ClassDB::register_class<InspectorBrowserProperty>();
		// ClassDB::register_class<InspectorBrowserPlugin>();

		ClassDB::register_class<DiscreteParameterControl>();
		ClassDB::register_class<LabeledParameterControl>();
		ClassDB::register_class<ContinuousParameterControl>();

		ClassDB::register_class<ProjectBrowserTree>();
		ClassDB::register_class<ProjectBrowserWindow>();
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		// FMOD modules
		ClassDB::register_class<FMODStudioModule>();
		fmod_module = memnew(FMODStudioModule);
		Engine::get_singleton()->register_singleton("FMODStudioModule", FMODStudioModule::get_singleton());

		ClassDB::register_class<FMODSettings>();
		fmod_settings = memnew(FMODSettings);

		// Studio API
		ClassDB::register_class<StudioApi::StudioSystem>();
		ClassDB::register_class<StudioApi::EventDescription>();
		ClassDB::register_class<StudioApi::EventInstance>();
		ClassDB::register_class<StudioApi::Bus>();
		ClassDB::register_class<StudioApi::Bank>();
		ClassDB::register_class<StudioApi::VCA>();
		ClassDB::register_class<StudioApi::CommandReplay>();

		// FMOD specific types
		ClassDB::register_class<FmodTypes::FMOD_STUDIO_PARAMETER_ID>();
		ClassDB::register_class<FmodTypes::FMOD_STUDIO_PARAMETER_DESCRIPTION>();
		ClassDB::register_class<FmodTypes::FMOD_3D_ATTRIBUTES>();
		ClassDB::register_class<FmodTypes::FMOD_STUDIO_BUFFER_INFO>();
		ClassDB::register_class<FmodTypes::FMOD_STUDIO_BUFFER_USAGE>();
		ClassDB::register_class<FmodTypes::FMOD_STUDIO_CPU_USAGE>();
		ClassDB::register_class<FmodTypes::FMOD_CPU_USAGE>();
		ClassDB::register_class<FmodTypes::FMOD_STUDIO_MEMORY_USAGE>();
		ClassDB::register_class<FmodTypes::FMOD_STUDIO_USER_PROPERTY>();

		// FMOD Resources
		ClassDB::register_class<FMODAsset>();
		ClassDB::register_class<BankAsset>();
		ClassDB::register_class<EventAsset>();
		ClassDB::register_class<BusAsset>();
		ClassDB::register_class<VCAAsset>();
		ClassDB::register_class<ParameterAsset>();

		// Runtime Utils
		ClassDB::register_class<RuntimeUtils>();

		// Custom nodes
		ClassDB::register_class<StudioEventEmitter3D>();
		ClassDB::register_class<StudioEventEmitter2D>();
		ClassDB::register_class<StudioParameterTrigger>();
		ClassDB::register_class<StudioGlobalParameterTrigger>();
		ClassDB::register_class<StudioBankLoader>();
		ClassDB::register_class<StudioListener3D>();
		ClassDB::register_class<StudioListener2D>();

		// Custom performance monitors
		ClassDB::register_class<FMODDebugMonitor>();
	}
}

void unregister_fmod_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
		Engine::get_singleton()->unregister_singleton("FMODStudioEditorModule");
		memdelete(fmod_editor_module);
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		Engine::get_singleton()->unregister_singleton("FMODStudioModule");
		memdelete(fmod_module);
	}
}

extern "C"
{
	// Initialization.

	GDExtensionBool GDE_EXPORT fmod_library_init(const GDExtensionInterface* p_interface,
			const GDExtensionClassLibraryPtr p_library,
			GDExtensionInitialization* r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

		init_obj.register_initializer(register_fmod_types);
		init_obj.register_terminator(unregister_fmod_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

		return init_obj.init();
	}
}