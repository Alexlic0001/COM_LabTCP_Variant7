// ifn_07.h - ИЗМЕНЕННАЯ ВЕРСИЯ
#pragma once
#include <windows.h>
#include <objbase.h>
#include <initguid.h>  // Добавьте эту строку!

// ВАШИ GUID (вставьте свои реальные)
// IID для интерфейса
DEFINE_GUID(IID_IFN07,
    0x4e780f01, 0x2fdd, 0x41fa, 0x83, 0x5d, 0xe2, 0xcc, 0xae, 0xa3, 0xf7, 0x4f);

// CLSID для компонента (СГЕНЕРИРУЙТЕ ВТОРОЙ GUID!)


// {791D94A7-47F3-4837-A1F6-36CCCC8616F0}
DEFINE_GUID(CLSID_IFN07Component,
    0x791d94a7, 0x47f3, 0x4837, 0xa1, 0xf6, 0x36, 0xcc, 0xcc, 0x86, 0x16, 0xf0);

class IFN_07 : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE Fun71(int a, int b, int* result) = 0;
    virtual HRESULT STDMETHODCALLTYPE Fun72(int a, int b, double* result) = 0;
    virtual HRESULT STDMETHODCALLTYPE Fun73(double in, double* out) = 0;
};