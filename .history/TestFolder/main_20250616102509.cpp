#include <cstdio>

#include "TestHarness.hpp"

int main()
{
    TestHarness* testHarness = new TestHarness();

    //Run all tests
    //testHarness->runTestsFromFile(alltestsfile);

    //Rerun failed
    //testHarness->runTestsFromFile(failedfile);

    //Rerun passed
    //testHarness->runTestsFromFile(passedfile);

    //Single file
    testHarness->runTest("get_Software_Version_Correctness");

    return 0;
}