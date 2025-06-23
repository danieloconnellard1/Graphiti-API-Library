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
    testHarness->runTest("show_Message_On_The_Device_Functionality");

    //testHarness->runTestsFromFile("OtherTests.txt");

    return 0;
}