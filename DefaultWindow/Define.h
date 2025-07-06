#pragma once

#define	WINCX 800
#define	WINCY 600

#define PURE = 0
#define NOEVENT 0
#define DEAD    1 
#define PI		3.141592f
#define VK_MAX		0xff
#define GRAVITY 980.f

#define MAXHP 100

#define fDT  CTimeManager::Get_Instance()->GetfDT()
#define DT CTimeManager::Get_Instance()->GetDT()

#define SAFE_DELETE(p) {if(p) delete p; p=nullptr;}

#define CheckTrue(x) { if(x == true) return; }

#define CheckFalse(x) { if(x == false) return; }

#define CheckNull(x) { if(x == nullptr) return; }

#define CheckTrueResult(x, y) { if(x == true) return y; }

#define CheckFalseResult(x, y) { if(x == false) return y;}

#define CheckNullResult(x, y) { if(x == nullptr) return y;}

#define PlaySoundEffect(str,volume) CSoundMgr::Get_Instance()->PlaySoundW(str,SOUND_EFFECT,volume);

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

enum class UI_TYPE
{
	INVENTORY,
	SETTINGS,
	END
};

enum class ITEM_TYPE
{
	ACTIVE,
	CONTINUE,
	END
};

enum class HPBAR_TYPE
{
	BACKGROUND,
	DAMAGE,
	CURRENT,
	END
};

enum class INTERACTIVE_TYPE
{
	CHEST,
	CRYSTAL,
	JUMP_PAD,
	ELEVATOR,
	END
};


enum OBJID
{
	OBJ_INTERACTIVE,
	OBJ_ITEM,
	OBJ_WALL,
	OBJ_COIN,
	OBJ_NPC,
	OBJ_PLAYER,
	OBJ_GROUND,
	OBJ_MONSTER,
	OBJ_WEAPON,
	OBJ_ARROW,
	OBJ_MONSTERWEAPON,
	OBJ_SHIELD,
	OBJ_PORTAL,
	OBJ_BALL,
	OBJ_TRIGGER,
	OBJ_UI,
	OBJ_LAPPED,
	OBJ_END
};

enum class PORTAL_TYPE
{
	STANDARD,
	UP,
	END
};
enum class STAGE_TYPE
{
	LOGO,
	MENU,
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
	DEATHMENU,
	SELECTMENU,
	ENDSCENE,
	END,


};

enum class PANNEL_TYPE
{
	ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,END
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

typedef struct tagFrame
{
	int		iPosX;
	int		iPosY;
	int		iSizeX;
	int		iSizeY;
	int		iFrameStart;
	int		iFrameEnd;
	int		iMotion;
	DWORD	dwSpeed;
	DWORD	dwTime;

public:
	tagFrame() { ZeroMemory(this, sizeof(tagFrame)); }
	tagFrame(int _iPosX, int _iPosY,
		int _iSizeX, int _iSizeY,
		int _iFrameStart, int _iFrameEnd, int _iMotion,
		DWORD _dwSpeed, DWORD _dwTime)
		:iPosX(_iPosX),iPosY(_iPosY),iSizeX(_iSizeX),iSizeY(_iSizeY),
		iFrameStart(_iFrameStart),iFrameEnd(_iFrameEnd),iMotion(_iMotion),
		dwSpeed(_dwSpeed),dwTime(_dwTime)
	{

	}
}FRAME;


//Minval에서 maxval까지로 보정
template<typename T>
T clamp(T value, T minVal, T maxVal)
{
	if (value < minVal)
		return minVal;

	else if (value > maxVal)
		return maxVal;

	return value;

}


typedef struct tagCombo
{
	int		m_iCombo;
	int		m_iMaxCombo;

	float	m_fComboMaxTime;		//콤보 유지시간, 이 시간 이후 초기화


	void	Reset_Combo()
	{
		m_iCombo = 0;


	}

	

}COMBOINFO;

struct ATTACKINFO
{
	float m_fDamage;
	float m_fDelay;
	float m_dtAttackTime;
};


enum CHANNELID { SOUND_EFFECT, SOUND_EFFECT2,SOUND_EFFECT3, SOUND_EFFECT4, 
	SOUND_EFFECT5,
	SOUND_EFFECT6,
	SOUND_MOVINGTILE,
	SOUND_CHARGE,
	SOUND_JUMP,
	SOUND_UI,
	SOUND_BGM, MAXCHANNEL };




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
extern bool bColRender;
extern float g_fVolume;
extern bool		bNotDead;