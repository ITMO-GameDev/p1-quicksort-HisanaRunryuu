#include "stdafx.h"
#include <cassert>
#include "Allocator.h"
#include "FSAllocator.h"
#include "CoalesceAllocator.h"

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

MemoryAllocator::MemoryAllocator()
{
	bool need_delete_on_throw = false;
	try
	{
		fs_al = new FSAllocator;
		need_delete_on_throw = true;
		co_al = new CoaleseAllocator;
	}
	catch (...)
	{
		if (need_delete_on_throw)
			delete fs_al;
		throw;
	}
}

MemoryAllocator::~MemoryAllocator()
{
	delete fs_al;
	delete co_al;
}

void MemoryAllocator::init()
{
	fs_al->init();
	co_al->init();
}

void MemoryAllocator::destroy()
{
	fs_al->destroy();
	co_al->destroy();
}

void* MemoryAllocator::alloc(size_t nbytes)
{
	void* result = nullptr;
	if (nbytes > BYTES_ON_SWITCH_ALLOCATOR)
		result = co_al->alloc(nbytes);
	else
		result = fs_al->alloc(nbytes);
	return result;
}

void MemoryAllocator::free(void* ptr)
{
	if (ptr)
	{
		char* p = static_cast<char*>(ptr) - ALIGN;
		size_t nbytes = *reinterpret_cast<size_t*>(p);
		nbytes &= 0x0000'ffff'ffff'ffff;
		if (nbytes > BYTES_ON_SWITCH_ALLOCATOR)
			co_al->free(ptr);
		else
			fs_al->free(ptr);
	}
}

void MemoryAllocator::dumpStat() const
{
	fs_al->dumpStat();
	co_al->dumpStat();
}

void MemoryAllocator::dumpBlocks() const
{
	fs_al->dumpBlocks();
	co_al->dumpBlocks();
}