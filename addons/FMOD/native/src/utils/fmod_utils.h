#ifndef FMOD_UTILS
#define FMOD_UTILS

#include <fmod_errors.h>
#include <fmod_studio_common.h>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

enum class PlatformSettingsPath
{
	FMOD_SETTINGS_PATH,
	GODOT_WINDOWS_SETTING_POSTFIX,
	GODOT_MAC_OSX_SETTING_POSTFIX,
	GODOT_LINUX_SETTING_POSTFIX,
	GODOT_IOS_SETTING_POSTFIX,
	GODOT_ANDROID_SETTING_POSTFIX
};

static bool error_check(const FMOD_RESULT result, const char* function, const char* file,
		int line)
{
	if (result != FMOD_OK)
	{
		String warning_message = "[WARNING]: " + String(FMOD_ErrorString(result)) + " " + function + " in " + file +
				":" + String::num(line);
		UtilityFunctions::push_warning(warning_message);

		return false;
	}

	return true;
}

#define ERROR_CHECK(result) error_check(result, __FUNCTION__, __FILE__, __LINE__)

static void debug_monitor(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* func, const char* message)
{
	if (flags & FMOD_DEBUG_LEVEL_ERROR)
	{
		UtilityFunctions::print(String("[ERROR]: "), String(message));
	}
	else if (flags & FMOD_DEBUG_LEVEL_WARNING)
	{
		UtilityFunctions::print(String("[WARNING]: "), String(message));
	}
	else if (flags & FMOD_DEBUG_LEVEL_LOG)
	{
		UtilityFunctions::print(String("[LOG]: "), String(message));
	}
}

static String get_platform_setting_path(PlatformSettingsPath path)
{
	switch (path)
	{
		case PlatformSettingsPath::FMOD_SETTINGS_PATH:
			return "fmod/settings/";
		case PlatformSettingsPath::GODOT_WINDOWS_SETTING_POSTFIX:
			return ".windows";
		case PlatformSettingsPath::GODOT_MAC_OSX_SETTING_POSTFIX:
			return ".macos";
		case PlatformSettingsPath::GODOT_LINUX_SETTING_POSTFIX:
			return ".linux";
		case PlatformSettingsPath::GODOT_IOS_SETTING_POSTFIX:
			return ".ios";
		case PlatformSettingsPath::GODOT_ANDROID_SETTING_POSTFIX:
			return ".android";
		default:
			return "";
	}
}

static Variant get_platform_project_setting(const String& setting, bool force_platform_agnostic = false)
{
	ProjectSettings* project_settings = ProjectSettings::get_singleton();

	String platformSetting = setting;

#ifdef FMOD_WIN
	platformSetting = platformSetting + get_platform_setting_path(PlatformSettingsPath::GODOT_WINDOWS_SETTING_POSTFIX);
#elif FMOD_OSX
	platformSetting = platformSetting + get_platform_setting_path(PlatformSettingsPath::GODOT_MAC_OSX_SETTING_POSTFIX);
#elif FMOD_LINUX
	platformSetting = platformSetting + get_platform_setting_path(PlatformSettingsPath::GODOT_LINUX_SETTING_POSTFIX);
#elif FMOD_IOS
	platformSetting = platformSetting + get_platform_setting_path(PlatformSettingsPath::GODOT_IOS_SETTING_POSTFIX);
#elif FMOD_ANDROID
	platformSetting = platformSetting + get_platform_setting_path(PlatformSettingsPath::GODOT_ANDROID_SETTING_POSTFIX);
#else
#error "Platform not supported"
#endif

	// Try to get the platform-specific setting, if it exists
	if (project_settings && project_settings->has_setting(platformSetting) && !force_platform_agnostic)
	{
		return project_settings->get(platformSetting);
	}

	// Otherwise,´try to get the default platform-agnostic setting
	if (project_settings && project_settings->has_setting(setting))
	{
		return project_settings->get(setting);
	}
	else
	{
		UtilityFunctions::print("[FMOD] Failed to get setting: " + platformSetting);
		return "";
	}
}

enum class VectorType
{
	POSITION,
	FORWARD,
	UP,
	VELOCITY
};

static inline void vector3_to_fmod_vector(const Vector3& in_vector, FMOD_VECTOR& out_vector)
{
	out_vector.x = in_vector.x;
	out_vector.y = in_vector.y;
	out_vector.z = in_vector.z;
}

static inline void fmod_vector_to_vector3(const FMOD_VECTOR& in_vector, Vector3& out_vector)
{
	out_vector.x = in_vector.x;
	out_vector.y = in_vector.y;
	out_vector.z = in_vector.z;
}

static inline void vector2_to_fmod_vector(const Vector2& in_vector, FMOD_VECTOR& out_vector, float distance_scale_2d = 1.0f)
{
	// todo(alex): What should we do with out_vector.z? How do Godot 2D games handle depth?
	out_vector.x = in_vector.x / distance_scale_2d;
	out_vector.y = in_vector.y / distance_scale_2d;
	out_vector.z = 0.0f;
}

static inline void transform3d_to_fmod_vector(const Transform3D& in_transform, FMOD_VECTOR& out_vector,
		const VectorType& type, Object* physicsbody3d = nullptr)
{
	switch (type)
	{
		case VectorType::POSITION:
			vector3_to_fmod_vector(in_transform.get_origin(), out_vector);
			break;
		case VectorType::FORWARD:
			vector3_to_fmod_vector(-in_transform.get_basis().get_column(2).normalized(), out_vector);
			break;
		case VectorType::UP:
			vector3_to_fmod_vector(in_transform.get_basis().get_column(1).normalized(), out_vector);
			break;
		case VectorType::VELOCITY:
			if (physicsbody3d)
			{
				const RigidBody3D* rigidbody3d = Object::cast_to<RigidBody3D>(physicsbody3d);
				if (rigidbody3d)
				{
					vector3_to_fmod_vector(rigidbody3d->get_linear_velocity(), out_vector);
				}
				else
				{
					const CharacterBody3D* characterbody3d = Object::cast_to<CharacterBody3D>(physicsbody3d);
					if (characterbody3d)
					{
						vector3_to_fmod_vector(characterbody3d->get_velocity(), out_vector);
					}
				}
			}
			else
			{
				vector3_to_fmod_vector(Vector3(0.0f, 0.0f, 0.0f), out_vector);
			}
			break;
		default:
			break;
	}
}

static inline void transform3d_to_3dattributes(const Transform3D& in_transform, FMOD_3D_ATTRIBUTES& out_attributes, Object* physicsbody3d = nullptr)
{
	transform3d_to_fmod_vector(in_transform, out_attributes.position, VectorType::POSITION);
	transform3d_to_fmod_vector(in_transform, out_attributes.forward, VectorType::FORWARD);
	transform3d_to_fmod_vector(in_transform, out_attributes.up, VectorType::UP);
	transform3d_to_fmod_vector(in_transform, out_attributes.velocity, VectorType::VELOCITY, physicsbody3d);
}

static inline void transform2d_to_fmod_vector(const Transform2D& in_transform, FMOD_VECTOR& out_vector,
		const VectorType& type, Object* physicsbody2d = nullptr, float distance_scale_2d = 1.0f)
{
	switch (type)
	{
		case VectorType::POSITION:
			vector3_to_fmod_vector(Vector3(in_transform.get_origin().x / distance_scale_2d, (in_transform.get_origin().y * -1.0f) / distance_scale_2d, 0.0f), out_vector);
			break;
		case VectorType::FORWARD:
			vector3_to_fmod_vector(Vector3(in_transform.columns[1].x, 0.0f, in_transform.columns[1].y).normalized(),
					out_vector);
			break;
		case VectorType::UP:
			vector3_to_fmod_vector(Vector3(0.0f, 1.0f, 0.0f), out_vector);
			break;
		case VectorType::VELOCITY:
			if (physicsbody2d)
			{
				const RigidBody2D* rigidbody2d = Object::cast_to<RigidBody2D>(physicsbody2d);
				if (rigidbody2d)
				{
					vector3_to_fmod_vector(Vector3(rigidbody2d->get_linear_velocity().x, rigidbody2d->get_linear_velocity().y, 0),
							out_vector);
				}
				else
				{
					const CharacterBody2D* characterbody2d = Object::cast_to<CharacterBody2D>(physicsbody2d);
					if (characterbody2d)
					{
						vector3_to_fmod_vector(Vector3(characterbody2d->get_velocity().x, characterbody2d->get_velocity().y, 0),
								out_vector);
					}
				}
			}
			else
			{
				vector3_to_fmod_vector(Vector3(0.0f, 0.0f, 0.0f), out_vector);
			}
			break;
		default:
			break;
	}
}

static inline void transform2d_to_3dattributes(const Transform2D& in_transform, FMOD_3D_ATTRIBUTES& out_attributes, Object* physicsbody2d = nullptr, float distance_scale_2d = 1.0f)
{
	transform2d_to_fmod_vector(in_transform, out_attributes.position, VectorType::POSITION, nullptr, distance_scale_2d);
	transform2d_to_fmod_vector(in_transform, out_attributes.forward, VectorType::FORWARD);
	transform2d_to_fmod_vector(in_transform, out_attributes.up, VectorType::UP);
	transform2d_to_fmod_vector(in_transform, out_attributes.velocity, VectorType::VELOCITY, physicsbody2d);
}

static inline String guid_to_string(const FMOD_GUID& guid)
{
	std::array<char, 40> output{};
	snprintf(output.data(), output.size(), "{%08x-%04hx-%04hx-%02x%02x-%02x%02x%02x%02x%02x%02x}", guid.Data1,
			guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4],
			guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	return String(output.data());
}

template <class T>
static inline Ref<T> create_ref()
{
	Ref<T> ref;
	ref.instantiate();
	return ref;
}

template <class T>
static inline void fmod_obj_to_path(const T& obj, String& out_path)
{
	int actualSize = 512;
	auto rawBuffer = std::string(512, ' ');
	FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_TRUNCATED;
	do
	{
		rawBuffer.resize(actualSize);
		result = obj->getPath(&rawBuffer[0], actualSize, &actualSize);

		if (result == FMOD_ERR_EVENT_NOTFOUND)
		{
			UtilityFunctions::printerr("[FMOD] Failed to get the Path of an FMOD Object", __FUNCTION__, __FILE__,
					__LINE__);
			break;
		}
	} while (result == FMOD_ERR_TRUNCATED);

	if (result == FMOD_OK)
	{
		out_path = String(rawBuffer.c_str());
	}
	else
	{
		out_path = String();
	}
}

#endif // FMOD_UTILS