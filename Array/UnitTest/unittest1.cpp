#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Array/Header.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTestArray)
	{
	public:
		
		TEST_METHOD(initTest)
		{
			Array<int> tmp;
			Assert::AreEqual(0, tmp.size());
			Assert::AreEqual(8, tmp.capacity());
		}

		TEST_METHOD(initTest_size)
		{
			Array<int> tmp(20);
			Assert::AreEqual(0, tmp.size());
			Assert::AreEqual(20, tmp.capacity());
		}

		TEST_METHOD(insertInEmpty)
		{
			Array<int> tmp;
			tmp.insert(2);
			Assert::AreEqual(2, tmp[0]);
			tmp.insert(5);
			Assert::AreEqual(5, tmp[1]);

			Assert::AreEqual(2, tmp.size());
		}

		TEST_METHOD(insertInFull)
		{
			Array<int> tmp(1);
			tmp.insert(2);
			Assert::AreEqual(2, tmp[0]);
			tmp.insert(5);
			Assert::AreEqual(5, tmp[1]);

			Assert::AreEqual(2, tmp.size());
		}

		TEST_METHOD(insertInEmptyPlace)
		{
			Array<int> tmp(3);
			tmp.insert(2);
			Assert::AreEqual(2, tmp[0]);
			tmp.insert(1, 5);
			Assert::AreEqual(5, tmp[1]);


			Assert::AreEqual(2, tmp.size());
		}

		TEST_METHOD(insertInTakenPlace)
		{
			Array<int> tmp(3);
			tmp.insert(2);
			Assert::AreEqual(2, tmp[0]);
			tmp.insert(5);
			Assert::AreEqual(5, tmp[1]);
			tmp.insert(1, 10);
			Assert::AreEqual(10, tmp[1]);
			Assert::AreEqual(5, tmp[2]);

			Assert::AreEqual(3, tmp.size());
		}

		TEST_METHOD(insertInPlace_Full)
		{
			Array<int> tmp(2);
			tmp.insert(2);
			Assert::AreEqual(2, tmp[0]);
			tmp.insert(5);
			Assert::AreEqual(5, tmp[1]);
			tmp.insert(1, 10);
			Assert::AreEqual(10, tmp[1]);
			Assert::AreEqual(5, tmp[2]);

			Assert::AreEqual(3, tmp.size());
		}

		TEST_METHOD(removeLast)
		{
			Array<int> tmp(2);
			tmp.insert(2);
			tmp.insert(5);
			Assert::AreEqual(2, tmp.size());
			tmp.remove(1);
			Assert::AreEqual(1, tmp.size());
		}

		TEST_METHOD(removeMiddle)
		{
			Array<int> tmp(2);
			tmp.insert(2);
			Assert::AreEqual(2, tmp[0]);
			tmp.insert(5);
			Assert::AreEqual(5, tmp[1]);
			tmp.insert(10);
			Assert::AreEqual(10, tmp[2]);

			tmp.remove(1);

			Assert::AreEqual(2, tmp.size());
			Assert::AreEqual(10, tmp[1]);
		}


		TEST_METHOD(iterInitTest)
		{			
			Array<int> tmp;
			for (int i = 0; i < 3; ++i)
				tmp.insert(i);
			auto it = tmp.iterator();

			Assert::AreEqual(0, it.get());
			Assert::IsFalse(it.hasPrev());			
		}

		TEST_METHOD(iterNextEmpty)
		{
			Array<int> tmp;
			tmp.insert(2);
			auto it = tmp.iterator();
			Assert::IsFalse(it.hasNext());
		}

		TEST_METHOD(iterPrevEmpty)
		{
			Array<int> tmp;
			tmp.insert(2);
			auto it = tmp.iterator();
			Assert::IsFalse(it.hasPrev());
		}

		TEST_METHOD(iterNext)
		{
			Array<int> tmp;
			tmp.insert(2);
			tmp.insert(5);
			auto it = tmp.iterator();
			Assert::IsTrue(it.hasNext());
		}

		TEST_METHOD(iterPrev)
		{
			Array<int> tmp;
			tmp.insert(2);
			tmp.insert(5);
			auto it = tmp.iterator();
			it.next();
			Assert::IsTrue(it.hasPrev());
		}

		TEST_METHOD(iterMoveTooFar)
		{
			Array<int> tmp;
			tmp.insert(2);
			tmp.insert(5);
			auto it = tmp.iterator();
			it.next();
			it.next();
			Assert::AreEqual(5, it.get());
		}

		TEST_METHOD(iterMoveTooFarBack)
		{
			Array<int> tmp;
			tmp.insert(2);
			tmp.insert(5);
			auto it = tmp.iterator();
			it.next();
			it.prev();
			it.prev();
			Assert::AreEqual(2, it.get());
		}

		TEST_METHOD(iterMoveToIndex)
		{
			Array<int> tmp;
			for (int i = 0; i < 3; ++i)
				tmp.insert(i);
			auto it = tmp.iterator();
			it.toIndex(1);
			Assert::AreEqual(1, it.get());
			it.next();
			Assert::AreEqual(2, it.get());
			Assert::IsFalse(it.hasNext());
		}

		TEST_METHOD(iterInsert)
		{
			Array<int> tmp;
			for (int i = 0; i < 2; ++i)
				tmp.insert(i);
			auto it = tmp.iterator();
			it.insert(10);
			Assert::AreEqual(10, it.get());
			Assert::AreEqual(3, tmp.size());
		}

		TEST_METHOD(iterRemove)
		{
			Array<int> tmp;
			for (int i = 0; i < 2; ++i)
				tmp.insert(i);
			auto it = tmp.iterator();
			it.remove();
			Assert::AreEqual(1, it.get());
			Assert::AreEqual(1, tmp.size());
		}
	};
}