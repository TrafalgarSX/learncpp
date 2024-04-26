#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <unordered_map>

void existence_queries() {
  std::vector<int> v{1, 2, 3, 4, 5, 2 , 2};
  fmt::print("std::binary_search(v.begin(), v.end(), 3) = {}\n",
             std::binary_search(v.begin(), v.end(), 3));
  fmt::print("std::binary_search(v.begin(), v.end(), 3) = {}\n",
             std::binary_search(v.begin(), v.end(), 6));

  // any_of
  bool has_even =
      std::any_of(v.begin(), v.end(), [](int i) { return i % 2 == 0; });

  fmt::print("v has_even = {}\n",
             has_even ? "true" : "false");  // v has_even = true

  // all_of
  bool all_even = std::all_of(v.begin(), v.end(), [](int i) {
    return i % 2 == 0;
  });  // all_even will be true
  fmt::print("v all_even = {}\n", all_even ? "true" : "false");

  // none_of
  bool none_even = std::none_of(v.begin(), v.end(), [](int i) {
    return i % 2 == 0;
  });  // none_even will be false
  fmt::print("v none_even = {}\n", none_even ? "true" : "false");

  // count
  int twos = std::count(v.begin(), v.end(), 2);
  fmt::print("v has {} twos\n", twos);

  // count_if
  int odds = std::count_if(v.begin(), v.end(), [](int i) {
    return i % 2 != 0;
  });  // odds will be 3
  fmt::print("v has {} odd numbers\n", odds);

  // min_element
  auto min = std::min_element(v.begin(), v.end());
  auto max = std::max_element(v.begin(), v.end());
  fmt::print("min = {}, max = {}\n", *min, *max);

  // find_if
  auto it = std::find_if(v.begin(), v.end(), [](int i) {
      return i % 2 == 0;
  });  // it will point to the first even number
  fmt::print("first even number = {}\n", *it);

  // equal 
  std::vector<int> v2{1, 2, 3, 4, 5, 2 , 2};
  bool equal = std::equal(v.begin(), v.end(), v2.begin(), v2.end());
  fmt::print("v == v2 ? {}\n", equal ? "true" : "false");

  // mismatch
  auto p = std::mismatch(v.begin(), v.end(), v2.begin(), v2.end());
  fmt::print("{}, {}\n", p.first == v.end() ? "true" : "false", p.second == v2.end() ? "true" : "false");
  fmt::print("first mismatch = {}, {}\n", *p.first, *p.second);

  // hashmap none_of
  std::unordered_map<int, std::string> m{{1, "a"}, {2, "b"}, {3, "c"}};
  int index = 5;
  bool none_index = std::none_of(m.begin(), m.end(), [index](auto& p) {
      return p.first == index;
  });
  fmt::print("m don't have index = {} ? {}\n", index, none_index ? "true" : "false");

  // find_if 
  auto it2 = std::find_if(m.begin(), m.end(), [index](auto& p) {
      return p.first == index;
  });
  fmt::print("m has index = {} ? {}\n", index, it2 == m.end() ? "false" : "true");
}

void std_binary_search()
{
    std::vector<int> v{1, 2, 3, 4, 5, 2 , 2};
    std::sort(v.begin(), v.end());
    // fmt print vector
    fmt::print("v = {}\n", fmt::join(v, ", "));

    fmt::print("std::binary_search(v.begin(), v.end(), 3) = {}\n",
                 std::binary_search(v.begin(), v.end(), 3));
    fmt::print("std::binary_search(v.begin(), v.end(), 3) = {}\n",
                 std::binary_search(v.begin(), v.end(), 6));

    auto it = std::lower_bound(v.begin(), v.end(), 2);
    fmt::print("lower_bound = {}\n", *it);

    auto it2 = std::upper_bound(v.begin(), v.end(), 2);
    fmt::print("upper_bound = {}, {}\n", *it2, *(it2 - 1));

    auto p = std::equal_range(v.begin(), v.end(), 2);
    fmt::print("equal_range = {}, {}\n", *p.first, *p.second);

}

void reordering_elements()
{
    // std::vector<int> v{4,2,5,6,3,7,1};
    std::vector<int> v{9, 20, 7, 6, 20, 4, 3, 2, 1};

    std::nth_element(v.begin(), v.begin() + 3, v.end());
    fmt::print("v = {}\n", fmt::join(v, ", "));

    std::nth_element(v.begin(), v.begin() + 3, v.end(), std::greater<int>());
    fmt::print("v = {}\n", fmt::join(v, ", "));

    // partition
    std::vector<int> v2{4,2,5,6,3,7,1};
    auto it = std::partition(v2.begin(), v2.end(), [](int i) {
        return i % 2 != 0;
    });
    fmt::print("v2 = {}\n", fmt::join(v2, ", "));

    //rotate
    std::rotate(v2.begin(), it, v2.end());
    fmt::print("v2 = {}\n", fmt::join(v2, ", "));

    // shuffle
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(v2.begin(), v2.end(), g);
    fmt::print("v2 = {}\n", fmt::join(v2, ", "));
}

void manipulate_sored_sequences()
{
    std::vector<int> v{1, 2, 3, 17, 20, 2 , 2};
    std::vector<int> v2{10, 8, 77, 2};
    std::sort(v.begin(), v.end());
    std::sort(v2.begin(), v2.end());

    std::vector<int> v3;
    std::merge(v.begin(), v.end(), v2.begin(), v2.end(), std::back_inserter(v3));
    fmt::print("v3 = {}\n", fmt::join(v3, ", "));

    std::vector<int> v4;
    std::set_union(v.begin(), v.end(), v2.begin(), v2.end(), std::back_inserter(v4));
    fmt::print("v4 = {}\n", fmt::join(v4, ", "));

    std::vector<int> vec{1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
    auto it = std::unique(vec.begin(), vec.end());
    vec.erase(it, vec.end());
    fmt::print("vec = {}\n", fmt::join(vec, ", "));
}

void change_values()
{
    std::vector<int> v{1, 2, 3, 17, 20, 2 , 2};

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ", "));

    std::vector<int> copy;
    std::copy_if(v.begin(), v.end(), std::back_inserter(copy), [](int i) {
        return i % 2 == 0;
    });
    fmt::print("\ncopy = {}\n", fmt::join(copy, ", "));

    std::transform(v.begin(), v.end(), v.begin(), [](int i) {
        return i * 2;
    });
    fmt::print("v = {}\n", fmt::join(v, ", "));

    std::vector<int> generated(10);
    std::generate(generated.begin(), generated.end(), []() {
        return std::rand() % 100;
    });
    fmt::print("generated = {}\n", fmt::join(generated, ", "));


}

int main() {
//   existence_queries();
//   std_binary_search();
//   reordering_elements();
//   manipulate_sored_sequences();
  change_values();
  return 0;
}