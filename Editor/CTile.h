#pragma once

#include "CObj.h"


class CTile
	:public CObj
{
public:
	CTile();
	~CTile();


public:
    void    Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


};

