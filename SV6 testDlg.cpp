
// SV6 testDlg.cpp : implementation file
//

#include <iostream>
#include "stdafx.h"
#include "SV6 test.h"
#include "SV6 testDlg.h"
#include "afxdialogex.h"
#include "Globals.h"
#include "GlowESP.h"
#include "Triggerbot.h"
#include "Aimbot.h"
#include "OverlayESP.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "ConsoleManager.h"
#include "RankReveal.h"
#include "RankPanelDlg.h"
#include "Blocker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <codecvt>

using namespace std;
// CSV6testDlg dialog
vector<filesystem::directory_entry> configarray;
int oldsel = -50;
int configselected = -1;
bool filterbypass = false;

CSV6testDlg::CSV6testDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SV6TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSV6testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, tbotbox);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, TerroristColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON3, CTcolor);
	DDX_Control(pDX, IDC_SLIDER1, fovslider);
	DDX_Control(pDX, IDC_SLIDER2, smoothslider);
	DDX_Control(pDX, IDC_COMBO1, BoneSelector);
	DDX_Control(pDX, IDC_COMBO2, Aimkeyselector);
	DDX_Control(pDX, IDC_COMBO3, triggerkeyselector);
	DDX_Control(pDX, IDC_CHECK1, glowesptoggle);
	DDX_Control(pDX, IDC_CHECK4, aimbottoggle);
	DDX_Control(pDX, IDC_CHECK2, triggerbottoggle);
	DDX_Control(pDX, IDC_COMBO4, glowesptselect);
	DDX_Control(pDX, IDC_COMBO5, triggertselect);
	DDX_Control(pDX, IDC_COMBO6, aimbottselect);
	DDX_Control(pDX, IDC_COMBO7, configsbox);
	DDX_Control(pDX, IDC_EDIT1, newcfgname);
	DDX_Control(pDX, IDC_CHECK7, overlayesptoggle);
	DDX_Control(pDX, IDC_CHECK10, hpcheckbox);
	DDX_Control(pDX, IDC_CHECK8, namecheckbox);
	DDX_Control(pDX, IDC_CHECK9, espboxcheckbox);
	DDX_Control(pDX, IDC_CHECK11, enemyonlycheckbox);
	DDX_Control(pDX, IDC_CHECK3, vischeckbox);
	DDX_Control(pDX, IDC_CHECK5, teamcheckbox);
	DDX_Control(pDX, IDC_CHECK6, triggeronkeybox);
	DDX_Control(pDX, IDC_COMBO8, menukeyselect);
	DDX_Control(pDX, IDC_EDIT3, chatmessagebox);
	DDX_Control(pDX, IDC_CHECK13, bypassfiltercheckbox);
	DDX_Control(pDX, IDC_COMBO9, chatbindselect);
	DDX_Control(pDX, IDC_COMBO10, blockertargetkeyselect);
}

BEGIN_MESSAGE_MAP(CSV6testDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &CSV6testDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CSV6testDlg::OnBnClickedCheck2)
	ON_EN_CHANGE(IDC_EDIT2, &CSV6testDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON2, &CSV6testDlg::OnBnClickedMfccolorbutton2)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON3, &CSV6testDlg::OnBnClickedMfccolorbutton3)
	ON_BN_CLICKED(IDC_CHECK4, &CSV6testDlg::OnBnClickedCheck4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CSV6testDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CSV6testDlg::OnNMCustomdrawSlider2)
	ON_BN_CLICKED(IDC_CHECK3, &CSV6testDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK5, &CSV6testDlg::OnBnClickedCheck5)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSV6testDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CSV6testDlg::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CSV6testDlg::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_CHECK6, &CSV6testDlg::OnBnClickedCheck6)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CSV6testDlg::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CSV6testDlg::OnCbnSelchangeCombo5)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CSV6testDlg::OnCbnSelchangeCombo6)
	ON_BN_CLICKED(IDC_CHECK7, &CSV6testDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK10, &CSV6testDlg::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK8, &CSV6testDlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, &CSV6testDlg::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK11, &CSV6testDlg::OnBnClickedCheck11)
	ON_BN_CLICKED(IDC_BUTTON1, &CSV6testDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSV6testDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSV6testDlg::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CSV6testDlg::OnCbnSelchangeCombo7)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CSV6testDlg::OnCbnSelchangeCombo8)
	ON_BN_CLICKED(IDC_CHECK12, &CSV6testDlg::OnBnClickedCheck12)
	ON_BN_CLICKED(IDC_BUTTON4, &CSV6testDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK13, &CSV6testDlg::OnBnClickedCheck13)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CSV6testDlg::OnCbnSelchangeCombo9)
	ON_BN_CLICKED(IDC_BUTTON5, &CSV6testDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK15, &CSV6testDlg::OnBnClickedCheck15)
	ON_BN_CLICKED(IDC_CHECK16, &CSV6testDlg::OnBnClickedCheck16)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CSV6testDlg::OnCbnSelchangeCombo10)
END_MESSAGE_MAP()


void CSV6testDlg::RefreshConfigs()
{
	configarray.clear();
	configsbox.ResetContent();

	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

	string cfgpath = "C:\\Configs";

	for (const auto& dirEntry : recursive_directory_iterator(cfgpath))
	{
		if (dirEntry.is_regular_file())
		{
			string fp = dirEntry.path().string();
			if (fp.find(".silverblade") != -1)
				configarray.push_back(dirEntry);
		}

	}


	for (int i = 0; i < configarray.size(); i++)
	{
		string first = configarray[i].path().string();
		int start_position_to_erase = first.find(".silverblade");
		first.erase(start_position_to_erase, 13);

		wstring sec(first.begin(), first.end());
		CString itemname = sec.c_str();
		itemname.Delete(0, 11);
		configsbox.AddString(itemname);
	}
}

void CSV6testDlg::SetDefaults()
{
	glowesptoggle.SetCheck(0);
	OnBnClickedCheck1();
	triggerbottoggle.SetCheck(0);
	OnBnClickedCheck2();
	overlayesptoggle.SetCheck(0);
	OnBnClickedCheck7();
	aimbottoggle.SetCheck(0);
	OnBnClickedCheck4();
	vischeckbox.SetCheck(0);
	OnBnClickedCheck3();
	teamcheckbox.SetCheck(0);
	OnBnClickedCheck5();
	triggeronkeybox.SetCheck(0);
	OnBnClickedCheck6();
	hpcheckbox.SetCheck(0);
	OnBnClickedCheck10();
	namecheckbox.SetCheck(0);
	OnBnClickedCheck8();
	espboxcheckbox.SetCheck(0);
	OnBnClickedCheck9();
	enemyonlycheckbox.SetCheck(0);
	OnBnClickedCheck11();
	Aimkeyselector.SetCurSel(-1);
	Globals::aimkey = VK_MENU;
	BoneSelector.SetCurSel(-1);
	Globals::aimbone = 8;
	triggerkeyselector.SetCurSel(-1);
	Globals::triggerbotkey = VK_LSHIFT;
}

// CSV6testDlg message handlers
BOOL CSV6testDlg::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();
	
	string lb = "SilverBlade by BladeMaster v";
	lb = lb + VERSION;
	wstring lb2 = wstring(lb.begin(), lb.end());
	SetWindowText(lb2.c_str());
	//SetWindowText(L"TEST");
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	fovslider.SetRangeMin(1);
	fovslider.SetRangeMax(180);

	BoneSelector.SetCurSel(1);
	Aimkeyselector.SetCurSel(0);
	triggerkeyselector.SetCurSel(2);

	CButton* vischeckbox = (CButton*)GetDlgItem(IDC_CHECK3);
	vischeckbox->SetCheck(1);// check it

	CButton* teamcheckbox = (CButton*)GetDlgItem(IDC_CHECK5);
	teamcheckbox->SetCheck(1);// check it

	
	CButton* triggermodebox = (CButton*)GetDlgItem(IDC_CHECK6);
	triggermodebox->SetCheck(0);// uncheck it

	glowesptselect.SetCurSel(Globals::oldglowsel);
	triggertselect.SetCurSel(Globals::oldtsel);
	aimbottselect.SetCurSel(Globals::oldaimsel);

	COLORREF ctcolor = RGB(Globals::GlowCounterTerroristRed, Globals::GlowCounterTerroristGreen, Globals::GlowCounterTerroristBlue);
	CTcolor.SetColor(ctcolor);

	COLORREF tcolor = RGB(Globals::GlowTerroristRed, Globals::GlowTerroristGreen, Globals::GlowTerroristBlue);
	TerroristColor.SetColor(tcolor);

	RefreshConfigs();

	menukeyselect.SetCurSel(3);

	blockertargetkeyselect.SetCurSel(1);

	chatbindselect.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSV6testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSV6testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSV6testDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//MessageBoxA(0, (LPCSTR)"And text here", (LPCSTR)"MessageBox caption", MB_OK);


	//CDialogEx::OnOK();
}



void CSV6testDlg::OnBnClickedCheck1()
{
	if (IsDlgButtonChecked(IDC_CHECK1))
	{
		glowthread = StartThread(GlowESP);
	}
	else
	{
		TerminateThread(glowthread, 0);
		CloseHandle(glowthread);
	}
}




void CSV6testDlg::OnBnClickedCheck2()
{
	if (IsDlgButtonChecked(IDC_CHECK2))
	{
		tbotthread = StartThread(Triggerbot);
	}
	else
	{
		TerminateThread(tbotthread, 0);
		CloseHandle(tbotthread);
	}
}


void CSV6testDlg::OnEnChangeEdit2()
{
	CString val;
	tbotbox.GetWindowTextW(val);
	int delay = _ttoi(val);

	Globals::triggerdelay = delay;
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CSV6testDlg::OnBnClickedMfccolorbutton2()
{
	COLORREF color = TerroristColor.GetColor();

	COLORREF otherteamcolor = RGB(Globals::GlowCounterTerroristRed, Globals::GlowCounterTerroristGreen, Globals::GlowCounterTerroristBlue);

	if (color != otherteamcolor)
	{
		RGBTRIPLE rgb;
		
		rgb.rgbtRed = GetRValue(color);
		rgb.rgbtGreen = GetGValue(color);
		rgb.rgbtBlue = GetBValue(color);

		Globals::GlowTerroristRed = rgb.rgbtRed;
		Globals::GlowTerroristGreen = rgb.rgbtGreen;
		Globals::GlowTerroristBlue = rgb.rgbtBlue;
	}
	else
	{
		COLORREF oldcolor2 = RGB(Globals::GlowTerroristRed, Globals::GlowTerroristGreen, Globals::GlowTerroristBlue);
		TerroristColor.SetColor(oldcolor2);
		MessageBoxA(0, (LPCSTR)"CT and Ts cannot have the same glow color", (LPCSTR)"Invalid color", MB_ICONWARNING);
	}
	
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnBnClickedMfccolorbutton3()
{
	COLORREF color = CTcolor.GetColor();
	
	COLORREF otherteamcolor = RGB(Globals::GlowTerroristRed, Globals::GlowTerroristGreen, Globals::GlowTerroristBlue);

	if (color != otherteamcolor)
	{
		RGBTRIPLE rgb;

		rgb.rgbtRed = GetRValue(color);
		rgb.rgbtGreen = GetGValue(color);
		rgb.rgbtBlue = GetBValue(color);

		Globals::GlowCounterTerroristRed = rgb.rgbtRed;
		Globals::GlowCounterTerroristGreen = rgb.rgbtGreen;
		Globals::GlowCounterTerroristBlue = rgb.rgbtBlue;
	}
	else
	{
		COLORREF oldcolor = RGB(Globals::GlowCounterTerroristRed, Globals::GlowCounterTerroristGreen, Globals::GlowCounterTerroristBlue);
		CTcolor.SetColor(oldcolor);
		MessageBoxA(0, (LPCSTR)"CT and Ts cannot have the same glow color", (LPCSTR)"Invalid color", MB_ICONWARNING);
	}
	
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnBnClickedCheck4()
{
	if (IsDlgButtonChecked(IDC_CHECK4))
	{
		aimthread = StartThread(Aimbot);
		entrefreshthread = StartThread(entRefresher);
	}
	else
	{
		TerminateThread(aimthread, 0);
		CloseHandle(aimthread);
		TerminateThread(entrefreshthread, 0);
		CloseHandle(entrefreshthread);
	}
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	int cFOV = fovslider.GetPos();
	if (cFOV <= 180 && cFOV > 0)
	{
		Globals::aimfov = cFOV;
	}
	

	string lb = "Aimbot FOV : " + to_string(cFOV);

	wstring wide = wstring(lb.begin(), lb.end());
	const wchar_t* result = wide.c_str();

	CWnd *label = GetDlgItem(AFX_ID_PREVIEW_PRINT);
	label->SetWindowText(result);

	*pResult = 0;
}


void CSV6testDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	int cSmooth = smoothslider.GetPos();
	float fSmooth = (float)cSmooth;
	if (cSmooth <= 100 && cSmooth >0)
	{
		Globals::aimsmooth = ((100 - fSmooth) / 100);
	}

	string text = "Aimbot smooth : " + to_string(cSmooth) + "%";

	wstring wide = wstring(text.begin(), text.end());
	const wchar_t* result = wide.c_str();

	CWnd *label = GetDlgItem(AFX_IDP_SQL_CONNECT_FAIL);
	label->SetWindowText(result);

	*pResult = 0;
}


void CSV6testDlg::OnBnClickedCheck3()
{
	if (IsDlgButtonChecked(IDC_CHECK3))
	{
		Globals::vischeckswitch = true;
	}
	else
	{
		Globals::vischeckswitch = false;
	}
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnBnClickedCheck5()
{
	Globals::teamcheck = IsDlgButtonChecked(IDC_CHECK5);
}


void CSV6testDlg::OnCbnSelchangeCombo1()
{
	int sel = BoneSelector.GetCurSel();
	switch (sel)
	{
	case 0:
		Globals::aimbone = 5;
		break;
	case 1:
		Globals::aimbone = 8;
		break;
	case 2:
		Globals::aimbone = 7;
		break;
	case 3:
		Globals::aimbone = 0;
		break;
	}
}


void CSV6testDlg::OnCbnSelchangeCombo2()
{
	int sel = Aimkeyselector.GetCurSel();
	switch (sel)
	{
	case 0:
		Globals::aimkey = VK_MENU;
		break;
	case 1:
		Globals::aimkey = VK_LSHIFT;
		break;
	case 2:
		Globals::aimkey = VK_LBUTTON;
		break;
	case 3:
		Globals::aimkey = VK_MBUTTON;
	}
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnCbnSelchangeCombo3()
{
	int sel = triggerkeyselector.GetCurSel();
	switch (sel)
	{
	case 0:
		Globals::triggerbotkey = VK_MENU;
		break;
	case 1:
		Globals::triggerbotkey = VK_LSHIFT;
		break;
	case 2:
		Globals::triggerbotkey = VK_MBUTTON;
		break;
	}
}


void CSV6testDlg::OnBnClickedCheck6()
{
	Globals::tbotmode = IsDlgButtonChecked(IDC_CHECK6);
}


void CSV6testDlg::OnCbnSelchangeCombo4()
{
	int sel = glowesptselect.GetCurSel();

	if (sel == Globals::oldaimsel || sel == Globals::oldtsel)
	{
		glowesptselect.SetCurSel(Globals::oldglowsel);
		MessageBoxA(0, (LPCSTR)"You may not have same hotkey with other toggles", (LPCSTR)"Invalid hotkey", MB_ICONWARNING);
	}
	else
	{
		switch (sel)
		{
		case 0:
			Globals::glowesptogglekey = VK_F1;
			break;
		case 1:
			Globals::glowesptogglekey = VK_F2;
			break;
		case 2:
			Globals::glowesptogglekey = VK_F3;
			break;
		case 3:
			Globals::glowesptogglekey = VK_F4;
			break;
		case 4:
			Globals::glowesptogglekey = VK_NUMPAD1;
			break;
		case 5:
			Globals::glowesptogglekey = VK_NUMPAD2;
			break;
		case 6:
			Globals::glowesptogglekey = VK_NUMPAD3;
			break;
		case 7:
			Globals::glowesptogglekey = VK_NUMPAD4;
			break;
		case 8:
			Globals::glowesptogglekey = VK_NUMPAD5;
			break;
		}
		Globals::oldglowsel = sel;
	}
	
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnCbnSelchangeCombo5()
{
	int sel = triggertselect.GetCurSel();

	if (sel == Globals::oldglowsel || sel == Globals::oldaimsel)
	{
		triggertselect.SetCurSel(Globals::oldtsel);
		MessageBoxA(0, (LPCSTR)"You may not have same hotkey with other toggles", (LPCSTR)"Invalid hotkey", MB_ICONWARNING);
	}
	else
	{
		switch (sel)
		{
		case 0:
			Globals::triggerbottogglekey = VK_F1;
			break;
		case 1:
			Globals::triggerbottogglekey = VK_F2;
			break;
		case 2:
			Globals::triggerbottogglekey = VK_F3;
			break;
		case 3:
			Globals::triggerbottogglekey = VK_F4;
			break;
		case 4:
			Globals::triggerbottogglekey = VK_NUMPAD1;
			break;
		case 5:
			Globals::triggerbottogglekey = VK_NUMPAD2;
			break;
		case 6:
			Globals::triggerbottogglekey = VK_NUMPAD3;
			break;
		case 7:
			Globals::triggerbottogglekey = VK_NUMPAD4;
			break;
		case 8:
			Globals::triggerbottogglekey = VK_NUMPAD5;
			break;
		}
		Globals::oldtsel = sel;
	}

	
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnCbnSelchangeCombo6()
{
	int sel = aimbottselect.GetCurSel();

	if (sel == Globals::oldglowsel || sel == Globals::oldtsel)
	{
		aimbottselect.SetCurSel(Globals::oldaimsel);
		MessageBoxA(0, (LPCSTR)"You may not have same hotkey with other toggles", (LPCSTR)"Invalid hotkey", MB_ICONWARNING);
	}
	else
	{
		switch (sel)
		{
		case 0:
			Globals::aimbottogglekey = VK_F1;
			break;
		case 1:
			Globals::aimbottogglekey = VK_F2;
			break;
		case 2:
			Globals::aimbottogglekey = VK_F3;
			break;
		case 3:
			Globals::aimbottogglekey = VK_F4;
			break;
		case 4:
			Globals::aimbottogglekey = VK_NUMPAD1;
			break;
		case 5:
			Globals::aimbottogglekey = VK_NUMPAD2;
			break;
		case 6:
			Globals::aimbottogglekey = VK_NUMPAD3;
			break;
		case 7:
			Globals::aimbottogglekey = VK_NUMPAD4;
			break;
		case 8:
			Globals::aimbottogglekey = VK_NUMPAD5;
			break;
		}
		Globals::oldaimsel = sel;
	}
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnBnClickedCheck7()
{
	if (IsDlgButtonChecked(IDC_CHECK7))
	{
		espoverlayinfothread = StartThread(EntityInfoThread);
		esprenderthread = StartThread(RendererThread);
	}
	else
	{
		TerminateThread(espoverlayinfothread, 0);
		CloseHandle(espoverlayinfothread);
		TerminateThread(esprenderthread, 0);
		CloseHandle(esprenderthread);
		dx_renderer clear = Globals::overlay.create_renderer();
		clear.begin_rendering();
		clear.draw_text("", 2, 2, 0xFFFFFFFF, false);
		clear.end_rendering();
	}
}


void CSV6testDlg::OnBnClickedCheck10()
{
	if (IsDlgButtonChecked(IDC_CHECK10))
		Globals::renderhps = true;
	else
		Globals::renderhps = false;
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnBnClickedCheck8()
{
	if (IsDlgButtonChecked(IDC_CHECK8))
		Globals::rendernames = true;
	else
		Globals::rendernames = false;
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnBnClickedCheck9()
{
	if (IsDlgButtonChecked(IDC_CHECK9))
		Globals::renderboxes = true;
	else
		Globals::renderboxes = false;
	// TODO: Add your control notification handler code here
}


void CSV6testDlg::OnBnClickedCheck11()
{
	if (IsDlgButtonChecked(IDC_CHECK11))
		Globals::renderonlyenemy = true;
	else
		Globals::renderonlyenemy = false;
	// TODO: Add your control notification handler code here
}



void CSV6testDlg::OnBnClickedButton1()
{
	string workfile = configarray[configselected].path().string();
	ofstream outfile(workfile);
	string configdata = "";
	configdata += "glowespswitch=" + to_string(glowesptoggle.GetCheck());
	configdata += "\n";
	configdata += "GlowTerroristRed=" + to_string(Globals::GlowTerroristRed);
	configdata += "\n";
	configdata += "GlowTerroristBlue=" + to_string(Globals::GlowTerroristBlue);
	configdata += "\n";
	configdata += "GlowTerroristGreen=" + to_string(Globals::GlowTerroristGreen);
	configdata += "\n";
	configdata += "GlowCounterTerroristRed=" + to_string(Globals::GlowCounterTerroristRed);
	configdata += "\n";
	configdata += "GlowCounterTerroristBlue=" + to_string(Globals::GlowCounterTerroristBlue);
	configdata += "\n";
	configdata += "GlowCounterTerroristGreen=" + to_string(Globals::GlowCounterTerroristGreen);
	configdata += "\n";
	configdata += "triggerbotswitch=" + to_string(triggerbottoggle.GetCheck());
	configdata += "\n";
	configdata += "triggerbotmode=" + to_string(Globals::tbotmode);
	configdata += "\n";
	configdata += "triggerbotdelay=" + to_string(Globals::triggerdelay);
	configdata += "\n";
	configdata += "triggerbotkey=" + to_string(Globals::triggerbotkey);
	configdata += "\n";
	configdata += "overlayespswitch=" + to_string(IsDlgButtonChecked(IDC_CHECK7));
	configdata += "\n";
	configdata += "overlayespnames=" + to_string(Globals::rendernames);
	configdata += "\n";
	configdata += "overlayesphp=" + to_string(Globals::renderhps);
	configdata += "\n";
	configdata += "overlayespbox=" + to_string(Globals::renderboxes);
	configdata += "\n";
	configdata += "overlayespenenemyonly=" + to_string(Globals::renderonlyenemy);
	configdata += "\n";
	configdata += "aimbotswitch=" + to_string(aimbottoggle.GetCheck());
	configdata += "\n";
	configdata += "aimbotbone=" + to_string(Globals::aimbone);
	configdata += "\n";
	configdata += "aimbotfov=" + to_string(Globals::aimfov);
	configdata += "\n";
	configdata += "aimbotpsmooth=" + to_string(smoothslider.GetPos());
	configdata += "\n";
	configdata += "aimbotkey=" + to_string(Globals::aimkey);
	configdata += "\n";
	configdata += "aimbotvischeck=" + to_string(Globals::vischeckswitch);
	configdata += "\n";
	configdata += "aimbotteamcheck=" + to_string(Globals::teamcheck);
	configdata += "\n";
	outfile << configdata;
	outfile.close();
}

vector<string> split(const string &s, char delim) {
	vector<string> result;
	stringstream ss(s);
	string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

void CSV6testDlg::OnBnClickedButton2()
{
	SetDefaults();
	string workfile = configarray[configselected].path().string();
	ifstream infile(workfile);
	string line;
	while (getline(infile, line))
	{
		vector<string> values = split(line, '=');
		if (values[0] == "glowespswitch")
		{
			if (!(glowesptoggle.GetCheck() && stoi(values[1]) == 1))
			{
				glowesptoggle.SetCheck(stoi(values[1]));
				OnBnClickedCheck1();
			}
		}
		if (values[0] == "GlowTerroristRed")
		{
			Globals::GlowTerroristRed = stof(values[1]);
		}
		if (values[0] == "GlowTerroristBlue")
		{
			Globals::GlowTerroristBlue = stof(values[1]);
		}
		if (values[0] == "GlowTerroristGreen")
		{
			Globals::GlowTerroristGreen = stof(values[1]);
		}
		if (values[0] == "GlowCounterTerroristRed")
		{
			Globals::GlowCounterTerroristRed = stof(values[1]);
		}
		if (values[0] == "GlowCounterTerroristBlue")
		{
			Globals::GlowCounterTerroristBlue = stof(values[1]);
		}
		if (values[0] == "GlowCounterTerroristGreen")
		{
			Globals::GlowCounterTerroristGreen = stof(values[1]);
		}
		if (values[0] == "triggerbotswitch")
		{
			if (!(triggerbottoggle.GetCheck() && stoi(values[1]) == 1))
			{
				triggerbottoggle.SetCheck(stoi(values[1]));
				OnBnClickedCheck2();
			}
			
		}
		if (values[0] == "triggerbotmode")
		{
			Globals::tbotmode = stoi(values[1]);
			triggeronkeybox.SetCheck(Globals::tbotmode);
		}
		if (values[0] == "triggerbotdelay")
		{
			Globals::triggerdelay = stoi(values[1]);
			wstring s(values[1].begin(), values[1].end());
			tbotbox.SetWindowTextW(s.c_str());
		}
		if (values[0] == "triggerbotkey")
		{
			Globals::triggerbotkey = stoi(values[1]);
			switch (Globals::triggerbotkey)
			{
			case VK_MENU:
				triggerkeyselector.SetCurSel(0);
				break;
			case VK_LSHIFT:
				triggerkeyselector.SetCurSel(1);
				break;
			case VK_MBUTTON:
				triggerkeyselector.SetCurSel(2);
				break;
			}
		}
		if (values[0] == "overlayespswitch")
		{
			if (!(overlayesptoggle.GetCheck() && stoi(values[1]) == 1))
			{
				overlayesptoggle.SetCheck(stoi(values[1]));
				OnBnClickedCheck7();
			}
		}
		if (values[0] == "overlayespnames")
		{
			Globals::rendernames = stoi(values[1]);
			namecheckbox.SetCheck(Globals::rendernames);
		}
		if (values[0] == "overlayesphp")
		{
			Globals::renderhps = stoi(values[1]);
			hpcheckbox.SetCheck(Globals::renderhps);
		}
		if (values[0] == "overlayespbox")
		{
			Globals::renderboxes = stoi(values[1]);
			espboxcheckbox.SetCheck(Globals::renderboxes);
		}
		if (values[0] == "overlayespenenemyonly")
		{
			Globals::renderonlyenemy = stoi(values[1]);
			enemyonlycheckbox.SetCheck(Globals::renderonlyenemy);
		}
		if (values[0] == "aimbotswitch") // nope
		{
			if ((aimbottoggle.GetCheck() == 0) && (stoi(values[1]) == 1)) // check if threads are already running, so they dont duplicate and fuck shit up
			{
				aimbottoggle.SetCheck(1);
				OnBnClickedCheck4();
			}
			if ((aimbottoggle.GetCheck() == 1) && (stoi(values[1]) == 0))
			{
				aimbottoggle.SetCheck(0);
				OnBnClickedCheck4();
			}
			
		}
		if (values[0] == "aimbotbone")
		{
			Globals::aimbone = stoi(values[1]);
			switch (Globals::aimbone)
			{
			case 5:
				BoneSelector.SetCurSel(0);
				break;
			case 8:
				BoneSelector.SetCurSel(1);
				break;
			case 7:
				BoneSelector.SetCurSel(2);
				break;
			case 0:
				BoneSelector.SetCurSel(3);
				break;
			}
		}
		if (values[0] == "aimbotfov")
		{
			Globals::aimfov = stoi(values[1]);
			fovslider.SetPos(Globals::aimfov);
		}
		/*if (values[0] == "aimbotsmooth") // no need for this, was causing trouble
		{
			Globals::aimsmooth = stoi(values[1]);
		}*/
		if (values[0] == "aimbotpsmooth")
		{
			smoothslider.SetPos(stoi(values[1]));
		}
		if (values[0] == "aimbotkey")
		{
			int ak = stoi(values[1]);
			switch (ak)
			{
			case VK_MENU:
				Aimkeyselector.SetCurSel(0);
				Globals::aimkey = VK_MENU;
				break;
			case VK_LSHIFT:
				Aimkeyselector.SetCurSel(1);
				Globals::aimkey = VK_LSHIFT;
				break;
			case VK_LBUTTON:
				Aimkeyselector.SetCurSel(2);
				Globals::aimkey = VK_LBUTTON;
				break;
			case VK_MBUTTON:
				Aimkeyselector.SetCurSel(3);
				Globals::aimkey = VK_MBUTTON;
			}
		}
		if (values[0] == "aimbotvischeck")
		{
			Globals::vischeckswitch = stoi(values[1]);
			vischeckbox.SetCheck(Globals::vischeckswitch);
		}
		if (values[0] == "aimbotteamcheck")
		{
			Globals::teamcheck = stoi(values[1]);
			teamcheckbox.SetCheck(Globals::teamcheck);
		}
	}
	COLORREF tcolor = RGB(Globals::GlowTerroristRed, Globals::GlowTerroristGreen, Globals::GlowTerroristBlue);
	TerroristColor.SetColor(tcolor);
	OnBnClickedMfccolorbutton2();

	COLORREF ctcolor = RGB(Globals::GlowCounterTerroristRed, Globals::GlowCounterTerroristGreen, Globals::GlowCounterTerroristBlue);
	CTcolor.SetColor(ctcolor);
	OnBnClickedMfccolorbutton3();

	//oldsel = sel;

}


void CSV6testDlg::OnBnClickedButton3()
{
	CString cfgname;
	newcfgname.GetWindowTextW(cfgname);
	CT2CA pszConvertedAnsiString(cfgname);
	string cfgnamestr(pszConvertedAnsiString);
	string filepath = "C:\\Configs\\" + cfgnamestr;
	ofstream cfgfile(filepath);
	cfgfile.close();
	RefreshConfigs();
}


void CSV6testDlg::OnCbnSelchangeCombo7()
{
	configselected = configsbox.GetCurSel();
}


void CSV6testDlg::OnCbnSelchangeCombo8()
{
	int sel = menukeyselect.GetCurSel();
	switch (sel)
	{
		case 3:
			Globals::menukey = VK_INSERT;
			break;
		case 2:
			Globals::menukey = VK_HOME;
			break;
		case 1:
			Globals::menukey = VK_END;
			break;
		case 0:
			Globals::menukey = VK_DELETE;
			break;
	}
}

std::wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

void CSV6testDlg::preparespam()
{
	CString msg;
	chatmessagebox.GetWindowText(msg);

	CT2CA pszConvertedAnsiString(msg);
	std::string msgstring(pszConvertedAnsiString);

	Globals::command = "say \"" + msgstring + "\"";

	if (filterbypass)
	{
		auto new_cmd = s2ws(Globals::command);
		for (size_t i = 5; i < new_cmd.size(); i++) // skip "say "
			if (i % 2)
				new_cmd.insert(i, L"\u200B");

		Globals::command = ws2s(new_cmd);
	}
}

void CSV6testDlg::OnBnClickedCheck12()
{
	if (IsDlgButtonChecked(IDC_CHECK12))
	{
		preparespam();

		commandmanagerthread = StartThread(CMThread);
	}
	else
	{
		TerminateThread(commandmanagerthread,0);
		CloseHandle(commandmanagerthread);
	}
}

void CSV6testDlg::OnBnClickedButton4()
{
	preparespam();

	sendcommand();
}


void CSV6testDlg::OnBnClickedCheck13()
{
	filterbypass = IsDlgButtonChecked(IDC_CHECK13);
}


void CSV6testDlg::OnCbnSelchangeCombo9()
{
	int sel = chatbindselect.GetCurSel();

	switch (sel)
	{
		case 0:
			Globals::spambind = 0x4B;
			break;
		case 1:
			Globals::spambind = 0x4C;
			break;
		case 2:
			Globals::spambind = 0x4F;
			break;
		case 3:
			Globals::spambind = 0x50;
			break;
	}
}


void CSV6testDlg::OnBnClickedButton5()
{
	if (getgamestate() == 6)
	{
		getrankdata();
		RankPanelDlg rankpanel;
		rankpanel.DoModal();
	}
	else
	{
		MessageBoxA(0, (LPCSTR)"You are not in a matchmaking server.", (LPCSTR)"Error", MB_ICONWARNING);
	}
}


void CSV6testDlg::OnBnClickedCheck15()
{
	Globals::enablespambind = IsDlgButtonChecked(IDC_CHECK15);
}


void CSV6testDlg::OnBnClickedCheck16()
{
	StartThread(BlockerThread);
}


void CSV6testDlg::OnCbnSelchangeCombo10()
{
	int sel = blockertargetkeyselect.GetCurSel();

	switch (sel)
	{
	case 0:
		Globals::spambind = 0x43;
		break;
	case 1:
		Globals::spambind = 0x46;
		break;
	case 2:
		Globals::spambind = 0x4A;
		break;
	}
}
