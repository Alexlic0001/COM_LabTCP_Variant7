// cfn_07.h
#pragma once
#include "ifn_07.h"

// Глобальные счетчики
extern long g_lObjs;   // Счетчик активных объектов
extern long g_lLocks;  // Счетчик блокировок сервера

// Класс реализации компонента
class CIFN07 : public IFN_07 {
protected:
    long m_lRef; // Счетчик ссылок

public:
    CIFN07();
    virtual ~CIFN07();

    // Методы IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;

    // Методы IFN_07
    STDMETHOD(Fun71)(int a, int b, int* result) override;
    STDMETHOD(Fun72)(int a, int b, double* result) override;
    STDMETHOD(Fun73)(double in, double* out) override;
};

// Фабрика классов
class CIFN07ClassFactory : public IClassFactory {
protected:
    long m_lRef;

public:
    CIFN07ClassFactory();
    virtual ~CIFN07ClassFactory();

    // Методы IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;

    // Методы IClassFactory
    STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppv) override;
    STDMETHOD(LockServer)(BOOL fLock) override;
};