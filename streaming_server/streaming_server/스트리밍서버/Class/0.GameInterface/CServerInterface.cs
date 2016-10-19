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
    class CServerInterface
    {
        CStreamServer m_Server;
        Thread m_hServerThread;

        public int Intialize()
        {
            m_hServerThread = new Thread(new ThreadStart(MainServer));
            m_hServerThread.Start();

            return 0;
        }

        public bool Progress()
        {
            string strInput = Console.ReadLine();

            if (strInput == "Quit") return false;

            return true;
        }

        public int Render()
        {
            return 0;
        }

        public int Release()
        {
            m_Server.Exit();

            m_hServerThread.Join();

            return 0;
        }

        public void MainServer()
        {
            m_Server = new CStreamServer();
            m_Server.Intialize();

            while (m_Server.Progress()) ;

            m_Server.Release();
        }
    }
}
