


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#include "Main.h"
#include "RawPlayer.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRawPlayer �ྲ̬����
BOOL CRawPlayer::m_bSync = FALSE;
BOOL CRawPlayer::m_bPlaying = FALSE;
CRawPlayer *CRawPlayer::m_pHead = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���캯��
CRawPlayer::CRawPlayer()
{
	m_pNext = m_pHead;
	m_pHead = this;

	m_hPlayWnd = NULL;
	m_mrTimer = 0;
	m_uCurFrame = 0;
	m_dwWipeChannel = 0;
	SetRectEmpty(&m_rtSrc);
	SetRectEmpty(&m_rtDraw);
	SetRectEmpty(&m_rtDirectDraw);
	m_bDrawing = FALSE;

	m_pDraw = &CRawPlayer::Paint;

	m_bSync = FALSE;
	m_bPlaying = FALSE;

	m_pDDraw = NULL;
	m_pSurface = NULL;
	m_pClipper = NULL;
	m_pPrimary = NULL;
	_ZeroMem(m_sdSurface);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
CRawPlayer::~CRawPlayer()
{
	Destroy();

	if (m_pHead == this)
	{
		m_pHead = m_pNext;
	}
	else
	{
		for (CRawPlayer *p = m_pHead; p; p = p->m_pNext)
		{
			if (p->m_pNext == this)
			{
				p->m_pNext = m_pNext;
				break;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��
BOOL CRawPlayer::Open(PCTSTR ptzFileName, PRAWIMAGE priFormat)
{
	// ����
	Destroy();

	// ����
	if (priFormat)
	{
		m_csColorSpace = priFormat->m_csColorSpace;
		m_uWidth = priFormat->m_uWidth;
		m_iHeight = priFormat->m_iHeight;
		m_iFrameStep = priFormat->m_iFrameStep;
		m_uFrameRate = priFormat->m_uFrameRate;
	}

	// ��
	if (CRawVideo::Open(ptzFileName) == FALSE)
	{
		return FALSE;
	}

	m_riDst = *this;
	m_riDst.m_iHeight = -m_iHeight;
	m_riDst.Update(CS_RGB24);

	// ��������
	Restart();
	SetSrcRect();
	SetDrawRect();

	// ֪ͨ
	SendMessage(m_hPlayWnd, WM_COMMAND, IDC_RawVideo_Change, (LPARAM) this);

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����
VOID CRawPlayer::Destroy()
{
	// ֹͣ����
	Stop();
	EnableDDraw(FALSE);
	m_uCurFrame = 0;

	// ����
	CRawVideo::Destroy();

	// ֪ͨ
	SendMessage(m_hPlayWnd, WM_COMMAND, IDC_RawVideo_Change, (LPARAM) this);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����Ϊλͼ�ļ�
BOOL CRawPlayer::SaveBitmap(UINT uIndex, PCTSTR ptzFileName)
{
	HANDLE hFile;
	DWORD dwSize;
	PBYTE pbBitmap;
	BITMAPFILEHEADER bfhFile;
	BITMAPINFOHEADER bihBitmap;

	// ��ȡͼ��
	pbBitmap = Convert(uIndex, CS_RGB24);
	_ReturnValIf(pbBitmap == NULL, E_FAIL);

	// ���ͼ����Ϣ
	_ZeroMem(bihBitmap);
	bihBitmap.biSize = sizeof(BITMAPINFOHEADER);
	bihBitmap.biPlanes = 1;
	bihBitmap.biBitCount = 24;
	bihBitmap.biWidth = m_uWidth;
	bihBitmap.biHeight = -m_iHeight;
	bihBitmap.biSizeImage = _DibSize24(m_uWidth, m_uHeight);

	// �����ļ���Ϣ
	bfhFile.bfType = 0x4D42;
	bfhFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bihBitmap.biSizeImage;
	bfhFile.bfReserved1 = 0;
	bfhFile.bfReserved2 = 0;
	bfhFile.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	hFile = _CreateFileForWrite(ptzFileName);
	_ReturnValIf(hFile == INVALID_HANDLE_VALUE, FALSE);

	WriteFile(hFile, &bfhFile, sizeof(BITMAPFILEHEADER), &dwSize, NULL);
	WriteFile(hFile, &bihBitmap, sizeof(BITMAPINFOHEADER), &dwSize, NULL);
	WriteFile(hFile, pbBitmap, bihBitmap.biSizeImage, &dwSize, NULL);

	return CloseHandle(hFile);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �� GDI ����
HRESULT CRawPlayer::Paint(HDC hDC)
{
	// ��ȡͼ��
	PBYTE pbBitmap = Read(m_uCurFrame);
	if (pbBitmap)
	{
		CheckDiff(pbBitmap);

		// ȥ�� YUV ͨ��
		if (NeedWipeYUV(m_dwWipeChannel))
		{
			PBYTE p, pbU, pbV, pbEnd;
			p = pbBitmap;
			switch (m_csColorSpace)
			{
			case CS_YV12:
			case CS_I420:
				GetUV(pbBitmap, &pbU, &pbV);
				_ExIf(m_dwWipeChannel & CH_WIPEY, memset(p, 0, m_uXStride * m_uHeight));
				_ExIf(m_dwWipeChannel & CH_WIPEU, memset(pbU, 128, m_uUvStride * m_uHeight / 2));
				_ExIf(m_dwWipeChannel & CH_WIPEV, memset(pbV, 128, m_uUvStride * m_uHeight / 2));
				break;

			case CS_YUY2:
				for (pbEnd = pbBitmap + m_uXStride * m_uHeight; p < pbEnd; p += 4)
				{
					_ExIf(m_dwWipeChannel & CH_WIPEY, p[0] = p[2] = 0);
					_ExIf(m_dwWipeChannel & CH_WIPEU, p[1] = 128);
					_ExIf(m_dwWipeChannel & CH_WIPEV, p[3] = 128);
				}
				break;

			case CS_UYVY:
				for (pbEnd = pbBitmap + m_uXStride * m_uHeight; p < pbEnd; p += 4)
				{
					_ExIf(m_dwWipeChannel & CH_WIPEY, p[1] = p[3] = 0);
					_ExIf(m_dwWipeChannel & CH_WIPEU, p[0] = 128);
					_ExIf(m_dwWipeChannel & CH_WIPEV, p[2] = 128);
				}
				break;
			}
		}

		pbBitmap = CRawImage::Convert(m_riDst, m_pbConvert, pbBitmap);
	}
	_ReturnValIf(pbBitmap == NULL, E_FAIL);

	// ȥ�� RGB ͨ��
	if (NeedWipeRGB(m_dwWipeChannel))
	{
		for (UINT y = 0; y < m_riDst.m_uHeight; y++)
		{
			PBYTE p = pbBitmap + m_riDst.m_uXStride * y;
			for (UINT x = 0; x < m_riDst.m_uWidth; x++, p += 3)
			{
				_ExIf(m_dwWipeChannel & CH_WIPEB, p[0] = 0);
				_ExIf(m_dwWipeChannel & CH_WIPEG, p[1] = 0);
				_ExIf(m_dwWipeChannel & CH_WIPER, p[2] = 0);
			}
		}
	}

	// ��ȡͼ����Ϣ
	BITMAPINFOHEADER bihBitmap = {0};
	bihBitmap.biSize = sizeof(BITMAPINFOHEADER);
	bihBitmap.biPlanes = 1;
	bihBitmap.biBitCount = 24;
	bihBitmap.biWidth = m_uWidth;
	bihBitmap.biHeight = m_uHeight;
	bihBitmap.biSizeImage = _DibSize24(m_uWidth, m_uHeight);

	// ����
	BOOL bGetDC = (hDC == NULL);
	_ExIf(bGetDC, hDC = GetDC(m_hPlayWnd));
	SetStretchBltMode(hDC, COLORONCOLOR);

	StretchDIBits(hDC,
		m_rtDraw.left, m_rtDraw.top, _RectWidth(m_rtDraw), _RectHeight(m_rtDraw),
		m_rtSrc.left, m_rtSrc.top, _RectWidth(m_rtSrc), _RectHeight(m_rtSrc),
		pbBitmap, (PBITMAPINFO) &bihBitmap, DIB_RGB_COLORS, SRCCOPY);

	_ExIf(bGetDC, ReleaseDC(m_hPlayWnd, hDC));

	return S_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʼ�� DirectDraw
HRESULT CRawPlayer::DDInit()
{
	// �ж��Ƿ������ò��Ŵ���
	_ReturnValIf(m_hPlayWnd == NULL, E_FAIL);
	DDRelease();

	// ���� DirectDraw ����
	HRESULT hResult = DirectDrawCreate(NULL, &m_pDDraw, NULL);
	_ReturnValIf(FAILED(hResult), hResult);
	hResult = m_pDDraw->SetCooperativeLevel(m_hPlayWnd, DDSCL_NORMAL);
	_ReturnValIf(FAILED(hResult), hResult);

	// ����������
	_ZeroMem(m_sdSurface);
	m_sdSurface.dwSize = sizeof(DDSURFACEDESC);
	m_sdSurface.dwFlags = DDSD_CAPS;
	m_sdSurface.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	hResult = m_pDDraw->CreateSurface(&m_sdSurface, &m_pPrimary, NULL);
	_ReturnValIf(FAILED(hResult), hResult);

	// ���ô��ڼ�����
	hResult = m_pDDraw->CreateClipper(0, &m_pClipper, NULL);
	m_pClipper->SetHWnd(0, m_hPlayWnd);
	m_pPrimary->SetClipper(m_pClipper);

	// ������������
	m_sdSurface.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	m_sdSurface.dwWidth = m_uWidth;
	m_sdSurface.dwHeight = m_uHeight;
	m_sdSurface.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	m_sdSurface.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	m_sdSurface.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
	switch (m_csColorSpace)
	{
	case CS_YV12:
	case CS_I420:
	case CS_YUY2:
		m_riDst.m_csColorSpace = CS_YUY2;
		m_sdSurface.ddpfPixelFormat.dwFourCC = FCC('YUY2');
		break;

	case CS_UYVY:
		m_riDst.m_csColorSpace = CS_UYVY;
		m_sdSurface.ddpfPixelFormat.dwFourCC = FCC('UYVY');
		break;

	default:
		m_sdSurface.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		break;
	}
	hResult = m_pDDraw->CreateSurface(&m_sdSurface, &m_pSurface, NULL);
	_ReturnValIf(FAILED(hResult), hResult);

	// ��ȡ��������
	m_sdSurface.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;
	hResult = m_pSurface->GetSurfaceDesc(&m_sdSurface);
	_ReturnValIf(FAILED(hResult), hResult);

	// ����Ŀ���ʽ
	if (m_sdSurface.ddpfPixelFormat.dwFourCC == 0)
	{
		switch (m_sdSurface.ddpfPixelFormat.dwRGBBitCount)
		{
		case 32:
			m_riDst.m_csColorSpace = CS_RGB32;
			break;

		case 24:
			m_riDst.m_csColorSpace = CS_RGB24;
			break;

		case 16:
			m_riDst.m_csColorSpace = CS_RGB16;
			break;

		case 15:
			m_riDst.m_csColorSpace = CS_RGB15;
			break;

		default:
			hResult = E_FAIL;
			break;
		}
	}

	m_riDst.Update();
	m_riDst.m_uXStride = m_sdSurface.lPitch;

	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ʹ�� DirectDraw ��Ⱦ
HRESULT CRawPlayer::Render(HDC hDC)
{
	// ��������
	_Assert(m_pSurface);
	HRESULT hResult = m_pSurface->Lock(NULL, &m_sdSurface, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	if (SUCCEEDED(hResult))
	{
		// ת��������
		if (PBYTE pbData = CRawVideo::Convert(m_uCurFrame, m_riDst, (PBYTE) m_sdSurface.lpSurface))
		{
			CheckDiff(pbData);

			// ����
			m_pSurface->Unlock(NULL);
			hResult = m_pPrimary->Blt(&m_rtDirectDraw, m_pSurface, &m_rtSrc, 0, NULL);
		}
		else
		{
			m_pSurface->Unlock(NULL);
			hResult = E_FAIL;
		}
	}

	// ʧ����
	if (FAILED(hResult))
	{
		// �ͷ�
		EnableDDraw(FALSE, FALSE);
		Paint();
	}

	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// �ͷ� DirectDraw
HRESULT CRawPlayer::DDRelease()
{
	// �ͷ�
	_SafeRelease(m_pSurface);
	_SafeRelease(m_pPrimary);
	_SafeRelease(m_pClipper);
	_SafeRelease(m_pDDraw);

	return S_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ʹ�� DirectDraw ��ʽ
HRESULT CRawPlayer::EnableDDraw(BOOL bEnable, BOOL bStop)
{
	BOOL bPlay = bStop && Stop();
	HRESULT hResult = DDRelease();
	if (bEnable)
	{
		m_riDst.m_iHeight = m_iHeight;
		hResult = DDInit();
		if (SUCCEEDED(hResult))
		{
			m_dwWipeChannel = 0;
			m_pDraw = &CRawPlayer::Render;
		}
		else
		{
			DDRelease();
		}
	}
	else
	{
		m_riDst.m_iHeight = -m_iHeight;
		m_riDst.Update(CS_RGB24);
		m_pDraw = &CRawPlayer::Paint;
	}

	if (bPlay)
	{
		Play();
	}

	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����
BOOL CRawPlayer::Play()
{
	_Assert(m_hPlayWnd);

	if (m_bSync)
	{
		if (m_bPlaying == NULL)
		{
			m_bPlaying = TRUE;
			for (CRawPlayer *p = m_pHead; p; p = p->m_pNext)
			{
				PostMessage(p->m_hPlayWnd, WM_COMMAND, IDC_RawVideo_Play, TRUE);
			}
			m_mrTimer = timeSetEvent(1000 / m_uFrameRate, 0,
				(LPTIMECALLBACK) SyncPlayProc, (DWORD_PTR) this, TIME_PERIODIC);
		}

		return TRUE;
	}
	else
	{
		if (m_mrTimer == 0)
		{
			m_mrTimer = timeSetEvent(1000 / m_uFrameRate, 0,
				(LPTIMECALLBACK) PlayProc, (DWORD_PTR) this, TIME_PERIODIC);
			PostMessage(m_hPlayWnd, WM_COMMAND, IDC_RawVideo_Play, IsPlaying());
		}

		return IsPlaying();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ֹͣ
BOOL CRawPlayer::Stop()
{
	BOOL bPrev;

	if (m_bSync)
	{
		bPrev = m_bPlaying;
		m_bPlaying = FALSE;
		for (CRawPlayer *p = m_pHead; p; p = p->m_pNext)
		{
			// ��ֹʱ��
			if (p->m_mrTimer)
			{
				timeKillEvent(p->m_mrTimer);
				p->m_mrTimer = NULL;
			}

			// �ȴ����ƽ���
			while (p->m_bDrawing)
			{
				Sleep(1);
			}

			// ֪ͨ����
			PostMessage(p->m_hPlayWnd, WM_COMMAND, IDC_RawVideo_Play, FALSE);
		}

		return bPrev;
	}
	else
	{
		// ��ֹʱ��
		bPrev = m_mrTimer;
		if (m_mrTimer)
		{
			timeKillEvent(m_mrTimer);
			m_mrTimer = NULL;
		}

		// �ȴ����ƽ���
		while (m_bDrawing)
		{
			Sleep(1);
		}

		// ֪ͨ����
		PostMessage(m_hPlayWnd, WM_COMMAND, IDC_RawVideo_Play, IsPlaying());

		return bPrev;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��λ
VOID CRawPlayer::Seek(INT iIndex)
{
	if (m_bSync)
	{
		for (CRawPlayer *p = m_pHead; p; p = p->m_pNext)
		{
			p->SetCurFrame(iIndex);
			p->Draw();
		}
	}
	else
	{
		SetCurFrame(iIndex);
		Draw();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ָ��������
VOID CRawPlayer::Step(INT iMultiple)
{
	if (m_bSync)
	{
		for (CRawPlayer *p = m_pHead; p; p = p->m_pNext)
		{
			p->SetCurFrame(p->m_uCurFrame + iMultiple * abs(p->m_iFrameStep));
			p->Draw();
		}
	}
	else
	{
		SetCurFrame(m_uCurFrame + iMultiple * abs(m_iFrameStep));
		Draw();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ָ��֡����
VOID CRawPlayer::StepFrame(INT iFrame)
{
	if (m_bSync)
	{
		for (CRawPlayer *p = m_pHead; p; p = p->m_pNext)
		{
			p->SetCurFrame(p->m_uCurFrame + iFrame);
			p->Draw();
		}
	}
	else
	{
		SetCurFrame(m_uCurFrame + iFrame);
		Draw();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �趨��ǰ֡
DWORD CRawPlayer::SetCurFrame(INT iIndex)
{
	if (iIndex < 0)
	{
		m_uCurFrame = 0;
	}
	else if (iIndex >= (INT) m_uTotalFrame)
	{
		if (m_uTotalFrame)
		{
			m_uCurFrame = m_uTotalFrame - 1;
		}
		else
		{
			m_uCurFrame = 0;
		}
	}
	else
	{
		m_uCurFrame = (DWORD) iIndex;
	}

	if (!m_bSync || (m_pHead == this))
	{
		PostMessage(m_hPlayWnd, WM_COMMAND, IDC_RawVideo_Seek, m_uCurFrame);
	}

	return m_uCurFrame;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���Ź���
VOID CALLBACK CRawPlayer::PlayProc(UINT uID, UINT uMsg, CRawPlayer *pThis, DWORD dwParam1, DWORD dwParam2)
{
	// ����
	pThis->Draw();

	// ������һ֡
	if (((pThis->m_iFrameStep < 0) && (pThis->m_uCurFrame == 0)) ||
		((pThis->m_iFrameStep >= 0) && (pThis->m_uCurFrame == pThis->m_uTotalFrame - 1)))
	{
		pThis->Stop();
	}
	else
	{
		pThis->SetCurFrame((INT) pThis->m_uCurFrame + pThis->m_iFrameStep);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ͬ�����Ź���
VOID CALLBACK CRawPlayer::SyncPlayProc(UINT uID, UINT uMsg, CRawPlayer *pThis, DWORD dwParam1, DWORD dwParam2)
{
	// �������ж���
	for (CRawPlayer *p = m_pHead; p; p = p->m_pNext)
	{
		// ����
		p->Draw();

		// ������һ֡
		if (((p->m_iFrameStep < 0) && (p->m_uCurFrame == 0)) ||
			((p->m_iFrameStep >= 0) && (p->m_uCurFrame == p->m_uTotalFrame - 1)))
		{
			p->Stop();
		}
		else
		{
			p->SetCurFrame((INT) p->m_uCurFrame + p->m_iFrameStep);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ͬ������ʱ��������
#include <AtlBase.h>
#include <AtlCrypt.h>
HRESULT CRawPlayer::CheckDiff(PBYTE pbData)
{
	if (m_bSync)
	{
		// ���㵱ǰ֡ MD5 ��
		BYTE md5[32];
		DWORD len = sizeof(md5);
		CCryptProv prov;
		CCryptMD5Hash hash;
		HRESULT hResult = prov.Initialize();
		hResult = hash.Initialize(prov);
		hResult = hash.AddData(pbData, m_uFrameSize);

		static BYTE s_md5[32];
		if (this == m_pHead)
		{
			hResult = hash.GetValue(s_md5, &len);
		}
		else
		{
			hResult = hash.GetValue(md5, &len);
			CChildWnd::GetWnd(m_hPlayWnd)->SetStatusText((memcmp(md5, s_md5, len) == 0) ? TEXT("SAME") : TEXT("DIFF"), 6);
		}
	}
	return S_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
