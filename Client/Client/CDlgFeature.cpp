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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	DDX_Control(pDX, IDC_TO_INPUT, m_to_input);
	DDX_Control(pDX, IDC_SUBJECT_INPUT, m_subject_input);
	DDX_Control(pDX, IDC_CONTENT_INPUT, m_content_input);
}


BOOL CDlgFeature::OnInitDialog() {
	
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);
	
	ShowHideControls(IDC_COMPOSE, 0);
	ShowHideControls(IDC_INBOX, 1);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgFeature, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOGOUT, &CDlgFeature::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_COMPOSE_VIEW, &CDlgFeature::OnBnClickedComposeView)
	ON_BN_CLICKED(IDC_IBX_VIEW, &CDlgFeature::OnBnClickedIbxView)
	ON_BN_CLICKED(IDC_SEND, &CDlgFeature::OnBnClickedSend)
	ON_LBN_SELCHANGE(IDC_INBOX, &CDlgFeature::OnLbnSelchangeInbox)
	//ON_EN_CHANGE(IDC_CONTENT_INPUT, &CDlgFeature::OnEnChangeContentInput)
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
		font.DeleteObject();
		CDialogEx::OnPaint();
	}
}

HCURSOR CDlgFeature::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// CDlgFeature message handlers


void CDlgFeature::OnLbnSelchangeInbox()
{
	// TODO: Add your control notification handler code here
	int selectedIndex = m_inbox.GetCurSel();
	if (selectedIndex != LB_ERR) {
		CString selected;
		m_inbox.GetText(selectedIndex, selected);

		int nameIndex = -1;
		for (int i = 0; i < users.size(); i++) {
			if (users[i].name_displayed == std::string(CT2A(selected))) {
				nameIndex = i;
				break;
			}
		}

		if (nameIndex != -1)
		{
			send(client, "RETR", 4, 0);
			std::string message = "You choose: " + users[nameIndex].name_displayed;
			CString temp(message.c_str());
			MessageBox(temp);

			send(client, users[nameIndex].file_name.c_str(), strlen(users[nameIndex].file_name.c_str()), 0);
			EndDialog(IDD_DIALOG1);
		}
		
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

	int n = atoi(buffer);

	send(client, "OK", 2, 0);

	m_inbox.ResetContent();
	if (n != 0)
	{
		for (int i = 0; i < n; i++)
		{
			//real file name
			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
			send(client, "OK", 2, 0);
			std::string file_name_temp = std::string(buffer);

			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
			send(client, "OK", 2, 0);
			std::string name_displayed_temp = std::string(buffer);
		

			users.push_back( { file_name_temp, name_displayed_temp });
			/*CString str(buffer);
			m_inbox.AddString(str);*/
			int index = m_inbox.InsertString(-1, CString(name_displayed_temp.c_str()));
			
		}
	}
	m_inbox.ShowWindow(SW_SHOW);
	m_inbox.Invalidate();
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

//handle shift enter in content edit control
BOOL CDlgFeature::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && GetKeyState(VK_SHIFT) < 0) {
		if (pMsg->wParam == VK_RETURN) {
			CWnd* pFocusWnd = GetFocus();
			if (pFocusWnd && pFocusWnd->GetDlgCtrlID() == IDC_CONTENT_INPUT) {
				CEdit* pEdit = (CEdit*)pFocusWnd;
				pEdit->ReplaceSel(_T("\r\n"));
				return TRUE;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFeature::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	auto now = std::chrono::system_clock::now();

	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	tm localTime;
	localtime_s(&localTime, &now_time);

	CString text;
	m_to_input.GetWindowTextW(text);

	std::string to_user = std::string(CT2A(text));

	m_subject_input.GetWindowTextW(text);
	std::string subject = std::string(CT2A(text));


	m_content_input.GetWindowTextW(text);
	std::string content = std::string(CT2A(text));

	/*std::string data = "From: " + std::string(CT2A(username)) + "\nTo: "
				+ to_user + "\nSubject: " + subject + "\nContent: " + content;*/

	if (to_user.empty() || subject.empty() || content.empty())
	{
		MessageBox(_T("Fields must not be emptied"));
	}
	else
	{
		bool sentSuccessfully = false;
		while (!sentSuccessfully)
		{
			
			//mail from:...
			std::string command = "MAIL FROM:<" + std::string(CT2A(username)) + '>';
			send(client, command.c_str(), strlen(command.c_str()), 0);
			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';

			//rcpt to: user
			command = "RCPT TO:<" + to_user + '>';
			send(client, command.c_str(), strlen(command.c_str()), 0);
			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';

			// Check if the server response is "250 OK"
			if (std::string(buffer) == reply_code[6])
			{
				std::string month, day, hour, min, sec;
				month = std::to_string(localTime.tm_mon + 1);
				day = std::to_string(localTime.tm_mday);
				hour = std::to_string(localTime.tm_hour);
				min = std::to_string(localTime.tm_min);
				sec = std::to_string(localTime.tm_sec);
				if (month.length() == 1)
					month = '0' + month;
				if (day.length() == 1)
					day = '0' + day;
				if (hour.length() == 1)
					hour = '0' + hour;
				if (min.length() == 1)
					min = '0' + min;
				if (sec.length() == 1)
					sec = '0' + sec;
				std::string time = std::to_string(localTime.tm_year + 1900) + "/"
					+ month + "/" + day	+ " "+ hour + ":" + min + ":" + sec ;

				std::string title = time + "\nFrom: " + std::string(CT2A(username)) 
										+ "\nTo: " + to_user + "\nSubject: " + subject 
										+ "\nContent: " + content;
				// Data
				send(client, "DATA", 4, 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';


				send(client, title.c_str(), strlen(title.c_str()), 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';

				/*data = time + '\n' + data;
				send(client, data.c_str(), strlen(data.c_str()), 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';*/

				MessageBox(_T("Message sent"));
				sentSuccessfully = true;

				// Reset interface fields
				m_to_input.SetWindowTextW(_T(""));
				m_subject_input.SetWindowTextW(_T(""));
				m_content_input.SetWindowTextW(_T(""));
			}
			else
			{
				// Server response is not "250 OK"
				MessageBox(_T("Recipient email address is invalid. Please enter again."));
				// Clear the recipient email field and wait for the user to re-enter
				m_to_input.SetWindowTextW(_T(""));
				m_to_input.GetWindowTextW(text);
				to_user = std::string(CT2A(text));
				if (to_user.empty())
				{
					// If the user cancels re-entering the email, break the loop
					break;
				}
			}
		}
	}
}
