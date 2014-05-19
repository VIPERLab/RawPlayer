


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#include "Main.h"
#include "MainWnd.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMainWnd �ྲ̬����
HWND CMainWnd::m_hWnd = NULL;
HMENU CMainWnd::m_hMenu = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����������
HWND CMainWnd::Create()
{
	WNDCLASSEX wceClass;

	// ע����������
	wceClass.cbSize = sizeof(WNDCLASSEX);
	wceClass.style = CS_VREDRAW | CS_HREDRAW;
	wceClass.lpfnWndProc = MainWndProc;
	wceClass.cbClsExtra = 0;
	wceClass.cbWndExtra = 0;
	wceClass.hInstance = g_hInst;
	wceClass.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_Main));
	wceClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wceClass.hbrBackground = (HBRUSH) COLOR_BTNSHADOW;
	wceClass.lpszMenuName = MAKEINTRESOURCE(IDR_Menu);
	wceClass.lpszClassName = STR_AppName;
	wceClass.hIconSm = NULL;
	RegisterClassEx(&wceClass);

	// ����������
	return CreateWindowEx(WS_EX_ACCEPTFILES | WS_EX_APPWINDOW, STR_AppName, STR_AppName, WS_OVERLAPPEDWINDOW,
		CIni::GetInt(INI_WindowLeft, CW_USEDEFAULT),
		CIni::GetInt(INI_WindowTop, CW_USEDEFAULT),
		CIni::GetInt(INI_WindowWidth, CW_USEDEFAULT),
		CIni::GetInt(INI_WindowHeight, CW_USEDEFAULT),
		NULL, NULL, g_hInst, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʾ�����ڣ���������Ϣѭ�����ȴ��������
VOID CMainWnd::Show(PCTSTR ptzCmdLine, INT iCmdShow)
{
	MSG msgMsg;
	HACCEL hAccel;

	// ��ʾ������
	if (iCmdShow <= SW_NORMAL)
	{
		iCmdShow = _Bound(CIni::GetInt(INI_WindowState), SW_NORMAL, SW_MAX);
	}
	ShowWindow(m_hWnd, iCmdShow);

	// ����˵����ټ�
	hAccel = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDR_Accel));

	// ����������
	if (ptzCmdLine && ptzCmdLine[0])
	{
		OnFileOpen(ptzCmdLine);
	}
	else if (CIni::GetInt(INI_ShowOpen))
	{
		PostMessage(m_hWnd, WM_COMMAND, IDM_File_Open, 0);
	}

	// ��Ϣѭ��
	while (GetMessage(&msgMsg, NULL, 0, 0))
	{
		if ((TranslateMDISysAccel(CClientWnd::m_hWnd, &msgMsg) == 0) &&
			(TranslateAccelerator(m_hWnd, hAccel, &msgMsg) == 0))
		{
			TranslateMessage(&msgMsg);
			DispatchMessage(&msgMsg);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ص�����
LRESULT CALLBACK CMainWnd::MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CChildWnd *pWnd;
	TCHAR tzTemp[MAX_PATH];

	switch (uMsg)
	{
	case WM_CREATE:
		// ���ڱ�����
		m_hWnd = hWnd;
		m_hMenu = GetMenu(m_hWnd);
		OnCreate();
		break;

	case WM_SIZE:
		// �ߴ�ı�
		OnSize(wParam, lParam);
		return 0;

	case WM_WINDOWPOSCHANGED:
		// λ�øı�
		for (pWnd = CChildWnd::m_pHead; pWnd; pWnd = pWnd->m_pNext)
		{
			if (pWnd->m_hWnd)
			{
				pWnd->SetDrawRect();
			}
		}
		break;

	case WM_INITMENUPOPUP:
		// �����˵���ʼ��
		OnMenuPopup(wParam, lParam);
		break;

	case WM_MENUSELECT:
		// �˵��ѡ��
		OnMenuSelect(LOWORD(wParam));
		break;

	case WM_EXITMENULOOP:
		// �˳��˵�ѭ��
		CClientWnd::SetStatusText(LNG_Ready);
		break;

	case WM_COMMAND:
		// �����
		OnCommand(wParam, lParam);
		break;

	case WM_COPYDATA:
		// �����������ʵ��֪ͨ���ļ�
		if (((PCOPYDATASTRUCT) lParam)->dwData == IDM_File_Open)
		{
			static TCHAR s_tzFileName[MAX_PATH];

			lstrcpyn(s_tzFileName, (PTSTR) ((PCOPYDATASTRUCT) lParam)->lpData, MAX_PATH);
			PostMessage(m_hWnd, WM_COMMAND, IDM_File_Open, (LPARAM) s_tzFileName);
		}
		break;

	case WM_HOTKEY:
		// �ȼ���Ϣ
		if ((m_hWnd != GetForegroundWindow()) && (wParam == IDC_TrayIcon))
		{
			OnTrayIcon(FALSE);
		}
		break;

	case WM_DROPFILES:
		// �϶��ļ�
		DragQueryFile((HDROP) wParam, 0, tzTemp, MAX_PATH);
		DragFinish((HDROP) wParam);
		OnFileOpen(tzTemp);
		break;

	case WM_HELP:
		// ����
		OnCommand(IDM_Help_Content);
		break;

	case WM_DESTROY:
		// ���ڱ�����
		OnDestroy();
		break;
	}

	return DefFrameProc(hWnd, CClientWnd::m_hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ڱ�����
VOID CMainWnd::OnCreate()
{
#ifdef _MAKELANG
	OnViewLanguage(IDM_View_Default);
#else
	// �г�����
	if (CLanguage::List(GetSubMenu(GetSubMenu(m_hMenu, 2), 12)) != IDM_View_Default)
	{
		// ����˵�
		CLanguage::TranslateMenu(m_hMenu, MAKEINTRESOURCE(IDR_Menu));
	}
#endif

	// ����
	CClientWnd::Create();

	// ���ò˵���״̬
	OnViewMenu();

	// ע���ȼ�
	RegisterHotKey(m_hWnd, IDC_TrayIcon, MOD_WIN, 'V');

	// ��ȡ�½��ļ��б�
	CRecentFile::Load(GetSubMenu(m_hMenu, 0), 4);

	// ����ͬ��״̬
	//CRawVideo::SetSync(CIni::GetInt(INI_Synchronize));
	//CheckCommand(IDM_Play_Synchronize, CRawPlayer::m_bSync);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��С�ı�
VOID CMainWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	if (wParam == SIZE_MINIMIZED)
	{
		if (IsMenuChecked(IDM_View_MinToTray))
		{
			// ���ϵͳ����ͼ�꣬����������
			OnTrayIcon();
		}
	}
	else
	{
		MoveWindow(CClientWnd::m_hWnd, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		_ExIf(CClientWnd::IsScrabble(), CClientWnd::Scrabble());
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����˵���ʼ��
VOID CMainWnd::OnMenuPopup(WPARAM wParam, LPARAM lParam)
{
	HWND hWnd;
	DWORD dwSize;
	TCHAR tzTemp[512];
	TCHAR tzPath[512];

	switch (GetMenuItemID((HMENU) wParam, 0))
	{
	case IDM_Help_Desktop:
		// �жϿ�ݷ�ʽ�Ƿ����
		CheckCommand(IDM_Help_Desktop, CShortcut::Exist(CSIDL_DESKTOP));
		CheckCommand(IDM_Help_StartMenu, CShortcut::Exist(CSIDL_STARTMENU));
		CheckCommand(IDM_Help_ProgramMenu, CShortcut::Exist(CSIDL_PROGRAMS));
		CheckCommand(IDM_Help_QuickLaunch, CShortcut::Exist(CSIDL_APPDATA));
		CheckCommand(IDM_Help_VisualStudio, CVSTool::Exist());
		break;

	case IDM_Play_Play:
		hWnd = CClientWnd::GetActive();
		_ExIf(hWnd, SendMessage(hWnd, WM_INITMENUPOPUP, wParam, lParam));
		break;

	case IDM_View_Toolbar:
		GetModuleFileName(NULL, tzTemp, MAX_PATH);
		wsprintf(tzPath, TEXT("\"%s\" \"%%1\""), tzTemp);
		dwSize = _NumOf(tzTemp);
		SHGetValue(HKEY_CLASSES_ROOT, STR_AppName TEXT("\\shell\\open\\command"), NULL, NULL, tzTemp, &dwSize);
		CheckCommand(IDM_View_AssociateFile, lstrcmpi(tzTemp, tzPath) == 0);			
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �˵��ѡ��
VOID CMainWnd::OnMenuSelect(UINT uCmd)
{
	// ��������Բ˵���
	if ((uCmd > IDM_View_Default) && (uCmd < IDM_View_Default + 50))
	{
		CLanguage::GetDescription(m_hMenu, uCmd);
	}
	else
	{
		// ������½��ļ��˵���
		if ((uCmd > IDM_File_Recent) && (uCmd < IDM_File_Recent + 50))
		{
			uCmd = IDM_File_Recent;
		}
		CLanguage::TranslateString(uCmd);
	}

	// ��״̬����ʾ��Ӧ�Ĳ˵�����ʾ
	CClientWnd::SetStatusText(CLanguage::m_tzText[0] ? CLanguage::m_tzText : LNG_Ready);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����
VOID CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	PTSTR p;
	DWORD dwSize;
	CChildWnd *pWnd;	
	TCHAR tzTemp[512];
	TCHAR tzPath[512];

	if ((LOWORD(wParam) >= IDM_View_Default) && (LOWORD(wParam) < IDM_View_Default + 50))
	{
		// �ı�����
		if (IsMenuChecked(LOWORD(wParam)) == FALSE)
		{
			OnViewLanguage(LOWORD(wParam));
		}
		return;
	}
	else if ((LOWORD(wParam) >= IDM_File_Recent) && (LOWORD(wParam) < IDM_File_Recent + 10))
	{
		// ���½��ļ�
		GetMenuString(m_hMenu, LOWORD(wParam), tzTemp, _NumOf(tzTemp), MF_BYCOMMAND);
		OnFileOpen(tzTemp + 4);
		return;
	}

	switch (LOWORD(wParam))
	{
	case IDM_File_Open:
		OnFileOpen((PTSTR) lParam);
		break;

	case IDM_File_Exit:
		PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;

	case IDM_View_Toolbar:
	case IDM_View_StatusBar:
	case IDM_View_AlwaysOnTop:
	case IDM_View_MinToTray:
	case IDM_View_ShowSplash:
	case IDM_View_ShowOpen:
	case IDM_View_PlayOnOpen:
		// �鿴ѡ��
		OnViewMenu(LOWORD(wParam), FALSE);
		break;

	case IDM_View_AssociateFile:
		GetModuleFileName(NULL, tzPath, MAX_PATH);
		wsprintf(tzTemp, TEXT("\"%s\" \"%%1\""), tzPath);
		dwSize = _NumOf(tzPath);
		SHGetValue(HKEY_CLASSES_ROOT, STR_AppName TEXT("\\shell\\open\\command"), NULL, NULL, tzPath, &dwSize);
		if (lstrcmpi(tzTemp, tzPath) == 0)
		{
			SHDeleteKey(HKEY_CLASSES_ROOT, STR_AppName);

			for (p = STR_ExtList; *p; p += lstrlen(p) + 1)
			{
				SHDeleteKey(HKEY_CLASSES_ROOT, p);
			}
		}
		else
		{
			SHSetValue(HKEY_CLASSES_ROOT, 
				STR_AppName TEXT("\\shell\\open\\command"), NULL, REG_SZ, tzTemp, _StrSize(tzTemp));

			GetModuleFileName(NULL, tzPath, MAX_PATH);
			wsprintf(tzTemp, TEXT("\"%s\",1"), tzPath);
			SHSetValue(HKEY_CLASSES_ROOT, STR_AppName TEXT("\\DefaultIcon"), NULL, REG_SZ, tzTemp, _StrSize(tzTemp));

			for (p = STR_ExtList; *p; p += lstrlen(p) + 1)
			{
				SHSetValue(HKEY_CLASSES_ROOT, p, NULL, REG_SZ, STR_AppName, sizeof(STR_AppName));
			}
		}
		break;

	case IDM_Window_Cascade:
		SendMessage(CClientWnd::m_hWnd, WM_MDICASCADE, 0, 0);
		return;

	case IDM_Window_TitleHorz:
		SendMessage(CClientWnd::m_hWnd, WM_MDITILE, MDITILE_HORIZONTAL, 0);
		break;

	case IDM_Window_TitleVert:
		SendMessage(CClientWnd::m_hWnd, WM_MDITILE, MDITILE_VERTICAL, 0);
		break;

	case IDM_Window_ArrangeIcons:
		SendMessage(CClientWnd::m_hWnd, WM_MDIICONARRANGE, 0, 0);
		break;

	case IDM_Window_CloseAll:
		CClientWnd::CloseAll();
		break;

	case IDM_Help_Content:
		// �鿴��������
		GetModuleFileName(NULL, tzTemp, MAX_PATH);
		lstrcpy(_StrEnd(tzTemp) - _LenOf(EXT_Chm), EXT_Chm);
		if (ShellExecute(NULL, NULL, tzTemp, NULL, NULL, SW_SHOW) <= (HINSTANCE) 32)
		{
			ErrorBox(ERR_HelpErr);
		}
		break;

	case IDM_Help_Desktop:
	case IDM_Help_StartMenu:
	case IDM_Help_ProgramMenu:
	case IDM_Help_QuickLaunch:
	case IDM_Help_VisualStudio:
		// ������ɾ����ݷ�ʽ
		OnHelpShortcut(LOWORD(wParam));
		break;

	case IDM_Help_About:
		// ��ʾ���ڶԻ���
		CAboutDlg::Show(m_hWnd);
		break;

	case IDC_TrayIcon:
		// ϵͳ������Ϣ
		if ((lParam == WM_LBUTTONUP) || (lParam == WM_RBUTTONUP))
		{
			OnTrayIcon(FALSE);
		}
		break;

	default:
		pWnd = CClientWnd::GetChildWnd();
		_ExIf(pWnd, pWnd->OnCommand(wParam, lParam));
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
VOID CMainWnd::OnDestroy()
{
	WINDOWPLACEMENT wpWindow;

	// ���洰��λ����Ϣ
	wpWindow.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_hWnd, &wpWindow);
	CIni::SetInt(INI_WindowLeft, wpWindow.rcNormalPosition.left);
	CIni::SetInt(INI_WindowTop, wpWindow.rcNormalPosition.top);
	CIni::SetInt(INI_WindowWidth, _RectWidth(wpWindow.rcNormalPosition));
	CIni::SetInt(INI_WindowHeight, _RectHeight(wpWindow.rcNormalPosition));
	CIni::SetInt(INI_WindowState, wpWindow.showCmd);

	// ����ͬ��״̬
	//CIni::SetInt(INI_Synchronize, CRawPlayer::m_bSync);

	// �����½��ļ��б�
	CRecentFile::Save();

	// �����������
	CLanguage::Destroy();

	// �˳�����
	PostQuitMessage(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ļ�
VOID CMainWnd::OnFileOpen(PCTSTR ptzFileName)
{
	TCHAR tzFileName[MAX_PATH];
	TCHAR tzOpenError[MAX_PATH * 2];
	static BOOL s_bShowOpen = FALSE;

	if (s_bShowOpen == FALSE)
	{
		s_bShowOpen = TRUE;
		if (CRawPlayer::m_bSync)
		{
			CheckCommand(IDM_Play_Synchronize, CRawPlayer::ToggleSync());
		}

		if (ptzFileName)
		{
			CUtility::TrimQuotMark(tzFileName, ptzFileName);
		}
		else
		{
			tzFileName[0] = 0;
		}

		if (COpenDlg::Open(tzFileName) != ERROR_CANCELLED)
		{
			// �������
			if (COpenDlg::m_pWnd && CClientWnd::Load(COpenDlg::m_pWnd))
			{
				// ��ӵ��½��ļ��б�
				CRecentFile::Add(tzFileName);
			}
			else
			{
				// ���ļ�����
				wsprintf(tzOpenError, LNG_OpenErr, tzFileName);
				ErrorBox(tzOpenError);
			}
		}

		s_bShowOpen = FALSE;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �л��鿴�˵�״̬
VOID CMainWnd::OnViewMenu(UINT uCmd, BOOL bInitialize)
{
	UINT i;
	BOOL bChecked;
	const PTSTR STR_KeyNames[] =
	{
		INI_Toolbar,
		INI_StatusBar,
		INI_AlwaysOnTop,
		INI_MinToTray,
		INI_ShowSplash,
		INI_ShowOpen,
		INI_PlayOnOpen
	};

	// ��ȡ INI ����������
	i = uCmd - IDM_View_Toolbar;

	// �������Ч����
	if (i < _NumOf(STR_KeyNames))
	{
		// �л��˵���
		bChecked = !IsMenuChecked(uCmd);
		CheckCommand(uCmd, bChecked);

		// ������ǳ�ʼ��ָ�����л�����¼�� INI ��
		if (bInitialize == FALSE)
		{
			CIni::SetInt(STR_KeyNames[i], bChecked);
		}

		switch (uCmd)
		{
		case IDM_View_Toolbar:
		case IDM_View_StatusBar:
			CClientWnd::ToggleBar();
			break;

		case IDM_View_AlwaysOnTop:
			// ������ǰ
			SetWindowPos(m_hWnd, bChecked ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			return;

		case IDM_View_MinToTray:
			// ��С����ϵͳ����
			if (bChecked && (bInitialize == FALSE))
			{
				CloseWindow(m_hWnd);
			}
			return;

		default:
			return;
		}
	}
	// ����ǳ�ʼ���˵���
	else if (bInitialize)
	{
		// ��ʼ���˵���
		for (i = 0; i < _NumOf(STR_KeyNames); i++)
		{
			// Ĭ���Ƿ�ѡ��
			bChecked = IsMenuChecked(IDM_View_Toolbar + i);

			// ���Ĭ��ѡ���� INI �в�һ��
			if (bChecked != (BOOL) CIni::GetInt(STR_KeyNames[i], bChecked))
			{
				// �л��˵���
				OnViewMenu(IDM_View_Toolbar + i, TRUE);
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ı�����
VOID CMainWnd::OnViewLanguage(UINT uLang)
{
#ifdef _MAKELANG
	// ���泣���ַ���
	LNG_Description; LNG_Copyright; LNG_Ready; LNG_Version; LNG_Build;
	LNG_CheckUpdate; LNG_CancelUpdate; LNG_VisitWeb; LNG_WriteEmail;
	LNG_CheckUpdateTip; LNG_VisitWebTip; LNG_WriteEmailTip;
	LNG_BitLiveCheck; LNG_BitLiveUpdate; LNG_BitLiveUpToDate; LNG_BitLiveCheckErr; LNG_BitLiveUpdateErr;
	LNG_BitLiveRecieve; LNG_BitLiveRecvPercent; LNG_BitLiveConfirm;

	LNG_ExportFilter; LNG_BrowseFolder;
	LNG_OpenErr; ERR_HelpErr; LNG_CreateShortcutErr; LNG_RemoveShortcutErr;
	LNG_ExportErr; LNG_AddToVSErr; LNG_RemoveFromVSErr; LNG_InputErr; LNG_UnexpectedErr; LNG_DirectDrawErr;
	LNG_CreateShortcutMsg; LNG_RemoveShortcutMsg; LNG_ConvertMsg; LNG_ExportMsg; LNG_AddToVSMsg; LNG_RemoveFromVSMsg;
	LNG_ColorSpaceMsg; LNG_FrameWidthMsg; LNG_FrameHeightMsg; LNG_FrameRateMsg; LNG_FrameStepMsg; LNG_FrameIndexeMsg;
	
	// ������Դ�ַ���
	for (INT i = IDM_File_Open; i <= IDM_Help_About; i++)
	{
		CLanguage::TranslateString(i);
	}

	// ����˵�
	CLanguage::TranslateMenu(m_hMenu, MAKEINTRESOURCE(IDR_Menu));

	// ����Ի����ַ���
	PostMessage(m_hWnd, WM_COMMAND, IDM_File_Open, 0);

#else

	// ��������
	CLanguage::Set(m_hMenu, uLang);

	// ����˵�
	OnCommand(IDM_Window_Cascade);
	CLanguage::TranslateMenu(m_hMenu, MAKEINTRESOURCE(IDR_Menu));
	DrawMenuBar(m_hWnd);

	// ����״̬��
	OnMenuSelect(uLang);

#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ɾ����ݷ�ʽ
VOID CMainWnd::OnHelpShortcut(UINT uCmd)
{
	INT iFolder;

	switch (uCmd)
	{
	case IDM_Help_Desktop:
		iFolder = CSIDL_DESKTOP;
		break;

	case IDM_Help_StartMenu:
		iFolder = CSIDL_STARTMENU;
		break;

	case IDM_Help_ProgramMenu:
		iFolder = CSIDL_PROGRAMS;
		break;

	case IDM_Help_QuickLaunch:
		iFolder = CSIDL_APPDATA;
		break;

	default:
		// ������ Visual Studio �еĿ�ݷ�ʽ
		if (IsMenuChecked(IDM_Help_VisualStudio))
		{
			if (CVSTool::Remove())
			{
				MsgBox(LNG_RemoveFromVSMsg);
			}
			else
			{
				ErrorBox(LNG_RemoveFromVSErr);
			}
		}
		else
		{
			if (CVSTool::Add())
			{
				MsgBox(LNG_AddToVSMsg);
			}
			else
			{
				ErrorBox(LNG_AddToVSErr);
			}
		}
		return;
	}

	if (IsMenuChecked(uCmd))
	{
		if (CShortcut::Remove(iFolder))
		{
			MsgBox(LNG_RemoveShortcutMsg);
		}
		else
		{
			ErrorBox(LNG_RemoveShortcutErr);
		}
	}
	else
	{
		if (CShortcut::Create(iFolder))
		{
			MsgBox(LNG_CreateShortcutMsg);
		}
		else
		{
			ErrorBox(LNG_CreateShortcutErr);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��С����ϵͳ����
VOID CMainWnd::OnTrayIcon(BOOL bEnable)
{
	NOTIFYICONDATA nidTray = {0};
	nidTray.cbSize = sizeof(NOTIFYICONDATA);
	nidTray.hWnd = m_hWnd;
	nidTray.uID = IDC_TrayIcon;

	if (bEnable)
	{
		// ���ϵͳ����ͼ�꣬����������
		nidTray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nidTray.uCallbackMessage = WM_COMMAND;
		nidTray.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_Main));
		GetWindowText(m_hWnd, nidTray.szTip, _NumOf(nidTray.szTip));
		Shell_NotifyIcon(NIM_ADD, &nidTray);
		ShowWindow(m_hWnd, SW_HIDE);
	}
	else
	{
		// ɾ��ϵͳ����ͼ�꣬�ָ�������
		Shell_NotifyIcon(NIM_DELETE, &nidTray);
		ShowWindow(m_hWnd, SW_RESTORE);
		SetForegroundWindow(m_hWnd);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
