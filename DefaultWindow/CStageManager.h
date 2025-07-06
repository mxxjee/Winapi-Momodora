#pragma once
class CStage;
class CObj;


class CStageManager
{
private:
    CStageManager();
    ~CStageManager();
private:
    CStage* m_arrStage[(UINT)STAGE_TYPE::END]; // 모든 씬 목록
    CStage* m_pCurScene; // 현재 씬
    CObj* m_pPlayer; // 플레이어 공유 포인터 추가
    vector<CObj*>* m_pWeapon;    //플레이어 웨폰 공유포인터

public:
    void Initialize();
    void Update();
    void Render(HDC _dc);

public:
    void    Delete_Player();

public:
    CStage* GetCurScene() { return m_pCurScene; }
    STAGE_TYPE  GetCurSceneID() { return m_CurSceneNum; }

    void SetPlayer(CObj* _pPlayer) { m_pPlayer = _pPlayer; }
    void SetSlash(vector<CObj*>* _pWeapon) { m_pWeapon = _pWeapon; }

    CObj* GetPlayer() { return m_pPlayer; }
    vector<CObj*>* GetSlashes() { return m_pWeapon; }

    void        SetChangeScene(bool bChange, STAGE_TYPE NextSceneNum) { m_bChange = bChange; m_NextSceneNum = NextSceneNum; }



public:
    void ChangeScene(STAGE_TYPE _eNext);
    void Restart();
    
public:
    void    Reset_EventTrigger();

    void    Set_GetFlower(bool b) { m_pEventData.m_bGetFlower = b; }
    void    Set_Enter(bool b) { m_bEnter = b; }
    void    Set_Open4Stage(bool b) { m_pEventData.m_bOpen4Stage = b; }
    void    Reset_Alpha() { m_FadeOutAlpha = 0.f; m_FadeInAlpha = 255.f; }
    void    Set_EnterNewScene(bool b) { m_bEnterNewScene = b; }

    void    Set_EnterBossStage(bool b) { m_bEnterBossStage = b; }
    bool    Get_EnterBossStage() { return m_bEnterBossStage; }

    void    Set_KillAllMonsters(bool b) { m_pEventData.m_bKillMonsters = b; }
    bool    Get_KillAllMonsters() { return m_pEventData.m_bKillMonsters;}
    bool    Get_m_bOpen4Stage() { return m_pEventData.m_bOpen4Stage; }
    bool    Get_EnterNewScene() { return m_bEnterNewScene; }

    void    Set_TalkEnd(bool b) { m_bTalkEnd = b; }
    bool    Get_TalkEnd() { return m_bTalkEnd; }

public:
    bool    Get_GetFlower() { return m_pEventData.m_bGetFlower;}
    bool    Get_Enter() { return m_bEnter; }


public:
    bool    Get_SelectMenu() { return m_bSelectMenu; }
    void    Set_SelectMenu(bool _b) { m_bSelectMenu = _b; }


public:
    void    Set_PlayBGM(bool _b) { m_bPlayBGM = _b; }
    bool    Get_PlayBGM() { return m_bPlayBGM; }
public:
    bool        FadeOut(HDC hDC); // 페이드아웃이 모두 되었다면 true, 아직 진행중이라면 false반환
    bool         FadeIn(HDC hDC);


public:
    void        Set_KillBoss(bool b) { m_bKillBoss = b; }
    bool        Get_KillBoss() { return m_bKillBoss; }

public:
    void    Set_FadeDC(const TCHAR* _Key);
    void    Set_FadeSpeed(float _f) { m_fFadeSpeed = _f; }

public:
    static CStageManager* Get_Instance()
    {
        if (!m_pInstance)
        {
            m_pInstance = new CStageManager;
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
    static CStageManager* m_pInstance;
    STAGE_TYPE           m_CurSceneNum;      //현재 씬 번호(ENUM)
    
    bool                 m_bChange;
    STAGE_TYPE          m_NextSceneNum;


private:
    /*Event관련 변수*/
    EVENTDATA       m_pEventData;

private:
    bool            m_bEnterBossStage;
    bool            m_bSelectMenu;


private:
    bool            m_bEnter;           //씬 진입 시 true, 나가면 false
    bool            m_bEnterNewScene;
   
private:
    HDC             m_FadeDC;

    float           m_FadeOutAlpha;
    float           m_FadeInAlpha;

    bool             FadeOutResult;
    bool            FadeInResult;

    float           m_fFadeSpeed = 1000;



private:
    bool            m_bShowUI;
    bool            m_bPlayBGM;


    bool            m_bTalkEnd=false;//돈 지불하면 true.
    bool            m_bKillBoss;


};

