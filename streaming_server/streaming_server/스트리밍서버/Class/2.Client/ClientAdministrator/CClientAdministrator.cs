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
    class CClientAdministrator
    {
        CRoom[] m_Room;
        Thread[] m_hRoomThread;
        public int Intialize() 
        {
            m_Room = new CRoom[Define.ROOM_COUNT];
            m_hRoomThread = new Thread[Define.ROOM_COUNT];


            for (int i = 0; i < Define.ROOM_COUNT; ++i)
            {
                m_Room[i] = new CRoom();
                m_Room[i].Initialize();
            }

            for (int i = 0; i < Define.ROOM_COUNT; ++i)
            {
                m_hRoomThread[i] = new Thread(new ParameterizedThreadStart(RoomProgress));
                m_hRoomThread[i].Start(m_Room[i]);
            }


            return 0;
        }

        public int Progress()
        {
            return 0;
        }

        public int Render()
        {
            return 0;
        }

        public int Release()
        {
            for (int i = 0; i < Define.ROOM_COUNT; ++i)
                m_Room[i].Exit();

            for (int i = 0; i < Define.ROOM_COUNT; ++i)
                m_hRoomThread[i].Join();

            return 0;
        }

        public void AddClient(Socket ClientSocket)
        {
            int nRoomNumber = getClientRoomNumber(ClientSocket);

            CClient Client = new CClient();
            Client.CreateClient(ClientSocket, nRoomNumber);

            Console.WriteLine("클라이언트 접속 : " + nRoomNumber + "번호에 입장함");

            m_Room[nRoomNumber].AddClient(Client);
        }

        private int getClientRoomNumber(Socket ClientSocket)
        {
            byte[] buffer = new byte[1];

            ClientSocket.Receive(buffer);

            return Convert.ToInt32(buffer[0]);
        }

        private void RoomProgress(Object Room)
        {
            while (((CRoom)Room).Progress()) ;

            ((CRoom)Room).Release();
        }
    }
}