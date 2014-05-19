


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#include "Main.h"
#include "AboutDlg.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �궨��
#define IDC_CheckUpdate				1226										// �����������ʶ
#define VAL_AboutDlg_Width			400											// �Ի�����
#define VAL_AboutDlg_Height			275											// �Ի���߶�
#define IDT_AboutDlg_Close			1534										// �ر�ʱ�ӱ�ʶ
#define COLOR_AboutDlg_Link			0x00FF0000									// ��������ɫ
#define COLOR_AboutDlg_HotLink		0x000000FF									// ����ȵ㳬������ɫ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAboutDlg �ྲ̬����
CAboutDlg::DIALOGINFO CAboutDlg::m_diDialog =
{
	{DS_CENTER | WS_POPUP | WS_BORDER | WS_VISIBLE, 0, 0, 0, 0,
		(VAL_AboutDlg_Width * 2 / 3), (VAL_AboutDlg_Height * 2 / 3)},			// �Ի���ģ��
};

CAboutDlg::LINEINFO CAboutDlg::m_liLines[] =
{
	{{0, 1, 88, 74}, 0x00FF9966},												// ������
	{{60, 116, VAL_AboutDlg_Width, 132}, 0x00FF9966},							// ������
	{{60, 175, VAL_AboutDlg_Width, 176}, 0x00000000},							// һ������
};

CAboutDlg::RECTINFO CAboutDlg::m_riRects[] =
{
	{{0, 75, VAL_AboutDlg_Width, 111}, 0x00FF9966},								// ��ɫ����
	{{0, 111, VAL_AboutDlg_Width, 115}, 0x00CC3333},							// ����ɫ����
	{{60, 47, 116, 103}, 0x00CC3333},											// ͼ���������ɫ����
	{{64, 51, 112, 99}, 0x00FFFFFF},											// ͼ������ɫ����
	{{68, 55, 86, 73}, RGB(0xEE, 0xEE, 0xEE + VER_Major % 16)},					// ͼ�걳������ɫ����
	{{90, 55, 108, 73}, RGB(0xEE, 0xEE, 0xEE + VER_Minor % 16)},				// ͼ�걳������ɫ����
	{{68, 79, 86, 95}, RGB(0xEE, 0xEE, 0xEE + VER_Release % 16)},				// ͼ�걳������ɫ����
	{{90, 79, 108, 95}, RGB(0xEE, 0xEE, 0xEE + VER_Build % 16)},				// ͼ�걳������ɫ����
};

CAboutDlg::TEXTINFO CAboutDlg::m_tiTexts[] =
{
	{60, 162, TEXT(""), 0x00000000},											// ״̬��Ϣ
	{60, 140, TEXT(""), 0x00BBBBBB},											// �汾��Ϣ
	{230, 140, TEXT(""), 0x00BBBBBB},											// ������Ϣ
	{60, 240, TEXT(""), 0x00000000},											// ������Ϣ
	{60, 255, TEXT(""), 0x00000000},											// ��Ȩ��Ϣ
	{125, 95, TEXT(""), 0x00FFFFFF},

#ifdef _BETA
	{270, 40, TEXT("BETA"), 0x00AAAAAA},										// ���԰汾
#endif
};

CAboutDlg::FONTTEXTINFO CAboutDlg::m_ftiTexts[] =
{

	{125, 32, STR_Corporation, 0x00FFAAAA, {14, 0, 0, 0, 400, 1,
	0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, TEXT("Arial")}},						// ��˾����

	{125, 40, STR_AppName, 0x00000000, {36, 0, 0, 0, 800, 1,
		0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, TEXT("Arial")}},					// ��������
};

CAboutDlg::LINKINFO CAboutDlg::m_liLinks[] =
{
	{{60, 185, 0, 185 + 13}, TEXT(""), TEXT(""), (PCTSTR) IDC_CheckUpdate},		// ������
	{{60, 200, 0, 200 + 13}, TEXT(""), TEXT(""), STR_WebUrl},					// ������վ
	{{60, 215, 0, 215 + 13}, TEXT(""), TEXT(""), STR_EmailUrl},					// д�Ÿ�����
};

HWND CAboutDlg::m_hWnd = NULL;
BOOL CAboutDlg::m_bSplash = FALSE;
INT CAboutDlg::m_iLinkIndex = -1;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʾ���ڶԻ���
VOID CAboutDlg::Show(HWND hParent)
{
	if (m_hWnd == NULL)
	{
		DialogBoxIndirect(g_hInst, (LPDLGTEMPLATE) &m_diDialog, hParent, (DLGPROC) AboutDlgProc);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������ʽ��ʾ���ڶԻ���
VOID CAboutDlg::Splash(UINT uTimer, HWND hParent)
{
	if (uTimer && (m_hWnd == NULL))
	{
		// ������ģʽ�Ի���
		CreateDialogIndirect(g_hInst, (LPDLGTEMPLATE) &m_diDialog, hParent, (DLGPROC) AboutDlgProc);

		// ���ָ�����Զ��ر�
		if (uTimer != -1)
		{
			SetTimer(m_hWnd, IDT_AboutDlg_Close, uTimer * 1000, NULL);
		}

		// ���õ�ǰ������������ʽ��ʾ�ı��
		m_bSplash = TRUE;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �رմ��ڹ��ڶԻ���
VOID CAboutDlg::Close(UINT uTimer)
{
	// ����������ر�
	if (uTimer)
	{
		SetTimer(m_hWnd, IDT_AboutDlg_Close, uTimer, NULL);
	}
	else
	{
		// ���� BitLive ֪ͨ����
		CBitLive::SetNotify(CMainWnd::m_hWnd);

		// ɾ����ʱ��
		KillTimer(m_hWnd, IDT_AboutDlg_Close);

		// �����ǰ������������ʽ��ʾ
		if (m_bSplash)
		{
			DestroyWindow(m_hWnd);
			m_bSplash = FALSE;
		}
		else
		{
			EndDialog(m_hWnd, 0);
		}

		m_hWnd = NULL;
		m_tiTexts[0].tzText[0] = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����״̬��Ϣ
VOID CAboutDlg::SetStatusText(PCTSTR ptzText)
{
	HDC hMem;
	RECT rtRect;

	// ״̬��Ϣ����
	rtRect.left = m_tiTexts[0].x;
	rtRect.top = m_tiTexts[0].y;
	rtRect.right = VAL_AboutDlg_Width;
	rtRect.bottom = rtRect.top + _RectHeight(m_liLinks[0].rtRect);

	// ��ȡ״̬��Ϣ
	if (ptzText)
	{
		_StrToStr(m_tiTexts[0].tzText, ptzText);
	}
	else
	{
		m_tiTexts[0].tzText[0] = 0;
	}

	// ����״̬��Ϣ
	hMem = GetDC(m_hWnd);
	SetTextColor(hMem, m_tiTexts[0].crColor);
	FillRect(hMem, &rtRect, (HBRUSH) GetStockObject(WHITE_BRUSH));
	SelectObject(hMem, (HFONT) GetStockObject(DEFAULT_GUI_FONT));
	TextOut(hMem, m_tiTexts[0].x, m_tiTexts[0].y, m_tiTexts[0].tzText, lstrlen(m_tiTexts[0].tzText));
	ReleaseDC(m_hWnd, hMem);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ڶԻ���ص�����
INT_PTR CALLBACK CAboutDlg::AboutDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT i;
	POINT ptPoint;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		// ��ʼ������
		m_hWnd = hWnd;
		m_iLinkIndex = NULL;

		// �������ָ��
		SetClassLongPtr(m_hWnd, GCLP_HCURSOR, 0);
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		// ���öԻ�����
		if (GetParent(m_hWnd) == NULL)
		{
			SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, (GetWindowLongPtr(m_hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW));
		}
		SetWindowPos(m_hWnd, GetParent(m_hWnd) ? 0 : HWND_TOPMOST, 0, 0, VAL_AboutDlg_Width, VAL_AboutDlg_Height, SWP_NOMOVE);

		// ��ȡ��������ַ���
		wsprintf(m_tiTexts[1].tzText, LNG_Version, STR_VersionStamp);
		wsprintf(m_tiTexts[2].tzText, LNG_Build, STR_BuilStamp);
		_StrToStr(m_tiTexts[3].tzText, LNG_Description);
		_StrToStr(m_tiTexts[4].tzText, LNG_Copyright);
		_StrToStr(m_liLinks[0].tzTitle, LNG_CheckUpdate);

		_StrToStr(m_liLinks[1].tzTitle, LNG_VisitWeb);
		_StrToStr(m_liLinks[2].tzTitle, LNG_WriteEmail);
		_StrToStr(m_liLinks[0].tzTip, LNG_CheckUpdateTip);
		_StrToStr(m_liLinks[1].tzTip, LNG_VisitWebTip);
		_StrToStr(m_liLinks[2].tzTip, LNG_WriteEmailTip);

		// ��ȡ CPU ����
		CRawImage::GetCpuFeature(m_tiTexts[5].tzText);

		break;

	case WM_ERASEBKGND:
		OnPaint((HDC) wParam);
		return TRUE;

	case WM_ACTIVATEAPP:
		// �������
		GetCursorPos(&ptPoint);
		MapWindowPoints(NULL, m_hWnd, &ptPoint, 1);
		SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELONG(ptPoint.x, ptPoint.y));
		break;

	case WM_MOUSEMOVE:
		// ����������λ�õĳ����Ӹı�
		ptPoint.x = LOWORD(lParam);
		ptPoint.y = HIWORD(lParam);
		i = GetLinkIndex(ptPoint);
		if (i != m_iLinkIndex)
		{
			m_iLinkIndex = i;
			PaintLinks();
		}
		break;

	case WM_LBUTTONDOWN:
		// �����ͼ����ڣ��϶��Ի���
		ptPoint.x = LOWORD(lParam);
		ptPoint.y = HIWORD(lParam);
		if (PtInRect(&m_riRects[2].rtRect, ptPoint))
		{
			ReleaseCapture();
			PostMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
		break;

	case WM_LBUTTONUP:
		// �������ڳ�������
		if (m_iLinkIndex != -1)
		{
			// ��� HIWORD(ptzUrl) Ϊ 0����ʾ��������ʾ������
			if (HIWORD(m_liLinks[m_iLinkIndex].ptzUrl))
			{
				ShellExecute(NULL, NULL, m_liLinks[m_iLinkIndex].ptzUrl, NULL, NULL, SW_NORMAL);
			}
			else
			{
				PostMessage(m_hWnd, WM_COMMAND, (WPARAM) m_liLinks[m_iLinkIndex].ptzUrl, 0);
			}
		}
		else
		{
			// �رնԻ���
			Close();
		}
		break;

	case WM_COMMAND:
		// ������Ϣ
		OnCommand(wParam, lParam);
		break;

	case WM_TIMER:
	case WM_RBUTTONUP:
		// �رնԻ���
		Close();
		break;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ƶԻ���
VOID CAboutDlg::OnPaint(HDC hDC)
{
	INT i;
	INT j;
	HDC hMem;
	HPEN hPen;
	HFONT hFont;
	HBRUSH hBrush;
	POINT ptPoint;
	HBITMAP hBitmap;
	static RECT s_rtRect = {0, 0, VAL_AboutDlg_Width, VAL_AboutDlg_Height};

	// ��ʼ����
	hMem = CreateCompatibleDC(hDC);
	hBitmap = (HBITMAP) SelectObject(hMem, CreateCompatibleBitmap(hDC, VAL_AboutDlg_Width, VAL_AboutDlg_Height));
	FillRect(hMem, &s_rtRect, (HBRUSH) GetStockObject(WHITE_BRUSH));

	// ����ˮƽ����
	for (i = 0; i < _NumOf(m_liLines); i++)
	{
		hPen = (HPEN) SelectObject(hMem, CreatePen(PS_SOLID, 0, m_liLines[i].crColor));
		for (j = m_liLines[i].rtRect.top; j < m_liLines[i].rtRect.bottom; j += 2)
		{
			MoveToEx(hMem, m_liLines[i].rtRect.left, j, NULL);
			LineTo(hMem, m_liLines[i].rtRect.right, j);
		}
		DeleteObject(SelectObject(hMem, hPen));
	}

	// ���ƾ�������
	for (i = 0; i < _NumOf(m_riRects); i++)
	{
		hBrush = CreateSolidBrush(m_riRects[i].crColor);
		FillRect(hMem, &m_riRects[i].rtRect, hBrush);
		DeleteObject(hBrush);
	}

	// ����ͼ��
	DrawIcon(hMem,
		m_riRects[2].rtRect.left + (_RectWidth(m_riRects[2].rtRect) - 32) / 2,
		m_riRects[2].rtRect.top + (_RectHeight(m_riRects[2].rtRect) - 32) / 2,
		LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_Main)));

	// ���Ƴ�����
	GetCursorPos(&ptPoint);
	MapWindowPoints(NULL, m_hWnd, &ptPoint, 1);
	m_iLinkIndex = GetLinkIndex(ptPoint);
	PaintLinks(hMem);

	// ʹ�öԻ���Ĭ�����塢͸�����������ı�
	SetBkMode(hMem, TRANSPARENT);
	SelectObject(hMem, (HFONT) GetStockObject(DEFAULT_GUI_FONT));
	for (i = 0; i < _NumOf(m_tiTexts); i++)
	{
		SetTextColor(hMem, m_tiTexts[i].crColor);
		TextOut(hMem, m_tiTexts[i].x, m_tiTexts[i].y, m_tiTexts[i].tzText, lstrlen(m_tiTexts[i].tzText));
	}

	// ����ָ��������ı�
	for (i = 0; i < _NumOf(m_ftiTexts); i++)
	{
		hFont = (HFONT) SelectObject(hMem, CreateFontIndirect(&m_ftiTexts[i].lfFont));
		SetTextColor(hMem, m_ftiTexts[i].crColor);
		TextOut(hMem, m_ftiTexts[i].x, m_ftiTexts[i].y, m_ftiTexts[i].ptzText, lstrlen(m_ftiTexts[i].ptzText));
		DeleteObject(SelectObject(hMem, hFont));
	}

	// ��������
	BitBlt(hDC, 0, 0, VAL_AboutDlg_Width, VAL_AboutDlg_Height, hMem, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(hMem, hBitmap));
	DeleteDC(hMem);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����
VOID CAboutDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDOK:
	case IDCANCEL:
		// �رնԻ���
		Close();
		break;

	case IDC_CheckUpdate:
		// ������
		CBitLive::Live(m_hWnd);
		break;

	case IDC_BitLive:
		// ��Ӧ������Ϣ
		switch (HIWORD(wParam))
		{
		case BLL_Check:
		case BLL_CheckErr:
		case BLL_News:
		case BLL_Update:
		case BLL_Progress:
		case BLL_UpdateEnd:
			SetStatusText((PTSTR) lParam);
			break;

		case BLL_UpToDate:
			SetStatusText((PTSTR) lParam);
			CBitLive::Cancel();
			break;

		case BLL_NewsBox:
			MessageBox(m_hWnd, (PTSTR) lParam, INI_BitLive, MB_ICONINFORMATION);
			break;

		case BLL_UpdateErr:
			MessageBox(m_hWnd, (PTSTR) lParam, INI_BitLive, MB_ICONSTOP);
			break;

		case BLL_Query:
		case BLL_Confirm:
			if (MessageBox(m_hWnd, (PTSTR) lParam, INI_BitLive, MB_ICONQUESTION | MB_YESNO) == IDNO)
			{
				CBitLive::Cancel();
			}
			break;

		case BLL_Exit:
			Close();
			PostQuitMessage(0);
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ȡָ����ĳ�����
INT CAboutDlg::GetLinkIndex(POINT ptPoint)
{
	INT i;

	for (i = 0; i < _NumOf(m_liLinks); i++)
	{
		if (PtInRect(&m_liLinks[i].rtRect, ptPoint))
		{
			return i;
		}
	}

	return PtInRect(&m_riRects[2].rtRect, ptPoint) ? -2 : -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���Ƴ�����
VOID CAboutDlg::PaintLinks(HDC hMem)
{
	INT i;
	BOOL bGetDC;
	HFONT hFont;
	LOGFONT lfFont;
	PTSTR ptzCursor;

	if (hMem == NULL)
	{
		hMem = GetDC(m_hWnd);
		bGetDC = TRUE;
	}
	else
	{
		bGetDC = FALSE;
	}

	GetObject((HFONT) GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lfFont);
	for (i = 0; i < _NumOf(m_liLinks); i++)
	{
		lfFont.lfUnderline = (i == m_iLinkIndex);
		hFont = (HFONT) SelectObject(hMem, CreateFontIndirect(&lfFont));
		SetTextColor(hMem, lfFont.lfUnderline ? COLOR_AboutDlg_HotLink : COLOR_AboutDlg_Link);

		// ������ο򣬻����ı�
		m_liLinks[i].rtRect.bottom = m_liLinks[i].rtRect.top +
			DrawText(hMem, m_liLinks[i].tzTitle, -1, &m_liLinks[i].rtRect,
			DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);
		DrawText(hMem, m_liLinks[i].tzTitle, -1, &m_liLinks[i].rtRect, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);

		DeleteObject(SelectObject(hMem, hFont));
	}

	if ((m_iLinkIndex >= 0))
	{
		SetStatusText(m_liLinks[m_iLinkIndex].tzTip);

#if (_WINVER >= 0x0401)
		ptzCursor = IDC_HAND;
#else
		ptzCursor = _WinVerAboveEqual(4, 1) ? IDC_HAND : IDC_CROSS;
#endif
	}
	else
	{
		SetStatusText();
		ptzCursor = (m_iLinkIndex == -2) ? IDC_SIZEALL : IDC_ARROW;
	}

	_ExIf(bGetDC, ReleaseDC(m_hWnd, hMem));

	SetCursor(LoadCursor(NULL, ptzCursor));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
