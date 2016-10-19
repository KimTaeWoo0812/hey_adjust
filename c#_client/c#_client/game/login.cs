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


namespace game
{
    public partial class login : Form
    {
        private Info InfoData;
        private CSocket Socket;
        public int chackIDandPW;
        private gameEnd Lobby;
        private bool HasLobby;
        private gameRoom Room;
        private bool HasRoom;
        System.Media.SoundPlayer loginSound;
       //System.Media.SoundPlayer Sound = new System.Media.SoundPlayer("day001.wav");
        
        public login(CSocket socket)
        {
            InitializeComponent();
            this.IsMdiContainer = true; // 또는 디자인 모드의 프로퍼티에서 바꿀 수도 있다.

            HasRoom = false;
            HasLobby = false;
            InfoData = new Info();
            Socket = socket;

            loginSound = new System.Media.SoundPlayer(socket.MusicPath + "..\\login\\" + "login.wav");
            loginSound.PlayLooping();//노래 재생
        }


        private void btnCreate_Click(object sender, EventArgs e)
        {
            CreateForm form = new CreateForm(Socket,this);
            form.Owner = this;
            form.StartPosition = FormStartPosition.Manual;
            form.Location = new Point(200, 200);

            form.ShowDialog(); //켜진 창 끝날때까지 부모창 사용 불가
            form.Opacity = 0.9;
        }
        /*이미지 삽입 소스
        private void login_Paint(object sender, PaintEventArgs e)
        {
            Bitmap bmImg = new Bitmap(("C:\\Users\\김태우\\Documents\\Visual Studio 2010\\Projects\\game\\game\\bin\\Debug\\Test.jpg"));

            Graphics g = this.CreateGraphics();//CreateGraphics()이게 실제로 어떤 윈도우 화면상의 표현을 담당
            g.DrawImage(bmImg, 0, 0, bmImg.Width, bmImg.Height);
            bmImg.Dispose();
            g.Dispose();

        }*/



        void SendMessage()
        {
            chackIDandPW = 0;
            char[] Buffer = new char[128];
            Buffer[0] = 'A';
            Buffer[1] = 'M';
            Buffer[2] = '_';
            Buffer[3] = 'L';
            Buffer[4] = 'O';
            Buffer[5] = 'G';
            Buffer[6] = 'I';
            Buffer[7] = 'N';

            for (int j = 8; j <= 20; j++)//끝 표시
                Buffer[j] = '#';

            int size = txtID.Text.Length;
            int i;

            for (i = 21; i < size + 21; i++)//id 저장
                Buffer[i] = txtID.Text[i - 21];

            for (int j = i; j <= 40; j++)//끝 표시
                Buffer[j] = '#';


            size = txtPW.Text.Length;

            for (i = 41; i < size + 41; i++)//pw 저장
                Buffer[i] = txtPW.Text[i - 41];
             
            for (int j = i; j <= 60; j++)//끝표시
                Buffer[j] = '#';
            Socket.SendMessage(Buffer);

            ///////////////////////////////////출력

            char[] Name = new char[21];
            char[] Point = new char[21];
            char[] Rank = new char[21];

            char[] readBuf = new char[128];

            while (!Socket.hasMessage()) ;

            readBuf = Socket.GetMessage();

            int nIndex = 0;

            while (true)
            {
                if (readBuf[nIndex] == 0)
                    ++nIndex;

                else
                    break;
            }

            //readBuf[16] = '0';

            if (readBuf[21 + nIndex] == '1')
            {
                MessageBox.Show("로그인 성공!!");

                while(!Socket.hasMessage());
                readBuf = Socket.GetMessage();

                for (i = 21 + nIndex; readBuf[i] != '#'; i++)//이름 저장
                    Name[i - 21 + nIndex] = readBuf[i];

                InfoData.InputName(Name);

                for (i = 41  + nIndex; readBuf[i] != '#'; i++)//포인트 저장
                    Point[i - 41  + nIndex] = readBuf[i];

                InfoData.InputPoint(Point);

                for (i = 61  + nIndex; readBuf[i] != '#'; i++)//랭킹 저장
                    Rank[i - 61  + nIndex] = readBuf[i];

                InfoData.InputRank(Rank);
            }
            else
            {
                MessageBox.Show("없는 ID 거나 비밀번호가 틀렸습니다!!");
                txtPW.Text = string.Empty;
                chackIDandPW = 1;
            }
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            SendMessage();
            if (chackIDandPW == 0)
            {
                if (txtID.Text == string.Empty)
                    MessageBox.Show("아이디를 입력하세요");

                else if (txtPW.Text == string.Empty)
                    MessageBox.Show("패스워드를 입력하세요");

                else
                {
                    loginSound.Stop();
                    gameEnd lobby = new gameEnd(Socket, this);
                   // gameRoom Room = new gameRoom(Socket, this);
                    //this.Visible = false;

                    label1.Visible = false;
                    label2.Visible = false;
                    txtID.Visible = false;
                    txtPW.Visible = false;
                    btnCreate.Visible = false;
                    btnLogin.Visible = false;


                    lobby.Owner = this;
                    lobby.TopLevel = false;
                    this.Controls.Add(lobby);
                    lobby.SetInfoData(InfoData);
                    this.IsMdiContainer = true;
                    lobby.Show();


                   
                    //this.Activate();
                    //lobby.Focus();
                   // this.Hide();
                   // this.Activate(); 
                    //Owner
                    //.ShowDialog(); //켜진 창 끝날때까지 부모창 사용 불가능
                }
            }
            else
                return;

        }
        private void txtPW_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)//엔터 누르면 자동으로 넘어가게 함
                btnLogin_Click(null, null);
        }
        
        private void Login_FormClosing(object sender, FormClosingEventArgs e)
        {
            
            if (MessageBox.Show("프로그램을 종료하시겠습니까?", "프로그램 종료", MessageBoxButtons.YesNo) == System.Windows.Forms.DialogResult.Yes)
            {
                char[] Quit = new char[6];
                Quit[0] = 'Q';
                Quit[1] = 'u';
                Quit[2] = 'i';
                Quit[3] = 't';
                Socket.SendMessage(Quit);
                //m_hThread.Abort();
                KillThread();//자식 쓰레드 죽이기
                
                Socket.Release();
                Dispose(true);
                //lobby.Close();
                this.Close();
                return;
            }
            else
            {
                e.Cancel = true;
                return;
            }
        }

        public void SetLobby(gameEnd _lobby)
        {
            if (HasLobby == true)
                HasLobby = false;
            else
                HasLobby = true;

            Lobby = _lobby;
        }
        public void SetRoom(gameRoom _Room)
        {
            if (HasRoom == true)
                HasRoom = false;
            else
                HasRoom = true;
            Room = _Room;
        }
        public void KillThread()
        {
            if (HasLobby)
            {
                Lobby.Kill_Thread();
            }
            if (HasRoom)
                Room.Kill_RoomThread();
        }

        private void txtID_TextChanged(object sender, EventArgs e)
        {

        }
    }

}
