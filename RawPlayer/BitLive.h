


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��Ϣ: ֪ͨ���ڽ��յ� WM_COMMAND ��Ϣ
// ����: LOWORD(wParam) Ϊ IDC_BitLive��HIWORD(wParam) Ϊ��Ϣ����lParam Ϊ��ʾ�ı�
// ����: ���� CBitLive::Cancel() ��ֹ��̲���
#define IDC_BitLive			51426

#define BLL_Check			0	// ��ʼ���
#define BLL_CheckErr		1	// ���ʧ��
#define BLL_UpToDate		2	// û�з����°汾

#define BLL_NewsBox			3	// ������Ϣ��
#define BLL_News			4	// �����ı�

#define BLL_Query			5	// �����°汾��ѯ���Ƿ�����
#define BLL_Update			6	// ��ʼ���ظ���
#define BLL_Progress		7	// ���ؽ���
#define BLL_Confirm			8	// ȷ��Ӧ�ø���
#define BLL_Exit			9	// �����˳� (lParam Ϊ 0)
#define BLL_UpdateErr		10	// ����ʧ��
#define BLL_UpdateEnd		11	// ���½���
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBitLive ��
class CBitLive
{
private:
	static HWND m_hNotify;			// ֪ͨ��Ϣ���ھ��
	static BOOL m_bCancel;			// ȡ����ʶ
	static HANDLE m_hThread;		// �����߳̾��

public:
	// ��ʼ������
	inline static VOID Live(HWND hNotify)
	{
		DWORD dwThread;

		if (m_hThread == NULL)
		{
			SetNotify(hNotify);
			m_bCancel = FALSE;
			m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) Check, NULL, 0, &dwThread);
		}
	}

	// ����֪ͨ��Ϣ����
	inline static VOID SetNotify(HWND hNotify)
	{
		m_hNotify = hNotify;
	}

	// ȡ��������
	inline static VOID Cancel()
	{
		m_bCancel = TRUE;
	}

	// ��ֹ������
	inline static VOID Destroy()
	{
		if (m_hThread)
		{
			TerminateThread(m_hThread, 0);
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}

private:
	// ��ȡ��ʾ�ı�
	static BOOL GetPromptText(PCTSTR ptzFileName, PCTSTR ptzKeyName, PTSTR ptzText);

	// ������
	static BOOL WINAPI Check();

	// ��ȡ�ļ�
	static DWORD GetFile(PCTSTR ptzFileName, PCTSTR ptzUrl, PCTSTR ptzHeader = NULL, BOOL bNotify = TRUE);

	// ����
	static BOOL Update(PCTSTR ptzFileName, BOOL bReplaceMode);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
