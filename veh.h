#pragma once

#include <windows.h>
EXTERN_C DECLSPEC_IMPORT PVOID WINAPI AddVectoredExceptionHandler(ULONG FirstHandler, PVECTORED_EXCEPTION_HANDLER VectoredHandler);
EXTERN_C DECLSPEC_IMPORT PVOID WINAPI RemoveVectoredExceptionHandler(PVOID Handler);

#define CAST_MEMFUNC(FUNC) ((LPTHREAD_START_ROUTINE)&(void*&)FUNC)

class VEH {
private:
	static __declspec(thread) VEH* curVEH;

	LPVOID pHandler;
	VEH* preVEH;

	DWORD ExceptionCode;
	CONTEXT Context;
	bool bCaptured;

	static LONG VEHHandler(PEXCEPTION_POINTERS pExceptionInfo) {
		VEH& veh = *curVEH;
		if(!veh.bCaptured)
			return EXCEPTION_CONTINUE_SEARCH;

		veh.bCaptured = false;
		veh.ExceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;

		RtlRestoreContext(&veh.Context, 0);
		return EXCEPTION_CONTINUE_SEARCH;
	}
public:
	DWORD GetExceptionCode() {return ExceptionCode;}
	VEH() {
		pHandler = AddVectoredExceptionHandler(1, VEHHandler);
		preVEH = curVEH;
		curVEH = this;
		bCaptured = false;
	}

	~VEH() {
		RemoveVectoredExceptionHandler(pHandler);
		curVEH = preVEH;
	}

	DWORD SafeExecute(LPTHREAD_START_ROUTINE pRoutine, LPVOID lpParam = 0, LPTHREAD_START_ROUTINE pRoutineExcept = 0, LPVOID lpParamExcept = 0) {
		ExceptionCode = 0;
		bCaptured = true;
		RtlCaptureContext(&Context);
		//if exception occur, RIP set here with ExceptionCode.

		if(bCaptured) {
			DWORD result = pRoutine(lpParam);
			bCaptured = false;
			return result;
		}

		if(pRoutineExcept) {
			return pRoutineExcept(lpParamExcept);
		}
		return 0;
	}
};

VEH* VEH::curVEH = 0;