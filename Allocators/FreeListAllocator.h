#pragma once
#include "SinglyLinkedList.cpp"
#include <cstddef>
class FreeListAllocator 
{
public:
	enum PlacementPolicy
	{
		FIND_FIRST,
		FIND_BEST
	};

private:
	
	struct FreeHeader
	{
		std::size_t blockSize;
	};

	struct AllocationHeader
	{
		std::size_t blockSize;
		char padding;
	};

	typedef SinglyLinkedList<FreeHeader>::Node Node;

	std::size_t m_totalSize;
	std::size_t m_used;
	void* m_start_ptr = nullptr;
	PlacementPolicy m_pPolicy;
	SinglyLinkedList<FreeHeader> m_freeList;

public:
	FreeListAllocator(std::size_t totalSize, PlacementPolicy pPolicy);

	virtual ~FreeListAllocator();

	void* allocate(std::size_t size, std::size_t alignment = 8);

	void free(void* ptr);

	void init();

	void destroy();

	virtual void reset();
	std::size_t getFreeSize() const;
private:
	FreeListAllocator(FreeListAllocator& freeListAllocator);

	void coalescence(Node* previousNode, Node* freeNode);

	void find(std::size_t size, std::size_t alignment, std::size_t& padding, Node*& previousNode, Node*& foundNode) const;
	void find_best(std::size_t size, std::size_t alignment, std::size_t& padding, Node*& previousNode, Node*& foundNode) const;
	void find_first(std::size_t size, std::size_t alignment, std::size_t& padding, Node*& previousNode,
		Node*& foundNode) const;
};

