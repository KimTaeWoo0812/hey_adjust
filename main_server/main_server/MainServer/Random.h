#pragma once

#include "stdafx.h"

class CRandom
{
private :
    std::random_device                  m_RandomDevice;
    std::mt19937_64                     m_MakeRandom;
    std::uniform_int_distribution<int>  m_Random;

public :
    void Initialize();

    void SetNumber(const int& nSmall, const int& nBig);
    int GetRandomNumber();

public:
    CRandom();
    virtual ~CRandom();
};

