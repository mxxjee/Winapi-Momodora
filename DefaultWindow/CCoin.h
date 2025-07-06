#pragma once
#include "CItem.h"
class CCoin :
    public CItem
{
    enum STATE {UP,LEFT,DOWN,RIGHT,EFFECT,END};
public:
    CCoin();
    ~CCoin();


public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


public:
    virtual void OnCollisionEnter(CColider* _pOther);
    virtual void OnCollisionExit(CColider* _pOther);


private:
    void    Motion_Change();
    void    Change_State();

private:
    void                Launch();
private:
    float               m_fChangeTime;
    float               m_fChangeTimeMax;

    STATE               m_eCurState;
    STATE               m_ePreState;

    int                 m_iCount;
    COLORREF            m_cRemoveColor;

    bool                m_bChange;
    bool                m_bHit;


private:
    int                 m_iCoinValue;
    bool                m_bEnd;

    float               spreadX;
    float               LaunchY;

};

