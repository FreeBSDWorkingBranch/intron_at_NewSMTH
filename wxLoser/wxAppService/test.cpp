#include "stdafx.h"

#include "wxAppService.h"

class wxTestServiceApp : public wxAppService
{
public:
	wxTestServiceApp();
	virtual bool OnInit() wxOVERRIDE;
	virtual bool ServiceMain() wxOVERRIDE;
	virtual void OnNonService() wxOVERRIDE;
	virtual int OnExit() wxOVERRIDE;
};

wxTestServiceApp::wxTestServiceApp()
	: wxAppService(wxT("TestService"), wxT("A testing service."))
{
}

bool wxTestServiceApp::OnInit()
{
	if (m_bHasInstallOption)
	{
		InstallService();
		return (false);
	}
	else if(m_bHasUninstallOption)
	{
		UninstallService();
		return (false);
	}

	WriteServiceLog(wxLOG_Debug,
		wxString::Format("[%s] TID=%d", __FUNCTION__, (int)wxThread::GetCurrentId()));

	return (true);
}

bool wxTestServiceApp::ServiceMain()
{
	WriteServiceLog(wxLOG_Debug,
		wxString::Format("[%s] TID=%d", __FUNCTION__, (int)wxThread::GetCurrentId()));

	while (!IsStopping())
		wxSleep(1);

	return (true);
}

void wxTestServiceApp::OnNonService()
{
	printf("%ls", wxT("Please install this program as service firstly, and then run this program as service."));
}

int wxTestServiceApp::OnExit()
{
	WriteServiceLog(wxLOG_Debug,
		wxString::Format("[%s] TID=%d", __FUNCTION__, (int)wxThread::GetCurrentId()));

	return 0;
}

wxIMPLEMENT_APP_CONSOLE(wxTestServiceApp);
