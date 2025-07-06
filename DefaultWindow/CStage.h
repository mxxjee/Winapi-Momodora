#pragma once
class CObj;
class CWeapon;
class CStage
{
protected:
	list<CObj*> m_arrObj[(UINT)OBJID::OBJ_END]; // 오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언
	wstring m_strName; // Scene이름

	CObj* m_pPlayer;		// Player
	vector<CObj*>* m_pWeapon;		//Player's Weapon;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	CObj* GetPlayer() { return m_pPlayer; }
	void SetPlayer(CObj* pPlayer) { m_pPlayer = pPlayer; }
	vector<CObj*>* GetWeapon() { return m_pWeapon; }

	void RegisterPlayer(CObj* _pPlayer) { m_pPlayer = _pPlayer; }
	void RegisterWeapon(vector<CObj*>* _pWeapon) { m_pWeapon = _pWeapon; }

	virtual void Initialize();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render(HDC _dc);

	virtual void Enter(); // 해당 Scene에 진입 시 호출
	virtual void Exit() = 0; // 해당 Scene을 탈출 시 호출

	virtual void	Render_Map(HDC hdc, int iScrollX, int iScrollY);

public:
	void		Load_Datas(STAGE_TYPE eType,bool bLoadLine=true);
public:
	void AddObject(CObj* _pObj, OBJID _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}

	void	Set_DefaultCollision();
	list<CObj*>* GetList(OBJID _eType) { return &m_arrObj[_eType]; }

	const list<CObj*>& GetGroupObject(OBJID _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(OBJID _eTarget);
	void DeletePlayer();
	void DeleteAll();

protected:
	void		SetMaxScroll(const TCHAR* pImgKey);


public:
	virtual void	EventTrigger() {};
public:
	CStage();
	virtual ~CStage();

protected:
	bool	m_bFade;
	HDC MapDC;
	const TCHAR* m_pImgKey;
	BITMAP	MapDCBM;
};

