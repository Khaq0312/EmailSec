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

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL CDlgFeature::OnInitDialog();
	afx_msg void OnPaint();
	void CDlgFeature::OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	HICON m_hIcon;
	BOOL CDlgFeature::PreTranslateMessage(MSG* pMsg);

	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLbnSelchangeInbox();
	CListBox m_inbox;
	afx_msg void OnBnClickedLogout();
	CButton m_compose_view;
	CButton m_inbox_view;
	afx_msg void OnBnClickedComposeView();
	afx_msg void OnBnClickedIbxView();
	CStatic m_compose_box;
	void CDlgFeature::ShowHideControls(UINT id, BOOL hide);

	CEdit m_to_input;
	CEdit m_subject_input;
	CEdit m_content_input;
	afx_msg void OnBnClickedSend();
	CButton sent;
	afx_msg void OnBnClickedsent();
	afx_msg void OnLbnSelchangesentbox();
	CListBox m_sentbox;
};
