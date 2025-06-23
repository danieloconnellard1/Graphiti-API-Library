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

    //Single test
    testHarness->runTest("get_Date_And_Time_Correctness");

    //testHarness->runTestsFromFile("OtherTests.txt");

    return 0;
}