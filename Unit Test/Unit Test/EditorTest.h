#pragma once
#include "D:\Projects\C\12345\12345\Editor.h"
#include "stdafx.h";
using namespace std;
class CEditorTest :
	public CEditor
{
private:
	int m_VertexCountBackup;
	CVertexStack *m_VertexStackBackup;
	CVertexStack *m_VertexPrevious;
	CVertexStack m_VertexSample;
	CLinkStack *m_LinkStackBackup;
	int m_LinkCountBackup;
	CLinkStack *m_LinkPrevious;
	CLinkStack m_LinkSample;
	
public:
	CEditorTest();
	~CEditorTest();
	bool Test_AddVertex();
	bool Test_Select();
	bool Test_Connect();
	bool Test_Clear();
	void ErrorMessage(string hint);
	void InitLinkCheck();
	bool LinkCheck(int A,int B,int num, CLinkStack *next,CLinkStack sampleNext);
	void RestoreLinkBackup();
	void InitVertexCheck();
	bool VertexCheck(int x, int y, int num, CVertexStack *next, CVertexStack sampleNext);
	void RestoreVertexBackup();
};

