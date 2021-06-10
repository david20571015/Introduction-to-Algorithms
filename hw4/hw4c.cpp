#include <windows.h>

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

std::vector<int> GetRandomArray(int length) {
  std::vector<int> array(length);
  std::iota(array.begin(), array.end(), 0);
  std::random_shuffle(array.begin(), array.end());
  return array;
}

void Merge(std::vector<int> &v, int p, int q, int r) {
  int n1 = q - p + 1;
  int n2 = r - q;
  std::vector<int> left(v.begin() + p, v.begin() + p + n1);
  std::vector<int> right(v.begin() + q + 1, v.begin() + q + 1 + n2);
  left.push_back(INT_MAX);
  right.push_back(INT_MAX);

  int i = 0, j = 0;
  for (int k = p; k <= r; ++k) {
    if (left[i] < right[j]) {
      v[k] = left[i++];
    } else {
      v[k] = right[j++];
    }
  }
}

void MergeSort(std::vector<int> &v, int p, int r) {
  if (p < r) {
    int q = (p + r) / 2;
    MergeSort(v, p, q);
    MergeSort(v, q + 1, r);
    Merge(v, p, q, r);
  }
}

int Partition(std::vector<int> &v, int p, int r) {
  int x = v[r];
  int i = p - 1;

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

double ExecuteAndGetAvgTime(
    std::function<void(std::vector<int> &, int, int)> sort_func,
    std::vector<std::vector<int>> &test_vectors) {
  double total_time;
  LARGE_INTEGER start, end, ts;
  QueryPerformanceFrequency(&ts);

  for (std::vector<int> v : test_vectors) {
    QueryPerformanceCounter(&start);
    sort_func(v, 0, v.size() - 1);
    QueryPerformanceCounter(&end);
    total_time += double(end.QuadPart - start.QuadPart) / ts.QuadPart * 1e6;
  }

  return total_time / test_vectors.size();
}

int main(int argc, char **argv) {
  int array_length = atoi(argv[1]);
  int num_of_tests = atoi(argv[2]);

  srand(time(NULL));
  std::vector<std::vector<int>> test_vectors(num_of_tests);
  std::generate(test_vectors.begin(), test_vectors.end(),
                std::bind(GetRandomArray, array_length));

  std::cout << "Array Length: " << array_length
            << ", Executions:" << test_vectors.size() << std::endl;

  std::cout << "Merge Sort: " << std::setprecision(10)
            << ExecuteAndGetAvgTime(MergeSort, test_vectors) << " us."
            << std::endl;

  std::cout << "Randomized Quick Sort: " << std::setprecision(10)
            << ExecuteAndGetAvgTime(RandomizedQuickSort, test_vectors) << " us."
            << std::endl;
  return 0;
}