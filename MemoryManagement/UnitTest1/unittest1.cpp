#include "stdafx.h"
#include "CppUnitTest.h"
#include "../MemoryManagement/Allocator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			MemoryAllocator allocator;
			allocator.init();
			void* ptr = allocator.alloc(16);

			Assert::AreEqual(0, (int)ptr % 8);
		}

		TEST_METHOD(TestMethod2)
		{
			MemoryAllocator allocator;
			allocator.init();
			void* ptr = allocator.alloc(32);

			Assert::AreEqual(0, (int)ptr % 8);
		}

		TEST_METHOD(TestMethod3)
		{
			MemoryAllocator allocator;
			allocator.init();
			void* ptr = allocator.alloc(64);

			Assert::AreEqual(0, (int)ptr % 8);
		}

		TEST_METHOD(TestMethod4)
		{
			MemoryAllocator allocator;
			allocator.init();
			void* ptr = allocator.alloc(512);

			Assert::AreEqual(0, (int)ptr % 8);
		}

		TEST_METHOD(TestMethod5)
		{
			MemoryAllocator allocator;
			allocator.init();
			void* ptr = allocator.alloc(1000 * sizeof(int));

			Assert::AreEqual(0, (int)ptr % 8);
		}

	};
}