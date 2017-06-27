// Unit Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "EditorTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CEditorTest TestSubject;
	TestSubject.Test_AddVertex();
	TestSubject.Test_Select();
	TestSubject.Test_Connect();
	TestSubject.Test_Clear();
	char garbage;
	cin >> garbage;
	return 0;

}

