#pragma once
#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "CDlgMail.h"
#include "ClientDlg.h"
#include "CDlgFeature.h"


// CDlgSentBox dialog

class CDlgSentBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSentBox)

public:
	CDlgSentBox(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSentBox();
	void CDlgSentBox::OnCancel();
	BOOL CDlgSentBox::OnInitDialog();
	std::string filename;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBack();
	CStatic m_content;
};
