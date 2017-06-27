#include "stdafx.h"
#include "EditorTest.h"
using namespace std;

CEditorTest::CEditorTest()
{
}


CEditorTest::~CEditorTest()
{
}

void CEditorTest::RestoreVertexBackup()
{
	m_VertexStack = m_VertexStackBackup;
	m_VertexCount = m_VertexCountBackup;
	while (m_VertexStack)
	{
		CVertexStack *trash;
		trash = m_VertexStack;
		m_VertexStack = m_VertexStack->next;
		delete trash;
	}
}

void CEditorTest::RestoreLinkBackup()
{
	m_LinkStack = m_LinkStackBackup;
	m_LinkCount = m_LinkCountBackup;
	m_LinkCount = 0;
	while (m_LinkStack)
	{
		CLinkStack *trash;
		trash = m_LinkStack;
		m_LinkStack = m_LinkStack->next;
		delete trash;
	}
}


void CEditorTest::InitVertexCheck()
{
	m_VertexStackBackup = m_VertexStack;
	m_VertexCountBackup = m_VertexCount;

	m_VertexSample.x = 0;
	m_VertexSample.y = 0;
	m_VertexSample.num = 0;
	m_VertexSample.next = NULL;
	m_VertexSample.store = false;
	m_VertexPrevious = new CVertexStack;
	*m_VertexPrevious = m_VertexSample;
	m_VertexStack = m_VertexPrevious;
	m_VertexCount = 1;
}

void CEditorTest::InitLinkCheck()
{
	m_LinkStackBackup = m_LinkStack;
	m_LinkCountBackup = m_LinkCount;

	m_LinkSample.numA = 0;
	m_LinkSample.numB = 0;
	m_LinkSample.distance = 200;
	m_LinkSample.next = NULL;
	m_LinkSample.dir = 2;
	m_LinkPrevious = new CLinkStack;
	*m_LinkPrevious = m_LinkSample;
	m_LinkStack = m_LinkPrevious;
	m_LinkCount = 1;
}

bool CEditorTest::VertexCheck(int x,int y,int num, CVertexStack *next, CVertexStack sampleNext)
{
	CVertexStack SN = *next;
	if (!m_VertexStack)
	{
		ErrorMessage("..vertex stack remained empty..");
		return false;
	}
	if ((m_VertexStack->x != x) || m_VertexStack->y != y)
	{
		ErrorMessage("..saved coordinates don't match input..");
		return false;
	}
	if (m_VertexCount != num+1)
	{
		ErrorMessage("..vertexes is counted wrongly..");
		return false;
	}
	if (m_VertexStack->num != num)
	{
		ErrorMessage("..vertex number is wrong..");
		return false;
	}
	if (m_VertexStack->next != next)
	{
		ErrorMessage("..previous vertex isn't saved..");
		return false;
	}
	if (m_VertexStack->store != false)
	{
		ErrorMessage("..store marker isn't initialized..");
		return false;
	}
	if (next->x != SN.x || next->y != SN.y || next->num != SN.num || next->store != SN.store || next->next != SN.next)
	{
		ErrorMessage("..previous vertex data were changed..");
		return false;
	}
	if (m_Selected != m_VertexStack)
	{
		ErrorMessage("..created vertex isn't selected..");
		return false;
	}
	if (m_Mode != VERTEX)
	{
		ErrorMessage("..Vertex mode weren't set..");
		return false;
	}
	return true;
}

bool CEditorTest::LinkCheck(int A, int B, int num, CLinkStack *next, CLinkStack sampleNext)
{
	CLinkStack SN = *next;
	if (!m_LinkStack)
	{
		ErrorMessage("..link stack is empty..");
		return false;
	}
	if ((m_LinkStack->numA != A) || m_LinkStack->numB != B)
	{
		ErrorMessage("..Wrong vertexes linked..");
		return false;
	}
	if (m_LinkCount != num + 1)
	{
		ErrorMessage("..vertexes is counted wrongly..");
		return false;
	}
	if (m_LinkStack->dir != 2)
	{
		ErrorMessage("..link dirrection is wrong..");
		return false;
	}
	if (m_LinkStack->next != next)
	{
		ErrorMessage("..previous link isn't saved..");
		return false;
	}

	if (next->numA != SN.numA || next->numB != SN.numB || next->next != SN.next)
	{
		ErrorMessage("..previous link data were changed..");
		return false;
	}

	return true;
}

void CEditorTest::ErrorMessage(string hint)
{
	cout << "failed" << endl;
	cout << hint.c_str() << endl;
}

bool CEditorTest::Test_AddVertex()
{
	InitVertexCheck();
	cout << "Trying to add vertex...";
	AddVertex(42, 24);
	bool success = VertexCheck(42, 24, 1, m_VertexPrevious, m_VertexSample);
	if (success) cout << "successful" << endl;
	RestoreVertexBackup();
	return success;
}

bool CEditorTest::Test_Select()
{
	cout << "Trying to select vertex...";
	AddVertex(10, 7);
	AddVertex(3, 2);
	CVertexStack *RightVertex = m_Selected;
	AddVertex(8, 6);
	Select(3, 2);
	if (m_Selected != RightVertex)
	{
		ErrorMessage("..Wrong vertex is selected..");
		return false;
	}
	m_VertexCount = m_VertexCount-3;
	for (int i = 0; i < 3;i++)
	{
		CVertexStack *trash;
		trash = m_VertexStack;
		m_VertexStack = m_VertexStack->next;
		delete trash;
	}
	m_Selected = NULL;
	cout << "successful"<<endl;
	return true;
}

bool CEditorTest::Test_Connect()
{
	bool success=false;
	cout << "Trying to connect vertexes..";
	InitLinking();
	InitLinkCheck();
	InitVertexCheck();
	m_Selected = NULL;
	Connect(42,23);
	InitAdding();
	if (!VertexCheck(42, 23, 1, m_VertexPrevious, m_VertexSample))
	{
		RestoreVertexBackup();
		return false;
	}


	int A = m_Selected->num;
	CVertexStack *prev = m_VertexStack;
	InitLinking();
	Connect(84, 60);
	InitAdding();
	if (!VertexCheck(84, 60, 2, prev, *prev))
	{
		RestoreVertexBackup();
		RestoreLinkBackup();
		return false;
	}
	int B = m_VertexStack->num;
	if (!LinkCheck(A, B, 1, m_LinkPrevious, m_LinkSample))
	{
		RestoreVertexBackup();
		RestoreLinkBackup();
		return false;
	}
	cout << "successful" << endl;
	return true;
}

bool CEditorTest::Test_Clear()
{
	InitVertexCheck();
	InitLinkCheck();
	CVertexStack *v1 = m_VertexStack;
	CLinkStack *l1 = m_LinkStack;
	Connect(42, 23);
	CVertexStack *v2 = m_VertexStack;
	CLinkStack *l2 = m_LinkStack;
	Clear();
	cout << "Trying to erace internal data..";
	if ((m_VertexStack==NULL)&&(m_LinkStack==NULL))
	{
		cout << "successful" << endl;
		return true;
	}
	else
	{
		cout << "failed" << endl;
		return false;
	}
}

