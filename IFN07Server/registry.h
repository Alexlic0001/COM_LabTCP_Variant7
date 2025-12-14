// Файл заголовков/registry.h
#pragma once

#include <windows.h>

// Функции авторегистрации
HRESULT RegisterServer(HMODULE hModule,
    const CLSID& clsid,
    const wchar_t* szFriendlyName,
    const wchar_t* szVerIndProgID,
    const wchar_t* szProgID);

HRESULT UnregisterServer(const CLSID& clsid,
    const wchar_t* szVerIndProgID,
    const wchar_t* szProgID);