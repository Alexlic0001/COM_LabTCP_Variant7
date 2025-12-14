// server.cpp
#include <windows.h>
#include <initguid.h>  // ПЕРЕД ifn_07.h
#include "ifn_07.h"    // Ваш новый файл
#include "cfn_07.h"

// Определение глобальных переменных
long g_lObjs = 0;
long g_lLocks = 0;

// Точка входа DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// Основная точка входа COM: получение фабрики классов
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
    if (ppv == NULL) return E_POINTER;
    *ppv = NULL;

    // Проверяем, запрашивается ли наш компонент
    if (!IsEqualCLSID(rclsid, CLSID_IFN07Component)) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    // Создаем фабрику классов
    CIFN07ClassFactory* pCF = new CIFN07ClassFactory();
    if (pCF == NULL) {
        return E_OUTOFMEMORY;
    }

    // Запрашиваем требуемый интерфейс
    HRESULT hr = pCF->QueryInterface(riid, ppv);
    if (FAILED(hr)) {
        delete pCF;
    }

    return hr;
}

// Проверка возможности выгрузки DLL
STDAPI DllCanUnloadNow() {
    if (g_lObjs == 0 && g_lLocks == 0) {
        return S_OK;  // Можно выгружать
    }
    return S_FALSE;   // Нельзя выгружать
}

// Регистрация сервера (опционально, но рекомендуется)
STDAPI DllRegisterServer() {
    // Реализация будет добавлена позже
    return S_OK;
}

// Отмена регистрации сервера
STDAPI DllUnregisterServer() {
    // Реализация будет добавлена позже
    return S_OK;
}