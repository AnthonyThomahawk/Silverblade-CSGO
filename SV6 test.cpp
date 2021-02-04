
// SV6 test.cpp : Defines the class behaviors for the application.
//

#include <iostream>
#include "stdafx.h"
#include "SV6 test.h"
#include "SV6 testDlg.h"
#include <iostream>
#include "Globals.h"
#include "Offsets.h"
#include "GlowESP.h"
#include "ConsoleManager.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "dxoverlay.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSV6testApp

BEGIN_MESSAGE_MAP(CSV6testApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSV6testApp construction

CSV6testApp::CSV6testApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSV6testApp object

CSV6testApp theApp;
CSV6testDlg dlg;
CWnd* menu;
bool focus = false;

DWORD WINAPI Toggler(LPVOID lpParam)
{
	while (true) // loop for toggling things
	{
		Sleep(100);
		if (GetAsyncKeyState(Globals::glowesptogglekey) & 0x8000)
		{
			if (dlg.glowesptoggle.GetCheck() == 1)
			{
				dlg.glowesptoggle.SetCheck(0);
				dlg.OnBnClickedCheck1();
			}
			else
			{
				dlg.glowesptoggle.SetCheck(1);
				dlg.OnBnClickedCheck1();
			}
			Beep(523, 500);
		}
		if (GetAsyncKeyState(Globals::triggerbottogglekey) & 0x8000)
		{
			if (dlg.triggerbottoggle.GetCheck() == 1)
			{
				dlg.triggerbottoggle.SetCheck(0);
				dlg.OnBnClickedCheck2();
			}
			else
			{
				dlg.triggerbottoggle.SetCheck(1);
				dlg.OnBnClickedCheck2();
			}
			Beep(523, 500);
		}
		if (GetAsyncKeyState(Globals::aimbottogglekey) & 0x8000)
		{
			if (dlg.aimbottoggle.GetCheck() == 1)
			{
				dlg.aimbottoggle.SetCheck(0);
				dlg.OnBnClickedCheck4();
			}
			else
			{
				dlg.aimbottoggle.SetCheck(1);
				dlg.OnBnClickedCheck4();
			}
			Beep(523, 500);
		}
		if (GetAsyncKeyState(Globals::menukey) & 0x8000)
		{
			HWND windowhandle;
			if (!focus) {
				windowhandle = menu->GetSafeHwnd();
				focus = true;
			}
				
			else {
				windowhandle = FindWindow(NULL, L"Counter-Strike: Global Offensive");
				focus = false;
			}
				

			SwitchToThisWindow(windowhandle, false);
			SetActiveWindow(windowhandle);
		}

		if ( (GetAsyncKeyState(Globals::spambind) & 0x8000) && Globals::enablespambind)
		{
			if (getgamestate() == 6)
			{
				dlg.OnBnClickedButton4();
			}
		}
	}
}

// CSV6testApp initialization
using namespace std;

string GetHWID()
{
	string ss;
	ss = "Err_StringIsNull";
	UCHAR szFileSys[255],
		szVolNameBuff[255];
	DWORD dwSerial;
	DWORD dwMFL;
	DWORD dwSysFlags;
	int error = 0;

	bool success = GetVolumeInformationA("C:\\", (LPSTR)szVolNameBuff,
		255, &dwSerial,
		&dwMFL, &dwSysFlags,
		(LPSTR)szFileSys,
		255);
	if (!success) {
		ss = "Err_Not_Elevated";
		return ss;
	}
	std::stringstream errorStream;
	errorStream << std::hex << dwSerial;
	string HWID = string(errorStream.str().c_str());
	for (int i = 0; i < HWID.length(); i++)
		HWID[i] += 6;
	return HWID;
}

vector<string> split(string s, string delimiter)
{
	vector<string> res;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		res.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	res.push_back(s);
	return res;
}

void parseoffsetfile()
{
	ifstream offsetfile("C:\\Configs\\offsets.txt");
	if (offsetfile.good())
	{
			int index = 0;
			string line;
			while (getline(offsetfile, line))
			{
				try {
					vector<string> values = split(line, "=");

					if (values.size() != 2)
						continue;

					string offinfo = values[0];
					string offset = values[1].substr(0, values[1].size() - 1);
					DWORD offsetdword = stoul(offset, nullptr, 16);

					if (offinfo.find("m_vecVelocity") != std::string::npos)
						Offsets::m_vecVelocity = offsetdword;
					else if (offinfo.find("m_vecViewOffset") != std::string::npos)
						Offsets::vecViewOffset = offsetdword;
					else if (offinfo.find("SpottedByMask") != std::string::npos)
						Offsets::SpottedByMask = offsetdword;
					else if (offinfo.find("m_dwBoneMatrix ") != std::string::npos)
						Offsets::boneMatrix = offsetdword;
					else if (offinfo.find("vecOrigin") != std::string::npos)
						Offsets::vecOrigin = offsetdword;
					else if (offinfo.find("dwClientState ") != std::string::npos)
						Offsets::clientstate = offsetdword;
					else if (offinfo.find("dwClientState_ViewAngles ") != std::string::npos)
						Offsets::clientangle = offsetdword;
					else if (offinfo.find("m_aimPunchAngle ") != std::string::npos)
						Offsets::aimPunchoffset = offsetdword;
					else if (offinfo.find("GetLocalPlayer") != std::string::npos)
						Offsets::GetLocalPlayer = offsetdword;
					else if (offinfo.find("dwEntityList") != std::string::npos)
						Offsets::dwEntityList = offsetdword;
					else if (offinfo.find("dwLocalPlayer") != std::string::npos)
						Offsets::dwLocalPlayer = offsetdword;
					else if (offinfo.find("m_iCrosshairId ") != std::string::npos)
						Offsets::dwCrosshairId = offsetdword;
					else if (offinfo.find("dwViewMatrix") != std::string::npos)
						Offsets::dwViewMatrix = offsetdword;
					else if (offinfo.find("m_iTeamNum ") != std::string::npos)
						Offsets::dwTeam = offsetdword;
					else if (offinfo.find("dwGlowObjectManager") != std::string::npos)
						Offsets::dwGlowObject = offsetdword;
					else if (offinfo.find("bDormant") != std::string::npos)
						Offsets::bDormant = offsetdword;
					else if (offinfo.find("iGlowIndex") != std::string::npos)
						Offsets::iGlowIndex = offsetdword;
					else if (offinfo.find("dwForceAttack ") != std::string::npos)
						Offsets::Attack = offsetdword;
					else if (offinfo.find("dwPlayerResource") != std::string::npos)
						Offsets::dwPlayerResource = offsetdword;
					else if (offinfo.find("dwClientState_PlayerInfo ") != std::string::npos)
						Offsets::dwClientState_PlayerInfo = offsetdword;
					else if (offinfo.find("m_iCompetitiveRanking ") != std::string::npos)
						Offsets::m_iCompetitiveRanking = offsetdword;
					else if (offinfo.find("dwGameRulesProxy") != std::string::npos)
						Offsets::dwGameRulesProxy = offsetdword;
					else if (offinfo.find("m_SurvivalGameRuleDecisionTypes") != std::string::npos)
						Offsets::m_SurvivalGameRuleDecisionTypes = offsetdword;
					else if (offinfo.find("m_iHealth  ") != std::string::npos)
						Offsets::hpoffset = offsetdword;
					index++;
				}
				catch (exception e) {
					std::cout << endl << "ERROR!!!" << endl;
					std::cout << "An error was found in file, but any correct offsets will be loaded." << endl;
					std::cout << "Exception : " << e.what() << endl;
					continue;
				}
					
			}
			cout << "Offsets have been loaded from file (offsets.txt)" << endl;
	}
	else 
	{
		cout << "Offset file (offsets.txt) does not exist in cheat config folder (C:\\Configs\\)." << endl;
	}
}

void printcurrentoffsets()
{
	cout << endl << "Current offsets used by the cheat are : " << endl;
	cout << "m_vecVelocity = 0x" << hex << Offsets::m_vecVelocity << endl;
	cout << "vecViewOffset = 0x" << hex << Offsets::vecViewOffset << endl;
	cout << "SpottedByMask = 0x" << hex << Offsets::SpottedByMask << endl;
	cout << "boneMatrix = 0x" << hex << Offsets::boneMatrix << endl;
	cout << "vecOrigin = 0x" << hex << Offsets::vecOrigin << endl;
	cout << "clientstate = 0x" << hex << Offsets::clientstate << endl;
	cout << "clientangle = 0x" << hex << Offsets::clientangle << endl;
	cout << "aimPunchoffset = 0x" << hex << Offsets::aimPunchoffset << endl;
	cout << "GetLocalPlayer = 0x" << hex << Offsets::GetLocalPlayer << endl;
	cout << "dwEntityList = 0x" << hex << Offsets::dwEntityList << endl;
	cout << "dwLocalPlayer = 0x" << hex << Offsets::dwLocalPlayer << endl;
	cout << "dwCrosshairId = 0x" << hex << Offsets::dwCrosshairId << endl;
	cout << "dwViewMatrix = 0x" << hex << Offsets::dwViewMatrix << endl;
	cout << "dwTeam = 0x" << hex << Offsets::dwTeam << endl;
	cout << "hpoffset = 0x" << hex << Offsets::hpoffset << endl;
	cout << "dwGlowObject = 0x" << hex << Offsets::dwGlowObject << endl;
	cout << "bDormant = 0x" << hex << Offsets::bDormant << endl;
	cout << "iGlowIndex = 0x" << hex << Offsets::iGlowIndex << endl;
	cout << "Attack = 0x" << hex << Offsets::Attack << endl;
	cout << "dwPlayerResource = 0x" << hex << Offsets::dwPlayerResource << endl;
	cout << "dwClientState_PlayerInfo = 0x" << hex << Offsets::dwClientState_PlayerInfo << endl;
	cout << "m_iCompetitiveRanking = 0x" << hex << Offsets::m_iCompetitiveRanking << endl;
	cout << "m_iCompetitiveWins = 0x" << hex << Offsets::m_iCompetitiveWins << endl;
	cout << "dwGameRulesProxy = 0x" << hex << Offsets::dwGameRulesProxy << endl;
	cout << "m_SurvivalGameRuleDecisionTypes = 0x" << hex << Offsets::m_SurvivalGameRuleDecisionTypes << endl;
	cout << "m_nSurvivalTeam = 0x" << hex << Offsets::m_nSurvivalTeam << endl << endl;
}

BOOL CSV6testApp::InitInstance()
{
	string hwid = GetHWID();
	string lock = ""; // Insert your HWID here, that you get by running simple HWID for windows (link in github repo)
	if (hwid!=lock)
	{
		MessageBoxA(0, (LPCSTR)"This app is not authorized to run on your PC", (LPCSTR)"HWID lock", MB_ICONWARNING);
		return 0;
	}
	string hwiduser = "AnthonyT"; // Desired username
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
	freopen("CON", "r", stdin); // allow reading in console (solution by bladeZZZ)
	string lb = "SilverBlade by BladeMaster v";
	lb = lb + VERSION;
	wstring lb2 = wstring(lb.begin(), lb.end());

	SetConsoleTitle(lb2.c_str());

	string lb3 = lb + NOTES;
	printf(lb3.c_str());
	printf("\n");
	printf("Build date: %s\n", __DATE__);
	printf("Build time: %s\n", __TIME__);
	cout << "HWID : " << hwid << " has been authorised, welcome back " << hwiduser << endl;
	WCHAR exepath[MAX_PATH];
	GetModuleFileNameW(NULL, exepath, MAX_PATH);
	wcout << "Cheat is running from : " << exepath << endl;
	cout << "PAULI ON PASKA LOLZ" << endl;

	parseoffsetfile();
	printcurrentoffsets();

	printf("waiting to launch game......\n");

	wchar_t process[] = L"csgo.exe";
	while (true)
	{
		Globals::gamememory.SetProcess(process);
		if (Globals::gamememory.pHandle != 0)
			break;
		Sleep(100);
	}

	Globals::gamememory.SetProcess(process);
	printf("CSGO process handle:%d\n", Globals::gamememory.pHandle);
	wchar_t gamemodule[] = L"client.dll";
	while (true)
	{
		Globals::dwClient = Globals::gamememory.GetModuleHandleByName(gamemodule);
		Globals::clientmodule = Globals::gamememory.GetModule(gamemodule);
		if (Globals::dwClient != 0 && Globals::dwClient != 1)
			break;
	}
	printf("Module client.dll handle:0x%x\n", Globals::dwClient);
	{
		wchar_t gamemodule2[] = L"engine.dll";
		while (true)
		{
			Globals::dwEngine = Globals::gamememory.GetModuleHandleByName(gamemodule2);
			if (Globals::dwEngine != 0 && Globals::dwEngine != 1)
				break;
		}
		printf("Engine module handle (engine.dll) : 0x%x\n ", Globals::dwEngine);
		Offsets::clientbase = Globals::gamememory.Read<DWORD>(Globals::dwEngine + Offsets::clientstate);
		printf("client base : 0x%x\n", Offsets::clientbase);

		cout << "Game process and modules found, opening GUI"<<endl;
		HANDLE togglerthread = StartThread(Toggler);
		
		Globals::overlay.initoverlay(L"Valve001", L"Counter-Strike: Global Offensive", false);

		// InitCommonControlsEx() is required on Windows XP if an application
		// manifest specifies use of ComCtl32.dll version 6 or later to enable
		// visual styles.  Otherwise, any window creation will fail.
		INITCOMMONCONTROLSEX InitCtrls;
		InitCtrls.dwSize = sizeof(InitCtrls);
		// Set this to include all the common control classes you want to use
		// in your application.
		InitCtrls.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&InitCtrls);

		CWinApp::InitInstance();


		AfxEnableControlContainer();

		// Create the shell manager, in case the dialog contains
		// any shell tree view or shell list view controls.
		CShellManager *pShellManager = new CShellManager;

		// Activate "Windows Native" visual manager for enabling themes in MFC controls
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

		// Standard initialization
		// If you are not using these features and wish to reduce the size
		// of your final executable, you should remove from the following
		// the specific initialization routines you do not need
		// Change the registry key under which our settings are stored
		// TODO: You should modify this string to be something appropriate
		// such as the name of your company or organization
		SetRegistryKey(_T("Local AppWizard-Generated Applications"));


		
		m_pMainWnd = &dlg;
		menu = m_pMainWnd;
		
		INT_PTR nResponse = dlg.DoModal();
		
		
		
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
			TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
		}

		// Delete the shell manager created above.
		if (pShellManager != nullptr)
		{
			delete pShellManager;
		}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
		ControlBarCleanUp();
#endif

		// Since the dialog has been closed, return FALSE so that we exit the
		//  application, rather than start the application's message pump.
		return FALSE;
	}
}
