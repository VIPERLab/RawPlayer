


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <Windows.h>
#include "ChildWnd.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CClientWnd ��
class CClientWnd
{
public:
	static HWND m_hWnd;						// �ͻ����ھ��
	static HMENU m_hPopup;					// �����˵����

private:
	static UINT m_uChildNum;				// �Ӵ�����Ŀ
	static BOOL m_bScrabble;				// �Ƿ���ƴ��״̬

public:
	// �����ͻ�����
	static VOID Create();

	// ���ô��ڱ߿�
	static VOID SetEdge(BOOL bEdge = TRUE);

	// �����ļ�
	static BOOL Load(CChildWnd *pWnd);

	// �������д���
	static VOID OnPlayZoom(UINT uCmd = IDM_Play_ZoomActual);

	// �л���������״̬����ʾ
	static VOID ToggleBar();

	// �ر������Ӵ���
	static VOID CloseAll();

	// ʹ�����ڲ��ŵ�������Ч����Ч
	static VOID EnableCommand(BOOL bEnable = TRUE);

	// �Ӵ��ڹر�
	static VOID OnChildOpenClose(BOOL bOpen = FALSE);

	// ƴ�Ӵ���
	static VOID OnPlayScrabble();

	// ƴ�Ӵ���
	static VOID Scrabble();

public:
	// ��ȡ��Ϣ
	inline static UINT GetChildCount() {return m_uChildNum;}
	inline static BOOL IsScrabble() {return m_bScrabble;}
	inline static BOOL HasEdge() {return (GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_CLIENTEDGE);}

	// ��ȡ�Ӵ�����Ϣ
	inline static HWND GetActive() {return (HWND) SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0);}
	inline static CChildWnd *GetChildWnd(HWND hWnd = GetActive()) {return CChildWnd::GetWnd(hWnd);}
	inline static VOID SetStatusText(PCTSTR ptzText, INT iIndex = 0, HWND hWnd = GetActive()) {CChildWnd::SetStatusText(hWnd, ptzText, iIndex);}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
