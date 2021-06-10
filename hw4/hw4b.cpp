#include <windows.h>

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

class Counter {
 private:
  inline static long long count = 0;

 public:
  static void increase(int value = 1) { count += value; }
  static void reset() { count = 0; }
  static long long getResult() { return count; }
};

enum ArrayOrder {
  kAscending,
  kDescending,
  kRandom,
};

auto GetArray(const int length, ArrayOrder order) {
  std::vector<int> array(length);
  switch (order) {
    case kAscending: {
      iota(array.begin(), array.end(), 1);
      break;
    }
    case kDescending: {
      iota(array.rbegin(), array.rend(), 1);
      break;
    }
    case kRandom: {
      iota(array.begin(), array.end(), 1);
      std::random_shuffle(array.begin(), array.end());
      break;
    }
  }
  return array;
}

auto GetTestingArrays(const int num_of_tests, const int length,
                      ArrayOrder order) {
  std::vector<std::vector<int>> testing_arrays(num_of_tests);
  generate(testing_arrays.begin(), testing_arrays.end(),
           std::bind(GetArray, length, order));
  return testing_arrays;
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

void QuickSort(std::vector<int> &v, int p, int r) {
  if (p < r) {
    int q = Partition(v, p, r);
    QuickSort(v, p, q - 1);
    QuickSort(v, q + 1, r);
  }
}

int RandomizedPartition(std::vector<int> &v, int p, int r) {
  int i = rand() % (r - p + 1) + p;
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
    std::vector<std::vector<int>> &test_vectors) {
  Counter::reset();
  for (std::vector<int> v : test_vectors) {
    sort_func(v, 0, v.size() - 1);
  }
  return Counter::getResult() / double(test_vectors.size());
}

int main(int argc, char **argv) {
  int num_of_tests = atoi(argv[1]);
  int array_length = 1000;

  srand(time(NULL));
  auto ascending_arrays =
      GetTestingArrays(num_of_tests, array_length, kAscending);
  auto descending_arrays =
      GetTestingArrays(num_of_tests, array_length, kDescending);
  auto shuffled_arrays = GetTestingArrays(num_of_tests, array_length, kRandom);

  std::cout << "Array Length: " << array_length
            << ", Executions:" << num_of_tests << std::endl;

  std::cout << "Quick Sort: " << std::endl;
  std::cout << " Ascending: "
            << ExecuteAndGetAvgCount(QuickSort, ascending_arrays) << std::endl;
  std::cout << " Shuffled: "
            << ExecuteAndGetAvgCount(QuickSort, shuffled_arrays) << std::endl;
  std::cout << " Descending: "
            << ExecuteAndGetAvgCount(QuickSort, descending_arrays) << std::endl;

  std::cout << "Randomized Quick Sort: " << std::endl;
  std::cout << " Ascending: "
            << ExecuteAndGetAvgCount(RandomizedQuickSort, ascending_arrays)
            << std::endl;
  std::cout << " Shuffled: "
            << ExecuteAndGetAvgCount(RandomizedQuickSort, shuffled_arrays)
            << std::endl;
  std::cout << " Descending: "
            << ExecuteAndGetAvgCount(RandomizedQuickSort, descending_arrays)
            << std::endl;
  return 0;
}
