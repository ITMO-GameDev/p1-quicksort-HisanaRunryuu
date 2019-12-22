#pragma once
#include <map>
#include "FixedSizeAllocator.h"
#include  "FreeListAllocator.h"
class MemoryAllocator
{
private:
	std::map<int, FixedSizeAllocator*> FSAallocators;
	std::map<int, FreeListAllocator*> FLallocators;
	std::map<void*, FreeListAllocator*> dumpFLMap;
	bool isInit;

	static int align(int size);
public:
	MemoryAllocator();
	~MemoryAllocator();
	virtual void init();
	virtual void* alloc(int size);
	virtual void free(void* ptr);
	virtual void destroy();
	virtual void dumpBlocks();
	
	
};

