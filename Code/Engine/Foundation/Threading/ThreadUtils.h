#pragma once

#include <Foundation/Basics.h>
#include <Foundation/Threading/Implementation/ThreadingDeclarations.h>

/// \brief Contains general thread functions.
class EZ_FOUNDATION_DLL ezThreadUtils
{
public:

  /// \brief Suspends execution of the current thread.
  static void YieldTimeSlice();

  /// \brief Suspends the execution of the current thread for the given amount of time. (Precision may vary according to OS)
  static void Sleep(ezUInt32 uiMilliSeconds); // [tested]

  /// \brief Returns the current thread handle.
  static ezThreadHandle GetCurrentThreadHandle();

  /// \brief Helper function to check if the current thread is the main thread (e.g. the thread which initialized the foundation library)
  static bool IsMainThread();

private:
  EZ_MAKE_SUBSYSTEM_STARTUP_FRIEND(Foundation, ThreadUtils);

  /// \brief Initialization functionality of the threading system (called by foundation startup and thus private)
  static void Initialize();

  /// \brief Cleanup functionality of the threading system (called by foundation shutdown and thus private)
  static void Shutdown();
};

