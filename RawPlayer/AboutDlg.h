


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAboutDlg ��
class CAboutDlg
{
private:
	// �Ի���ģ��
	typedef struct tagDIALOGINFO
	{
		DLGTEMPLATE dtDialog;							// �Ի���
		WORD wNoMenu;									// û�жԻ���˵�
		WORD wNoClass;									// û�жԻ�������
		WORD wNoCaption;								// û�жԻ������
		WORD wEnd[5];									// �Ի���ģ�����
	}
	DIALOGINFO;

	// ˮƽ����
	typedef struct tagLINEINFO
	{
		RECT rtRect;									// λ��
		COLORREF crColor;								// ��ɫ
	}
	LINEINFO;

	// ����
	typedef struct tagRECTINFO
	{
		RECT rtRect;									// λ��
		COLORREF crColor;								// ��ɫ
	}
	RECTINFO;

	// �ı�
	typedef struct tagTEXTINFO
	{
		INT x;											// X ����
		INT y;											// Y ����
		TCHAR tzText[64];								// �ı�
		COLORREF crColor;								// ��ɫ
	}
	TEXTINFO;

	// ָ��������ı�
	typedef struct tagFONTTEXTINFO
	{
		INT x;											// X ����
		INT y;											// Y ����
		PCTSTR ptzText;									// �ı�
		COLORREF crColor;								// ��ɫ
		LOGFONT lfFont;									// ����
	}
	FONTTEXTINFO;

	// ������
	typedef struct tagLINKINFO
	{
		RECT rtRect;									// λ��
		TCHAR tzTitle[32];								// ����
		TCHAR tzTip[64];								// ��ʾ�ַ���
		PCTSTR ptzUrl;									// ������Ŀ�� (HIWORD(ptzUrl) Ϊ 0 ��ʾ������Ϣ)
	}
	LINKINFO;

public:
	static HWND m_hWnd;									// ���ھ��

private:
	static BOOL m_bSplash;								// �Ƿ���������ʽ��ʾ
	static INT m_iLinkIndex;							// ��ǰ������

	static DIALOGINFO m_diDialog;						// �Ի���ģ��
	static LINEINFO m_liLines[];						// ˮƽ����
	static RECTINFO m_riRects[];						// ����
	static TEXTINFO m_tiTexts[];						// �ı�
	static FONTTEXTINFO m_ftiTexts[];					// ָ��������ı�
	static LINKINFO m_liLinks[];						// ������

public:
	// ��ʾ���ڶԻ���
	static VOID Show(HWND hParent = NULL);

	// ��������ʽ��ʾ���ڶԻ���
	static VOID Splash(UINT uTimer = 1, HWND hParent = NULL);

	// �رմ��ڹ��ڶԻ���
	static VOID Close(UINT uTimer = 0);

	// ����״̬��Ϣ
	static VOID SetStatusText(PCTSTR ptzText = NULL);

private:
	// ���ڶԻ���ص�����
	static INT_PTR CALLBACK AboutDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ���ƶԻ���
	static VOID OnPaint(HDC hDC);

	// �����
	static VOID OnCommand(WPARAM wParam, LPARAM lParam = 0);

	// ��ȡָ����ĳ�����
	static INT GetLinkIndex(POINT ptPoint);

	// ���Ƴ�����
	static VOID PaintLinks(HDC hDC = NULL);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
