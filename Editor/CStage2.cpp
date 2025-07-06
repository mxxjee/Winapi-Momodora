#include "pch.h"
#include "CStage2.h"
#include "CBmpMgr.h"
#include "CObj.h"
#include "CScrollMgr.h"
#include "CStageManager.h"

CStage2::CStage2()
{

}

CStage2::~CStage2()
{

}

void CStage2::Update()
{
	CStage::Update();
}

void CStage2::Render(HDC _dc)
{
	HDC	MapDC = CBmpMgr::Get_Instance()->Find_Image(L"Map2");
	
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();



	GdiTransparentBlt(_dc,
		iScrollX,
		iScrollY,
		(int)1773,
		(int)1464,
		MapDC,
		0,
		0,
		(int)1773,	// 복사할 비트맵 가로 세로 사이즈
		(int)1464,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값

	CStage::Render(_dc);
}

void CStage2::Enter()
{
    // StageManager로부터 기존 Player 가져오기
    CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();

    if (pPlayer)
    {
        AddObject(pPlayer, OBJID::OBJ_PLAYER);  // 오브젝트 리스트에 다시 추가
        RegisterPlayer(pPlayer);               // 충돌 및 중심 참조 등록
    }

	Initialize();
}

void CStage2::Exit()
{
	DeleteAll();

}
