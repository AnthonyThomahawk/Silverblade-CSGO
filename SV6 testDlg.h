
// SV6 testDlg.h : header file
//

#pragma once


// CSV6testDlg dialog
class CSV6testDlg : public CDialogEx
{
// Construction
public:
	CSV6testDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SV6TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	BOOL OnActivate();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnEnChangeEdit2();
	CEdit tbotbox;
	afx_msg void OnBnClickedMfccolorbutton2();
	CMFCColorButton TerroristColor;
	CMFCColorButton CTcolor;
	afx_msg void OnBnClickedMfccolorbutton3();
	afx_msg void OnBnClickedCheck4();
	CSliderCtrl fovslider;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl smoothslider;
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox BoneSelector;
	afx_msg void OnCbnSelchangeCombo2();
	CComboBox Aimkeyselector;
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnBnClickedCheck6();
	CComboBox triggerkeyselector;
	CButton glowesptoggle;
	CButton aimbottoggle;
	CButton triggerbottoggle;
	afx_msg void OnCbnSelchangeCombo4();
	CComboBox glowesptselect;
	afx_msg void OnCbnSelchangeCombo5();
	CComboBox triggertselect;
	CComboBox aimbottselect;
	afx_msg void OnCbnSelchangeCombo6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck11();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CComboBox configsbox;
	void RefreshConfigs();
	void SetDefaults();
	CEdit newcfgname;
	CButton overlayesptoggle;
	CButton hpcheckbox;
	CButton namecheckbox;
	CButton espboxcheckbox;
	CButton enemyonlycheckbox;
	CButton vischeckbox;
	CButton teamcheckbox;
	CButton triggeronkeybox;
	afx_msg void OnCbnSelchangeCombo7();
	afx_msg void OnCbnSelchangeCombo8();
	CComboBox menukeyselect;
	afx_msg
		void preparespam();
	void OnBnClickedCheck12();
	afx_msg void OnBnClickedButton4();
	CEdit chatmessagebox;
	afx_msg void OnBnClickedCheck13();
	CButton bypassfiltercheckbox;
	afx_msg void OnCbnSelchangeCombo9();
	CComboBox chatbindselect;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedCheck15();
	afx_msg void OnBnClickedCheck16();
	afx_msg void OnCbnSelchangeCombo10();
	CComboBox blockertargetkeyselect;
};
