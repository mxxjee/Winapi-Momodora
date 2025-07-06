#pragma once
#include "CUI.h"
class CNumber :
    public CUI
{
public:
    CNumber();
    ~CNumber();


    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    void    Set_Number();

    void    One();
    void    Two();
    void    Three();

 

public:
    void    Set_Idx(int i) { m_iIdx = i; }
    void    Set_Value(int i) { m_iValue = i; }

private:
    int         m_iIdx;
    int         m_iValue;
};

