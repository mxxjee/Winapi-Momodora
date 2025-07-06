#include "pch.h"
#include "CSaveMgr.h"
#include "CStageManager.h"
#include "CObj.h"
#include "CCharacter.h"
#include "CInventoryMgr.h"


CSaveMgr* CSaveMgr::m_pInstance = nullptr;


CSaveMgr::CSaveMgr()
	:m_CurIdx(0)
{
	for (int i = 0; i < 3; ++i)
		m_pFileNames[i] = L"";

}

CSaveMgr::~CSaveMgr()
{
}

void CSaveMgr::Initialize()
{
	m_pSaveDatas.reserve(3);
	for (int i = 0; i < 3; ++i)
		m_pSaveDatas.push_back(SAVEDATA());

}

void CSaveMgr::Save_Data()
{
	//저장한 시각과 플레이어정보저장
	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Save/Save%d.dat", m_CurIdx + 1);

	//1.파일개방
	HANDLE hFile = CreateFile(szFilePath,
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	//2.파일 개방이 제대로 안됐다면
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte(0);
	SAVEDATA	pData;
	EVENTDATA	pEventData;

	pData.m_CurTime = GetCurrentTimeWstring();
	pData.m_CurScene = CStageManager::Get_Instance()->GetCurSceneID();

	CCharacter* pCharacter = dynamic_cast<CCharacter*>(CStageManager::Get_Instance()->GetPlayer());
	if (pCharacter)
		pData.m_iHp = pCharacter->Get_Hp();

	pData.m_iCoins = CInventoryMgr::Get_Instance()->Get_CoinCount();

	//문자열 길이부터저장
	DWORD dwLen = pData.m_CurTime.length();


	WriteFile(hFile, &(dwLen), sizeof(DWORD), &dwByte, nullptr);
	WriteFile(hFile, pData.m_CurTime.c_str(), sizeof(wchar_t) * dwLen, &dwByte, nullptr);

	WriteFile(hFile, &pData.m_CurScene, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, &pData.m_iHp, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, &pData.m_iCoins, sizeof(int), &dwByte, nullptr);

	//이벤트데이터 가져오고 저장,
	pEventData.m_bGetFlower = CStageManager::Get_Instance()->Get_GetFlower();
	pEventData.m_bKillMonsters = CStageManager::Get_Instance()->Get_KillAllMonsters();
	pEventData.m_bOpen4Stage = CStageManager::Get_Instance()->Get_m_bOpen4Stage();

	WriteFile(hFile, &pEventData.m_bGetFlower, sizeof(bool), &dwByte, nullptr);
	WriteFile(hFile, &pEventData.m_bKillMonsters, sizeof(bool), &dwByte, nullptr);
	WriteFile(hFile, &pEventData.m_bOpen4Stage, sizeof(bool), &dwByte, nullptr);

	//인벤 벡터 저장
	CInventoryMgr::Get_Instance()->Save(hFile, pData);

	CloseHandle(hFile);

}

void CSaveMgr::Load_FileName(int iIdx)
{

	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Save/Save%d.dat", iIdx + 1);

	//1.파일개방
	HANDLE hFile = CreateFile(szFilePath,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	//2.파일 개방이 제대로 안됐다면
	if (INVALID_HANDLE_VALUE == hFile)
		return;


	SAVEDATA	pData{};

	DWORD	dwByte(0);
	DWORD		dwLen = 0;



	//문자열 길이부터 읽어오기
	ReadFile(hFile, &dwLen, sizeof(DWORD), &dwByte, nullptr);
	pData.m_CurTime.resize(dwLen);



	ReadFile(hFile, &pData.m_CurTime[0], sizeof(wchar_t) * dwLen, &dwByte, nullptr);






	m_pFileNames[iIdx] = pData.m_CurTime;

	CloseHandle(hFile);
	
}

void CSaveMgr::Load_Data()
{
	//저장한 시각과 플레이어정보 읽어오기
	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Save/Save%d.dat", m_CurIdx +1);

	//1.파일개방
	HANDLE hFile = CreateFile(szFilePath,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	//2.파일 개방이 제대로 안됐다면
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	
	SAVEDATA	pData{};
	EVENTDATA	pEventData{};

	DWORD		dwLen=0;



	//문자열 길이부터 읽어오기
	ReadFile(hFile, &dwLen, sizeof(DWORD), &dwByte, nullptr);
	pData.m_CurTime.resize(dwLen);

	while (true)
	{
		ReadFile(hFile, &pData.m_CurTime[0], sizeof(wchar_t) * dwLen, &dwByte, nullptr);
		ReadFile(hFile, &pData.m_CurScene, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pData.m_iHp, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pData.m_iCoins, sizeof(int), &dwByte, nullptr);


		//이벤트데이터 가져오고 저장,

		ReadFile(hFile, &pEventData.m_bGetFlower, sizeof(bool), &dwByte, nullptr);
		ReadFile(hFile, &pEventData.m_bKillMonsters, sizeof(bool), &dwByte, nullptr);
		ReadFile(hFile, &pEventData.m_bOpen4Stage, sizeof(bool), &dwByte, nullptr);

		

		//인벤토리읽어오기.
		CInventoryMgr::Get_Instance()->Load(hFile, pData);

		if (dwByte == 0)
			break;


		m_pSaveDatas[m_CurIdx] = pData;

		CStageManager::Get_Instance()->Set_GetFlower(pEventData.m_bGetFlower);
		CStageManager::Get_Instance()->Set_KillAllMonsters(pEventData.m_bKillMonsters);
		CStageManager::Get_Instance()->Set_Open4Stage(pEventData.m_bOpen4Stage);


		CInventoryMgr::Get_Instance()->Set_CoinCount(pData.m_iCoins);
	}

	CloseHandle(hFile);


}

wstring CSaveMgr::GetCurrentTimeWstring()
{
	time_t now = time(nullptr);
	tm localTime;
	localtime_s(&localTime, &now); // 안전한 버전 사용

	std::wstringstream wss;
	wss << std::put_time(&localTime, L"%Y-%m-%d %H:%M:%S");

	return wss.str();
}
