#include <iostream>

#include <string>

int main() {
  std::string s1 = "hello";
  const std::string& s2 = s1;
  std::string s3(s2);
  std::cout << s3 << std::endl;
}