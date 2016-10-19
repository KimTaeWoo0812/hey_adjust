using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace 스트리밍서버
{
    class Program
    {
        static void Main(string[] args)
        {
            CServerInterface server = new CServerInterface();

            server.Intialize();

            while (server.Progress()) ;

            server.Release();
        }
    }
}
