#ifndef SMTP_CUSTOM
#define SMTP_CUSTOM

#include"CSmtp.h"
#include"opencv2\opencv.hpp"


TCHAR str[200];
bool ok=false;
BOOL CALLBACK InfoDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		SetDlgItemText(hDlg,IDC_STR,TEXT(""));
		

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg,IDC_STR, str,128);
			
			
			EndDialog(hDlg,IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}



class SMTP_Helper
{
public :
	void returnEmailString(std::string &emailAddress)
	{
		emailAddress=str;
	}

	

	bool  SendEmail(std::string emailAddress)
	{
	bool bError = false;

	try
	{
		CSmtp mail;

#define test_gmail_tls

#if defined(test_gmail_tls)
		mail.SetSMTPServer("smtp.gmail.com",587);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_gmail_ssl)
		mail.SetSMTPServer("smtp.gmail.com",465);
		mail.SetSecurityType(USE_SSL);
#elif defined(test_hotmail_TLS)
		mail.SetSMTPServer("smtp.live.com",25);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_aol_tls)
		mail.SetSMTPServer("smtp.aol.com",587);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_yahoo_ssl)
		mail.SetSMTPServer("plus.smtp.mail.yahoo.com",465);
		mail.SetSecurityType(USE_SSL);
#endif

		
		mail.SetLogin("EMAIL");
		mail.SetPassword("PASSWORD");
  		mail.SetSenderName("AJOU_OPIC_TEAM");
  		mail.SetSenderMail("AJOU_OPIC_TEAM.com");
  		mail.SetReplyTo("");
  		mail.SetSubject("The message");
		mail.AddRecipient(emailAddress.c_str());
  		mail.SetXPriority(XPRIORITY_NORMAL);
  		mail.SetXMailer("The Bat! (v3.02) Professional");
  		mail.AddMsgLine("v");
		mail.AddMsgLine("");
		mail.AddMsgLine("");
		mail.AddMsgLine("");
		mail.AddMsgLine("");
		mail.AddMsgLine("");
		mail.ModMsgLine(5,"regards");
		mail.DelMsgLine(2);
		mail.AddMsgLine("AJOU_OPIC_TEAM");


		mail.AddAttachment("result_conversion.jpg");

		mail.Send();
	}
	catch(ECSmtp e)
	{
		std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";
		bError = true;
		return false;
	}
	if(!bError)
		std::cout << "Mail was send successfully.\n";
	return true;
}

};


#endif 






