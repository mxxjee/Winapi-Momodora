#pragma once
#include "CActor.h"
class CGateWay :
    public CActor
{

public:
    CGateWay();
    ~CGateWay();



public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;



};

