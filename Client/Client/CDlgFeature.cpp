// CDlgFeature.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "ClientDlg.h"
#include "CDlgMail.h"
#include "CDlgSentBox.h"
#include "function.h"
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
	DDX_Control(pDX, IDC_sent, sent);
	DDX_Control(pDX, IDC_sentbox, m_sentbox);
	DDX_Control(pDX, IDC_spambox, m_spambox);
}


BOOL CDlgFeature::OnInitDialog() {
	
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);
	
	ShowHideControls(IDC_COMPOSE, 0);
	ShowHideControls(IDC_INBOX, 1);
	ShowHideControls(IDC_groupsent, 1);


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
	ON_COMMAND(IDCANCEL, &CDlgFeature::OnCancel)
	ON_BN_CLICKED(IDC_sent, &CDlgFeature::OnBnClickedsent)
	ON_LBN_SELCHANGE(IDC_sentbox, &CDlgFeature::OnLbnSelchangesentbox)
	ON_BN_CLICKED(IDC_SPAM, &CDlgFeature::OnBnClickedSpam)
	ON_LBN_SELCHANGE(IDC_spambox, &CDlgFeature::OnLbnSelchangespambox)
END_MESSAGE_MAP()


void CDlgFeature::OnCancel() {
	CDialogEx::OnCancel();
	send(client, "QUIT", 4, 0);
	closesocket(client);

}
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
	ShowHideControls(IDC_groupsent, 1);
	ShowHideControls(IDC_SPAM, 1);

}


void CDlgFeature::OnLbnSelchangeInbox()
{
	CDlgMail mail;

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
			std::string message = "You choose: " + users[nameIndex].name_displayed;
			CString temp(message.c_str());
			MessageBox(temp);

			mail.file = users[nameIndex].file_name;
			mail.box_type = "Inbox";
			EndDialog(IDD_DIALOG1);
		}
		mail.DoModal();
	}
}

void CDlgFeature::OnBnClickedIbxView()
{
	// TODO: Add your control notification handler code here
	send(client, "LIST", 4, 0);
	recvMessage = recv(client, buffer, BUF_SIZE, 0);
	buffer[recvMessage] = '\0';

	int n = atoi(buffer);
	char byte;
	std::fstream file;

	send(client, "OK", 2, 0);
	std::vector<std::string> file_name_temp;
	file_name_temp.clear();
	bool has_dkim = 0;
	for (int i = 0; i < n; ++i)
	{
		recvMessage = recv(client, buffer, BUF_SIZE, 0);
		buffer[recvMessage] = '\0';
		send(client, "OK", 2, 0);

		if (has_dkim)
		{
			has_dkim = 0;
			continue;	
		}
		
		std::string a = std::string(buffer);
		int underscore = a.find("_");
		if (underscore != std::string::npos)
		{
			has_dkim = 1;
		}
		else
		{
			has_dkim = 0;
		}
		file_name_temp.push_back(std::string(buffer));
	
	}
	n = file_name_temp.size();
	m_inbox.ResetContent();
	if (n != 0)
	{
		for (int i = 0; i < n; ++i)
		{
			//retrieve
			send(client, "RETR", 4, 0);
			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';

			send(client, file_name_temp[i].c_str(), strlen(file_name_temp[i].c_str()), 0);
			
			//spam or not spam dkim
			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
			std::string spam_dkim = std::string(buffer);
			send(client, "OK", 2, 0);


			//get ciphertext and aeskey
			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
			std::string cipher = std::string(buffer);
			send(client, "OK", 2, 0);

			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
			std::string aes_key = std::string(buffer);
			send(client, "OK", 2, 0);

			std::string userRoot = std::string(CT2A(username));
			fs::current_path(userRoot);

			
			if (!fs::exists("Inbox"))
			{
				fs::create_directory("Inbox");
			}
			if (!fs::exists("Spam"))
			{
				fs::create_directory("Spam");
			}

			file.open("sender_cipher.enc", std::ios::out);
			if (file.is_open())
			{
				file << cipher;
				file.close();
			}

			file.open("sender_aes_key.enc", std::ios::out);
			if (file.is_open())
			{
				file << aes_key;
				file.close();
			}

			std::string command =
				"openssl smime -decrypt -in ./sender_cipher.enc -recip ./mycert.crt -inkey ./private.key -out ./decrypted.txt && "
				"openssl base64 -d -in sender_aes_key.enc -out temp.enc &&"//aes key encrypted
				"openssl rsautl -decrypt -in temp.enc -out sender_aes_key.txt -inkey private.key && "
				"openssl enc -d -aes-128-cbc -in decrypted.txt -out sender_signed.txt -pass file:sender_aes_key.txt -base64";//get the sign

			std::system(command.c_str());


			std::string content, temp = "";
			file.open("sender_signed.txt", std::ios::in);
			if (file.is_open())
			{
				while (file.get(byte))
				{
					content += byte;
				}
				file.close();
			}
			size_t pos = content.find("From:");
			if (pos != std::string::npos)
			{
				pos += 6;
				while (content[pos] != '\r') {
					temp += content[pos];
					pos++;
				}
			}

			send(client, temp.c_str(), strlen(temp.c_str()), 0);
			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';

			std::string certificate = temp + "Cert.crt";
			command = "openssl smime -verify -in ./sender_signed.txt -CAfile " + certificate + " -out verify.txt";
			std::system(command.c_str());

			temp = "";
			file.open("verify.txt", std::ios::in);
			if (file.is_open())
			{
				while (file.get(byte))
				{
					temp += byte;
				}
				file.close();
			}

			//spam or not spam spf
			recvMessage = recv(client, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
			std::string spam_spf = std::string(buffer);
			send(client, "OK", 2, 0);

			if (spam_dkim == "SPAM" || spam_spf == "SPAM")
			{
				fs::current_path("Spam");
			}
			if(spam_dkim == "NOT SPAM" && spam_spf == "NOT SPAM")
			{
				fs::current_path("Inbox");
			}
			
			file.open(file_name_temp[i], std::ios::out);
			if (file.is_open())
			{
				file << temp;
				file.close();
			}
			fs::current_path("..");
			fs::current_path("..");

		}
	
	}

	std::string folder_path = std::string(CT2A(username)) + "\\Inbox";
	if (fs::exists(folder_path))
	{
		std::vector<std::string> filename;
		for (const auto& entry : fs::directory_iterator(folder_path)) {
			if (fs::is_regular_file(entry)) {
				filename.push_back(entry.path().filename().string());
			}
		}

		//std::reserve(filename.begin(), filename.end());
		users.clear();
		for (int i = 0; i < filename.size(); ++i)
		{
			std::string title = "";

			file.open(folder_path + "\\" + filename[i], std::ios::in);
			if (file.is_open())
			{
				std::string from, to, date, subject;
				getline(file, date);
				getline(file, from);
				getline(file, to);
				getline(file, subject);
				date = date.substr(0, date.length() - 1);
				from = from.substr(0, from.length() - 1);
				subject = subject.substr(0, subject.length() - 1);

				title = date + "        " + from + "           " + subject;
				users.push_back({ filename[i], title });
				file.close();
			}
			int index = m_inbox.InsertString(0, CString(title.c_str()));

		}
		
	}
	m_inbox.ShowWindow(SW_SHOW);
	m_inbox.Invalidate();
	ShowHideControls(IDC_COMPOSE, 1); 
	ShowHideControls(IDC_INBOX, 0);
	ShowHideControls(IDC_groupsent, 1);
	ShowHideControls(IDC_SPAM, 1);


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
	case IDC_groupsent:
	{
		GetDlgItem(IDC_groupsent)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_sentbox)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
	}
	case IDC_SPAM:
	{
		GetDlgItem(IDC_spambox)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
		GetDlgItem(IDC_Spam_view)->ShowWindow(hide ? SW_HIDE : SW_NORMAL);
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
	std::string sender = std::string(CT2A(username));
	
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
					+ month + "/" + day + " " + hour + ":" + min + ":" + sec;

				std::string header = time + "\nFrom: " + std::string(CT2A(username))
					+ "\nTo: " + to_user + "\nSubject: " + subject;
				std::string body = "\nContent: " + content;


				// Data
				send(client, "DATA", 4, 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';

				send(client, header.c_str(), strlen(header.c_str()), 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';

				send(client, body.c_str(), strlen(body.c_str()), 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';

				send(client, sender.c_str(), strlen(sender.c_str()), 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';

				send(client, to_user.c_str(), strlen(to_user.c_str()), 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';
				//smtp send
				/*send(client, title.c_str(), strlen(title.c_str()), 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';*/

				//smime
				std::string userRoot = std::string(CT2A(username));
				std::string to_userRoot = to_user;
				std::string command;
				std::fstream file;
				std::string publickey, certificate;
				char byte;

				//to_user root dir
				if (!fs::exists(to_userRoot))
				{
					fs::create_directory(to_userRoot);  //create folder
				}
				if (fs::exists(to_userRoot) && fs::is_directory(to_userRoot))
				{
					fs::current_path(to_userRoot);
					//check if no privateKey created
					if (!fs::exists("private.key"))
					{
						//create private key
						std::string command = "openssl genrsa -out ./private.key 2048 && "
							"openssl rsa -in ./private.key -pubout -out ./public.key && "
							"openssl req -new -out ./myreq.csr -key ./private.key -subj \"/C=AU/ST=Some-State/O=Internet Widgits Pty Ltd\" && "
							"openssl genrsa -out ./ca.key 2048 && "
							"openssl req -new -x509 -days 365 -key ./ca.key -out ./ca.crt -subj \"/C=US/ST=State/L=City/O=Organization/OU=Organizational Unit/CN=Common Name\" && "
							"openssl x509 -req -in ./myreq.csr -CA ./ca.crt -CAkey ./ca.key -CAcreateserial -out ./mycert.crt -days 60 && "
							"openssl rand -base64 16 > ./aes_key.txt";
						std::system(command.c_str());

					}

					//get to_user pubkey
					file.open("mycert.crt", std::ios::in);
					if (file.is_open()) {
						while (file.get(byte))
						{
							certificate += byte;
						}
						file.close();
					}
					file.open("public.key", std::ios::in);
					if (file.is_open()) {
						while (file.get(byte))
						{
							publickey += byte;
						}
						file.close();
					}
				}


				//change to_user_dir to user_dir
				fs::current_path("..");

				//user_dir
				if (!fs::exists(userRoot))
				{
					//create folder
					fs::create_directory(userRoot);
				}
				if (fs::exists(userRoot) && fs::is_directory(userRoot))
				{
					fs::current_path(userRoot);
					//check if no privateKey created
					if (!fs::exists("private.key"))
					{
						//create private key

						command = "openssl genrsa -out ./private.key 2048 && "
							"openssl rsa -in ./private.key -pubout -out ./public.key && "
							"openssl req -new -out ./myreq.csr -key ./private.key -subj \"/C=AU/ST=Some-State/O=Internet Widgits Pty Ltd\" && "
							"openssl genrsa -out ./ca.key 2048 && "
							"openssl req -new -x509 -days 365 -key ./ca.key -out ./ca.crt -subj \"/C=US/ST=State/L=City/O=Organization/OU=Organizational Unit/CN=Common Name\" && "
							"openssl x509 -req -in ./myreq.csr -CA ./ca.crt -CAkey ./ca.key -CAcreateserial -out ./mycert.crt -days 60 && "
							"openssl rand -base64 16 > ./aes_key.txt";
						std::system(command.c_str());
					}
				}

				file.open("message.txt", std::ios::out);
				if (file.is_open())
				{
					file << header;
					file << body;
					file.close();
				}
				std::string userCert = to_user + "Cert.crt";
				file.open(userCert, std::ios::out);
				if (file.is_open())
				{
					file << certificate;
					file.close();
				}

				std::string userKey = to_user + "Pub.key";
				file.open(userKey, std::ios::out);
				if (file.is_open())
				{
					file << publickey;
					file.close();
				}

				command =
					"openssl smime -sign -in ./message.txt -signer ./mycert.crt -inkey ./private.key -out ./signed.sign && "//sign
					"openssl enc -aes-128-cbc -in signed.sign -out aes_signed.enc -pass file:aes_key.txt -base64 && "//enc Signed
					//"openssl rsautl -encrypt -pubin -inkey ./" + userKey + " -in aes_key.txt -out aes_key.enc &&"
					"openssl rsautl -encrypt -pubin -inkey "+ userKey + " -in aes_key.txt -out temp.enc &&"//encrypted aesKey
					"openssl base64 -in temp.enc -out aes_key.enc &&"
					"openssl smime -encrypt -in ./aes_signed.enc -out ./cipher.enc ./" + userCert;
				std::system(command.c_str());


				std::string cipher, aeskey;
				file.open("cipher.enc", std::ios::in);
				if (file.is_open()) {
					while (file.get(byte))
					{
						cipher += byte;
					}
					file.close();
				}
				file.open("aes_key.enc", std::ios::in);
				if (file.is_open()) {
					while (file.get(byte))
					{
						aeskey += byte;
					}
					file.close();
				}

				send(client, cipher.c_str(), strlen(cipher.c_str()), 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';

				send(client, aeskey.c_str(), strlen(aeskey.c_str()), 0);
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';

				MessageBox(_T("Message sent"));

				std::string mycert;
				file.open("ca.crt", std::ios::in);
				if (file.is_open())
				{
					while (file.get(byte))
					{
						mycert += byte;
					}
					file.close();
				}


				if (!fs::exists("Sent"))
				{
					fs::create_directory("Sent");
				}
				fs::current_path("Sent");

				int i = 0;
				std::string filename;
				while (time[i] != '\0')
				{
					if (time[i] != '/' && time[i] != ':' && time[i] != ' ')
					{
						filename += time[i];
					}
					i++;
				}
				file.open(filename, std::ios::out);
				if (file.is_open())
				{
					file << header;
					file << body;
					file.close();
				}
				
				fs::current_path("..");
				fs::current_path("..");

				fs::current_path(to_userRoot);
				file.open(userRoot + "Cert.crt", std::ios::out);
				if (file.is_open())
				{
					file << mycert;
					file.close();
				}
				sentSuccessfully = true;

				fs::current_path("..");

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


void CDlgFeature::OnBnClickedsent()
{
	std::string path = std::string(CT2A(username)) + "\\Sent";
	if (fs::exists(path))
	{
		std::vector<std::string> file;
		
		for (const auto& entry : fs::directory_iterator(path)) {
			if (fs::is_regular_file(entry)) {
				file.push_back(entry.path().filename().string());
			}
		}
		std::fstream fIn;
		std::string name, firstUser;
		users.clear();
		for (int i = 0; i < file.size(); ++i)
		{
			fIn.open(path + "\\" + file[i], std::ios::in);
			if (fIn.is_open())
			{
				std::string time, from, to, subject;
				getline(fIn, time);
				getline(fIn, from);
				getline(fIn, to);
				getline(fIn, subject);
				subject = subject.substr(9);
				if (i == 0)
				{
					firstUser = to;
					name = time + "     " + to + +"          " + subject;
				}
				else
				{
					name = time + "     " + to;
					int space;
					if (firstUser.length() > to.length())
					{
						space = 10 + to.length() - firstUser.length();
					}
					else
					{
						space = 10 - to.length() + firstUser.length();

					}
					name = name.append(space, ' ');
					name += subject;
				}
				users.push_back({ file[i],name });
				fIn.close();
			}
		}	
		std::vector<CString> reversedNames;

		for (int i = users.size() - 1; i >= 0; --i) {
			reversedNames.push_back(CString(users[i].name_displayed.c_str()));
		}

		m_sentbox.ResetContent();
		for (const auto& name : reversedNames) {
			m_sentbox.InsertString(-1, name);
		}
	}
	
	m_sentbox.ShowWindow(SW_SHOW);
	m_sentbox.Invalidate();
	ShowHideControls(IDC_COMPOSE, 1);
	ShowHideControls(IDC_INBOX, 1);
	ShowHideControls(IDC_groupsent, 0);
	ShowHideControls(IDC_SPAM, 1);

}


void CDlgFeature::OnLbnSelchangesentbox()
{
	// TODO: Add your control notification handler code here
	int selectedIndex = m_sentbox.GetCurSel();
	if (selectedIndex != LB_ERR) {
		CString selected;
		m_sentbox.GetText(selectedIndex, selected);

		int nameIndex = -1;
		for (int i = 0; i < users.size(); i++) {
			if (users[i].name_displayed == std::string(CT2A(selected))) {
				nameIndex = i;
				break;
			}
		}

		if (nameIndex != -1)
		{
			std::string message = "You choose: " + users[nameIndex].name_displayed;
			CString temp(message.c_str());
			MessageBox(temp);
			EndDialog(IDD_DIALOG1);
			CDlgSentBox mail;
			mail.filename = users[nameIndex].file_name;
			mail.DoModal();
		}
	}

}


void CDlgFeature::OnBnClickedSpam()
{
	std::string folder_path = std::string(CT2A(username)) + "\\Spam";
	m_spambox.ResetContent();
	if (fs::exists(folder_path))
	{
		std::vector<std::string> filename;
		filename.empty();
		for (const auto& entry : fs::directory_iterator(folder_path)) {
			if (fs::is_regular_file(entry)) {
				filename.push_back(entry.path().filename().string());
			}
		}

		//std::reserve(filename.begin(), filename.end());
		users.empty();
		for (int i = 0; i < filename.size(); ++i)
		{
			std::string title = "";

			std::fstream file(folder_path + "\\" + filename[i], std::ios::in);
			if (file.is_open())
			{
				std::string from, to, date, subject;
				getline(file, date);
				getline(file, from);
				getline(file, to);
				getline(file, subject);
				date = date.substr(0, date.length() - 1);
				from = from.substr(0, from.length() - 1);
				subject = subject.substr(0, subject.length() - 1);

				title = date + "        " + from + "           " + subject;
				users.push_back({ filename[i], title });
				file.close();
			}
			int index = m_spambox.InsertString(0, CString(title.c_str()));
			//m_inbox.AddString(CString(title.c_str()));

		}

	}
	ShowHideControls(IDC_COMPOSE, 1);
	ShowHideControls(IDC_INBOX, 1);
	ShowHideControls(IDC_groupsent, 1);
	ShowHideControls(IDC_SPAM, 0);
}


void CDlgFeature::OnLbnSelchangespambox()
{
	CDlgMail mail;

	// TODO: Add your control notification handler code here
	int selectedIndex = m_spambox.GetCurSel();
	if (selectedIndex != LB_ERR) {
		CString selected;
		m_spambox.GetText(selectedIndex, selected);

		int nameIndex = -1;
		for (int i = 0; i < users.size(); i++) {
			if (users[i].name_displayed == std::string(CT2A(selected))) {
				nameIndex = i;
				break;
			}
		}

		if (nameIndex != -1)
		{
			std::string message = "You choose: " + users[nameIndex].name_displayed;
			CString temp(message.c_str());
			MessageBox(temp);

			mail.file = users[nameIndex].file_name;
			mail.box_type = "Spam";
			EndDialog(IDD_DIALOG1);
		}
		mail.DoModal();
	}
}
