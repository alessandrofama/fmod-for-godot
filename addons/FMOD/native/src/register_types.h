#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include "gdextension_interface.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "api/studio_api.h"
#include "editor/inspector_browser.h"
#include "editor/project_browser.h"
#include "editor/studio_event_emitter_3d_gizmo_plugin.h"
#include "fmod_assets.h"
#include "fmod_studio_editor_module.h"
#include "fmod_studio_module.h"
#include "scene/studio_bank_loader.h"
#include "scene/studio_event_emitter.h"
#include "scene/studio_global_parameter_trigger.h"
#include "scene/studio_listener.h"
#include "scene/studio_parameter_trigger.h"
#include "utils/debug_monitors.h"
#include "utils/runtime_utils.h"
#include "utils/tcp_client.h"

using namespace godot;

void register_fmod_types(ModuleInitializationLevel p_level);
void unregister_fmod_types(ModuleInitializationLevel p_level);

#endif // REGISTER_TYPES_H