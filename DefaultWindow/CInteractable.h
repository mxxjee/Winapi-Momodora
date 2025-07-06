#pragma once
#include "CActor.h"

class CInteractable :
    public CActor
{
public:
    CInteractable();
    ~CInteractable();

public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

public:
    INTERACTIVE_TYPE        Get_Type() const { return m_eInterType; }
    void                    Set_Type(INTERACTIVE_TYPE type) { m_eInterType = type; }
protected:
    const TCHAR*        m_pFrameKey;
    INTERACTIVE_TYPE    m_eInterType;
   
};

