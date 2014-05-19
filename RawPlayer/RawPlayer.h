


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <DDraw.h>
#include <Windows.h>
#include "RawVideo.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �궨��
#define IDC_RawVideo_Change		11344
#define IDC_RawVideo_Play		11345
#define IDC_RawVideo_Seek		11346
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ȥ��ͨ��
#define CH_WIPEY				0x00000001
#define CH_WIPEU				0x00000010
#define CH_WIPEV				0x00000100
#define CH_WIPER				0x00010000
#define CH_WIPEG				0x00100000
#define CH_WIPEB				0x01000000

#define NeedWipeYUV(d)			(LOWORD(d))
#define NeedWipeRGB(d)			(HIWORD(d))
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRawPlayer ��
class CRawPlayer: public CRawVideo
{
public:
	static BOOL m_bSync;				// �Ƿ�ͬ������
	static BOOL m_bPlaying;				// �Ƿ�����ͬ������	

	static CRawPlayer *m_pHead;			// ��󴴽��Ķ���

	CRawPlayer *m_pNext;				// ѭ����������һ�������ָ��

	HWND m_hPlayWnd;					// ���Ŵ���	
	DWORD m_uCurFrame;					// ��ǰ֡����
	DWORD m_dwWipeChannel;				// ȥ��ͨ��
	MMRESULT m_mrTimer;					// ���Ŷ�ʱ��
	CRawImage m_riDst;					// Ŀ���ʽ

	BOOL m_bDrawing;					// ���ڻ���

	RECT m_rtSrc;						// ����Դ����
	RECT m_rtDraw;						// ���ƾ���
	RECT m_rtDirectDraw;				// ���ƾ���

	LPDIRECTDRAW m_pDDraw;				// DirectDraw ָ��
	DDSURFACEDESC m_sdSurface;			// DirectDraw ��������
	LPDIRECTDRAWSURFACE m_pSurface;		// DirectDraw ����ָ��
	LPDIRECTDRAWCLIPPER m_pClipper;		// DirectDraw ������ָ��
	LPDIRECTDRAWSURFACE m_pPrimary;		// DirectDraw ������ָ��

	HRESULT (CRawPlayer::*m_pDraw)(HDC hDC);	// ���ƺ���ָ��

public:
	// ���캯��
	CRawPlayer();

	// ��������
	~CRawPlayer();

public:
	// ��
	BOOL Open(PCTSTR ptzFileName, PRAWIMAGE priFormat = NULL);

	// ����
	VOID Destroy();

	// ����Ϊλͼ�ļ�
	BOOL SaveBitmap(UINT uIndex, PCTSTR ptzFileName);

private:
	// �� GDI ����
	HRESULT Paint(HDC hDC = NULL);

	// ��ʼ�� DirectDraw
	HRESULT DDInit();

	// �� DirectDraw ��Ⱦ
	HRESULT Render(HDC hDC = NULL);

	// �ͷ� DirectDraw
	HRESULT DDRelease();

	// ������
	HRESULT CheckDiff(PBYTE pbData);

public:
	// ʹ�� DirectDraw ��ʽ
	HRESULT EnableDDraw(BOOL bEnable = TRUE, BOOL bStop = TRUE);

public:
	// ��ʼ����
	BOOL Play();

	// ֹͣ����
	BOOL Stop();

	// ��λ
	VOID Seek(INT iIndex = 0);

	// ����
	VOID Step(INT iMultiple = 1);

	// ֡��
	VOID StepFrame(INT iFrame = 1);

	// ���õ�ǰ֡
	DWORD SetCurFrame(INT iIndex);

private:
	// ���Ź���
	static VOID CALLBACK PlayProc(UINT uID, UINT uMsg, CRawPlayer *pThis, DWORD dwParam1, DWORD dwParam2);

	// �������Ź���
	static VOID CALLBACK SyncPlayProc(UINT uID, UINT uMsg, CRawPlayer *pThis, DWORD dwParam1, DWORD dwParam2);

public:
	// ����
	inline VOID Draw(HDC hDC = NULL) {if (m_bDrawing == FALSE) {m_bDrawing = TRUE; (this->*m_pDraw)(hDC); m_bDrawing = FALSE;}}
	inline BOOL IsDDraw() {return m_pDraw == &CRawPlayer::Render;}

	// ����
	inline BOOL IsPlaying() {return m_bSync ? m_bPlaying : m_mrTimer;}
	inline BOOL TogglePlay() {return IsPlaying() ? Stop() : Play();}
	inline VOID Restart() {SetCurFrame((m_iFrameStep < 0) ? MAX_Frame : 0);}

	// ��������
	inline VOID SetPlayWnd(HWND hWnd) {m_hPlayWnd = hWnd; SetDrawRect();}
	inline VOID SetSrcRect(LPCRECT prtRect = NULL) {prtRect ? CopyRect(&m_rtSrc, prtRect) : SetRect(&m_rtSrc, 0, 0, m_uWidth, m_uHeight);}
	inline VOID SetDrawRect(LPCRECT prtRect = NULL) {prtRect ? CopyRect(&m_rtDraw, prtRect) : GetClientRect(m_hPlayWnd, &m_rtDraw); CopyRect(&m_rtDirectDraw, &m_rtDraw); MapWindowPoints(m_hPlayWnd, NULL, (PPOINT) &m_rtDirectDraw, 2);}
	
	// ȥ��ͨ��
	inline DWORD GetWipeChannel() {return m_dwWipeChannel;}
	inline VOID SetWipeChannel(DWORD dwMask, BOOL bWipe) {bWipe ? m_dwWipeChannel |= dwMask : m_dwWipeChannel &= ~dwMask; m_dwWipeChannel ? EnableDDraw(FALSE) : EnableDDraw(); Draw();}

	// ͬ������
	inline static BOOL SetSync(BOOL bSync) {BOOL bPrev = m_bSync; m_bSync = TRUE; if (m_pHead) m_pHead->Stop(); m_bSync = bSync; return bPrev;}
	
	inline static BOOL ToggleSync()	
	{
		BOOL bPrev = m_bSync;
		m_bSync = TRUE;
		if (m_pHead) m_pHead->Stop();
		m_bSync = !bPrev;

		for (CRawPlayer *p = m_pHead; p; p = p->m_pNext)
		{
			p->SetCurFrame(p->m_uCurFrame + 0);
			p->Draw();
		}
		return m_bSync;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
