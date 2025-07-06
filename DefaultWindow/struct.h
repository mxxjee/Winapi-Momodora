#pragma once

struct Vec2
{
	float x, y;
public:
	static Vec2 Up() { return { 0.f, -1.f }; }
	static Vec2 Down() { return { 0.f, 1.f }; }
	static Vec2 Left() { return { -1.f, 0.f }; }
	static Vec2 Right() { return { 1.f, 0.f }; }
	static Vec2 Zero() { return { 0.f, 0.f }; }
	static float Dot(Vec2 _Tmp, Vec2 _Src) { return ((_Tmp.x * _Src.x) + (_Tmp.y * _Src.y)); }
	float Length()
	{
		return sqrt(x * x + y * y);
	}
	Vec2& Normalize()
	{
		float fLen = Length();

		if (fLen == 0.f)
			return *this;

		x /= fLen;
		y /= fLen;

		return *this;
	}

	static float Distance(const Vec2& _a, const Vec2& _b)
	{
		float dx = _a.x - _b.x;
		float dy = _a.y - _b.y;
		return sqrt((dx * dx) + (dy * dy));
	}

public:
	void operator -=(float other)
	{
		x -= other;
		y -= other;
	}
	void operator -=(Vec2 other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator +=(Vec2 other)
	{
		x += other.x;
		y += other.y;
	}
	void operator =(Vec2 other)
	{
		x = other.x;
		y = other.y;
	}
	Vec2 operator +(Vec2 other) const
	{
		return Vec2(x + other.x, y + other.y);
	}
	Vec2 operator +(float other) const
	{
		return Vec2(x + other, y + other);
	}
	Vec2 operator -(Vec2 other) const
	{
		return Vec2(x - other.x, y - other.y);
	}
	Vec2 operator *(Vec2 other)  const
	{
		return Vec2(x * other.x, y * other.y);
	}
	Vec2 operator *(float f) const
	{
		return Vec2(x * f, y * f);
	}

	Vec2 operator / (float f) const
	{
		return Vec2(x / f, y / f);
	}
	Vec2& operator*=(float f) {
		x *= f;
		y *= f;
		return *this;
	}
	Vec2 operator / (Vec2 other)
	{
		//0나누기 방지
		if (other.x == 0.f && other.y == 0.f)
			return Vec2(x / other.x, y / other.y);
	}

	bool operator==(Vec2 other)
	{
		return (other.x == x && other.y == y);
	}

	

public:
	Vec2()
		:x(0.f)
		, y(0.f)
	{
	}
	Vec2(float _x, float _y)
		:x(_x)
		, y(_y)
	{
	}
	Vec2(int _x, int _y)	//explicit으로 암묵적 형변환 방지
		:x((float)_x)
		, y((float)_y)
	{
	}
	Vec2(UINT _x, UINT _y)	//explicit으로 암묵적 형변환 방지
		:x((float)_x)
		, y((float)_y)
	{
	}
};

class CObj;
typedef struct tagItemInfo
{
	TCHAR* m_pItemName;
	ITEM_TYPE m_eType;
	int m_iHp;
	CObj* m_pOwner;

	tagItemInfo()
		: m_eType(ITEM_TYPE::END), m_iHp(0), m_pOwner(nullptr)
	{
		m_pItemName = new TCHAR[64];
		ZeroMemory(m_pItemName, sizeof(TCHAR) * 64);
	}

	tagItemInfo(const TCHAR* _pItemName, ITEM_TYPE _eType, int _iHp,CObj* _Owner)
		: m_eType(_eType), m_iHp(_iHp), m_pOwner(_Owner)
	{
		m_pItemName = new TCHAR[64];
		lstrcpy(m_pItemName, _pItemName);
	}

	~tagItemInfo()
	{
		delete[] m_pItemName;
		m_pItemName = nullptr;
		m_pOwner = nullptr;
	}

	// 깊은 복사 생성자
	tagItemInfo(const tagItemInfo& other)
		: m_eType(other.m_eType), m_iHp(other.m_iHp),m_pOwner(other.m_pOwner)
	{
		m_pItemName = new TCHAR[64];
		lstrcpy(m_pItemName, other.m_pItemName);
	}

	
	tagItemInfo& operator=(const tagItemInfo& other)
	{
		if (this == &other)
			return *this;

		if (m_pItemName)
		{
			delete[] m_pItemName;
			m_pItemName = nullptr;

		}
		
		m_pItemName = new TCHAR[64];
		lstrcpy(m_pItemName, other.m_pItemName);
		m_iHp = other.m_iHp;
		m_eType = other.m_eType;
		m_pOwner = other.m_pOwner;

		return *this;
	}

	void	clear()
	{

		if (m_pItemName)
		{
			delete[] m_pItemName;
			m_pItemName = nullptr;
		}
		m_iHp = 0;
		m_eType = ITEM_TYPE::END;
		m_pOwner = nullptr;

	}


}ITMINFO;


typedef struct tagStateTime
{
	float		m_fTime;
	float		m_fTimeMax;


	tagStateTime()
		:m_fTime(0.f), m_fTimeMax(0.f)
	{

	}

	tagStateTime(float _fTime, float _fTimeMax)
		:m_fTime(_fTime), m_fTimeMax(_fTimeMax)
	{

	}


}STATETIME;

typedef struct tagSaveData
{
	wstring		m_CurTime;		//현재저장시간
	STAGE_TYPE	m_CurScene;		//저장한 위치
	int			m_iHp;			//플레이어 체력
	int			m_iCoins;

	vector<ITMINFO>	m_pAddItems;//아이템리스트에 추가
	vector<ITMINFO> m_pEquipItems;

public:
	tagSaveData()
		:m_CurTime(L""), m_CurScene(STAGE_TYPE::END), m_iHp(0), m_iCoins(0)
	{

	}

	bool Empty()
	{
		return (m_CurTime == L"") && (m_iHp == 0) && (m_CurScene == (STAGE_TYPE::END));

	}
}SAVEDATA;


typedef struct tagEventData
{
	bool            m_bGetFlower;
	bool            m_bKillMonsters;//2스테이지 한정
	bool            m_bOpen4Stage;  //4스테이지 한정

	tagEventData()
		:m_bGetFlower(false), m_bKillMonsters(false), m_bOpen4Stage(false)
	{

	}

}EVENTDATA;