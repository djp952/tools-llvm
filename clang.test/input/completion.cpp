#include "nonexistent_header.h"
struct X {
  int member;
  
  enum E { Val1 };
};

struct Y {
  float member;
  void memfunc(int i = 17);
};

/// Aaa.
struct Z : X, Y {
  __attribute__((annotate("some annotation"))) double member;	///< Bbb.
  operator int() const; ///< Ccc.
};
struct W { };
struct Z get_Z();
namespace N { }
void test_Z() {
  get_Z().member = 17;
}


float& overloaded(int i, long second);
double& overloaded(float f, int second);
int& overloaded(Z z, int second);
                
void test_overloaded() {
  overloaded(Z(), 0);
}

Z::operator int() const {
  return 0;
}
