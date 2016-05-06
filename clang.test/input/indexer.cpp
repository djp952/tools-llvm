#include <stdio.h>
#include "header1.h"
#include "header2.h"

template<typename T>
class MyTemplate
{
public:

	MyTemplate() {}
};

class BaseClass
{
protected:

	BaseClass() {}
};

class DerivedClass : public BaseClass
{
public:

	DerivedClass() : BaseClass() {}
};

void no_return_value(int in)
{
	// this generates at least one diagnostic for the unit tests
	return in;
}

int main(int argc, char* argv[]) {

	MyTemplate<int> templ();

    printf("hello world\n");
	no_return_value(12345);
    return 0;
}
