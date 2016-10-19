using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace game
{
    public partial class UserMakeSubject : Form
    {
        CSocket Socket;
        login Login;

        public UserMakeSubject(CSocket socket, login _login)
        {
            InitializeComponent();
            Socket = socket;
            Login = _login;
            
        }

        private void cancel_Click(object sender, EventArgs e)
        {
            Login.Opacity = 1;//투명도
            this.Close();
        }

        private void make_Click(object sender, EventArgs e)
        {
            char[] message = new char[128];

            message[0] = 'A';
            message[1] = 'M';
            message[2] = '_';
            message[3] = 'M';
            message[4] = 'A';
            message[5] = 'K';
            message[6] = 'E';
            message[7] = '_';
            message[8] = 'P';
            message[9] = 'R';
            message[10] = 'O';
            message[11] = 'B';
            message[12] = 'L';
            message[13] = 'E';
            message[14] = 'M';
            message[15] = '#';

            int i;
            for (i = 21; i < problem.Text.Length + 21; i++)
                message[i] = problem.Text[i - 21];

            message[i] = '#';
            Socket.SendMessage(message);//보내기
            Login.Opacity = 1;//투명도
            this.Close();
        }
    }
}
