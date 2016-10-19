using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace 스트리밍서버
{
    class CClient
    {
        private bool    m_bExit;
        private bool    m_bDelete;
        private Socket  m_hSocket;
        private int     m_nRoomNumber;
        System.Diagnostics.Stopwatch m_ReceiveTime = new System.Diagnostics.Stopwatch();
        Thread          m_hTimerThread;
        Thread          m_hReceiveThread;
        Queue<byte[]>   m_Queue;
        bool            m_bFlag;

        public CClient()
        {
            m_ReceiveTime = new System.Diagnostics.Stopwatch();
            m_Queue = new Queue<byte[]>();
            m_bExit = true;
            m_bDelete = true;
            m_bFlag = false;
        }

        public void Progress()
        {
            Release();
        }

        public void Release()
        {
            if(m_bExit)
            {
                if (m_bDelete == false)
                    DeleteClient();
            }
        }

        public void CreateClient(Socket hSocket, int nRoomNumber)
        {
            TimerReset();
            m_bExit = false;
            m_bDelete = false;
            m_hSocket = hSocket;
            CreateThread();
        }

        public int getRoomNumber()
        {
            return m_nRoomNumber;
        }

        public byte[] getMessage()
        {
            return m_Queue.Dequeue();
        }

        public bool hasMessage()
        {
            return m_Queue.Count != 0;
        }

        private void TimerReset()
        {
            Console.WriteLine("리셋");
            m_ReceiveTime.Reset();
            m_ReceiveTime.Start();
        }

        private void SetRoomNumber()
        {
            byte[] buffer = new byte[1];           //1byte로 방번호를 전송받는다. ㅂㄷㅂㄷ
            m_hSocket.Receive(buffer);

            m_nRoomNumber = buffer[0];
        }

        public bool isDelete()
        {
            return m_bDelete;
        }

        private void DeleteClient()
        {
            m_bExit = true;
            m_bDelete = true;
            DeleteThread();
        }

        private void CreateThread()
        {
            m_hTimerThread = new Thread(new ThreadStart(WatchMyself));
            m_hReceiveThread = new Thread(new ThreadStart(Receive));
            m_hTimerThread.Start();
            m_hReceiveThread.Start();
        }

        private void DeleteThread()
        {
            Console.WriteLine("클라이언트 소멸 : " + m_nRoomNumber + "번호에서 소멸");
            m_hReceiveThread.Abort();
            m_hReceiveThread.Join();
            m_hTimerThread.Join();
            m_hSocket.Close();
        }

        private void Receive()
        {
            int nNumber = 0;

            while (true)
            {

                byte[] buffer = new byte[Define.BYTE_SIZE];

                try
                {

                    int nBuffer = m_hSocket.Receive(buffer);

                    if (nBuffer <= 10)
                    {
                        m_bExit = true;
                        return;
                    }
                }
                catch (System.Net.Sockets.SocketException e)
                {
                    m_bExit = true;
                    return;
                }


                TimerReset();

                for (int i = 0; i < 20; ++i)
                    nNumber += buffer[i];

                if (nNumber == 0)
                {
                    m_bFlag = true;
                    continue;
                }

                m_Queue.Enqueue(buffer);
                nNumber = 0;
            }
        }

        public bool getFlag()
        {
            return m_bFlag;
        }

        public void ResetFlag()
        {
            m_bFlag = false;
        }

        public void SendMessage(byte[] buffer)
        {
            try
            {
                m_hSocket.Send(buffer);
            }
            catch (System.Net.Sockets.SocketException)
            {
                return;
            }
        }

        public void SendFlagMessage()
        {
            byte[] buffer = new byte[Define.BYTE_SIZE];

            for (int i = 0; i < 20; ++i)
                buffer[i] = 0;

            try
            {
                m_hSocket.Send(buffer);
            }
            catch (System.Net.Sockets.SocketException)
            {
                return;
            }
        }

        private void WatchMyself()
        {
            TimerReset();

            while(true)
            {
                if (m_bExit) break;

                if (m_ReceiveTime.Elapsed.Seconds >= 40 && m_ReceiveTime.Elapsed.Seconds <= 50 && m_ReceiveTime.Elapsed.Seconds != 0)              //40초가 기준점이다.
                {
                    Console.WriteLine(m_ReceiveTime.Elapsed.Seconds);
                    m_bExit = true;
                }
            }
        }
    }
}
