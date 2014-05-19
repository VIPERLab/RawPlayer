


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#include "Main.h"
#include "Shortcut.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �궨��
#define STR_QuickLaunch		TEXT("\\Microsoft\\Internet Explorer\\Quick Launch")	// ��������·��
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �жϿ�ݷ�ʽ�Ƿ����
BOOL CShortcut::Exist(INT iFolder)
{
	PTSTR ptzFileName;
	TCHAR tzPath[MAX_PATH];
	TCHAR tzBuffer[MAX_PATH];

	if (CUtility::GetFolder(iFolder, tzPath) == FALSE)
	{
		return FALSE;
	}

	_ExIf(iFolder == CSIDL_APPDATA, lstrcat(tzPath, STR_QuickLaunch));
	return SearchPath(tzPath, STR_AppName, EXT_Lnk, MAX_PATH, tzBuffer, &ptzFileName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ݷ�ʽ
BOOL CShortcut::Create(INT iFolder)
{
	HRESULT hResult;
	TCHAR tzPath[MAX_PATH];
	WCHAR wzPath[MAX_PATH];
	IShellLink *pShellLink;
	IPersistFile *pPersistFile;

	if (CUtility::GetFolder(iFolder, tzPath) == FALSE)
	{
		return FALSE;
	}
	_ExIf(iFolder == CSIDL_APPDATA, lstrcat(tzPath, STR_QuickLaunch));
	lstrcat(tzPath, TEXT("\\") STR_AppName EXT_Lnk);
	_StrToWStr(wzPath, tzPath);

	hResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *) &pShellLink);
	if (hResult == S_OK)
	{
		hResult = pShellLink->QueryInterface(IID_IPersistFile, (PVOID *) &pPersistFile);
		if (hResult == S_OK)
		{
			GetModuleFileName(NULL, tzPath, MAX_PATH);
			hResult = pShellLink->SetPath(tzPath);
			if (hResult == S_OK)
			{
				pShellLink->SetDescription(LNG_Description);
				hResult = pPersistFile->Save(wzPath, FALSE);
			}
			pPersistFile->Release();
		}
		pShellLink->Release();
	}

	return (hResult == S_OK);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ɾ����ݷ�ʽ
BOOL CShortcut::Remove(INT iFolder)
{
	TCHAR tzPath[MAX_PATH];

	if (CUtility::GetFolder(iFolder, tzPath) == FALSE)
	{
		return FALSE;
	}

	_ExIf(iFolder == CSIDL_APPDATA, lstrcat(tzPath, STR_QuickLaunch));
	lstrcat(tzPath, TEXT("\\") STR_AppName EXT_Lnk);
	return DeleteFile(tzPath);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
