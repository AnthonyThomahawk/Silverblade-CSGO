#pragma once


// RankPanelDlg dialog

class RankPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RankPanelDlg)

public:
	RankPanelDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RankPanelDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl RankPreviewList;
};
