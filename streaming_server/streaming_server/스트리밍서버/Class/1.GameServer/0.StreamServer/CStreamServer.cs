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
    class CStreamServer
    {
        CClientAdministrator m_Administrator;
        Socket m_hServerSocket;
        Thread m_hReceiveThread;
        bool m_bExit;

        public int Intialize()
        {
            m_bExit = false;

            m_hServerSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 9190);
            m_hServerSocket.Bind(ipep);
            m_hServerSocket.Listen(10);

            m_Administrator = new CClientAdministrator();
            m_Administrator.Intialize();

            m_hReceiveThread = new Thread(new ThreadStart(RecvClient));
            m_hReceiveThread.Start();

            return 0;
        }

        public bool Progress()
        {
            return !m_bExit;
        }

        public int Render()
        {
            return 0;
        }

        public int Release()
        {
            m_hReceiveThread.Abort();
            return 0;
        }

        public void Exit()
        {
            m_bExit = true;
            m_Administrator.Release();
            Release();
        }

        private void RecvClient()
        {
            while (true)
            {
                Socket hClient = m_hServerSocket.Accept();
                m_Administrator.AddClient(hClient);
            }
        }
    }
}
