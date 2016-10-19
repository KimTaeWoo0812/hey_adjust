using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace game
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            CSocket socket = new CSocket();

            socket.Initialize("211.214.234.220", 5000);
            //socket.Initialize("210.118.75.145", 7788);
            //socket.Initialize("210.118.75.145", 7788);//창회형 데스크탑
            //socket.Initialize("210.118.75.148", 7788);//내꺼 데스크탑
            Application.Run(new login(socket));

            socket.Release();
        }
    }
}
