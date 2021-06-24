#include <windows.h>

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

template <typename T>
using Table = std::vector<std::vector<T>>;

template <typename T>
inline Table<T> MakeTable(size_t m, size_t n) {
  return Table<T>(m, std::vector<T>(n));
}

inline void PrintVector(std::string name, std::vector<double> v) {
  std::cout << name << ": ";
  std::cout << std::fixed << std::setprecision(2) << std::setw(4);
  for (double i : v) std::cout << i << " ";
  std::cout << std::endl;
}

void PrintBstBase(const Table<int> &root, int r, int leftmost, int rightmost) {
  if (r - 1 < leftmost) {
    std::cout << "d" << r - 1 << " is the left node of k" << r << std::endl;
  } else {
    int left_root = root[leftmost][r - 1];
    std::cout << "k" << left_root << " is the left node of k" << r << std::endl;
    PrintBstBase(root, left_root, leftmost, r - 1);
  }

  if (r + 1 > rightmost) {
    std::cout << "d" << r << " is the right node of k" << r << std::endl;
  } else {
    int right_root = root[r + 1][rightmost];
    std::cout << "k" << right_root << " is the right node of k" << r << std::endl;
    PrintBstBase(root, right_root, r + 1, rightmost);
  }
}

void PrintBst(Table<int> root) {
  int root_node = root[1][root.size() - 1];
  std::cout << "k" << root_node << " is the root " << std::endl;
  PrintBstBase(root, root_node, 1, root.size() - 1);
}

inline auto GetRandomArrays(size_t n) {
  std::vector<double> p(n + 1), q(n + 1), tmp(n * 2 + 1);
  p[0] = 0.;

  std::generate(tmp.begin(), tmp.end(), [] { return std::rand() / double(RAND_MAX); });
  double sum = std::accumulate(tmp.begin(), tmp.end(), 0.);
  auto normalize_fn = [sum](double i) { return i / sum; };

  std::transform(tmp.begin(), tmp.begin() + n, p.begin() + 1, normalize_fn);
  std::transform(tmp.begin() + n, tmp.end(), q.begin(), normalize_fn);

  return std::make_tuple(p, q);
}

auto OptimalBst(std::vector<double> p, std::vector<double> q) {
  int n = p.size() - 1;
  Table<double> e = MakeTable<double>(n + 2, n + 1);
  Table<double> w = MakeTable<double>(n + 2, n + 1);
  Table<int> root = MakeTable<int>(n + 1, n + 1);

  for (int i = 1; i <= n + 1; ++i) {
    e[i][i - 1] = q[i - 1];
    w[i][i - 1] = q[i - 1];
  }

  for (int l = 1; l <= n; ++l) {
    for (int i = 1; i <= n - l + 1; ++i) {
      int j = i + l - 1;
      e[i][j] = std::numeric_limits<double>::max();
      w[i][j] = w[i][j - 1] + p[j] + q[j];
      for (int r = i; r <= j; ++r) {
        double t = e[i][r - 1] + e[r + 1][j] + w[i][j];
        if (t < e[i][j]) {
          e[i][j] = t;
          root[i][j] = r;
        }
      }
    }
  }

  return std::make_tuple(e[1][n], root);
}

double ExecuteAndGetAvgTime(size_t n, unsigned int num_of_tests) {
  double total_time = 0;
  LARGE_INTEGER start, end, ts;
  QueryPerformanceFrequency(&ts);

  for (unsigned int i = 0; i < num_of_tests; ++i) {
    auto [p, q] = GetRandomArrays(n);

    QueryPerformanceCounter(&start);
    auto [e, root] = OptimalBst(p, q);
    QueryPerformanceCounter(&end);

    total_time += double(end.QuadPart - start.QuadPart) / ts.QuadPart;
  }

  return total_time / num_of_tests;
}

int main(int argc, char **argv) {
  switch (argc) {
    case 1: {  // part 1
      std::vector<double> p = {0, 0.05, 0.04, 0.05, 0.07, 0.08, 0.09, 0.04, 0.08};
      std::vector<double> q = {0.08, 0.06, 0.04, 0.06, 0.03, 0.06, 0.07, 0.06, 0.04};

      // discussion 1.
      // std::vector<double> p = {0, 0.05, 0.15, 0.05, 0.15, 0.15};
      // std::vector<double> q = {0.1, 0.05, 0.05, 0.1, 0.05, 0.1};

      PrintVector("p", p);
      PrintVector("q", q);

      auto [e, root] = OptimalBst(p, q);
      std::cout << "cost: " << e << ", root: " << root[1][p.size() - 1] << std::endl;

      PrintBst(root);

      break;
    }
    case 3: {  // part 2
      const size_t kSize = std::atoi(argv[1]);
      const unsigned int kTests = std::atoi(argv[2]);

      srand(time(NULL));

      std::cout << "size: " << kSize << std::endl;
      double avg_time = ExecuteAndGetAvgTime(kSize, kTests);
      std::cout << "Avg. Time: " << avg_time << " s" << std::endl;
      break;
    }
  }
  return 0;
}