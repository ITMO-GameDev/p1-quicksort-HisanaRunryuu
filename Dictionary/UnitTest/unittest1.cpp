#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Dictionary/Dictionary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(InitTest)
		{
			Dictionary<int, std::string> tmp;
			Assert::AreEqual(0, tmp.size());
		}

		TEST_METHOD(InputTest)
		{
			Dictionary<int, std::string> tmp;
			for (auto i = 0; i<5; i++)
			{
				tmp.put(i, std::to_string(i));
			}
			Assert::AreEqual(5, tmp.size());
		}

		TEST_METHOD(ContainsTest)
		{
			Dictionary<int, std::string> tmp;
			for (auto i = 0; i<5; i++)
			{
				tmp.put(i, std::to_string(i));
			}

			for (auto i = 0; i<5; i++)
			{
				Assert::IsTrue(tmp.contains(i));
			}
			Assert::AreEqual(5, tmp.size());
			Assert::IsFalse(tmp.contains(6));
		}

		TEST_METHOD(RemoveTest)
		{
			Dictionary<int, std::string> tmp;
			for (auto i = 0; i<5; i++)
			{
				tmp.put(i, std::to_string(i));
			}
			for (auto i = 0; i < 3; i++)
			{
				tmp.remove(i);
			}

			for (auto i = 0; i<3; i++)
			{
				Assert::IsFalse(tmp.contains(i));
			}

			for (auto i = 4; i<5; i++)
			{
				Assert::IsTrue(tmp.contains(i));
			}

			Assert::AreEqual(2, tmp.size());			
		}

		// ÏÅÐÅÏÈÑÀÒÜ

		TEST_METHOD(OperatorTest)
		{
			Dictionary<std::string, int> tmp;
			for (auto i = 0; i < 100; i++)
			{
				tmp[std::to_string(i)] = i;
			}
			for (auto i = 0; i < 100; i++)
			{
				Assert::IsTrue(tmp.contains(std::to_string(i)));
				Assert::IsTrue(tmp[std::to_string(i)] == i);
			}
			Assert::AreEqual(100, tmp.size());
		}


		TEST_METHOD(IteratorForwardTest)
		{
			Dictionary<std::string, int> tmp;
			for (auto i = 0; i < 100; i++)
			{
				tmp[std::to_string(i)] = i;
			}
			auto i = 1;
			for (auto it = tmp.iterator(); it.hasNext(); it.next())
			{
				Assert::IsTrue(tmp[it.key()] == it.get());
				i++;
			}
			Assert::AreEqual(i, tmp.size());
		}


		TEST_METHOD(IteratorForwardBackwardTest)
		{
			Dictionary<std::string, int> tmp;
			for (auto i = 0; i < 100; i++)
			{
				tmp[std::to_string(i)] = i;
			}
			auto i = 1;
			auto it = tmp.iterator();
			for (it; it.hasNext(); it.next())
			{
				Assert::IsTrue(tmp[it.key()] == it.get());
			}
			for (auto it = tmp.iterator(); it.hasNext(); it.next())
			{
				Assert::IsTrue(tmp[it.key()] == it.get());
				i++;
			}
			Assert::AreEqual(i, tmp.size());
		}


		TEST_METHOD(IteratorSetTest)
		{
			Dictionary<std::string, int> tmp;
			for (auto i = 0; i < 100; i++)
			{
				tmp[std::to_string(i)] = i;
			}
			auto it = tmp.iterator();
			for (it; it.hasNext(); it.next())
			{
				it.set(42);
			}
			it.set(42);
			for (auto i = 0; i < 100; i++)
			{
				Assert::AreEqual(42, tmp[std::to_string(i)]);
			}
		}

	};
}