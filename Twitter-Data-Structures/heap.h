#ifndef HEAP_H
#define HEAP_H

#include <stdexcept>
#include <iostream>
#include <vector>


template <typename T>
struct Hashtag
{
  T val;
  int p;
};

template <typename T, typename Comparator >
class Heap
{
 public:
  /// Constructs an m-ary heap for any m >= 2
  Heap(int m, Comparator c)
  {  
    m_ary = m;
    comp = c;
  }

  /// Destructor as needed
  ~Heap();

  /// Adds an item
  void push(const T& item)
  {
    int index = exists(item);
    if (index == -1) // doesn't exist
    {
        Hashtag<T> h;
        h.val = item;
        h.p = 1;
        heap_vector.push_back(h);
        int cur = heap_vector.size()-1;
        trickleUp(cur);
    }  
    else
    {
        heap_vector[index].p++;
        trickleUp(index);
    }

  };

  /// returns the element at the top of the heap 
  ///  max (if max-heap) or min (if min-heap)
  T const & top() const // EXCEPTIONS
  {  
    if (heap_vector.empty())
    {
      throw std::logic_error("can't top an empty heap");
    }
    return heap_vector[0].val;   
  };

  int topPriority()
  { 
    if (heap_vector.empty())
    {
      throw std::logic_error("can't top an empty heap");
    }
    return heap_vector[0].p;
  }

  /// Removes the top element
  void pop()
  {
    if (heap_vector.empty())
    {
      throw std::logic_error("can't pop an empty heap");
      return;
    }
    int size = heap_vector.size()-1;
    heap_vector[0] = heap_vector[size];
    heap_vector.pop_back();
    trickleDown(0);
  }

  /// returns true if the heap is empty
  bool empty() const
  {  return heap_vector.size() == 0;   };
  
  int heapVectorSize() const
  {
    return heap_vector.size();
  }

 private:
  /// Add whatever helper functions and data members you need below
  
  std::vector< Hashtag<T> > heap_vector;
  int m_ary;
  Comparator comp;

  void trickleUp(int cur)
  {

    if (cur == (cur-1)/m_ary)
      return;
    if ((cur-1)/m_ary < 0)
      return;

    if (comp(heap_vector[cur], heap_vector[(cur-1)/m_ary]) ) // min heap
    {
      Hashtag<T> temp = heap_vector[cur];
      heap_vector[cur] = heap_vector[(cur-1)/m_ary];
      heap_vector[(cur-1)/m_ary] = temp;
      cur = (cur-1)/m_ary;
      trickleUp(cur);
    }
  }

  void trickleDown(int cur)
  {
    int best_child = m_ary*cur+1;
    if (best_child >= heap_vector.size()) // leaf node
    {
      return;
    }


    // finding best child
    for (int i = m_ary*cur+1+1; i < m_ary*cur+m_ary+1; i++)
    {

      if (!comp(heap_vector[best_child], heap_vector[i]))
      {
        best_child = i;
      }
    }

    if (comp (heap_vector[best_child], heap_vector[cur]) )
    {
      Hashtag<T> temp = heap_vector[cur];
      heap_vector[cur] = heap_vector[best_child];
      heap_vector[best_child] = temp;
      cur = best_child;  
      trickleDown(cur);
    }

  }

  /// Add whatever helper functions and data members you need below
  int exists(T t1)
  {
    for (unsigned i = 0; i < heap_vector.size(); ++i)
    {
        if (heap_vector[i].val == t1)
        {
            return i;
        }
    }
    return -1;
  }

  T compare(T t1, T t2)
  {
    if (comp(heap_vector[t1], heap_vector[t2])) // means t1>t2
      return t1;
    else
      return t2;
  }

};

#endif
