#pragma once
#include "afxdialogex.h"


// CDlgFeature dialog

class CDlgFeature : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFeature)

public:
	CDlgFeature(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgFeature();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL CDlgFeature::OnInitDialog();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeInbox();
	CListBox m_inbox;
	afx_msg void OnBnClickedLogout();
	CButton m_compose_view;
	CButton m_inbox_view;
	afx_msg void OnBnClickedComposeView();
	afx_msg void OnBnClickedIbxView();
	CStatic m_compose_box;
	void CDlgFeature::ShowHideControls(UINT id, BOOL hide);

	afx_msg void OnStnClickedSubject();
	afx_msg void OnBnClickedIbxFrame();
};
