#pragma once

#include "event_enums.h"
#include "writer_flags.h"

#include "json/json.h"

typedef struct instance_item_s {
	pfc::string8 name;
	pfc::string8 filename;
	pfc::string8 format_string;
	bool write_to_file = false;
	bool log_mode = false;
	bool enable_delay = false;
	uint32_t delay = 0;
	bool events[EVENT_COUNT];
	bool on_exit = false;
	pfc::string8 on_exit_str;
	uint8_t encoding = ENCODING_UTF8;
	bool changes_only = false;
	bool clipboard = false;
	bool _breserved2 = false;
	bool _breserved3 = false;
	bool _breserved4 = false;
	uint32_t _u32reserved1 = 0;
	uint32_t _u32reserved2 = 0;
	uint32_t _u32reserved3 = 0;
	uint32_t _u32reserved4 = 0;
	pfc::string8 _sreserved1;
	pfc::string8 _sreserved2;
	pfc::string8 _sreserved3;
	pfc::string8 _sreserved4;

	instance_item_s(const char *p_name) : name(p_name) {
		reset_events();
	}
	instance_item_s(Json::Value &p_data) {
		if (!p_data["name"].isString()) {
			throw std::invalid_argument("'name' expected type string");
		}
		name = p_data["name"].asString().c_str();
		if (!p_data["filename"].isString()) {
			throw std::invalid_argument("'filename' expected type string");
		}
		filename = p_data["filename"].asString().c_str();
		if (!p_data["format_string"].isString()) {
			throw std::invalid_argument("'format_string' expected type string");
		}
		format_string = p_data["format_string"].asString().c_str();
		if (!p_data["write_to_file"].isBool()) {
			throw std::invalid_argument("'write_to_file' expected type bool");
		}
		write_to_file = p_data["write_to_file"].asBool();
		if (!p_data["log_mode"].isBool()) {
			throw std::invalid_argument("'log_mode' expected type bool");
		}
		log_mode = p_data["log_mode"].asBool();
		if (!p_data["enable_delay"].isBool()) {
			throw std::invalid_argument("'enable_delay' expected type bool");
		}
		enable_delay = p_data["enable_delay"].asBool();
		if (!p_data["delay"].isUInt()) {
			throw std::invalid_argument("'delay' expected typen uint");
		}
		delay = (uint8_t)p_data["delay"].asUInt();
		const Json::Value p_events = p_data["events"];
		if (p_events.size() != EVENT_COUNT) {
			throw std::invalid_argument("Incorrect number of events");
		}
		if (!p_events.isArray()) {
			throw std::invalid_argument("'events' expected typen array");
		}
		for (Json::ArrayIndex i = 0; i < p_events.size(); i++) {
			if (!p_events[i].isBool()) {
				throw std::invalid_argument("'events' member expected type bool");
			}
			events[i] = p_events[i].asBool();
		}
		if (!p_data["on_exit"].isBool()) {
			throw std::invalid_argument("'on_exit' expected type bool");
		}
		on_exit = p_data["on_exit"].asBool();
		if (!p_data["on_exit_str"].isString()) {
			throw std::invalid_argument("'on_exit_str' expected type string");
		}
		on_exit_str = p_data["on_exit_str"].asString().c_str();
		if (!p_data["encoding"].isUInt()) {
			throw std::invalid_argument("'encoding' expected typen uint");
		}
		encoding = (uint8_t)p_data["encoding"].asUInt();
		if (!p_data["changes_only"].isBool()) {
			throw std::invalid_argument("'changes_only' expected type bool");
		}
		changes_only = p_data["changes_only"].asBool();
		if (!p_data["clipboard"].isBool()) {
			throw std::invalid_argument("'clipboard' expected type bool");
		}
		clipboard = p_data["clipboard"].asBool();
	}
	instance_item_s(const struct instance_item_s *data) {
		memcpy((void*)this, (const void *)data, sizeof(struct instance_item_s));
	}
	instance_item_s() {
		reset_events();
	}
	Json::Value to_json() {
		Json::Value out;
		out["name"] = name.c_str();
		out["filename"] = filename.c_str();
		out["format_string"] = format_string.c_str();
		out["write_to_file"] = write_to_file;
		out["log_mode"] = log_mode;
		out["enable_delay"] = enable_delay;
		out["delay"] = delay;
		for (uint32_t i = 0; i < EVENT_COUNT; i++) {
			out["events"][i] = events[i];
		}
		out["on_exit"] = on_exit;
		out["on_exit_str"] = on_exit_str.c_str();
		out["encoding"] = encoding;
		out["changes_only"] = changes_only;
		out["clipboard"] = clipboard;
		return out;
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
	bool operator==(const instance_item_s& b) { return b.name == name; }

} instance_item;

FB2K_STREAM_READER_OVERLOAD(instance_item) {
	stream >> value.name >> value.filename >> value.format_string >>
		value.write_to_file >> value.log_mode >> value.enable_delay >>
		value.delay >> value.events >> value.on_exit >> value.on_exit_str >>
		value.encoding >> value.changes_only

		>> value.clipboard >> value._breserved2 >> value._breserved3 >> value._breserved4
		>> value._u32reserved1 >> value._u32reserved2 >> value._u32reserved3 >> value._u32reserved4
		>> value._sreserved1 >> value._sreserved2 >> value._sreserved3 >> value._sreserved4;
	return stream;
}

FB2K_STREAM_WRITER_OVERLOAD(instance_item) {
	stream << value.name << value.filename << value.format_string <<
		value.write_to_file << value.log_mode << value.enable_delay <<
		value.delay << value.events << value.on_exit << value.on_exit_str <<
		value.encoding << value.changes_only

		<< value.clipboard << value._breserved2 << value._breserved3 << value._breserved4
		<< value._u32reserved1 << value._u32reserved2 << value._u32reserved3 << value._u32reserved4
		<< value._sreserved1 << value._sreserved2 << value._sreserved3 << value._sreserved4;
	return stream;
}
