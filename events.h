#pragma once

#include "stdafx.h"
#include "prefs_instance.h"
#include "event_enums.h"

#include <map>
#include <vector>

#include <math.h>

static std::map<int, std::vector<pfc::string8>> _evtostr = {
	{EVENT_PLAYBACK_STARTING, {"Playback start","Trigger when playback starts"}},
	{EVENT_PLAYBACK_NEW_TRACK, {"New track","Trigger when new track starts playing"}},
	{EVENT_PLAYBACK_STOP, {"Playback stop","Trigger when playback stops"}},
	{EVENT_PLAYBACK_SEEK, {"Playback seek","Trigger on seek"}},
	{EVENT_PLAYBACK_PAUSE, {"Playback pause","Triger when playback is paused"}},
	{EVENT_PLAYBACK_EDITED, {"Playback edited",""}},
	{EVENT_PLAYBACK_DINFO, {"Playback dynamic info",""}},
	{EVENT_PLAYBACK_DINFO_TRACK, {"Playback dynamic info track",""}},
	{EVENT_PLAYBACK_TIME, {"Playback time","Trigger every second"}},
	{EVENT_VOLCHANGE, {"Volume change",""}},
};


class titleformat_hook_test : public titleformat_hook {
public:
	titleformat_hook_test(metadb_handle_ptr p_track) : track(p_track) {}

	bool process_field(titleformat_text_out * p_out, const char * p_name, t_size p_name_length, bool & p_found_flag) {
		if ( pfc::stricmp_ascii_ex(p_name, p_name_length, "isplaying", ~0) == 0) {
			if (m_playback_control->is_playing()) {
				p_out->write(titleformat_inputtypes::unknown, "1");
				p_found_flag = true; return true;
			}
		}
		else if (pfc::stricmp_ascii_ex(p_name, p_name_length, "ispaused", ~0) == 0) {
			if (m_playback_control->is_paused()) {
				p_out->write(titleformat_inputtypes::unknown, "1");
				p_found_flag = true; return true;
			}
		}
		else if (pfc::stricmp_ascii_ex(p_name, p_name_length, "playback_time", ~0) == 0) {
			pfc::string_formatter s;
			t_int64 total = (t_int64)m_playback_control->playback_get_position(), seconds, hours, minutes;
			minutes = total / 60;
			seconds = total % 60;
			hours = minutes / 60;
			minutes = minutes % 60;
			if (hours > 0) 
				s << hours << ":";
			if (minutes > 0)
				s << minutes << ":";
			s << seconds;

			p_out->write(titleformat_inputtypes::unknown, s);
			p_found_flag = true; return true;
		}				
		else if (pfc::stricmp_ascii_ex(p_name, p_name_length, "title", ~0) == 0 ||
			pfc::stricmp_ascii_ex(p_name, p_name_length, "artist", ~0) == 0 ||
			pfc::stricmp_ascii_ex(p_name, p_name_length, "album", ~0) == 0) {
			if (!m_playback_control->is_playing()) {
				p_out->write(titleformat_inputtypes::unknown, "");
				p_found_flag = true; return true;
			}
		}
		else if (pfc::stricmp_ascii_ex(p_name, p_name_length, "volume", ~0) == 0) {
			pfc::string8 s;
			s << m_playback_control->get_volume();
			p_out->write(titleformat_inputtypes::unknown, s );
			p_found_flag = true; return true;
		}
		/*
		else if (pfc::stricmp_ascii_ex(p_name, p_name_length, "test", ~0) == 0) {
			if (m_playback_control->is_playing()) {
				//track->metadb_lock();
				file_info_impl fi;
				track->get_info(fi);
				p_out->write_int(titleformat_inputtypes::unknown,track->codec );
				p_found_flag = true; return true;
			}
		}
		*/

		p_found_flag = false; return false;
		
	}

	bool process_function(titleformat_text_out * p_out, const char * p_name, t_size p_name_length, titleformat_hook_function_params * p_params, bool & p_found_flag) { return false; }

private:
	static_api_ptr_t<playback_control> m_playback_control;
	metadb_handle_ptr track;
};

typedef struct event_item_s {
	instance_item item;
	titleformat_object::ptr m_script;

	event_item_s(instance_item *p_item) {
		item = *p_item;
		static_api_ptr_t<titleformat_compiler>()->compile_safe_ex(m_script, item.format_string);
	}
	event_item_s() {}

} event_item;

class CEventsBase : private play_callback_impl_base {
public:
	pfc::string8 format_title(titleformat_object::ptr &m_script) {
		pfc::string8 state;
	
		if (last_track.is_empty()) {
			metadb_handle_ptr t;
			if (m_playback_control->get_now_playing(t)) {
				last_track = t;
			}
		}

		if (!last_track.is_empty()) {
			m_playback_control->playback_format_title_ex(last_track, &titleformat_hook_test(last_track), state, m_script, NULL, playback_control::display_level_all);
		}
		else {
			m_playback_control->playback_format_title(NULL, state, m_script, NULL, playback_control::display_level_all);
		}

		return state;
	}

	static_api_ptr_t<playback_control> m_playback_control;
	metadb_handle_ptr last_track{ 0 };
private:
	virtual void on_playback_starting(play_control::t_track_command p_command, bool p_paused) {
		event_pre(EVENT_PLAYBACK_STARTING);
	};
	virtual void on_playback_new_track(metadb_handle_ptr p_track) {
		last_track = p_track;
		event_pre(EVENT_PLAYBACK_NEW_TRACK);
		
	}
	virtual void on_playback_stop(play_control::t_stop_reason p_reason) {
		if (p_reason != play_control::stop_reason_starting_another) {
			event_pre(EVENT_PLAYBACK_STOP);
		}
	}
	virtual void on_playback_pause(bool p_state) {
		event_pre(EVENT_PLAYBACK_PAUSE);
	}
	virtual void on_playback_seek(double p_time) {
		event_pre(EVENT_PLAYBACK_SEEK);
	}
	virtual void on_playback_edited(metadb_handle_ptr p_track) {
		event_pre(EVENT_PLAYBACK_EDITED);
	}
	virtual void on_playback_dynamic_info(const file_info & p_info) {
		event_pre(EVENT_PLAYBACK_DINFO);
	}
	virtual void on_playback_dynamic_info_track(const file_info & p_info) {
		event_pre(EVENT_PLAYBACK_DINFO_TRACK);
	}
	virtual void on_playback_time(double p_time) {
		event_pre(EVENT_PLAYBACK_TIME);
	}
	virtual void on_volume_change(float p_new_val) {
		event_pre(EVENT_VOLCHANGE);
	}

	void event_pre(uint32_t event)
	{
		event_update(event);
	}

	virtual void event_update(uint32_t event) {}

};

class CEvents : private CEventsBase {
public:

	event_item UpdateInstance(instance_item *item);
	void RemoveInstance(pfc::string8 name);
private:

	virtual void event_update(uint32_t event);

	std::map<pfc::string8, event_item> m_instancemap[EVENT_COUNT];
	
};

class IEvents {
public:

	static void Initialize() {
		if (m_Events == nullptr) {
			m_Events = new CEvents();
		}
	}

	static void Destroy() {
		if (m_Events != nullptr) {
			delete m_Events;
		}
	}

	static event_item UpdateInstance(instance_item *item) {
		return m_Events->UpdateInstance(item);
	}

	static void RemoveInstance(pfc::string8 name) {
		m_Events->RemoveInstance(name);
	}

	static pfc::string8 EventToString(int ev)
	{
		if (_evtostr.count(ev) > 0) {
			return _evtostr[ev][0];
		}
		else {
			return "EVENT_UNKNOWN";
		}
	}

	static pfc::string8 GetEventDescription(int ev)
	{
		if (_evtostr.count(ev) > 0) {
			return _evtostr[ev][1];
		}
		else {
			return "EVENT_UNKNOWN";
		}
	}

private:
	static CEvents *m_Events;
};