#include "stdafx.h"

#include <fstream>
#include <algorithm>

using namespace std;

static const GUID cfg_instance_list_guid =
{ 0xAAD2BD90,0xAECA,0x42AA,{0x8B, 0x9D, 0x5A, 0x4B, 0xD9, 0x64, 0xB2, 0x82} };
static const GUID guid_cfg_masterswitch =
{ 0x2130D72F,0x8A53,0x41A5,{0xAE, 0x67, 0xE2, 0x83, 0xD9, 0x2A, 0x7D, 0x43} };

static cfg_bool cfg_masterswitch(guid_cfg_masterswitch, true);
cfg_bool *IConfig::m_cfg_masterswitch = &cfg_masterswitch;

static cfg_objList<instance_item> g_cfg_instance_list(cfg_instance_list_guid);
cfg_objList<instance_item> *IConfig::m_cfg_objlist = &g_cfg_instance_list;

BOOL CNPAPreferences::OnInitDialog(CWindow, LPARAM)
{
	m_ComboBoxInstance = GetDlgItem(IDC_COMBO1);
	m_CheckBoxLogMode = GetDlgItem(IDC_CHECK1);
	m_CheckBoxWriteToFile = GetDlgItem(IDC_CHECK2);
	m_EditFilename = GetDlgItem(IDC_FILENAME);
	m_EditPattern = GetDlgItem(IDC_PATTERN);
	m_ButtonAddInstance = GetDlgItem(IDC_BUTTON1);
	m_ButtonRemoveInstance = GetDlgItem(IDC_BUTTON2);
	m_ButtonRenameInstance = GetDlgItem(IDC_BUTTON4);
	m_ButtonFileChooser = GetDlgItem(IDC_BUTTON3);
	m_ButtonEvent = GetDlgItem(IDC_CONTEXTMENU_EVENTS);
	m_ButtonConfig = GetDlgItem(IDC_CONTEXT_CONFIG);
	m_EditDelay = GetDlgItem(IDC_DELAY);
	m_CheckBoxDelay = GetDlgItem(IDC_CHECK3);
	m_WinDelaySpin = GetDlgItem(IDC_SPIN1);
	m_CheckBoxOnExit = GetDlgItem(IDC_CHECK4);
	m_EditOnExit = GetDlgItem(IDC_ON_EXIT);
	m_ComboBoxEncoding = GetDlgItem(IDC_ENCODING);
	m_StaticEncoding = GetDlgItem(IDC_STATIC5);
	m_CheckBoxChangesOnly = GetDlgItem(IDC_CHECK5);
	m_CheckBoxClipboard = GetDlgItem(IDC_CHECK6);
	m_CheckEnabled = GetDlgItem(IDC_ENABLED);
	m_StaticInstance = GetDlgItem(IDC_STATIC_INSTANCE);

	m_CheckEnabled.SetCheck(cfg_masterswitch ? 1 : 0);

	SendMessage(m_WinDelaySpin, UDM_SETBUDDY, (WPARAM)(HWND)m_EditDelay, 0);
	SendMessage(m_WinDelaySpin, UDM_SETRANGE32, 0, CNPAPreferences::idc_delay_hardlimit);

	SetDlgItemInt(IDC_DELAY, 0, false);

	m_ComboBoxEncoding.AddString(_T("ANSI"));
	m_ComboBoxEncoding.AddString(_T("UTF-8"));
	m_ComboBoxEncoding.AddString(_T("UTF-8 BOM"));
	m_ComboBoxEncoding.AddString(_T("UCS-2 BE"));
	m_ComboBoxEncoding.AddString(_T("UCS-2 LE"));
	m_ComboBoxEncoding.AddString(_T("UCS-2 BE BOM"));
	m_ComboBoxEncoding.AddString(_T("UCS-2 LE BOM"));
	m_ComboBoxEncoding.AddString(_T("OEM"));
	m_ComboBoxEncoding.AddString(_T("MAC"));

	PopulateContextList();

	return FALSE;
}

void CNPAPreferences::PopulateContextList()
{
	for (t_size i = 0; i < g_cfg_instance_list.get_count(); i++) {
		instance_item &item = g_cfg_instance_list.get_item(i);
		CA2CT w_name(item.name.c_str());
		m_ComboBoxInstance.AddString(w_name);
	}

	if (m_ComboBoxInstance.GetCount() > 0) {
		ComboInstanceSelect(0);
	}
}

void CNPAPreferences::OnChangeDefault(UINT, int, CWindow)
{
	OnChanged();
}

void CNPAPreferences::ResetToUnselectedState()
{
	m_curIndex = -1;
	uSetDlgItemText(*this, IDC_COMBO1, "");
	uSetDlgItemText(*this, IDC_FILENAME, "");
	uSetDlgItemText(*this, IDC_PATTERN, "");
	SetDlgItemInt(IDC_DELAY, 0);
	m_script.release();
	m_CheckBoxWriteToFile.SetCheck(0);
	m_CheckBoxLogMode.SetCheck(0);
	m_EditPattern.EnableWindow(false);
	m_ButtonEvent.EnableWindow(false);
	m_EditFilename.EnableWindow(false);
	m_CheckBoxWriteToFile.EnableWindow(false);
	m_CheckBoxLogMode.EnableWindow(false);
	m_ButtonFileChooser.EnableWindow(false);
	m_ButtonAddInstance.EnableWindow(false);
	m_ButtonRenameInstance.EnableWindow(false);
	m_ButtonRemoveInstance.EnableWindow(false);
	m_CheckBoxDelay.EnableWindow(false);
	m_EditDelay.EnableWindow(false);
	m_WinDelaySpin.EnableWindow(false);
	m_CheckBoxOnExit.EnableWindow(false);
	m_EditOnExit.EnableWindow(false);
	m_ComboBoxEncoding.EnableWindow(false);
	m_StaticEncoding.EnableWindow(false);
	m_CheckBoxChangesOnly.EnableWindow(false);
	m_CheckBoxClipboard.EnableWindow(false);
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
	m_WinDelaySpin.EnableWindow(false);
	m_EditOnExit.EnableWindow(false);
	m_ComboBoxEncoding.EnableWindow(false);
	m_StaticEncoding.EnableWindow(false);
	m_CheckBoxChangesOnly.EnableWindow(false);
	ResetFlags();
}

bool get_item_using_clipboard(instance_item &out) {
	for (t_size i = 0; i < g_cfg_instance_list.get_count(); i++) {
		instance_item &item = g_cfg_instance_list.get_item(i);
		if (item.clipboard) {
			out = item;
			return true;
		}
	}
	return false;
}

void CNPAPreferences::ComboInstanceSelect(int index) {
	m_ComboBoxInstance.SetCurSel(index);
	OnComboInstanceSelChange(0, CBN_SELCHANGE, m_ComboBoxInstance);
}

void CNPAPreferences::OnComboInstanceSelChange(UINT, int, CWindow)
{
	m_curIndex = m_ComboBoxInstance.GetCurSel();

	m_script.release();

	instance_item item = g_cfg_instance_list.get_item(m_curIndex);

	uSetDlgItemText(*this, IDC_FILENAME, item.filename);
	uSetDlgItemText(*this, IDC_PATTERN, item.format_string);
	uSetDlgItemText(*this, IDC_ON_EXIT, item.on_exit_str);
	m_CheckBoxWriteToFile.SetCheck(item.write_to_file ? 1 : 0);
	m_CheckBoxLogMode.SetCheck(item.log_mode ? 1 : 0);
	m_CheckBoxDelay.SetCheck(item.enable_delay ? 1 : 0);
	m_CheckBoxOnExit.SetCheck(item.on_exit ? 1 : 0);
	m_CheckBoxChangesOnly.SetCheck(item.changes_only ? 1 : 0);
	m_CheckBoxClipboard.SetCheck(item.clipboard ? 1 : 0);
	SetDlgItemInt(IDC_DELAY, item.delay);
	m_ComboBoxEncoding.SetCurSel(item.encoding);

	for (int i = 0; i < EVENT_COUNT; i++) {
		event_flags[i] = item.events[i];
	}

	m_EditPattern.EnableWindow(true);
	m_ButtonEvent.EnableWindow(true);
	m_CheckBoxWriteToFile.EnableWindow(true);
	m_ButtonFileChooser.EnableWindow(true);
	m_CheckBoxOnExit.EnableWindow(true);

	instance_item cpitem;
	if (get_item_using_clipboard(cpitem)) {
		if (cpitem == item) {
			m_CheckBoxClipboard.EnableWindow(true);
		}
		else {
			m_CheckBoxClipboard.EnableWindow(false);
		}
	}
	else {
		m_CheckBoxClipboard.EnableWindow(true);
	}

	SetControlAvailabilityClipboard();
	SetControlAvailabilityOnExit();

	m_ButtonAddInstance.EnableWindow(false);
	m_ButtonRemoveInstance.EnableWindow(true);
	m_ButtonRenameInstance.EnableWindow(false);

	/*
	pfc::string8 title;
	title << "Instance (" << item.name << ")";
	uSetWindowText(m_StaticInstance, title);
	*/

	OnChanged();
}

void CNPAPreferences::OnComboTextChange(UINT, int, CWindow)
{
	CString str;
	GetDlgItemText(IDC_COMBO1, str);
	str = str.Trim().MakeLower();

	if (str.GetLength() == 0) {
		m_ButtonAddInstance.EnableWindow(false);
		m_ButtonRemoveInstance.EnableWindow(false);
		m_ButtonRenameInstance.EnableWindow(false);
		return;
	}
	else {
		m_ButtonRemoveInstance.EnableWindow(true);
	}

	if (HasComboString(str)) {
		m_ButtonAddInstance.EnableWindow(false);		
		m_ButtonRenameInstance.EnableWindow(false);
	}
	else {
		m_ButtonAddInstance.EnableWindow(true);
		m_ButtonRenameInstance.EnableWindow(true);
	}
	
}

void CNPAPreferences::OnBnClickedAdd(UINT, int, CWindow)
{
	CString str;
	GetDlgItemText(IDC_COMBO1, str);
	str = str.Trim().MakeLower();

	if (HasComboString(str)) {
		return;
	}

	g_cfg_instance_list.add_item(instance_item((const char *)CT2CA(str)));
	m_ComboBoxInstance.InsertString(m_ComboBoxInstance.GetCount(), str);
	ComboInstanceSelect(m_ComboBoxInstance.GetCount() - 1);
}

void CNPAPreferences::OnBnClickedRemove(UINT, int, CWindow)
{
	if (m_curIndex < 0) {
		return;
	}

	instance_item &item = g_cfg_instance_list.get_item(m_curIndex);
	g_cfg_instance_list.remove_by_idx(m_curIndex);
	m_ComboBoxInstance.DeleteString(m_curIndex);
	IEvents::RemoveInstance(item.name);

	if (m_ComboBoxInstance.GetCount() > 0) {
		ComboInstanceSelect(min(m_curIndex, m_ComboBoxInstance.GetCount() - 1));
	}
	else {
		ResetToUnselectedState();
	}
}

void CNPAPreferences::OnBnClickedRename(UINT, int, CWindow)
{
	if (m_curIndex < 0) {
		return;
	}

	CString str;
	GetDlgItemText(IDC_COMBO1, str);
	str = str.Trim().MakeLower();

	if (HasComboString(str)) {
		return;
	}

	instance_item &item = g_cfg_instance_list.get_item(m_curIndex);
	item.name = pfc::string8(CT2CA(str));
	g_cfg_instance_list.replace_item(m_curIndex, item);
	IEvents::UpdateInstance(item);

	m_ComboBoxInstance.DeleteString(m_curIndex);
	m_ComboBoxInstance.InsertString(m_curIndex, str);
	OnComboTextChange(0, IDC_COMBO1, m_ComboBoxInstance);
}

bool CNPAPreferences::HasComboString(CString &ls) {
	for (int i = 0; i < m_ComboBoxInstance.GetCount(); i++) {
		CString rs;
		m_ComboBoxInstance.GetLBText(i, rs);

		if (ls == rs.Trim().MakeLower()) {
			return true;
		}
	}
	return false;
}

void CNPAPreferences::SetControlAvailabilityFile()
{
	if (m_CheckBoxWriteToFile.IsChecked() &&
		m_CheckBoxWriteToFile.IsWindowEnabled()) {
		m_EditFilename.EnableWindow(true);
		m_CheckBoxLogMode.EnableWindow(true);
		m_ButtonFileChooser.EnableWindow(true);
		m_CheckBoxDelay.EnableWindow(true);
		m_ComboBoxEncoding.EnableWindow(true);
		m_StaticEncoding.EnableWindow(true);
		m_CheckBoxChangesOnly.EnableWindow(true);
	}
	else {
		m_EditFilename.EnableWindow(false);
		m_CheckBoxLogMode.EnableWindow(false);
		m_ButtonFileChooser.EnableWindow(false);
		m_CheckBoxDelay.EnableWindow(false);
		m_ComboBoxEncoding.EnableWindow(false);
		m_StaticEncoding.EnableWindow(false);
		m_CheckBoxChangesOnly.EnableWindow(false);
	}
	SetControlAvailabilityDelay();
}

void CNPAPreferences::SetControlAvailabilityDelay()
{
	if (m_CheckBoxWriteToFile.IsChecked() &&
		m_CheckBoxDelay.IsChecked()) {
		m_WinDelaySpin.EnableWindow(true);
		m_EditDelay.EnableWindow(true);
	}
	else {
		m_WinDelaySpin.EnableWindow(false);
		m_EditDelay.EnableWindow(false);
	}
}

void CNPAPreferences::SetControlAvailabilityOnExit() {
	if (m_CheckBoxOnExit.IsChecked()) {
		m_EditOnExit.EnableWindow(true);
	}
	else {
		m_EditOnExit.EnableWindow(false);
	}
}

void CNPAPreferences::SetControlAvailabilityClipboard() {
	if (!m_CheckBoxClipboard.IsChecked()) {
		m_CheckBoxWriteToFile.EnableWindow(true);
	}
	else {
		m_CheckBoxWriteToFile.EnableWindow(false);
	}
	SetControlAvailabilityFile();
}

void CNPAPreferences::OnCheckBoxFileClicked(UINT, int, CWindow)
{
	SetControlAvailabilityFile();
	OnChanged();
}

void CNPAPreferences::OnCheckBoxDelayClicked(UINT, int, CWindow)
{
	SetControlAvailabilityDelay();
	OnChanged();
}

void CNPAPreferences::OnCheckBoxOnExitClicked(UINT, int, CWindow) {
	SetControlAvailabilityOnExit();
	OnChanged();
}

void CNPAPreferences::OnCheckBoxClipboardClick(UINT, int, CWindow) {
	SetControlAvailabilityClipboard();
	OnChanged();
}

bool CNPAPreferences::file_dialog(int mode, pfc::string8 &out, const vector<fn_filter> &filter) {
	LPWSTR szFile = (LPWSTR)malloc(MAX_PATH);

	OPENFILENAME ofn;
	ZeroMemory(szFile, sizeof(szFile));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = *this;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	CString szFilter;
	for (auto const& v : filter) {
		pfc::string8 s = v.desc;
		s << "|*." << v.ext << "|";
		szFilter += CA2CT(s);
	}
	szFilter += "|";
	szFilter.Replace('|', '\0');
	ofn.lpstrFilter = (LPCWSTR)szFilter;
	ofn.lpstrDefExt = (LPCWSTR)NULL;
	ofn.nFilterIndex = min(filter.size(), 1);
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER;

	bool res;
	if (mode == FN_DIALOG_OPEN) {
		ofn.Flags |= (OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST);
		res = GetOpenFileName(&ofn);
	}
	else {
		res = GetSaveFileName(&ofn);
	}

	if (res) {
		pfc::string8 fn = CT2CA(szFile);
		if (ofn.nFileExtension == 0) {
			if (ofn.nFilterIndex > 0 && ofn.nFilterIndex <= filter.size()) {
				const fn_filter &f = filter[ofn.nFilterIndex - 1];
				if (f.ext != pfc::string8("*")) {
					fn << "." << f.ext;
				}
			}
		}
		out = fn;
	}

	free(szFile);
	return res;
}

void CNPAPreferences::OnBnClickedFileChooser(UINT, int, CWindow)
{
	pfc::string8 fn;
	const vector<fn_filter> fmt = {
		fn_filter("txt", "Text") ,
		fn_filter("*", "All")
	};

	if (file_dialog(FN_DIALOG_SAVE, fn, fmt)) {
		uSetDlgItemText(*this, IDC_FILENAME, fn);
		OnChanged();
	}
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

void CNPAPreferences::config_export(pfc::string8 &fn) {
	if (g_cfg_instance_list.get_count() == 0) {
		return;
	}
	try {
		Json::Value root;
		for (t_size i = 0; i < g_cfg_instance_list.get_count(); i++) {
			instance_item &item = g_cfg_instance_list.get_item(i);
			root.append(item.to_json());
		}

		fstream fs(fn, ios::out | ios::trunc | ifstream::binary);
		fs << root;
		fs.close();
	}
	catch (exception const & e) {
		popup_message::g_complain("Export failed", e);
	}
}

static bool find_instance(const pfc::string8 &name, instance_item &out, t_size &index) {
	for (t_size i = 0; i < g_cfg_instance_list.get_count(); i++) {
		const instance_item &item = g_cfg_instance_list.get_item(i);
		if (item.name == name) {
			index = i;
			out = item;
			return true;
		}
	}
	return false;
}

void CNPAPreferences::config_import(pfc::string8 &fn) {
	Json::Value root;
	try {
		fstream fs(fn, ios::in | ifstream::binary);
		fs >> root;
		fs.close();
	}
	catch (exception const & e) {
		popup_message::g_complain("Import failed", e);
		return;
	}

	vector<instance_item> items;

	try {
		for (Json::ArrayIndex i = 0; i < root.size(); i++) {
			instance_item item(root[i]);
			items.push_back(item);
		}
	}
	catch (exception const & e) {
		popup_message::g_complain("Importing items failed", e);
		return;
	}

	if (items.size() == 0) {
		return;
	}

	bool wasempty = m_curIndex < 0 ? true : false;

	console::info("NPA: Starting import");
	pfc::string8 s;
	for (const auto &item : items) {
		instance_item ex;
		t_size index;
		if (find_instance(item.name, ex, index)) {
			console::info(s << "NPA: Updating instance '" << item.name << "'");
			g_cfg_instance_list.replace_item(index, item);
		}
		else {
			console::info(s << "NPA: Adding instance: '" << item.name << "'");
			g_cfg_instance_list.add_item(item);
			m_ComboBoxInstance.AddString(CA2CT(item.name));
		}
		s.reset();
		IEvents::UpdateInstance(item);
	}
	console::info("NPA: Import done");

	if (wasempty) {
		ComboInstanceSelect(0);
	}
	else {
		ComboInstanceSelect(m_curIndex);
	}
}

class CContextMenuEvent : public CContextMenuBase {
public:
	CContextMenuEvent(CWindow &wnd, CPoint &p_point, CNPAPreferences *p_parent) :
		CContextMenuBase(wnd, p_point, p_parent) {}

	void popup() {
		CMenuDescriptionHybrid menudesc(*parent);

		for (int i = 0; i < EVENT_COUNT; i++) {
			CA2CT w(IEvents::EventToString(i));
			UINT flags = MF_STRING;
			if (parent->event_flags[i]) {
				flags |= MF_CHECKED;
			}
			int id = i + 1;
			menu.AppendMenu(flags, id, w);
			menudesc.Set(id, IEvents::GetEventDescription(i));
		}

		int cmd = menu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, menudesc, 0);
		if (cmd > 0 && cmd <= EVENT_COUNT) {
			int eventid = cmd - 1;
			parent->event_flags[eventid] = !parent->event_flags[eventid];
			parent->OnChanged();
		}
	}
};

class CContextMenuConfig : public CContextMenuBase {
public:
	CContextMenuConfig(CWindow &wnd, CPoint &p_point, CNPAPreferences *p_parent) :
		CContextMenuBase(wnd, p_point, p_parent) {}

	void popup() {
		CMenuDescriptionHybrid menudesc(*parent);

		enum {
			ID_IMPORT = 1,
			ID_EXPORT,
		};

		menu.AppendMenu(MF_STRING, ID_IMPORT, _T("Import"));
		menudesc.Set(ID_IMPORT, "Import configuration");
		if (g_cfg_instance_list.get_count() > 0) {
			menu.AppendMenu(MF_STRING, ID_EXPORT, _T("Export"));
			menudesc.Set(ID_EXPORT, "Export configuration");
		}

		int cmd = menu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, menudesc, 0);
		if (cmd < 1) {
			return;
		}
		pfc::string8 fn;
		if (cmd == ID_IMPORT) {
			const vector<fn_filter> fmt = {
				fn_filter("json", "JSON") ,
				fn_filter("*", "All")
			};
			if (parent->file_dialog(parent->FN_DIALOG_OPEN, fn, fmt)) {
				parent->config_import(fn);
			}
		}
		else if (cmd == ID_EXPORT) {
			pfc::string8 fn;
			vector<fn_filter> fmt = {
				fn_filter("json", "JSON") ,
				fn_filter("*", "All")
			};
			if (parent->file_dialog(parent->FN_DIALOG_SAVE, fn, fmt)) {
				parent->config_export(fn);
			}
		}
	}
};

void CNPAPreferences::OnContextMenu(CWindow wnd, CPoint point)
{
	try {
		if (wnd == m_ButtonEvent) {
			CContextMenuEvent(wnd, point, this).popup();
		}
		else if (wnd == m_ButtonConfig) {
			CContextMenuConfig(wnd, point, this).popup();
		}
	}
	catch (exception const & e) {
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
	if (m_curIndex < 0) {
		return;
	}

	cfg_masterswitch = m_CheckEnabled.IsChecked();

	instance_item item = g_cfg_instance_list.get_item(m_curIndex);

	pfc::string8 str;
	uGetDlgItemText(*this, IDC_FILENAME, str);
	if (m_CheckBoxWriteToFile.IsChecked() && str.length() == 0) {
		popup_message::g_complain("Set a file path");
		OnChanged();
		return;
	}
	item.filename = str;
	uGetDlgItemText(*this, IDC_PATTERN, str);
	item.format_string = str;
	uGetDlgItemText(*this, IDC_ON_EXIT, str);
	item.on_exit_str = str;
	item.write_to_file = m_CheckBoxWriteToFile.IsChecked();
	item.encoding = max(min((uint8_t)m_ComboBoxEncoding.GetCurSel(), ENCODING_COUNT - 1), 0);
	item.log_mode = m_CheckBoxLogMode.IsChecked();
	item.enable_delay = m_CheckBoxDelay.IsChecked();
	item.on_exit = m_CheckBoxOnExit.IsChecked();
	item.changes_only = m_CheckBoxChangesOnly.IsChecked();
	item.clipboard = m_CheckBoxClipboard.IsChecked();
	item.delay = (uint32_t)GetDlgItemInt(IDC_DELAY, false);

	for (int i = 0; i < EVENT_COUNT; i++) {
		item.events[i] = event_flags[i];
	}

	g_cfg_instance_list.replace_item(m_curIndex, item);
	IEvents::UpdateInstance(item);

	OnChanged();
}

bool CNPAPreferences::HasChanged()
{
	if (m_curIndex < 0) {
		return false;
	}

	if (m_CheckEnabled.IsChecked() != cfg_masterswitch) {
		return true;
	}

	instance_item item = g_cfg_instance_list.get_item(m_curIndex);

	if (m_CheckBoxLogMode.IsChecked() != item.log_mode) {
		return true;
	}

	if (m_CheckBoxWriteToFile.IsChecked() != item.write_to_file) {
		return true;
	}

	if (m_ComboBoxEncoding.GetCurSel() != item.encoding) {
		return true;
	}

	if (m_CheckBoxDelay.IsChecked() != item.enable_delay) {
		return true;
	}

	if (m_CheckBoxOnExit.IsChecked() != item.on_exit) {
		return true;
	}

	if (m_CheckBoxClipboard.IsChecked() != item.clipboard) {
		return true;
	}

	if (m_CheckBoxChangesOnly.IsChecked() != item.changes_only) {
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

	uGetDlgItemText(*this, IDC_ON_EXIT, str);
	if (str != item.on_exit_str) {
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
	PatternPreviewUpdate(0, true);
	m_callback->on_state_changed();
}

void CNPAPreferences::PatternPreviewUpdate(uint32_t event, bool force)
{
	if (force || m_script.is_empty()) {
		pfc::string8 pattern;
		uGetDlgItemText(*this, IDC_PATTERN, pattern);
		static_api_ptr_t<titleformat_compiler>()->compile_safe_ex(m_script, pattern);
		uSetDlgItemText(*this, IDC_PREVIEW, format_title(m_script));
	}
	else {
		if (event_flags[event]) {
			uSetDlgItemText(*this, IDC_PREVIEW, format_title(m_script));
		}
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
