// CDlgMail.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "CDlgMail.h"
#include "ClientDlg.h"

// CDlgMail dialog

IMPLEMENT_DYNAMIC(CDlgMail, CDialogEx)

CDlgMail::CDlgMail(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CDlgMail::~CDlgMail()
{
}

void CDlgMail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTENT, m_content);
}


BEGIN_MESSAGE_MAP(CDlgMail, CDialogEx)
	ON_BN_CLICKED(IDC_BACK, &CDlgMail::OnBnClickedBack)
END_MESSAGE_MAP()

BOOL CDlgMail::OnInitDialog() {

	CDialog::OnInitDialog();

	recvMessage = recv(client, buffer, BUF_SIZE, 0);
	buffer[recvMessage] = '\0';
	CString temp(buffer);
	m_content.SetWindowTextW(temp);

	return TRUE;
}
// CDlgMail message handlers


void CDlgMail::OnBnClickedBack()
{
	// TODO: Add your control notification handler code here
	EndDialog(IDD_DIALOG2);
	CDlgFeature ft;
	ft.DoModal();
}
