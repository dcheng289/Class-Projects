#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

template <class T, class Comparator>
void Merge(std::vector<T>& left, std::vector<T>& right, std::vector<T>& output
    , Comparator comp)
{
    unsigned int l = 0;
    unsigned int r = 0;

    while (l < left.size() && r < right.size())
    {
        if (comp(left[l], right[r]))
            output.push_back(left[l++]);
        else
            output.push_back(right[r++]);
    }

    while(l < left.size())
        output.push_back(left[l++]);
    while(r < right.size())

        output.push_back(right[r++]);

}

template <class T, class Comparator>
void merge_sort(std::vector<T>& myList, int n, Comparator comp)
{
    if (n < 2)
        return;
    int mid = n/2;
    std::vector<T> left;
    std::vector<T> right;
    for (int i = 0; i < mid; i++)
    {
        left.push_back(myList[i]);
    }
    for (int i = mid; i < n; i++)
    {
        right.push_back(myList[i]);
    }
    merge_sort(left, mid, comp);
    merge_sort(right, n-mid, comp);


    myList.clear();
    Merge(left, right, myList, comp);
}

template <class T, class Comparator>
void mergeSort (std::vector<T>& myList, Comparator comp)
{
    merge_sort(myList, myList.size(), comp);
}