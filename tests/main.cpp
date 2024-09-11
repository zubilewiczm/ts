#include "UnitTest++/UnitTestPP.h"
#include "UnitTest++/TestReporterStdout.h"

#include <iostream>

using namespace UnitTest;

int main(int, char const *[])
{
  TestReporterStdout rep;

  auto all_suites = {"tsInit", "tsNames"};
  auto len = all_suites.size();
  int cur = 0;
  int res = 0;

  for (const char* suite : all_suites)
  {
    TestRunner run(rep);

    cur+= 1;
    std::cout
      << "[" << cur << "/" << len << "] → "
      << suite << " : ";
    res += run.RunTestsIf(Test::GetTestList(), suite, True(), 0);
    std::cout << std::endl;
  }
  std::cout << "Total tests failed: " << res << std::endl;
  return res;
}
