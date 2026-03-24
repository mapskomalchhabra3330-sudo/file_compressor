#pragma once

#include <iostream>

template<typename T> class Heap{
    T* arr;
    int size;
    int capacity;
    void heapifyDown(int index) {
      int left = 2 * index + 1;
      int right = 2 * index + 2;
      int smallest = index;
      if (left < this->size && this->arr[left] < this->arr[smallest]) {
        smallest = left;
      }
      if (right < this->size && this->arr[right] < this->arr[smallest]) {
        smallest = right;
      }
      if (smallest != index) {
        swap(this->arr[index], this->arr[smallest]);
        this->heapifyDown(smallest);
      }
    }
    void heapifyUp(int index) {
      int parent = (index - 1) / 2;
      if (index > 0 && this->arr[index] < this->arr[parent]) {
        swap(this->arr[index], this->arr[parent]);
        this->heapifyUp(parent);
      }
    }
    void swap(T &a, T &b) {
      T temp = a;
      a = b;
      b = temp;
    }

    public:
        Heap(int capacity){
            this->capacity = capacity;
            this->size = 0;
            this->arr = new T[capacity];
        }
        Heap(const Heap &other) {
          capacity = other.capacity;
          size = other.size;
          arr = new T[capacity];
          for (int i = 0; i < size; i++) {
            arr[i] = other.arr[i];
          }
        }

        Heap &operator=(const Heap &other) {
          if (this == &other)
            return *this;

          delete[] arr;

          capacity = other.capacity;
          size = other.size;
          arr = new T[capacity];
          for (int i = 0; i < size; i++) {
            arr[i] = other.arr[i];
          }

          return *this;
        }

        ~Heap() {
          delete[] arr;
        }

        void insert(T val){
            if(this->size == this->capacity){
                return;
            }
            this->arr[this->size] = val;
            this->size++;
            this->heapifyUp(this->size-1);
        }
        T extractMin(){
            if(this->size == 0){
                return T();
            }
            T min = this->arr[0];
            this->arr[0] = this->arr[this->size-1];
            this->size--;
            this->heapifyDown(0);
            return min;
        }
        int getSize() { return this->size; }

        friend std::ostream &operator<<(std::ostream &out, const Heap<T> &h) {
          std::cout<<'s'<<" "<<h.size<<std::endl;
          for (int i = 0; i < h.size; i++) {
            std::cout<<"h";
            out << h.arr[i]<<std::endl;
          }
          return out;
        }
};