#pragma once

#include "Define.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	float		Get_ScrollX() { return m_fScrollX+ m_fShakeOffsetX; }
	float		Get_ScrollY() { return m_fScrollY+ m_fShakeOffsetY; }

	float		Get_ScrollXMax() { return m_fScrollXMax; }
	float		Get_ScrollYMax() { return m_fScrollYMax; }


public:
	void		Set_ScrollX(float _fX);
	void		Set_ScrollY(float _fY);

public:
	void		Fix_ScrollX(float _fX) { m_fScrollX = _fX; }
	void		Fix_ScrollY(float _fY) { m_fScrollY = _fY; }



public:
	void	   Set_ScrollXMax(float _fX) { m_fScrollXMax = _fX; }
	void		Set_ScrollYMax(float _fY) { m_fScrollYMax = _fY; }

public:
	void	  Set_LockScrollX(bool _b) { m_bLockScrollX = _b; }
	void	  Set_LockScrollY(bool _b) { m_bLockScrollY = _b; }

public:
	void	  Set_ScrollOffSetX(float _fX) { m_fOffSetX = _fX; }
	void	  Set_ScrollOffSetY(float _fY) { m_fOffSetY = _fY; }

	float	 Get_ScrollOffSetX() { return m_fOffSetX;}
	float	 Get_ScrollOffSetY() { return m_fOffSetY; }


public:
	void		Update();

public:
	void		Reset_Scroll() { m_fScrollX = 0.f; m_fScrollY = 0.f; m_fOffSetX = 0.f; m_fOffSetY = 0.f; }
public:
	void		CameraShake(float fX, float fY, float ShakeTime);

public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CScrollMgr;
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
	static CScrollMgr* m_pInstance;

	float			m_fScrollX;
	float			m_fScrollY;

	/*카메라 셰이크*/
	bool			m_bShake;

	float			m_fShakeTime;		//흔들림 지속시간
	float			m_fShakeX;			//흔들림x범위
	float			m_fShakeY;			//흔들림 y범위

	float			InitShakeX;
	float			InitShakeY;

	float			 m_fShakeOffsetX=0.f;
	float			m_fShakeOffsetY=0.f;

private:
	float			m_fScrollXMax;
	float			m_fScrollYMax;

private:
	bool			m_bLockScrollX;
	bool			m_bLockScrollY;

	float			m_fOffSetX;
	float			m_fOffSetY;

};

