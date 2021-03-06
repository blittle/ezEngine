#pragma once

#include <Foundation/Math/Float16.h>
#include <Foundation/Math/Color.h>

/// \brief A 16bit per channel float color storage format.
/// 
/// For any calculations or conversions use ezColor.
/// \see ezColor
class ezColor16f
{
public:
  // Means that colors can be copied using memcpy instead of copy construction.
  EZ_DECLARE_POD_TYPE();


  // *** Data ***
public:

  ezFloat16 r;
  ezFloat16 g;
  ezFloat16 b;
  ezFloat16 a;

  // *** Constructors ***
public:

  /// \brief default-constructed color is uninitialized (for speed)
  ezColor16f(); // [tested]

  /// \brief Initializes the color with r, g, b, a
  ezColor16f(ezFloat16 r, ezFloat16 g, ezFloat16 b, ezFloat16 a); // [tested]

  /// \brief Initializes the color with ezColor
  ezColor16f(const ezColor& color); // [tested]

  // no copy-constructor and operator= since the default-generated ones will be faster

  // *** Functions ***
public:

  /// \brief Conversion to ezColor.
  operator ezColor () const; // [tested]

  /// \brief Conversion to const ezFloat16*.
  operator const ezFloat16* () const { return &r; }

  /// \brief Conversion to ezFloat16* - use with care!
  operator ezFloat16* () { return &r; }

  /// \brief Conversion to const ezUInt64.
  operator ezUInt64 () const { return *reinterpret_cast<const ezUInt64*>(&r); }

  /// \brief Equality Check (bitwise)
  bool IsIdentical(const ezColor16f& rhs) const; // [tested]
};

/// \brief Returns true, if both colors are identical in all components.
bool operator== (const ezColor16f& c1, const ezColor16f& c2); // [tested]

/// \brief Returns true, if both colors are not identical in all components.
bool operator!= (const ezColor16f& c1, const ezColor16f& c2); // [tested]

#include <Foundation/Math/Implementation/Color16f_inl.h>


