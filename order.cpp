#include <iostream>
#include <string>

int main() {
 std::string s = "but I have heard it works even if you don't believe in it";
 s.replace(0, 4, "").replace(s.find("even"), 4, "only")
  .replace(s.find(" don't"), 6, "");
  std::cout << s << std::endl;
  return 0;
}