#include <utility>
#include <iostream>
int m1( int a ) {
  int b = 5;
  return a + b;
}

int g;

int m2() {
  int &&p = int( 3 ) + 1;
  g = p;
  int u = 1;
  return u;
}

int m3() {
  const int &p = int( 3 ) + 1;
  int u = p + 1;
  return u;
}

int m4() {
  int *&&p = new int&&( int( 3 ) + 1 );
  return 1;
}

int main() {
  int a = m2();
  std::cout << a << std::endl;
}
