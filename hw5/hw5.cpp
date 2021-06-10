#include <functional>
#include <iostream>
#include <limits>
#include <vector>

int TopDown(const std::vector<int> &price, int length,
            std::vector<int> &record) {
  if (length == 0) {
    return 0;
  } else {
    int q = std::numeric_limits<int>::min();
    for (int i = 1; i <= length; ++i) {
      int subprice = TopDown(price, length - i, record);
      if (price[i] + subprice > q) {
        q = price[i] + subprice;
        record[length] = i;
      }
    }
    return q;
  }
}

int MemoizedTopDownAux(const std::vector<int> &price, int length,
                       std::vector<int> &record, std::vector<int> &r) {
  if (r[length] >= 0) {
    return r[length];
  }
  if (length == 0) {
    r[0] = 0;
    return 0;
  }

  int q = std::numeric_limits<int>::min();
  for (int i = 1; i <= length; ++i) {
    int subprice = MemoizedTopDownAux(price, length - i, record, r);
    if (price[i] + subprice > q) {
      q = price[i] + subprice;
      record[length] = i;
    }
  }
  r[length] = q;
  return q;
}

int MemoizedTopDown(const std::vector<int> &price, int length,
                    std::vector<int> &record) {
  std::vector<int> r(length + 1, std::numeric_limits<int>::min());
  return MemoizedTopDownAux(price, length, record, r);
}

int BottomUp(const std::vector<int> &price, int length,
             std::vector<int> &record) {
  std::vector<int> r(length + 1, 0);
  for (int j = 1; j <= length; ++j) {
    int q = std::numeric_limits<int>::min();
    for (int i = 1; i <= j; ++i) {
      if (price[i] + r[j - i] > q) {
        q = price[i] + r[j - i];
        record[j] = i;
      }
    }
    r[j] = q;
  }
  return r[length];
}

auto CutRod(
    std::function<int(const std::vector<int> &, int, std::vector<int> &)> func,
    std::vector<int> price, int length) {
  std::vector<int> record(length + 1, 0);
  int max_price = func(price, length, record);

  std::vector<int> fragment;
  int i = record.size() - 1;
  while (i > 0) {
    fragment.push_back(record[i]);
    i -= record[i];
  }

  return std::tuple<int, std::vector<int>>(max_price, fragment);
}

void DisplayResult(std::string type, int price, std::vector<int> fragment) {
  std::cout << type << ":" << std::endl;
  std::cout << "Price: " << price << std::endl;
  for (int c : fragment) {
    std::cout << c << ' ';
  }
  std::cout << std::endl;
  std::cout << "Pieces: " << fragment.size() << std::endl;
}

int main(int argc, char **argv) {
  const int length = std::atoi(argv[1]);

  std::vector<int> price = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
  price.resize(length + 1, 0);

  auto [top_down_price, top_down_fragment] = CutRod(TopDown, price, length);
  auto [bottom_up_price, bottom_up_fragment] = CutRod(BottomUp, price, length);

  std::cout << "Total Length: " << length << std::endl << std::endl;
  DisplayResult("Top Down", top_down_price, top_down_fragment);
  std::cout << std::endl;
  DisplayResult("Bottom Up", bottom_up_price, bottom_up_fragment);

  return 0;
}