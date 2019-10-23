#include "stdafx.h"
#include <algorithm>
#include <random>
#include "CppUnitTest.h"
#include "../Sort/Sort.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLesson1
{
	TEST_CLASS(UnitTestSort)
	{
	public:
		TEST_METHOD(TestSwap)
		{
			int arrayTest[2] = { 1, 2 };
			int expectedArray[2] = {2, 1};

			swap(arrayTest, arrayTest + 1);
			for (int i = 0; i < 2; i++)
			{
				Assert::AreEqual(expectedArray[i], arrayTest[i]);
			}
		}

		TEST_METHOD(PivotTest) {
			const int length = 7;
			int arrayTest[length] = { 6, 5, 4, 3, 2, 1, 0 };
			int expectedArray[length] = { 0, 1, 2, 3, 4, 5, 6 };
			auto lambda = [](int a, int b) { return a < b; };

			int pivot = pivot_point(arrayTest, arrayTest + length - 1, lambda);
			Assert::AreEqual(3, pivot);
		}

		TEST_METHOD(EmptyArrayTest)
		{
			const int length = 1;
			int arrayTest[length] = {};
			int expectedArray[length] = {};
			auto lambda = [](int a, int b) { return a < b; };

			sort(arrayTest, arrayTest + length - 1, lambda);
			for (int i = 0; i < length; i++)
			{
				Assert::AreEqual(expectedArray[i], arrayTest[i]);
			}
		}
		
		TEST_METHOD(SingleElementTest)
		{
			int arrayTest[] = { 1 };
			auto lambda = [](int a, int b) { return a < b; };
			sort(arrayTest, arrayTest, lambda);
			Assert::AreEqual(1, arrayTest[0]);
		}
		
		TEST_METHOD(InsertTest_WorstCase) 
		{
			const int length = 7;
			int arrayTest[length] = { 6, 5, 4, 3, 2, 1, 0 };
			int expectedArray[length] = { 0, 1, 2, 3, 4, 5, 6 };
			auto lambda = [](int a, int b) { return a < b; };

			sort(arrayTest, arrayTest + length - 1, lambda);

			for (int i = 0; i < length; i++)
			{
				Assert::AreEqual(expectedArray[i], arrayTest[i]);
			}
		}

		TEST_METHOD(Insert_Float)
		{
			const int length = 7;
			float arrayTest[length] = { 1.6, 0.5, 2.1, 0.2, 0.0, 4.3, 3.4 };
			float expectedArray[length] = { 0.0, 0.2, 0.5, 1.6, 2.1, 3.4, 4.3 };
			auto lambda = [](int a, int b) { return a < b; };

			sort(arrayTest, arrayTest + length - 1, lambda);
			for (int i = 0; i < length; i++)
			{
				Assert::AreEqual(expectedArray[i], arrayTest[i]);
			}
		}

		TEST_METHOD(QuickSort) 
		{
			const int length = 30;
			auto lambda = [](int a, int b) { return a < b; };
			int arrayTest[length] = {};
			int expectedArray[length] = {};

			for (int i = 0; i < length; i++) {
				int randInt = rand() % 100;
				arrayTest[i] = randInt;
				expectedArray[i] = randInt;
			}
			std::sort(expectedArray, expectedArray + length);
			sort(arrayTest, arrayTest + length - 1, lambda);

			for (int i = 0; i < length; i++)
			{
				Assert::AreEqual(expectedArray[i], arrayTest[i]);
			}
		}

		TEST_METHOD(QuickSort_100elements)
		{
			const int length = 100;
			auto lambda = [](int a, int b) { return a < b; };
			int arrayTest[length] = {};
			int expectedArray[length] = {};

			for (int i = 0; i < length; i++) {
				int randInt = rand() % 100;
				arrayTest[i] = randInt;
				expectedArray[i] = randInt;
			}
			std::sort(expectedArray, expectedArray + length);
			sort(arrayTest, arrayTest + length -1, lambda);

			for (int i = 0; i < length; i++)
			{
				Assert::AreEqual(expectedArray[i], arrayTest[i]);
			}
		}
	};
}