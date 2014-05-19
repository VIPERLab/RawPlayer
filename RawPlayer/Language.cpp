


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#include "Main.h"
#include "Language.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����ļ������ͼ���
#define LIN_Language			TEXT("Language")
#define LIN_Description			TEXT("Description")
#define LIN_FontName			TEXT("FontName")
#define LIN_FontSize			TEXT("FontSize")
#define LIN_Text				TEXT("Text")
#define LIN_String				TEXT("String")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLanguage �ྲ̬����
UINT CLanguage::m_uLang = IDM_View_Default;
UINT CLanguage::m_uMax = IDM_View_Default;
HFONT CLanguage::m_hFont = NULL;
TCHAR CLanguage::m_tzText[1024] = {0};
TCHAR CLanguage::m_tzFileName[MAX_PATH] = {0};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʼ������
LANGID CLanguage::Initialize()
{
	HANDLE hFind;
	LANGID liLang;
	LANGID liResult;
	WIN32_FIND_DATA fdFind;
	TCHAR tzPath[MAX_PATH];

#ifdef _MAKELANG

	// ���������ļ�
	hFind = NULL;
	_ZeroMem(fdFind);
	liLang = GetUserDefaultLangID();
	liResult = liLang;
	CIni::SetInt(INI_Language, liResult);
	GetModuleFileName(NULL, tzPath, MAX_PATH);
#ifdef _CHS
	lstrcpy(_StrRChr(tzPath, '\\') + 1, TEXT("Chinese (Simplified)") EXT_Lng);
	WritePrivateProfileString(LIN_Language, LIN_Language, TEXT("2052"), tzPath);
	WritePrivateProfileString(LIN_Language, LIN_Description, TEXT("�������ģ�Yonsm ���죬��ӭʹ�á�"), tzPath);
	WritePrivateProfileString(LIN_Language, LIN_FontName, TEXT("����"), tzPath);
	WritePrivateProfileString(LIN_Language, LIN_FontSize, TEXT("9"), tzPath);
#else
	lstrcpy(_StrRChr(tzPath, '\\') + 1, TEXT("English") EXT_Lng);
	WritePrivateProfileString(LIN_Language, LIN_Language, TEXT("1033"), tzPath);
	WritePrivateProfileString(LIN_Language, LIN_Description, TEXT("English. Powered by Yonsm. No rights reserved."), tzPath);
	WritePrivateProfileString(LIN_Language, LIN_FontName, TEXT("Tahoma"), tzPath);
	WritePrivateProfileString(LIN_Language, LIN_FontSize, TEXT("8"), tzPath);
#endif
	lstrcpy(m_tzFileName, tzPath);

#else
	// ��ȡ���Ա�ʶ
	liResult = 0;
	liLang = CIni::GetInt(INI_Language, GetUserDefaultLangID());
	if (liLang)
	{
		// ���������ļ�
		GetModuleFileName(NULL, tzPath, MAX_PATH);
		lstrcpy(_StrRChr(tzPath, '\\') + 1, TEXT("*") EXT_Lng);
		hFind = FindFirstFile(tzPath, &fdFind);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				// �����ָ��������
				lstrcpy(_StrRChr(tzPath, '\\') + 1, fdFind.cFileName);
				if (liLang == GetPrivateProfileInt(LIN_Language, LIN_Language, 0, tzPath))
				{
					// ���������ļ���
					liResult = liLang;
					lstrcpy(m_tzFileName, tzPath);
					Set(NULL, liResult);
					break;
				}
			}
			while (FindNextFile(hFind, &fdFind));
			FindClose(hFind);
		}
	}
#endif

	return liResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �г�����
UINT CLanguage::List(HMENU hMenu)
{
	HANDLE hFind;
	LANGID liLang;
	LANGID liTemp;
	WIN32_FIND_DATA fdFind;
	TCHAR tzPath[MAX_PATH];

	// ���������ļ�
	liLang = CIni::GetInt(INI_Language);
	GetModuleFileName(NULL, tzPath, MAX_PATH);
	lstrcpy(_StrRChr(tzPath, '\\') + 1, TEXT("*") EXT_Lng);
	hFind = FindFirstFile(tzPath, &fdFind);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// ��ȡ���Ա�ʶ���ж��Ƿ�Ϊ��Ч
			lstrcpy(_StrRChr(tzPath, '\\') + 1, fdFind.cFileName);
			liTemp = GetPrivateProfileInt(LIN_Language, LIN_Language, 0, tzPath);
			if (liTemp)
			{
				// ��һ����ӣ�����ָ��������� 50 ��������
				if (m_uMax == IDM_View_Default)
				{
					AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
				}
				else if (m_uMax >= IDM_View_Default + 50)
				{
					break;
				}

				// ��Ӳ˵���
				m_uMax++;
				fdFind.cFileName[lstrlen(fdFind.cFileName) - _LenOf(EXT_Lng)] = 0;
				AppendMenu(hMenu, MF_BYCOMMAND, m_uMax, fdFind.cFileName);
				if (liTemp == liLang)
				{
					// ��¼ָ������
					m_uLang = m_uMax;
				}
			}
		}
		while (FindNextFile(hFind, &fdFind));
		FindClose(hFind);
	}

	// ѡ��˵���
	CheckMenuRadioItem(hMenu, IDM_View_Default, m_uMax, m_uLang, MF_BYCOMMAND);

	return m_uLang;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
VOID CLanguage::Set(HMENU hMenu, UINT uLang)
{
	HDC hDC;
	LANGID liLang;
	LOGFONT lfFont;
	TCHAR tzName[MAX_PATH];

	// ����ǳ�ʼ������������
	if (hMenu == NULL)
	{
		liLang = uLang;
	}
	else
	{
		if ((uLang <= IDM_View_Default) || (uLang > m_uMax))
		{
			// �л���Ĭ������
			liLang = 0;
			m_tzFileName[0] = 0;
			m_uLang = IDM_View_Default;
		}
		else
		{
			// �л�����������
			m_uLang = uLang;
			GetModuleFileName(NULL, m_tzFileName, MAX_PATH);
			GetMenuString(hMenu, uLang, tzName, MAX_PATH, MF_BYCOMMAND);
			wsprintf(_StrRChr(m_tzFileName, '\\'), TEXT("\\%s%s"), tzName, EXT_Lng);
			liLang = GetPrivateProfileInt(LIN_Language, LIN_Language, 0, m_tzFileName);
		}

		// ѡ��˵���
		CheckMenuRadioItem(hMenu, IDM_View_Default, m_uMax, m_uLang, MF_BYCOMMAND);
	}

	// �������Ա�ʶ
	CIni::SetInt(INI_Language, liLang);

	// ��������
	_SafeDeleteObject(m_hFont);
	_ZeroMem(lfFont);
	if (GetPrivateProfileString(LIN_Language, LIN_FontName, NULL, lfFont.lfFaceName, LF_FACESIZE, m_tzFileName))
	{
		lfFont.lfCharSet = DEFAULT_CHARSET;
		lfFont.lfHeight = GetPrivateProfileInt(LIN_Language, LIN_FontSize, 0, m_tzFileName);
		if (lfFont.lfHeight)
		{
			hDC = CreateIC(TEXT("DISPLAY"), NULL, NULL, NULL);
			lfFont.lfHeight = -MulDiv(lfFont.lfHeight, GetDeviceCaps(hDC, LOGPIXELSY), 72);
			DeleteDC(hDC);
		}
		m_hFont = CreateFontIndirect(&lfFont);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �Ӳ˵��л�ȡ�����ļ���
PTSTR CLanguage::GetDescription(HMENU hMenu, UINT uLang)
{
	TCHAR tzName[MAX_PATH];

	GetModuleFileName(NULL, m_tzText, MAX_PATH);
	GetMenuString(hMenu, uLang, tzName, MAX_PATH, MF_BYCOMMAND);
	wsprintf(_StrRChr(m_tzText, '\\'), TEXT("\\%s%s"), tzName, EXT_Lng);
	if (GetPrivateProfileString(LIN_Language, LIN_Description, NULL, m_tzText, _NumOf(m_tzText), m_tzText) == 0)
	{
		TranslateString(IDM_View_FirstLang);
	}

	return m_tzText;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ȡ�����ַ���
PTSTR CLanguage::TranslateText(PCTSTR ptzName, PCTSTR ptzDefault)
{
#ifdef _MAKELANG
	WritePrivateProfileString(LIN_Text, ptzName, ptzDefault, m_tzFileName);
	return (PTSTR) ptzDefault;
#else
	if ((m_tzFileName[0] == 0) ||
		(GetPrivateProfileString(LIN_Text, ptzName, NULL, m_tzText, _NumOf(m_tzText), m_tzFileName) == 0))
	{
		return (PTSTR) ptzDefault;
	}
	return m_tzText;
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ȡ�����ַ��������滻�����ַ�
PTSTR CLanguage::TranslateTextEx(PCTSTR ptzName, PCTSTR ptzDefault)
{
#ifdef _MAKELANG
	// �ж��Ƿ����������ַ�
	BOOL bDoubleNull = FALSE;
	CopyMemory(m_tzText, ptzDefault, sizeof(m_tzText));
	for (UINT i = 0; i < _LenOf(m_tzText); i++)
	{
		if ((m_tzText[i] == 0) && (m_tzText[i + 1] == 0))
		{
			bDoubleNull = TRUE;
			break;
		}
	}

	for (PTSTR p = m_tzText; ; p++)
	{
		if (*p == '\n')
		{
			*p = '`';
		}
		else if (*p == 0)
		{
			if (bDoubleNull)
			{
				*p = '~';
				if (*(p + 1) == 0)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	WritePrivateProfileString(LIN_Text, ptzName, m_tzText, m_tzFileName);
	return (PTSTR) ptzDefault;
#else
	if ((m_tzFileName[0] == 0) ||
		(GetPrivateProfileString(LIN_Text, ptzName, NULL, m_tzText, _NumOf(m_tzText), m_tzFileName) == 0))
	{
		return (PTSTR) ptzDefault;
	}
	for (PTSTR p = m_tzText; *p; p++)
	{
		if (*p == '`')
		{
			*p = '\n';
		}
		else if (*p == '~')
		{
			*p = 0;
		}
	}
	return m_tzText;
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ȡ��Դ�ַ���
PTSTR CLanguage::TranslateString(UINT uResID)
{
	TCHAR tzName[32];

	wsprintf(tzName, TEXT("%u"), uResID);

#ifdef _MAKELANG
	LoadString(g_hInst, uResID, m_tzText, _NumOf(m_tzText));
	if (m_tzText[0])
	{
		WritePrivateProfileString(LIN_String, tzName, m_tzText, m_tzFileName);
	}
#else
	if ((m_tzFileName[0] == 0) ||
		(GetPrivateProfileString(LIN_String, tzName, NULL, m_tzText, _NumOf(m_tzText), m_tzFileName) == 0))
	{
		if (LoadString(g_hInst, uResID, m_tzText, _NumOf(m_tzText)) == 0)
		{
			return NULL;
		}
	}
#endif

	return m_tzText;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���²˵�
VOID CLanguage::TranslateMenu(HMENU hMenu, PCTSTR ptzResName)
{
	TCHAR tzSection[32];

	_Assert(hMenu);
	if (m_tzFileName[0])
	{
		// ��ȡ����
		if (IS_INTRESOURCE(ptzResName))
		{
			wsprintf(tzSection, TEXT("%u"), ptzResName);
		}
		else
		{
			_StrToStr(tzSection, ptzResName);
		}

		// �������ļ��и��²˵�
		UpdateMenuFromLanguage(hMenu, tzSection);
	}
	else
	{
		// ����Դ�и��²˵�
		UpdateMenuFromResource(hMenu, ptzResName);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���¶Ի���
VOID CLanguage::TranslateDialog(HWND hWnd, PCTSTR ptzResName)
{
	TCHAR tzSection[32];

	_Assert(hWnd);
	if (m_tzFileName[0])
	{
		// ��ȡ����
		if (IS_INTRESOURCE(ptzResName))
		{
			wsprintf(tzSection, TEXT("%u"), ptzResName);
		}
		else
		{
			_StrToStr(tzSection, ptzResName);
		}

		// �޸ĶԻ����ַ���
		UpdateDialogFromLanguage(hWnd, tzSection);

#ifndef _TRANSRECUR
		// ö�ٲ��޸��Ӵ����ַ���
		EnumChildWindows(hWnd, (WNDENUMPROC) UpdateDialogFromLanguage, (LPARAM) tzSection);
#endif
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ò˵��ַ���
BOOL CLanguage::SetMenuString(HMENU hMenu, UINT uItemID, PCTSTR ptzString, BOOL bByPosition)
{
	MENUITEMINFO miiItem;

#if (_WINVER >= 0x0500)
	miiItem.cbSize = sizeof(MENUITEMINFO);
#else
	miiItem.cbSize = (_WinVerMajor < 5) ? CDSIZEOF_STRUCT(MENUITEMINFO, cch) : sizeof(MENUITEMINFO);
#endif

#if (_WINVER >= 0x0410)
	miiItem.fMask = MIIM_STRING;
#else
	miiItem.fMask = MIIM_TYPE;
	miiItem.cch = 0;
	miiItem.dwTypeData = NULL;
	GetMenuItemInfo(hMenu, uItemID, bByPosition, &miiItem);
#endif

	miiItem.dwTypeData = (PTSTR) ptzString;

	return SetMenuItemInfo(hMenu, uItemID, bByPosition, &miiItem);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������ļ��и��²˵�
VOID CLanguage::UpdateMenuFromLanguage(HMENU hMenu, PCTSTR ptzSection, PCTSTR ptzPath)
{
	INT i;
	INT iNum;
	TCHAR tzName[256];
	TCHAR tzText[1024];
	MENUITEMINFO miiItem;

	// ��ʼ����������ȡ�˵�������

#if (_WINVER >= 0x0500)
	miiItem.cbSize = sizeof(MENUITEMINFO);
#else
	miiItem.cbSize = (_WinVerMajor < 5) ? CDSIZEOF_STRUCT(MENUITEMINFO, cch) : sizeof(MENUITEMINFO);
#endif

	miiItem.fMask = MIIM_SUBMENU | MIIM_ID;
	iNum = GetMenuItemCount(hMenu);
	for (i = 0; i < iNum; i++)
	{
		// ��ȡ�����˵�����ͱ�ʶ
		GetMenuItemInfo(hMenu, i, TRUE, &miiItem);
		if (miiItem.wID)
		{
			// ��ȡ�˵���ʶ�ı�
			if (miiItem.hSubMenu)
			{
				_Assert(ptzPath);
				_Assert(lstrlen(ptzPath) < _NumOf(tzName) - 16);
				wsprintf(tzName, TEXT("%s|%u"), ptzPath, i);
			}
			else
			{
				wsprintf(tzName, TEXT("%u"), miiItem.wID);
			}

#ifdef _MAKELANG
			// ���������ļ����ݹ��޸Ĳ˵��ַ���
			if ((miiItem.wID <= IDM_View_Default) || (miiItem.wID >= IDM_View_Default + 50) &&
				(miiItem.wID <= IDM_File_Recent) || (miiItem.wID >= IDM_File_Recent + 50))
			{
				GetMenuString(hMenu, i, tzText, _NumOf(tzText), MF_BYPOSITION);
				WritePrivateProfileString(ptzSection, tzName, tzText, m_tzFileName);
			}
#else
			// ���ò˵��ַ���
			if (GetPrivateProfileString(ptzSection, tzName, NULL, tzText, _NumOf(tzText), m_tzFileName))
			{
				SetMenuString(hMenu, i, tzText, TRUE);
			}
#endif

			// �ݹ��޸Ĳ˵��ַ���
			if (miiItem.hSubMenu)
			{
				UpdateMenuFromLanguage(miiItem.hSubMenu, ptzSection, tzName);
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����Դ�и��²˵�
VOID CLanguage::UpdateMenuFromResource(HMENU hMenu, PCTSTR ptzResName)
{
	PBYTE p;
	PBYTE pbEnd;
	BOOL bMenuEx;
	WORD wOptions;
	DWORD dwMenuID;
	HRSRC hResource;
	HGLOBAL hGlobal;
	TCHAR tzText[1024];

	// �˵���ζ�ջ�����ڸ��µ����˵��ַ���
	INT i = 0;					// ���
	INT iPos[32] = {0};			// λ��
	BOOL bHilite[32] = {FALSE};	// ����
	HMENU hMenus[32] = {hMenu};	// ���

	// ������Դ
	hResource = FindResource(g_hInst, ptzResName, RT_MENU);
	_Assert(hResource);
	hGlobal = LoadResource(g_hInst, hResource);
	_Assert(hGlobal);

	// ��ȡ��Դ����ʼ��ַ����ֹ��ַ�Ͱ汾
	p = (PBYTE) LockResource(hGlobal);
	pbEnd = p + SizeofResource(g_hInst, hResource);
	bMenuEx = ((PMENUITEMTEMPLATEHEADER) p)->versionNumber;

	// ת���˵�������
	p += ((PMENUITEMTEMPLATEHEADER) p)->offset + sizeof(MENUITEMTEMPLATEHEADER);
	_ExIf(bMenuEx, p += sizeof(DWORD));

	// ���²˵�
	while (p < pbEnd)
	{
		// ��ȡ�˵����ʶ������
		if (bMenuEx)
		{
			p += 3 * sizeof(DWORD);
			dwMenuID = *((PDWORD) p);
			p += sizeof(DWORD) + sizeof(WORD);
		}
		else
		{
			wOptions = *((PWORD) p);
			p += sizeof(WORD);
			if ((wOptions & MF_POPUP) == FALSE)
			{
				dwMenuID = *((PWORD) p);
				p += sizeof(WORD);
			}
		}

		// ��ȡ�˵����ı���ת����һ���˵���
		for (_WStrToStr(tzText, (PWSTR) p); *((PWSTR) p); p += sizeof(WCHAR));
		p += sizeof(WCHAR);

		if (bMenuEx)
		{
			// ���������ʶ���ò˵��ı�
			SetMenuString(hMenu, dwMenuID, tzText);
		}
		else
		{
			// ����ǵ����˵���
			_Assert(i < _NumOf(iPos));
			iPos[i]++;
			if (wOptions & MF_POPUP)
			{
				// ����λ�����ò˵��ı�
				SetMenuString(hMenus[i], iPos[i] - 1, tzText, TRUE);

				// ��ȡ��һ���˵��������������ѹ���ջ
				bHilite[i] = wOptions & MF_HILITE;
				hMenus[i + 1] = GetSubMenu(hMenus[i], iPos[i] - 1);
				i++;
				iPos[i] = 0;
				_Assert(hMenus[i]);
			}
			else
			{
				// ���������ʶ���ò˵��ı�
				SetMenuString(hMenu, dwMenuID, tzText);

				if (wOptions & MF_HILITE)
				{
					// ������ջ������Ѿ�������˳�
					for (i--; (i >= 0) && bHilite[i]; i--);
					_ExIf(i < 0, break);
				}
			}
		}
	}

	FreeResource(hGlobal);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������ļ��и��¶Ի���
BOOL CALLBACK CLanguage::UpdateDialogFromLanguage(HWND hWnd, PCTSTR ptzSection)
{
	TCHAR tzText[1024];
	TCHAR tzName[MAX_PATH];

	// ��ȡ���ڱ�ʶ�ַ���
	wsprintf(tzName, TEXT("%u"), GetDlgCtrlID(hWnd));

	// ���ô��ڱ���
#ifdef _MAKELANG
	GetWindowText(hWnd, tzText, _NumOf(tzText));
	if (tzText[0] && ((tzText[0] < '0') || (tzText[0] > '9')))
	{
		WritePrivateProfileString(ptzSection, tzName, tzText, m_tzFileName);
	}
#else
	if (GetPrivateProfileString(ptzSection, tzName, NULL, tzText, _NumOf(tzText), m_tzFileName))
	{
		SetWindowText(hWnd, tzText);
	}
#endif

#ifdef _TRANSRECUR
	// ö�ٲ��޸��Ӵ����ַ���
	EnumChildWindows(hWnd, (WNDENUMPROC) UpdateDialogFromLanguage, (LPARAM) ptzSection);
#endif

	if (m_hFont)
	{
		SendMessage(hWnd, WM_SETFONT, (WPARAM) m_hFont, FALSE);
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
