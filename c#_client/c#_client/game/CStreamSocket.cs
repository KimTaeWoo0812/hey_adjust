using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.Diagnostics;
using OpenCvSharp;
using OpenCvSharp.Extensions;

namespace game
{
    //방에 들어가서부터 부터 접근.
    //1. 방장이 게임 시작되면 메세지를 전달할 것
    class CStreamSocket
    {
        private Socket m_hClient;
        private Queue<byte[]> m_MessageQueue;
        private Stopwatch m_hStopWatch;
        private Thread m_hWatchThread;
        private Thread m_hReceiveThread;
        private bool m_bExit;
        private bool m_bFlag;
        private Image m_Image;

        private void TimeReset()
        {
            m_hStopWatch.Reset();
            m_hStopWatch.Start();
        }

        public CStreamSocket()
        {
            m_MessageQueue = null;
            m_bFlag = false;
        }

        public void Initialize(string strIPAddr, int nPort, int nRoomNumber)
        {
            m_hClient = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress ipAddr = IPAddress.Parse(strIPAddr);
            IPEndPoint serverEndPoint = new IPEndPoint(ipAddr, nPort);
            m_hClient.Connect(serverEndPoint);

            m_bExit = false;

            m_MessageQueue = new Queue<byte[]>();

            m_hStopWatch = new Stopwatch();
            TimeReset();

            byte[] bArray = new byte[1];
            bArray[0] = Convert.ToByte(nRoomNumber);
            m_hClient.Send(bArray);

            CreateThread();
        }

        private void CreateThread()
        {
            m_hWatchThread = new Thread(new ThreadStart(AutoMessage));
            m_hReceiveThread = new Thread(new ThreadStart(ReceiveMessage));
            m_hWatchThread.Start();
            m_hReceiveThread.Start();
        }


        public void Release()
        {
            m_bExit = true;

            m_hReceiveThread.Abort();

            m_hWatchThread.Join();

            m_hClient.Close();
        }


        public bool hasMessage()
        {
            if (m_MessageQueue == null) return false;
            return m_MessageQueue.Count > 0;
        }

        public void SendMessage(IplImage image)
        {
            Bitmap bitmap = (Bitmap)BitmapConverter.ToBitmap(image);
            System.IO.MemoryStream BITMAPMST = new MemoryStream();
            bitmap.Save(BITMAPMST, System.Drawing.Imaging.ImageFormat.Jpeg);

            byte[] bTemp = BITMAPMST.ToArray();
            Send(bTemp);
        }

        public bool hasImage()
        {
            if (m_MessageQueue.Count == 0)
                return false;

            byte[] recvImage = m_MessageQueue.Dequeue();

            MemoryStream ms = new MemoryStream(recvImage);
            try
            {
                m_Image = Image.FromStream(ms);
                return true;
            }
            catch (ArgumentException)
            {
                return false;
            }
        }

        public Image getImage()
        {
            return m_Image;
        }

        private void Send(byte[] buffer)
        {
            m_hClient.Send(buffer);
        }

        public void SendFlag()
        {
            byte[] buffer = new byte[15000];

            for (int i = 0; i < 20; ++i)
                buffer[i] = 0;

            Send(buffer);
        }

        public void ResetFlag()
        {
            m_bFlag = false;
        }

        public bool getFlag()
        {
            return m_bFlag;
        }

        private void ReceiveMessage()
        {
            byte[] buffer = new byte[15000];
            int nNumber = 0;

            while (true)
            {
                TimeReset();
                m_hClient.Receive(buffer);

                for (int i = 0; i < 20; ++i)
                    nNumber += buffer[i];

                if (nNumber == 0)
                {
                    m_bFlag = true;
                    continue;
                }

                m_MessageQueue.Enqueue(buffer);
                buffer = new byte[15000];
                nNumber = 0;
            }
        }

        private void AutoMessage()
        {
            while (!m_bExit)
            {
                TimeSpan Time = m_hStopWatch.Elapsed;

                if (Time.Seconds >= 10)
                {
                    byte[] buffer = new byte[15000];

                    for (int i = 0; i < 20; ++i)
                        buffer[i] = 0;

                    Send(buffer);
                    TimeReset();
                }
            }
        }
    }
}


/*
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.Diagnostics;
using OpenCvSharp;
using OpenCvSharp.Extensions;

namespace game
{
    //방에 들어가서부터 부터 접근.
    //1. 방장이 게임 시작되면 메세지를 전달할 것
    class CStreamSocket
    {
        private Socket m_hClient;
        private Queue<byte[]> m_MessageQueue;
        private Stopwatch m_hStopWatch;
        private Thread m_hWatchThread;
        private Thread m_hReceiveThread;
        private bool m_bExit;
        private bool m_bFlag;
        private Image m_Image;

        private void TimeReset()
        {
            m_hStopWatch.Reset();
            m_hStopWatch.Start();
        }

        public CStreamSocket()
        {
            m_MessageQueue = null;
            m_bFlag = false;
        }

        public void Initialize(string strIPAddr, int nPort, int nRoomNumber)
        {
            m_hClient = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress ipAddr = IPAddress.Parse(strIPAddr);
            IPEndPoint serverEndPoint = new IPEndPoint(ipAddr, nPort);
            m_hClient.Connect(serverEndPoint);

            m_bExit = false;

            m_MessageQueue = new Queue<byte[]>();

            m_hStopWatch = new Stopwatch();
            TimeReset();

            byte[] bArray = new byte[1];
            bArray[0] = Convert.ToByte(nRoomNumber);
            m_hClient.Send(bArray);

            CreateThread();
        }

        private void CreateThread()
        {
            m_hWatchThread = new Thread(new ThreadStart(AutoMessage));
            m_hReceiveThread = new Thread(new ThreadStart(ReceiveMessage));
            m_hWatchThread.Start();
            m_hReceiveThread.Start();
        }


        public void Release()
        {
            m_bExit = true;

            m_hReceiveThread.Abort();

            m_hWatchThread.Join();

            m_hClient.Close();
        }


        public bool hasMessage()
        {
            if (m_MessageQueue == null) return false ;
            return m_MessageQueue.Count > 0;
        }

        public void SendMessage(IplImage image)
        {
            Bitmap bitmap = (Bitmap)BitmapConverter.ToBitmap(image);
            System.IO.MemoryStream BITMAPMST = new MemoryStream();
            bitmap.Save(BITMAPMST, System.Drawing.Imaging.ImageFormat.Jpeg);

            byte[] bTemp = BITMAPMST.ToArray();
            Send(bTemp);
        }

        public bool hasImage()
        {
            byte[] recvImage = m_MessageQueue.Dequeue();

            try
            {
                MemoryStream ms = new MemoryStream(recvImage);
                m_Image = Image.FromStream(ms);
                return true;
            }
            catch (ArgumentException)
            {
                return false;
            }
        }

        public Image getImage()
        {
            return m_Image;
        }

        private void Send(byte[] buffer)
        {
            m_hClient.Send(buffer);
        }

        public void SendFlag()
        {
            byte[] buffer = new byte[15000];

            for (int i = 0; i < 20; ++i)
                buffer[i] = 1;

            Send(buffer);
        }

        public void ResetFlag()
        {
            m_bFlag = false;
        }

        public bool getFlag()
        {
            return m_bFlag;
        }
        
        private void ReceiveMessage()
        {
            byte[] buffer = new byte[15000];
            int nNumber = 0;

            while (true)
            {
                TimeReset();
                m_hClient.Receive(buffer);

                for (int i = 0; i < 20; ++i)
                    nNumber += buffer[i];

                if (nNumber == 20)
                {
                    m_bFlag = true;
                    continue;
                }

                m_MessageQueue.Enqueue(buffer);
                buffer = new byte[15000];
                nNumber = 0;
            }
        }

        private void AutoMessage()
        {
            while (!m_bExit)
            {
                TimeSpan Time = m_hStopWatch.Elapsed;

                if (Time.Seconds >= 10)
                {
                    SendFlag();
                    TimeReset();
                }
            }
        }
    }
}
*/