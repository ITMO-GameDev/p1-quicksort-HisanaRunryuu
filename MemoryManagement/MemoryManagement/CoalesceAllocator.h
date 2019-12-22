#pragma once

class CoaleseAllocator
{
	enum { ALIGN = 8 };
	enum { MIN_BYTES = 512 };
	enum { MAX_BYTES = 10 * 1024 * 1024 };
	enum { CHUNK_SIZE = 1024 * 1024 - 4 * sizeof(void*) };

public:
	CoaleseAllocator() = default;

	~CoaleseAllocator()
	{
		assert(state != State::Destroyed);
		destroy();
	}

	CoaleseAllocator(const CoaleseAllocator&) = delete;
	CoaleseAllocator& operator=(const CoaleseAllocator&) = delete;

	void init()
	{
		assert(state != State::Initialized);

		reserveBlock();

#ifndef NDEBUG
		state = State::Initialized;
#endif
	}

	void destroy();

	void* alloc(size_t nbytes);
	void free(void* ptr);

#ifndef NDEBUG
	void dumpStat() const;
	void dumpBlocks() const;
#endif

private:
	struct Header;
	struct BlockList
	{
		void* chunk;
		BlockList* next;
		Header* first;
		size_t size;

		Header* getFirstRecord() const
		{
			char* ptr = static_cast<char*>(chunk) + sizeof(BlockList);
			return reinterpret_cast<Header*>(ptr);
		}
	};

	struct Header
	{
		Header* next_or_first;
		uint64_t size;

		void init(uint64_t sz)
		{
			size = 0xdead;
			size <<= 48;
			size |= sz;
		}

		uint64_t getSize(bool drop_busy = true) const
		{
			if (drop_busy)
				return size & 0x0000'ffff'ffff'fff8;
			return size & 0x0000'ffff'ffff'ffff;
		}

		void aquire() { size |= 1; }

		void release() { size &= ~1; }

		bool isBusy() const { return size & 1; }

		bool isValid() const { return ((size >> 48) & 0xffff) == 0xdead; }

		Header* nextHeader()
		{
			char* p = reinterpret_cast<char*>(this) + align(sizeof(Header));
			p += getSize();
			return reinterpret_cast<Header*>(p);
		}

		void coalese(Header* rhs)
		{
			size_t coalese_size = (size_t)(getSize() + align(sizeof(Header)) + rhs->getSize());
			init(coalese_size);
			next_or_first = rhs->next_or_first;
		}
	};

	template<typename T, uint8_t Align = ALIGN>
	static T align(T val) { return (val + Align - 1) & ~static_cast<T>(Align - 1); }

	void reserveBlock(size_t nbytes = CHUNK_SIZE);

	BlockList* block_list = nullptr;

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
