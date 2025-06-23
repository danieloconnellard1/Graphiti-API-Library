#pragma once
#include <cstdio>

#include <map>
#include <functional>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

inline const std::string alltestsfile = "Tests.txt";
inline const std::string passedfile = "passedTests.txt";
inline const std::string failedfile = "failedTests.txt";
inline const std::string testDataDir = "TestFolder/TestData/";

inline const std::string displayTests = "DisplayTests.txt";
inline const std::string drawTests = "DrawTests.txt";
inline const std::string imageTests = "ImageTests.txt";
inline const std::string keyTests = "KeyTests.txt";
inline const std::string statusTests = "StatusTests.txt";
inline const std::string stringTests = "StringTests.txt";

namespace fs = std::filesystem;
using std::endl;
using std::cerr;
using std::cout;

#include "Tests.hpp"

class TestHarness {
public:

    TestHarness();

    /**
     * @brief Runs a single test
     * 
     * Outputs to passedTests and failedTests in TestData folder
     * 
     * Test must exist in testRegistry
     * 
     * @param testName name of test
     */
    void runTest(const std::string& testName);

    /**
     * @brief Runs tests from a file seperated by new lines
     * 
     * Outputs to passedTests and failedTests in TestData folder
     * 
     * Tests must exist in testRegistry
     * 
     * @param targetTests string name of tests file in TestData folder
     */
    void runTestsFromFile(
        const std::string& targetTests
    );

private:

    Tests* test;

    bool loopCondition = true;

    /**
     * @brief Checks the TestFolder exists
     * 
     */
    void ensureTestDirectoryExists();

    /**
     * @brief Used to remove a test
     * 
     * @param filename testfile
     * @param lineToRemove testname
     */
    void removeLineFromFile(
        const std::string& filename, 
        const std::string& lineToRemove
    );


};