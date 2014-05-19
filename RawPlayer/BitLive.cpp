


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#include "Main.h"
#include "BitLive.h"
#include <WinInet.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �궨��
#define _BLPostMsg(t, l)		PostMessage(m_hNotify, WM_COMMAND, MAKELONG(IDC_BitLive, t), (LPARAM) (l))
#define _BLSendMsg(t, l)		SendMessage(m_hNotify, WM_COMMAND, MAKELONG(IDC_BitLive, t), (LPARAM) (l))
#define _BLGetInt(f, k, d)		GetPrivateProfileInt(INI_BitLive, k, d, f)
#define _BLGetString(f, k, t)	GetPrivateProfileString(INI_BitLive, k, NULL, t, 1024, f)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBitLive �ྲ̬����
HWND CBitLive::m_hNotify = NULL;
BOOL CBitLive::m_bCancel = NULL;
HANDLE CBitLive::m_hThread = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ȡ��ʾ�ı�
BOOL CBitLive::GetPromptText(PCTSTR ptzFileName, PCTSTR ptzKeyName, PTSTR ptzText)
{
	PTSTR p;
	TCHAR tzKeyName[MAX_PATH];

	// ��ȡ��������ı������û�����Ի�ȡĬ���ı�
	wsprintf(tzKeyName, TEXT("%s.%u"), ptzKeyName, CIni::GetInt(INI_Language, GetUserDefaultLangID()));
	if (_BLGetString(ptzFileName, tzKeyName, ptzText) || _BLGetString(ptzFileName, ptzKeyName, ptzText))
	{
		// �滻���з�
		for (p = ptzText; *p; p++)
		{
			_ExIf(*p == '`', *p = '\n');
		}

		return TRUE;
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������
BOOL WINAPI CBitLive::Check()
{
	BOOL bResult;
	DWORD dwBuild;
	TCHAR tzUrl[1024];
	TCHAR tzText[1024];
	TCHAR tzUpdateFile[MAX_PATH];
	TCHAR tzBitLiveFile[MAX_PATH];

	_Try
	{
		tzBitLiveFile[0] = 0;
		_LeaveIf(CIni::GetString(INI_BitLive, tzUrl, 1024,
			STR_WebUrl TEXT("BitLive/") STR_AppName TEXT(".blv")) < 2);

		// ��ȡ�ļ�
		_BLSendMsg(BLL_Check, LNG_BitLiveCheck);
		GetTempPath(MAX_PATH, tzBitLiveFile);
		GetTempFileName(tzBitLiveFile, STR_AppName, 0, tzBitLiveFile);
		if ((GetFile(tzBitLiveFile, tzUrl, NULL, FALSE) == 0) ||
			(_BLGetString(tzBitLiveFile, INI_UpdateUrl, tzUrl) == 0))
		{
			// ���ʧ��
			_BLSendMsg(BLL_CheckErr, LNG_BitLiveCheckErr);
			_Leave;
		}

		// ������
		dwBuild = _BLGetInt(tzBitLiveFile, INI_UpdateBuild, 0);
		if (dwBuild <= VER_Build)
		{
			_BLSendMsg(BLL_UpToDate, LNG_BitLiveUpToDate);
			_LeaveIf(m_bCancel);
		}
		else
		{
			// ѯ���Ƿ����
			if (_BLGetInt(tzBitLiveFile, INI_UpdateBox, TRUE))
			{
				GetPromptText(tzBitLiveFile, INI_UpdateText, tzText);
				_BLSendMsg(BLL_Query, tzText);
			}
			if (m_bCancel == FALSE)
			{
				// ��ȡ����
				_BLSendMsg(BLL_Update, LNG_BitLiveUpdate);
				GetModuleFileName(NULL, tzUpdateFile, MAX_PATH);
				wsprintf(_StrEnd(tzUpdateFile) - _LenOf(EXT_Exe), TEXT(".%u") EXT_Exe, dwBuild);
				bResult = (GetFile(tzUpdateFile, tzUrl) == _BLGetInt(tzBitLiveFile, INI_UpdateSize, -1));
				if (bResult)
				{
					// ȷ�ϸ���
					wsprintf(tzText, LNG_BitLiveConfirm, tzUpdateFile);
					_BLSendMsg(BLL_Confirm, tzText);
					if (m_bCancel == FALSE)
					{
						bResult = Update(tzUpdateFile, _BLGetInt(tzBitLiveFile, INI_UpdateReplace, FALSE));
						if (bResult && _BLGetInt(tzBitLiveFile, INI_UpdateExit, FALSE))
						{
							// �˳�����
							_BLPostMsg(BLL_Exit, 0);
						}
					}
				}

				if (bResult == FALSE)
				{
					// ����ʧ��
					_BLSendMsg(BLL_UpdateErr, LNG_BitLiveUpdateErr);
					DeleteFile(tzUpdateFile);
				}

				_BLSendMsg(BLL_UpdateEnd, (bResult ? LNG_Ready : LNG_BitLiveUpdateErr));

				_Leave;
			}
		}

		// ��ʾ�����ı�
		GetPromptText(tzBitLiveFile, INI_NewsText, tzText);
		if (_BLGetInt(tzBitLiveFile, INI_NewsBox, 0))
		{
			_BLSendMsg(BLL_NewsBox, tzText);
		}
		_BLSendMsg(BLL_News, tzText);
	}
	_Finally
	{
		if (tzBitLiveFile[0])
		{
			// ɾ����ʱ�ļ�
			DeleteFile(tzBitLiveFile);
		}

		// �ر��߳̾��
		_SafeCloseHandle(m_hThread);

		return TRUE;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ȡ�ļ�
DWORD CBitLive::GetFile(PCTSTR ptzFileName, PCTSTR ptzUrl, PCTSTR ptzHeader, BOOL bNotify)
{
	DWORD dwSize;
	HANDLE hFile;
	DWORD dwRecv;
	DWORD dwTotal;
	HINTERNET hInet;
	HINTERNET hHttp;
	TCHAR tzLen[32];
	BYTE bBuffer[4096];
	TCHAR tzText[MAX_PATH];

	// �����Ự
	dwRecv = 0;
	hInet = InternetOpen(STR_AppName, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInet)
	{
		// ������
		hHttp = InternetOpenUrl(hInet, ptzUrl, ptzHeader, lstrlen(ptzHeader), INTERNET_FLAG_RELOAD, 0);
		if (hHttp)
		{
			// ��ȡ��С
			dwSize = _LenOf(tzLen);
			if (HttpQueryInfo(hHttp, HTTP_QUERY_CONTENT_LENGTH, tzLen, &dwSize, NULL))
			{
				dwTotal = _StrToInt(tzLen);
			}
			else
			{
				dwTotal = -1;
			}

			// �����ļ�
			hFile = _CreateFileForWrite(ptzFileName);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				// ��ȡ����
				while ((m_bCancel == FALSE) && InternetReadFile(hHttp, bBuffer, sizeof(bBuffer), &dwSize) && dwSize)
				{
					WriteFile(hFile, bBuffer, dwSize, &dwSize, NULL);
					dwRecv += dwSize;
					if (bNotify)
					{
						// ���ɽ�����ʾ�ı�
						if (dwTotal == -1)
						{
							wsprintf(tzText, LNG_BitLiveRecieve, dwRecv);
						}
						else
						{
							wsprintf(tzText, LNG_BitLiveRecvPercent, dwRecv, (dwRecv * 100 / dwTotal));
						}

						// ����֪ͨ
						_BLSendMsg(BLL_Progress, tzText);
					}
				}

				CloseHandle(hFile);
			}

			InternetCloseHandle(hHttp);
		}

		InternetCloseHandle(hInet);
	}

	return dwRecv;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����
BOOL CBitLive::Update(PCTSTR ptzFileName, BOOL bReplaceMode)
{
	DWORD dwSize;
	HANDLE hFile;
	TCHAR tzText[1024];
	TCHAR tzExePath[MAX_PATH];
	TCHAR tzBatFile[MAX_PATH];

	if (bReplaceMode)
	{
		GetModuleFileName(NULL, tzExePath, MAX_PATH);
		lstrcpy(tzBatFile, tzExePath);
		lstrcpy(_StrRChr(tzBatFile, '\\'), TEXT("\\BitLive") EXT_Bat);

		hFile = _CreateFileForWrite(tzBatFile);
		_ReturnValIf(hFile == INVALID_HANDLE_VALUE, FALSE);

		dwSize = wsprintf(tzText,
				TEXT(":REPEAT\r\n")
				TEXT("DEL \"%s\"\r\n")
				TEXT("IF EXIST \"%s\" GOTO REPEAT\r\n")
				TEXT("MOVE \"%s\" \"%s\"\r\n")
				TEXT("START%s\"%s\"\r\n")
				TEXT("DEL %%0\r\n"),
				tzExePath, tzExePath, ptzFileName, tzExePath, (_WinVerMajor >= 5) ? TEXT(" \"\" ") : TEXT(" "), tzExePath);

#ifdef _UNICODE
		CHAR szText[1024];
		dwSize = _StrToAStr(szText, tzText) - 1;
		WriteFile(hFile, szText, dwSize, &dwSize, 0);
#else
		WriteFile(hFile, tzText, dwSize, &dwSize, 0);
#endif

		CloseHandle(hFile);

		ptzFileName = tzBatFile;
	}

	// ֱ��ִ��
	return (_ShellOpen(ptzFileName, NULL, bReplaceMode ? SW_HIDE : SW_SHOWNORMAL) > (HINSTANCE) 32);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
