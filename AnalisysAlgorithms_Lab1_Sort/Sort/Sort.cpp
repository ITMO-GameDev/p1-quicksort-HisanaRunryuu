#include "..\AnalisysAlgorithms_Lab1_Sort\stdafx.h"
#include <iostream>

// граница между применением сортировки вставками и быстрой
const int borderline = 15;

//template<typename T>
//bool comp(const T& a, const T& b);

template<typename T>
void swap(T a, T b)
{
	auto tmp = a;
	a = b;
	b = tmp;
}

template <typename T, typename Compare>
void sort(T *first, T *last, Compare comp) { // first,last - массив и его конец; указывается через тесткейсы
	while (first < last) {
		int size = last - first;
		// условие использование сорт.вставками
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
		for (j = i - 1; j >= 0 && comp(tmp, vec[j]); j--) // поиск места элемента в левой, отсортированной, части
			vec[j + 1] = vec[j];    // сдвигаем элемент направо, пока не дошли
		vec[j + 1] = tmp; // место найдено, вставить элемент  
	}
}

template <typename T, typename Compare>
void partition(T *first, T *last, Compare comp) {
	//поиск медианы
	T arr[3];
	T* middle = first + (last - first) / 2;
	arr[0] = *first;
	arr[1] = *middle;
	arr[2] = *last;
	insertion_sort(arr, arr + sizeof(arr), comp);
	auto pivot = arr[1];
	//quicksort
	T* i = first;
	T* j = last;
	while (i <= j) {
		while (comp(first[i], pivot)) i++;
		while (comp(pivot, first[j])) j--;
		if (i <= j) {
			swap(first[i], first[j]);
			i++;
			j--;
		}
	}
	//сравнение размера новых частей
	if ((i - first) < (last - j)) {
		sort(first, i, comp);
		first = j;
	}
	else {
		sort(j, last, comp);
		last = i;
	}

}


void main() {
	getchar();
}