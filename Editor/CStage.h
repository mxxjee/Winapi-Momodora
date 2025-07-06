#pragma once
class CObj;
class CStage
{
protected:
	list<CObj*> m_arrObj[(UINT)OBJID::OBJ_END]; // 오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언
	wstring m_strName; // Scene이름

	CObj* m_pPlayer; // Player

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	CObj* GetPlayer() { return m_pPlayer; }
	void RegisterPlayer(CObj* _pPlayer) { m_pPlayer = _pPlayer; }

	virtual void Initialize();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render(HDC _dc);

	virtual void Enter() = 0; // 해당 Scene에 진입 시 호출
	virtual void Exit() = 0; // 해당 Scene을 탈출 시 호출



public:
	void AddObject(CObj* _pObj, OBJID _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}
	const list<CObj*>& GetGroupObject(OBJID _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(OBJID _eTarget);
	void DeleteAll();



public:
	CStage();
	virtual ~CStage();


protected:
	const TCHAR* m_pImgKey;
	HDC MapDC;
	BITMAP	MapDCBM;
};

