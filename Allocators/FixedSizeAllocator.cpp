#include "FixedSizeAllocator.h"
#include "windows.h"
#include  <malloc.h>
#include <iostream>


FixedSizeAllocator::FixedSizeAllocator(const std::size_t chunkSize) : m_chunkSize(chunkSize), m_freeList()
{
	isInit = false;
}

FixedSizeAllocator::~FixedSizeAllocator()
{
	this->FixedSizeAllocator::destroy();
}

void FixedSizeAllocator::init()
{
	request_page();
	isInit = true;
}

void* FixedSizeAllocator::alloc()
{
	
	if(m_freeList.head == nullptr)
	{
		request_page();
	}
	Node* freePosition = m_freeList.pop();
	dumpSet.insert(freePosition);
	return static_cast<void*>(freePosition);
}

bool FixedSizeAllocator::free(void* ptr)
{
	auto contains = dumpSet.erase(ptr);
	if (contains) {
		m_freeList.push(static_cast<Node*>(ptr));
	}
	return contains;
}

void FixedSizeAllocator::destroy()
{
	for (auto page : pages)
	{
		VirtualFree(page, 0, MEM_RELEASE);
	}
	isInit = false;
	dumpSet.clear();
	
}
void FixedSizeAllocator::dumpBlocks()
{
	for (auto val : dumpSet) {
		std::cout << val << " " << m_chunkSize << std::endl;
	}
	
}

void FixedSizeAllocator::chunk_page(void* page)
{
	const int nChunks = pageSize / m_chunkSize;
	for (int i = 0; i < nChunks; ++i) {
		const std::size_t address = reinterpret_cast<std::size_t>(page) + i * m_chunkSize;
		m_freeList.push(reinterpret_cast<Node*>(address));
	}
}

void* FixedSizeAllocator::request_page()
{
	const auto page = VirtualAlloc(nullptr, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	pages.push_back(page);
	chunk_page(page);
	return page;
}
