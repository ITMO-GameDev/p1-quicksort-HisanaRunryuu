// MemoryManagement.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "Allocator.h"

int main()
{
	MemoryAllocator allocator;
	allocator.init();
	void* ptr = allocator.alloc(16);

	return 0;
}

