#pragma once
#include "CUI.h"
class CBar :
    public CUI
{
public:
    CBar();
    virtual ~CBar();

public:
	virtual void	Initialize();
	void		Initialize(HPBAR_TYPE _Type,Vec2 _pos, Vec2 _Scale, const TCHAR* _FrameKey);
	virtual int		Update();
	virtual void	Late_Update();
	virtual void	Render(HDC hDC);
	virtual void	Release();

public:
	void	Set_PicSize(Vec2 _Size) { m_pPicSize = _Size; }
	void	Set_Value(float _Value) { m_fValue = _Value;}
	void	Set_RemoveColor(COLORREF _Color) { m_Color = _Color; }
public:
	float	Get_PicSizeX() { return m_pPicSize.x; }
	
public:
	int		Get_BarWidth() {return m_barWidth;}

public:
	void	Render_Current(HDC hDC);
	void	Render_BackGround(HDC hDC);
	void	Render_Damage(HDC hDC);

	void		Set_SkipDamageAnim(bool b) { m_bSkipDamageAnim = b; }
private:
	HPBAR_TYPE		m_eType;

	float			m_fValue;
	float			m_fRenderValue;

	Vec2			m_pPicSize;

	int				m_barWidth;
	COLORREF		m_Color;


private:
	bool		m_bSkipDamageAnim = false;
};

