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
		pfc::string_formatter str;

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
};