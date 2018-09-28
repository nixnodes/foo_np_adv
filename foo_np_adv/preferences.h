#pragma once

#include "resource.h"
#include "events.h"
#include "prefs_instance.h"

#include <vector>

typedef struct fn_filter_s {
	pfc::string8 ext, desc;
	fn_filter_s(const char *p_ext, const char *p_desc) : ext(p_ext), desc(p_desc) {}
}fn_filter;

class CNPAPreferences : public CDialogImpl<CNPAPreferences>,
	public preferences_page_instance,
	private CEventsBase {
public:

	CNPAPreferences(preferences_page_callback::ptr callback)
		: m_callback(callback) {
		ResetFlags();
	}

	enum {
		IDD = IDD_NPAPREFERENCES
	};

	t_uint32 get_state();
	void apply();
	void reset();

	BEGIN_MSG_MAP(CNPAPreferences)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_FILENAME, EN_CHANGE, OnChangeDefault)
		COMMAND_HANDLER_EX(IDC_COMBO1, CBN_EDITCHANGE, OnComboTextChange)
		COMMAND_HANDLER_EX(IDC_COMBO1, CBN_SELCHANGE, OnComboInstanceSelChange)
		COMMAND_HANDLER_EX(IDC_ENCODING, CBN_SELCHANGE, OnChangeDefault)
		COMMAND_HANDLER_EX(IDC_CHECK1, BN_CLICKED, OnChangeDefault)
		COMMAND_HANDLER_EX(IDC_DELAY, EN_CHANGE, OnEditDelayChange)
		COMMAND_HANDLER_EX(IDC_CHECK2, BN_CLICKED, OnCheckBoxFileClicked)
		COMMAND_HANDLER_EX(IDC_CHECK3, BN_CLICKED, OnCheckBoxDelayClicked)
		COMMAND_HANDLER_EX(IDC_BUTTON1, BN_CLICKED, OnBnClickedAdd)
		COMMAND_HANDLER_EX(IDC_BUTTON2, BN_CLICKED, OnBnClickedRemove)
		COMMAND_HANDLER_EX(IDC_BUTTON3, BN_CLICKED, OnBnClickedFileChooser)
		COMMAND_HANDLER_EX(IDC_BUTTON4, BN_CLICKED, OnBnClickedRename)
		COMMAND_HANDLER_EX(IDC_PATTERN, EN_CHANGE, OnEditPatternChange)
		COMMAND_HANDLER_EX(IDC_CHECK4, BN_CLICKED, OnCheckBoxOnExitClicked)
		COMMAND_HANDLER_EX(IDC_ON_EXIT, EN_CHANGE, OnChangeDefault)
		COMMAND_HANDLER_EX(IDC_CHECK5, BN_CLICKED, OnChangeDefault)
		COMMAND_HANDLER_EX(IDC_CHECK6, BN_CLICKED, OnCheckBoxClipboardClick)
		COMMAND_HANDLER_EX(IDC_ENABLED, BN_CLICKED, OnChangeDefault)
		MSG_WM_CONTEXTMENU(OnContextMenu)
	END_MSG_MAP()

	enum {
		FN_DIALOG_SAVE = 0,
		FN_DIALOG_OPEN
	};

	void config_export(pfc::string8 &fn);
	void config_import(pfc::string8 &fn);
	bool file_dialog(int mode, pfc::string8 &out, const std::vector<fn_filter> &filter);
	void OnChanged();

	bool event_flags[EVENT_COUNT];
private:
	BOOL OnInitDialog(CWindow, LPARAM);
	void OnChangeDefault(UINT, int, CWindow);
	void OnComboInstanceSelChange(UINT, int, CWindow);
	void OnComboTextChange(UINT, int, CWindow);
	void OnBnClickedAdd(UINT, int, CWindow);
	void OnBnClickedRemove(UINT, int, CWindow);
	void OnBnClickedRename(UINT, int, CWindow);
	void OnCheckBoxFileClicked(UINT, int, CWindow);
	void OnCheckBoxDelayClicked(UINT, int, CWindow);
	void OnCheckBoxOnExitClicked(UINT, int, CWindow);
	void OnBnClickedFileChooser(UINT, int, CWindow);
	void OnEditPatternChange(UINT, int, CWindow);
	void OnEditDelayChange(UINT, int, CWindow);
	void OnCheckBoxClipboardClick(UINT, int, CWindow);
	void OnContextMenu(CWindow wnd, CPoint point);

	void PatternPreviewUpdate(uint32_t event, bool force = false);
	bool HasChanged();

	void PopulateContextList();
	void SetControlAvailabilityFile();
	void SetControlAvailabilityDelay();
	void SetControlAvailabilityOnExit();
	void SetControlAvailabilityClipboard();
	void ResetToUnselectedState();
	void ResetToDefault();
	bool HasComboString(CString &str);
	void ComboInstanceSelect(int index);

	void ResetFlags()
	{
		for (int i = 0; i < EVENT_COUNT; i++) {
			event_flags[i] = false;
		}
		event_flags[EVENT_PLAYBACK_STOP] = true;
		event_flags[EVENT_PLAYBACK_PAUSE] = true;
		event_flags[EVENT_PLAYBACK_NEW_TRACK] = true;
	}

	virtual void event_update(uint32_t event) {
		PatternPreviewUpdate(event);
	}

	CComboBox m_ComboBoxInstance;
	CComboBox m_ComboBoxEncoding;
	CCheckBox m_CheckBoxLogMode;
	CCheckBox m_CheckBoxWriteToFile;
	CCheckBox m_CheckBoxDelay;
	CCheckBox m_CheckBoxOnExit;
	CCheckBox m_CheckBoxChangesOnly;
	CCheckBox m_CheckBoxClipboard;
	CCheckBox m_CheckEnabled;
	CEdit m_EditFilename;
	CEdit m_EditPattern;
	CEdit m_EditDelay;
	CEdit m_EditOnExit;
	CButton m_ButtonAddInstance;
	CButton m_ButtonRemoveInstance;
	CButton m_ButtonEvent;
	CButton m_ButtonFileChooser;
	CButton m_ButtonRenameInstance;
	CButton m_ButtonConfig;
	CWindow m_WinDelaySpin;
	CStatic m_StaticEncoding;

	const preferences_page_callback::ptr m_callback;
	titleformat_object::ptr m_script;

	int m_curIndex = -1;

	static const INT idc_delay_hardlimit = 3600000;
};

class CContextMenuBase {
public:
	CContextMenuBase(CWindow &wnd, CPoint &p_point, CNPAPreferences *p_parent) :
		parent(p_parent), point(p_point) {

		if (point == CPoint(-1, -1)) {
			CRect rc;
			WIN32_OP(wnd.GetWindowRect(&rc));
			point = rc.CenterPoint();
		}

		WIN32_OP(menu.CreatePopupMenu());
	};

	virtual void popup() {}

	CMenu menu;
	CPoint point;
	CNPAPreferences *parent;
};

class IConfig {
public:

	static t_size Count()
	{
		return m_cfg_objlist->get_count();
	}

	static instance_item Get(t_size i)
	{
		return m_cfg_objlist->get_item(i);
	}

	static bool IsEnabled() {
		return m_cfg_masterswitch->get_value();
	}
private:
	static cfg_objList<instance_item> *m_cfg_objlist;
	static cfg_bool *m_cfg_masterswitch;
};
