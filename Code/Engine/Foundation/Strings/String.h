#pragma once

#include <Foundation/Strings/StringUtils.h>
#include <Foundation/Strings/StringIterator.h>
#include <Foundation/Strings/Implementation/StringBase.h>
#include <Foundation/Strings/StringConversion.h>
#include <Foundation/Strings/StringBuilder.h>
#include <Foundation/Containers/HybridArray.h>

/// \brief A string class for storing and passing around strings.
///
/// This class only allows read-access to its data. It does not allow modifications.
/// To build / modify strings, use the ezStringBuilder class.
/// ezHybridString has an internal array to store short strings without any memory allocations, it will dynamically
/// allocate additional memory, if that cache is insufficient. Thus a hybrid string will always take up a certain amount
/// of memory, which might be of concern when it is used as a member variable, in such cases you might want to use an
/// ezHybridString with a very small internal array (1 would basically make it into a completely dynamic string).
/// On the other hand, creating ezHybridString instances on the stack and working locally with them, is quite fast.
/// Prefer to use the typedef'ed string types \a ezString, \a ezDynamicString, \a ezString32 etc.
/// Most strings in an application are rather short, typically shorter than 20 characters.
/// Use \a ezString, which is a typedef'ed ezHybridString to use a cache size that is sufficient for more than 90%
/// of all use cases.
template <ezUInt16 Size>
class ezHybridStringBase : public ezStringBase<ezHybridStringBase<Size> >
{
protected:

  /// \brief Creates an empty string.
  ezHybridStringBase(ezAllocatorBase* pAllocator); // [tested]

  /// \brief Copies the data from \a rhs.
  ezHybridStringBase(const ezHybridStringBase& rhs, ezAllocatorBase* pAllocator); // [tested]

  /// \brief Copies the data from \a rhs.
  ezHybridStringBase(const char* rhs, ezAllocatorBase* pAllocator); // [tested]

  /// \brief Copies the data from \a rhs.
  ezHybridStringBase(const wchar_t* rhs, ezAllocatorBase* pAllocator); // [tested]

  /// \brief Copies the data from \a rhs.
  ezHybridStringBase(const ezStringIterator& rhs, ezAllocatorBase* pAllocator); // [tested]

  /// \brief Copies the data from \a rhs.
  ezHybridStringBase(const ezStringBuilder& rhs, ezAllocatorBase* pAllocator); // [tested]

  /// \brief Destructor.
  ~ezHybridStringBase(); // [tested]

  /// \brief Copies the data from \a rhs.
  void operator=(const ezHybridStringBase& rhs); // [tested]

  /// \brief Copies the data from \a rhs.
  void operator=(const char* rhs); // [tested]

  /// \brief Copies the data from \a rhs.
  void operator=(const wchar_t* rhs); // [tested]

  /// \brief Copies the data from \a rhs.
  void operator=(const ezStringIterator& rhs); // [tested]

  /// \brief Copies the data from \a rhs.
  void operator=(const ezStringBuilder& rhs); // [tested]

public:

  /// \brief Resets this string to an empty string.
  ///
  /// This will not deallocate any previously allocated data, but reuse that memory.
  void Clear(); // [tested]

  /// \brief Returns a pointer to the internal Utf8 string.
  const char* GetData() const; // [tested]

  /// \brief Returns the amount of bytes that this string takes (excluding the '\0' terminator).
  ezUInt32 GetElementCount() const; // [tested]

  /// \brief Returns the number of characters in this string.
  ezUInt32 GetCharacterCount() const; // [tested]

  /// \brief Returns an iterator to this string, which points to the very first character.
  ///
  /// Note that this iterator will only be valid as long as this ezHybridString lives.
  /// Once the original string is destroyed, all iterators to them will point into invalid memory.
  ezStringIterator GetIteratorFront() const; // [tested]

  /// \brief Returns an iterator to this string, which points to the very last character (NOT the end).
  ///
  /// Note that this iterator will only be valid as long as this ezHybridString lives.
  /// Once the original string is destroyed, all iterators to them will point into invalid memory.
  ezStringIterator GetIteratorBack() const; // [tested]

  /// \brief Returns an iterator to a sub-string of this string, starting at character uiFirstCharacter, up until uiFirstCharacter +  uiNumCharacters.
  ///
  /// Note that this iterator will only be valid as long as this ezHybridString lives.
  /// Once the original string is destroyed, all iterators to them will point into invalid memory.
  ezStringIterator GetSubString(ezUInt32 uiFirstCharacter, ezUInt32 uiNumCharacters) const; // [tested]

  /// \brief Returns an iterator to the sub-string containing the first uiNumCharacters characters of this string.
  ///
  /// Note that this iterator will only be valid as long as this ezHybridString lives.
  /// Once the original string is destroyed, all iterators to them will point into invalid memory.
  ezStringIterator GetFirst(ezUInt32 uiNumCharacters) const; // [tested]

  /// \brief Returns an iterator to the sub-string containing the last uiNumCharacters characters of this string.
  ///
  /// Note that this iterator will only be valid as long as this ezHybridString lives.
  /// Once the original string is destroyed, all iterators to them will point into invalid memory.
  ezStringIterator GetLast(ezUInt32 uiNumCharacters) const; // [tested]

private:
  ezHybridArray<char, Size> m_Data;
  ezUInt32 m_uiCharacterCount;
};


/// \brief \see ezHybridStringBase
template <ezUInt16 Size, typename AllocatorWrapper = ezDefaultAllocatorWrapper>
class ezHybridString : public ezHybridStringBase<Size>
{
public:
  ezHybridString();
  ezHybridString(ezAllocatorBase* pAllocator);

  ezHybridString(const ezHybridString<Size, AllocatorWrapper>& other);
  ezHybridString(const ezHybridStringBase<Size>& other);
  ezHybridString(const char* rhs);
  ezHybridString(const wchar_t* rhs);
  ezHybridString(const ezStringIterator& rhs);
  ezHybridString(const ezStringBuilder& rhs);

  void operator=(const ezHybridString<Size, AllocatorWrapper>& rhs);
  void operator=(const ezHybridStringBase<Size>& rhs);
  void operator=(const char* szString);
  void operator=(const wchar_t* szString);
  void operator=(const ezStringIterator& rhs);
  void operator=(const ezStringBuilder& rhs);
};


typedef ezHybridString<1> ezDynamicString;
typedef ezHybridString<32> ezString;
typedef ezHybridString<16> ezString16;
typedef ezHybridString<24> ezString24;
typedef ezHybridString<32> ezString32;
typedef ezHybridString<48> ezString48;
typedef ezHybridString<64> ezString64;
typedef ezHybridString<128> ezString128;
typedef ezHybridString<256> ezString256;

#include <Foundation/Strings/Implementation/String_inl.h>

