
template <typename T, ezUInt32 Size>
ezHybridArrayBase<T, Size>::ezHybridArrayBase(ezAllocatorBase* pAllocator)
{
  this->m_pElements = GetStaticArray();
  this->m_uiCapacity = Size;
  this->m_pAllocator = pAllocator;
}

template <typename T, ezUInt32 Size>
ezHybridArrayBase<T, Size>::ezHybridArrayBase(const ezHybridArrayBase<T, Size>& other, ezAllocatorBase* pAllocator)
{
  this->m_pElements = GetStaticArray();
  this->m_uiCapacity = Size;
  this->m_pAllocator = pAllocator;

  *this = (ezArrayPtr<T>) other; // redirect this to the ezArrayPtr version
}

template <typename T, ezUInt32 Size>
ezHybridArrayBase<T, Size>::ezHybridArrayBase(const ezArrayPtr<T>& other, ezAllocatorBase* pAllocator)
{
  this->m_pElements = GetStaticArray();
  this->m_uiCapacity = Size;
  this->m_pAllocator = pAllocator;

  *this = other;
}

template <typename T, ezUInt32 Size>
ezHybridArrayBase<T, Size>::~ezHybridArrayBase()
{
  this->Clear();

  if (this->m_pElements != GetStaticArray())
    EZ_DELETE_RAW_BUFFER(this->m_pAllocator, this->m_pElements);

  this->m_pElements = NULL;
}

template <typename T, ezUInt32 Size>
EZ_FORCE_INLINE T* ezHybridArrayBase<T, Size>::GetStaticArray()
{
  return reinterpret_cast<T*>(m_StaticData);
}

template <typename T, ezUInt32 Size>
EZ_FORCE_INLINE void ezHybridArrayBase<T, Size>::operator= (const ezHybridArrayBase<T, Size>& rhs)
{
  *this = (ezArrayPtr<T>) rhs; // redirect this to the ezArrayPtr version
}

template <typename T, ezUInt32 Size>
void ezHybridArrayBase<T, Size>::operator= (const ezArrayPtr<T>& rhs)
{
  this->SetCount(rhs.GetCount());
  ezMemoryUtils::Copy(this->m_pElements, rhs.GetPtr(), rhs.GetCount());
}

template <typename T, ezUInt32 Size>
void ezHybridArrayBase<T, Size>::SetCapacity(ezUInt32 uiCapacity)
{
  T* pNewData = NULL;

  // if the static buffer is sufficient, use that
  if (uiCapacity <= Size)
  {
    this->m_uiCapacity = Size;

    // if we already use that static buffer, no reallocation is needed
    if (this->m_pElements == GetStaticArray())
      return;

    pNewData = GetStaticArray();
  }
  else
  {
    // otherwise allocate a new buffer
    this->m_uiCapacity = uiCapacity;
    pNewData = EZ_NEW_RAW_BUFFER(m_pAllocator, T, this->m_uiCapacity);
  }

  ezMemoryUtils::Construct(pNewData, this->m_pElements, this->m_uiCount);
  ezMemoryUtils::Destruct(this->m_pElements, this->m_uiCount);

  // if the previous buffer is not the static array, deallocate it
  if (this->m_pElements != GetStaticArray())
    EZ_DELETE_RAW_BUFFER(this->m_pAllocator, this->m_pElements);

  this->m_pElements = pNewData;
}

template <typename T, ezUInt32 Size>
void ezHybridArrayBase<T, Size>::Reserve(ezUInt32 uiCapacity)
{
  // m_uiCapacity will always be at least Size
  if (this->m_uiCapacity >= uiCapacity)
    return;

  ezUInt32 uiNewCapacity = ezMath::Max(this->m_uiCapacity + (this->m_uiCapacity / 2), uiCapacity);
  uiNewCapacity = (uiNewCapacity + (CAPACITY_ALIGNMENT-1)) & ~(CAPACITY_ALIGNMENT-1);

  SetCapacity(uiNewCapacity);
}

template <typename T, ezUInt32 Size>
void ezHybridArrayBase<T, Size>::Compact()
{
  if (this->IsEmpty())
  {
    // completely deallocate all data, if the array is empty.
    if (this->m_pElements != GetStaticArray())
      EZ_DELETE_RAW_BUFFER(this->m_pAllocator, this->m_pElements);

    this->m_uiCapacity = Size;
    this->m_pElements = GetStaticArray();
  }
  else
  {
    const ezUInt32 uiNewCapacity = (this->m_uiCount + (CAPACITY_ALIGNMENT-1)) & ~(CAPACITY_ALIGNMENT-1);
    if (this->m_uiCapacity != uiNewCapacity)
      SetCapacity(uiNewCapacity);
  }
}


template <typename T, ezUInt32 Size, typename A>
ezHybridArray<T, Size, A>::ezHybridArray() : ezHybridArrayBase<T, Size>(A::GetAllocator())
{
}

template <typename T, ezUInt32 Size, typename A>
ezHybridArray<T, Size, A>:: ezHybridArray(ezAllocatorBase* pAllocator) : ezHybridArrayBase<T, Size>(pAllocator)
{
}

template <typename T, ezUInt32 Size, typename A>
ezHybridArray<T, Size, A>::ezHybridArray(const ezHybridArray<T, Size, A>& other) : ezHybridArrayBase<T, Size>(other, A::GetAllocator())
{
}

template <typename T, ezUInt32 Size, typename A>
ezHybridArray<T, Size, A>:: ezHybridArray(const ezHybridArrayBase<T, Size>& other) : ezHybridArrayBase<T, Size>(other, A::GetAllocator())
{
}

template <typename T, ezUInt32 Size, typename A>
ezHybridArray<T, Size, A>::ezHybridArray(const ezArrayPtr<T>& other) : ezHybridArrayBase<T, Size>(other, A::GetAllocator())
{
}

template <typename T, ezUInt32 Size, typename A>
void ezHybridArray<T, Size, A>::operator=(const ezHybridArray<T, Size, A>& rhs)
{
  ezHybridArrayBase<T, Size>::operator=(rhs);
}

template <typename T, ezUInt32 Size, typename A>
void ezHybridArray<T, Size, A>::operator=(const ezHybridArrayBase<T, Size>& rhs)
{
  ezHybridArrayBase<T, Size>::operator=(rhs);
}

template <typename T, ezUInt32 Size, typename A>
void ezHybridArray<T, Size, A>::operator=(const ezArrayPtr<T>& rhs)
{
  ezHybridArrayBase<T, Size>::operator=(rhs);
}

