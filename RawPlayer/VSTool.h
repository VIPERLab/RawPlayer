


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CVSTool ��
class CVSTool
{
public:
	// �жϹ����Ƿ��Ѿ�����
	inline static BOOL Exist()
	{
		return Operate(FALSE, 0) || Operate(TRUE, 0);
	}

	// ��ӹ���
	inline static BOOL Add()
	{
		BOOL bResult = Operate(FALSE, 1);
		bResult = Operate(TRUE, 1) | bResult;
		return bResult;
	}

	// ɾ������
	inline static BOOL Remove()
	{
		BOOL bResult = Operate(FALSE, -1);
		bResult = Operate(TRUE, -1) | bResult;
		return bResult;
	}

private:
	// ö�ٰ汾������
	static BOOL Operate(BOOL bDotNet, INT iCommand);

	// ����Ѵ򿪵Ĺ��߼�����
	static BOOL Operate(HKEY hToolKey, BOOL bDotNet, INT iCommand);

	// ��ȡ���߱��
	static DWORD Get(HKEY hToolKey, BOOL bDotNet, DWORD dwNumTools);

	// ��ӹ���
	static BOOL Add(HKEY hToolKey, DWORD dwNumTools, BOOL bDotNet, INT iCommand);

	// ���¹���
	static BOOL Update(HKEY hToolKey, BOOL bDotNet, DWORD dwIndex);

	// ɾ������
	static BOOL Remove(HKEY hToolKey, BOOL bDotNet, DWORD dwIndex, DWORD dwNumTools);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
