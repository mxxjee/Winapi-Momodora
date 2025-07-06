#pragma once

#define	WINCX 800
#define	WINCY 600

#define PURE = 0
#define NOEVENT 0
#define DEAD    1 
#define PI		3.141592f
#define VK_MAX		0xff
#define GRAVITY 980.f

#define fDT CTimeManager::Get_Instance()->GetfDT()
#define DT CTimeManager::Get_Instance()->GetDT()

#define TILECX		30
#define TILECY		30

#define TILEX		64
#define TILEY		42

enum DIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	LUP,
	RUP,
	END
};

enum OBJID
{
	OBJ_PLAYER,
	OBJ_BULLET,
	OBJ_MONSTER,
	OBJ_MOUSE,
	OBJ_SHIELD,
	OBJ_END
};

enum class STAGE_TYPE
{
	STAGE1,
	STAGE2,
	STAGE2_UNDER,
	STAGE3,
	STAGE4,
	STAGE5,
	STAGE6,
	STAGE7,
	STAGE8,
	STAGE9,
	STAGE9_LEFT,
	STAGE9_RIGHT,
	STAGE10,
	STAGE11,
	STAGE12,
	END,


};

enum class INTERACTIVE_TYPE
{
	CHEST,
	CRYSTAL,
	JUMP_PAD,
	ELEVATOR,
	END
};


typedef struct tagInfo
{
	float	fX;		// 중점 X,Y
	float	fY;
	float	fCX;	// 가로, 세로 길이
	float	fCY;	

}INFO;

typedef struct tagPoint
{
	float	fX;
	float	fY;

	tagPoint() 
	{
		ZeroMemory(this, sizeof(tagPoint));
	}
	tagPoint(float _fX ,float _fY) : fX(_fX), fY(_fY){}

	void clear()
	{
		ZeroMemory(this, sizeof(tagPoint));
	}

}LINEPOINT;

typedef struct tagLine
{
	LINEPOINT	tLeft;
	LINEPOINT	tRight;


	tagLine()
	{
		ZeroMemory(this, sizeof(tagLine));
	}
	tagLine(LINEPOINT _tLeft, LINEPOINT _tRight) : tLeft(_tLeft), tRight(_tRight) {}


}LINE;


template<typename T>
void Safe_Delete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

template<typename T>
void Safe_Delete_List(std::list<T*>& _list)
{
	for (T* ptr : _list)
	{
		if (ptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}
	_list.clear();
}


struct tagFinder
{
	tagFinder(const TCHAR* pString) : m_pString(pString) {}

	template<typename T>
	bool operator()(T& MyPair)
	{
		return !lstrcmp(MyPair.first, m_pString);
	}

	const TCHAR* m_pString;
};



struct DeleteObj
{	
	template<typename T>
	void operator()(T& p)
	{
		if (p)
		{
			delete p;
			p = nullptr;
		}
	}
};

struct DeleteMap
{
	template<typename T>
	void operator()(T& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	}
};


extern HWND g_hWnd;
