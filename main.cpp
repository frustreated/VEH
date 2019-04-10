#include <stdio.h>
#include <windows.h>
#include "veh.h"

class myclass {
public:
	void devide() {
		MessageBox(0, "devide", 0, 0);
		int a = 0;
		a = 1 / a;
	}

	void access() {
		MessageBox(0, "access", 0, 0);
		memcpy(0, 0, 1);
	}

	void normal() {
		MessageBox(0, "normal", 0, 0);
	}

	void except() {
		MessageBox(0, "except", 0, 0);
	}
};

DWORD kk(LPVOID lpParam) {
	char str[100];
	myclass val;
	VEH veh;

	veh.SafeExecute(CAST_MEMFUNC(myclass::normal), &val, CAST_MEMFUNC(myclass::except), &val);
	sprintf(str, "end2 : %08X", veh.GetExceptionCode());
	MessageBox(0, str, 0, 0);

	veh.SafeExecute(CAST_MEMFUNC(myclass::devide), &val, CAST_MEMFUNC(myclass::except), &val);
	sprintf(str, "end2 : %08X", veh.GetExceptionCode());
	MessageBox(0, str, 0, 0);

	veh.SafeExecute(CAST_MEMFUNC(myclass::access), &val, CAST_MEMFUNC(myclass::except), &val);
	sprintf(str, "end2 : %08X", veh.GetExceptionCode());
	MessageBox(0, str, 0, 0);

	return 0;
}

void main() {
	char str[100];
	myclass val;
	VEH veh;

	veh.SafeExecute(CAST_MEMFUNC(myclass::normal), &val, CAST_MEMFUNC(myclass::except), &val);
	sprintf(str, "end1 : %08X", veh.GetExceptionCode());
	MessageBox(0, str, 0, 0);

	veh.SafeExecute(CAST_MEMFUNC(myclass::devide), &val, CAST_MEMFUNC(myclass::except), &val);
	sprintf(str, "end1 : %08X", veh.GetExceptionCode());
	MessageBox(0, str, 0, 0);

	veh.SafeExecute(kk);

	veh.SafeExecute(CAST_MEMFUNC(myclass::access), &val, CAST_MEMFUNC(myclass::except), &val);
	sprintf(str, "end1 : %08X", veh.GetExceptionCode());
	MessageBox(0, str, 0, 0);
}