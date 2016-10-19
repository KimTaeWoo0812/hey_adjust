#include "Random.h"

CRandom::CRandom()
{
}


CRandom::~CRandom()
{
}

void CRandom::Initialize()
{
    m_MakeRandom = std::mt19937_64(m_RandomDevice());
}

void CRandom::SetNumber(const int& nSmall, const int& nBig)
{
    m_Random = std::uniform_int_distribution<int>(nSmall, nBig);
}

int CRandom::GetRandomNumber()
{
    return m_Random(m_MakeRandom);
}