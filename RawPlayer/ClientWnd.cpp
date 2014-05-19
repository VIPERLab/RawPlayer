


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#include "Main.h"
#include "ClientWnd.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClientWnd �ྲ̬����
HWND CClientWnd::m_hWnd = NULL;
HMENU CClientWnd::m_hPopup = NULL;
UINT CClientWnd::m_uChildNum = 0;
BOOL CClientWnd::m_bScrabble = FALSE;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����ͻ�����
VOID CClientWnd::Create()
{
	WNDCLASSEX wceClass;
	CLIENTCREATESTRUCT ccsClient;	

	// �����ͻ�����
	m_hPopup = GetSubMenu(CMainWnd::m_hMenu, 1);
	ccsClient.hWindowMenu = GetSubMenu(CMainWnd::m_hMenu, 3);
	ccsClient.idFirstChild = IDM_Window_First;
	m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("MDICLIENT"), NULL, WS_VISIBLE | WS_CHILD, 
		0, 0, 0, 0, CMainWnd::m_hWnd, NULL, g_hInst, &ccsClient);

	// ע���Ӵ�����
	wceClass.cbSize = sizeof(WNDCLASSEX);
	wceClass.style = CS_VREDRAW | CS_HREDRAW;
	wceClass.lpfnWndProc = CChildWnd::ClildWndProc;
	wceClass.cbClsExtra = 0;
	wceClass.cbWndExtra = 0;
	wceClass.hInstance = g_hInst;
	wceClass.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_File));
	wceClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wceClass.hbrBackground = (HBRUSH) COLOR_BTNSHADOW;
	wceClass.lpszMenuName = NULL;
	wceClass.lpszClassName = STR_AppName TEXT("ChildWnd");
	wceClass.hIconSm = NULL;
	RegisterClassEx(&wceClass);

	// ע�Ქ�Ŵ�����
	wceClass.lpfnWndProc = CChildWnd::PlayWndProc;
	wceClass.hbrBackground = NULL;//(HBRUSH) COLOR_WINDOWTEXT;
	wceClass.lpszClassName = STR_AppName TEXT("PlayWnd");
	RegisterClassEx(&wceClass);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ô��ڱ߿�
VOID CClientWnd::SetEdge(BOOL bEdge)
{
	DWORD dwExStyle;

	dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (bEdge)
	{
		_ReturnIf(dwExStyle & WS_EX_CLIENTEDGE);
		dwExStyle |= WS_EX_CLIENTEDGE;
	}
	else
	{
		_ReturnIf((dwExStyle & WS_EX_CLIENTEDGE) == FALSE);
		dwExStyle &= ~WS_EX_CLIENTEDGE;
	}
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);
	SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
		SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����ļ�
BOOL CClientWnd::Load(CChildWnd *pWnd)
{
	HWND hWnd;
	TCHAR tzText[MAX_PATH];

	// ����ļ��Ƿ�������
	_Assert(pWnd);
	hWnd = GetWindow(m_hWnd, GW_CHILD);
	for (hWnd = GetWindow(hWnd, GW_HWNDFIRST); hWnd; hWnd = GetWindow(hWnd, GW_HWNDNEXT))
	{
		// ����ļ�������
		GetWindowText(hWnd, tzText, MAX_PATH);
		if (lstrcmpi(tzText, pWnd->m_tzFileName) == 0)
		{
			CChildWnd *p = GetChildWnd(hWnd);
			if (p)
			{
				// �����ʽ��ͬ
				if (*pWnd == *p)
				{
					delete pWnd;
					SendMessage(m_hWnd, WM_MDIACTIVATE, (WPARAM) hWnd, 0);
					return TRUE;
				}
			}

			// �ر��Ѵ򿪵��ļ���Ȼ���������µĸ�ʽ��������
			SendMessage(m_hWnd, WM_MDIDESTROY, (WPARAM) hWnd, 0);
			break;
		}
	}

	// �����Ӵ���
	if (pWnd->Create())
	{
		OnChildOpenClose(TRUE);
		return TRUE;
	}

	delete pWnd;

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������д���
VOID CClientWnd::OnPlayZoom(UINT uCmd)
{
	RECT rtMain;
	RECT rtClient;
	RECT rtChild;

	SendMessage(m_hWnd, WM_MDICASCADE, 0, 0);
	HWND hWnd = GetWindow(m_hWnd, GW_CHILD);
	for (hWnd = GetWindow(hWnd, GW_HWNDFIRST); hWnd; )
	{
		_Assert(GetChildWnd(hWnd));
		GetChildWnd(hWnd)->OnPlayZoom(uCmd, FALSE);
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}

	if (m_bScrabble)
	{
		GetWindowRect(CMainWnd::m_hWnd, &rtMain);
		GetClientRect(m_hWnd, &rtClient);
		hWnd = GetWindow(GetWindow(m_hWnd, GW_CHILD), GW_HWNDFIRST);
		GetWindowRect(hWnd, &rtChild);

		MoveWindow(CMainWnd::m_hWnd, rtMain.left, rtMain.top, 
			_RectWidth(rtMain) - _RectWidth(rtClient) + _RectWidth(rtChild),
			_RectHeight(rtMain) - _RectHeight(rtClient) + _RectHeight(rtChild), TRUE);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �л���������״̬����ʾ
VOID CClientWnd::ToggleBar()
{
	HWND hWnd;

	hWnd = GetWindow(m_hWnd, GW_CHILD);
	for (hWnd = GetWindow(hWnd, GW_HWNDFIRST); hWnd; )
	{
		_Assert(GetChildWnd(hWnd));
		GetChildWnd(hWnd)->OnSize();
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ر������Ӵ���
VOID CClientWnd::CloseAll()
{
	HWND hWnd;
	HWND hDestroy;

	hWnd = GetWindow(m_hWnd, GW_CHILD);
	for (hWnd = GetWindow(hWnd, GW_HWNDFIRST); hWnd; )
	{
		hDestroy = hWnd;
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
		SendMessage(m_hWnd, WM_MDIDESTROY, (WPARAM) hDestroy, 0);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ʹ�����ڲ��ŵ�������Ч����Ч
VOID CClientWnd::EnableCommand(BOOL bEnable)
{
	UINT i;

	CMainWnd::EnableCommand(IDM_File_Export, bEnable);
	CMainWnd::EnableCommand(IDM_View_Toolbar, bEnable);
	CMainWnd::EnableCommand(IDM_View_StatusBar, bEnable);
	for (i = IDM_Play_Play; i <= IDM_Play_ZoomDouble; i++)
	{
		CMainWnd::EnableCommand(i, bEnable);
	}
	for (i = IDM_Window_Cascade; i <= IDM_Window_CloseAll; i++)
	{
		CMainWnd::EnableCommand(i, bEnable);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
VOID CClientWnd::OnChildOpenClose(BOOL bOpen)
{
	_ExIf(m_bScrabble, OnPlayScrabble());

	if (bOpen)
	{
		m_uChildNum++;
		if (m_uChildNum == 1)
		{
			EnableCommand();
			CMainWnd::EnableCommand(IDM_Play_Synchronize, FALSE);
			CMainWnd::EnableCommand(IDM_Play_Scrabble, FALSE);
		}
		else if (m_uChildNum == 2)
		{
			CMainWnd::EnableCommand(IDM_Play_Synchronize, TRUE);
			CMainWnd::EnableCommand(IDM_Play_Scrabble, TRUE);
		}
	}
	else
	{
		m_uChildNum--;
		if (m_uChildNum <= 1)
		{
			if (m_uChildNum == 0)
			{
				// �����Ӵ������״̬
				BOOL bMaximized = (HasEdge() == FALSE);
				CIni::SetInt(INI_MaxOnOpen, bMaximized);
				_ExIf(bMaximized, SetEdge());

				EnableCommand(FALSE);
			}
			CMainWnd::EnableCommand(IDM_Play_Synchronize, FALSE);
			CMainWnd::EnableCommand(IDM_Play_Scrabble, FALSE);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ƴ�Ӵ���
VOID CClientWnd::OnPlayScrabble()
{
	// ���ò˵�״̬
	m_bScrabble = !CMainWnd::IsMenuChecked(IDM_Play_Scrabble);
	CMainWnd::CheckCommand(IDM_Play_Scrabble, m_bScrabble);

	// ͬ��
	if (CRawPlayer::m_bSync == FALSE)
	{
		SendMessage(CMainWnd::m_hWnd, WM_COMMAND, IDM_Play_Synchronize, 0);
	}

	// ����������
	if (m_bScrabble)
	{
		OnPlayZoom();
	}

	Scrabble();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ƴ�Ӵ���
VOID CClientWnd::Scrabble()
{
	UINT i;
	RECT rtClient;
	DWORD dwWidth;
	DWORD dwStyle;
	CChildWnd *pWnd;

	_ReturnIf(m_uChildNum == 0);

	// ��ȡ�ͻ�����
	GetClientRect(m_hWnd, &rtClient);
	dwWidth = _RectWidth(rtClient) / m_uChildNum;
	for (pWnd = CChildWnd::m_pHead, i = 0; pWnd; pWnd = pWnd->m_pNext, i++)
	{
		// �����Ӵ���
		dwStyle = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
		if (m_bScrabble)
		{
			dwStyle &= ~WS_CAPTION;
			dwStyle &= ~WS_THICKFRAME;
		}
		else
		{
			dwStyle |= WS_CAPTION | WS_THICKFRAME;
		}
		SetWindowLong(pWnd->m_hWnd, GWL_STYLE, dwStyle);
		if (m_bScrabble == FALSE)
		{
			SetWindowPos(pWnd->m_hWnd, NULL, 0, 0, 0, 0, 
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
		}

		// ���ò��Ŵ���
		dwStyle = GetWindowLong(pWnd->m_hPlayWnd, GWL_EXSTYLE);
		if (m_bScrabble)
		{
			dwStyle &= ~WS_EX_CLIENTEDGE;
		}
		else
		{
			dwStyle |= WS_EX_CLIENTEDGE;
		}
		SetWindowLong(pWnd->m_hPlayWnd, GWL_EXSTYLE, dwStyle);
		if (m_bScrabble == FALSE)
		{
			SetWindowPos(pWnd->m_hPlayWnd, NULL, 0, 0, 0, 0, 
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
		}

		if (m_bScrabble)
		{
			// �ƶ��Ӵ���
			MoveWindow(pWnd->m_hWnd, dwWidth * i, 0, dwWidth, _RectHeight(rtClient), TRUE);
		}

		if (m_bScrabble)
		{
			RECT rtRect;
			rtRect.left = pWnd->m_uWidth * i / m_uChildNum;
			rtRect.right = rtRect.left + (pWnd->m_uWidth / m_uChildNum);
			rtRect.top = 0;
			rtRect.bottom = pWnd->m_uHeight;
			pWnd->SetSrcRect(&rtRect);
		}
		else
		{
			pWnd->SetSrcRect();
		}
	}

	if (m_bScrabble == FALSE)
	{
		SendMessage(m_hWnd, WM_MDICASCADE, 0, 0);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
