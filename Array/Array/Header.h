#pragma once
#include <iostream>

template <typename T>
class Array final
{
	class Iterator
	{
		friend class Array<T>;
	public:
		Iterator(Array<T>& arr);
		const T& get() const;
		void set(const T& value);
		void insert(const T& value);
		void remove();
		void next();
		void prev();
		void toIndex(int index);
		bool hasNext() const;
		bool hasPrev() const;
	private:
		Array<T>& arr;
		int index;
	};

public:
	Array();
	Array(int capacity);
	~Array();
	void insert(const T& value);
	void insert(int index, const T& value);
	void remove(int index);
	int size();
	int capacity();
	const T& operator[](int index) const;
	T& operator[](int index);
	Iterator iterator() {
		Iterator iterator(*this);
		return iterator;
	}
	const Iterator iterator() const {
		Iterator iterator(*this);
		return iterator;
	}

private:
	T* arr;
	int countFilled; //counter of filled parts of array
	int arrayCapacity; //max current array's capacity
	void expandArray(); //expand array if needed
};



template <typename T>
Array<T>::Array() {
	arr = new T[8];
	countFilled = 0;
	arrayCapacity = 8;
}

template <typename T>
Array<T>::Array(int capacity) {
	arr = new T[capacity];
	countFilled = 0;
	arrayCapacity = capacity;
}

template <typename T>
Array<T>::~Array() {
	delete[] arr;
}

template <typename T>
void Array<T>::insert(const T& value) {
	if (countFilled == arrayCapacity) {
		expandArray();
	}
	arr[countFilled] = value;
	countFilled++;
}

template <typename T>
void Array<T>::insert(int index, const T& value) {
	if (index > countFilled) {
		throw std::out_of_range("Wrong index for insertion");
	}
	if (countFilled == arrayCapacity) {
		expandArray();
	}
	std::copy(arr + index, arr + countFilled, arr + index + 1);
	arr[index] = value;
	countFilled++;
}

template <typename T>
void Array<T>::remove(int index) {
	if (index > arrayCapacity) {
		throw std::out_of_range("Wrong index for removal");
	}
	if (countFilled > 0) {
		std::copy(arr + index + 1, arr + countFilled, arr + index);
		arr[countFilled - 1] = 0;
		countFilled--;
	}
}

template<typename T>
int Array<T>::size()
{
	return countFilled;
}

template<typename T>
int Array<T>::capacity()
{
	return arrayCapacity;
}

template <typename T>
T& Array<T>::operator[](int index) {

	if (index < 0 || index >= arrayCapacity) {
		throw std::out_of_range("Wrong index");
	}
	return arr[index];
}

template<typename T>
const T& Array<T>::operator[](int index) const
{
	if (index < 0 || index >= arrayCapacity) {
		throw std::out_of_range("Wrong index");
	}
	return arr[index];
}


template<typename T>
void Array<T>::expandArray()
{
	T* tmp = NULL;
	if (countFilled == arrayCapacity) {
		tmp = new T[arrayCapacity * 2];
		std::move(arr, arr + countFilled, tmp);
	}
	delete arr;
	arr = tmp;
	arrayCapacity = arrayCapacity * 2;
}


template<typename T>
Array<T>::Iterator::Iterator(Array<T>& arr) : arr(arr)
{
	index = 0;
}

template<typename T>
const T& Array<T>::Iterator::get() const
{
	return arr[index];
}

template<typename T>
void Array<T>::Iterator::set(const T& value)
{
	arr[index] = value;
}

template<typename T>
void Array<T>::Iterator::insert(const T& value)
{
	if (arr.countFilled < arr.arrayCapacity)
		arr.insert(index, value);
}

template<typename T>
void Array<T>::Iterator::remove()
{
	arr.remove(index);
}

template<typename T>
void Array<T>::Iterator::next()
{
	if (hasNext())
		index++;
}

template<typename T>
void Array<T>::Iterator::prev()
{
	if (hasPrev())
		index--;
}

template<typename T>
void Array<T>::Iterator::toIndex(int index)
{
	if (index < arr.countFilled)
		this->index = index;
}

template<typename T>
bool Array<T>::Iterator::hasNext() const
{
	return index < arr.countFilled - 1;
}

template<typename T>
bool Array<T>::Iterator::hasPrev() const
{
	return index > 0;
}

