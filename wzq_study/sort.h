#ifndef _SORT_H_
#define _SORT_H_


#include "common.h"

void print_arr(int arr[],int cnt);
void bubble_sort(int arr[],int cnt) ;
void selection_sort(int arr[],int cnt) ;
void quick_sort(int arr[],int cnt) ;
void my_quick_sort(int arr[],int low,int high);
int my_partition(int arr[],int low,int high);

void test_sort();
#endif //_SORT_H_
