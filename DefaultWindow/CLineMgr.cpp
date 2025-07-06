#include "pch.h"
#include "CLineMgr.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CTimeManager.h"
#include "CScrollMgr.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
	:m_pTarget(nullptr)
{
}

CLineMgr::~CLineMgr()
{
	Release();
}

bool CLineMgr::Collision_Line(CObj* pOther)
{

	float _fX = pOther->GetPos().x;
	float _fY = pOther->GetPos().y;
	float _fCY = pOther->GetScale().y;

	if (pOther->GetName() == L"Player")
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pOther);
		if (pPlayer)
		{
			CheckTrueResult(pPlayer->Get_IsJump(), false);
			CheckTrueResult(pPlayer->Get_IsDownJump(), false);
		}
	}

	if (m_LineList.empty())
		return false;

	//타겟 라인 위의 점으로 변경(직선의 방정식)
	float	MinDistance = 150.f;

	//1차로 x값 범위에 있는 선들을 걸러준다.
	for (CLine* pLine : m_LineList)
	{
		if (pLine->Get_Info().tLeft.fX <= _fX &&
			pLine->Get_Info().tRight.fX >= _fX)
		{

			float x1 = pLine->Get_Info().tLeft.fX;
			float y1 = pLine->Get_Info().tLeft.fY;
			float x2 = pLine->Get_Info().tRight.fX;
			float y2 = pLine->Get_Info().tRight.fY;


			float fValue = y1 + pLine->Get_Slop() * (_fX - x1);
			float OffSet = 0.f;

			if (pOther->Get_LineColOffSet())
				OffSet = 0.5f;

			//보정이후의 값으로 오차범위 비교
		/*	if (pOther->GetName() == L"Player")
			{
				if ((fValue - (_fCY) * 0.9f) - 10.f <= _fY && _fY <= (fValue - (_fCY) * 0.9f) + 10.f)
				{
					m_pTarget = pLine;

					_fY = fValue - (_fCY) *0.9f;
					pOther->SetPosY(_fY);
					return true;
				}
			}

			else
			{*/
				if ((fValue - (_fCY) *OffSet) - 10.f <= _fY && _fY <= (fValue - (_fCY) *OffSet) + 10.f)
				{
					m_pTarget = pLine;

					_fY = fValue - (_fCY) *OffSet;
					pOther->SetPosY(_fY);
					return true;
				}


			//}
		}


	}


	m_pTarget = nullptr;
	return false;
}

void CLineMgr::Load_Data(int iIdx)
{

	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Data/Line%d.dat", (iIdx -1));

	HANDLE	hFile = CreateFile(szFilePath,	// 파일 이름이 명시된 경로
		GENERIC_READ,		// 파일 접근 모드(GENERIC_WRITE : 쓰기, GENERIC_READ : 읽기)
		NULL,				// 공유 방식(파일이 열려있는 상태에서 다른 프로세스에서 접근할 때 허용할 것인가)
		NULL,				// 보안 속성(NULL 기본 보안 속성)
		OPEN_EXISTING,		// 생성 방식(쓰기 전용 : 파일이 없다면 생성, 있으면 덮어 쓰기, 읽기 전용(OPEN_EXISTING : 파일이 있는 경우면 불러오기))
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성 : 아무런 속성이 없는 일반 파일 형식
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일(NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		//MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	Release();

	DWORD		dwByte(0);
	LINE		tLine{};

	while (true)
	{
		bool b=ReadFile(hFile, &tLine, sizeof(LINE), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		m_LineList.push_back(new CLine(tLine.tLeft, tLine.tRight));
	}

	CloseHandle(hFile);

}
bool CLineMgr::Collision_Ladder(float _fX, float _fY, int _iDir, float* _fOffsetY)
{
	for (auto& pLine : m_LineList)
	{
		if (_iDir > 0)
		{
			if (pLine->Get_Info().tRight.fX < _fX + 80.f
				&& pLine->Get_Info().tRight.fX > _fX - 80.f)
			{
				float x1 = pLine->Get_Info().tLeft.fX;
				float y1 = pLine->Get_Info().tLeft.fY;
				float x2 = pLine->Get_Info().tRight.fX;
				float y2 = pLine->Get_Info().tRight.fY;

				float minY = min(y1, y2);
				float maxY = max(y1, y2);

				if (minY > _fY
					|| maxY < _fY)
					return false;

				if (x2 - x1 == 0.f)
				{
					*_fOffsetY = x1;
					return true;
				}

				float gradient = (y2 - y1) / (x2 - x1);
				if ((abs(gradient) >= 20.f || abs(gradient) <= -20.f))
				{
					*_fOffsetY = x1;
					return true;
				}
			}
		}
		else
		{
			if (pLine->Get_Info().tLeft.fX > _fX - 30.f
				&& pLine->Get_Info().tLeft.fX < _fX + 30.f)
			{
				float x1 = pLine->Get_Info().tLeft.fX;
				float y1 = pLine->Get_Info().tLeft.fY;
				float x2 = pLine->Get_Info().tRight.fX;
				float y2 = pLine->Get_Info().tRight.fY;

				float minY = min(y1, y2);
				float maxY = max(y1, y2);

				if (minY > _fY
					|| maxY < _fY)
					return false;

				if (x2 - x1 == 0.f)
				{
					*_fOffsetY = x1;
					return true;
				}

				float gradient = (y2 - y1) / (x2 - x1);
				if ((abs(gradient) >= 5.f || abs(gradient) <= -5.f))
				{
					*_fOffsetY = x1;
					return true;
				}
			}
		}
	}

	return false;
}

void CLineMgr::Initialize()
{

}

void CLineMgr::Render(HDC hDC)
{
	if (!bColRender)
		return;

	for (auto& pLine : m_LineList)
		pLine->Render(hDC);

}

void CLineMgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), Safe_Delete<CLine*>);
	m_LineList.clear();
}
