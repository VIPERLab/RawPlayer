


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#include "Main.h"
#include "ChildWnd.h"

#pragma warning(disable: 4244)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SBI_Information	0
#define SBI_ColorSpace	1
#define SBI_Width		2
#define SBI_Height		3
#define SBI_FrameRate	4
#define SBI_FrameStep	5
#define SBI_CurFrame	6
#define SBI_TotalFrame	7
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CChildWnd �ྲ̬����
CChildWnd *CChildWnd::m_pHead = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���캯��
CChildWnd::CChildWnd()
{
	// ��ʼ������ָ��
	m_pNext = m_pHead;
	m_pHead = this;

	// ��ʼ������
	m_hWnd = NULL;
	m_uEditBox = 0;
	m_bCancel = FALSE;
	m_bExporting = FALSE;
	m_bPrevPlaying = FALSE;
	m_bStartOfScroll = TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
CChildWnd::~CChildWnd()
{
	UINT i;

	// �ȴ�����
	m_bCancel = TRUE;
	while (m_bExporting)
	{
		Sleep(5);
	}

	// ɾ��������ͼ���б�
	for (i = 0; i < _NumOf(m_hImageLists); i++)
	{
		ImageList_Destroy(m_hImageLists[i]);
	}

	if (m_pHead == this)
	{
		m_pHead = m_pNext;
	}
	else
	{
		for (CChildWnd *p = m_pHead; p; p = p->m_pNext)
		{
			if (p->m_pNext == this)
			{
				p->m_pNext = m_pNext;
				break;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���캯��
HWND CChildWnd::Create()
{
	// ת����ʼ
	Restart();

	// ��ȡ����״̬
	BOOL bMaximized = CClientWnd::GetChildCount() ? 
		((GetWindowLong(CClientWnd::m_hWnd, GWL_EXSTYLE) & WS_EX_CLIENTEDGE) == FALSE) : CIni::GetInt(INI_MaxOnOpen);

	// ��������	
	m_hWnd = CreateMDIWindow(STR_AppName TEXT("ChildWnd"), m_tzFileName, 0, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		CClientWnd::m_hWnd, g_hInst, (LPARAM) this);

	if (m_hWnd)
	{
		// �������Ŵ��ڴ�С
		SetVideoSize(m_uWidth, m_uHeight);
		_ExIf(bMaximized, ShowWindow(m_hWnd, SW_MAXIMIZE));

		// ��������
		SetPlayWnd(m_hPlayWnd);
		_ExIf(CIni::GetInt(INI_DirectDraw, TRUE), EnableDDraw());
		_ExIf(CIni::GetInt(INI_PlayOnOpen), Play());
	}

	return m_hWnd;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �Ӵ��ڻص�����
LRESULT CALLBACK CChildWnd::ClildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CChildWnd *pThis;
	LPMDICREATESTRUCT pmcsCreate;

	switch (uMsg)
	{
	case WM_CREATE:
		// ��ȡ this ָ��
		_Assert(lParam);
		pmcsCreate = (LPMDICREATESTRUCT) ((LPCREATESTRUCT) lParam)->lpCreateParams;
		_Assert(pmcsCreate);
		pThis = (CChildWnd *) pmcsCreate->lParam;
		_Assert(pThis);		
		pThis->m_hWnd = hWnd;
		pThis->OnCreate();
		break;

	case WM_WINDOWPOSCHANGED:
		// λ�øı�
		GetWnd(hWnd)->SetDrawRect();
		break;

	case WM_SIZE:
		// �ߴ�ı�
		if (wParam != SIZE_MINIMIZED)
		{
			_Assert(GetWnd(hWnd));
			GetWnd(hWnd)->OnSize(wParam, lParam);
		}
		break;

	case WM_INITMENUPOPUP:
		if (GetMenuItemID((HMENU) wParam, 0) == IDM_Play_Play)
		{
			GetWnd(hWnd)->OnMenuPopup();
		}
		break;

	case WM_NOTIFY:
	case WM_MENUSELECT:
	case WM_EXITMENULOOP:
		// ���ù�������״̬����ʾ��Ϣ
		OnNotify(hWnd, uMsg, wParam, lParam);
		break;

	case WM_COMMAND:
		// ������Ϣ
		GetWnd(hWnd)->OnCommand(wParam, lParam);
		break;

	case WM_KILLFOCUS:
		// ȡ��
		_ExIf(GetWnd(hWnd)->m_bExporting, SetFocus(hWnd));
		break;

	case WM_KEYDOWN:
		// ȡ��
		_ExIf(wParam == VK_ESCAPE, GetWnd(hWnd)->m_bCancel = TRUE);
		break;

	case WM_NCDESTROY:
		// ��������
		CClientWnd::OnChildOpenClose();
		_Assert(GetWnd(hWnd));
		delete GetWnd(hWnd);
		break;
	}

	return DefMDIChildProc(hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���Ŵ��ڻص�����
LRESULT CALLBACK CChildWnd::PlayWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CONTEXTMENU:
		// ���������Ĳ˵�
		TrackPopupMenu(CClientWnd::m_hPopup, 0, LOWORD(lParam), HIWORD(lParam), 0, GetParent(hWnd), NULL);
		break;

	case WM_ERASEBKGND:
		_Assert(GetWnd(hWnd));
		if (GetWnd(hWnd)->IsPlaying() == FALSE)
		{
			GetWnd(hWnd)->Draw((HDC) wParam);
		}
		return TRUE;

	case WM_COMMAND:
		GetWnd(hWnd)->OnCommand(wParam, lParam);
		return 0;

	case WM_LBUTTONDOWN:
		ShowWindow(GetWnd(hWnd)->m_hEditBox, SW_HIDE);
		//GetWnd(hWnd)->StepFrame((wParam & MK_CONTROL) ? -1 : 1);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���������ڻص�����
LRESULT CALLBACK CChildWnd::ToolbarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_HSCROLL)
	{
		SendMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
	}

	return CallWindowProc((WNDPROC) GetWindowLongPtr(hWnd, GWLP_USERDATA), hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �༭���ڻص�����
LRESULT CALLBACK CChildWnd::EditBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KILLFOCUS:
		SendMessage(GetParent(hWnd), SB_SETTEXT, SBT_NOBORDERS, (LPARAM) LNG_Ready);
		ShowWindow(hWnd, SW_HIDE);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			ShowWindow(hWnd, SW_HIDE);
		}
		else if (wParam == VK_RETURN)
		{
			PostMessage(GetParent(GetParent(hWnd)), WM_COMMAND, MAKELONG(IDC_EditBox, EN_KILLFOCUS), (LPARAM) hWnd);
		}
		break;
	}

	return CallWindowProc((WNDPROC) GetWindowLongPtr(hWnd, GWLP_USERDATA), hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ڱ�����
VOID CChildWnd::OnCreate()
{
	const TBBUTTON TBB_Button[] =
	{
		{0,		0,						0,					TBSTYLE_SEP,		0,	0},
		{0,		IDM_File_Open,			TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
		{1,		IDM_File_Export,		TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
		{0,		0,						TBSTATE_ENABLED,	TBSTYLE_SEP,		0,	0},
		{2,		IDM_Play_Play,			TBSTATE_ENABLED,	TBSTYLE_CHECK,		0,	0},
		{0,		0,						0,					TBSTYLE_SEP,		0,	0},
		{3,		IDM_Play_GotoStart,		TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
		{4,		IDM_Play_StepBackward,	TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
		{5,		IDM_Play_StepForward,	TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
		{6,		IDM_Play_GotoEnd,		TBSTATE_ENABLED,	TBSTYLE_BUTTON,		0,	0},
		{0,		0,						0,					TBSTYLE_SEP,		0,	0},
	};

	// ��¼ this ָ��
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR) this);

	// ��������ʼ��״̬��
	m_hStatusBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m_hWnd, IDC_StatusBar);
	m_hEditBox = CreateWindowEx(0, TEXT("EDIT"), NULL, WS_CHILD | ES_UPPERCASE, 
		0, 0, 0, 0, m_hStatusBar, (HMENU) IDC_EditBox, g_hInst, (PVOID) this);
	SendMessage(m_hEditBox, WM_SETFONT, (WPARAM) SendMessage(m_hStatusBar, WM_GETFONT, 0, 0), 0);
	SetWindowLongPtr(m_hEditBox, GWLP_USERDATA, (LONG_PTR) SetWindowLongPtr(m_hEditBox, GWLP_WNDPROC, (LONG_PTR) EditBoxProc));
	AdjustStatusBar();

	// ����״̬��
	SetStatusText(LNG_Ready);
	OnCommand(IDC_RawVideo_Change);

	// ����������
	m_hToolbar = CUtility::CreateToolbar(WS_VISIBLE | WS_CHILD | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT,
		m_hWnd, MAKEINTRESOURCE(IDB_Toolbar), m_hImageLists, _NumOf(TBB_Button), TBB_Button);

	// ���������ö�λ��
	m_hSeekBar = CreateWindowEx(0, TRACKBAR_CLASS, NULL, 
		WS_CHILD | WS_VISIBLE | TBS_HORZ,
		0, 0, 0, 0, m_hToolbar, (HMENU) IDC_SeekBar, g_hInst, NULL);
	SendMessage(m_hSeekBar, TBM_SETTICFREQ, 1, 0);
	SendMessage(m_hSeekBar, TBM_SETRANGE, TRUE, MAKELONG(0, m_uTotalFrame - 1));
	SendMessage(m_hSeekBar, TBM_SETPAGESIZE, 0, (m_uTotalFrame + 9) / 10);
	SetWindowLongPtr(m_hToolbar, GWLP_USERDATA, (LONG_PTR) SetWindowLongPtr(m_hToolbar, GWLP_WNDPROC, (LONG_PTR) ToolbarWndProc));

	// �������Ŵ���
	m_hPlayWnd = CreateWindowEx(WS_EX_CLIENTEDGE, STR_AppName TEXT("PlayWnd"), STR_AppName TEXT("PlayWnd"), WS_CHILD | WS_VISIBLE, 
		0, 0, 0, 0, m_hWnd, (HMENU) IDC_PlayWnd, g_hInst, (PVOID) this);
	SetWindowLongPtr(m_hPlayWnd, GWLP_USERDATA, (LONG_PTR) this);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��С�ı�
VOID CChildWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	RECT rtRect;
	INT iToolbar;
	INT iStatusBar;
	SIZE sizeToolbar;

	// ���ָ��Ҫ��ȡ���ڴ�С
	if (lParam == 0)
	{
		GetClientRect(m_hWnd, &rtRect);
		lParam = MAKELONG(_RectWidth(rtRect), _RectHeight(rtRect));
	}

	// ��ȡ��������״̬���ĸ߶�
	if (CMainWnd::IsMenuChecked(IDM_View_Toolbar))
	{
		ShowWindow(m_hToolbar, SW_SHOW);
		GetWindowRect(m_hToolbar, &rtRect);
		iToolbar = _RectHeight(rtRect);
	}
	else
	{
		ShowWindow(m_hToolbar, SW_HIDE);
		iToolbar = 0;
	}
	if (CMainWnd::IsMenuChecked(IDM_View_StatusBar))
	{
		ShowWindow(m_hStatusBar, SW_SHOW);
		GetWindowRect(m_hStatusBar, &rtRect);
		iStatusBar = _RectHeight(rtRect);
	}
	else
	{
		ShowWindow(m_hStatusBar, SW_HIDE);
		iStatusBar = 0;
	}

	// �������Ŵ��ڴ�С��λ��
	MoveWindow(m_hPlayWnd, 0, iToolbar, LOWORD(lParam), HIWORD(lParam) - iToolbar - iStatusBar, TRUE);
	SetDrawRect();

	// ����״̬����С��λ��
	AdjustStatusBar(wParam, lParam);

	// ������������С��λ��
	SendMessage(m_hToolbar, TB_AUTOSIZE, 0, 0);

	// ������λ���Ĵ�С
	SendMessage(m_hToolbar, TB_GETMAXSIZE, 0, (LPARAM) &sizeToolbar);
	MoveWindow(m_hSeekBar, sizeToolbar.cx, 1, LOWORD(lParam) - sizeToolbar.cx, sizeToolbar.cy - 2, TRUE);

	// �����ͻ����ڱ߿�
	if (m_hWnd == CClientWnd::GetActive())
	{
		CClientWnd::SetEdge(wParam != SIZE_MAXIMIZED);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ֪ͨ��Ϣ
VOID CChildWnd::OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch ((uMsg == WM_NOTIFY)	? ((LPNMHDR) lParam)->code : uMsg)
	{
	case TTN_NEEDTEXT:
		// ��ʾ��������ʾ
		GetMenuString(CMainWnd::m_hMenu, (UINT) wParam, ((LPTOOLTIPTEXT) lParam)->szText, 80, MF_BYCOMMAND);

	case WM_MENUSELECT:
		// ��״̬����ʾ��Ӧ�Ĳ˵�����ʾ
		CLanguage::TranslateString(LOWORD(wParam));
		if (CLanguage::m_tzText[0])
		{
			SetStatusText(hWnd, CLanguage::m_tzText);
			break;
		}

	case TTN_POP:
	case WM_EXITMENULOOP:
		// ��״̬����ʾ��������
		SetStatusText(hWnd, LNG_Ready);
		break;

	case NM_CLICK:
		if (((LPNMHDR) lParam)->idFrom == IDC_StatusBar)
		{
			GetWnd(hWnd)->OnClickStatusBar((UINT) ((LPNMMOUSE) lParam)->dwItemSpec);
		}
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���״̬��
VOID CChildWnd::OnClickStatusBar(UINT uIndex)
{
	UINT i;
	RECT rtRect;
	DWORD dwStyle;
	TCHAR tzText[512];
	TCHAR tzCSConv[MAX_PATH];

	// ������ڿ��Ա༭����Ŀ���
	_ReturnIf(m_bExporting);
	m_uEditBox = uIndex;
	if ((m_uEditBox >= SBI_ColorSpace) && (m_uEditBox <= SBI_CurFrame))
	{
		// �ƶ��༭����Ӧ��Ŀ
		SendMessage(m_hStatusBar, SB_GETRECT, m_uEditBox, (LPARAM) &rtRect);
		InflateRect(&rtRect, -2, -2);
		MoveWindow(m_hEditBox, rtRect.left, rtRect.top, _RectWidth(rtRect), _RectHeight(rtRect), TRUE);

		// ���ñ༭�����״̬����ʾ
		dwStyle = GetWindowLong(m_hEditBox, GWL_STYLE);
		dwStyle |= ES_NUMBER;
		switch (m_uEditBox)
		{
		case SBI_ColorSpace:
			tzCSConv[0] = 0;
			for (i = 0; i < _NumOf(c_tzColorSpace); i++)
			{
				if (IsCharUpper(c_tzColorSpace[i][0]))
				{
					lstrcat(tzCSConv, c_tzColorSpace[i]);
					if (i < _NumOf(c_tzColorSpace) - 1)
					{
						lstrcat(tzCSConv, TEXT("/"));
					}
				}
			}
			wsprintf(tzText, LNG_ColorSpaceMsg, tzCSConv);
			dwStyle &= ~ES_NUMBER;
			break;

		case SBI_Width:
			wsprintf(tzText, LNG_FrameWidthMsg, MIN_Resolution, MAX_Resolution);
			break;

		case SBI_Height:
			wsprintf(tzText, LNG_FrameHeightMsg, MAX_Resolution, MIN_Resolution, MIN_Resolution, MAX_Resolution);
			dwStyle &= ~ES_NUMBER;
			break;

		case SBI_FrameRate:
			wsprintf(tzText, LNG_FrameRateMsg, 1, MAX_FrameRate);
			break;

		case SBI_FrameStep:
			wsprintf(tzText, LNG_FrameStepMsg, (INT) (1 - m_uTotalFrame), m_uTotalFrame - 1);
			dwStyle &= ~ES_NUMBER;
			break;

		case SBI_CurFrame:
			wsprintf(tzText, LNG_FrameIndexeMsg, 1, m_uTotalFrame);
			break;
		}
		SetStatusText(tzText);
		SetWindowLong(m_hEditBox, GWL_STYLE, dwStyle);

		// ���ñ༭�����ݲ���ʾ�༭��
		SendMessage(m_hStatusBar, SB_GETTEXT, m_uEditBox, (LPARAM) tzText);
		SetWindowText(m_hEditBox, tzText);
		ShowWindow(m_hEditBox, SW_SHOW);
		SetFocus(m_hEditBox);
		SendMessage(m_hEditBox, EM_SETSEL, 0, -1);
	}
	else
	{
		ShowWindow(m_hEditBox, SW_HIDE);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����
VOID CChildWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_File_Open:
		PostMessage(CMainWnd::m_hWnd, WM_COMMAND, IDM_File_Open, 0);
		break;

	case IDM_File_Export:
		OnFileExport();
		break;

	case IDM_Play_Play:
		TogglePlay();
		break;

	case IDM_Play_GotoStart:
		Stop();
		Seek(0);
		break;

	case IDM_Play_GotoEnd:
		Stop();
		Seek(MAX_Frame);
		break;

	case IDM_Play_StepBackward:
		Stop();
		Step(-1);
		break;

	case IDM_Play_StepForward:
		Stop();
		Step(1);
		break;

	case IDM_Play_Synchronize:
		CMainWnd::CheckCommand(IDM_Play_Synchronize, CRawPlayer::ToggleSync());
		break;

	case IDM_Play_Scrabble:
		CClientWnd::OnPlayScrabble();
		break;

	case IDM_Play_GdiDraw:
		// �л�Ϊ GDI ��ʽ
		if (IsDDraw())
		{
			EnableDDraw(FALSE);
			Draw();
			CIni::SetInt(INI_DirectDraw, FALSE);
		}
		break;

	case IDM_Play_DirectDraw:
		// �л�Ϊ DirectDraw ��ʽ
		if (IsDDraw() == FALSE)
		{
			HRESULT hResult = EnableDDraw();
			if (SUCCEEDED(hResult))
			{
				Draw();
				CIni::SetInt(INI_DirectDraw, TRUE);
			}
			else
			{
				TCHAR tzText[MAX_PATH];
				if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, (DWORD) hResult, 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), tzText, MAX_PATH, NULL) == 0)	
				{
					wsprintf(tzText, LNG_DirectDrawErr, hResult);
				}
				CMainWnd::ErrorBox(tzText);
				CIni::SetInt(INI_DirectDraw, FALSE);
			}
		}
		break;

	case IDM_Play_YChannel:
	case IDM_Play_UChannel:
	case IDM_Play_VChannel:
	case IDM_Play_RChannel:
	case IDM_Play_GChannel:
	case IDM_Play_BChannel:
		OnPlayChannel(LOWORD(wParam));
		break;

	case IDM_Play_ZoomHalf:
	case IDM_Play_ZoomActual:
	case IDM_Play_ZoomDouble:
		if (CRawPlayer::m_bSync || CClientWnd::IsScrabble())
		{
			CClientWnd::OnPlayZoom(LOWORD(wParam));
		}
		else
		{
			OnPlayZoom(LOWORD(wParam));
		}
		break;

	case TB_THUMBPOSITION:
		Seek((DWORD) SendMessage(m_hSeekBar, TBM_GETPOS, 0, 0));
		break;

	case TB_PAGEUP:
	case TB_PAGEDOWN:
	case TB_ENDTRACK:
	case TB_THUMBTRACK:
		// ��ͣ
		if (m_bStartOfScroll)
		{
			m_bStartOfScroll = FALSE;
			m_bPrevPlaying = Stop();
		}

		// ��λ
		Seek((DWORD) SendMessage(m_hSeekBar, TBM_GETPOS, 0, 0));

		// �ָ�״̬
		if (LOWORD(wParam) == TB_ENDTRACK)
		{
			if (m_bPrevPlaying)
			{
				Play();
			}
			m_bStartOfScroll = TRUE;
		}

		break;

	case IDC_RawVideo_Play:
		// ���Ż�ֹͣ
		SendMessage(m_hToolbar, TB_CHECKBUTTON, IDM_Play_Play, lParam);
		break;

	case IDC_RawVideo_Seek:
		// �ض�λ
		SendMessage(m_hSeekBar, TBM_SETPOS, TRUE, lParam);
		SetStatusText(m_uCurFrame + 1, SBI_CurFrame);
		break;

	case IDC_RawVideo_Change:
		// ��ʾ��ʽ��Ϣ
		SetStatusText(c_tzColorSpace[m_csColorSpace], SBI_ColorSpace);
		SetStatusText(m_uWidth, SBI_Width);
		SetStatusText(m_iHeight, SBI_Height);
		SetStatusText(m_uFrameRate, SBI_FrameRate);
		SetStatusText(m_iFrameStep, SBI_FrameStep);
		SetStatusText(m_uCurFrame + 1, SBI_CurFrame);
		SetStatusText(m_uTotalFrame, SBI_TotalFrame);
		break;

	case IDC_EditBox:
		// �༭��ʽ
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			if (OnChangeFormat())
			{
				SetStatusText(LNG_Ready);
				OnClickStatusBar(m_uEditBox + 1);
			}
			else
			{
				ShowWindow(m_hEditBox, SW_HIDE);
				SetStatusText(LNG_InputErr);
			}
		}
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����
VOID CChildWnd::OnFileExport()
{
	if (m_bExporting == FALSE)
	{
		BOOL bPlay = Stop();
		if (CExportDlg::Show(this))
		{
			_BeginThread(vExportProc, this);
		}
		else if (bPlay)
		{
			Play();
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ͨ���˵�
VOID CChildWnd::OnMenuPopup()
{
	CMainWnd::CheckCommand(IDM_Play_Play, IsPlaying());

	CMainWnd::CheckRadioCommand(IDM_Play_GdiDraw, IDM_Play_DirectDraw, 
		IsDDraw() ? IDM_Play_DirectDraw : IDM_Play_GdiDraw);

	CMainWnd::CheckCommand(IDM_Play_YChannel, ((GetWipeChannel() & CH_WIPEY) == FALSE));
	CMainWnd::CheckCommand(IDM_Play_UChannel, ((GetWipeChannel() & CH_WIPEU) == FALSE));
	CMainWnd::CheckCommand(IDM_Play_VChannel, ((GetWipeChannel() & CH_WIPEV) == FALSE));
	CMainWnd::CheckCommand(IDM_Play_RChannel, ((GetWipeChannel() & CH_WIPER) == FALSE));
	CMainWnd::CheckCommand(IDM_Play_GChannel, ((GetWipeChannel() & CH_WIPEG) == FALSE));
	CMainWnd::CheckCommand(IDM_Play_BChannel, ((GetWipeChannel() & CH_WIPEB) == FALSE));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ͨ��
VOID CChildWnd::OnPlayChannel(UINT uCmd)
{
	DWORD dwChannel;

	switch (uCmd)
	{
	case IDM_Play_YChannel:
		dwChannel = CH_WIPEY;
		break;

	case IDM_Play_UChannel:
		dwChannel = CH_WIPEU;
		break;

	case IDM_Play_VChannel:
		dwChannel = CH_WIPEV;
		break;

	case IDM_Play_RChannel:
		dwChannel = CH_WIPER;
		break;

	case IDM_Play_GChannel:
		dwChannel = CH_WIPEG;
		break;

	case IDM_Play_BChannel:
		dwChannel = CH_WIPEB;
		break;
	}

	SetWipeChannel(dwChannel, CMainWnd::IsMenuChecked(uCmd));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����
VOID CChildWnd::OnPlayZoom(UINT uCmd, BOOL bCascade)
{
	DWORD dwWidth;
	DWORD dwHeight;

	_ExIf(bCascade && (CClientWnd::HasEdge() == FALSE), SendMessage(CClientWnd::m_hWnd, WM_MDICASCADE, 0, 0));
	dwWidth = m_uWidth;
	dwHeight = m_uHeight;
	if (uCmd == IDM_Play_ZoomHalf)
	{
		dwWidth /= 2;
		dwHeight /= 2;
	}
	else if (uCmd == IDM_Play_ZoomDouble)
	{
		dwWidth *= 2;
		dwHeight *= 2;
	}
	SetVideoSize(dwWidth, dwHeight);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����״̬����С��λ��
VOID CChildWnd::AdjustStatusBar(WPARAM wParam, LPARAM lParam)
{
	INT i;
	INT j;
	RECT rtRect;
	INT iParts[] = {-1, 50, 35, 40, 35, 35, 50, 50};

	for (i = 0; i < _NumOf(iParts); i++)
	{
		iParts[i] = LOWORD(lParam);
		_ExIf(wParam != SIZE_MAXIMIZED, iParts[i] -= 16);
		for (j = i + 1; j < _NumOf(iParts); j++)
		{
			iParts[i] -= iParts[j];
		}
	}
	_ExIf(wParam == SIZE_MAXIMIZED, iParts[_NumOf(iParts) - 1] -= -1);

	SendMessage(m_hStatusBar, SB_SETPARTS, _NumOf(iParts), (LPARAM) iParts);
	SendMessage(m_hStatusBar, WM_SIZE, wParam, lParam);

	SendMessage(m_hStatusBar, SB_GETRECT, m_uEditBox, (LPARAM) &rtRect);
	InflateRect(&rtRect, -2, -2);
	MoveWindow(m_hEditBox, rtRect.left, rtRect.top, _RectWidth(rtRect), _RectHeight(rtRect), TRUE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ø�ʽ
BOOL CChildWnd::OnChangeFormat()
{
	TCHAR tzText[16];

	INT i = GetDlgItemInt(m_hStatusBar, IDC_EditBox, NULL, TRUE);
	CRawImage riFormat = *this;

	switch (m_uEditBox)
	{
	case SBI_ColorSpace:
		GetWindowText(m_hEditBox, tzText, _NumOf(tzText));
		riFormat.m_csColorSpace = CS_ALL;
		for (i = 0; i < _NumOf(c_tzColorSpace); i++)
		{
			if (lstrcmpi(tzText, c_tzColorSpace[i]) == 0)
			{
				riFormat.m_csColorSpace = (COLORSPACE) (i % CS_ALL);
				break;
			}
		}
		if (riFormat.m_csColorSpace == CS_ALL)
		{
			return FALSE;
		}
		else if (riFormat.m_csColorSpace == m_csColorSpace)
		{
			return TRUE;
		}
		break;

	case SBI_Width:
		i = (i + 1) & ~1;		
		_ReturnValIf(riFormat.m_uWidth == (UINT) i, TRUE);
		riFormat.m_uWidth = i;
		break;

	case SBI_Height:
		i = (i + 1) & ~1;
		_ReturnValIf(riFormat.m_iHeight == i, TRUE);
		riFormat.m_iHeight = i;
		break;

	case SBI_FrameRate:
		_ReturnValIf(riFormat.m_uFrameRate == (UINT) i, TRUE);
		riFormat.m_uFrameRate = i;
		break;

	case SBI_FrameStep:
		riFormat.m_iFrameStep = GetDlgItemInt(m_hStatusBar, IDC_EditBox, NULL, TRUE);
		_ReturnValIf(riFormat.m_iFrameStep == m_iFrameStep, TRUE);
		break;

	case SBI_CurFrame:
		if ((i > 0) && (i <= (INT) m_uTotalFrame))
		{
			Stop();
			Seek(i - 1);
			return TRUE;
		}		

	default:
		return FALSE;
	}

	_ExIf(CClientWnd::IsScrabble(), CClientWnd::OnPlayScrabble());

	Open(m_tzFileName, &riFormat);
	EnableDDraw();
	Draw();

	// ��¼��ʷ��ʽ
	CIni::SetStruct(INI_HistroyFormat, m_tzFileName, &riFormat, sizeof(RAWIMAGE));

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ò��Ŵ��ڳߴ�
VOID CChildWnd::SetVideoSize(DWORD dwWidth, DWORD dwHeight)
{
	RECT rtRect;
	RECT rtWindow;

	// ���Ŵ��ڷǿͻ����ߴ�
	GetClientRect(m_hPlayWnd, &rtRect);
	GetWindowRect(m_hPlayWnd, &rtWindow);
	dwWidth += _RectWidth(rtWindow) - _RectWidth(rtRect);
	dwHeight += _RectHeight(rtWindow) - _RectHeight(rtRect);

	// �Ӵ��ڷǿͻ����ߴ�
	GetClientRect(m_hWnd, &rtRect);
	GetWindowRect(m_hWnd, &rtWindow);
	dwWidth += _RectWidth(rtWindow) - _RectWidth(rtRect);
	dwHeight += _RectHeight(rtWindow) - _RectHeight(rtRect);

	if (CMainWnd::IsMenuChecked(IDM_View_Toolbar))
	{
		GetWindowRect(m_hToolbar, &rtRect);
		dwHeight += _RectHeight(rtRect);
	}

	if (CMainWnd::IsMenuChecked(IDM_View_StatusBar))
	{
		GetWindowRect(m_hStatusBar, &rtRect);
		dwHeight += _RectHeight(rtRect);
	}

	// �������ڴ�С
	MapWindowPoints(NULL, CMainWnd::m_hWnd, (PPOINT) &rtWindow, 2);
	MoveWindow(m_hWnd, rtWindow.left, rtWindow.top, dwWidth, dwHeight, TRUE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����ص�����
BOOL CChildWnd::ExportProc()
{
	INT i;
	PBYTE pbDst;
	HANDLE hFile;
	DWORD dwSize;
	TCHAR tzText[MAX_PATH];
	TCHAR tzFileName[MAX_PATH];

	m_bExporting = TRUE;
	DWORD dwStartTime = GetTickCount();

	INT iMin = CExportDlg::m_dwFromFrame;
	INT iMax = CExportDlg::m_dwToFrame;
	INT iStep = m_iFrameStep;
	if (iStep < 0)
	{
		i = iMax;
		iMax = -iMin;
		iMin = -i;
		iStep = -iStep;
	}
	else if (iStep == 0)
	{
		iStep = 1;
	}

	BOOL bResult = TRUE;
	m_bCancel = FALSE;

	CRawImage riDst(*this, CExportDlg::m_csColorSpace);

	if (CExportDlg::m_bOneFile)
	{
		HANDLE hFile = _CreateFileForWrite(CExportDlg::m_tzFileName);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			bResult = FALSE;
		}
		else
		{
			for (i = iMin; i <= iMax; i += iStep)
			{
//#define _QUIET
#ifndef _QUIET
				wsprintf(tzText, LNG_ConvertMsg, abs(i) + 1);
				SetStatusText(tzText);
				_ExIf(m_bCancel, bResult = FALSE; break);
#endif
				pbDst = Convert(abs(i), riDst);
				_ExIf(pbDst == NULL, bResult = FALSE; break);

//#define _NULLFILE
#ifndef _NULLFILE
				WriteFile(hFile, pbDst, riDst.m_uFrameSize, &dwSize, NULL);
#endif
			}

			CloseHandle(hFile);
		}
	}
	else
	{
		PTSTR ptzExt = _StrRChr(CExportDlg::m_tzFileName, '\\');
		ptzExt = _StrRChr(ptzExt, '.');
		_ExIf(ptzExt, *ptzExt++ = 0);

		for (i = iMin; i <= iMax; i += iStep)
		{
			wsprintf(tzText, LNG_ConvertMsg, abs(i) + 1);
			SetStatusText(tzText);
			_ExIf(m_bCancel, bResult = FALSE; break);

			wsprintf(tzFileName, TEXT("%s-%d.%s"), CExportDlg::m_tzFileName, abs(i) + 1, ptzExt);

			if (CExportDlg::m_csColorSpace < CS_ALL)
			{
				hFile = _CreateFileForWrite(tzFileName);
				_ExIf(hFile == INVALID_HANDLE_VALUE, bResult = FALSE; break);

				pbDst = Convert(abs(i), riDst);
				_ExIf(pbDst == NULL, bResult = FALSE; break);

				WriteFile(hFile, pbDst, riDst.m_uFrameSize, &dwSize, NULL);
				CloseHandle(hFile);
			}
			else if (SaveBitmap(abs(i), tzFileName) == FALSE)
			{
				bResult = FALSE;
				break;
			}
		}
	}

	if (bResult)
	{
		dwStartTime = GetTickCount() - dwStartTime;
		_ExIf(dwStartTime == 0, dwStartTime = 1)
		wsprintf(tzText, LNG_ExportMsg, dwStartTime);
		SetStatusText(tzText);
	}
	else
	{
		SetStatusText(LNG_ExportErr);
	}

	m_bExporting = FALSE;

	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����ص�����
DWORD CChildWnd::vExportProc(CChildWnd *pThis)
{
	if (CExportDlg::m_csColorSpace == CS_ALL)
	{
		PTSTR p = _StrRChr(CExportDlg::m_tzFileName, '.');
		if (p == NULL)
		{
			p = _StrEnd(CExportDlg::m_tzFileName);
			*p = '.';
		}
		p++;
		for (UINT i = 0; i < CS_ALL; i++)
		{
			CExportDlg::m_csColorSpace = (COLORSPACE) i;
			lstrcpy(p, c_tzColorSpace[i]);
			pThis->ExportProc();
		}

		return TRUE;
	}
	else
	{
		return pThis->ExportProc();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
