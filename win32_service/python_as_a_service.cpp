
#pragma comment(lib, "advapi32.lib")

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#define LOGFILE "C:\\.python_as_a_service.log")
#define SERVICE_NAME "PythonAsAService"
#define SLEEP_TIME 5000
#define MY_CONFIG_FILE "C:\\.python_as_a_service.cfg"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void ServiceMain(int argc, char** argv);
void ControlHandler(DWORD request);
int InitService();
int WorkerLoop() ;

PROCESS_INFORMATION TheProcess;

void main()
{
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = SERVICE_NAME;
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;
    // Start the control dispatcher thread for our service
    StartServiceCtrlDispatcher(ServiceTable);
}

int WorkerLoop(std::ostream &fout) {
    using namespace std;
    ifstream fin(MY_CONFIG_FILE);
    fout << "Open " << MY_CONFIG_FILE << endl;
    if (!fin) {
      fout << "Failed to open file" << endl;
      return 0;
    }

    string buf;
    getline(fin, buf);  // get the python command
    fout << "Read command " << buf << endl;
    fin.close();

    STARTUPINFO si;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &TheProcess, sizeof(TheProcess) );

    fout << "Create Process" << endl;
    int res = CreateProcess (
            NULL,  // app name
    		    (char*)(buf.c_str()),
        		NULL,  // proc attributes
            NULL,  // thread attributes
            FALSE, // inherit handles
            NORMAL_PRIORITY_CLASS, // creation flags
        		NULL,  // environment
        		TEXT("C:\\"),  // CWD
        		&si,
        		&TheProcess
  		    );
   fout << "Returned " << res << endl;
   return res;
}


void ServiceMain(int argc, char** argv)
{
    int error;

    ServiceStatus.dwServiceType        = SERVICE_WIN32;
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode      = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint         = 0;
    ServiceStatus.dwWaitHint           = 0;

    hStatus = RegisterServiceCtrlHandler(
		SERVICE_NAME,
		(LPHANDLER_FUNCTION)ControlHandler);
    if (hStatus == (SERVICE_STATUS_HANDLE)0)
    {
        // Registering Control Handler failed
        return;
    }
    // Initialize Service
    error = InitService();
    if (error)
    {
		// Initialization failed
        ServiceStatus.dwCurrentState       = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode      = -1;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
    }
    // We report the running status to SCM.
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus (hStatus, &ServiceStatus);

    // Setup log file
    using std::ofstream;
    using std::endl;
    ofstream fout(LOGFILE;

    // Run service once
    if (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
	  {
        if (! WorkerLoop(fout) ) return;
   	}

    fout << "Running" << endl;
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
      Sleep(SLEEP_TIME);
    }

    // Kill Process
    fout << "Kill" << endl;
    fout << "Terminate"
         << TerminateProcess(TheProcess.hProcess, -1)
         << endl;
    fout << "Wait"
         << WaitForSingleObject(TheProcess.hProcess, INFINITE)
         << endl;
    fout << "Close thread"
         << CloseHandle(TheProcess.hThread)
         << endl;
    fout << "Close process"
         << CloseHandle(TheProcess.hProcess)
         << endl;
    return;
}

// Service initialization
int InitService()
{
	return 0;
}

// Control handler function
void ControlHandler(DWORD request)
{
    switch(request)
    {
        case SERVICE_CONTROL_STOP:
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
            SetServiceStatus (hStatus, &ServiceStatus);
            return;

        case SERVICE_CONTROL_SHUTDOWN:
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
            SetServiceStatus (hStatus, &ServiceStatus);
            return;

        default:
            break;
    }

    // Report current status
    SetServiceStatus (hStatus,  &ServiceStatus);

    return;
}
