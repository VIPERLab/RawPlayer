


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include "Define.h"
#include "Macro.h"
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �궨��
#ifdef _UNICODE
#define CF_TTEXT					CF_UNICODETEXT
#define STR_GetFileAttributesEx		"GetFileAttributesExW"
#else
#define CF_TTEXT					CF_TEXT
#define STR_GetFileAttributesEx		"GetFileAttributesExA"
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUtility ��
class CUtility
{
public:
	// ��ȡ�����ļ���·��
	static BOOL GetFolder(INT iFolder, PTSTR ptzFolder);

	// ��ȡͼƬ�ļ���
	static PTSTR GetPicturesFolder(PTSTR ptzFolder, BOOL bIniFirst = TRUE);

	// �����ȡ�ļ���
	static BOOL BrowseFolder(PTSTR ptzFolder, HWND hParent = NULL);

	// ȥ��ĩβ�ķ�б��
	inline static PTSTR TrimBackslash(PTSTR ptzString)
	{
		PTSTR p = _StrEnd(ptzString) - 1;
		_ExIf(*p == '\\', *p = 0);
		return ptzString;
	}

	// ȡ�����ڵ��ַ���
	inline static PTSTR TrimQuotMark(PTSTR ptzDst, PCTSTR ptzSrc)
	{
		if (ptzSrc)
		{
			PTSTR p, q;
			for (p = (PTSTR) ptzSrc; *p == '"'; p++);
			for (q = (PTSTR) _StrEnd(ptzSrc) - 1; (*q == '"') && (q > p); q--);
			lstrcpyn(ptzDst, p, (INT) (q - p + 2));
		}
		else
		{
			ptzDst[0] = 0;
		}

		return ptzDst;
	}

	// �ж��Ƿ���Ŀ¼
	inline static BOOL IsDirectory(PTSTR ptzString)
	{
		WIN32_FILE_ATTRIBUTE_DATA fadAttrib;

#if (_WINVER > 0x0400)
		return GetFileAttributesEx(ptzString, GetFileExInfoStandard, &fadAttrib) &&
			(fadAttrib.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
#else
		typedef BOOL (WINAPI* GFAEPROC)(PCTSTR, GET_FILEEX_INFO_LEVELS, PVOID);
		GFAEPROC fpGfae = (GFAEPROC) GetProcAddress(GetModuleHandle(TEXT("KERNEL32")), STR_GetFileAttributesEx);
		if (fpGfae)
		{
			return fpGfae(ptzString, GetFileExInfoStandard, &fadAttrib) &&
				(fadAttrib.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
		}
		else
		{
			DWORD dwAttrib = GetFileAttributes(ptzString);
			return (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
		}
#endif
	}

	// ��ȡ������С
	inline static DWORD GetSectorSize(PCTSTR ptzPath)
	{
		PTSTR p;
		DWORD dwFree;
		DWORD dwTotal;
		DWORD dwSector;
		DWORD dwSectorSize;
		TCHAR tzRootPath[MAX_PATH];

		// ��ȡ������С
		lstrcpy(tzRootPath, ptzPath);
		p = tzRootPath;
		if (tzRootPath[0] == '\\')
		{
			p = _StrChr(p + 2, '\\') + 1;
		}
		p = _StrChr(p, '\\');
		_ExIf(p, *(p + 1) = 0);
		GetDiskFreeSpace(tzRootPath, &dwSector, &dwSectorSize, &dwFree, &dwTotal);
		return dwSectorSize;
	}

private:
	// ����ļ��лص�����
	static INT CALLBACK BrowseFolderProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

public:
	// ���õ�����������
	static BOOL SetClipboard(UINT uFormat, PVOID pvData, UINT uSize);

	// ��ȡ����������
	static PVOID GetClipboard(UINT uFormat);

	// ���õ��������ı�
	inline static BOOL SetClipboardText(PCTSTR ptzText, UINT uSize = -1)
	{
		return SetClipboard(CF_TTEXT, (PVOID) ptzText, uSize == -1 ? _StrSize(ptzText) : uSize);
	}

	// ��ȡ�������ı�
	inline static PTSTR GetClipboardText()
	{
		return (PTSTR) GetClipboard(CF_TTEXT);
	}

	// �����������Ƿ�Ϊ�ı���ʽ
	inline static BOOL IsClipboardText()
	{
		return (IsClipboardFormatAvailable(CF_TEXT) ||
			IsClipboardFormatAvailable(CF_OEMTEXT) ||
			IsClipboardFormatAvailable(CF_UNICODETEXT));
	}

public:
	// ����������λͼ
	static HBITMAP CreateToolbarBitmap(HBITMAP hBitmap, BOOL bDisabled = FALSE);

	// ����������
	static HWND CreateToolbar(DWORD dwStyle, HWND hParent, PCTSTR ptzBitmapName, HIMAGELIST *phImageList,
		UINT uButtons, LPCTBBUTTON ptbbButtons);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
