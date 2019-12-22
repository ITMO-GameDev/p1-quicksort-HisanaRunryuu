#include "FreeListAllocator.h"
#include <limits>  /* limits_max */
#include "Utils.h"
#include <windows.h>
#include <iostream>
#define NOMINMAX   


FreeListAllocator::FreeListAllocator(const std::size_t totalSize, const PlacementPolicy pPolicy)
	: m_totalSize(totalSize), m_used(0), m_pPolicy(pPolicy)
{	
}

void FreeListAllocator::init()
{
	if (m_start_ptr != nullptr)
	{
		free(m_start_ptr);
		m_start_ptr = nullptr;
	}
	m_start_ptr = VirtualAlloc(nullptr, this->m_totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	this->reset();
}

void FreeListAllocator::destroy()
{
	VirtualFree(m_start_ptr, 0, MEM_RELEASE);
	m_used = 0;
	m_start_ptr = nullptr;
}

std::size_t FreeListAllocator::getFreeSize() const
{
	return m_totalSize - m_used;
}

FreeListAllocator::~FreeListAllocator()
{
	this->destroy();
}

void* FreeListAllocator::allocate(const std::size_t size, const std::size_t alignment)
{
	const std::size_t allocationHeaderSize = sizeof(AllocationHeader);
	const std::size_t freeHeaderSize = sizeof(FreeHeader);
	
	// Search through the free list for a free block that has enough space to allocate our data
	std::size_t padding;
	Node* affectedNode,
		* previousNode;
	this->find(size, alignment, padding, previousNode, affectedNode);


	const std::size_t alignmentPadding = padding - allocationHeaderSize;
	const std::size_t requiredSize = size + padding;
	const std::size_t rest = affectedNode->data.blockSize - requiredSize;

	if (rest > 0)
	{
		// We have to split the block into the data block and a free block of size 'rest'
		Node* newFreeNode = reinterpret_cast<Node*>(std::size_t(affectedNode) + requiredSize);
		newFreeNode->data.blockSize = rest;
		m_freeList.insert(affectedNode, newFreeNode);
	}
	m_freeList.remove(previousNode, affectedNode);

	// Setup data block
	const std::size_t headerAddress = std::size_t(affectedNode) + alignmentPadding;
	const std::size_t dataAddress = headerAddress + allocationHeaderSize;
	reinterpret_cast<AllocationHeader*>(headerAddress)->blockSize = requiredSize;
	reinterpret_cast<AllocationHeader*>(headerAddress)->padding = alignmentPadding;

	m_used += requiredSize;
	

	return reinterpret_cast<void*>(dataAddress);
}

void FreeListAllocator::find(const std::size_t size, const std::size_t alignment, std::size_t& padding,
	Node*& previousNode, Node*& foundNode) const
{
	switch (m_pPolicy)
	{
	case FIND_FIRST:
		find_first(size, alignment, padding, previousNode, foundNode);
		break;
	case FIND_BEST:
		find_best(size, alignment, padding, previousNode, foundNode);
		break;
	}
}

void FreeListAllocator::find_first(const std::size_t size, const std::size_t alignment, std::size_t& padding,
	Node*& previousNode, Node*& foundNode) const
{
	Node* it = m_freeList.head,
		* itPrev = nullptr;

	while (it != nullptr)
	{
		padding = Utils::CalculatePaddingWithHeader(std::size_t(it), alignment, sizeof(AllocationHeader));
		const std::size_t requiredSpace = size + padding;
		if (it->data.blockSize >= requiredSpace)
		{
			break;
		}
		itPrev = it;
		it = it->next;
	}
	previousNode = itPrev;
	foundNode = it;
}

void FreeListAllocator::find_best(const std::size_t size, const std::size_t alignment, std::size_t& padding,
	Node*& previousNode, Node*& foundNode) const
{
#undef max
	 auto smallestDiff = std::numeric_limits<std::size_t>::max();
	Node* bestBlock = nullptr;
	Node* it = m_freeList.head,
		* itPrev = nullptr;
	while (it != nullptr)
	{
		padding = Utils::CalculatePaddingWithHeader(std::size_t(it), alignment, sizeof(AllocationHeader));
		const std::size_t requiredSpace = size + padding;
		if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff))
		{
			bestBlock = it;
		}
		itPrev = it;
		it = it->next;
	}
	previousNode = itPrev;
	foundNode = bestBlock;
}

void FreeListAllocator::free(void* ptr)
{
	// Insert it in a sorted position by the address number
	const auto currentAddress = std::size_t(ptr);
	const std::size_t headerAddress = currentAddress - sizeof(AllocationHeader);
	const AllocationHeader* allocationHeader{ reinterpret_cast<AllocationHeader*>(headerAddress) };

	Node* freeNode = reinterpret_cast<Node*>(headerAddress);
	freeNode->data.blockSize = allocationHeader->blockSize + allocationHeader->padding;
	freeNode->next = nullptr;

	Node* it = m_freeList.head;
	Node* itPrev = nullptr;
	while (it != nullptr)
	{
		if (ptr < it)
		{
			m_freeList.insert(itPrev, freeNode);
			break;
		}
		itPrev = it;
		it = it->next;
	}

	m_used -= freeNode->data.blockSize;

	// Merge contiguous nodes
	coalescence(itPrev, freeNode);

}

void FreeListAllocator::coalescence(Node* previousNode, Node* freeNode)
{
	if (freeNode->next != nullptr &&
		std::size_t(freeNode) + freeNode->data.blockSize == std::size_t(freeNode->next))
	{
		freeNode->data.blockSize += freeNode->next->data.blockSize;
		m_freeList.remove(freeNode, freeNode->next);
	}

	if (previousNode != nullptr &&
		std::size_t(previousNode) + previousNode->data.blockSize == std::size_t(freeNode))
	{
		previousNode->data.blockSize += freeNode->data.blockSize;
		m_freeList.remove(previousNode, freeNode);

	}
}

void FreeListAllocator::reset()
{
	m_used = 0;
	Node* firstNode = static_cast<Node*>(m_start_ptr);
	firstNode->data.blockSize = m_totalSize;
	firstNode->next = nullptr;
	m_freeList.head = nullptr;
	m_freeList.insert(nullptr, firstNode);
}
