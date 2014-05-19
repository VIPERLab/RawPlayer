


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <Windows.h>
#include "RawImage.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRawVideo ��
class CRawVideo: public CRawImage
{
public:
	HANDLE m_hFile;						// �ļ����
	PBYTE m_pbRead;						// �ļ�������
	PBYTE m_pbConvert;					// ת��������
	ULARGE_INTEGER m_uliSize;			// �ļ���С
	TCHAR m_tzFileName[MAX_PATH];		// �ļ�����
	DWORD m_dwSectorSize;				// �ļ����ڵĴ��̵�������С

	UINT m_uTotalFrame;					// ��֡��

public:
	// ���캯��
	CRawVideo();

	// ��������
	~CRawVideo();

public:
	// ���ļ�
	BOOL Open(PCTSTR ptzFileName);

	// ��ȡ
	PBYTE Read(UINT uIndex);

	// ����
	VOID Destroy();

private:
	// ����
	VOID Update();

public:
	// ת��
	inline PBYTE Convert(UINT uIndex, CRawImage &riDst)
	{
		PBYTE p = Read(uIndex);
		if (p)
		{
			p = CRawImage::Convert(riDst, m_pbConvert, p);
		}
		return p;
	}

	// ת��
	inline PBYTE Convert(UINT uIndex, COLORSPACE csDst)
	{
		PBYTE p = Read(uIndex);
		if (p)
		{
			p = CRawImage::Convert(csDst, m_pbConvert, p);
		}
		return p;
	}

	// ת��
	inline PBYTE Convert(UINT uIndex, CRawImage &riDst, PBYTE pbDst)
	{
		PBYTE p = Read(uIndex);
		if (p)
		{
			CRawImage::Convert(riDst, pbDst, p);
		}
		return p;
	}

	// ת��
	inline PBYTE Convert(UINT uIndex, COLORSPACE csDst, PBYTE pbDst)
	{
		PBYTE p = Read(uIndex);
		if (p)
		{
			p = CRawImage::Convert(csDst, pbDst, p);
		}
		return p;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
