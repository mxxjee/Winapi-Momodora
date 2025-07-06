#include "pch.h"
#include "CBmpMgr.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}

HDC CBmpMgr::Find_Image(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

HBITMAP CBmpMgr::Get_HBitmap(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_BitMap();
}

BITMAP CBmpMgr::Get_Bitmap(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
		return BITMAP{};

	else
	{
		BITMAP bmpInfo = {};

		HBITMAP map = iter->second->Get_BitMap();
		GetObject(map, sizeof(bmpInfo), &bmpInfo);
		return bmpInfo;

	}
}

void CBmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
	{
		CMyBmp* pMyBmp = new CMyBmp;
		pMyBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({ pImgKey, pMyBmp });
	}

}

void CBmpMgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), DeleteMap());
	m_mapBit.clear();

}

void CBmpMgr::Load()
{
	Load_Background();
	Load_Character();
	Load_Weapon();
	Load_Object();
	Load_Effect();
	Load_UI();
}

void CBmpMgr::Load_Background()
{
	Insert_Bmp(L"../Image/Back.bmp", L"Back");
	Insert_Bmp(L"../Image/Ground.bmp", L"Ground");



	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/1.bmp", L"1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/2.bmp", L"2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/2_Under.bmp", L"2_Under");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/3.bmp", L"3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/4.bmp", L"4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/5.bmp", L"5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/6.bmp", L"6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/7.bmp", L"7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/8.bmp", L"8");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/9.bmp", L"9");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/9_Left.bmp", L"9_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/9_Right.bmp", L"9_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/10.bmp", L"10");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/11.bmp", L"11");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/12.bmp", L"12");

	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/9_Right_Parts1.bmp", L"Parts1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/9_Right_Parts2.bmp", L"Parts2");


	Insert_Bmp(L"../Image/Portal/Portal.bmp", L"Portal");

	//Logo
	Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");

	//MainMenu BackGround
	Insert_Bmp(L"../Image/Menu/Menu.bmp", L"Menu");
	Insert_Bmp(L"../Image/Menu/TitleKaho.bmp", L"TitleKaho");

	Insert_Bmp(L"../Image/Map/Gate.bmp", L"Gate");

}

void CBmpMgr::Load_Character()
{
	/////////////////NPC/////////////////

	Insert_Bmp(L"../Image/NPC/CatNPC.bmp", L"CatNPC");



	//////////Player/////////////
	Insert_Bmp(L"../Image/Player/Player_Left.bmp", L"Player_Left");
	Insert_Bmp(L"../Image/Player/Player_Right.bmp", L"Player_Right");
	Insert_Bmp(L"../Image/Player/Stretch_Player.bmp", L"Player_Stretch");
	Insert_Bmp(L"../Image/Player/Stretch2.bmp", L"Player_Stretch2");
	Insert_Bmp(L"../Image/Player/Stretch_Slash.bmp", L"Stretch_Slash");
	Insert_Bmp(L"../Image/Player/Stretch_PlayerDeath.bmp", L"Stretch_PlayerDeath");
	////////////////Monster//////////////////

#pragma region CM_BigMonk LOAD
	Insert_Bmp(L"../Image/Monster/CM_BigMonk/BigPlant_Left 복사.bmp", L"BigPlant_Left");
	Insert_Bmp(L"../Image/Monster/CM_BigMonk/BigPlant_Right 복사.bmp", L"BigPlant_Right");

	Insert_Bmp(L"../Image/Monster/CM_BigMonk/BigPlantAttack_Stretch.bmp", L"BigPlant_Attack_Stretch");
	Insert_Bmp(L"../Image/Monster/CM_BigMonk/BigPlant_Stretch.bmp", L"BigPlant_Stretch");


#pragma endregion

#pragma region CM_JUMP LOAD
	///CM_JUMP
	Insert_Bmp(L"../Image/Monster/CM_JUMP/Left.bmp", L"CM_JUMP_L");
	Insert_Bmp(L"../Image/Monster/CM_JUMP/Right.bmp", L"CM_JUMP_R");
#pragma endregion

#pragma region CM_POISION LOAD
	Insert_Bmp(L"../Image/Monster/CM_POISION/sss.bmp", L"CM_R");
	Insert_Bmp(L"../Image/Monster/CM_POISION/sss_L.bmp", L"CM_L");

	Insert_Bmp(L"../Image/Stretch.bmp", L"Stretch");


#pragma endregion


#pragma region CM_STAFF LOAD
	Insert_Bmp(L"../Image/Monster/CM_STAFF/Left.bmp", L"CM_STAFF_L");
	Insert_Bmp(L"../Image/Monster/CM_STAFF/Right.bmp", L"CM_STAFF_R");
#pragma endregion


#pragma region CM_FENNEL
	Insert_Bmp(L"../Image/Monster/CM_Fennel/Boss_Left.bmp", L"CM_Fennel_L");
	Insert_Bmp(L"../Image/Monster/CM_Fennel/Boss_Right.bmp", L"CM_Fennel_R");
	Insert_Bmp(L"../Image/Monster/CM_Fennel/Boss_Stretch.bmp", L"Boss_Stretch");

#pragma endregion
}

void CBmpMgr::Load_Weapon()
{
	Insert_Bmp(L"../Image/Weapon/Arrow.bmp", L"Arrow");
	Insert_Bmp(L"../Image/Weapon/Arrow_Stretch.bmp", L"Arrow_Stretch");

	Insert_Bmp(L"../Image/Weapon/Bomb.bmp", L"Bomb");
	Insert_Bmp(L"../Image/Weapon/BombCloud.bmp", L"BombCloud");
	Insert_Bmp(L"../Image/Weapon/Ball.bmp", L"FireBall");


	//Knife
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/CM_JUMP/Knife_Right.bmp", L"Knife_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/CM_JUMP/Knife_Left.bmp", L"Knife_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/CM_JUMP/Reset.bmp", L"Reset");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/CM_JUMP/Plg.bmp", L"Plg");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Weapon/Arrow_Plg.bmp", L"Arrow_Plg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Weapon/Arrow_Reset.bmp", L"Arrow_Reset");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Weapon/Staff.bmp", L"Staff");


	Insert_Bmp(L"../Image/Monster/CM_Fennel/Thunder.bmp", L"Thunder");
	Insert_Bmp(L"../Image/Monster/CM_Fennel/BossEffectStretch.bmp", L"BossEffectStretch");
	Insert_Bmp(L"../Image/Monster/CM_Fennel/HitBound.bmp", L"HitBound");

}

void CBmpMgr::Load_Object()
{
	///인터렉티브 오브젝트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/Chest.bmp", L"Chest");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/Crystal.bmp", L"Crystal");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/JumpPad.bmp", L"JumpPad");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/MovingTile.bmp", L"MovingTile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/JumpPad_2.bmp", L"JumpPad_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/Elevator.bmp", L"Elevator");



	//아이템
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Flower.bmp", L"Flower");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Flower.bmp", L"Flower");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Statue.bmp", L"Statue");

}

void CBmpMgr::Load_Effect()
{
	Insert_Bmp(L"../Image/Effect/Particle_Stretch.bmp", L"Particle_Stretch");
	Insert_Bmp(L"../Image/Effect/Charge_Stretch.bmp", L"Charge_Stretch");

	Insert_Bmp(L"../Image/Effect/ChargeEffect.bmp", L"ChargeEffect");
	Insert_Bmp(L"../Image/Effect/ArrowEffect.bmp", L"ArrowEffect");
	Insert_Bmp(L"../Image/Effect/ItemEffect.bmp", L"ItemEffect");
	Insert_Bmp(L"../Image/Effect/AttackEffect_.bmp", L"AttackEffect");
	Insert_Bmp(L"../Image/Effect/HitLarge.bmp", L"HitLarge");

	Insert_Bmp(L"../Image/Effect/UsingItem.bmp", L"UsingItem");

	Insert_Bmp(L"../Image/Effect/SaveEffect.bmp", L"SaveEffect");

	
}

void CBmpMgr::Load_UI()
{
	Insert_Bmp(L"../Image/UI/GameMenu.bmp", L"GameMenu");
	Insert_Bmp(L"../Image/UI/Slot.bmp", L"Slot");
	Insert_Bmp(L"../Image/UI/Inven_background.bmp", L"Inven_background");
	Insert_Bmp(L"../Image/UI/Menu_Back2.bmp", L"Menu_Back2");

	Insert_Bmp(L"../Image/Item/Flower_Name_.bmp", L"Flower_Name");
	Insert_Bmp(L"../Image/Item/Flower_UI.bmp", L"Flower_UI");
	Insert_Bmp(L"../Image/Item/Flower_Script.bmp", L"Flower_Script");

	Insert_Bmp(L"../Image/Item/Coin/Money.bmp", L"Money");
	Insert_Bmp(L"../Image/Item/Coin/MoneyEffect.bmp", L"MoneyEffect");



	Insert_Bmp(L"../Image/UI/Search.bmp", L"Search");
	Insert_Bmp(L"../Image/UI/Pray.bmp", L"Pray");
	Insert_Bmp(L"../Image/UI/Talk.bmp", L"Talk");

	Insert_Bmp(L"../Image/UI/PlayerHP_Bar_0.bmp", L"HPBar_Back");
	Insert_Bmp(L"../Image/UI/PlayerHP_Bar_1.bmp", L"HPBar_Current");
	Insert_Bmp(L"../Image/UI/PlayerHP_Bar_2.bmp", L"HPBar_Damage");
	Insert_Bmp(L"../Image/UI/Hpbar_Effect.bmp", L"Hpbar_Effect");

	Insert_Bmp(L"../Image/UI/BigSlot.bmp", L"BigSlot");

	Insert_Bmp(L"../Image/UI/MoneyUI.bmp", L"MoneyUI");

	Insert_Bmp(L"../Image/UI/BackRed.bmp", L"HitScreen");

	Insert_Bmp(L"../Image/UI/Cat_Dialogue.bmp", L"Cat_Dialogue");



#pragma region Number
	Insert_Bmp(L"../Image/UI/Number/Zero.bmp", L"Number_0");
	Insert_Bmp(L"../Image/UI/Number/One.bmp", L"Number_1");
	Insert_Bmp(L"../Image/UI/Number/Two.bmp", L"Number_2");
	Insert_Bmp(L"../Image/UI/Number/Three.bmp", L"Number_3");
	Insert_Bmp(L"../Image/UI/Number/Four.bmp", L"Number_4");
	Insert_Bmp(L"../Image/UI/Number/Five.bmp", L"Number_5");
	Insert_Bmp(L"../Image/UI/Number/Six.bmp", L"Number_6");
	Insert_Bmp(L"../Image/UI/Number/Seven.bmp", L"Number_7");
	Insert_Bmp(L"../Image/UI/Number/Eight.bmp", L"Number_8");
	Insert_Bmp(L"../Image/UI/Number/Nine.bmp", L"Number_9");

#pragma endregion


#pragma region UI_Tutorial
	Insert_Bmp(L"../Image/UI/Pannel/1.Arrow.bmp", L"Tuto_1");
	Insert_Bmp(L"../Image/UI/Pannel/2.Jump_A.bmp", L"Tuto_2");
	Insert_Bmp(L"../Image/UI/Pannel/3.Jump(2).bmp", L"Tuto_3");
	Insert_Bmp(L"../Image/UI/Pannel/4.Down+A.bmp", L"Tuto_4");
	Insert_Bmp(L"../Image/UI/Pannel/5.Attack.bmp", L"Tuto_5");
	Insert_Bmp(L"../Image/UI/Pannel/6.Attack(2).bmp", L"Tuto_6");
	Insert_Bmp(L"../Image/UI/Pannel/7.Inventory.bmp", L"Tuto_7");
	Insert_Bmp(L"../Image/UI/Pannel/8.Save.bmp", L"Tuto_8");


#pragma endregion
	Insert_Bmp(L"../Image/UI/BossHP_0.bmp", L"BossHP_0");
	Insert_Bmp(L"../Image/UI/BossHP_1.bmp", L"BossHP_1");
	Insert_Bmp(L"../Image/UI/BossHP_2.bmp", L"BossHP_2");
	Insert_Bmp(L"../Image/UI/BossName.bmp", L"BossName");


	Insert_Bmp(L"../Image/UI/Death.bmp", L"Death");
	Insert_Bmp(L"../Image/UI/DeathMenu.bmp", L"DeathMenu");





#pragma region SelectMenu
	Insert_Bmp(L"../Image/UI/SelectMenu/SelectMenu.bmp", L"SelectMenu");
	Insert_Bmp(L"../Image/UI/SelectMenu/SaveSlot_None.bmp", L"SaveSlot_None");
	Insert_Bmp(L"../Image/UI/SelectMenu/SaveSlot_None.bmp", L"SaveSlot_None");
	Insert_Bmp(L"../Image/UI/SelectMenu/SaveSlot_Save.bmp", L"SaveSlot_Save");

	Insert_Bmp(L"../Image/UI/SelectMenu/Exit.bmp", L"Exit");




#pragma endregion
	Insert_Bmp(L"../Image/UI/SaveMenu.bmp", L"SaveMenu");
	Insert_Bmp(L"../Image/UI/White_Back.bmp", L"White_Back");
	Insert_Bmp(L"../Image/UI/Save_Complete.bmp", L"Save_Complete");
	Insert_Bmp(L"../Image/UI/Title.bmp", L"Title");


}
