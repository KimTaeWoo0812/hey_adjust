#ifndef __FUNCTION_H__
#define __FUNCTION_H__

template <typename T>
inline T* GetSingle(void)
{
	return (*T::GetInstance());
}
 
template <typename T>
inline void Safe_Delete(T& Temp)
{
    if (Temp != nullptr)
	{
		delete Temp;
        Temp = nullptr;
	}
}

struct DelObject
{
	template <typename T>
	void operator() ( T& Temp)
	{
        if (Temp != nullptr)
		{
			delete Temp;
            Temp = nullptr;
		}
	}
};

#endif