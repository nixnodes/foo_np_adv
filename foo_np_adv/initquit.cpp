#include "stdafx.h"

class npa_initquit : public initquit {
public:
	void on_init() {
		console::info(COMMON_NAME " - plugin initialized");
	}
	void on_quit() {
		IEvents::Destroy();
		IWriter::Destroy();

		for (t_size i = 0; i < IConfig::Count(); i++) {
			const instance_item item = IConfig::Get(i);
			if (item.on_exit && item.write_to_file) {
				CWriter::Write(&write_job(item.filename, item.on_exit_str, item.encoding));
			}
		}
	}
private:
};

static initquit_factory_t<npa_initquit> g_npainitquit_factory;

class InitHandler : public init_stage_callback {
public:
	void on_init_stage(t_uint32 stage) {
		switch (stage) {
		case init_stages::before_config_read:;
			IEvents::Initialize();
			IWriter::Initialize();
		case init_stages::after_config_read:;
			for (t_size i = 0; i < IConfig::Count(); i++) {
				IEvents::UpdateInstance(&IConfig::Get(i));
			}
		}
	}
};
static service_factory_single_t<InitHandler> initHandler;
