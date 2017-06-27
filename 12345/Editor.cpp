#include "Editor.h"
#include <math.h>
#include <fstream>
#include <string>




CEditor::CEditor()
{
	m_VertexStack = 0;
	m_VertexCount = 0;
	m_LinkStack = 0;
	m_LinkCount = 0;
	m_UndoStack = 0;
	m_StoreStack = 0;
	m_StoreCount = 0;
	m_Mode = VERTEX;
}

//------------------------------------------------------------------------------------------------------------

CEditor::~CEditor()
{
	Clear();
}

//------------------------------------------------------------------------------------------------------------
void CEditor::Clear()
{
	m_VertexCount = 0;
	while (m_VertexStack)
	{
		CVertexStack *trash;
		trash= m_VertexStack;
		m_VertexStack = m_VertexStack->next;
		delete trash;
	}
	m_LinkCount = 0;
	while (m_LinkStack)
	{
		CLinkStack *trash;
		trash = m_LinkStack;
		m_LinkStack = m_LinkStack->next;
		delete trash;
	}
	m_StoreCount = 0;
	while (m_StoreStack)
	{
		CStoreStack *trash;
		trash = m_StoreStack;
		m_StoreStack = m_StoreStack->next;
		delete trash;
	}
	while (m_UndoStack)
	{
		CUndoStack *trash;
		trash = m_UndoStack;
		m_UndoStack = m_UndoStack->next;
		delete trash;
	}
	m_Selected = NULL;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
}
//------------------------------------------------------------------------------------------------------------

int CEditor::Select(int x, int y)
{
	int dx,dy;
	CVertexStack *Current=m_VertexStack;
	while (Current)
	{

		dx = abs(Current->x - x);
		dy = abs(Current->y - y);
		if ((dx < m_R) && (dy < m_R))
		{
			m_Selected = Current;
			return m_Selected->num;
		}
		Current = Current->next;
	}
	if (!m_Selected) return -1;
	return m_Selected->num;
}

//------------------------------------------------------------------------------------------------------------

bool CEditor::AddVertex(int x, int y)
{
	CVertexStack *Current = new CVertexStack;
	Current->x = x;
	Current->y = y;
	Current->num = m_VertexCount;
	Current->store = false;
	Current->next = m_VertexStack;
	m_VertexStack = Current;
	m_Selected = m_VertexStack;
	m_VertexCount++;
	SaveAction(VERTEX);
	return true;
}
//------------------------------------------------------------------------------------------------------------

bool CEditor::AddStore(int Ammout)
{
	if (m_Mode != STORE) return false;
	CStoreStack *NewStore = new CStoreStack;
	NewStore->ammout = Ammout;
	NewStore->vertex = m_Selected->num;
	m_Selected->store = true;
	NewStore->next = m_StoreStack;
	m_StoreStack = NewStore;
	m_StoreCount++;
	SaveAction(STORE);
	return true;
}


//------------------------------------------------------------------------------------------------------------

bool CEditor::Draw(CPaintDC *dc)
{
	if (!m_VertexStack) return false;
	CLinkStack *CurrentLink=m_LinkStack;
	CVertexStack *CurrentVertex;
	while (CurrentLink)
	{
		CVertexStack *A=0;
		CVertexStack *B=0;
		CurrentVertex = m_VertexStack;
		while (!(A && B) && CurrentVertex)
		{
			
			if (CurrentVertex->num == CurrentLink->numA)
			{A = CurrentVertex;}
			if (CurrentVertex->num == CurrentLink->numB)
			{B = CurrentVertex;}
			CurrentVertex=CurrentVertex->next;
			
		}
		if (!(A && B)) return false;
		dc->MoveTo(A->x, A->y);
		dc->LineTo(B->x, B->y);
		CurrentLink = CurrentLink->next;
	}
	CurrentVertex = m_VertexStack;
	while (CurrentVertex)
	{
		int &x = CurrentVertex->x;
		int &y = CurrentVertex->y;
		dc->MoveTo(x, y);
		if (CurrentVertex->store)
		{
			CPen *LastPen = dc->GetCurrentPen();
			CPen GreenPen(PS_SOLID, 3, RGB(40, 150, 40));
			dc->SelectObject(GreenPen);
			dc->Ellipse(x - m_R, y - m_R, x + m_R, y + m_R);
			dc->SelectObject(LastPen);
		}
		else dc->Ellipse(x - m_R, y - m_R, x + m_R, y + m_R);
		CurrentVertex = CurrentVertex->next;
		
	}
	if(m_Selected)
	{
		int &x = m_Selected->x;
		int &y = m_Selected->y;
		CPen *LastPen = dc->GetCurrentPen();
		CPen BluePen (PS_SOLID,3,RGB(0, 0, 255));
		dc->SelectObject(BluePen);
		dc->Ellipse(x - m_R, y - m_R, x + m_R, y + m_R);
		dc->SelectObject(LastPen);
	}
	return true;
}
	


//------------------------------------------------------------------------------------------------------------

void CEditor::InitLinking()
{
	m_Mode = LINK;
}

//------------------------------------------------------------------------------------------------------------

void CEditor::InitAdding()
{
	m_Mode = VERTEX;
}
//------------------------------------------------------------------------------------------------------------

void CEditor::InitStores()
{
	m_Mode = STORE;
}

//------------------------------------------------------------------------------------------------------------
EEditorMode CEditor::GetMode()
{
	return m_Mode;
}
//------------------------------------------------------------------------------------------------------------

bool CEditor::Connect(int x, int y)
{
	if (m_Mode != LINK) return false;
	int dx, dy;
	bool Success = false;
	CVertexStack *Current = m_VertexStack;
	CVertexStack *B = 0;
	CVertexStack *A = m_Selected;
	if (!A)
	{
		AddVertex(x, y);
		return false;
	}
	while (Current)
	{
		dx = abs(Current->x - x);
		dy = abs(Current->y - y);
		if ((dx < m_R) && (dy < m_R))
		{
			B = Current;
			Current = 0;
			Success = true;
		}
		else if (Current)
		{
			Current = Current->next;
		}
	}
	if (!Success)
	{
		AddVertex(x, y);
		B = m_VertexStack;
	}
	CLinkStack *NewLink = new CLinkStack; 
	NewLink->numA = A->num;
	NewLink->numB = B->num;
	dx = A->x - B->x;
	dy = A->y - B->y;
	NewLink->distance = round(sqrt(dx*dx + dy*dy));
	NewLink->dir = 2;
	NewLink->next = m_LinkStack;
	m_LinkStack = NewLink;
	m_LinkCount ++;
	m_Selected = B;
	SaveAction(LINK);
	return true;
}

//------------------------------------------------------------------------------------------------------------

bool CEditor::Undo()
{
	//Извлекаем действие и удаляем запись, если есть
	EEditorMode Action;
	if (m_UndoStack)
	{
		Action = m_UndoStack->action;
		CUndoStack *trash = m_UndoStack;
		m_UndoStack = m_UndoStack->next;
		delete trash;
	}
	else return false;
	//Удаляем последнее добавленное что-либо
	switch (Action)
	{
		case LINK:
			if (m_LinkStack)
			{
				CLinkStack *trash = m_LinkStack;
				m_LinkStack = m_LinkStack->next;
				delete trash;
				m_LinkCount--;
				return true;
			}
		return false;
		case VERTEX:
			if (m_VertexStack)
			{
				CVertexStack *trash = m_VertexStack;
				m_VertexStack = m_VertexStack->next;
				if (trash == m_Selected) 
					m_Selected = NULL;
				delete trash;
				m_VertexCount--;
				return true;
			}
		return false;
		case STORE:
			if (m_StoreStack)
			{
				CVertexStack *StoreVertex = m_VertexStack;
				while (m_StoreStack->vertex != StoreVertex->num)
				{
					if (!StoreVertex) return false;
					StoreVertex=StoreVertex->next;
				}
				StoreVertex->store = false;

				CStoreStack *trash = m_StoreStack;
				m_StoreStack = m_StoreStack->next;
				delete trash;
				m_StoreCount--;
				return true;
			}
		return false;
	}
	return false;

}
//-----------------------------------------------------------------------------------------------------------
bool CEditor::SaveAction(EEditorMode Action)
{
	CUndoStack *Report = new CUndoStack;
	Report->action = Action;
	Report->next = m_UndoStack;
	m_UndoStack = Report;
	return true;
}
//------------------------------------------------------------------------------------------------------------

bool CEditor::Save(char* FileName)
{
	using namespace std;
	fstream SaveFile;
	SaveFile.open(FileName,fstream::trunc|fstream::out);
	
	SaveFile << "Vertexes:" << endl;
	SaveFile << m_VertexCount << endl;
	CVertexStack *Vertex;
	for (int i = 0; i < m_VertexCount;i++)
	{
		Vertex = m_VertexStack;
		bool notFound = true;
		while(notFound)
		{	
			if (!Vertex) return false;
			if (Vertex->num == i) notFound = false;
			else Vertex = Vertex->next;
		}
		SaveFile << Vertex->x << " ";
		SaveFile << Vertex->y << endl;
	}

	SaveFile << "Roads:" <<endl;
	SaveFile << m_LinkCount << endl;
	CLinkStack *Road = m_LinkStack;
	while (Road)
	{
		SaveFile << Road->numA << " ";
		SaveFile << Road->numB << " ";
		SaveFile << Road->distance << " ";
		SaveFile << Road->dir << endl;
		Road = Road->next;
	}
	return true;
}

//------------------------------------------------------------------------------------------------------------

bool CEditor::Open(char* FileName)
{
	using namespace::std;
	Clear();
	fstream SourceFile;
	SourceFile.open(FileName, fstream::in);
	if (!SourceFile) return false;
	string title;
	do { SourceFile >> title; } while (title != "Vertexes:" && !SourceFile.eof());
	if (title != "Vertexes:") return false;
	SourceFile >> m_VertexCount;
	for (int i = 0; i < m_VertexCount; i++)
	{
		CVertexStack *Loaded=new CVertexStack;
		Loaded->num=i;
		Loaded->store = false;
		SourceFile >> Loaded->x;
		SourceFile >> Loaded->y;
		Loaded->next = m_VertexStack;
		m_VertexStack = Loaded;
	}

	do { SourceFile >> title; } while (title != "Roads:" && !SourceFile.eof());
	if (title != "Roads:") return false;
	SourceFile >> m_LinkCount;
	for (int i = 0; i < m_LinkCount; i++)
	{
		CLinkStack *Loaded = new CLinkStack;
		SourceFile >> Loaded->numA;
		SourceFile >> Loaded->numB;
		SourceFile >> Loaded->distance;
		SourceFile >> Loaded->dir;
		Loaded->next = m_LinkStack;
		m_LinkStack = Loaded;
	}
	return true;
}	

//-------------------------------------------------------------------------------------------------------------

bool CEditor::SaveStores(char* FileName)
{
	using namespace std;
	fstream SaveFile;
	SaveFile.open(FileName, fstream::out | fstream::trunc);
	SaveFile << m_StoreCount<< endl;
	CStoreStack *Store=m_StoreStack;
	for (int i=0; i < m_StoreCount; i++)
	{
		if (!Store) return false;
		SaveFile << i<<" ";
		SaveFile << Store->vertex << " ";
		SaveFile << Store->ammout << endl;
		Store = Store->next;
	}
	return true;
}

bool CEditor::OpenStores(char* FileName)
{
	using namespace std;
	fstream SourceFile;
	SourceFile.open(FileName, fstream::in);
	SourceFile >> m_StoreCount;
	if (!SourceFile) return false;
	while (m_StoreStack)
	{
		CStoreStack *trash;
		trash = m_StoreStack;
		m_StoreStack = m_StoreStack->next;
		delete trash;
	}

	for (int i = 0; i < m_StoreCount; i++)
	{
		int waste;
		CStoreStack *Loaded= new CStoreStack;
		SourceFile >> waste;
		SourceFile >> Loaded->vertex;
		SourceFile >> Loaded->ammout;
		CVertexStack *StoreVertex=m_VertexStack;
		while (StoreVertex)
		{
			if (StoreVertex->num == Loaded->vertex)
			{
				StoreVertex->store = true;
				StoreVertex = NULL;
			}
			else StoreVertex = StoreVertex->next;	
			
		}
		Loaded->next = m_StoreStack;
		m_StoreStack = Loaded;
	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------