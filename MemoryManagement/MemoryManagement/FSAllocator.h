#pragma once

class FSAllocator
{
	enum { ALIGN = 8 };
	enum { MIN_BYTES = 16 };
	enum { MAX_BYTES = 512 };
	enum { BUCKETS_COUNT = 6 };
	enum { CHUNK_SIZE = 4096 - 4 * sizeof(void*) };

public:
	FSAllocator() = default;

	~FSAllocator()
	{
		assert(state != State::Destroyed);
		destroy();
	}

	FSAllocator(const FSAllocator&) = delete;
	FSAllocator& operator=(const FSAllocator&) = delete;

	void init();
	void destroy();

	void* alloc(size_t);
	void free(void*);

#ifndef NDEBUG
	void dumpStat() const;
	void dumpBlocks() const;
#endif

private:
	using BucketMap = unsigned short;

	struct BlockList
	{
		void* chunk;
		BlockList* next;
	};

	union FreeList
	{
		FreeList* next;

		struct Header
		{
			uint64_t size;

			void init(uint64_t sz)
			{
				size = 0xdead;
				size <<= 48;
				size |= sz;
			}

			uint64_t getSize() const { return size & 0x0000'ffff'ffff'ffff; }

			void release() { size = 0; }

			bool isValid() const { return ((size >> 48) & 0xffff) == 0xdead; }
		} header;
	};

	struct Bucket
	{
		FreeList* first = nullptr;
		BlockList* address = nullptr;
	};

	BucketMap* bucket_map = nullptr;
	Bucket* buckets = nullptr;

#ifndef NDEBUG
	enum class State
	{
		NotInitialized,
		Initialized,
		Destroyed
	};
	State state = State::NotInitialized;
#endif
};
