


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <Windows.h>
#include "RawPlayer.h"

#pragma warning(disable: 4312)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CChildWnd ��
class CChildWnd: public CRawPlayer
{
public:
	HWND m_hWnd;						// �ͻ����ھ��
	HWND m_hSeekBar;					// ��λ�����ھ��
	HWND m_hPlayWnd;					// ���Ŵ��ھ��

	CChildWnd *m_pNext;					// ѭ����������һ�������ָ��
	static CChildWnd *m_pHead;			// ��󴴽��Ķ���

private:
	UINT m_uEditBox;					// �༭����λ��
	HWND m_hEditBox;					// �༭���ھ��
	HWND m_hToolbar;					// ���������ھ��	
	HWND m_hStatusBar;					// ״̬�����ھ��	
	HIMAGELIST m_hImageLists[3];		// ������ͼ���б���	

	BOOL m_bCancel;						// �û�ȡ����������
	BOOL m_bExporting;					// ���ڵ���

	BOOL m_bPrevPlaying;				// �϶���λ��֮ǰ�Ƿ񲥷�
	BOOL m_bStartOfScroll;				// �Ƿ�ʼ�϶���λ��

public:
	// ���캯��
	CChildWnd();

	// ��������
	~CChildWnd();

	// ��������
	HWND Create();

public:
	// �Ӵ��ڻص�����
	static LRESULT CALLBACK ClildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ���Ŵ��ڻص�����
	static LRESULT CALLBACK PlayWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ���������ڻص�����
	static LRESULT CALLBACK ToolbarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// �༭���ڻص�����
	static LRESULT CALLBACK EditBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	// ���ڱ�����
	VOID OnCreate();

	// ��С�ı�
	VOID OnSize(WPARAM wParam = SIZE_RESTORED, LPARAM lParam = 0);

	// ֪ͨ��Ϣ
	static VOID OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ���״̬��
	VOID OnClickStatusBar(UINT uIndex);

	// �����
	VOID OnCommand(WPARAM wParam, LPARAM lParam = 0);

	// ����
	VOID OnFileExport();

	// ����ͨ���˵�
	VOID OnMenuPopup();

	// ͨ��
	VOID OnPlayChannel(UINT uCmd);

	// ����
	VOID OnPlayZoom(UINT uCmd, BOOL bCascade = TRUE);

	// ���ø�ʽ
	BOOL OnChangeFormat();

	// ����������
	VOID CreateToolbar();

	// ���ù�����ͼƬ
	HIMAGELIST SetToolbarBitmap(UINT uMsg, HBITMAP hBitmap);

	// ����״̬����С��λ��
	VOID AdjustStatusBar(WPARAM wParam = SIZE_RESTORED, LPARAM lParam = 0);

	// ���ò��Ŵ��ڳߴ�
	VOID SetVideoSize(DWORD dwWidth, DWORD dwHeight);

private:
	// �����ص�����
	BOOL ExportProc();
	static DWORD vExportProc(CChildWnd *pThis);

public:
	inline BOOL SetStatusText(PCTSTR ptzText, INT iIndex = 0) {return (BOOL) SendMessage(m_hStatusBar, SB_SETTEXT, iIndex ? iIndex : SBT_NOBORDERS, (LPARAM) ptzText);}
	inline BOOL SetStatusText(INT iValue = 0, INT iIndex = 0) {TCHAR tzText[32]; wsprintf(tzText, TEXT("%d"), iValue); return SetStatusText(tzText, iIndex);}

public:
	inline static CChildWnd *GetWnd(HWND hWnd)
	{
		return (CChildWnd *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	inline static VOID SetStatusText(HWND hWnd, PCTSTR ptzText, INT iIndex = 0)
	{
		SendDlgItemMessage(hWnd, IDC_StatusBar, SB_SETTEXT, iIndex ? iIndex : SBT_NOBORDERS, (LPARAM) ptzText);
	}

	inline BOOL operator==(CChildWnd &cwWnd)
	{
		return (m_csColorSpace == cwWnd.m_csColorSpace) &&
			(m_uWidth == cwWnd.m_uWidth) &&
			(m_iHeight == cwWnd.m_iHeight) &&
			(m_iFrameStep == cwWnd.m_iFrameStep) &&
			(m_uFrameRate == cwWnd.m_uFrameRate);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
