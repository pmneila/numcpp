#include "externTemplate.h"

int main()
{
    int i = foo<int>(3);

    Bar<int> h = Bar<int>(5);
    Bar<double> u = Bar<double>(5);

    return 0;
}
