#pragma once

#include "CMyBmp.h"

class CBmpMgr
{
private:
	CBmpMgr();
	~CBmpMgr();

public:
	HDC		Find_Image(const TCHAR* pImgKey);
	HBITMAP Get_HBitmap(const TCHAR* pImgKey);
	BITMAP	Get_Bitmap(const TCHAR* pImgKey);

	void	Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	void	Release();

public:
	void	Load();

private:
	void	Load_Background();
	void	Load_Character();
	void	Load_Weapon();
	void	Load_Object();
	void	Load_Effect();
	void	Load_UI();

public:
	static CBmpMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBmpMgr;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CBmpMgr* m_pInstance;
	map<const TCHAR*, CMyBmp*>		m_mapBit;

};

