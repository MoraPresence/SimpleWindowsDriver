#include "StdAfx.h"
#include "SCMgr.h"

CSCMgr::CSCMgr(void){
	hscm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
}

CSCMgr::~CSCMgr(void){
	CloseServiceHandle(hscm);
}

void CSCMgr::Add(LPCTSTR sname, LPCTSTR fname){

	SC_HANDLE hserv = CreateService(hscm, sname, sname, SERVICE_ALL_ACCESS, 
		SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL, fname, NULL, NULL, NULL,
		NULL, NULL);
	if(!hserv){
		throw -1;
		return;
	}
		
	CloseServiceHandle(hserv);
}

void CSCMgr::Del(LPCTSTR sname){
	//1. Open Service 
	SC_HANDLE hserv = OpenService(hscm, sname, SERVICE_ALL_ACCESS);
	if(!hserv) return;
	//2. Control command
	if(!DeleteService(hserv)){
		throw -1;
	}else{
		//access message
	}
	//3. Close Service
	CloseServiceHandle(hserv);
}

void CSCMgr::Start(LPCTSTR sname){
	//1. Open Service 
	SC_HANDLE hserv = OpenService(hscm, sname, SERVICE_ALL_ACCESS);
	if(!hserv) return;
	//2. Control command
	if(!StartService(hserv, 0, NULL)){
		throw -1;
	}else{
		//access message
	}
	//3. Close Service
	CloseServiceHandle(hserv);
}

void CSCMgr::Stop(LPCTSTR sname){
	//1. Open Service 
	SC_HANDLE hserv = OpenService(hscm, sname, SERVICE_ALL_ACCESS);
	if(!hserv) return;
	//2. Control command
	SERVICE_STATUS status;
	if(!ControlService(hserv, SERVICE_CONTROL_STOP, &status)){
		throw -1;
	}else{
		//access message
	}
	//3. Close Service
	CloseServiceHandle(hserv);
}

HANDLE CSCMgr::hfile = INVALID_HANDLE_VALUE;

void CSCMgr::Open(LPCTSTR lname){
	CloseHandle(hfile);
	hfile = CreateFile(lname, FILE_ALL_ACCESS, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

void CSCMgr::Close(){
	CloseHandle(hfile);
	hfile = INVALID_HANDLE_VALUE;
}

bool CSCMgr::Read(void* buf, DWORD bsize){
	DWORD lpNubmerOfBytesRead;
	if(!ReadFile(hfile, buf, 8, &lpNubmerOfBytesRead, NULL)){
		throw -1;
	}
	return true;
}

bool CSCMgr::Write(void* buf, DWORD bsize){
	DWORD lpNubmerOfBytesWritten;
	if(!WriteFile(hfile, buf, 16, &lpNubmerOfBytesWritten, NULL)){
		throw -1;
	}
	return true;
}

