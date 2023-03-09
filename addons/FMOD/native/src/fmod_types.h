#ifndef FMOD_TYPES_H
#define FMOD_TYPES_H

#include "utils/fmod_utils.h"
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

namespace FmodTypes
{

class FMOD_STUDIO_PARAMETER_ID : public Resource
{
	GDCLASS(FMOD_STUDIO_PARAMETER_ID, Resource);

protected:
	static void _bind_methods();

private:
	unsigned int data1{};
	unsigned int data2{};

public:
	void set_data1(const unsigned int& data1);
	unsigned int get_data1() const;
	void set_data2(const unsigned int& data2);
	unsigned int get_data2() const;

	void set_parameter_id(const ::FMOD_STUDIO_PARAMETER_ID& parameter_id);
	void get_parameter_id(::FMOD_STUDIO_PARAMETER_ID& out_id) const;
};

class FMOD_STUDIO_PARAMETER_DESCRIPTION : public Resource
{
	GDCLASS(FMOD_STUDIO_PARAMETER_DESCRIPTION, Resource);

protected:
	static void _bind_methods();

private:
	String name{};
	Ref<FMOD_STUDIO_PARAMETER_ID> id;
	float minimum{};
	float maximum{};
	float defaultvalue{};
	FMOD_STUDIO_PARAMETER_TYPE type{};
	int flags{};
	String guid{};

public:
	void set_name(const String& name);
	String get_name() const;
	void set_id(const Ref<FMOD_STUDIO_PARAMETER_ID>& id);
	Ref<FMOD_STUDIO_PARAMETER_ID> get_id() const;
	void set_minimum(float minimum);
	float get_minimum() const;
	void set_maximum(float maximum);
	float get_maximum() const;
	void set_default_value(float defaultValue);
	float get_default_value() const;
	void set_type(int type);
	int get_type() const;
	void set_flags(int flags);
	int get_flags() const;
	void set_guid(const String& guid);
	String get_guid() const;

	void set_parameter_description(const ::FMOD_STUDIO_PARAMETER_DESCRIPTION& parameter_description);
	void get_parameter_description(::FMOD_STUDIO_PARAMETER_DESCRIPTION& out_description) const;
};

class FMOD_STUDIO_USER_PROPERTY : public RefCounted
{
	GDCLASS(FMOD_STUDIO_USER_PROPERTY, RefCounted);

protected:
	static void _bind_methods();

private:
	String name{};
	FMOD_STUDIO_USER_PROPERTY_TYPE type{};
	int intvalue{};
	bool boolvalue{};
	float floatvalue{};
	String stringvalue{};

public:
	void set_name(const String& name);
	String get_name() const;
	void set_type(int type);
	int get_type() const;
	void set_intvalue(int intvalue);
	int get_intvalue() const;
	void set_boolvalue(bool boolvalue);
	bool get_boolvalue() const;
	void set_floatvalue(float floatvalue);
	float get_floatvalue() const;
	void set_stringvalue(const String& stringvalue);
	String get_stringvalue() const;

	void set_user_property(const ::FMOD_STUDIO_USER_PROPERTY& user_property);
	void get_user_property(::FMOD_STUDIO_USER_PROPERTY& out_user_property) const;
};

class FMOD_3D_ATTRIBUTES : public RefCounted
{
	GDCLASS(FMOD_3D_ATTRIBUTES, RefCounted);

protected:
	static void _bind_methods();

private:
	Vector3 position{};
	Vector3 velocity{};
	Vector3 forward{};
	Vector3 up{};

public:
	void set_position(const Vector3& position);
	Vector3 get_position() const;
	void set_velocity(const Vector3& velocity);
	Vector3 get_velocity() const;
	void set_forward(const Vector3& forward);
	Vector3 get_forward() const;
	void set_up(const Vector3& up);
	Vector3 get_up() const;

	void set_3d_attributes(const ::FMOD_3D_ATTRIBUTES& attributes);
	void get_3d_attributes(::FMOD_3D_ATTRIBUTES& out_attributes) const;
};

class FMOD_STUDIO_BUFFER_INFO : public RefCounted
{
	GDCLASS(FMOD_STUDIO_BUFFER_INFO, RefCounted);

protected:
	static void _bind_methods();

private:
	int currentusage{};
	int peakusage{};
	int capacity{};
	int stallcount{};
	float stalltime{};

public:
	void set_currentusage(int currentusage);
	int get_currentusage() const;
	void set_peakusage(int peakusage);
	int get_peakusage() const;
	void set_capacity(int capacity);
	int get_capacity() const;
	void set_stallcount(int stallcount);
	int get_stallcount() const;
	void set_stalltime(float stalltime);
	float get_stalltime() const;

	void set_buffer_info(const ::FMOD_STUDIO_BUFFER_INFO& buffer_info);
	void get_buffer_info(::FMOD_STUDIO_BUFFER_INFO& out_buffer_info) const;
};

class FMOD_STUDIO_BUFFER_USAGE : public RefCounted
{
	GDCLASS(FMOD_STUDIO_BUFFER_USAGE, RefCounted);

protected:
	static void _bind_methods();

private:
	Ref<FMOD_STUDIO_BUFFER_INFO> studiocommandqueue;
	Ref<FMOD_STUDIO_BUFFER_INFO> studiohandle;

public:
	void set_studio_command_queue(const Ref<FMOD_STUDIO_BUFFER_INFO>& studiocommandqueue);
	Ref<FMOD_STUDIO_BUFFER_INFO> get_studio_command_queue() const;
	void set_studio_handle(const Ref<FMOD_STUDIO_BUFFER_INFO>& studiohandle);
	Ref<FMOD_STUDIO_BUFFER_INFO> get_studio_handle() const;

	void set_buffer_usage(const ::FMOD_STUDIO_BUFFER_USAGE& buffer_usage);
	void get_buffer_usage(::FMOD_STUDIO_BUFFER_USAGE& out_buffer_usage) const;
};

class FMOD_STUDIO_CPU_USAGE : public RefCounted
{
	GDCLASS(FMOD_STUDIO_CPU_USAGE, RefCounted);

protected:
	static void _bind_methods();

private:
	float update{};

public:
	void set_update(float update);
	float get_update() const;

	void set_cpu_usage(const ::FMOD_STUDIO_CPU_USAGE& cpu_usage);
	void get_cpu_usage(::FMOD_STUDIO_CPU_USAGE& out_cpu_usage) const;
};

class FMOD_CPU_USAGE : public RefCounted
{
	GDCLASS(FMOD_CPU_USAGE, RefCounted);

protected:
	static void _bind_methods();

private:
	float dsp{};
	float stream{};
	float geometry{};
	float update{};
	float convolution1{};
	float convolution2{};

public:
	void set_dsp(float dsp);
	float get_dsp() const;
	void set_stream(float stream);
	float get_stream() const;
	void set_geometry(float geometry);
	float get_geometry() const;
	void set_update(float update);
	float get_update() const;
	void set_convolution_1(float convolution1);
	float get_convolution_1() const;
	void set_convolution_2(float convolution2);
	float get_convolution_2() const;

	void set_cpu_usage(const ::FMOD_CPU_USAGE& cpu_usage);
	void get_cpu_usage(::FMOD_CPU_USAGE& out_memory_usage) const;
};

class FMOD_STUDIO_MEMORY_USAGE : public RefCounted
{
	GDCLASS(FMOD_STUDIO_MEMORY_USAGE, RefCounted);

protected:
	static void _bind_methods();

private:
	int exclusive{};
	int inclusive{};
	int sampledata{};

public:
	void set_exclusive(int exclusive);
	int get_exclusive() const;
	void set_inclusive(int inclusive);
	int get_inclusive() const;
	void set_sampledata(int sampledata);
	int get_sampledata() const;

	void set_memory_usage(const ::FMOD_STUDIO_MEMORY_USAGE& memory_usage);
	void get_memory_usage(::FMOD_STUDIO_MEMORY_USAGE& out_memory_usage) const;
};

} //namespace FmodTypes
#endif // FMOD_TYPES_H