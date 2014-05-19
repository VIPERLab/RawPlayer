


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <Windows.h>
#include "ClientWnd.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ļ��Ի���ṹ
typedef struct tagOFNEX
{
	OPENFILENAME ofnFile;
	PVOID pvReserved;
	DWORD dwReserved;
	DWORD FlagEx;
}
OFNEX, *POFNEX;

#define SIZE_OFN		CDSIZEOF_STRUCT(OPENFILENAME, lpTemplateName)
#define SIZE_OFNEX		((sizeof(OPENFILENAME) <= SIZE_OFN) ? sizeof(OFNEX) : sizeof(OPENFILENAME))
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COpenDlg ��
class COpenDlg
{
private:
	static HWND m_hWnd;
	static HWND m_hParent;
	static HWND m_hToolbar;
	static RECT m_rtPreview;
	static RAWIMAGE m_riFormat;
	static HMENU m_hFormatMenu;
	static BOOL m_bFilterChange;	
	static HIMAGELIST m_hImageLists[3];

public:
	static CChildWnd *m_pWnd;

public:
	// ���ļ�
	static HRESULT Open(PTSTR ptzFileName);

private:
	// ���ļ��Ի���ص�����
	static UINT_PTR CALLBACK OpenDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ��ʼ���Ի���
	static VOID OnInitDialog();

	// ֪ͨ��Ϣ
	static UINT OnNotify(WPARAM wParam, LPARAM lParam);

	// ���͸ı�
	static VOID OnTypeChange(LPOPENFILENAME pOfn);

	// ������С
	static VOID OnSize();

	// �����
	static VOID OnCommand(WPARAM wParam, LPARAM lParam = 0);

	// ����
	static VOID OnPaint();

	// �����ļ�����ȡ��Ƶ��С
	static BOOL GetSize(PCTSTR ptzFileName);

private:
	// �Ƿ�ѡ��ҪԤ��
	inline static BOOL IsPreviewChecked() {return IsDlgButtonChecked(m_hWnd, IDC_Open_Preview) == BST_CHECKED;}

	// �Ƿ����Ԥ��
	inline static BOOL CanPreview() {return m_pWnd->m_uTotalFrame && IsPreviewChecked();}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
