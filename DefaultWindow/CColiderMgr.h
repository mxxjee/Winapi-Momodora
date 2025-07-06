#pragma once
class CColider;
#include "Define.h"

union COLIDER_ID
{
	struct
	{
		UINT Left_Id;
		UINT Right_Id;
	};
	ULONGLONG ID;
};
class CColiderMgr
{
private:
	CColiderMgr();
	~CColiderMgr();
	
private:
	map<ULONGLONG, bool> m_mapColInfo;		// 충돌체 간의 이전 프레임 충돌 정보
	UINT m_arrCheck[(UINT)OBJID::OBJ_END];		// 그룹간의 충돌 체크 매트릭스
public:
	void Update();
	void CheckGroup(OBJID _eLeft, OBJID _eRight);
	void Reset()
	{
		memset(m_arrCheck, 0, sizeof(UINT) * (UINT)OBJID::OBJ_END);
	}

private:
	void CollisionUpdateGroup(OBJID _eLeft, OBJID _eRight);
	bool IsCollision(CColider* _pLeftCol, CColider* _pRightCol);

public:
	static CColiderMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CColiderMgr;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CColiderMgr* m_pInstance;
};

