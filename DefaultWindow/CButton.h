#pragma once
#include "CUI.h"
class CButton :
    public CUI
{
public:
    CButton();
    ~CButton();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

};

