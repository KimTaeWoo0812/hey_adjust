using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.Diagnostics;

namespace game
{
    public class CSocket
    {
        private Socket m_hClient;
        private NetworkStream m_hStream;
        private StreamWriter m_hWriter;
        private StreamReader m_hReader;
        private Thread m_hThread;
        private Queue<char[]> m_MessageQueue;
        private Thread m_hSendThread;
        private Mutex m_hSendMutex;
        private Stopwatch m_hStopWatch;
        public String MusicPath;

        public void Initialize(string strIPAddr, int nPort)
        {
            m_hClient = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress ipAddr = IPAddress.Parse(strIPAddr);
            IPEndPoint serverEndPoint = new IPEndPoint(ipAddr, nPort);
            m_hClient.Connect(serverEndPoint);

            m_hStream = new NetworkStream(m_hClient);
            m_hWriter = new StreamWriter(m_hStream);
            m_hReader = new StreamReader(m_hStream);


            m_hStopWatch = new Stopwatch();
            m_hStopWatch.Start();
            m_hSendMutex = new Mutex();

            m_MessageQueue = new Queue<char[]>();

            m_hThread = new Thread(new ThreadStart(ReadMessage));
            m_hThread.Start();

            m_hSendThread = new Thread(new ThreadStart(AutoSendMessage));
            m_hSendThread.Start();

            MusicPath = @"..\\Music";
        }

        public void Release()
        {
            m_hThread.Abort();
            m_hSendThread.Abort();
            m_hSendMutex.Close();
            m_hReader.Close();
            m_hWriter.Close();
            m_hStream.Close();
            m_hClient.Close();
        }

        public void SendMessage(char[] Message)
        {
            m_hSendMutex.WaitOne();
            m_hStopWatch.Reset();
            m_hStopWatch.Start();
            m_hWriter.Write(Message);
            m_hWriter.Flush();
            m_hSendMutex.ReleaseMutex();
        }

        private void ReadMessage()//캐릭터
        {
            while (true)
            {
                char[] Buffer = new char[128];
                int Index = 0;
                while (true)
                {
                    char[] Temp = new char[1];
                    m_hReader.Read(Temp, 0, 1);

                    if (Temp[0] == '|' || Index>=128) break;
                    else Buffer[Index++] = Temp[0];
                }
                if(Buffer[0]!=null)
                    m_MessageQueue.Enqueue(Buffer);
            }
        }

        private void AutoSendMessage()
        {
            char[] szAutoMessage = new char[128];
            szAutoMessage[0] = 'T';
            szAutoMessage[1] = 'I';
            szAutoMessage[2] = 'M';
            szAutoMessage[3] = 'E';

            while (true)
            {
                TimeSpan CurrentTime = m_hStopWatch.Elapsed;

                if (CurrentTime.Seconds >= 10)
                    SendMessage(szAutoMessage);
            }
        }

        public char[] GetMessage()
        {
            return m_MessageQueue.Dequeue();
        }

        public char[] PeekMessage()
        {
            return m_MessageQueue.Peek();
        }

        public bool hasMessage()
        {
            return m_MessageQueue.Count > 0;
        }
    }
}