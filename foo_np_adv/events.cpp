#include "stdafx.h"

CEvents *IEvents::m_Events = nullptr;
static std::map<pfc::string8, instance_state> c_instst;

event_item CEvents::UpdateInstance(const instance_item *item)
{
	RemoveInstance(item->name);

	event_item evitem(item);

	for (uint32_t i = 0; i < EVENT_COUNT; i++) {
		if (item->events[i] == true) {
			m_instancemap[i][item->name] = evitem;
		}
	}

	c_instst[item->name] = instance_state();

	return evitem;
}

void CEvents::RemoveInstance(pfc::string8 name) {
	for (uint32_t i = 0; i < EVENT_COUNT; i++) {
		for (const auto &p : m_instancemap[i]) {
			if (name == p.first) {
				m_instancemap[i].erase(p.first);
				break;
			}
		}
	}

	if (c_instst.count(name) > 0) {
		c_instst.erase(name);
	}
}

void CEvents::Clear() {
	for (uint32_t i = 0; i < EVENT_COUNT; i++) {
		m_instancemap[i].clear();
	}
}

void CEvents::event_update(uint32_t event) {
	for (const auto &p : m_instancemap[event]) {
		event_item &ievent = (event_item&)p.second;
		titleformat_object::ptr m_script = ievent.m_script;
		pfc::string8 state = format_title(m_script);

		const instance_item &item = ievent.item;

		if (item.clipboard) {
			ClipboardHelper::OpenScope hclip;
			hclip.Open(NULL);
			ClipboardHelper::SetString(state);
		}
		else if (item.write_to_file) {
			instance_state &st = c_instst[item.name];
			if (item.changes_only) {
				if (st.last_state == state &&
					st.ponce != false) {
					continue;
				}
			}
			st.ponce = true;
			st.last_state = state;

			uint32_t flags;

			if (item.log_mode) {
				flags = F_WRITER_APPEND;
				if (state.length() == 0) {
					continue;
				}
				state << "\n";
			}
			else {
				flags = 0;
			}

			write_job j(item.filename, state, item.encoding, flags);
			if (item.enable_delay) {
				IWriter::WriteAsync(&j, item.delay);
			}
			else {
				IWriter::Write(&j);
			}
		}
	}
}

bool titleformat_hook_glob::process_field(titleformat_text_out * p_out, const char * p_name, t_size p_name_length, bool & p_found_flag)
{
	if (pfc::stricmp_ascii_ex(p_name, p_name_length, "volume", pfc::infinite_size) == 0) {
		pfc::string8 s;
		s << m_playback_control->get_volume();
		p_out->write(titleformat_inputtypes::unknown, s);
		TR_RETURN(true)
	}
	else if (pfc::stricmp_ascii_ex(p_name, p_name_length, "datetime", pfc::infinite_size) == 0) {
		p_out->write(titleformat_inputtypes::unknown, CDateTime().datetime());
		TR_RETURN(true)
	}
	else if (pfc::stricmp_ascii_ex(p_name, p_name_length, "time", pfc::infinite_size) == 0) {
		p_out->write(titleformat_inputtypes::unknown, CDateTime().time());
		TR_RETURN(true)
	}
	else if (pfc::stricmp_ascii_ex(p_name, p_name_length, "active_playlist_name", pfc::infinite_size) == 0) {
		pfc::string8 name;
		if (!m_playlist_manager->activeplaylist_get_name(name)) {
			TR_RETURN2(false, true)
		}
		p_out->write(titleformat_inputtypes::unknown, name);
		TR_RETURN(true)
	}
	else if (pfc::stricmp_ascii_ex(p_name, p_name_length, "playing_playlist_name", pfc::infinite_size) == 0) {
		pfc::string8 name;
		t_size pindex = m_playlist_manager->get_playing_playlist();
		if (pindex == pfc::infinite_size ||
			!m_playlist_manager->playlist_get_name(pindex, name)) {
			TR_RETURN2(false, true)
		}
		p_out->write(titleformat_inputtypes::unknown, name);
		TR_RETURN(true)
	}
	else {
		TR_RETURN(false)
	}
}

bool titleformat_hook_glob::process_function(titleformat_text_out * p_out, const char * p_name, t_size p_name_length, titleformat_hook_function_params * p_params, bool & p_found_flag)
{
	if (pfc::strcmp_ex(p_name, p_name_length, "strftime", pfc::infinite_size) == 0) {
		if (p_params->get_param_count() == 0) {
			TR_RETURN2(false, true)
		}

		const char *field_name = 0;
		t_size field_name_length = 0;
		p_params->get_param(0, field_name, field_name_length);

		pfc::string8 fmt;

		// sanitize format string to avoid segfaults in C strftime
		for (t_size i = 0; i < field_name_length; i++) {
			char c = field_name[i];
			if (c == '%') {
				i++;
				if (i == field_name_length) {
					break;
				}
				c = field_name[i];
				if (CDateTime::is_fs_allowed(c)) {
					fmt << "%" << std::string(1, c).c_str();
				}
			}
			else {
				fmt << std::string(1, c).c_str();
			}
		}
		p_out->write(titleformat_inputtypes::unknown, CDateTime().fmt(fmt));
		TR_RETURN(true)
	}
	else {
		TR_RETURN(false)
	}
}