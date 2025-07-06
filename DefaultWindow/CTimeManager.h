#pragma once
class CTimeManager
{
private:
	CTimeManager();
	~CTimeManager();

private:
	LARGE_INTEGER m_llPrevCount;
	LARGE_INTEGER m_llFrequency;
	LARGE_INTEGER m_llCurCount;
	// FPS
	// 1 프레임당 시간 Delta Time
	double m_dDT; // 프레임 사이의 시간값 
	double m_dAcc; // 1초 체크를 위한 누적 시간
	UINT  m_iCallCount;  // 함수 호출 횟수 체크
	UINT m_iFPS; // 초당 호출 횟수

public:
	void Initialize();
	void Update();
	void Render();
public:
	double GetDT() { return m_dDT; }
	float GetfDT() { return (float)m_dDT; }

public:
	static CTimeManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTimeManager;
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
	static CTimeManager* m_pInstance;
};

