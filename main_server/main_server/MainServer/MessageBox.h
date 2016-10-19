#pragma once
#include "IBase.h"

template <typename T>
class CMessageBox :
	public IBase
{	
private :
	std::queue<T>		m_Queue;

public :
	void PushMessage(const T& pMessage);

public :
	const T			getMessage();
	const bool	    hasMessage()    const;

public:
	virtual int Initialize()					override;
	virtual int Progress()						override;
	virtual int Render()						override;
	virtual int Release()						override;

public:
	CMessageBox();
	virtual ~CMessageBox();
};

template <typename T>
CMessageBox<T>::CMessageBox()
{
}

template <typename T>
CMessageBox<T>::~CMessageBox()
{
}

template <typename T>
int CMessageBox<T>::Initialize()
{
	return 0;
}

template <typename T>
int CMessageBox<T>::Progress()
{
	return 0;
}

template <typename T>
int CMessageBox<T>::Render()
{
	return 0;
}

template <typename T>
int CMessageBox<T>::Release()
{
	return 0;
}

template <typename T>
void CMessageBox<T>::PushMessage(const T& strMessage)
{
	m_Queue.push(strMessage);
}

template <typename T>
const T CMessageBox<T>::getMessage()
{
	T strMessage;

	strMessage = m_Queue.front();
	m_Queue.pop();

	return strMessage;
}

template <typename T>
const bool CMessageBox<T>::hasMessage()  const
{
	return !m_Queue.empty();
}