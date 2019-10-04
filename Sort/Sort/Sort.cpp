#include "stdafx.h"
#include <iostream>
//#include "Sort.h"

//namespace Sorting {
	// ������� ����� ����������� ���������� ��������� � �������
	const int borderline = 15;

	template<typename T>
	bool comp(T a, T b)
	{
		return a > b;
	}

	template<typename T>
	void swap(T a, T b)
	{
		auto tmp = a;
		a = b;
		b = tmp;
	}

	template <typename T, typename Compare>
	void sort(T *first, T *last, Compare comp) { // first,last - ������ � ��� �����; ����������� ����� ���������
		while (first < last) {
			int size = last - first;
			// ������� ������������� ����.���������
			if (size <= borderline) {
				insertion_sort(first, last, comp);
				//return;
			}

			partition(first, last, comp);


		}

	}


	template <typename T, typename Compare>
	void insertion_sort(T *vec, T *last, Compare comp) {
		int length = last - vec + 1;
		if (length <= 1) return;

		T tmp;
		for (int i = 1, j; i < length; i++) {
			tmp = vec[i];
			for (j = i - 1; j >= 0 && comp(tmp, vec[j]); j--) // ����� ����� �������� � �����, ���������������, �����
				vec[j + 1] = vec[j];    // �������� ������� �������, ���� �� �����
			vec[j + 1] = tmp; // ����� �������, �������� �������  
		}
	}

	template <typename T, typename Compare>
	void partition(T *first, T *last, Compare comp) {
		//����� �������
		T arr[3];
		T* middle = first + (last - first) / 2;
		arr[0] = *first;
		arr[1] = *middle;
		arr[2] = *last;
		insertion_sort(arr, arr + (std::size(arr)-1), comp);
		T pivot = arr[1];
		//quicksort
		T* left = first;
		T* right = last;
		while (left <= right) {
			while (comp(left[0], pivot))
				left++;
			while (comp(pivot, right[0])) right--;
			if (left <= right) {
				swap(left, right);
				left++;
				right--;
			}
		}
		//��������� ������� ����� ������
		if ((left - first) < (last - right)) {
			sort(first, left, comp);
			first = right;
		}
		else {
			sort(right, last, comp);
			last = left;
		}

	}


	int main() {
		const int length = 10;
		int arrayTest[length] = { 9,8,7,6,5,4,3,2,1,0 };
		int resultArray[length] = { 0,1,2,3,4,5,6,7,8,9 };
		auto lambda = [](int a, int b) { return a > b; };

		sort(arrayTest, arrayTest + length - 1, lambda);

		getchar();
		return 0;
	}
//}