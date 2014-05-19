


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMainWnd ��
class CMainWnd
{
public:
	static HWND m_hWnd;						// �����ھ��
	static HMENU m_hMenu;					// ���˵����

public:
	// ����������
	static HWND Create();

	// ��ʾ�����ڣ���������Ϣѭ�����ȴ��������
	static VOID Show(PCTSTR ptzCmdLine = NULL, INT iCmdShow = SW_NORMAL);

private:
	// �ص�����
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ���ڱ�����
	static VOID OnCreate();

	// ��С�ı�
	static VOID OnSize(WPARAM wParam = SIZE_RESTORED, LPARAM lParam = 0);

	// �����˵���ʼ��
	static VOID OnMenuPopup(WPARAM wParam, LPARAM lParam);

	// �˵��ѡ��
	static VOID OnMenuSelect(UINT uCmd);

	// �����
	static VOID OnCommand(WPARAM wParam, LPARAM lParam = 0);

	// ��������
	static VOID OnDestroy();

	// ���ļ�
	static VOID OnFileOpen(PCTSTR ptzFileName = NULL);

	// �л��鿴�˵�״̬
	static VOID OnViewMenu(UINT uCmd = -1, BOOL bInitialize = TRUE);

	// �ı�����
	static VOID OnViewLanguage(UINT uLang);

	// ������ɾ����ݷ�ʽ
	static VOID OnHelpShortcut(UINT uCmd);

	// ��С����ϵͳ����
	static VOID OnTrayIcon(BOOL bEnable = TRUE);

public:
	// �жϲ˵����Ƿ�ѡ��
	inline static BOOL IsMenuChecked(UINT uCmd)
	{
		return ((MF_CHECKED & GetMenuState(m_hMenu, uCmd, MF_BYCOMMAND)) == MF_CHECKED);
	}

	// ʹ�˵��͹�����������Ч����Ч
	inline static BOOL EnableCommand(UINT uCmd, BOOL bEnable = TRUE)
	{
		return EnableMenuItem(m_hMenu, uCmd, bEnable ? MF_ENABLED : MF_GRAYED);
	}

	// ʹ�˵��͹���������ѡ���ѡ��
	inline static BOOL CheckCommand(UINT uCmd, BOOL bChecked = TRUE)
	{
		return CheckMenuItem(m_hMenu, uCmd, bChecked ? MF_CHECKED : MF_UNCHECKED);
	}

	// ʹ�˵��͹��������ѡѡ��
	inline static BOOL CheckRadioCommand(UINT uFirst, UINT uLast, UINT uCmd)
	{
		return CheckMenuRadioItem(m_hMenu, uFirst, uLast, uCmd, MF_BYCOMMAND);
	}

	// ��Ϣ��
	inline static INT MsgBox(PCTSTR ptzText = NULL, PCTSTR ptzCaption = STR_AppName, UINT uType = MB_ICONINFORMATION)
	{
		return MessageBox(m_hWnd, ptzText, ptzCaption, uType);
	}

	// ������ʾ��
	inline static INT ErrorBox(PCTSTR ptzText = NULL, PCTSTR ptzCaption = STR_AppName)
	{
		return MsgBox(ptzText, ptzCaption, MB_ICONSTOP);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
