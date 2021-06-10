#include <windows.h>

#include <algorithm>
#include <climits>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

vector<int> GetRandomArray(int length) {
  vector<int> array(length);
  iota(array.begin(), array.end(), 0);
  random_shuffle(array.begin(), array.end());
  return array;
}

void InsertSort(vector<int> &v, int length) {
  for (int i = 1; i < length; ++i) {
    int key = v[i];
    int j = i - 1;
    while (j >= 0 && v[j] > key) {
      v[j + 1] = v[j];
      --j;
    }
    v[j + 1] = key;
  }
}

void Merge(vector<int> &v, int p, int q, int r) {
  int n1 = q - p + 1;
  int n2 = r - q;
  vector<int> left(v.begin() + p, v.begin() + p + n1);
  vector<int> right(v.begin() + q + 1, v.begin() + q + 1 + n2);
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

void MergeSortBase(vector<int> &v, int p, int r) {
  if (p < r) {
    int q = (p + r) / 2;
    MergeSortBase(v, p, q);
    MergeSortBase(v, q + 1, r);
    Merge(v, p, q, r);
  }
}

void MergeSort(vector<int> &v, int length) { MergeSortBase(v, 0, length - 1); }

void Sorting(function<void(vector<int> &, int)> sort_func,
             vector<vector<int>> &test_vectors) {
  double total_time;
  int correct = 0, wrong = 0;
  LARGE_INTEGER start, end, ts;
  QueryPerformanceFrequency(&ts);

  for (vector<int> v : test_vectors) {
    QueryPerformanceCounter(&start);
    sort_func(v, v.size());
    QueryPerformanceCounter(&end);
    total_time += double(end.QuadPart - start.QuadPart) / ts.QuadPart * 1e6;
    is_sorted(v.begin(), v.end()) ? ++correct : ++wrong;
  }

  cout << correct << " Correct, " << wrong << " Wrong" << endl;
  cout << "Ave. time: " << setprecision(10) << total_time / test_vectors.size()
       << " us for " << test_vectors.size() << " executions" << endl;
}

int main(int argc, char **argv) {
  int array_length = atoi(argv[1]);
  int num_of_tests = atoi(argv[2]);

  srand(time(NULL));
  vector<vector<int>> test_vectors(num_of_tests);
  generate(test_vectors.begin(), test_vectors.end(),
           [array_length] { return GetRandomArray(array_length); });

  cout << "Array Length: " << array_length << endl;
  cout << "Insert Sort: ";
  Sorting(InsertSort, test_vectors);
  cout << "Merge Sort: ";
  Sorting(MergeSort, test_vectors);
}
