#pragma once

#include <conio.h>
#include <FLImaging.h>

static void ErrorPrint(const CResult& result, const char* pAdditionalText = nullptr)
{
	wprintf(L"Error code : %d\nError name : %s\n", result.GetResultCode(), result.GetString());

	if(pAdditionalText)
		printf(pAdditionalText);

	_getch();
}

static void ErrorPrint(const CResult& result, const wchar_t* pAdditionalText)
{
	wprintf(L"Error code : %d\nError name : %s\n", result.GetResultCode(), result.GetString());

	if(pAdditionalText)
		wprintf(pAdditionalText);

	_getch();
}