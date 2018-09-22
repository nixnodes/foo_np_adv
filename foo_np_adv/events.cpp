#include "stdafx.h"
#include "events.h"
#include "writer.h"

CEvents *IEvents::m_Events = nullptr;

event_item CEvents::UpdateInstance(instance_item *item)
{
	RemoveInstance(item->name);

	event_item evitem(item);

	for (uint32_t i = 0; i < EVENT_COUNT; i++) {
		if (item->events[i] == true) {
			m_instancemap[i][item->name] = evitem;
		}
	}

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
}

void CEvents::event_update(uint32_t event) {
	for (const auto &p : m_instancemap[event]) {
		titleformat_object::ptr m_script = p.second.m_script;
		pfc::string8 state = format_title(m_script);

		if (p.second.item.write_to_file) {
			uint32_t flags;

			if (p.second.item.log_mode) {
				flags = F_WRITER_APPEND;
				if (state.length() == 0) {
					continue;
				}
				state << "\n";
			}
			else {
				flags = 0;
			}

			write_job j(p.second.item.filename, state, flags);
			if (p.second.item.enable_delay) {
				IWriter::WriteAsync(&j, p.second.item.delay);
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
	else {
		TR_RETURN(false)
	}
}

bool titleformat_hook_glob::process_function(titleformat_text_out * p_out, const char * p_name, t_size p_name_length, titleformat_hook_function_params * p_params, bool & p_found_flag)
{
	if (pfc::strcmp_ex(p_name, p_name_length, "strftime", pfc::infinite_size) == 0) {
		if (p_params->get_param_count() == 0) {
			p_found_flag = false; return true;
		}

		const char *field_name = 0;
		t_size field_name_length = 0;
		p_params->get_param(0, field_name, field_name_length);

		pfc::string8 fmt;
		const char *p = field_name;

		while (*p) {
			if (*p == '%') {
				p++;
				if (*p == '\0') {
					break;
				}
				if (CDateTime::is_fs_allowed(*p)) {
					fmt << "%" << std::string(1, *p).c_str();
				}
			}
			else {
				fmt << std::string(1, *p).c_str();
			}
			p++;
		}

		p_out->write(titleformat_inputtypes::unknown, CDateTime().fmt(fmt));
		TR_RETURN(true)
	}
	else {
		TR_RETURN(false)
	}
}