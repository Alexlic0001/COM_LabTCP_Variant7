// client.cpp
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <initguid.h>  // ДОЛЖЕН БЫТЬ ПЕРЕД ifn_07.h
#include "ifn_07.h"

int main() {
    // Для правильного отображения русских букв
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    printf("=========================================\n");
    printf("COM Client for IFN07 Component\n");
    printf("Variant 7\n");
    printf("=========================================\n\n");

    printf("[1] Initializing COM...\n");
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        printf("   ERROR: COM initialization failed (0x%08X)\n", hr);
        return 1;
    }
    printf("   OK: COM initialized\n\n");

    printf("[2] Creating COM component...\n");
    IFN_07* pIFN = NULL;

    // Используем прямые GUID из ifn_07.h
    hr = CoCreateInstance(
        CLSID_IFN07Component,    // {791D94A7-47F3-4837-A1F6-36CCCC8616F0}
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IFN07,               // {4E780F01-2FDD-41FA-835D-E2CCAEA3F74F}
        (void**)&pIFN
    );

    if (SUCCEEDED(hr) && pIFN) {
        printf("   SUCCESS: Component created!\n\n");

        printf("[3] Testing functions:\n");
        printf("-----------------------------------------\n");

        // Тестируем функции
        int result71;
        hr = pIFN->Fun71(10, 20, &result71);
        if (SUCCEEDED(hr)) {
            printf("   Fun71(10, 20) = %d\n", result71);
        }

        double result72;
        hr = pIFN->Fun72(100, 3, &result72);
        if (SUCCEEDED(hr)) {
            printf("   Fun72(100, 3) = %.2f\n", result72); 
        }

        double result73;
        hr = pIFN->Fun73(15.5, &result73);
        if (SUCCEEDED(hr)) {
            printf("   Fun73(15.5) = %.2f\n", result73);
        }

        printf("-----------------------------------------\n");

        pIFN->Release();
        printf("\n   Component released.\n");
    }
    else {
        printf("   ERROR: Cannot create component (0x%08X)\n", hr);

        if (hr == 0x80040154) {
            printf("   This means: Class not registered\n");
            printf("   Your CLSID: {791D94A7-47F3-4837-A1F6-36CCCC8616F0}\n");
            printf("   Check registration with:\n");
            printf("   reg query \"HKCR\\CLSID\\{791D94A7-47F3-4837-A1F6-36CCCC8616F0}\\InprocServer32\"\n");
        }
    }

    CoUninitialize();
    printf("\n[4] COM uninitialized.\n");
    printf("=========================================\n");

    system("pause");
    return 0;
}