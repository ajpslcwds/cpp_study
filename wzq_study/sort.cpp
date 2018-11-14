#include "sort.h"

void print_arr(int arr[], int cnt)
{
	for (int i = 0; i < cnt; i++)
	{
		cout << arr[i] << ",";
	}
	cout << endl;
}

void bubble_sort(int arr[], int cnt)
{
	cout << "bubble_sort begin:";
	print_arr(arr, cnt);

	int temp = 0;
	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < cnt - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}

	cout << "bubble_sort end:";
	print_arr(arr, cnt);
}

void selection_sort(int arr[], int cnt)
{
	cout << "selection_sort begin:";
	print_arr(arr, cnt);

	int temp = 0;
	int k = 0;
	for (int i = 0; i < cnt; i++)
	{
		k = i;
		for (int j = i + 1; j < cnt; j++)
		{
			if (arr[j] < arr[k]) k = j;

		}

		if (i != k)
		{
			temp = arr[i];
			arr[i] = arr[k];
			arr[k] = temp;
		}
	}

	cout << "selection_sort end:";
	print_arr(arr, cnt);
}


void quick_sort(int arr[], int cnt)
{
	cout << "quick_sort begin:";
	print_arr(arr, cnt);

	my_quick_sort(arr, 0, cnt - 1);

	cout << "quick_sort end:";
	print_arr(arr, cnt);
}

void my_quick_sort(int arr[], int low, int high)
{
	int locate = 0;
	if (low < high)
	{
		locate = my_partition(arr, low, high);
		my_quick_sort(arr, low, locate - 1);
		my_quick_sort(arr, locate + 1, high);
	}
}

int my_partition(int arr[], int low, int high)
{
	int flag = arr[low];
	while (low < high)
	{
		while (low < high && flag < arr[high])  high--;
		arr[low] = arr[high];
		while (low < high && flag > arr[low]) low++;
		arr[high] = arr[low];
	}

	arr[low] = flag;
	return low;
}

void test_sort()
{
	void(*fun[3])(int a[],int);

	int arr[] = { 4, 2, 5, 8, 7, 3, 1, 6, 9, 0 };
	fun[0] = bubble_sort;
	fun[0](arr, 10);

	int arr1[] = { 4, 2, 5, 8, 7, 3, 1, 6, 9, 0 };
	fun[1] = selection_sort;
	fun[1](arr1, 10);


	int arr2[] = { 4, 2, 5, 8, 7, 3, 1, 6, 9, 0 };
	fun[2] = quick_sort;
	fun[2](arr2, 10);
}
