#pragma once

#include <TestFramework/Basics.h>
#include <Foundation/Profiling/Profiling.h>
#include <TestFramework/Framework/Declarations.h>
#include <TestFramework/Framework/TestBaseClass.h>
#include <TestFramework/Framework/SimpleTest.h>
#include <TestFramework/Utilities/TestOrder.h>
#include <TestFramework/Framework/TestResults.h>


class EZ_TEST_DLL ezTestFramework
{
public:
  ezTestFramework(const char* szTestName, const char* szAbsTestDir, int argc, const char** argv);
  virtual ~ezTestFramework();

  typedef void(*OutputHandler)(ezTestOutput::Enum Type, const char* szMsg);

  // Test management
  void CreateOutputFolder();
  const char* GetTestName() const;
  const char* GetAbsOutputPath() const;
  void RegisterOutputHandler(OutputHandler Handler);
  void GatherAllTests();
  void LoadTestOrder();
  void SaveTestOrder();
  void SetAllTestsEnabledStatus(bool bEnable);

  void GetTestSettingsFromCommandLine(int argc, const char** argv);

  // Test execution
  void ResetTests();
  void ExecuteAllTests();

  void StartTests();
  void ExecuteTest(ezUInt32 uiTestIndex);
  void EndTests();

  // Test queries
  ezUInt32 GetTestCount() const;
  ezUInt32 GetTestEnabledCount() const;
  ezUInt32 GetSubTestEnabledCount(ezUInt32 uiTestIndex) const;
  bool IsTestEnabled(ezUInt32 uiTestIndex) const;
  bool IsSubTestEnabled(ezUInt32 uiTestIndex, ezUInt32 uiSubTestIndex) const;
  void SetTestEnabled(ezUInt32 uiTestIndex, bool bEnabled);
  void SetSubTestEnabled(ezUInt32 uiTestIndex, ezUInt32 uiSubTestIndex, bool bEnabled);

  ezInt32 GetCurrentTestIndex() {return m_iCurrentTestIndex;}
  ezInt32 GetCurrentSubTestIndex() {return m_iCurrentSubTestIndex;}
  ezTestEntry* GetTest(ezUInt32 uiTestIndex);
  bool GetTestsRunning() const { return m_bTestsRunning; }

  // Global settings
  TestSettings GetSettings() const;
  void SetSettings(const TestSettings& settings);

  // Test results
  ezTestFrameworkResult& GetTestResult();
  ezInt32 GetTotalErrorCount() const;
  ezInt32 GetTestsPassedCount() const;
  ezInt32 GetTestsFailedCount() const;
  double GetTotalTestDuration() const;

protected:
  virtual void ErrorImpl(const char* szError, const char* szFile, ezInt32 iLine, const char* szFunction, const char* szMsg);
  virtual void OutputImpl(ezTestOutput::Enum Type, const char* szMsg);
  virtual void TestResultImpl(ezInt32 iSubTestIndex, bool bSuccess, double fDuration);

  // ignore this for now
public:
  static const char* s_szTestBlockName;

  // static functions
public:
  static ezTestFramework* GetInstance() { return s_pInstance; }

  /// \brief Returns whether to asset on test fail, will return false if no debugger is attached to prevent crashing. 
  static bool GetAssertOnTestFail();

  static void Output(ezTestOutput::Enum Type, const char* szMsg, ...);
  static void Error(const char* szError, const char* szFile, ezInt32 iLine, const char* szFunction, const char* szMsg);
  static void TestResult(ezInt32 iSubTestIndex, bool bSuccess, double fDuration);

  // static members
private:
  static ezTestFramework* s_pInstance;
  
private:
  std::string m_sTestName;  ///< The name of the tests being done
  std::string m_sAbsTestDir; ///< Absolute path to the output folder where results and temp data is stored
  ezInt32 m_iErrorCount;
  ezInt32 m_iTestsFailed;
  ezInt32 m_iTestsPassed;
  TestSettings m_Settings;
  std::deque<OutputHandler> m_OutputHandlers;
  std::deque<ezTestEntry> m_TestEntries;
  ezTestFrameworkResult m_Result;
  ezAssertHandler m_PreviousAssertHandler;

protected:
  ezInt32 m_iCurrentTestIndex;
  ezInt32 m_iCurrentSubTestIndex;
  bool m_bTestsRunning;
};

/// \brief Enum for usage in EZ_TEST_BLOCK to enable or disable the block.
struct ezTestBlock
{
  /// \brief Enum for usage in EZ_TEST_BLOCK to enable or disable the block.
  enum Enum
  {
    Disabled, ///< The test block will be skipped. The testframework will print a warning message, that some block is deactivated.
    Enabled   ///< The test block is enabled.
  };
};

/// \brief Starts a small test block inside a larger test. 
///
/// First parameter allows to quickly disable a block depending on a condition (e.g. platform). 
/// Second parameter just gives it a name for better error reporting.
/// Also skipped tests are highlighted in the output, such that people can quickly see when a test is currently deactivated.
#define EZ_TEST_BLOCK(enable, name) \
  ezTestFramework::s_szTestBlockName = name; \
  if (enable == ezTestBlock::Disabled) \
  { \
    ezTestFramework::s_szTestBlockName = ""; \
    ezTestFramework::Output(ezTestOutput::Message, "Skipped Test Block '%s'", name); \
  } \
  else

#define EZ_TEST_FAILURE(erroroutput, msg) \
{\
  ezTestFramework::Error(erroroutput, EZ_SOURCE_FILE, EZ_SOURCE_LINE, EZ_SOURCE_FUNCTION, msg);\
}

/// \brief Will trigger a debug break, if the test framework is configured to do so on test failure
#define EZ_TEST_DEBUG_BREAK if (ezTestFramework::GetAssertOnTestFail())\
  EZ_DEBUG_BREAK

/// \brief Tests for a boolean condition, does not output an extra message.
#define EZ_TEST_BOOL(condition) EZ_TEST_BOOL_MSG(condition, "")

/// \brief Tests for a boolean condition, outputs a custom message on failure.
#define EZ_TEST_BOOL_MSG(condition, msg) if (!(condition)) \
{ \
  EZ_TEST_FAILURE("Test failed: " EZ_STRINGIZE(condition), msg) \
  EZ_TEST_DEBUG_BREAK \
}

inline float ToFloat(int f) { return (float) f; }
inline float ToFloat(float f) { return f; }
inline float ToFloat(double f) { return (float) f; }

/// \brief Tests two floats for equality, within a given epsilon. On failure both actual and expected values are output.
#define EZ_TEST_FLOAT(f1, f2, epsilon) EZ_TEST_FLOAT_MSG(f1, f2, epsilon, "")

/// \brief Tests two floats for equality, within a given epsilon. On failure both actual and expected values are output, also a custom message is printed.
#define EZ_TEST_FLOAT_MSG(f1, f2, epsilon, msg) \
{ \
  const float internal_r1 = ToFloat(f1); \
  const float internal_r2 = ToFloat(f2); \
  const float internal_fD = internal_r1 - internal_r2; \
  const float internal_fEps = ToFloat(epsilon); \
  if (internal_fD < -internal_fEps || internal_fD > +internal_fEps) \
  { \
    char szLocal_TestMacro[256]; \
    sprintf (szLocal_TestMacro, "Failure: '%s' (%.8f) does not equal '%s' (%.8f) within an epsilon of %.8f", EZ_STRINGIZE(f1), internal_r1, EZ_STRINGIZE(f2), internal_r2, internal_fEps); \
    EZ_TEST_FAILURE(szLocal_TestMacro, msg); \
    EZ_TEST_DEBUG_BREAK \
  } \
}

/// \brief Tests two doubles for equality, within a given epsilon. On failure both actual and expected values are output.
#define EZ_TEST_DOUBLE(f1, f2, epsilon) EZ_TEST_DOUBLE_MSG(f1, f2, epsilon, "")

/// \brief Tests two doubles for equality, within a given epsilon. On failure both actual and expected values are output, also a custom message is printed.
#define EZ_TEST_DOUBLE_MSG(f1, f2, epsilon, msg) \
{ \
  const double internal_r1 = (f1); \
  const double internal_r2 = (f2); \
  const double internal_fD = internal_r1 - internal_r2; \
  const double internal_fEps = (double) epsilon; \
  if (internal_fD < -internal_fEps || internal_fD > +internal_fEps) \
  { \
    char szLocal_TestMacro[256]; \
    sprintf (szLocal_TestMacro, "Failure: '%s' (%.8f) does not equal '%s' (%.8f) within an epsilon of %.8f", EZ_STRINGIZE(f1), internal_r1, EZ_STRINGIZE(f2), internal_r2, internal_fEps); \
    EZ_TEST_FAILURE(szLocal_TestMacro, msg); \
    EZ_TEST_DEBUG_BREAK \
  } \
}

/// \brief Tests two ints for equality. On failure both actual and expected values are output.
#define EZ_TEST_INT(i1, i2) EZ_TEST_INT_MSG(i1, i2, "")

/// \brief Tests two ints for equality. On failure both actual and expected values are output, also a custom message is printed.
#define EZ_TEST_INT_MSG(i1, i2, msg) \
{ \
  const ezInt32 internal_r1 = (ezInt32) (i1); \
  const ezInt32 internal_r2 = (ezInt32) (i2); \
  \
  if (internal_r1 != internal_r2) \
  { \
    char szLocal_TestMacro[256]; \
    sprintf (szLocal_TestMacro, "Failure: '%s' (%i) does not equal '%s' (%i)", EZ_STRINGIZE(i1), internal_r1, EZ_STRINGIZE(i2), internal_r2); \
    EZ_TEST_FAILURE(szLocal_TestMacro, msg); \
    EZ_TEST_DEBUG_BREAK \
  } \
}

/// \brief Tests two strings for equality. On failure both actual and expected values are output.
#define EZ_TEST_STRING(i1, i2) EZ_TEST_STRING_MSG(i1, i2, "")

/// \brief Tests two strings for equality. On failure both actual and expected values are output, also a custom message is printed.
#define EZ_TEST_STRING_MSG(s1, s2, msg) \
{ \
  const char* internal_sz1 = s1; \
  const char* internal_sz2 = s2; \
  \
  if (strcmp(internal_sz1, internal_sz2) != 0) \
  { \
    char szLocal_TestMacro[512]; \
    sprintf (szLocal_TestMacro, "Failure: '%s' (%s) does not equal '%s' (%s)", EZ_STRINGIZE(internal_s1), internal_sz1, EZ_STRINGIZE(internal_s2), internal_sz2); \
    EZ_TEST_FAILURE(szLocal_TestMacro, msg); \
    EZ_TEST_DEBUG_BREAK \
  } \
}

/// \brief Tests two ezVec2's for equality, using some epsilon. On failure both actual and expected values are output.
#define EZ_TEST_VEC2(i1, i2, epsilon) EZ_TEST_VEC2_MSG(i1, i2, epsilon, "")

/// \brief Tests two ezVec2's for equality. On failure both actual and expected values are output, also a custom message is printed.
#define EZ_TEST_VEC2_MSG(r1, r2, epsilon, msg) \
{ \
  const ezVec2T internal_v1 = (ezVec2T) (r1); \
  const ezVec2T internal_v2 = (ezVec2T) (r2); \
  \
  EZ_TEST_FLOAT_MSG(internal_v1.x, internal_v2.x, epsilon, msg); \
  EZ_TEST_FLOAT_MSG(internal_v1.y, internal_v2.y, epsilon, msg); \
}

/// \brief Tests two ezVec3's for equality, using some epsilon. On failure both actual and expected values are output.
#define EZ_TEST_VEC3(i1, i2, epsilon) EZ_TEST_VEC3_MSG(i1, i2, epsilon, "")

/// \brief Tests two ezVec3's for equality. On failure both actual and expected values are output, also a custom message is printed.
#define EZ_TEST_VEC3_MSG(r1, r2, epsilon, msg) \
{ \
  const ezVec3T internal_v1 = (ezVec3T) (r1); \
  const ezVec3T internal_v2 = (ezVec3T) (r2); \
  \
  EZ_TEST_FLOAT_MSG(internal_v1.x, internal_v2.x, epsilon, msg); \
  EZ_TEST_FLOAT_MSG(internal_v1.y, internal_v2.y, epsilon, msg); \
  EZ_TEST_FLOAT_MSG(internal_v1.z, internal_v2.z, epsilon, msg); \
}

/// \brief Tests two ezVec4's for equality, using some epsilon. On failure both actual and expected values are output.
#define EZ_TEST_VEC4(i1, i2, epsilon) EZ_TEST_VEC4_MSG(i1, i2, epsilon, "")

/// \brief Tests two ezVec4's for equality. On failure both actual and expected values are output, also a custom message is printed.
#define EZ_TEST_VEC4_MSG(r1, r2, epsilon, msg) \
{ \
  const ezVec4T internal_v1 = (ezVec4T) (r1); \
  const ezVec4T internal_v2 = (ezVec4T) (r2); \
  \
  EZ_TEST_FLOAT_MSG(internal_v1.x, internal_v2.x, epsilon, msg); \
  EZ_TEST_FLOAT_MSG(internal_v1.y, internal_v2.y, epsilon, msg); \
  EZ_TEST_FLOAT_MSG(internal_v1.z, internal_v2.z, epsilon, msg); \
  EZ_TEST_FLOAT_MSG(internal_v1.w, internal_v2.w, epsilon, msg); \
}

