#ifndef FMOD_IO_H
#define FMOD_IO_H

#include "fmod_common.h"
#include <godot_cpp/classes/file_access.hpp>

namespace godot
{
class FMODGodotBlockingIO
{
	struct FileHandle
	{
		Ref<FileAccess> file;
	};

public:
	static FMOD_RESULT F_CALLBACK file_open(const char* name, unsigned int* filesize, void** handle, void* userdata)
	{
		String file_path = name;

		FileHandle* const file_handle = memnew(FileHandle);
		Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::ModeFlags::READ);
		file_handle->file = file;

		*handle = static_cast<void*>(file_handle);

		if (FileAccess::get_open_error() != Error::OK)
		{
			*filesize = 0;
			memfree(file_handle);
			return FMOD_ERR_FILE_NOTFOUND;
		}

		*filesize = file->get_length();

		return FMOD_OK;
	}

	static FMOD_RESULT F_CALLBACK file_close(void* handle, void* userdata)
	{
		FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_INVALID_PARAM;

		if (!handle)
		{
			return result;
		}

		FileHandle* const file_handle = static_cast<FileHandle*>(handle);
		file_handle->file->close();
		memfree(file_handle);

		result = FMOD_OK;
		return result;
	}

	static FMOD_RESULT F_CALLBACK file_read(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread,
			void* userdata)
	{
		FileHandle* const file_handle = static_cast<FileHandle*>(handle);
		Ref<FileAccess> file = file_handle->file;

		PackedByteArray file_buffer = file->get_buffer(sizebytes);
		int64_t size = file_buffer.size();
		const uint8_t* data = file_buffer.ptr();

		memcpy(buffer, data, size * sizeof(uint8_t));

		*bytesread = size;
		if (*bytesread == 0 || file->eof_reached())
		{
			return FMOD_ERR_FILE_EOF;
		}

		return FMOD_OK;
	}

	static FMOD_RESULT F_CALLBACK file_seek(void* handle, unsigned int pos, void* userdata)
	{
		FileHandle* const file_handle = static_cast<FileHandle*>(handle);
		Ref<FileAccess> file = file_handle->file;

		const uint64_t file_read_position = file->get_position();
		const uint64_t wanted_file_read_position = pos;

		if (file_read_position != wanted_file_read_position)
		{
			file->seek(wanted_file_read_position);
		}

		return FMOD_OK;
	}
};
} // namespace godot

#endif // FMOD_IO_H