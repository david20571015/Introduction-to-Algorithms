#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

enum ArrayOrder {
  kIncrease,
  kDecrease,
  kRandom,
};

template <typename Iterator>
inline void RandomizedInPlace(Iterator first, Iterator last) {
  if (first != last)
    for (auto i = first; i != last; ++i) {
      auto j = i + std::rand() % (last - i);
      if (i != j) std::iter_swap(i, j);
    }
}

auto GetArray(const int length, ArrayOrder order) {
  std::vector<int> array(length);
  switch (order) {
    case kIncrease: {
      iota(array.begin(), array.end(), 1);
      break;
    }
    case kDecrease: {
      iota(array.rbegin(), array.rend(), 1);
      break;
    }
    case kRandom: {
      iota(array.begin(), array.end(), 1);
      RandomizedInPlace(array.begin(), array.end());
      break;
    }
  }
  return array;
}

inline auto GetTestingArrays(const int num_of_tests, const int length,
                             ArrayOrder order) {
  std::vector<std::vector<int>> testing_arrays(num_of_tests);
  generate(testing_arrays.begin(), testing_arrays.end(),
           std::bind(GetArray, length, order));
  return testing_arrays;
}

auto HireAssistant(std::vector<int> &candidates, const int interview_cost,
                   const int hiring_cost) {
  int best_candidate = 0;

  // hire the first candidate
  int hired = 1;
  int cost = interview_cost + hiring_cost;

  for (int i = 1; i < candidates.size(); ++i) {
    cost += interview_cost;

    if (candidates[i] > candidates[best_candidate]) {
      best_candidate = i;
      hired += 1;
      cost += hiring_cost;
    }
  }

  return std::make_tuple(cost, hired);
}

auto ExecuteAndGetAvgCost(
    std::function<std::tuple<int, int>(std::vector<int> &)> func,
    std::vector<std::vector<int>> &testing_arrays) {
  double total_cost = 0, total_hired = 0;
  for (auto array : testing_arrays) {
    auto [cost, hired] = func(array);
    total_cost += cost;
    total_hired += hired;
  }

  return std::make_tuple(total_cost / testing_arrays.size(),
                         total_hired / testing_arrays.size());
}

int main(int argc, char **argv) {
  std::cout << RAND_MAX;
  const int array_length = atoi(argv[1]);
  const int num_of_tests = atoi(argv[2]);
  const int interview_cost = atoi(argv[3]) ?: 1;
  const int hiring_cost = atoi(argv[4]) ?: 5;


  auto increase_arrays = GetTestingArrays(1, array_length, kDecrease);
  auto decrease_arrays = GetTestingArrays(1, array_length, kIncrease);
  auto random_arrays = GetTestingArrays(num_of_tests, array_length, kRandom);

  auto hiring = std::bind(HireAssistant, std::placeholders::_1, interview_cost,
                          hiring_cost);

  std::cout << "Array Length: " << array_length << std::endl;
  
  auto [best_cost, best_hired] = ExecuteAndGetAvgCost(hiring, increase_arrays);
  std::cout << "Best Case:" << std::endl
            << "  cost: " << best_cost << std::endl
            << "  hired: " << best_hired << std::endl;

  auto [worst_cost, worst_hired] =
      ExecuteAndGetAvgCost(hiring, decrease_arrays);
  std::cout << "Worst Case:" << std::endl
            << "  cost: " << worst_cost << std::endl
            << "  hired: " << worst_hired << std::endl;

  auto [avg_cost, avg_hired] = ExecuteAndGetAvgCost(hiring, random_arrays);
  std::cout << "Average Case:" << std::endl
            << "  cost: " << avg_cost << std::endl
            << "  hired: " << avg_hired << std::endl;
}
