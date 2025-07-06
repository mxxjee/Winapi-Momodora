#pragma once

#include "CObj.h"
#include "CStageManager.h"
#include "CUITrigger.h"

class CPortal;
class CM_BigMonk;
class CM_POISION;
class CM_POISION;
class CM_SHIELD;



template<typename T>
class CAbstractFactory
{
public:
	static CObj* Create_Obj()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create_Obj(float fX, float fY , DIRECTION eDir = END)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(fX, fY);
		pObj->Set_Dir(eDir);

		return pObj;
	}

	static CObj* Create_Obj(float fX, float fY, OBJID _eType)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(fX, fY);
		CStageManager::Get_Instance()->GetCurScene()->AddObject(pObj, _eType);

		return pObj;
	}
	
	
public:

	static CObj* Create_Monster(float fX, float fY)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(Vec2(fX,fY));
		CStageManager::Get_Instance()->GetCurScene()->AddObject(pObj, OBJID::OBJ_MONSTER);

		return pObj;
	}

	static CObj* Create_Portal(float fX, float fY, STAGE_TYPE _Next)
	{
		CObj* Portal = new CPortal;

		Portal->Initialize();
		Portal->SetPos(Vec2(fX, fY));
		Portal->SetName(L"Portal");
		static_cast<CPortal*>(Portal)->SetNextScene(_Next);
		CStageManager::Get_Instance()->GetCurScene()->AddObject(Portal, OBJID::OBJ_PORTAL);

		return Portal;
	}

	static CObj* Create_UITrigger(float fX, float fY, PANNEL_TYPE _Type)
	{
		CObj* pObj = new T;
		CUITrigger* pTrigger = dynamic_cast<CUITrigger*>(pObj);
		if (pTrigger)
			pTrigger->Set_Type(_Type);

		pObj->Initialize();
		pObj->SetPos(Vec2(fX, fY));
		CStageManager::Get_Instance()->GetCurScene()->AddObject(pObj, OBJID::OBJ_TRIGGER);

		return pObj;
	}

	
};

