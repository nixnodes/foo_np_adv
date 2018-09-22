#pragma once

#include "resource.h"
#include "stdafx.h"
#include "events.h"
#include "prefs_instance.h"

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
		COMMAND_HANDLER_EX(IDC_BOGO1, EN_CHANGE, OnChangeDefault)
		COMMAND_HANDLER_EX(IDC_BOGO2, EN_CHANGE, OnChangeDefault)
		COMMAND_HANDLER_EX(IDC_FILENAME, EN_CHANGE, OnChangeDefault)
		COMMAND_HANDLER_EX(IDC_COMBO1, CBN_EDITCHANGE, OnComboTextChange)
		COMMAND_HANDLER_EX(IDC_COMBO1, CBN_SELCHANGE, OnComboSelChange)
		COMMAND_HANDLER_EX(IDC_CHECK1, BN_CLICKED, OnChangeDefault)
		COMMAND_HANDLER_EX(IDC_DELAY, EN_CHANGE, OnEditDelayChange)
		COMMAND_HANDLER_EX(IDC_CHECK2, BN_CLICKED, OnCheckBoxFileClicked)
		COMMAND_HANDLER_EX(IDC_CHECK3, BN_CLICKED, OnCheckBoxDelayClicked)
		COMMAND_HANDLER_EX(IDC_BUTTON1, BN_CLICKED, OnBnClickedAdd)
		COMMAND_HANDLER_EX(IDC_BUTTON2, BN_CLICKED, OnBnClickedRemove)
		COMMAND_HANDLER_EX(IDC_BUTTON3, BN_CLICKED, OnBnClickedFileChooser)
		COMMAND_HANDLER_EX(IDC_PATTERN, EN_CHANGE, OnEditPatternChange)
		COMMAND_HANDLER_EX(IDC_CHECK4, BN_CLICKED, OnCheckBoxOnExitClicked)
		COMMAND_HANDLER_EX(IDC_ON_EXIT, EN_CHANGE, OnChangeDefault)
		MSG_WM_CONTEXTMENU(OnContextMenu)
	END_MSG_MAP()
private:
	BOOL OnInitDialog(CWindow, LPARAM);
	void OnChangeDefault(UINT, int, CWindow);
	void OnComboSelChange(UINT, int, CWindow);
	void OnComboTextChange(UINT, int, CWindow);
	void OnBnClickedAdd(UINT, int, CWindow);
	void OnBnClickedRemove(UINT, int, CWindow);
	void OnCheckBoxFileClicked(UINT, int, CWindow);
	void OnCheckBoxDelayClicked(UINT, int, CWindow);
	void OnCheckBoxOnExitClicked(UINT, int, CWindow);
	void OnBnClickedFileChooser(UINT, int, CWindow);
	void OnEditPatternChange(UINT, int, CWindow);
	void OnEditDelayChange(UINT, int, CWindow);
	void OnContextMenu(CWindow wnd, CPoint point);
	void PatternPreviewUpdate(uint32_t ev);
	bool HasChanged();
	void OnChanged();
	void PopulateContextList();
	void SetControlAvailabilityFile();
	void SetControlAvailabilityDelay();
	void SetControlAvailabilityOnExit();
	void ResetToUnselectedState();
	void ResetToDefault();

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
	CCheckBox m_CheckBoxLogMode;
	CCheckBox m_CheckBoxWriteToFile;
	CCheckBox m_CheckBoxDelay;
	CCheckBox m_CheckBoxOnExit;
	CEdit m_EditFilename;
	CEdit m_EditPattern;
	CEdit m_EditDelay;
	CEdit m_EditOnExit;
	CButton m_ButtonAddInstance;
	CButton m_ButtonRemoveInstance;
	CButton m_ButtonEvent;
	CButton m_ButtonFileChooser;
	CWindow m_DelaySpin;
	
	const preferences_page_callback::ptr m_callback;
	titleformat_object::ptr m_script;
	static_api_ptr_t<playback_control> m_playback_control;

	bool event_flags[EVENT_COUNT];

	static const INT idc_delay_hardlimit = 3600000;
};

class IConfig{
public:
	
	static t_size Count()
	{
		return m_cfg_objlist->get_count();
	}

	static instance_item Get(t_size i)
	{
		return m_cfg_objlist->get_item(i);
	}
private:
	static cfg_objList<instance_item> *m_cfg_objlist;
};
