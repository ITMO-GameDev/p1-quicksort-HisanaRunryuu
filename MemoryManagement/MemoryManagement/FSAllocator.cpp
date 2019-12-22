#include "stdafx.h"
#include <cassert>
#include "Allocator.h"
#include "FSAllocator.h"

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif


void FSAllocator::init()
{
	assert(state != State::Initialized);

	bucket_map = ::new BucketMap[MAX_BYTES + 1];
	BucketMap* pbucket_type = bucket_map;
	BucketMap bucket_type_max = MIN_BYTES;
	BucketMap bucket_type = 0;
	for (BucketMap b = 0; b <= MAX_BYTES; ++b)
	{
		if (b > bucket_type_max)
		{
			bucket_type_max <<= 1;
			++bucket_type;
		}
		*pbucket_type++ = bucket_type;
	}
	buckets = ::new Bucket[BUCKETS_COUNT];

#ifndef NDEBUG
	state = State::Initialized;
#endif
}

void FSAllocator::destroy()
{
	assert(state != State::Destroyed);

	if (buckets)
	{
		for (BucketMap i = 0; i < BUCKETS_COUNT; ++i)
		{
#ifndef NDEBUG
			const BucketMap bucket_type_size = (MIN_BYTES << i) + ALIGN;
			const size_t bucket_type_count = (CHUNK_SIZE - sizeof(BlockList)) / bucket_type_size;
#endif
			Bucket& bucket = buckets[i];
			while (bucket.address)
			{
				BlockList* block = bucket.address;
#ifndef NDEBUG
				char* ptr = static_cast<char*>(block->chunk) + sizeof(BlockList);
				for (size_t j = 0; j < bucket_type_count; ++j)
				{
					const FreeList* record = reinterpret_cast<const FreeList*>(ptr);
					if (record->header.isValid())
					{
						std::cout << "LEAK: at Addr=" << (void*)(reinterpret_cast<const char*>(record) + ALIGN)
							<< " FSABlkSz=" << (MIN_BYTES << i)
							<< " UserReqSz=" << record->header.getSize() << "\n";
					}
					ptr += bucket_type_size;
				}
#endif
				bucket.address = block->next;
				::operator delete(block->chunk);
			}
		}
		::delete[] buckets;
		::delete[] bucket_map;
	}
#ifndef NDEBUG
	state = State::Destroyed;
#endif
}

void* FSAllocator::alloc(size_t nbytes)
{
	assert(state == State::Initialized);

	char* result = nullptr;
	if (nbytes > MAX_BYTES)
	{
		result = static_cast<char*>(::operator new(nbytes + ALIGN));
		FreeList* record = reinterpret_cast<FreeList*>(result);
		record->header.init(nbytes);
		result += ALIGN;
	}
	else if (nbytes > 0)
	{
		FreeList* record;
		BucketMap which = bucket_map[nbytes];
		Bucket& bucket = buckets[which];
		if (!bucket.first)
		{
			const BucketMap bucket_type_size = (MIN_BYTES << which) + ALIGN;
			size_t bucket_type_count = (CHUNK_SIZE - sizeof(BlockList)) / bucket_type_size;

			void* chunk = ::operator new(CHUNK_SIZE);
			BlockList* block = static_cast<BlockList*>(chunk);
			block->chunk = chunk;
			block->next = bucket.address;
			bucket.address = block;

			char* ptr = static_cast<char*>(chunk) + sizeof(BlockList);
			record = reinterpret_cast<FreeList*>(ptr);
			bucket.first = record;
			while (--bucket_type_count > 0)
			{
				ptr += bucket_type_size;
				record->next = reinterpret_cast<FreeList*>(ptr);
				record = record->next;
			}
			record->next = 0;
		}
		record = bucket.first;
		bucket.first = record->next;

		record->header.init(nbytes);
		result = reinterpret_cast<char*>(record) + ALIGN;
	}
	return static_cast<void*>(result);
}

void FSAllocator::free(void* ptr)
{
	assert(state == State::Initialized);

	if (ptr)
	{
		char* p = static_cast<char*>(ptr) - ALIGN;
		FreeList* record = reinterpret_cast<FreeList*>(p);

		assert(record->header.isValid());

		auto nbytes = record->header.getSize();
		record->header.release();

		if (nbytes > MAX_BYTES)
		{
			::operator delete(p);
		}
		else
		{
			BucketMap which = bucket_map[nbytes];
			Bucket& bucket = buckets[which];
			FreeList* block = reinterpret_cast<FreeList*>(p);

			block->next = bucket.first;
			bucket.first = block;
		}
	}
}


#ifndef NDEBUG

void FSAllocator::dumpStat() const
{
	assert(state == State::Initialized);

	std::cout << "\n\nFSA:\n";
	size_t total_reserved = 0;
	size_t total_used = 0;
	size_t total_mem = 0;
	for (BucketMap i = 0; i < BUCKETS_COUNT; ++i)
	{
		const BucketMap fsa_size = MIN_BYTES << i;
		const BucketMap bucket_type_size = fsa_size + ALIGN;
		const size_t bucket_type_count = (CHUNK_SIZE - sizeof(BlockList)) / bucket_type_size;
		const BlockList* block = buckets[i].address;

		size_t reserved = 0;
		size_t used = 0;
		while (block)
		{
			++reserved;
			const char* ptr = static_cast<const char*>(block->chunk) + sizeof(BlockList);
			for (size_t j = 0; j < bucket_type_count; ++j)
			{
				const FreeList* record = reinterpret_cast<const FreeList*>(ptr);
				if (record->header.isValid())
					++used;
				ptr += bucket_type_size;
			}
			block = block->next;
		}
		total_reserved += reserved;
		total_used += used;
		total_mem += used * fsa_size;

		if (reserved)
		{
			std::cout << "  Buckets for " << fsa_size << "-bytes blocks reserved: " << reserved << '\n';
			std::cout << "    Blocks allocated: " << used << '\n';
		}
	}
	std::cout << "Total buckets reserved: " << total_reserved << '\n';
	std::cout << "Total allocated: " << total_used << " blocks in " << total_mem << " bytes\n\n\n";
}

void FSAllocator::dumpBlocks() const
{
	assert(state == State::Initialized);

	std::cout << "FSA:\nUser allocated blocks:\n";
	std::cout << "| Address of block |    Block Size    |  Requested size  |\n";
	for (BucketMap i = 0; i < BUCKETS_COUNT; ++i)
	{
		const BucketMap fsa_size = MIN_BYTES << i;
		const BucketMap bucket_type_size = fsa_size + ALIGN;
		const size_t bucket_type_count = (CHUNK_SIZE - sizeof(BlockList)) / bucket_type_size;
		const BlockList* block = buckets[i].address;

		while (block)
		{
			const char* ptr = static_cast<const char*>(block->chunk) + sizeof(BlockList);
			for (size_t j = 0; j < bucket_type_count; ++j)
			{
				const FreeList* record = reinterpret_cast<const FreeList*>(ptr);
				if (record->header.isValid())
				{
					std::cout << "| " << std::setw(16) << std::left
						<< (void*)(reinterpret_cast<const char*>(record) + ALIGN)
						<< " | " << std::setw(16) << std::left << fsa_size
						<< " | " << std::setw(16) << std::left << record->header.getSize() << "|\n";
				}
				ptr += bucket_type_size;
			}
			block = block->next;
		}
	}
}
#endif

