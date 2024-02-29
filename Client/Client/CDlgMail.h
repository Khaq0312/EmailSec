#pragma once
#include "afxdialogex.h"
#include "framework.h"

// CDlgMail dialog

class CDlgMail : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMail)

public:
	CDlgMail(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgMail();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL CDlgMail::OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_content;
	afx_msg void OnBnClickedBack();
};
