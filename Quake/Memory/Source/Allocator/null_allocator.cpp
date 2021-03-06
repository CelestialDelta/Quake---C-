#include "..\..\Include\Allocator\null_allocator.h"

Block NullAllocator::allocate(size_t size)
{
	return(UNALLOCATED_BLOCK);
}

void NullAllocator::deallocate(Block& block)
{
}

QBool NullAllocator::owns(Block block)
{
	return false;
}

ALLOCATOR_ID NullAllocator::getID() const
{
	return ALLOCATOR_ID::NULL_ALLOCATOR;
}