#include <windows.h>

#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

vector<int> GetRandomArray(int length, default_random_engine &generator) {
  vector<int> array(length);
  uniform_int_distribution<short> distribution(numeric_limits<short>::min(),
                                               numeric_limits<short>::max());
  generate(array.begin(), array.end(), bind(distribution, ref(generator)));
  return array;
}

auto BruteForce(vector<int> &array, int low, int high) {
  int max_sum = numeric_limits<int>::min();
  int low_index, high_index;

  for (int i = low + 1; i <= high; ++i) array[i] += array[i - 1];

  for (int i = 0; i <= high; ++i)
    for (int j = i; j <= high; ++j) {
      int sum = array[j] - array[i];
      if (sum > max_sum) {
        max_sum = sum;
        low_index = i;
        high_index = j;
      }
    }
  return tuple<int, int, int>(low_index, high_index, max_sum);
}

auto FindMaxCrossingSubarray(vector<int> &array, int low, int mid, int high) {
  int max_left, max_right;
  int left_sum = numeric_limits<int>::min();
  int sum = 0;
  for (int i = mid; i >= low; --i) {
    sum += array[i];
    if (sum > left_sum) {
      left_sum = sum;
      max_left = i;
    }
  }
  int right_sum = numeric_limits<int>::min();
  sum = 0;
  for (int i = mid + 1; i <= high; ++i) {
    sum += array[i];
    if (sum > right_sum) {
      right_sum = sum;
      max_right = i;
    }
  }
  return tuple<int, int, int>(max_left, max_right, left_sum + right_sum);
}

auto FindMaxSubarray(vector<int> &array, int low, int high) {
  if (high == low) return tuple<int, int, int>(low, high, array[low]);

  int mid = (low + high) / 2;
  auto [left_low, left_high, left_sum] = FindMaxSubarray(array, low, mid);
  auto [right_low, right_high, right_sum] =
      FindMaxSubarray(array, mid + 1, high);
  auto [cross_low, cross_high, cross_sum] =
      FindMaxCrossingSubarray(array, low, mid, high);

  if (left_sum >= right_sum && left_sum >= cross_sum)
    return tuple<int, int, int>(left_low, left_high, left_sum);
  if (right_sum >= left_sum && right_sum >= cross_sum)
    return tuple<int, int, int>(right_low, right_high, right_sum);
  return tuple<int, int, int>(cross_low, cross_high, cross_sum);
}

auto ExecuteAndGetAvgTimeCost(
    function<tuple<int, int, int>(vector<int> &, int, int)> func,
    vector<vector<int>> &test_vectors) {
  double total_time;
  LARGE_INTEGER start, end, ts;
  QueryPerformanceFrequency(&ts);

  vector<int> ans;

  for (vector<int> v : test_vectors) {
    QueryPerformanceCounter(&start);
    auto [low, high, sum] = func(v, 0, v.size() - 1);
    QueryPerformanceCounter(&end);
    total_time += double(end.QuadPart - start.QuadPart) / ts.QuadPart;

    ans.push_back(sum);
  }
  return tuple<double, vector<int>>(total_time / test_vectors.size(), ans);
}

int main(int argc, char **argv) {
  int array_length = atoi(argv[1]);
  int num_of_tests = atoi(argv[2]);

  vector<vector<int>> test_vectors(num_of_tests);

  auto seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);

  generate(test_vectors.begin(), test_vectors.end(),
           bind(GetRandomArray, array_length, ref(generator)));

  cout << "Array Length: " << array_length << ", Execution: " << num_of_tests
       << endl;

  auto [time1, ans1] = ExecuteAndGetAvgTimeCost(BruteForce, test_vectors);
  cout << "Brute Force:        ";
  for (int s : ans1) cout << s << " ";
  cout << endl;
  cout << setw(20) << fixed << time1 * 1e6 << " us" << endl;

  auto [time2, ans2] = ExecuteAndGetAvgTimeCost(FindMaxSubarray, test_vectors);
  cout << "Divide And Conquer: ";
  for (int s : ans2) cout << s << " ";
  cout << endl;
  cout << setw(20) << fixed << time2 * 1e6 << " us" << endl;
}
