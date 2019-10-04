#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Sort\Sort.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			const int length = 10;
			int arrayTest[length] = { 9,8,7,6,5,4,3,2,1,0 };
			int resultArray[length] = { 0,1,2,3,4,5,6,7,8,9 };
			auto lambda = [](int a, int b) { return a > b; };

			sort(arrayTest, arrayTest + length - 1, lambda);

			for (int i = 0; i < length; i++)
			{
				Assert::AreEqual(arrayTest[i], resultArray[i]);
			}
		}

	};
}