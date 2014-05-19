


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ͷ�ļ�
#include "Define.h"

#include <Windows.h>
#include <CommCtrl.h>
#include <ShlObj.h>
#include <Malloc.h>
#include <AviRiff.h>
#include <MMSystem.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ Visual C++ 6.0 ����
#if (_MSC_VER < 1300)
typedef LPCSTR							PCSTR;
typedef LPCSTR							PCSTR;
typedef LPSTR							PSTR;
typedef LPTSTR							PTSTR;
typedef LPCTSTR							PCTSTR;
typedef LPVOID							PVOID;
#if defined(_WIN64)
typedef LONGLONG						LONG_PTR;
#else
typedef LONG							LONG_PTR;
#endif
#endif

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL					(WM_MOUSELAST + 1)
#define WHEEL_DELTA						120
#endif
#ifndef MIIM_STRING
#define MIIM_STRING						0x00000040
#endif
#ifndef IDC_HAND
#define IDC_HAND						MAKEINTRESOURCE(32649)
#endif
#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE				0x0040
#endif
#ifndef CSIDL_MYPICTURES
#define CSIDL_MYPICTURES				0x0027
#endif
#ifndef GetClassLongPtr
#define GetClassLongPtr					GetClassLong
#define SetClassLongPtr					SetClassLong
#define GCLP_HCURSOR					GCL_HCURSOR
#endif
#ifndef GetWindowLongPtr
#define GetWindowLongPtr				GetWindowLong
#define SetWindowLongPtr				SetWindowLong
#define GWLP_USERDATA					GWL_USERDATA
#define GWLP_WNDPROC					GWL_WNDPROC
#define DWLP_MSGRESULT					DWL_MSGRESULT
#define GCLP_HBRBACKGROUND				GCL_HBRBACKGROUND
#endif
#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES			-1
#endif
#ifndef VK_OEM_PLUS
#define VK_OEM_PLUS						0xBB
#define VK_OEM_COMMA					0xBC
#define VK_OEM_MINUS					0xBD
#define VK_OEM_PERIOD					0xBE
#endif
#ifndef IS_INTRESOURCE
#define IS_INTRESOURCE(r)				(((ULONG) (r) >> 16) == 0)
#endif
#ifndef CDSIZEOF_STRUCT
#define CDSIZEOF_STRUCT(s, m)			(((INT) ((PBYTE) (&((s *) 0)->m) - ((PBYTE) ((s *) 0)))) + sizeof(((s *) 0)->m))
#endif
#ifndef LVM_SORTITEMSEX
#define LVM_SORTITEMSEX					(LVM_FIRST + 81)
#define ListView_SortItemsEx(h, f, p)	(BOOL) SNDMSG((h), LVM_SORTITEMSEX, (WPARAM) (p), (LPARAM) (f))
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ͷ�ļ�
#include "Macro.h"
#include "Resource.h"

#include "MainWnd.h"
#include "OpenDlg.h"
#include "ExportDlg.h"
#include "ChildWnd.h"
#include "ClientWnd.h"
#include "AboutDlg.h"

#include "Ini.h"
#include "VSTool.h"
#include "BitLive.h"
#include "Utility.h"
#include "Shortcut.h"
#include "Language.h"
#include "RecentFile.h"

#include "RawPlayer.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���
extern HINSTANCE g_hInst;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INI ����
#define INI_Language			TEXT("Language")
#define INI_WindowLeft			TEXT("WindowLeft")
#define INI_WindowTop			TEXT("WindowTop")
#define INI_WindowWidth			TEXT("WindowWidth")
#define INI_WindowHeight		TEXT("WindowHeight")
#define INI_WindowState			TEXT("WindowState")
#define INI_RecentFile			TEXT("RecentFile")
#define INI_MaxRecentFile		TEXT("MaxRecentFile")
#define INI_Toolbar				TEXT("Toolbar")
#define INI_StatusBar			TEXT("StatusBar")
#define INI_AlwaysOnTop			TEXT("AlwaysOnTop")
#define INI_MinToTray			TEXT("MinToTray")

#define INI_BitLive				TEXT("BitLive")
#define INI_UpdateBuild			TEXT("UpdateBuild")
#define INI_UpdateUrl			TEXT("UpdateUrl")
#define INI_UpdateBox			TEXT("UpdateBox")
#define INI_UpdateText			TEXT("UpdateText")
#define INI_UpdateSize			TEXT("UpdateSize")
#define INI_UpdateReplace		TEXT("UpdateReplace")
#define INI_UpdateExit			TEXT("UpdateExit")
#define INI_NewsBox				TEXT("NewsBox")
#define INI_NewsText			TEXT("NewsText")

#define INI_OpenPeview			TEXT("OpenPreview")
#define INI_PicturesFolder		TEXT("PicturesFolder")
#define INI_ClientColor			TEXT("ClientColor")
#define INI_ShowSplash			TEXT("ShowSplash")
#define INI_ShowOpen			TEXT("ShowOpen")

#define INI_HistroyFormat		TEXT("HistroyFormat")
#define INI_ExportFormat		TEXT("ExportFormat")
#define INI_ExportRange			TEXT("ExportRange")
#define INI_PlayOnOpen			TEXT("PlayOnOpen")
#define INI_MaxOnOpen			TEXT("MaxOnOpen")
#define INI_DirectDraw			TEXT("DirectDraw")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����������ַ���
#ifdef _CHS

#define LNG_Ready				_Lang(100, "������")
#define LNG_Version				_Lang(101, "�汾: %s")
#define LNG_Build				_Lang(102, "����: %s")

#define	LNG_CheckUpdate			_Lang(103, "������")
#define LNG_CancelUpdate		_Lang(104, "ȡ������")
#define	LNG_VisitWeb			_Lang(105, "������վ")
#define	LNG_WriteEmail			_Lang(106, "������д��")

#define LNG_CheckUpdateTip		_Lang(107, "�� Internet �ϼ�鱾����ĸ��°汾��")
#define LNG_VisitWebTip			_TLang(108, TEXT("���� ") STR_Web TEXT("��"))
#define LNG_WriteEmailTip		_TLang(109, TEXT("д�Ÿ� ") STR_Email TEXT("��"))

#define LNG_BitLiveCheck		_Lang(110, "��ʼ������...")
#define LNG_BitLiveUpdate		_Lang(111, "��ʼ��ȡ����...")
#define LNG_BitLiveUpToDate		_Lang(112, "����ʹ�õ��Ѿ������°汾��")
#define LNG_BitLiveCheckErr		_Lang(113, "�����¹����г��ִ���")
#define LNG_BitLiveUpdateErr	_Lang(114, "����δ�ܳɹ���ɡ�")
#define LNG_BitLiveRecieve		_Lang(115, "���յ� %u �ֽ�...")
#define LNG_BitLiveRecvPercent	_Lang(116, "���յ� %u �ֽ� (%u%%)...")
#define LNG_BitLiveConfirm		_LangEx(117, "�°汾�Ѿ�������ɣ�������Ϊ:\n\n    %s\n\n����ҪӦ�ø�����?")

#else

#define LNG_Ready				_Lang(100, "Ready.")
#define LNG_Version				_Lang(101, "Ver: %s")
#define LNG_Build				_Lang(102, "Build: %s")

#define	LNG_CheckUpdate			_Lang(103, "Check Update")
#define	LNG_CancelUpdate		_Lang(104, "Cancel Update")
#define	LNG_VisitWeb			_Lang(105, "Visit Web Site")
#define	LNG_WriteEmail			_Lang(106, "Email To Author")

#define LNG_CheckUpdateTip		_Lang(107, "Check new version via Internet.")
#define LNG_VisitWebTip			_TLang(108, TEXT("Visit ") STR_Web TEXT("."))
#define LNG_WriteEmailTip		_TLang(109, TEXT("Email to ") STR_Email TEXT("."))

#define LNG_BitLiveCheck		_Lang(110, "Begin BitLive...")
#define LNG_BitLiveUpdate		_Lang(111, "Begin LiveUpdate...")
#define LNG_BitLiveUpToDate		_Lang(112, "The current version is up to date.")
#define LNG_BitLiveCheckErr		_Lang(113, "Failed to check update.")
#define LNG_BitLiveUpdateErr	_Lang(114, "LiveUpdate completed unsuccessfully.")
#define LNG_BitLiveRecieve		_Lang(115, "Recieved %u bytes ...")
#define LNG_BitLiveRecvPercent	_Lang(116, "Recieved %u bytes (%u%%) ...")
#define LNG_BitLiveConfirm		_LangEx(117, "The update version has been downloaded successfully:\n\n    %s\n\nApply this update now?")

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������ַ���
#ifdef _CHS
#define LNG_ExportFilter		_LangEx(200, "YV12 �ļ� (*.yv12;*.yuv)\0*.yv12;*.yuv\0I420 �ļ� (*.i420;*.yuv)\0*.i420;*.yuv\0YUY2 �ļ� (*.yuy2;*.yuv)\0*.yuy2;*.yuv\0YVYU �ļ� (*.yvyu;*.yuv)\0*.yvyu;*.yuv\0UYVY �ļ� (*.uyvy;*.yuv)\0*.uyvy;*.yuv\0VYUY �ļ� (*.vyuy;*.yuv)\0*.vyuy;*.yuv\0RGB32 �ļ� (*.rgb32;*.rgb)\0*.rgb32;*.rgb\0RGB24 �ļ� (*.rgb24;*.rgb)\0*.rgb24;*.rgb\0RGB16 �ļ� (*.rgb16;*.rgb)\0*.rgb16;*.rgb\0RGB15 �ļ� (*.rgb15;*.rgb)\0*.rgb15;*.rgb\0�����ļ� (*.*)\0*.*\0λͼ�ļ� (*.bmp)\0*.bmp\0")
#define LNG_BrowseFolder		_Lang(201, "��ѡ���ļ��е�λ�á�")

#define LNG_OpenErr				_Lang(300, "�޷����ļ� %s��")
#define ERR_HelpErr				_Lang(301, "�򿪰����ļ�ʧ�ܣ������°�װ MVPlayer��")
#define LNG_CreateShortcutErr	_Lang(302, "������ݷ�ʽʧ�ܡ�")
#define LNG_RemoveShortcutErr	_Lang(303, "ɾ����ݷ�ʽʧ�ܡ�")

#define LNG_ExportErr			_Lang(304, "��������δ�ܳɹ���ɡ�")
#define LNG_AddToVSErr			_Lang(305, "������ݷ�ʽʧ�ܣ�����û�а�װ Microsoft Visual Studio��")
#define LNG_RemoveFromVSErr		_Lang(306, "�� Microsoft Visual Studio �о߲˵���ɾ����ݷ�ʽʧ�ܡ�")
#define LNG_InputErr			_Lang(307, "���ܽ����������롣")
#define LNG_UnexpectedErr		_Lang(308, "������δ֪����")
#define LNG_DirectDrawErr		_Lang(309, "DirectDraw ���ִ���: %#08X������������Կ���֧�� YUV ���档")

#define LNG_CreateShortcutMsg	_Lang(400, "�Ѿ��ɹ�������ݷ�ʽ��")
#define LNG_RemoveShortcutMsg	_Lang(401, "�Ѿ��ɹ�ɾ����ݷ�ʽ��")
#define LNG_ConvertMsg			_Lang(402, "����ת���� %u ֡ (�� Esc ȡ��)...")
#define LNG_ExportMsg			_Lang(403, "���������ɹ���ɣ��ܹ���ʱ %u ���롣")
#define LNG_AddToVSMsg			_LangEx(404, "�Ѿ��� Microsoft Visual Studio ���߲˵��д�����ݷ�ʽ��\n\n��ȷ�� Microsoft Visual Studio 5.x/6.x û�����У���������޷������ݷ�ʽ��")
#define LNG_RemoveFromVSMsg		_LangEx(405, "�Ѿ��� Microsoft Visual Studio ���߲˵���ɾ����ݷ�ʽ��\n\n��ȷ�� Microsoft Visual Studio 5.x/6.x û�����У���������޷�ɾ����ݷ�ʽ��")

#define LNG_ColorSpaceMsg		_LangEx(406, "�����ɫϵ (%s)���� Enter ȷ�ϣ�Esc ȡ����")
#define LNG_FrameWidthMsg		_LangEx(407, "������� (%d - %d)���� Enter ȷ�ϣ�Esc ȡ����")
#define LNG_FrameHeightMsg		_LangEx(408, "�����߶� (-%d - -%d, %d - %d)���� Enter ȷ�ϣ�Esc ȡ����")
#define LNG_FrameRateMsg		_LangEx(409, "�����֡�� (%d - %d)���� Enter ȷ�ϣ�Esc ȡ����")
#define LNG_FrameStepMsg		_LangEx(410, "�����֡�� (%d - %d)���� Enter ȷ�ϣ�Esc ȡ����")
#define LNG_FrameIndexeMsg		_LangEx(411, "�����Ҫת������֡ (%d - %d)���� Enter ȷ�ϣ�Esc ȡ����")

#else // _CHS

#define LNG_ExportFilter		_LangEx(200, "YV12 Files (*.yv12;*.yuv)\0*.yv12;*.yuv\0I420 Files (*.i420;*.yuv)\0*.i420;*.yuv\0YUY2 Files (*.yuy2;*.yuv)\0*.yuy2;*.yuv\0YVYU Files (*.yvyu;*.yuv)\0*.yvyu;*.yuv\0UYVY Files (*.uyvy;*.yuv)\0*.uyvy;*.yuv\0VYUY Files (*.vyuy;*.yuv)\0*.vyuy;*.yuv\0RGB32 Files (*.rgb32;*.rgb)\0*.rgb32;*.rgb\0RGB24 Files (*.rgb24;*.rgb)\0*.rgb24;*.rgb\0RGB16 Files (*.rgb16;*.rgb)\0*.rgb16;*.rgb\0RGB15 Files (*.rgb15;*.rgb)\0*.rgb15;*.rgb\0All Files (*.*)\0*.*\0Bitmap Files (*.bmp)\0*.bmp\0")
#define LNG_BrowseFolder		_Lang(201, "Please select folder location.")

#define LNG_OpenErr				_Lang(300, "Failed to open file %s.")
#define ERR_HelpErr				_Lang(301, "Failed to open help file. Please re-install MVPlayer.")
#define LNG_CreateShortcutErr	_Lang(302, "Failed to create shortcut.")
#define LNG_RemoveShortcutErr	_Lang(303, "Failed to remove shortcut.")

#define LNG_ExportErr			_Lang(304, "Export operation complete unsuccessfully.")
#define LNG_AddToVSErr			_Lang(305, "Failed to create shortcut. Probably Microsoft Visual Studio is not installed.")
#define LNG_RemoveFromVSErr		_Lang(306, "Failed to remove shortcut from Microsoft Visual Studio.")
#define LNG_InputErr			_Lang(307, "Could not accept your input.")
#define LNG_UnexpectedErr		_Lang(308, "An unexpected error occurred.")
#define LNG_DirectDrawErr		_Lang(309, "DirectDraw error occurred: %#08X. Maybe your video card could not support YUV surface.")

#define LNG_CreateShortcutMsg	_Lang(400, "Shortcut has been created successfully.")
#define LNG_RemoveShortcutMsg	_Lang(401, "Shortcut has been removed successfully.")
#define LNG_ConvertMsg			_Lang(402, "Converting %u (Press Esc to cancel)...")
#define LNG_ExportMsg			_Lang(403, "Export operation completed in %u ms.")
#define LNG_AddToVSMsg			_LangEx(404, "Shortcut has been created successfully.\n\nBe sure that Microsoft Visual Studio 5.x/6.x is not running, otherwise the shortcut may not be saved.\n\n")
#define LNG_RemoveFromVSMsg		_LangEx(405, "Shortcut has been removed successfully.\n\nBe sure that Microsoft Visual Studio 5.x/6.x is not running, otherwise the shortcut may not be removed.\n\n")

#define LNG_ColorSpaceMsg		_LangEx(406, "Input color space (%s). Enter to confirm, Esc to cancel.")
#define LNG_FrameWidthMsg		_LangEx(407, "Input frame width (%d - %d). Enter to confirm, Esc to cancel.")
#define LNG_FrameHeightMsg		_LangEx(408, "Input frame height (-%d - -%d, %d - %d). Enter to confirm, Esc to cancel.")
#define LNG_FrameRateMsg		_LangEx(409, "Input frame rate (%d - %d). Enter to confirm, Esc to cancel.")
#define LNG_FrameStepMsg		_LangEx(410, "Input frame step (%d - %d). Enter to confirm, Esc to cancel.")
#define LNG_FrameIndexeMsg		_LangEx(411, "Input frame index that you want to go (%d - %d). Enter to confirm, Esc to cancel.")

#endif // _CHS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
