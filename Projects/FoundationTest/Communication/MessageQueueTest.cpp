#include <PCH.h>
#include <Foundation/Communication/MessageQueue.h>
#include <Foundation/Threading/Mutex.h>

namespace
{
  struct TestMessage : public ezMessage
  {
    EZ_DECLARE_MESSAGE_TYPE(TestMessage);

    int x;
    int y;
  };

  struct MetaData
  {
    int receiver;
  };

  typedef ezMessageQueue<MetaData, ezNoMutex> TestMessageQueue;

  EZ_IMPLEMENT_MESSAGE_TYPE(TestMessage);
}

EZ_CREATE_SIMPLE_TEST(Communication, MessageQueue)
{
  EZ_CHECK_AT_COMPILETIME(ezIsPodType<TestMessage>::value);

  {
    TestMessage msg;
    EZ_TEST_INT(msg.GetSize(), sizeof(TestMessage));
    EZ_TEST_INT(msg.x, 0);
    EZ_TEST_INT(msg.y, 0);
  }

  TestMessageQueue q;
  
  EZ_TEST_BLOCK(ezTestBlock::Enabled, "Enqueue")
  {
    for (ezUInt32 i = 0; i < 100; ++i)
    {
      TestMessage msg;
      msg.x = rand();
      msg.y = rand();

      MetaData md;
      md.receiver = rand() % 10;

      q.Enqueue(msg, md);
    }
  }

  EZ_TEST_BLOCK(ezTestBlock::Enabled, "Sorting")
  {
    struct MessageComparer
    {
      static bool Less(const TestMessageQueue::Entry& a, const TestMessageQueue::Entry& b)
      {
        if (a.m_MetaData.receiver != b.m_MetaData.receiver)
          return a.m_MetaData.receiver < b.m_MetaData.receiver;

        return a.m_pMessage->GetHash() < b.m_pMessage->GetHash();
      }
    };

    q.Sort<MessageComparer>();
  }

  EZ_TEST_BLOCK(ezTestBlock::Enabled, "operator[]")
  {
    ezMessage* pLastMsg = q[0].m_pMessage;
    MetaData lastMd = q[0].m_MetaData;

    for (ezUInt32 i = 1; i < q.GetCount(); ++i)
    {
      ezMessage* pMsg = q[i].m_pMessage;
      MetaData md = q[i].m_MetaData;

      if (md.receiver == lastMd.receiver)
      {
        EZ_TEST_BOOL(pMsg->GetHash() >= pLastMsg->GetHash());
      }
      else
      {
        EZ_TEST_BOOL(md.receiver >= lastMd.receiver);
      }
    }
  }

  EZ_TEST_BLOCK(ezTestBlock::Enabled, "Dequeue")
  {
    ezMessage* pMsg = NULL;
    MetaData md;

    for (ezUInt32 i = 0; i < 10; ++i)
    {
      if (q.TryDequeue(pMsg, md))
      {
        EZ_DELETE(q.GetStorageAllocator(), pMsg);
      }
    }
  }
}
