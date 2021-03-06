#pragma once

#include <Foundation/Basics.h>

namespace ezMemoryPolicies
{
  /// \brief Default heap memory allocation policy.
  ///
  /// \see ezAllocator
  class ezHeapAllocation
  {
  public:
    EZ_FORCE_INLINE ezHeapAllocation(ezAllocatorBase* pParent) { }
    EZ_FORCE_INLINE ~ezHeapAllocation() { }

    EZ_FORCE_INLINE void* Allocate(size_t uiSize, size_t uiAlign)
    {
      void* ptr = malloc(uiSize);
      EZ_CHECK_ALIGNMENT(ptr, uiAlign);

      return ptr;
    }

    EZ_FORCE_INLINE void Deallocate(void* ptr)
    {
      free(ptr);
    }
     
    EZ_FORCE_INLINE ezAllocatorBase* GetParent() const { return NULL; }
  };
}

