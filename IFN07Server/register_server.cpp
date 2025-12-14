// register_server.cpp
#include <windows.h>
#include <iostream>
#include <string>

// Функция для регистрации DLL
BOOL RegisterServer(const wchar_t* dllPath) {
    HMODULE hModule = LoadLibrary(dllPath);
    if (!hModule) {
        std::wcerr << L"Не удалось загрузить DLL: " << dllPath << std::endl;
        return FALSE;
    }

    // Получаем адрес функции DllRegisterServer
    FARPROC pFunc = GetProcAddress(hModule, "DllRegisterServer");
    if (!pFunc) {
        std::wcerr << L"Функция DllRegisterServer не найдена" << std::endl;
        FreeLibrary(hModule);
        return FALSE;
    }

    // Вызываем функцию регистрации
    HRESULT hr = ((HRESULT(WINAPI*)())pFunc)();

    FreeLibrary(hModule);

    if (SUCCEEDED(hr)) {
        std::wcout << L"Сервер успешно зарегистрирован" << std::endl;
        return TRUE;
    }
    else {
        std::wcerr << L"Ошибка регистрации: 0x" << std::hex << hr << std::endl;
        return FALSE;
    }
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc != 2) {
        std::wcout << L"Использование: RegisterServer.exe <путь_к_dll>" << std::endl;
        return 1;
    }

    if (RegisterServer(argv[1])) {
        return 0;
    }

    return 1;
}