// CDlgFeature.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "ClientDlg.h"
#include "CDlgMail.h"

// CDlgFeature dialog

IMPLEMENT_DYNAMIC(CDlgFeature, CDialogEx)

CDlgFeature::CDlgFeature(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CDlgFeature::~CDlgFeature()
{
}

void CDlgFeature::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INBOX, m_inbox);
	DDX_Control(pDX, IDC_COMPOSE_VIEW, m_compose_view);
	DDX_Control(pDX, IDC_IBX_VIEW, m_inbox_view);
	DDX_Control(pDX, IDC_COMPOSE, m_compose_box);
}


BOOL CDlgFeature::OnInitDialog() {
	
	CDialog::OnInitDialog();
	
	ShowHideControls(IDC_COMPOSE, 0);
	ShowHideControls(IDC_INBOX, 1);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgFeature, CDialogEx)
	ON_LBN_SELCHANGE(IDC_INBOX, &CDlgFeature::OnLbnSelchangeInbox)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOGOUT, &CDlgFeature::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_COMPOSE_VIEW, &CDlgFeature::OnBnClickedComposeView)
	ON_BN_CLICKED(IDC_IBX_VIEW, &CDlgFeature::OnBnClickedIbxView)
END_MESSAGE_MAP()


void CDlgFeature::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		/*CRect rect;
		GetClientRect(&rect);*/
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// Title
		CFont font;
		font.CreateFontW(25, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
		CFont* pFont = dc.SelectObject(&font);
		dc.SetTextColor(RGB(255, 62, 76));
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextAlign(TA_CENTER);
		dc.SetTextAlign(TA_CENTER);
		dc.TextOut(650, 25, L""+username);

		dc.SelectObject(pFont);
		font.DeleteObject();

		font.CreateFontW(25, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
		/*GetDlgItem(IDC_STATIC_NAME)->SetFont(&font);
		GetDlgItem(IDC_STATIC_PW)->SetFont(&font);*/
		font.DeleteObject();
		CDialogEx::OnPaint();
	}
}
// CDlgFeature message handlers


void CDlgFeature::OnLbnSelchangeInbox()
{
	// TODO: Add your control notification handler code here
	int selectedIndex = m_inbox.GetCurSel();
	if (selectedIndex != LB_ERR) {
		CString selected;
		m_inbox.GetText(selectedIndex, selected);
		send(client, "RETR", 4, 0);
		MessageBox(_T("You choose: ") + selected);
		
		send(client, CStringA(selected), strlen(CStringA(selected)), 0);
		EndDialog(IDD_DIALOG1);
		CDlgMail mail;
		mail.DoModal();
		
	}
}


void CDlgFeature::OnBnClickedLogout()
{
	// TODO: Add your control notification handler code here
	send(client, "RSET", 4, 0);
	EndDialog(IDD_DIALOG1);
	CClientDlg c;
	c.DoModal();
}


void CDlgFeature::OnBnClickedComposeView()
{
	// TODO: Add your control notification handler code here
	ShowHideControls(IDC_COMPOSE, 0);
	ShowHideControls(IDC_INBOX, 1);

}


void CDlgFeature::OnBnClickedIbxView()
{
	// TODO: Add your control notification handler code here
	send(client, "LIST", 4, 0);
	recvMessage = recv(client, buffer, BUF_SIZE, 0);
	buffer[recvMessage] = '\0';
	m_inbox.ResetContent();

	int n = atoi(buffer);
	if (n != 0)
	{
		for (int i = 0; i < n; i++)
		{
			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';

			CString str(buffer);
			m_inbox.AddString(str);
		}
	}
	ShowHideControls(IDC_COMPOSE, 1); 
	ShowHideControls(IDC_INBOX, 0);

}

void CDlgFeature::ShowHideControls(UINT id, BOOL hide)
{
	switch (id) {
	case IDC_COMPOSE:
	{
		GetDlgItem(IDC_COMPOSE)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_TO)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_TO_INPUT)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_SUBJECT)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_SUBJECT_INPUT)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_CONTENT)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_CONTENT_INPUT)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_SEND)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);

	}
	break;
	case IDC_INBOX:
	{
		GetDlgItem(IDC_IBX_FRAME)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_INBOX)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
	}
	break;
	}
}


