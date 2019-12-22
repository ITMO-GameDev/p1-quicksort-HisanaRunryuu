#include "stdafx.h"
#include <cassert>
#include "Allocator.h"
#include "CoalesceAllocator.h"

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

void CoaleseAllocator::destroy()
{
	assert(state != State::Destroyed);

	while (block_list)
	{
		BlockList* block = block_list;
#ifndef NDEBUG
		Header* record = block->getFirstRecord();
		size_t size = align(sizeof(Header));
		while (record && size < block->size)
		{
			size += (size_t)record->getSize();
			if (record->isBusy())
			{
				std::cout << "LEAK: at address="
					<< (void*)(reinterpret_cast<char*>(record) + align(sizeof(Header)))
					<< " of size=" << record->getSize() << '\n';
			}
			record = record->nextHeader();
		}
#endif 
		block_list = block->next;
		::operator delete(block->chunk);
	}

#ifndef NDEBUG
	state = State::Destroyed;
#endif
}

void* CoaleseAllocator::alloc(size_t nbytes)
{
	assert(state == State::Initialized);

	char* result = nullptr;
	if (nbytes > MAX_BYTES)
	{
		size_t alloc_size = align(sizeof(Header)) + nbytes;
		result = static_cast<char*>(::operator new(alloc_size));
		Header* record = reinterpret_cast<Header*>(result);
		record->init(nbytes);
		result += align(sizeof(Header));
	}
	else
	{
		BlockList* block = block_list;
		Header* record = nullptr;
		Header* prev_record = nullptr;
		while (block)
		{
			record = block->first;
			while (record && (size_t)record->getSize() < nbytes)
			{
				prev_record = record;
				record = record->next_or_first;
			}
			if (record)
				break;
			block = block->next;
		}
		if (!record)
		{
			size_t block_size = align(sizeof(BlockList)) + align(sizeof(Header)) + nbytes;
			block_size = block_size > CHUNK_SIZE ? block_size : CHUNK_SIZE;
			reserveBlock(block_size);
			block = block_list;
			record = block->getFirstRecord();
			prev_record = nullptr;
		}

		size_t size_avail = (size_t)record->getSize();
		size_t result_size = size_avail;
		Header* next_record = nullptr;
		if (size_avail - nbytes < MIN_BYTES)
		{
			if (prev_record)
				prev_record->next_or_first = record->next_or_first;
			else
				block->first = record->next_or_first;
		}
		else
		{
			result_size = align(nbytes);
			next_record = (Header*)((char*)(record)+align(sizeof(Header)) + result_size);
			next_record->init(size_avail - result_size);
			next_record->next_or_first = record->next_or_first;

			if (prev_record)
				prev_record->next_or_first = next_record;
			else
				block->first = next_record;
		}

		record->next_or_first = block->getFirstRecord();
		record->init(result_size);
		record->aquire();
		result = reinterpret_cast<char*>(record) + align(sizeof(Header));
	}
	return static_cast<void*>(result);
}

void CoaleseAllocator::free(void* ptr)
{
	assert(state == State::Initialized);

	if (ptr)
	{
		char* p = static_cast<char*>(ptr) - align(sizeof(Header));
		Header* record = reinterpret_cast<Header*>(p);

		assert(record->isValid());

		size_t nbytes = (size_t)record->getSize(false);
		if (nbytes > MAX_BYTES)
		{
			::operator delete(p);
		}
		else
		{
			assert(record->isBusy());

			record->release();

			p = reinterpret_cast<char*>(record->next_or_first) - align(sizeof(BlockList));
			BlockList* block = reinterpret_cast<BlockList*>(p);

			if (!block->first)
			{
				record->next_or_first = block->first;
				block->first = record;
			}
			else if (record < block->first)
			{
				if (record->nextHeader() == block->first)
					record->coalese(block->first);
				else
					record->next_or_first = block->first;
				block->first = record;
			}
			else
			{
				Header* prev_record = block->first;
				Header* next_record = prev_record->next_or_first;
				while (next_record && next_record < record)
				{
					prev_record = next_record;
					next_record = next_record->next_or_first;
				}

				if (next_record && record->nextHeader() == next_record)
					record->coalese(next_record);
				else
					record->next_or_first = next_record;

				if (prev_record->nextHeader() == record)
					prev_record->coalese(record);
				else
					prev_record->next_or_first = record;
			}
		}
	}
}
void CoaleseAllocator::reserveBlock(size_t nbytes)
{
	void* chunk = ::operator new(nbytes);
	BlockList* block = static_cast<BlockList*>(chunk);
	block->chunk = chunk;

	char* ptr = static_cast<char*>(chunk) + align(sizeof(BlockList));
	Header* record = reinterpret_cast<Header*>(ptr);
	record->next_or_first = nullptr;
	nbytes -= align(sizeof(BlockList));
	block->size = nbytes;
	nbytes -= align(sizeof(Header));
	record->init(nbytes);
	block->first = record;

	block->next = block_list;
	block_list = block;
}

#ifndef NDEBUG


void CoaleseAllocator::dumpStat() const
{
	assert(state == State::Initialized);

	std::cout << "\n\nCoalese:\n";
	size_t total_reserved = 0;
	size_t total_used_mem = 0;
	size_t total_mem = 0;

	BlockList* block = block_list;
	while (block)
	{
		size_t block_used = 0;
		size_t block_free = 0;
		size_t block_used_mem = 0;
		size_t block_free_mem = 0;

		Header* record = block->getFirstRecord();
		size_t size = align(sizeof(Header));
		while (record && size < block->size)
		{
			size += (size_t)record->getSize();
			if (record->isBusy())
			{
				++block_used;
				block_used_mem += (size_t)record->getSize();
			}
			else
			{
				++block_free;
				block_free_mem += (size_t)record->getSize();
			}
			record = record->nextHeader();
		}
		++total_reserved;
		total_used_mem += block_used_mem;
		total_mem += block->size;

		std::cout << "Block at address=" << (void*)block->chunk << ":\n";
		std::cout << "  Allocated Num=" << block_used << " parts of total Size=" << block_used_mem << '\n';
		std::cout << "  Free Num=" << block_free << " parts of total Size=" << block_free_mem << '\n';

		block = block->next;
	}
	std::cout << "Total blocks reserved: " << total_reserved << '\n';
	std::cout << "Total allocated: " << total_used_mem << " of " << total_mem << " bytes\n\n";
}

void CoaleseAllocator::dumpBlocks() const
{
	assert(state == State::Initialized);

	std::cout << "Coalese:\nUser allocated blocks:\n";
	std::cout << "|     Address      |        Size      |\n";
	BlockList* block = block_list;
	while (block)
	{
		Header* record = block->getFirstRecord();
		size_t header_size = align(sizeof(Header));
		size_t size = header_size;
		while (record && size < block->size)
		{
			size += (size_t)record->getSize();
			if (record->isBusy())
			{
				std::cout << "| " << std::setw(16) << std::left
					<< (void*)(reinterpret_cast<const char*>(record) + header_size)
					<< " | " << std::setw(16) << std::left << record->getSize() << " |\n";
			}
			record = record->nextHeader();
		}
		block = block->next;
	}
}
#endif