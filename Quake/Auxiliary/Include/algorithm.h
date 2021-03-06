#pragma once

#include "..\..\Container\Include\iterator.hpp"

template<typename TIterator>
Distance iterator_distance(TIterator A, TIterator B)
{
	return A.ptr() - B.ptr();
}

template<typename TIterator, typename TDistance = size_t>
TIterator iterator_traverse(TIterator iterator, TDistance distance)
{
	return iterator + distance;
}

//Distance index_distance(Distance A, Distance B)
//{
//	return A - B;
//}

template<typename Index = size_t>
Index index_traverse(Index index, Distance distance)
{
	return index + distance;
}

template<typename TPointer>
Distance array_distance(TPointer A, TPointer B)
{
	return A - B;
}

template<typename TPointer>
TPointer array_traverse(TPointer array_ptr, Distance distance)
{
	return array_ptr + distance;
}

template<typename TIterator, typename TValue>
TIterator find(TIterator begin, TIterator end, TValue value)
{
	for (TIterator i = begin; i < end; ++i)
	{
		if (i.get() == value)
		{
			return i;
		}
	}
	return end;
}

template<typename TIterator, typename TValue>
TIterator find_last(TIterator begin, TIterator end, TValue value)
{
	TIterator last = end;
	for (TIterator i = begin; i < end; ++i)
	{
		if (i.get() == value)
		{
			last = i;
		}
	}
	return last;
}

template<typename TIterator, typename TConditional>
TIterator find_if(TIterator begin, TIterator end, TConditional conditional)
{
	for (TIterator i = begin; i < end; ++i)
	{
		if (conditional(i.get()))
		{
			return i;
		}
	}
	return end;
}

template<typename TIterator, typename TConditional, typename TValue>
TIterator find_if_val(TIterator begin, TIterator end, TConditional conditional, TValue value)
{
	for (TIterator i = begin; i < end; ++i)
	{
		if (conditional(value, i.get()))
		{
			return i;
		}
	}
	return end;
}

template<typename TIterator, typename TConditional, typename TValue>
TIterator find_last_if_val(TIterator begin, TIterator end, TConditional conditional, TValue value)
{
	TIterator last = end;
	for (TIterator i = begin; i < end; ++i)
	{
		if (conditional(value, i.get()))
		{
			last = i;
		}
	}
	return last;
}

template<typename TIterator, typename TConditional>
TIterator find_if_not(TIterator begin, TIterator end, TConditional conditional)
{
	for (TIterator i = begin; i < end; ++i)
	{
		if (!conditional(i.get()))
		{
			return i;
		}
	}
	return end;
}

template<typename TIterator, typename TConditional, typename TValue>
TIterator find_if_not_val(TIterator begin, TIterator end, TConditional conditional, TValue value)
{
	for (TIterator i = begin; i < end; ++i)
	{
		if (!conditional(value, i.get()))
		{
			return i;
		}
	}
	return end;
}

template<typename TIterator, typename TConverter, typename TValue = TIterator::Value>
void convert_to(TIterator begin, TIterator end, TConverter converter)
{
	for (TIterator i = begin; i < end; ++i)
	{
		TValue value = converter(i.get());
		i.set(value);
	}
}