#include <windows.h>

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

class Counter {
 private:
  inline static int count = 0;

 public:
  static void increase(int value = 1) { count += value; }
  static void reset() { count = 0; }
  static int getResult() { return count; }
};

std::vector<int> GetRandomArray(int length) {
  std::vector<int> array(length);
  std::iota(array.begin(), array.end(), 0);
  std::random_shuffle(array.begin(), array.end());
  return array;
}

int Partition(std::vector<int> &v, int p, int r) {
  int x = v[r];
  int i = p - 1;

  Counter::increase(r - p);
  for (int j = p; j <= r - 1; ++j) {
    if (v[j] <= x) {
      ++i;
      std::swap(v[i], v[j]);
    }
  }
  std::swap(v[i + 1], v[r]);
  return i + 1;
}

int RandomizedPartition(std::vector<int> &v, int p, int r) {
  int i = std::rand() % (r - p + 1) + p;
  std::swap(v[i], v[r]);
  return Partition(v, p, r);
}

void RandomizedQuickSort(std::vector<int> &v, int p, int r) {
  if (p < r) {
    int q = RandomizedPartition(v, p, r);
    RandomizedQuickSort(v, p, q - 1);
    RandomizedQuickSort(v, q + 1, r);
  }
}

double ExecuteAndGetAvgCount(
    std::function<void(std::vector<int> &, int, int)> sort_func,
    std::vector<int> vec, int executions) {
  Counter::reset();
  for (int i = 0; i < executions; ++i) {
    sort_func(vec, 0, vec.size() - 1);
  }
  return Counter::getResult() / double(executions);
}

int main(int argc, char **argv) {
  int num_of_tests = atoi(argv[1]);
  int array_length = 1000;

  srand(time(NULL));
  std::vector<int> vec = GetRandomArray(array_length);

  std::cout << "Array Length: " << array_length << std::endl;
  std::cout << "Ave. count: " << std::setprecision(10)
            << ExecuteAndGetAvgCount(RandomizedQuickSort, vec, num_of_tests)
            << " times." << std::endl;

  return 0;
}
