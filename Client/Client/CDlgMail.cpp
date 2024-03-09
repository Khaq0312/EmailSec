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
    send(client, "OK", 2, 0);

    recvMessage = recv(client, buffer, BUF_SIZE, 0);
    buffer[recvMessage] = '\0';
    CString temp(buffer);

    CRect rect;
    m_content.GetClientRect(&rect);

    CDC* pDC = m_content.GetDC();
    if (pDC) {
        CFont* pFont = m_content.GetFont();
        if (pFont) {
            CFont* pOldFont = pDC->SelectObject(pFont);

            CString wrappedText; // Chuỗi văn bản đã được xuống hàng
            CString currentLine; // Chuỗi hiện tại đang xử lý

            int currentLineWidth = 0; // Độ dài hiện tại của dòng đang xử lý

            for (int i = 0; i < temp.GetLength(); ++i)
            {
                // Thêm ký tự mới vào dòng hiện tại và cập nhật độ dài dòng
                currentLine += temp[i];
                currentLineWidth = pDC->GetTextExtent(currentLine).cx;

                // Kiểm tra nếu độ dài của dòng vượt quá chiều rộng của control hoặc là ký tự xuống dòng
                if (currentLineWidth > rect.Width() || temp[i] == '\n')
                {
                    // Thêm dòng hiện tại vào chuỗi đã xuống hàng
                    wrappedText += currentLine.Left(currentLine.GetLength() - 1) + _T("\r\n");
                    // Bắt đầu dòng mới với ký tự hiện tại (nếu không phải là ký tự xuống dòng)
                    if (temp[i] != '\n')
                        currentLine = temp[i];
                    else
                        currentLine.Empty(); // Reset currentLine nếu là ký tự xuống dòng
                }
            }

            // Thêm dòng cuối cùng vào chuỗi đã xuống hàng (nếu có)
            if (!currentLine.IsEmpty())
                wrappedText += currentLine;

            pDC->SelectObject(pOldFont);
            m_content.ReleaseDC(pDC);

            // Đặt chuỗi đã xuống hàng vào control
            m_content.SetWindowTextW(wrappedText);
        }
    }



    return TRUE;
}

void CDlgMail::OnBnClickedBack()
{
	// TODO: Add your control notification handler code here
	EndDialog(IDD_DIALOG2);
	CDlgFeature ft;
	ft.DoModal();
}
