#pragma once
#include "CUI.h"
class CDialogue :
    public CUI
{
public:
    CDialogue();
    ~CDialogue();


public:
    // CUI을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


public:
    virtual void        Play();
    void    Stop();
    virtual bool    Get_Play() { return m_bPlay; }

private:
    void    Key_Input();

private:
    bool        m_bPlay;
};

