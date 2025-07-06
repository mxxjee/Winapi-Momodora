#pragma once
#include "CUI.h"

class CNumber;

class CoinCount :
    public CUI
{

public:
    CoinCount();
    ~CoinCount();


    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    CNumber* m_pNumbers[3];

};

