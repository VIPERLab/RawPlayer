


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <Windows.h>
#include "ChildWnd.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CExportDlg ��
class CExportDlg
{
private:
	static HWND m_hWnd;
	static HWND m_hParent;
	static CChildWnd *m_pWnd;

public:	
	static BOOL m_bOneFile;
	static DWORD m_dwFromFrame;
	static DWORD m_dwToFrame;
	static COLORSPACE m_csColorSpace;
	static TCHAR m_tzFileName[MAX_PATH];

public:
	// ����
	static BOOL Show(CChildWnd *pWnd);

private:
	// ���ļ��Ի���ص�����
	static UINT_PTR CALLBACK ExportDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
