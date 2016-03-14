#pragma once

#include "base_allocator.h"
#include "null_allocator.h"

#include "..\Internal\auxiliary.h"
#include "..\Internal\traits.h"

#include "..\..\..\Container\Include\qbool.h"
#include "..\..\..\common.h"

/*Batching is incorrect. Make freelist weave many blocks into one block*/

class FreeList
{
public:
	struct Node
	{
		Node* next;
	};
	inline FreeList();
	inline FreeList(size_t capacity, size_t blockSize);

	inline void allocate(void* memory);
	inline Block deallocate();
	inline QBool owns(Block block) const;

	inline QBool isFull() const;
	inline QBool hasFreeMemory() const;

	inline size_t getCapacity() const;
	inline size_t getNumNodes() const;
	inline size_t getMemoryUsed() const;
	inline size_t getBlockSize() const;
private:
	inline Node* _getNext(Node* current);
	inline void _setNext(Node* current, Node* next);

	inline Node* _blockToNode(Block& block);

	inline void _incrementNumNodes();
	inline void _decrementNumNodes();

	inline void _addMemoryUsed();
	inline void _subtractMemoryUsed();

	Node* _root;
	size_t _capacity;
	size_t _blockSize;
	size_t _numNodes;
	size_t _memoryUsed;

	FreeList(FreeList&) = delete;
	FreeList(const FreeList&) = delete;
	FreeList(FreeList&&) = delete;
	FreeList operator=(FreeList&) = delete;
	FreeList& operator=(const FreeList&) = delete;
	FreeList* operator&() = delete;

	void* operator new(size_t) = delete;
	void* operator new[](size_t) = delete;
	void operator delete(void*) = delete;
	void operator delete[](void*) = delete;
};

#include "..\..\Source\Allocator\freelist.inl"