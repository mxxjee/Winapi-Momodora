#pragma once

class CObj;


class CDebugManager
{
public:
	static void	PrintCursor(HDC hdc, const POINT& pt, int x, int y)
	{
#ifdef _DEBUG
		WCHAR szBuffer[64];
		wsprintf(szBuffer, L"Cursor: X = %d, Y = %d", pt.x, pt.y);
		TextOut(hdc, 10, 10, szBuffer, lstrlen(szBuffer));

#endif
	}

	static void PrintState(HDC hdc, LPCWSTR _State,int x, int y)
	{
#ifdef _DEBUG
		TextOut(hdc,x,y,_State,lstrlen(_State));
#endif
	}

	static void PrintValue(HDC hdc, int value, int x, int y)
	{
#ifdef _DEBUG
		WCHAR szBuffer[64];
		wsprintf(szBuffer,L"Value = %d", value);
		TextOut(hdc,x, y, szBuffer, lstrlen(szBuffer));

#endif
	}
	
};

