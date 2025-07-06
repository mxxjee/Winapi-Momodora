#include "pch.h"
#include "CColiderMgr.h"
#include "CStageManager.h"
#include "CStage.h"
#include "CObj.h"
#include "CColider.h"

CColiderMgr* CColiderMgr::m_pInstance = nullptr;

CColiderMgr::CColiderMgr() : m_arrCheck{}
{

}

CColiderMgr::~CColiderMgr()
{
}

void CColiderMgr::Update()
{
	for (UINT iRow = 0; iRow < (UINT)OBJID::OBJ_END; ++iRow)
	{
		m_arrCheck[iRow]; 
		for (UINT iCol = iRow; iCol < (UINT)OBJID::OBJ_END; ++iCol) // iRow 그룹과 충돌 체크할 iCol 그룹
		{
            //비트마스크를 통해 1이 나옴->둘이 충돌할그룹임.
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionUpdateGroup((OBJID)iRow, (OBJID)iCol); // 해당 그룹끼리 충돌 검사
			}
		}
	}
}

// 두 그룹 간의 충돌 처리
void CColiderMgr::CollisionUpdateGroup(OBJID _eLeft, OBJID _eRight)
{
    
    // 현재 스테이지에서 두 그룹의 오브젝트 리스트 가져오기
    CStage* pCurStage = CStageManager::Get_Instance()->GetCurScene();
    const list<CObj*>& listLeft = pCurStage->GetGroupObject(_eLeft);
    const list<CObj*>& listRight = pCurStage->GetGroupObject(_eRight);
    map<ULONGLONG, bool>::iterator iter;

    // 왼쪽 그룹의 모든 오브젝트 순회
    for (auto leftObj : listLeft)
    {
        if (nullptr == leftObj || nullptr == leftObj->GetColider()|| listRight.empty())
            continue;

        for (auto rightObj : listRight)
        {
            // 오른쪽 그룹의 모든 오브젝트 순회
            if (listRight.empty())
                break;

            if (nullptr == rightObj || nullptr == rightObj->GetColider())
                continue;

            // 자기 자신과의 충돌은 무시
            if (leftObj == rightObj)
                continue;

            CColider* pLeftCol = leftObj->GetColider();
            CColider* pRightCol = rightObj->GetColider();

            // 고유 충돌 ID 생성
            COLIDER_ID ID;
            ID.Left_Id = pLeftCol->GetID();
            ID.Right_Id = pRightCol->GetID();

            iter = m_mapColInfo.find(ID.ID);

            // 처음 충돌 체크하는 쌍이면 false로 초기화
            if (iter == m_mapColInfo.end())
            {
                m_mapColInfo.insert(make_pair(ID.ID, false));
                iter = m_mapColInfo.find(ID.ID);
            }
            // 충돌 발생 여부 확인
            if (IsCollision(pLeftCol, pRightCol))
            {
                if (iter->second)
                {
                    // 이전 프레임에서도 충돌 중이었고, 지금도 충돌 중
                    if (leftObj->Get_Dead() || rightObj->Get_Dead())
                    {
                        // 객체가 삭제 상태이면 충돌 종료 처리
                        pLeftCol->OnCollisionExit(pRightCol);
                        pRightCol->OnCollisionExit(pLeftCol);
                        iter->second = false;
                    }
                    else
                    {
                        // 계속 충돌 중
                        pLeftCol->OnCollision(pRightCol);
                        pRightCol->OnCollision(pLeftCol);
                    }
                }
                else
                {
                    // 이전 프레임에는 충돌 안 했고, 이번 프레임부터 충돌 시작
                    if (!leftObj->Get_Dead() && !rightObj->Get_Dead())
                    {
                        pLeftCol->OnCollisionEnter(pRightCol);
                        pRightCol->OnCollisionEnter(pLeftCol);
                        iter->second = true;
                    }
                }
            }
            else
            {
                // 이번 프레임 충돌이 아니고, 이전 프레임에는 충돌했었다면 종료 처리
                if (iter->second)
                {
                    pLeftCol->OnCollisionExit(pRightCol);
                    pRightCol->OnCollisionExit(pLeftCol);
                    iter->second = false;
                }
            }
        }
    }
}


// 실제 충돌 여부를 체크
bool CColiderMgr::IsCollision(CColider* _pLeftCol, CColider* _pRightCol)
{
    if (!(_pLeftCol->IsActive()) || !(_pRightCol->IsActive()))
        return false;

	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}
	return false;
}

// 두 그룹 간의 충돌 체크 설정
void CColiderMgr::CheckGroup(OBJID _eLeft, OBJID _eRight)
{

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

    // 항상 iRow가 작고 iCol이 크도록 정렬
	if (iCol < iRow)
	{
        // 체크 해제
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

    // 충돌 체크 여부 설정
	if (m_arrCheck[iRow] & (1 << iCol))
	{
        // 체크 해제
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
        // 체크 설정
		m_arrCheck[iRow] |= (1 << iCol);
	}
}
