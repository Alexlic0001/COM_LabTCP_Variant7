// registry.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <objbase.h>
#include <assert.h>
#include "registry.h"

// Константы
const int CLSID_STRING_SIZE = 39;
const int KEY_NAME_SIZE = 1024;

// Вспомогательные функции
void CLSIDtoStr(const CLSID& clsid, wchar_t* szCLSID, int length);
LONG recursiveDeleteKey(HKEY hKeyParent, const wchar_t* szKeyChild);
BOOL setKeyAndValue(const wchar_t* szKey, const wchar_t* szSubkey, const wchar_t* szValue);

// Регистрация сервера
HRESULT RegisterServer(HMODULE hModule, const CLSID& clsid,
    const wchar_t* szFriendlyName,
    const wchar_t* szVerIndProgID,
    const wchar_t* szProgID) {

    // Получаем путь к DLL
    wchar_t szModule[1024];
    DWORD dwResult = GetModuleFileNameW(hModule, szModule, sizeof(szModule) / sizeof(wchar_t));
    assert(dwResult != 0);

    // Конвертируем CLSID в строку
    wchar_t szCLSID[CLSID_STRING_SIZE];
    CLSIDtoStr(clsid, szCLSID, sizeof(szCLSID));

    // Строим путь CLSID\{...}
    wchar_t szKey[KEY_NAME_SIZE];
    lstrcpyW(szKey, L"CLSID\\");
    lstrcatW(szKey, szCLSID);

    // 1. Добавляем CLSID в реестр
    setKeyAndValue(szKey, NULL, szFriendlyName);

    // 2. Добавляем путь к DLL
    setKeyAndValue(szKey, L"InprocServer32", szModule);
    setKeyAndValue(szKey, L"InprocServer32", L"ThreadingModel", L"Apartment");

    // 3. Добавляем ProgID
    setKeyAndValue(szKey, L"ProgID", szProgID);

    // 4. Добавляем VersionIndependentProgID
    setKeyAndValue(szKey, L"VersionIndependentProgID", szVerIndProgID);

    // 5. Регистрируем VersionIndependentProgID
    setKeyAndValue(szVerIndProgID, NULL, szFriendlyName);
    setKeyAndValue(szVerIndProgID, L"CLSID", szCLSID);
    setKeyAndValue(szVerIndProgID, L"CurVer", szProgID);

    // 6. Регистрируем ProgID
    setKeyAndValue(szProgID, NULL, szFriendlyName);
    setKeyAndValue(szProgID, L"CLSID", szCLSID);

    return S_OK;
}

// Отмена регистрации сервера
HRESULT UnregisterServer(const CLSID& clsid,
    const wchar_t* szVerIndProgID,
    const wchar_t* szProgID) {

    // Конвертируем CLSID в строку
    wchar_t szCLSID[CLSID_STRING_SIZE];
    CLSIDtoStr(clsid, szCLSID, sizeof(szCLSID));

    // Удаляем ключи из реестра
    wchar_t szKey[128];
    lstrcpyW(szKey, L"CLSID\\");
    lstrcatW(szKey, szCLSID);

    // 1. Удаляем CLSID ключ
    LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);
    assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND));

    // 2. Удаляем VersionIndependentProgID
    lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);
    assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND));

    // 3. Удаляем ProgID
    lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);
    assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND));

    return S_OK;
}

// Конвертация CLSID в строку
void CLSIDtoStr(const CLSID& clsid, wchar_t* szCLSID, int length) {
    assert(length >= CLSID_STRING_SIZE);
    LPOLESTR wszCLSID = NULL;
    HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
    assert(SUCCEEDED(hr));
    lstrcpyW(szCLSID, wszCLSID);
    CoTaskMemFree(wszCLSID);
}

// Рекурсивное удаление ключа реестра
LONG recursiveDeleteKey(HKEY hKeyParent, const wchar_t* lpszKeyChild) {
    HKEY hKeyChild;
    LONG lRes = RegOpenKeyExW(hKeyParent, lpszKeyChild, 0, KEY_ALL_ACCESS, &hKeyChild);

    if (lRes != ERROR_SUCCESS) {
        return lRes;
    }

    // Удаляем все подключачи
    FILETIME time;
    wchar_t szBuffer[256];
    DWORD dwSize = 256;

    while (RegEnumKeyExW(hKeyChild, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time) == S_OK) {
        lRes = recursiveDeleteKey(hKeyChild, szBuffer);
        if (lRes != ERROR_SUCCESS) {
            RegCloseKey(hKeyChild);
            return lRes;
        }
        dwSize = 256;
    }

    RegCloseKey(hKeyChild);
    return RegDeleteKeyW(hKeyParent, lpszKeyChild);
}

// Создание ключа и установка значения
BOOL setKeyAndValue(const wchar_t* szKey, const wchar_t* szSubkey, const wchar_t* szValue) {
    wchar_t szKeyBuf[KEY_NAME_SIZE];
    lstrcpyW(szKeyBuf, szKey);

    if (szSubkey != NULL) {
        lstrcatW(szKeyBuf, L"\\");
        lstrcatW(szKeyBuf, szSubkey);
    }

    HKEY hKey;
    LONG lResult = RegCreateKeyExW(HKEY_CLASSES_ROOT, szKeyBuf, 0, NULL,
        REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
        NULL, &hKey, NULL);

    if (lResult != ERROR_SUCCESS) {
        return FALSE;
    }

    if (szValue != NULL) {
        int size = 2 * lstrlenW(szValue) + 2;  // +2 для завершающего нуля в Unicode
        RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)szValue, size);
    }

    RegCloseKey(hKey);
    return TRUE;
}

// Перегрузка для ThreadingModel
BOOL setKeyAndValue(const wchar_t* szKey, const wchar_t* szSubkey, const wchar_t* szName, const wchar_t* szValue) {
    wchar_t szKeyBuf[KEY_NAME_SIZE];
    lstrcpyW(szKeyBuf, szKey);

    if (szSubkey != NULL) {
        lstrcatW(szKeyBuf, L"\\");
        lstrcatW(szKeyBuf, szSubkey);
    }

    HKEY hKey;
    LONG lResult = RegCreateKeyExW(HKEY_CLASSES_ROOT, szKeyBuf, 0, NULL,
        REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
        NULL, &hKey, NULL);

    if (lResult != ERROR_SUCCESS) {
        return FALSE;
    }

    if (szName != NULL && szValue != NULL) {
        int size = 2 * lstrlenW(szValue) + 2;
        RegSetValueExW(hKey, szName, 0, REG_SZ, (BYTE*)szValue, size);
    }

    RegCloseKey(hKey);
    return TRUE;
}