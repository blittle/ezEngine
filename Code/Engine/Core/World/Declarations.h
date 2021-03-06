#pragma once

#include <Foundation/Basics/Types/Bitflags.h>
#include <Foundation/Basics/Types/Id.h>
#include <Foundation/Math/Mat4.h>
#include <Foundation/Math/Quat.h>

#include <Core/Basics.h>

class ezWorld;
namespace ezInternal
{
  struct WorldData;
}

class ezGameObject;
struct ezGameObjectDesc;

class ezComponentManagerBase;
class ezComponent;

struct ezGameObjectId
{
  typedef ezUInt32 StorageType;

  EZ_DECLARE_ID_TYPE(ezGameObjectId, 20);

  EZ_FORCE_INLINE ezGameObjectId(StorageType instanceIndex, StorageType generation, 
    StorageType worldIndex = 0)
  {
    m_Data = 0;
    m_InstanceIndex = instanceIndex;
    m_Generation = generation;
    m_WorldIndex = worldIndex;
  }

  union
  {
    StorageType m_Data;
    struct
    {
      StorageType m_InstanceIndex : 20;
      StorageType m_Generation : 6;
      StorageType m_WorldIndex : 6;
    };
  };
};

class ezGameObjectHandle
{
  EZ_DECLARE_HANDLE_TYPE(ezGameObjectHandle, ezGameObjectId);

  friend class ezWorld;
  friend class ezGameObject;
};

typedef ezGenericId<24, 8> ezGenericComponentId;

struct ezComponentId : public ezGenericComponentId
{
  EZ_FORCE_INLINE ezComponentId() : ezGenericComponentId()
  {
    m_TypeId = 0;
  }

  EZ_FORCE_INLINE ezComponentId(StorageType instanceIndex, StorageType generation, ezUInt16 typeId = 0) : 
    ezGenericComponentId(instanceIndex, generation)
  {
    m_TypeId = typeId;
  }

  EZ_FORCE_INLINE ezComponentId(ezGenericComponentId genericId, ezUInt16 typeId) : 
    ezGenericComponentId(genericId)
  {
    m_TypeId = typeId;
  }

  ezUInt16 m_TypeId;
};

class ezComponentHandle
{
  EZ_DECLARE_HANDLE_TYPE(ezComponentHandle, ezComponentId);

  friend class ezWorld;
  friend class ezComponentManagerBase;
  friend class ezComponent;
};

struct ezObjectFlags
{
  typedef ezUInt32 StorageType;

  enum Enum
  {
    Dynamic = EZ_BIT(0),
    Active  = EZ_BIT(1),

    Default = Dynamic | Active
  };

  struct Bits
  {
    StorageType Dynamic : 1;
    StorageType Active : 1;
  };
};

EZ_DECLARE_FLAGS_OPERATORS(ezObjectFlags);

struct ezObjectMsgRouting
{
  typedef ezUInt32 StorageType;

  enum Enum
  {
    ToParent   = EZ_BIT(0),
    ToChildren = EZ_BIT(1),

    Direct     = 0,
    QueuedForPostAsync = EZ_BIT(2),
    QueuedForNextFrame = EZ_BIT(3),

    Default    = Direct
  };

  struct Bits
  {
    StorageType ToParent : 1;
    StorageType ToChildren : 1;
    StorageType QueuedForPostAsync : 1;
    StorageType QueuedForNextFrame : 1;
  };
};

EZ_DECLARE_FLAGS_OPERATORS(ezObjectMsgRouting);

