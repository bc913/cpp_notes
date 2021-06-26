#include <string>
#include "bc_tuple.h"

void test_bc_tuple()
{
    using namespace bc;
    tuple<> t0;
    tuple<int> t1(1);
    tuple<int, float> t2(1, 3.14159f);
    tuple<int, float, const char*> t3a(1, 3.14159f, "Hello, world!");
    tuple<long, double, std::string> t3b(t3a);
    t3b = t3a;
    //  t3a = t3b; DPG: triggers an error, as it should.

    // TODO: Fails. Investigate
    //tuple<int, float, std::string> t3c = make_tuple(17, 2.718281828, std::string("Fun"));

    int seventeen = 17;
    double pi = 3.14159;
    tuple<int&, double&> seventeen_pi = make_tuple(ref(seventeen), ref(pi));
    tuple<int&, const double&> seventeen_pi2 =
    make_tuple(ref(seventeen), cref(pi));
    
    assert(get<0>(t3a) == 1);
    assert(get<1>(t3a) == 3.14159f);
    assert(std::strcmp(get<2>(t3a), "Hello, world!") == 0);

}