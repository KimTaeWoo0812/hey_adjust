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
    public partial class setupRoom : Form
    {
        private Info InfoData;
        login Login;
        private CSocket Socket;
        int nRoom;//방 인원수
        string strRoomNum = string.Empty; 
        gameEnd lobby;

        public setupRoom(CSocket socket, login _login,gameEnd Lobby)
        {
            InitializeComponent();
            Socket = socket;
            Login = _login;
            lobby = Lobby;
        }

        public void SetInfoData(Info data)//사용자 데이타 받아오기
        {
            InfoData = data;
        }

        private void setRoomName_Click(object sender, EventArgs e)
        {
            char[] lobbyChat = new char[128];

            int i,j;
            for (i = 21; i < inputRoomName.Text.Length + 21; i++)//# 제거
                lobbyChat[i] = inputRoomName.Text[i - 21];

            for ( j=0; j < i; j++)//# 제거
            {
                if (lobbyChat[j] == '#')
                    lobbyChat[j] = ' ';
            }


            if (inputRoomName.Text == string.Empty)
                MessageBox.Show("방제목을 입력하세요");
            else
            {
                lobby.StopSound();
                lobby.Kill_Thread();
                InfoSendToServer();
                gameRoom room = new gameRoom(Socket, Login);
                //room.RoomInof(inputRoomName.Text, nRoom, strRoomNum);
                room.SetInfoData(InfoData);

                Login.SetLobby(lobby);
                room.Owner = Login;
                room.TopLevel = false;
                Login.Controls.Add(room);
                this.IsMdiContainer = true;
                Login.Opacity = 1;
                room.Show();
                room.GoThread();
                room.HostChack();
                this.Close();
                lobby.Close();
            }
        }
        public void InfoSendToServer()//서버에게 방제, 최대인원 전달
        {
            char[] SendRoomInof = new char[128];
            char[] SendRoomName=new char[30];
            int i;


            SendRoomInof[0] = 'A';
            SendRoomInof[1] = 'M';
            SendRoomInof[2] = '_';
            SendRoomInof[3] = 'C';
            SendRoomInof[4] = 'R';
            SendRoomInof[5] = 'E';
            SendRoomInof[6] = 'A';
            SendRoomInof[7] = 'T';
            SendRoomInof[8] = 'E';
            SendRoomInof[9] = '_';
            SendRoomInof[10] = 'R';
            SendRoomInof[11] = 'O';
            SendRoomInof[12] = 'O';
            SendRoomInof[13] = 'M';
            SendRoomInof[14] = '#';

            
            //SendRoomInof[21] = (char)(nRoom + 50);
            

            for (i = 21; i < inputRoomName.Text.Length+21; i++)
            {
                SendRoomInof[i] = inputRoomName.Text[i - 21];
            }
            SendRoomInof[i] = '#';
            SendRoomInof[41] = (char)(nRoom+50);
            SendRoomInof[42] = '#';

            Socket.SendMessage(SendRoomInof);//보내기



            char[] getRoomInof = new char[128];
            while (!Socket.hasMessage()) ;
            getRoomInof = Socket.GetMessage();//방번호 받기

            
            strRoomNum += getRoomInof[21];


        }
        private void cancelsetupRoom_Click(object sender, EventArgs e)
        {
            Login.Opacity = 1;//투명도
            this.Close();
        }
        private void makeRoom_SelectedIndexChanged(object sender, EventArgs e)//최대인원 선택
        {
            nRoom = (int)RoomMax.SelectedIndex;

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void inputRoomName_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
