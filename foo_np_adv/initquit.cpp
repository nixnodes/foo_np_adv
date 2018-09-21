#include "stdafx.h"
#include "events.h"
#include "writer.h"
#include "preferences.h"

class npa_initquit : public initquit {
public:
	void on_init() {
		console::print(COMMON_NAME " - plugin initialized");
	}
	void on_quit() {
		IEvents::Destroy();
		IWriter::Destroy();
	}
private:
};

static initquit_factory_t<npa_initquit> g_npainitquit_factory;

class InitHandler : public init_stage_callback {
public:
	void on_init_stage(t_uint32 stage) {
		if (stage == init_stages::before_config_read) {
			IEvents::Initialize();
			IWriter::Initialize();
		}
		else if (stage == init_stages::after_config_read) {
			for (t_size i = 0; i < IConfig::Count(); i++) {
				instance_item item = IConfig::Get(i);
				IEvents::UpdateInstance(&item);
			}
		}
	}
};
static service_factory_single_t<InitHandler> initHandler;
