


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#include "Main.h"
#include "RecentFile.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRecentFile �ྲ̬����
UINT CRecentFile::m_uPos = 0;
UINT CRecentFile::m_uNum = 0;
UINT CRecentFile::m_uMax = 4;
HMENU CRecentFile::m_hMenu = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ȡ�½��ļ��б�
UINT CRecentFile::Load(HMENU hMenu, UINT uPos)
{
	TCHAR tzName[16];
	TCHAR tzValue[MAX_PATH];
	TCHAR tzMenu[MAX_PATH + 16];

	m_uPos = uPos;
	m_hMenu = hMenu;
	m_uMax = CIni::GetInt(INI_MaxRecentFile, m_uMax);
	_ExIf(m_uMax > 9, m_uMax = 9);

	// �����½��ļ��б�
	for (m_uNum = 0; m_uNum < m_uMax; m_uNum++)
	{
		wsprintf(tzName, INI_RecentFile TEXT("%d"), m_uNum);
		if (CIni::GetString(INI_RecentFile, tzName, tzValue) == 0)
		{
			break;
		}
		wsprintf(tzMenu, TEXT("&%d. %s"), m_uNum + 1, tzValue);
		InsertMenu(m_hMenu, m_uPos + m_uNum, MF_BYPOSITION, IDM_File_Recent + m_uNum, tzMenu);
	}

	// ����ָ��
	if (m_uNum)
	{
		InsertMenu(m_hMenu, m_uPos + m_uNum, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
	}

	return m_uNum;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����½��ļ��б�
UINT CRecentFile::Save()
{
	UINT i;
	TCHAR tzName[16];
	TCHAR tzMenu[MAX_PATH + 16];

	// �������ʹ�ù����ļ��б�
	for (i = 0; i < m_uNum; i++)
	{
		wsprintf(tzName, TEXT("%s%d"), INI_RecentFile, i);
		GetMenuString(m_hMenu, m_uPos + i, tzMenu, _NumOf(tzMenu), MF_BYPOSITION);
		CIni::SetString(INI_RecentFile, tzName, tzMenu + 4);
	}

	return m_uNum;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ӵ��½��ļ��б�
UINT CRecentFile::Add(PCTSTR ptzFileName)
{
	UINT i;
	TCHAR tzValue[MAX_PATH + 16];

	_Assert(ptzFileName);
	_ReturnValIf(m_uMax == 0, 0);

	// ����ָ��
	if (m_uNum == 0)
	{
		InsertMenu(m_hMenu, m_uPos, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
	}

	// ����ԭ�еĲ˵���
	for (i = 0; i < m_uNum; )
	{
		GetMenuString(m_hMenu, m_uPos + i, tzValue, _NumOf(tzValue), MF_BYPOSITION);
		if (lstrcmpi(tzValue + 4, ptzFileName) == 0)
		{
			m_uNum--;
			DeleteMenu(m_hMenu, m_uPos + i, MF_BYPOSITION);
		}
		else
		{
			tzValue[1] = '2' + i;
			ModifyMenu(m_hMenu, m_uPos + i, MF_BYPOSITION, IDM_File_Recent + i + 1, tzValue);
			i++;
		}
	}

	// �������
	wsprintf(tzValue, TEXT("&1. %s"), ptzFileName);
	InsertMenu(m_hMenu, m_uPos, MF_BYPOSITION, IDM_File_Recent, tzValue);

	if (m_uNum == m_uMax)
	{
		// ɾ�����һ��
		DeleteMenu(m_hMenu, m_uPos + m_uNum, MF_BYPOSITION);
	}
	else
	{
		m_uNum++;
	}

	return m_uNum;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����½��ļ��б�
VOID CRecentFile::Clear()
{
	UINT i;
	TCHAR tzName[MAX_PATH];

	for (i = 0; i <= 9; i++)
	{
		DeleteMenu(m_hMenu, IDM_File_Recent + i, MF_BYCOMMAND);
		wsprintf(tzName, TEXT("%s%d"), INI_RecentFile, i);
		CIni::SetString(INI_RecentFile, tzName, NULL);
	}
	if (m_uNum)
	{
		DeleteMenu(m_hMenu, m_uPos, MF_BYPOSITION);
		m_uNum = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
