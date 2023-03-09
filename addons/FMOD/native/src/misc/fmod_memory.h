#ifndef FMOD_MEMORY_H
#define FMOD_MEMORY_H

#include "fmod_common.h"
#include <godot_cpp/core/memory.hpp>

namespace godot
{
class FMODGodotMemory
{
public:
	static void* F_CALLBACK user_alloc(unsigned int size, FMOD_MEMORY_TYPE type, const char* sourcestr)
	{
		return Memory::alloc_static(size);
	}

	static void* F_CALLBACK user_realloc(void* ptr, unsigned int size, FMOD_MEMORY_TYPE type, const char* sourcestr)
	{
		return Memory::realloc_static(ptr, size);
	}

	static void F_CALLBACK user_free(void* ptr, FMOD_MEMORY_TYPE type, const char* sourcestr)
	{
		return Memory::free_static(ptr);
	}
};
} // namespace godot
#endif // FMOD_MEMORY_H