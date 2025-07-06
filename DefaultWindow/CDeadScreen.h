#pragma once
#include "CUI.h"

class CDeadScreen
    :public CUI
{
public:
    CDeadScreen();
    ~CDeadScreen();

public:
    // CUIÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    STATETIME       m_tHit;
    int             m_iCount;   //3¹ø¹Ù²î¸é ¹Ù²î´Â°Å ¸ØÃß±â.

private:
    const TCHAR* m_pFrameKeys[2];
};

