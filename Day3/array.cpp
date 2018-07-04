#include <iostream>
#include <array>


template<class T, std::size_t N>
class ArrayWrapper
{
	std::array<T, N> arr;
public:
	ArrayWrapper(class T, std::size_t N);
	T& operator[](std::size_t M);
	T& resize(std::size_t M);
};

template<class T, std::size_t N>
T& operator[](std::size_t M)
{
	if (M > arr.size())	
	{
		for(size_t i = 0; i < arr)
	}
	else
	{
		return arr[M];
	}
};

