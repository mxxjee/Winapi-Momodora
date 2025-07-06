#include "pch.h"
#include "CMyBmp.h"

CMyBmp::CMyBmp()
{
}

CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC	hDC = GetDC(g_hWnd);

	// CreateCompatibleDC : 매개 변수로 전달된 dc와 호환되는 dc를 할당

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,	// 프로그램의 인스턴스 핸들	
							pFilePath,		// 파일의 경로를 전달
							IMAGE_BITMAP,	// 어떤 형식의 파일
							0,				// 이미지 가로, 세로 크기
							0, 
							LR_LOADFROMFILE | LR_CREATEDIBSECTION);
							//	LR_LOADFROMFILE :  파일로부터 이미지를 읽어들임
							//	LR_CREATEDIBSECTION : 읽어온 파일을 DIB 형태로 변환

	// SelectObject : 준비한 dc에 불러온 gdi 오브젝트를 선택, 
	// 이 함수는 gdi 오브젝트를 선택하기 전에 기존에 가지고 있던 오브젝트를 반환

	m_hOldmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
