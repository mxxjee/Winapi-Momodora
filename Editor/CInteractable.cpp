#include "pch.h"
#include "CInteractable.h"

CInteractable::CInteractable()
	:m_eInterType(INTERACTIVE_TYPE::END), m_pFrameKey(nullptr)
{
}

CInteractable::~CInteractable()
{
}

void CInteractable::Initialize()
{
}

int CInteractable::Update()
{
	return NOEVENT;
}

void CInteractable::Late_Update()
{
}

void CInteractable::Render(HDC hDC)
{
}

void CInteractable::Release()
{
}
