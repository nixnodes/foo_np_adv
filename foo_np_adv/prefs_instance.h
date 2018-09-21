#pragma once

#include "stdafx.h"
#include "event_enums.h"

typedef struct instance_item_s {
	pfc::string8 name;
	pfc::string8 filename;
	pfc::string8 format_string;
	bool write_to_file;
	bool log_mode;
	bool enable_delay;
	uint32_t delay = 0;
	bool events[EVENT_COUNT];

	instance_item_s(const char * p_name, const char * p_filename, const char * p_format_string,
		const bool p_write_to_file, const bool p_log_mode, const bool p_enable_delay, const uint32_t p_delay, 
		const bool(&p_events)[EVENT_COUNT])
		: name(p_name), filename(p_filename), format_string(p_format_string), log_mode(p_log_mode),
		enable_delay(p_enable_delay), delay(p_delay)
	{
		for (uint32_t i = 0; i < EVENT_COUNT; i++) {
			events[i] = p_events[i];
			pfc::string_formatter str;
			console::print(str << p_events[i]);
		}
	}
	instance_item_s(const char * p_name, const char * p_filename, const char * p_format_string, const bool p_write_to_file, 
		const bool p_log_mode, const bool p_enable_delay, const uint32_t p_delay, const std::initializer_list <uint32_t> p_events)
		: name(p_name), filename(p_filename), format_string(p_format_string), log_mode(p_log_mode), 
		enable_delay(p_enable_delay), delay(p_delay), write_to_file(p_write_to_file)
	{
		reset_events();

		for (auto f : p_events) {
			if (f >= EVENT_COUNT) {
				pfc::string_formatter str;
				console::print(str << "WARNING: got invalid event during 'instance_item_s' initialization: " << f);
			}
			else {
				events[f] = true;
			}
		}
	}
	instance_item_s(const struct instance_item_s *data) {
		memcpy((void*)this, (const void *)data, sizeof(struct instance_item_s));
	}
	instance_item_s() {
		reset_events();
	}
	void reset_events()
	{
		for (uint32_t i = 0; i < EVENT_COUNT; i++) {
			events[i] = false;
		}
		events[EVENT_PLAYBACK_STOP] = true;
		events[EVENT_PLAYBACK_PAUSE] = true;
		events[EVENT_PLAYBACK_NEW_TRACK] = true;
	}

} instance_item;


FB2K_STREAM_READER_OVERLOAD(instance_item) {
	stream >> value.name >> value.filename >> value.format_string >>
		value.write_to_file >> value.log_mode >> value.enable_delay >> 
		value.delay >> value.events;
	return stream;
}

FB2K_STREAM_WRITER_OVERLOAD(instance_item) {
	stream << value.name << value.filename << value.format_string <<
		value.write_to_file << value.log_mode << value.enable_delay << 
		value.delay << value.events;
	return stream;
}
