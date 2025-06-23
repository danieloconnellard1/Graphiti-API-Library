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
    testHarness->runTest("get_Single_Pixel_Position_Status_Correctness");

    //testHarness->runTestsFromFile("OtherTests.txt");

    return 0;
}