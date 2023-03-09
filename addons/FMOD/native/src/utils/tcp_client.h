#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <godot_cpp/classes/stream_peer_tcp.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>

class FmodTCPClient : public StreamPeerTCP
{
	GDCLASS(FmodTCPClient, StreamPeerTCP);

protected:
	static void _bind_methods()
	{
		ClassDB::bind_method(D_METHOD("on_connection_timeout"), &FmodTCPClient::on_connection_timeout);
		ADD_SIGNAL(MethodInfo("connection_finished"));
	};

private:
	const String HOST = "127.0.0.1";
	const int PORT = 3663;
	Timer* timer;

public:
	bool client_is_connected = false;

	void connect_client(Node* node)
	{
		if (get_status() != Status::STATUS_NONE)
		{
			return;
		}

		if (!node)
		{
			return;
		}

		if (connect_to_host(HOST, PORT) == Error::OK)
		{
			timer = memnew(Timer);
			timer->connect("timeout", Callable(this, "on_connection_timeout"));
			timer->set_wait_time(0.3);
			timer->set_autostart(true);

			node->add_child(timer);
			timer->set_owner(node);
		}
	}

	void disconnect_client()
	{
		disconnect_from_host();
	}

	void on_connection_timeout()
	{
		poll();
		if (get_status() == Status::STATUS_CONNECTED)
		{
			client_is_connected = true;
		}

		if (client_is_connected)
		{
			String out_result;
			Error result = get_command("", out_result);
			if (result == Error::FAILED)
			{
				disconnect_from_host();
				client_is_connected = false;
			}
		}
		else
		{
			disconnect_from_host();
		}

		emit_signal("connection_finished");

		if (timer)
		{
			if (!timer->is_queued_for_deletion())
			{
				timer->queue_free();
			}
		}
	}

	Error get_command(const String& command, String& result)
	{
		if (put_data(command.to_utf8_buffer()) != Error::OK)
		{
			return Error::FAILED;
		}

		int32_t bytes = get_available_bytes();

		Array bytes_array;
		if (bytes >= 0)
		{
			Array result_array = get_data(get_available_bytes());
			bytes_array = result_array;
			if (bytes_array.size() == 0)
			{
				return Error::FAILED;
			}
		}
		else
		{
			return Error::FAILED;
		}
		int err = bytes_array[0];
		if (err != Error::OK)
		{
			return Error::FAILED;
		}

		PackedByteArray data_array = bytes_array[1];

		if (data_array.is_empty())
		{
			while (data_array.is_empty())
			{
				bytes_array = get_data(get_available_bytes());
				data_array = bytes_array[1];
			}
		}

		while (data_array[data_array.size() - 1] != 0)
		{
			bytes_array = get_data(get_available_bytes());
			data_array = bytes_array[1];
		}

		if (data_array.size() == 0)
		{
			return Error::FAILED;
		}

		if (data_array[data_array.size() - 1] == 0)
		{
			String message = data_array.get_string_from_utf8().strip_escapes();
			message = message.replace("out(): ", "");
			result = message;
			return Error::OK;
		}
		else
		{
			return Error::FAILED;
		}
	}
};
#endif // TCP_CLIENT_H