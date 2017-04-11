#include "stdafx.h"

#ifdef __WXMSW__
#include <windows.h>
#else  // ! __WXMSW__
// *NIX
#include <signal.h>
#endif // __WXMSW__

#include "wxAppService.h"

wxAppService::wxAppService(const wxString & service_name, const wxString & service_descr) :
	wxAppConsole(),
	m_bHasInstallOption(false),
	m_bHasUninstallOption(false),
	m_strServiceName(service_name),
	m_strServiceDescription(service_descr),
	m_bIsStopping(false)
{
#ifdef __WXMSW__
	m_dwCurrentState = SERVICE_STOPPED;
	m_dwCheckPoint = 1;
	m_hServiceStatus = NULL;
#endif // __WXMSW__
}

#ifndef __WXMSW__
// *NIX
static void gs_SignalHandler(int sig)
{
	wxTheAppService->TryToStop();
}
#endif // ! __WXMSW__

bool wxAppService::Initialize(int & argc, wxChar ** argv)
{
	bool bret;
	int i;
	wxString s;

	bret = wxAppConsole::Initialize(argc, argv);
	if (!bret)
		return (false);

	for (i = 1; i < argc; i++)
	{
		s = argv[i];

		if (s == wxT("--install"))
			m_bHasInstallOption = true;
		else if (s == wxT("--uninstall"))
			m_bHasUninstallOption = true;
	}

#ifndef __WXMSW__
	// *NIX
	signal(SIGTERM, gs_SignalHandler);
#endif // ! __WXMSW__

	// CallOnInit() will be called

	return (true);
}

#ifdef __WXMSW__
static VOID WINAPI gs_ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	wxTheAppService->ServiceMainWrapper();
}
#endif // __WXMSW__

int wxAppService::OnRun()
{
#ifdef __WXMSW__
	wxChar * pname = wxStrdup((const wxChar *)m_strServiceName);

	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		{ pname, (LPSERVICE_MAIN_FUNCTION)gs_ServiceMain },
		{ NULL, NULL }
	};

	// Return until service is stopped
	if (!StartServiceCtrlDispatcher(DispatchTable))
	{
		WriteServiceLog(wxLOG_Error, wxT("StartServiceCtrlDispatcher() failed."));
		OnNonService();
	}

	free(pname);

	return (0);
#else  // ! __WXMSW__
	// *NIX: Waiting for being stopped if ServiceMain() returns true
	if (ServiceMain())
		m_objSemaphore.Wait();

	return (0);
#endif // __WXMSW__
}

bool wxAppService::InstallService()
{
#ifdef __WXMSW__
	SC_HANDLE hmgr;
	SC_HANDLE hsrv;
	TCHAR exepath[MAX_PATH];
	SERVICE_DESCRIPTION descr;

	if (!GetModuleFileName(NULL, exepath, MAX_PATH))
	{
		wxLogError(wxT("Failed to install service: %d"), (int)GetLastError());
		return (false);
	}

	hmgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hmgr)
	{
		wxLogError(wxT("OpenSCManager() failed: %d"), (int)GetLastError());
		return (false);
	}

	hsrv = CreateService(hmgr, m_strServiceName, m_strServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		exepath, NULL, NULL, NULL, NULL, NULL);
	if (hsrv == NULL)
	{
		int ierr = GetLastError();

		if (ierr == ERROR_SERVICE_EXISTS)
		{
			wxLogError(wxT("CreateService() failed: Service already exists."));
		}
		else
		{
			wxLogError(wxT("CreateService() failed: %d"), ierr);
		}

		CloseServiceHandle(hmgr);
		return (false);
	}

	wxChar * pdescrtxt = wxStrdup((const wxChar *)m_strServiceDescription);
	descr.lpDescription = pdescrtxt;
	ChangeServiceConfig2(hsrv, SERVICE_CONFIG_DESCRIPTION, &descr);
	free(pdescrtxt);

	CloseServiceHandle(hsrv);
	CloseServiceHandle(hmgr);

	return (true);
#else  // ! __WXMSW__
	// *NIX
	return (true);
#endif // __WXMSW__
}

bool wxAppService::UninstallService()
{
#ifdef __WXMSW__
	SC_HANDLE hmgr;
	SC_HANDLE hsrv;
	BOOL bret;

	hmgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (NULL == hmgr)
	{
		wxLogError(wxT("OpenSCManager() failed: %d"), (int)GetLastError());
		return (false);
	}

	hsrv = OpenService(hmgr, m_strServiceName, DELETE);
	if (hsrv == NULL)
	{
		int ierr = GetLastError();

		if (ierr == ERROR_SERVICE_DOES_NOT_EXIST)
		{
			wxLogError(wxT("OpenService() failed: Service doesn't exist."));
		}
		else
		{
			wxLogError(wxT("OpenService() failed: %d"), ierr);
		}

		CloseServiceHandle(hmgr);
		return (false);
	}

	// Delete the service.

	bret = DeleteService(hsrv);

	CloseServiceHandle(hsrv);
	CloseServiceHandle(hmgr);

	if (!bret)
	{
		wxLogError(wxT("DeleteService() failed: %d"), (int)GetLastError());
		return (false);
	}

	return (true);
#else  // ! __WXMSW__
	// *NIX
	return (true);
#endif // __WXMSW__
}

bool wxAppService::IsStopping()
{
	return (m_bIsStopping);
}

void wxAppService::TryToStop()
{
#ifdef __WXMSW__
	ReportServiceState(SERVICE_STOP_PENDING, NO_ERROR, 0);
#endif // __WXMSW__
	m_bIsStopping = true;
	m_objSemaphore.Post();
}

void wxAppService::WriteServiceLog(wxLogLevelValues level, const wxString & txt)
{
#ifdef __WXMSW__
	HANDLE hlog;
	WORD wtype;
	LPCTSTR strptr[1];

	strptr[0] = txt;

	hlog = RegisterEventSource(NULL, m_strServiceName);

	switch (level)
	{
	case wxLOG_FatalError:
	case wxLOG_Error:
		wtype = EVENTLOG_ERROR_TYPE;
		break;
	case wxLOG_Warning:
		wtype = EVENTLOG_WARNING_TYPE;
		break;
	default:
		wtype = EVENTLOG_INFORMATION_TYPE;
		break;
	}

	if (NULL != hlog)
	{
		ReportEvent(hlog, wtype, 0, 0xC0020001L,
			NULL, sizeof(strptr) / sizeof(strptr[0]), 0, strptr, NULL);
		DeregisterEventSource(hlog);
	}
#else // ! __WXMSW__
	// *NIX
	wxLogGeneric(level, m_strServiceName + wxT(": ") + txt);
#endif // __WXMSW__
}

#ifdef __WXMSW__
void wxAppService::KeepServiceState(unsigned int op)
{
	ReportServiceState(m_dwCurrentState, NO_ERROR, 0);
}

static VOID WINAPI gs_ServiceControlHandler(DWORD op)
{
	if (op == SERVICE_CONTROL_STOP)
		wxTheAppService->TryToStop();
	else
		wxTheAppService->KeepServiceState(op);
}

void wxAppService::ServiceMainWrapper()
{
	m_hServiceStatus = RegisterServiceCtrlHandler(m_strServiceName,
		gs_ServiceControlHandler);

	if (!m_hServiceStatus)
	{
		WriteServiceLog(wxLOG_Error, wxT("RegisterServiceCtrlHandler() Failed."));
		return;
	}

#if 0
	ReportServiceState(SERVICE_START_PENDING, NO_ERROR, 3000);
#endif

	ReportServiceState(SERVICE_RUNNING, NO_ERROR, 0);

	// Waiting for being stopped if ServiceMain() returns true
	if (ServiceMain())
		m_objSemaphore.Wait();

	ReportServiceState(SERVICE_STOPPED, NO_ERROR, 0);
}

void wxAppService::ReportServiceState(DWORD state, DWORD exitcode, DWORD waithint)
{
	SERVICE_STATUS status;

	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwServiceSpecificExitCode = 0;

	status.dwCurrentState = state;
	status.dwWin32ExitCode = exitcode;
	status.dwWaitHint = waithint;

	status.dwControlsAccepted = (state == SERVICE_START_PENDING) ? 0 : SERVICE_ACCEPT_STOP;

	status.dwCheckPoint = (state == SERVICE_RUNNING || state == SERVICE_STOPPED)
		? 0 : m_dwCheckPoint;
	m_dwCheckPoint++;

	m_dwCurrentState = state;

	SetServiceStatus(m_hServiceStatus, &status);
}
#endif // __WXMSW__
