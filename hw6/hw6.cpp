#include <windows.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <tuple>
#include <vector>

template <typename T>
using Table = std::vector<std::vector<T>>;

enum class Direction {
  kUpperLeft,
  kUp,
  kLeft,
};

template <typename T>
inline Table<T> MakeTable(size_t m, size_t n) {
  return Table<T>(m, std::vector<T>(n));
}

inline std::vector<int> GetRandomArray(size_t length) {
  std::vector<int> array(length);
  std::generate(array.begin(), array.end(), [] { return std::rand() % 10; });
  return array;
}

auto LcsLength(const std::vector<int> &x, const std::vector<int> &y) {
  Table<Direction> b = MakeTable<Direction>(x.size() + 1, y.size() + 1);
  Table<int> c = MakeTable<int>(x.size() + 1, y.size() + 1);

  for (int i = 1; i <= x.size(); ++i) {
    for (int j = 1; j <= y.size(); ++j) {
      if (x[i - 1] == y[j - 1]) {
        c[i][j] = c[i - 1][j - 1] + 1;
        b[i][j] = Direction::kUpperLeft;
      } else if (c[i - 1][j] >= c[i][j - 1]) {
        c[i][j] = c[i - 1][j];
        b[i][j] = Direction::kUp;
      } else {
        c[i][j] = c[i][j - 1];
        b[i][j] = Direction::kLeft;
      }
    }
  }

  return std::make_tuple(c[x.size()][y.size()], b);
}

void GenerateLcs(const Table<Direction> &b, const std::vector<int> &x, int i, int j, std::vector<int> &lcs) {
  if (i == 0 || j == 0) return;

  switch (b[i][j]) {
    case Direction::kUpperLeft: {
      GenerateLcs(b, x, i - 1, j - 1, lcs);
      lcs.push_back(x[i - 1]);
      break;
    }
    case Direction::kUp: {
      GenerateLcs(b, x, i - 1, j, lcs);
      break;
    }
    case Direction::kLeft: {
      GenerateLcs(b, x, i, j - 1, lcs);
      break;
    }
  }
}

std::vector<int> GetLcs(const Table<Direction> b, const std::vector<int> x, const std::vector<int> y) {
  std::vector<int> lcs;
  GenerateLcs(b, x, x.size(), y.size(), lcs);
  return lcs;
}

void DisplayVector(std::string name, std::vector<int> v) {
  std::cout << name << ": ";
  for (int i : v) std::cout << i << " ";
  std::cout << std::endl;
}

auto ExecuteAndGetAvgTime(size_t x_len, size_t y_len, unsigned int num_of_tests) {
  double len_cal_total_time = 0, lcs_total_time = 0;
  LARGE_INTEGER start, mid, end, ts;
  QueryPerformanceFrequency(&ts);

  for (unsigned int i = 0; i < num_of_tests; ++i) {
    std::vector<int> x = GetRandomArray(x_len);
    std::vector<int> y = GetRandomArray(y_len);

    QueryPerformanceCounter(&start);
    auto [length, table] = LcsLength(x, y);
    QueryPerformanceCounter(&mid);
    std::vector<int> z = GetLcs(table, x, y);
    QueryPerformanceCounter(&end);

    len_cal_total_time += double(mid.QuadPart - start.QuadPart) / ts.QuadPart;
    lcs_total_time += double(end.QuadPart - mid.QuadPart) / ts.QuadPart;
  }

  return std::make_tuple(len_cal_total_time / num_of_tests, lcs_total_time / num_of_tests);
}

int main(int argc, char **argv) {
  switch (argc) {
    case 1: {  // part 1
      std::vector<int> x = {6, 6, 6, 0, 8, 9, 4, 5, 7, 9, 4, 5, 3};
      std::vector<int> y = {1, 6, 8, 0, 4, 8, 7, 5, 4, 8, 7, 7, 4, 1, 4};

      auto [length, table] = LcsLength(x, y);
      std::vector<int> z = GetLcs(table, x, y);

      DisplayVector("X", x);
      DisplayVector("Y", y);
      DisplayVector("Z", z);
      std::cout << "Length: " << length << std::endl;
      break;
    }
    case 4: {  // part 2
      const size_t kXLen = std::atoi(argv[1]);
      const size_t kYLen = std::atoi(argv[2]);
      const unsigned int kTests = std::atoi(argv[3]);

      std::cout << "x_len: " << kXLen << " , y_len: " << kYLen;
      std::cout << " , x_len + y_len: " << kXLen + kYLen << " , x_len * y_len: " << kXLen * kYLen << std::endl;

      srand(time(NULL));
      auto [len_avg_time, lcs_avg_time] = ExecuteAndGetAvgTime(kXLen, kYLen, kTests);
      std::cout << "LCS-Length: " << len_avg_time << " s" << std::endl;
      std::cout << "Print-LCS: " << lcs_avg_time << " s" << std::endl;
      std::cout << std::endl;
      break;
    }
  }
  return 0;
}