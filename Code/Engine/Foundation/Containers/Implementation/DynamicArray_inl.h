
template <typename T>
ezDynamicArrayBase<T>::ezDynamicArrayBase(ezAllocatorBase* pAllocator)
{
  this->m_uiCapacity = 0;
  m_pAllocator = pAllocator;
}

template <typename T>
ezDynamicArrayBase<T>::ezDynamicArrayBase(const ezDynamicArrayBase<T>& other, ezAllocatorBase* pAllocator)
{
  this->m_uiCapacity = 0;
  m_pAllocator = pAllocator;

  *this = (ezArrayPtr<T>) other; // redirect this to the ezArrayPtr version
}

template <typename T>
ezDynamicArrayBase<T>::ezDynamicArrayBase(const ezArrayPtr<T>& other, ezAllocatorBase* pAllocator)
{
  this->m_uiCapacity = 0;
  m_pAllocator = pAllocator;

  *this = other;
}

template <typename T>
ezDynamicArrayBase<T>::~ezDynamicArrayBase()
{
  this->Clear();
  EZ_DELETE_RAW_BUFFER(this->m_pAllocator, this->m_pElements);
  this->m_uiCapacity = 0;
}

template <typename T>
EZ_FORCE_INLINE void ezDynamicArrayBase<T>::operator= (const ezDynamicArrayBase<T>& rhs)
{
  *this = (ezArrayPtr<T>) rhs; // redirect this to the ezArrayPtr version
}

template <typename T>
void ezDynamicArrayBase<T>::operator= (const ezArrayPtr<T>& rhs)
{
  this->SetCount(rhs.GetCount());
  ezMemoryUtils::Copy(this->m_pElements, rhs.GetPtr(), rhs.GetCount());
}

template <typename T>
void ezDynamicArrayBase<T>::SetCapacity(ezUInt32 uiCapacity)
{
  this->m_uiCapacity = uiCapacity;  
  T* pNewData = EZ_NEW_RAW_BUFFER(this->m_pAllocator, T, this->m_uiCapacity);
  ezMemoryUtils::Construct(pNewData, this->m_pElements, this->m_uiCount);
  ezMemoryUtils::Destruct(this->m_pElements, this->m_uiCount);
  EZ_DELETE_RAW_BUFFER(this->m_pAllocator, this->m_pElements);
  this->m_pElements = pNewData;
}

template <typename T>
void ezDynamicArrayBase<T>::Reserve(ezUInt32 uiCapacity)
{
  if (this->m_uiCapacity >= uiCapacity)
    return;

  ezUInt32 uiNewCapacity = ezMath::Max(this->m_uiCapacity + (this->m_uiCapacity / 2), uiCapacity);
  uiNewCapacity = (uiNewCapacity + (CAPACITY_ALIGNMENT-1)) & ~(CAPACITY_ALIGNMENT-1);
  SetCapacity(uiNewCapacity);
}

template <typename T>
void ezDynamicArrayBase<T>::Compact()
{
  if (this->IsEmpty())
  {
    // completely deallocate all data, if the array is empty.
    EZ_DELETE_RAW_BUFFER(this->m_pAllocator, this->m_pElements);
    this->m_uiCapacity = 0;
  }
  else
  {
    const ezUInt32 uiNewCapacity = (this->m_uiCount + (CAPACITY_ALIGNMENT-1)) & ~(CAPACITY_ALIGNMENT-1);
    if (this->m_uiCapacity != uiNewCapacity)
      SetCapacity(uiNewCapacity);
  }
}


template <typename T, typename A>
ezDynamicArray<T, A>::ezDynamicArray() : ezDynamicArrayBase<T>(A::GetAllocator())
{
}

template <typename T, typename A>
ezDynamicArray<T, A>:: ezDynamicArray(ezAllocatorBase* pAllocator) : ezDynamicArrayBase<T>(pAllocator)
{
}

template <typename T, typename A>
ezDynamicArray<T, A>::ezDynamicArray(const ezDynamicArray<T, A>& other) : ezDynamicArrayBase<T>(other, A::GetAllocator())
{
}

template <typename T, typename A>
ezDynamicArray<T, A>:: ezDynamicArray(const ezDynamicArrayBase<T>& other) : ezDynamicArrayBase<T>(other, A::GetAllocator())
{
}

template <typename T, typename A>
ezDynamicArray<T, A>::ezDynamicArray(const ezArrayPtr<T>& other) : ezDynamicArrayBase<T>(other, A::GetAllocator())
{
}

template <typename T, typename A>
void ezDynamicArray<T, A>::operator=(const ezDynamicArray<T, A>& rhs)
{
  ezDynamicArrayBase<T>::operator=(rhs);
}

template <typename T, typename A>
void ezDynamicArray<T, A>::operator=(const ezDynamicArrayBase<T>& rhs)
{
  ezDynamicArrayBase<T>::operator=(rhs);
}

template <typename T, typename A>
void ezDynamicArray<T, A>::operator=(const ezArrayPtr<T>& rhs)
{
  ezDynamicArrayBase<T>::operator=(rhs);
}

