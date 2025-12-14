// ifn_07.h
#pragma once
#include <windows.h>
#include <objbase.h>

// Вставьте сгенерированные GUID
// {4E780F01-2FDD-41FA-835D-E2CCAEA3F74F}
DEFINE_GUID(IID_IFN07,
    0x4e780f01, 0x2fdd, 0x41fa, 0x83, 0x5d, 0xe2, 0xcc, 0xae, 0xa3, 0xf7, 0x4f);

// {СГЕНЕРИРУЙТЕ ВТОРОЙ GUID ДЛЯ CLSID!}
// Не используйте тот же GUID! Сгенерируйте новый!

// {9E9873BF-50FB-45A8-80E0-B2858B09259C}
DEFINE_GUID(CLSID_IFN07Component,
    0x9e9873bf, 0x50fb, 0x45a8, 0x80, 0xe0, 0xb2, 0x85, 0x8b, 0x9, 0x25, 0x9c);


// Интерфейс IFN_07
class IFN_07 : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE Fun71(int a, int b, int* result) = 0;
    virtual HRESULT STDMETHODCALLTYPE Fun72(int a, int b, double* result) = 0;
    virtual HRESULT STDMETHODCALLTYPE Fun73(double in, double* out) = 0;
};