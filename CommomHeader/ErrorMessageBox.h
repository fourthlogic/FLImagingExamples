#pragma once

#include <FLImaging.h>

static void ErrorMessageBox(const CResult& result, const char* pAdditionalText = nullptr)
{
	CFLStringW flsMessage;
	flsMessage.Format(L"Error code : %d\nError name : %s\n", result.GetResultCode(), result.GetString());

	if(pAdditionalText)
	{
		CFLStringW flsAdditionalText(pAdditionalText);
		flsMessage += flsAdditionalText;
	}

	MessageBox(nullptr, flsMessage, L"Error", MB_ICONERROR);
}

static void ErrorMessageBox(const CResult& result, const wchar_t* pAdditionalText)
{
	CFLStringW flsMessage;
	flsMessage.Format(L"Error code : %d\nError name : %s\n", result.GetResultCode(), result.GetString());

	if(pAdditionalText)
	{
		CFLStringW flsAdditionalText(pAdditionalText);
		flsMessage += flsAdditionalText;
	}

	MessageBox(nullptr, flsMessage, L"Error", MB_ICONERROR);
}