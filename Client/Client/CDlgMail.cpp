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


// CDlgMail message handlers

BOOL CDlgMail::OnInitDialog() {
    CDialog::OnInitDialog();

    recvMessage = recv(client, buffer, BUF_SIZE, 0);
    buffer[recvMessage] = '\0';
    CString temp(buffer);

    //// Lấy kích thước hiện tại của control
    //CRect rect;
    //m_content.GetClientRect(&rect);

    //// Tạo một đối tượng CClientDC để lấy thông tin về font
    //CDC* pDC = m_content.GetDC();
    //if (pDC) {
    //    // Lấy font của static text
    //    CFont* pFont = m_content.GetFont();
    //    if (pFont) {
    //        CFont* pOldFont = pDC->SelectObject(pFont);

    //        pDC->GetTextExtent(temp, recvMessage); 

    //        // Kiểm tra xem chiều rộng của văn bản có lớn hơn chiều rộng của static text không
    //        if (recvMessage < rect.Width()) {
    //            // Nếu có, thực hiện xuống dòng trong văn bản
    //            temp.Replace(_T(" "), _T("\r\n")); // Thay thế khoảng trắng bằng dấu xuống dòng
    //        }

    //        pDC->SelectObject(pOldFont);
    //    }
    //    m_content.ReleaseDC(pDC);
    //}

    //// Đặt văn bản đã thay đổi vào CStatic control
    m_content.SetWindowTextW(temp);

    return TRUE;
}

void CDlgMail::OnBnClickedBack()
{
	// TODO: Add your control notification handler code here
	EndDialog(IDD_DIALOG2);
	CDlgFeature ft;
	ft.DoModal();
}
