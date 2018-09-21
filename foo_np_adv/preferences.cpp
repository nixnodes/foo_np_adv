#include "stdafx.h"
#include "preferences.h"
#include "events.h"

static const GUID cfg_instance_list_guid =
{ 0xAAD2BD90,0xAECA,0x42AA,{0x8B, 0x9D, 0x5A, 0x4B, 0xD9, 0x64, 0xB2, 0x82} };

static cfg_objList<instance_item> g_cfg_instance_list(cfg_instance_list_guid);

cfg_objList<instance_item> *IConfig::m_cfg_objlist = &g_cfg_instance_list;

BOOL CNPAPreferences::OnInitDialog(CWindow, LPARAM)
{
	m_ComboBoxInstance = GetDlgItem(IDC_COMBO1);
	m_CheckBoxLogMode = GetDlgItem(IDC_CHECK1);
	m_CheckBoxWriteToFile = GetDlgItem(IDC_CHECK2);
	m_EditFilename = GetDlgItem(IDC_FILENAME);
	m_Pattern = GetDlgItem(IDC_PATTERN);
	m_ButtonAddInstance = GetDlgItem(IDC_BUTTON1);
	m_ButtonRemoveInstance = GetDlgItem(IDC_BUTTON2);
	m_ButtonFileChooser = GetDlgItem(IDC_BUTTON3);
	m_ButtonEvent = GetDlgItem(IDC_CONTEXTMENU);  
	m_EditDelay = GetDlgItem(IDC_DELAY);
	m_CheckBoxDelay = GetDlgItem(IDC_CHECK3);
	m_DelaySpin = GetDlgItem(IDC_SPIN1);
	
	SendMessage(m_DelaySpin, UDM_SETBUDDY, (WPARAM)(HWND)m_EditDelay, 0);
	SendMessage(m_DelaySpin, UDM_SETRANGE32, 0, CNPAPreferences::idc_delay_hardlimit);
	//SendMessage(m_DelaySpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(CNPAPreferences::idc_delay_hardlimit, 0));

	SetDlgItemInt(IDC_DELAY, 0, false);

	PopulateContextList();
	

	/*
	GUID guid;

	CoCreateGuid(&guid);

	pfc::string_formatter str;

	str << "{";
	str << "0x" << pfc::format_hex(guid.Data1) << ",";
	str << "0x" << pfc::format_hex(guid.Data2) << ",";
	str << "0x" << pfc::format_hex(guid.Data3) << ",";
	str << "{";

	uint32_t i;
	for (i = 0; i < sizeof(guid.Data4); i++) {
		str << "0x" << pfc::format_hex(guid.Data4[i]);
		if (i < sizeof(guid.Data4) - 1)
			str << ", ";
	}

	str << "} };";

	console::print(str);
	*/
	return FALSE;
}

void CNPAPreferences::PopulateContextList()
{
	uint32_t i;
	for (i = 0; i < g_cfg_instance_list.get_count(); i++) {
		instance_item item = g_cfg_instance_list.get_item(i);
		CA2T w_name(item.name.c_str());
		m_ComboBoxInstance.AddString(w_name);
	}

	if (m_ComboBoxInstance.GetCount() > 0) {
		m_ComboBoxInstance.SetCurSel(0);
		OnComboSelChange(0, CBN_SELCHANGE, m_ComboBoxInstance);
	}
}

void CNPAPreferences::OnChangeDefault(UINT, int, CWindow)
{
	OnChanged();
}

void CNPAPreferences::ResetToUnselectedState()
{
	uSetDlgItemText(*this, IDC_COMBO1, "");
	uSetDlgItemText(*this, IDC_FILENAME, "");
	uSetDlgItemText(*this, IDC_PATTERN, "");
	SetDlgItemInt(IDC_DELAY, 0);
	m_script.release();
	m_CheckBoxWriteToFile.SetCheck(0);
	m_CheckBoxLogMode.SetCheck(0);	
	m_Pattern.EnableWindow(false);
	m_ButtonEvent.EnableWindow(false);
	m_EditFilename.EnableWindow(false);
	m_CheckBoxWriteToFile.EnableWindow(false);
	m_CheckBoxLogMode.EnableWindow(false);
	m_ButtonFileChooser.EnableWindow(false);	
	m_ButtonAddInstance.EnableWindow(false);
	m_ButtonRemoveInstance.EnableWindow(false);
	m_CheckBoxDelay.EnableWindow(false);
	m_EditDelay.EnableWindow(false);
	m_DelaySpin.EnableWindow(false);
	ResetFlags();
}

void CNPAPreferences::ResetToDefault()
{
	uSetDlgItemText(*this, IDC_FILENAME, "");
	uSetDlgItemText(*this, IDC_PATTERN, "");
	SetDlgItemInt(IDC_DELAY, 0);
	m_script.release();
	m_CheckBoxWriteToFile.SetCheck(0);
	m_CheckBoxLogMode.SetCheck(0);	
	m_EditFilename.EnableWindow(false);
	m_CheckBoxWriteToFile.EnableWindow(false);
	m_CheckBoxLogMode.EnableWindow(false);
	m_ButtonFileChooser.EnableWindow(false);
	m_CheckBoxDelay.EnableWindow(false);
	m_EditDelay.EnableWindow(false);
	m_DelaySpin.EnableWindow(false);
	ResetFlags();
}

void CNPAPreferences::OnComboSelChange(UINT, int, CWindow)
{
	int index = m_ComboBoxInstance.GetCurSel();

	if (index < 0) {
		return;
	}

	instance_item item = g_cfg_instance_list.get_item(index);

	uSetDlgItemText(*this, IDC_FILENAME, item.filename);
	uSetDlgItemText(*this, IDC_PATTERN, item.format_string);
	m_script.release();
	m_CheckBoxWriteToFile.SetCheck(item.write_to_file ? 1 : 0);
	m_CheckBoxLogMode.SetCheck(item.log_mode ? 1 : 0);
	m_CheckBoxDelay.SetCheck(item.enable_delay ? 1 : 0);
	SetDlgItemInt(IDC_DELAY, item.delay);

	for (int i = 0; i < EVENT_COUNT; i++) {
		event_flags[i] = item.events[i];
	}

	if (!m_Pattern.IsWindowEnabled()) {
		m_Pattern.EnableWindow(true);
	}
	if (!m_ButtonEvent.IsWindowEnabled()) {
		m_ButtonEvent.EnableWindow(true);
	}
	if (!m_CheckBoxWriteToFile.IsWindowEnabled()) {
		m_CheckBoxWriteToFile.EnableWindow(true);
	}
	if (!m_ButtonFileChooser.IsWindowEnabled()) {
		m_ButtonFileChooser.EnableWindow(true);
	}

	SetControlAvailabilityFile();

	m_ButtonAddInstance.EnableWindow(false);
	m_ButtonRemoveInstance.EnableWindow(true);

	OnChanged();
}

void CNPAPreferences::OnComboTextChange(UINT, int, CWindow)
{
	CString str;
	GetDlgItemText(IDC_COMBO1, str);
	str = str.MakeLower();

	if (str.GetLength() == 0) {
		m_ButtonAddInstance.EnableWindow(false);
		m_ButtonRemoveInstance.EnableWindow(false);
		return;
	}

	for (int i = 0; i < m_ComboBoxInstance.GetCount(); i++) {
		CString str2;
		m_ComboBoxInstance.GetLBText(i, str2);

		if (str == str2.MakeLower()) {
			m_ButtonAddInstance.EnableWindow(false);
			m_ButtonRemoveInstance.EnableWindow(true);
			m_ComboBoxInstance.SetCurSel(i);
			OnComboSelChange(0, CBN_SELCHANGE, m_ComboBoxInstance);
			return;
		}
	}

	m_ButtonAddInstance.EnableWindow(true);
	m_ButtonRemoveInstance.EnableWindow(false);
}

void CNPAPreferences::OnBnClickedAdd(UINT, int, CWindow)
{
	CString str;
	GetDlgItemText(IDC_COMBO1, str);
	CT2CA pszCAS(str.MakeLower());

	for (int i = 0; i < m_ComboBoxInstance.GetCount(); i++) {
		CString str2;
		m_ComboBoxInstance.GetLBText(i, str2);

		if (str == str2.MakeLower()) {
			return;
		}
	}

	instance_item item((pfc::string8) pszCAS, "", "", false, false, false, 0, {});
	g_cfg_instance_list.add_item(item);
	m_ComboBoxInstance.InsertString(m_ComboBoxInstance.GetCount(), str);
	m_ComboBoxInstance.SetCurSel(m_ComboBoxInstance.GetCount() - 1);
	OnComboSelChange(0, CBN_SELCHANGE, m_ComboBoxInstance);
}

void CNPAPreferences::OnBnClickedRemove(UINT, int, CWindow)
{
	int curIndex = m_ComboBoxInstance.GetCurSel();

	if (curIndex < 0) {
		return;
	}

	instance_item item = g_cfg_instance_list.get_item(curIndex);

	g_cfg_instance_list.remove_by_idx(curIndex);
	m_ComboBoxInstance.DeleteString(curIndex);

	IEvents::RemoveInstance(item.name);

	if (m_ComboBoxInstance.GetCount() > 0) {
		m_ComboBoxInstance.SetCurSel(max(m_ComboBoxInstance.GetCount() - 1, 0));
		OnComboSelChange(0, CBN_SELCHANGE, m_ComboBoxInstance);
	}
	else {
		ResetToUnselectedState();
	}
}

void CNPAPreferences::SetControlAvailabilityFile()
{
	if (m_CheckBoxWriteToFile.IsChecked()) {
		m_EditFilename.EnableWindow(true);
		m_CheckBoxLogMode.EnableWindow(true);
		m_ButtonFileChooser.EnableWindow(true);
		m_CheckBoxDelay.EnableWindow(true);
	}
	else {
		m_EditFilename.EnableWindow(false);
		m_CheckBoxLogMode.EnableWindow(false);
		m_ButtonFileChooser.EnableWindow(false);
		m_CheckBoxDelay.EnableWindow(false);
	}	
	SetControlAvailabilityDelay();
}

void CNPAPreferences::SetControlAvailabilityDelay()
{	
	if (m_CheckBoxWriteToFile.IsChecked() && 
		m_CheckBoxDelay.IsChecked()) {
		m_DelaySpin.EnableWindow(true);
		m_EditDelay.EnableWindow(true);
	}
	else {
		m_DelaySpin.EnableWindow(false);
		m_EditDelay.EnableWindow(false);
	}
}

void CNPAPreferences::OnCheckFileClicked(UINT, int, CWindow)
{
	SetControlAvailabilityFile();
	OnChanged();
}

void CNPAPreferences::OnCheckDelayClicked(UINT, int, CWindow)
{
	SetControlAvailabilityDelay();
	OnChanged();
}

void CNPAPreferences::OnBnClickedFileChooser(UINT, int, CWindow) 
{
	OPENFILENAME ofn;
	LPWSTR szFile = (LPWSTR) malloc(MAX_PATH);

	ZeroMemory(szFile, sizeof(szFile));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = *this;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = _T("Text\0*.txt\0All\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_EXPLORER;

	if (GetSaveFileName(&ofn)) {
		SetDlgItemText(IDC_FILENAME, szFile);
		OnChanged();
	}

	free(szFile);	
}

void CNPAPreferences::OnEditPatternChange(UINT, int, CWindow) 
{
	m_script.release();
	OnChanged();
}

void CNPAPreferences::OnEditDelayChange(UINT, int, CWindow) {
	if (GetDlgItemInt(IDC_DELAY, false) > CNPAPreferences::idc_delay_hardlimit) {
		SetDlgItemInt(IDC_DELAY, CNPAPreferences::idc_delay_hardlimit, false);
	}	
	OnChanged();
}

void CNPAPreferences::OnContextMenu(CWindow wnd, CPoint point) {
	try {
		if (wnd == GetDlgItem(IDC_CONTEXTMENU)) {
			if (point == CPoint(-1, -1)) {
				CRect rc;
				WIN32_OP(wnd.GetWindowRect(&rc));
				point = rc.CenterPoint();
			}

			CMenuDescriptionHybrid menudesc(*this);
			CMenu menu;

			WIN32_OP(menu.CreatePopupMenu());

			for (int i = 0; i < EVENT_COUNT; i++) {
				CA2T w(IEvents::EventToString(i));
				UINT flags = MF_STRING;
				if (event_flags[i]) {
					flags |= MF_CHECKED;
				}
				int id = i + 1;
				menu.AppendMenu(flags, id, w);
				menudesc.Set(id, IEvents::GetEventDescription(i));
			}

			int cmd = menu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, menudesc, 0);
			if (cmd > 0) {
				int eventid = cmd - 1;
				event_flags[eventid] = !event_flags[eventid];
				OnChanged();
			}
		}
	}
	catch (std::exception const & e) {
		console::complain("Context menu failure", e);
	}
}

t_uint32 CNPAPreferences::get_state()
{
	t_uint32 state = preferences_state::resettable;
	if (HasChanged())
		state |= preferences_state::changed;
	return state;
}

void CNPAPreferences::reset()
{
	ResetToDefault();
	OnChanged();
}

void CNPAPreferences::apply()
{
	int curIndex = m_ComboBoxInstance.GetCurSel();

	if (curIndex < 0) {
		return;
	}

	instance_item item = g_cfg_instance_list.get_item(curIndex);

	pfc::string8 str;

	uGetDlgItemText(*this, IDC_FILENAME, str);
	if (m_CheckBoxWriteToFile.IsChecked() && str.length() == 0) {
		popup_message::g_complain("No file set");
		OnChanged();
		return;
	}
	item.filename = str;

	uGetDlgItemText(*this, IDC_PATTERN, str);
	item.format_string = str;	
	item.write_to_file = m_CheckBoxWriteToFile.IsChecked();
	item.log_mode = m_CheckBoxLogMode.IsChecked();
	item.enable_delay = m_CheckBoxDelay.IsChecked();
	item.delay = (uint32_t)GetDlgItemInt(IDC_DELAY, false);

	for (int i = 0; i < EVENT_COUNT; i++) {
		item.events[i] = event_flags[i];
	}

	g_cfg_instance_list.replace_item(curIndex, item);
	IEvents::UpdateInstance(&item);	

	OnChanged();
}

bool CNPAPreferences::HasChanged()
{
	int curIndex = m_ComboBoxInstance.GetCurSel();

	if (curIndex < 0) {
		return false;
	}

	instance_item item = g_cfg_instance_list.get_item(curIndex);

	if (m_CheckBoxLogMode.IsChecked() != item.log_mode) {
		return true;
	}

	if (m_CheckBoxWriteToFile.IsChecked() != item.write_to_file) {
		return true;
	}

	if (m_CheckBoxDelay.IsChecked() != item.enable_delay) {
		return true;
	}
	
	if ((uint32_t)GetDlgItemInt(IDC_DELAY, false) != item.delay) {
		return true;
	}

	pfc::string8 str;
	uGetDlgItemText(*this, IDC_FILENAME, str);

	if (str != item.filename) {
		return true;
	}

	uGetDlgItemText(*this, IDC_PATTERN, str);

	if (str != item.format_string) {
		return true;
	}

	for (int i = 0; i < EVENT_COUNT; i++) {
		if (event_flags[i] != item.events[i]) {
			return true;
		}
	}

	return false;
}

void CNPAPreferences::OnChanged()
{
	PatternPreviewUpdate(-1);
	m_callback->on_state_changed();
}

void CNPAPreferences::PatternPreviewUpdate(uint32_t event)
{
	bool trigger;
	if (m_script.is_empty()) {
		pfc::string8 pattern;
		uGetDlgItemText(*this, IDC_PATTERN, pattern);
		static_api_ptr_t<titleformat_compiler>()->compile_safe_ex(m_script, pattern);
		trigger = true;
	}
	else {
		trigger = event_flags[event];
	}

	if (trigger) {
		uSetDlgItemText(*this, IDC_PREVIEW, format_title(m_script));
	}
}

class preferences_page_npaimpl : public preferences_page_impl<CNPAPreferences> {
public:
	const char* get_name()
	{
		return COMMON_NAME;
	}
	GUID get_guid()
	{
		static const GUID guid = { 0xC247A676,0x2ACD,0x4CEB,{0xAC, 0x2D, 0xD8, 0x4B, 0x38, 0xD0, 0x89, 0x94} };
		return guid;
	}
	GUID get_parent_guid()
	{
		return guid_tools;
	}
};

static preferences_page_factory_t<preferences_page_npaimpl> g_preferences_page_npaimpl_factory;