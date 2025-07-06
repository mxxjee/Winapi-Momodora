#pragma once
class CSaveMgr
{


private:
	CSaveMgr();
	~CSaveMgr();


public:
	void	Initialize();
	void	Set_TargetIdx(int _iIdx) { m_CurIdx = _iIdx; }
	void	Save_Data();
	void	Load_FileName(int iIdx);	//슬롯에 표시할 파일이름
	wstring Get_LoadFileName(int iIdx) { return m_pFileNames[iIdx]; }
	void	Load_Data();		//각 슬롯이 가지고있는 세이브파일 불러오기.


public:
	SAVEDATA		Load_Game() { return m_pSaveDatas[m_CurIdx]; }

private:
	wstring			GetCurrentTimeWstring();


public:
	static CSaveMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSaveMgr;
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
	static CSaveMgr* m_pInstance;
	//map<wstring, 
	//
	int				m_CurIdx;		//현재 선택한 슬롯번호

	vector<SAVEDATA>	m_pSaveDatas;
	wstring				m_pFileNames[3];


};

