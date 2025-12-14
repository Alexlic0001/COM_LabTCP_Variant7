// cfn_07.cpp
#include "cfn_07.h"
#include <windows.h>

// Инициализация глобальных переменных (определение будет в server.cpp)
extern long g_lObjs;
extern long g_lLocks;

// ===== CIFN07 =====
CIFN07::CIFN07() : m_lRef(0) {
    InterlockedIncrement(&g_lObjs);
}

CIFN07::~CIFN07() {
    InterlockedDecrement(&g_lObjs);
}

// IUnknown методы
STDMETHODIMP CIFN07::QueryInterface(REFIID riid, void** ppv) {
    if (ppv == NULL) return E_POINTER;
    *ppv = NULL;

    if (riid == IID_IUnknown || riid == IID_IFN07) {
        *ppv = static_cast<IFN_07*>(this);
        this->AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CIFN07::AddRef() {
    return InterlockedIncrement(&m_lRef);
}

STDMETHODIMP_(ULONG) CIFN07::Release() {
    LONG ref = InterlockedDecrement(&m_lRef);
    if (ref == 0) {
        delete this;
    }
    return ref;
}

// Реализация методов IFN_07
STDMETHODIMP CIFN07::Fun71(int a, int b, int* result) {
    if (result == NULL) return E_POINTER;
    *result = a + b;  // Пример: сложение
    return S_OK;
}

STDMETHODIMP CIFN07::Fun72(int a, int b, double* result) {
    if (result == NULL) return E_POINTER;
    if (b == 0) return E_INVALIDARG;
    *result = static_cast<double>(a) / b;
    return S_OK;
}

STDMETHODIMP CIFN07::Fun73(double in, double* out) {
    if (out == NULL) return E_POINTER;
    *out = in * 2.0;  // Пример: умножение на 2
    return S_OK;
}

// ===== CIFN07ClassFactory =====
CIFN07ClassFactory::CIFN07ClassFactory() : m_lRef(0) {
}

CIFN07ClassFactory::~CIFN07ClassFactory() {
}

// IUnknown методы для фабрики
STDMETHODIMP CIFN07ClassFactory::QueryInterface(REFIID riid, void** ppv) {
    if (ppv == NULL) return E_POINTER;
    *ppv = NULL;

    if (riid == IID_IUnknown || riid == IID_IClassFactory) {
        *ppv = static_cast<IClassFactory*>(this);
        this->AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CIFN07ClassFactory::AddRef() {
    return InterlockedIncrement(&m_lRef);
}

STDMETHODIMP_(ULONG) CIFN07ClassFactory::Release() {
    LONG ref = InterlockedDecrement(&m_lRef);
    if (ref == 0) {
        delete this;
    }
    return ref;
}

// Создание экземпляра компонента
STDMETHODIMP CIFN07ClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv) {
    if (ppv == NULL) return E_POINTER;
    *ppv = NULL;

    // Не поддерживаем агрегирование
    if (pUnkOuter != NULL) {
        return CLASS_E_NOAGGREGATION;
    }

    // Создаем экземпляр компонента
    CIFN07* pObj = new CIFN07();
    if (pObj == NULL) {
        return E_OUTOFMEMORY;
    }

    // Запрашиваем требуемый интерфейс
    HRESULT hr = pObj->QueryInterface(riid, ppv);

    // Если не удалось, освобождаем объект
    if (FAILED(hr)) {
        delete pObj;
    }

    return hr;
}

// Блокировка сервера в памяти
STDMETHODIMP CIFN07ClassFactory::LockServer(BOOL fLock) {
    if (fLock) {
        InterlockedIncrement(&g_lLocks);
    }
    else {
        InterlockedDecrement(&g_lLocks);
    }
    return S_OK;
}