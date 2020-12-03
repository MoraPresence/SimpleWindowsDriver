#pragma once
class CSCMgr
{
private:
	SC_HANDLE hscm;
	static   HANDLE hfile;
public:
	CSCMgr(void);
	~CSCMgr(void);

	void Add(LPCTSTR sname, LPCTSTR fname);

	void Del(LPCTSTR sname);

	void Start(LPCTSTR sname);

	void Stop(LPCTSTR sname);

	void Open(LPCTSTR sname);

	bool Read(void* buf, DWORD bsize);

	bool Write(void* buf, DWORD bsize);

	void Close();
};
