#pragma once

#ifndef wxOVERRIDE
#define wxOVERRIDE override
#endif

#define wxTheAppService  reinterpret_cast<wxAppService *>(wxAppConsole::GetInstance())

class wxAppService : public wxAppConsole
{
public:

	// Override methods from wxAppConsoleBase
	wxAppService(const wxString & service_name, const wxString & service_descr);
	virtual bool Initialize(int& argc, wxChar **argv) wxOVERRIDE;
	virtual int OnRun() wxOVERRIDE;

	// Service-specific methods
	bool HasInstallOption() { return (m_bHasInstallOption);  }
	bool HasUninstallOption() { return (m_bHasUninstallOption); }
	bool InstallService();
	bool UninstallService();
	virtual bool ServiceMain() { return (true); }
	virtual void OnNonService() { };
	bool IsStopping();
	void TryToStop();
	void WriteServiceLog(enum wxLogLevelValues level, const wxString & txt);

#ifdef __WXMSW__
	void KeepServiceState(unsigned int op);
	void ServiceMainWrapper();
#endif // __WXMSW__

protected:

	bool m_bHasInstallOption;
	bool m_bHasUninstallOption;
	wxString m_strServiceName;
	wxString m_strServiceDescription;
	bool m_bIsStopping;
	wxSemaphore m_objSemaphore;

#ifdef __WXMSW__
	DWORD m_dwCurrentState;
	DWORD m_dwCheckPoint;
	SERVICE_STATUS_HANDLE m_hServiceStatus;

	void ReportServiceState(DWORD state, DWORD exitcode, DWORD waithint);
#endif // __WXMSW__
};
