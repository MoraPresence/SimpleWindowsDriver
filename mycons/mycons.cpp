#include <windows.h>
#include <stdio.h>

void main(){
	HANDLE hfile = CreateFile("\\\\.\\MyLink\\mydir\\myfile", FILE_ALL_ACCESS, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hfile)
	{
		printf("ERR: CreateFile");
		return;
	}
	printf("SUCCESS: CreateFile");
	CloseHandle(hfile);
}