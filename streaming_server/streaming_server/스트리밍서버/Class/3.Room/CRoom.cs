using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace 스트리밍서버
{
    class CRoom
    {
        CClient[] m_Clinet;
        Mutex m_hMutex;
        bool m_bExit;

        public void Initialize()
        {
            m_Clinet = new CClient[Define.CLINET_MAX_IN_ROOM];
            m_hMutex = new Mutex();
            m_bExit = false;
        }

        public bool Progress()
        {
            m_hMutex.WaitOne();

            for (int i = 0; i < Define.CLINET_MAX_IN_ROOM; ++i)
            {
                if (m_Clinet[i] != null)
                    m_Clinet[i].Progress();
            }

            Interpret();

            m_hMutex.ReleaseMutex();

            return !m_bExit;
        }

        public void Release()
        {
            for(int i = 0; i < Define.CLINET_MAX_IN_ROOM; ++i)
            {
                if (m_Clinet[i] != null)
                    m_Clinet[i].Release();
            }
        }

        public void AddClient(CClient Client)
        {
            m_hMutex.WaitOne();
                m_Clinet[getIndex()] = Client;
            m_hMutex.ReleaseMutex();
        }

        public int getIndex()
        {
            for(int i = 0; i < Define.CLINET_MAX_IN_ROOM; ++i)
            {
                if (m_Clinet[i] == null) 
                    return i;

                else if (m_Clinet[i].isDelete())
                    return i;
            }

            return -1;
        }

        private void Interpret()
        {
            for (int i = 0; i < Define.CLINET_MAX_IN_ROOM; ++i)
            {
                if (m_Clinet[i] == null) continue;

                if (m_Clinet[i].hasMessage())
                {
                    SendMessage(m_Clinet[i].getMessage(), i);
                    return;
                }
            }
        }

        private void SendMessage(byte[] buffer,int nSendIndex)
        {
            for(int i = 0; i < Define.CLINET_MAX_IN_ROOM; ++i)
            {
                if (m_Clinet[i] == null) continue;
                if (nSendIndex == i) continue;
                if (m_Clinet[i].isDelete()) continue;

                m_Clinet[i].ResetFlag();            //보내기 직전에 플레그 초기화
                m_Clinet[i].SendMessage(buffer);    //보낸다
                while (!m_Clinet[i].getFlag()) ;    //기다린다.
            }

            m_Clinet[nSendIndex].SendFlagMessage(); //신호메세지를 보낸다.
        }

        public void Exit()
        {
            m_bExit = true;
        }
    }
}