// RankPanelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SV6 test.h"
#include "RankPanelDlg.h"
#include "afxdialogex.h"
#include "Globals.h"
#include "atlstr.h"
#include <string>


// RankPanelDlg dialog

IMPLEMENT_DYNAMIC(RankPanelDlg, CDialogEx)

RankPanelDlg::RankPanelDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	
}

RankPanelDlg::~RankPanelDlg()
{

}

void RankPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, RankPreviewList);

	RankPreviewList.InsertColumn(0, _T("Player name"), LVCFMT_LEFT, 120);
	RankPreviewList.InsertColumn(1, _T("Competitive Rank"), LVCFMT_LEFT, 120);
	RankPreviewList.InsertColumn(2, _T("Competitive Wins"), LVCFMT_LEFT, 120);

	int nindex;

	for (PlayerRanking pr : Globals::playerranklist)
	{
		USES_CONVERSION;
		TCHAR stupidbitch[512];
		_tcscpy(stupidbitch, A2T(pr.Name.c_str()));
		nindex = RankPreviewList.InsertItem(0, stupidbitch);
		_tcscpy(stupidbitch, A2T(pr.Rank.c_str()));
		RankPreviewList.SetItemText(nindex, 1, stupidbitch);
		
		std::string winstr = std::to_string(pr.Wins);
		_tcscpy(stupidbitch, A2T(winstr.c_str()));

		RankPreviewList.SetItemText(nindex, 2, stupidbitch);

	}

}


BEGIN_MESSAGE_MAP(RankPanelDlg, CDialogEx)
END_MESSAGE_MAP()


// RankPanelDlg message handlers
