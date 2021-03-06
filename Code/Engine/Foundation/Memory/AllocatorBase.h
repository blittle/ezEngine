#pragma once

/// \file

#include <Foundation/Basics.h>
#include <Foundation/Basics/Types/ArrayPtr.h>

#ifdef new
  #undef new
#endif

#ifdef delete
  #undef delete
#endif

/// \brief Base class for all memory allocators.
class EZ_FOUNDATION_DLL ezAllocatorBase
{
public:
  struct Stats
  {
    EZ_DECLARE_POD_TYPE();

    EZ_FORCE_INLINE Stats() { ezMemoryUtils::ZeroFill(this); }

    ezUInt64 m_uiNumAllocations;      ///< total number of allocations
    ezUInt64 m_uiNumDeallocations;    ///< total number of deallocations
    ezUInt64 m_uiAllocationSize;      ///< total allocation size in bytes
  };

  ezAllocatorBase();
  virtual ~ezAllocatorBase();

  /// \brief Interface, do not use this directly, always use the new/delete macros below
  virtual void* Allocate(size_t uiSize, size_t uiAlign) = 0;
  virtual void Deallocate(void* ptr) = 0;
  virtual size_t AllocatedSize(const void* ptr) = 0;

  virtual Stats GetStats() const = 0;

private:
  EZ_DISALLOW_COPY_AND_ASSIGN(ezAllocatorBase);
};

#include <Foundation/Memory/Implementation/AllocatorBase_inl.h>

/// \brief creates a new instance of type using the given allocator
#define EZ_NEW(allocator, type) \
  new ((allocator)->Allocate(sizeof(type), EZ_ALIGNMENT_OF(type))) type

/// \brief deletes the instance stored in ptr using the given allocator and sets ptr to NULL
#define EZ_DELETE(allocator, ptr) \
  { ezInternal::Delete(allocator, ptr); ptr = NULL; }

/// \brief creates a new array of type using the given allocator with count elements, calls default constructor for non-POD types
#define EZ_NEW_ARRAY(allocator, type, count) \
  ezInternal::CreateArray<type>(allocator, count)

/// \brief Calls destructor on every element for non-POD types and deletes the array stored in arrayPtr using the given allocator
#define EZ_DELETE_ARRAY(allocator, arrayPtr) \
  { ezInternal::DeleteArray(allocator, arrayPtr); arrayPtr.Reset(); }

/// \brief creates a raw buffer of type using the given allocator with count elements, but does NOT call the default constructor
#define EZ_NEW_RAW_BUFFER(allocator, type, count) \
  ezInternal::CreateRawBuffer<type>(allocator, count)

/// \brief deletes a raw buffer stored in ptr using the given allocator, but does NOT call destructor
#define EZ_DELETE_RAW_BUFFER(allocator, ptr) \
  { ezInternal::DeleteRawBuffer(allocator, ptr); ptr = NULL; }



/// \brief creates a new instance of type using the default allocator
#define EZ_DEFAULT_NEW(type) EZ_NEW(ezFoundation::GetDefaultAllocator(), type)

/// \brief deletes the instance stored in ptr using the default allocator and sets ptr to NULL
#define EZ_DEFAULT_DELETE(ptr) EZ_DELETE(ezFoundation::GetDefaultAllocator(), ptr)

/// \brief creates a new array of type using the default allocator with count elements, calls default constructor for non-POD types
#define EZ_DEFAULT_NEW_ARRAY(type, count) EZ_NEW_ARRAY(ezFoundation::GetDefaultAllocator(), type, count)

/// \brief calls destructor on every element for non-POD types and deletes the array stored in arrayPtr using the default allocator
#define EZ_DEFAULT_DELETE_ARRAY(arrayPtr) EZ_DELETE_ARRAY(ezFoundation::GetDefaultAllocator(), arrayPtr)

/// \brief creates a raw buffer of type using the default allocator with count elements, but does NOT call the default constructor
#define EZ_DEFAULT_NEW_RAW_BUFFER(type, count) EZ_NEW_RAW_BUFFER(ezFoundation::GetDefaultAllocator(), type, count)

/// \brief deletes a raw buffer stored in ptr using the default allocator, but does NOT call destructor
#define EZ_DEFAULT_DELETE_RAW_BUFFER(ptr) EZ_DELETE_RAW_BUFFER(ezFoundation::GetDefaultAllocator(), ptr)


