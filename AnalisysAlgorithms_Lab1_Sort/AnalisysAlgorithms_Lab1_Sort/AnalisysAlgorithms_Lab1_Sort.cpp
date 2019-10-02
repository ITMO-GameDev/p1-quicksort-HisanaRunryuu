#include "stdafx.h"
#include <iostream>
#include <vector>

// граница между применением сортировки вставками и быстрой
#define CUT 10

template<typename T>
void swap(T& a, T& b)
{
	auto tmp = a;
	a = b;
	b = tmp;
}

template <typename T, typename Compare>
void sort(vector<T> &vec, int first, int last, Compare comp) { // first,last - индексы
	while (first < last) {
		int size = last - first;
		// условие использование сорт.вставками
		if (size <= CUT) {
			insertion_sort(vec, first, last, comp);
			return;
		}
		
		partition(vec, first, last, comp);


	}

}


template <typename T, typename Compare>
void insertion_sort(vector<T> &vec, int first, int last, Compare comp) {

}

template <typename T, typename Compare>
int pivot_point(vector<T> &vec, int first, int middle, int last, Compare comp) {

	return 0;
}

template <typename T, typename Compare>
void partition(vector<T> &vec, int first, int last, Compare comp) {
	//поиск медианы
	//int x = pivot_point(vec, first, first + (last-first)/2, last, comp);
	int x = first + (last-first) / 2; // ТЕСТОВОЕ, НА ЗАМЕНУ!!
	int i = first;
	int j = last;

	while (i <= j) {
		while (vec[i] < x) i++;
		while (vec[j] > x) j--;
		if (i <= j) {
			swap(vec[i], vec[j]);
			i++;
			j--;
		}
	}

}




//#include "stdafx.h"
//#include <iostream>
//#include <iomanip>
//#include <cstdlib>
////using namespace std;
//
//template <typename T>
//inline void swap(T& v1, T& v2)
//{
//	T temp = v2;
//	v2 = v1;
//	v1 = temp;
//}
//
//template <typename T>
//void insertsort(T *array, int size) {
//	T temp;
//	int i;
//	for (int j = 1; j<size; j++) {
//		temp = array[j];
//		i = j - 1;
//		while (i >= 0 && Array[i]>temp) {
//			array[i + 1] = array[i];
//			i = i - 1;
//		}
//		array[i + 1] = temp;
//	}
//}
//
//template <typename T>
//void quicksort(T *array, int hi, int lo = 0)
//{
//	while (hi>lo)
//	{
//		int i = lo;
//		int j = hi;
//		do
//		{
//			while (array[i]<array[lo] && i<j)
//				i++;
//			while (array[--j]>array[lo])
//				;
//			if (i<j)
//				swap(array[i], array[j]);
//		} while (i<j);
//		swap(array[lo], array[j]);
//
//		if (j - lo>hi - (j + 1)) {
//			quicksort(array, j - 1, lo);
//			lo = j + 1;
//		}
//		else {
//			quicksort(array, hi, j + 1);
//			hi = j - 1;
//		}
//	}
//}
//int main()
//{
//	int dim = 100;
//
//	int *arrs = new int[dim + 1];
//
//	for (int i = 0; i < dim; i++)
//		arrs[i] = rand();
//	std::cout << std::endl << "unsorted" << std::endl;
//
//	for (int i = 0; i<dim; i++)
//		std::cout << std::setw(8) << arrs[i];
//	quicksort(arrs, dim);
//
//	std::cout << std::endl << "sorted" << std::endl;
//	for (int i = 0; i<dim; i++)
//		std::cout << std::setw(8) << arrs[i];
//	delete arrs;
//	
//	system("PAUSE");
//	return 0;
//}