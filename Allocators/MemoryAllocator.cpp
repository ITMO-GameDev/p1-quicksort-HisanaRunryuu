#include "MemoryAllocator.h"
#include <windows.h>
#include <cmath>

int MemoryAllocator::align(int size)
{
    auto value = pow(2, ceil(log2(size)));
    return value >=16 ? value : 16;
}

MemoryAllocator::MemoryAllocator()
{
    isInit = false;
	
}

MemoryAllocator::~MemoryAllocator()
{
    this->MemoryAllocator::destroy();
}

void MemoryAllocator::init()
{
    int start = 16;

    for (; start <= 512; start *= 2)
    {
        auto allocator = new FixedSizeAllocator(start);
        allocator->init();

        FSAallocators.insert_or_assign(start, allocator);
    }
    auto freelist = new FreeListAllocator(52428800, FreeListAllocator::PlacementPolicy::FIND_FIRST);
    freelist->init();
    FLallocators.insert_or_assign(0, freelist);

    isInit = true;
}

void* MemoryAllocator::alloc(int size)
{
    if (!isInit) return nullptr;
	if(size > 10485760)
	{
		return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}
	if( size > 512)
	{
        for (auto it = FLallocators.begin(); it != FLallocators.end(); ++it)
        {
            if (it->second->getFreeSize() > size)
            {
                auto mem =  it->second->allocate(size);
                std::cout << mem<<std::endl;
                dumpFLMap.insert_or_assign(mem, it->second);
                return mem;
         
            }
        }
        auto freelist = new FreeListAllocator(52428800, FreeListAllocator::PlacementPolicy::FIND_FIRST);
        freelist->init();
        FLallocators.insert_or_assign(FLallocators.size()+1, freelist);
        auto mem = freelist->allocate(size);
        dumpFLMap.insert_or_assign(mem, freelist);
        return mem;
		
	}
    auto alignedSize = align(size);
    auto allocator = FSAallocators[alignedSize];
    return  allocator->alloc();
}

void MemoryAllocator::free(void* ptr)
{
    if (!isInit) return;

	for (auto it=FSAallocators.begin(); it!=FSAallocators.end();++it)
	{
       if( it->second->free(ptr)) 
       return;
	}
    
    if(dumpFLMap.count(ptr))
    {
           
            dumpFLMap[ptr]->free(ptr);
            dumpFLMap.erase(ptr);
            return;
    }
    
 
    VirtualFree(ptr, 0, MEM_RELEASE);
}

void MemoryAllocator::destroy()
{
    if (!isInit) return;
    for (auto it = FSAallocators.begin(); it != FSAallocators.end(); ++it)
    {
        it->second->destroy();
    }
    for (auto it = FLallocators.begin(); it != FLallocators.end(); ++it)
    {
        it->second->destroy();
    }
    FSAallocators.clear();
    FLallocators.clear();
    dumpFLMap.clear();
    isInit = false;
}

void MemoryAllocator::dumpBlocks()
{
    if (!isInit) return;
    for (auto it = FSAallocators.begin(); it != FSAallocators.end(); ++it)
    {
        it->second->dumpBlocks();
    }
}


