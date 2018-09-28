#include "stdafx.h"

class npa_initquit : public initquit {
public:
	void on_init() {
		IEvents::Initialize();
		IWriter::Initialize();
		for (t_size i = 0; i < IConfig::Count(); i++) {
			IEvents::UpdateInstance(IConfig::Get(i));
		}
		console::info(COMMON_NAME " - plugin initialized");
	}
	void on_quit() {
		IEvents::Destroy();
		IWriter::Destroy();

		if (IConfig::IsEnabled()) {
			for (t_size i = 0; i < IConfig::Count(); i++) {
				const instance_item &item = IConfig::Get(i);
				if (item.on_exit && item.write_to_file) {
					CWriter::Write(write_job(item.filename, item.on_exit_str, item.encoding));
				}
			}
		}
	}
private:
};

static initquit_factory_t<npa_initquit> g_npainitquit_factory;
