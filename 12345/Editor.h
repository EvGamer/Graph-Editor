#pragma once
#include "afxwin.h" 

enum EEditorMode { VERTEX, LINK, STORE };

class CEditor
{
public:
	CEditor();
	~CEditor();
	int Select(int x,int y);
	bool Connect(int x,int y);
	bool Undo();
	bool AddVertex(int x,int y);
	bool AddStore(int Ammout);
	bool Draw(CPaintDC *dc);
	bool Save(char* FileName);
	bool SaveStores(char* FileName);
	bool Open(char* FileName);
	bool OpenStores(char* FileName);
	void Clear();

	void InitLinking();
	void InitAdding();
	void InitStores();
	EEditorMode GetMode();
	inline int GetSelected(){ if (m_Selected)return m_Selected->num; return -1; }

protected:
	bool SaveAction(EEditorMode Action);
	//----------------------------------------
	struct CVertexStack
	{
		int num, x, y;
		bool store;
		CVertexStack *next;
	};
	struct CLinkStack
	{
		int numA, numB;
		int distance;
		int dir;
		CLinkStack *next;
	};
	struct CStoreStack
	{
		int vertex;
		int ammout;
		CStoreStack *next;
	};
	struct CUndoStack
	{
		EEditorMode action;
		CUndoStack *next;
	};
	//------------------------------------------------
	CVertexStack *m_VertexStack;
	CVertexStack *m_Selected;
	EEditorMode m_Mode;
	CLinkStack *m_LinkStack;
	CStoreStack *m_StoreStack;
	CUndoStack *m_UndoStack;
	int m_StoreCount;
	int m_VertexCount;
	int m_LinkCount;
	const int m_R=5;
};

