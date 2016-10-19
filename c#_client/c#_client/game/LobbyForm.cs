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

namespace game
{
    public partial class gameEnd : Form
    {
        private Info InfoData;
        private CSocket Socket;
        private Thread m_hThread;
        private bool CheckNoChat;
        private int checkCkear = 0;//방목록,대기자목록 초기화
        private bool Exit;
        System.Media.SoundPlayer LobbySound;
        login Login;
        private char[] message = new char[128];
   
        public gameEnd(CSocket socket, login _login)
        {
            CheckForIllegalCrossThreadCalls = false;
            InitializeComponent();
            InfoData = null;
            Socket = socket;

            Exit = false;

            Login = _login;
            Login.SetLobby(this);

            m_hThread = new Thread(new ThreadStart(GetInfo));
            m_hThread.Start();

            LobbySound = new System.Media.SoundPlayer(socket.MusicPath + "..\\lobby\\" + "lobby.wav");
            LobbySound.PlayLooping();//노래 재생
            //LobbySound.Stop();

        }

        private void GetInfo()
        {
            while (true)
            {
                if (Exit) break;

                if (CheckNoChat == false)
                {
                    while (!Socket.hasMessage())
                        if (Exit) break;

                    if (Exit) break;
                    char[] order = new char[30];
                    message = Socket.GetMessage();
                    for (int i = 0; i < 15; i++)
                        order[i] = message[i];

                    CheckNoChat = true;
                    if (order[5] == 'O' && order[6] == 'M' && order[8] == 'I' && order[9] == 'N' && order[10] == 'F')
                    {//AM_ROOM_INFO
                        if (checkCkear == 0)
                        {
                            RoomList.Items.Clear();
                            UserList.Items.Clear();
                        }
                        checkCkear = 1;
                        AddRoom();//방 추가
                    }
                    else if (order[3] == 'E' && order[4] == 'N' && order[5] == 'T')
                    {
                        if (message[21] == '0')
                        {
                            MessageBox.Show("방에 입장할 수 없습니다!");
                            Exit = false;

                        }
                        else
                            break;
                        //GetWhenRoomList_MouseDoubleClick();
                    }
                    else if (order[3] == 'U' && order[4] == 'S' && order[5] == 'E')
                    {
                        SatUser();//대기자 추가
                    }
                    else if (order[3] == 'L' && order[4] == 'O' && order[5] == 'B')
                    {
                        LookAtChat();//채팅
                    }
                    else if (order[3] == 'F' && order[4] == 'R' && order[5] == 'I')
                    {
                        //친구
                    }
                    /*
                else if (order[3] == 'E' && order[4] == 'N' && order[5] == 'T')
                {
                    GetWhenRoomList_MouseDoubleClick();//방 클릭했을때
                }*/
                    CheckNoChat = false;
                }

            }

        }
        private void AddRoom()//방 목록에
        {
            CheckNoChat = true;
            char[] RoomNum = new char[120];
            char[] RoomName = new char[100];
            string strRoomNum = string.Empty;
            string strRoomName = string.Empty;
            string strRoomPoeple = string.Empty;
            string strRoomPoepleMix = string.Empty;
            char isStart;
            string strIsStart = string.Empty;
            int i, nIndex=0;

            if (message[21] == '0')
            {
                checkCkear = 0;
                return;
            }

            while (true)
            {
                if (message[nIndex] == 0)
                    ++nIndex;
                //00AM_ROOM_INFO#0
                else
                    break;
            }

            for (i = 21 ; message[i] != '#'; i++)//방번
                 strRoomNum += message[i];

            for (i = 41; message[i] != '#'; i++)//방제
                strRoomName += message[i];

            strRoomPoeple += message[61];//현재 인원
            strRoomPoepleMix += message[81];//최대 인원

            isStart = message[101];
            if (isStart == '1')
            {
                strIsStart += "O";
            }
            else
                strIsStart += "X";

            RoomList.Items.Add(strRoomNum + "         " + strRoomName + "                                             " + strRoomPoeple + "/" + strRoomPoepleMix + "        " + strIsStart);

            Update();
        }
         private void SatUser()
         {
             int i, nIndex;
             char[] UserListName = new char[40];
             char[] UserListRank = new char[20];
                nIndex = 0;
                //string result = Encoding.UTF8.GetString(read);
                // Unicode -> string  : Encoding.UTF8.GetString( 유니코드byte );
                //string -> Unicode : Encoding.UTF8.GetBytes( string );
                if (message[21] == '0')
                    return;

                while (true)
                {
                    if (message[nIndex] == 0)
                        ++nIndex;

                    else
                        break;
                }
                int UserListSize = 0;

                for (i = 21 + nIndex; message[i] != '#'; i++)//유저 이름
                {
                    ++UserListSize;
                    UserListName[i - 21 + nIndex] = message[i];
                }

                for (i = 61 + nIndex; message[i] != '#'; i++)//유저 랭킹
                    UserListRank[i - 61 + nIndex] = message[i];

                //텍스트에 삽입

                UserListName[UserListSize] = ' ';
                UserListName[UserListSize + 1] = ' ';
                for (int j = 0; j < UserListRank.Length; ++j)
                {
                    UserListName[UserListSize + (j + 2)] = UserListRank[j];
                }

                string strUserList = new string(UserListName);

                UserList.Items.Add(strUserList);

            
           //CheckNoChat = false;

        }
        private void refreshRoom_Click(object sender, EventArgs e)//새로고침
        {

        }
        public void SetInfoData(Info data)
        {
            InfoData = data;

            //데이터 넘기기
            char[] szData = InfoData.OutputName();//이름
            string strData = string.Empty;
            for (int i = 0; i < szData.Length; ++i)
                strData += szData[i];

            LobbyUserName.Text = strData;

            szData = InfoData.OutputPoint();//포인트
            strData = string.Empty;
            for (int i = 0; i < szData.Length; ++i)
                strData += szData[i];

            LobbyUesrPoint.Text = strData;

            szData = InfoData.OutputRank();//순위
            strData = string.Empty;
            for (int i = 0; i < szData.Length; ++i)
                strData += szData[i];

            LobbyUserRank.Text = strData;

        }

        public void SetID(string strID)//삭제할꺼
        {
            UserList.Items.Add("김재연");//접속자 추가
            UserList.Items.Remove("임영민");// 삭제
            UserList.Items.Remove("나 없쪙");

            UserList.Items.Clear();

            Update();
        }
        private void RoomList_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (RoomList.SelectedIndex == -1) return;

            //사용자가 방 클릭 시 서버에 메시지 보냄
            char[] sendMessage = new char[128];
//방번호 받기용
            string createRoomNum = ((sender as ListBox).SelectedItem as string);
                //SelectedItem as string);
            if (createRoomNum == String.Empty)
            {
                return;
            }
            sendMessage[0] = 'A';
            sendMessage[1] = 'M';
            sendMessage[2] = '_';
            sendMessage[3] = 'E';
            sendMessage[4] = 'N';
            sendMessage[5] = 'T';
            sendMessage[6] = 'E';
            sendMessage[7] = 'R';
            sendMessage[8] = '_';
            sendMessage[9] = 'R';
            sendMessage[10] = 'O';
            sendMessage[11] = 'O';
            sendMessage[12] = 'M';
            sendMessage[13] = '#';
            sendMessage[21] = createRoomNum[0];
            sendMessage[22] = '#';

            CheckNoChat = true;
            Socket.SendMessage(sendMessage);



            while(message[3] != 'E' && message[4] != 'N' && message[5] != 'T')
                /*
            for (; ; )
            {
                if (message[3] == 'E' && message[4] == 'N' && message[5] == 'T')
                {
                    break;
                }
                if (Socket.hasMessage())
                {
                    message = Socket.GetMessage();
                    if (message[3] == 'E' && message[4] == 'N' && message[5] == 'T')
                        break;
                }
            }
                */
            if (message[21] == '0')
            {
                return;
            }
            Login.SetLobby(this);
            StopSound();
            Exit = true;
            gameRoom room = new gameRoom(Socket, Login);
            m_hThread.Join();
            //this.Kill_Thread();
            room.SetInfoData(InfoData);
            room.Owner = Login;
            room.TopLevel = false;
            //Login.Visible = true;
            Login.Controls.Add(room);
            Login.IsMdiContainer = true;
            room.Show();
            room.GoThread();
            this.Close();
        }


        private void makeRoom_Click_Click(object sender, EventArgs e)//방만들기 클릭
        {

            setupRoom goSetupRoom = new setupRoom(Socket, Login,this);
            goSetupRoom.SetInfoData(InfoData);
            goSetupRoom.Owner = this;
            Login.Opacity = 0.9;
            goSetupRoom.StartPosition = FormStartPosition.Manual;
            goSetupRoom.Location = new Point(200, 200);
            goSetupRoom.ShowDialog(); //켜진 창 끝날때까지 부모창 사용 불가
        }


        

        private void LookAtChat()//서버로부터 텍스트 내용 받음
        {

            char[] chat = new char[128];
            int i;
            for (i = 21; message[i] != '#'; i++)//닉네임
            {
                chat[i - 21] = message[i];
            }
            chat[i - 21] = ' ';
            chat[i - 20] = ':';
            chat[i - 19] = ' ';
            for (int j = 41; message[j] != '#'; j++)//대화내용
            {
                chat[j - 41 + i - 18] = message[j];
            }
            PushChat(chat);

        }
        
        private void PushChat(char[] chat)//텍스트 출력
        {
            string strChat = new string(chat);

            if (outputChat.Text != string.Empty)
                outputChat.Text += "\r\n";
            outputChat.AppendText(strChat);
            //outputChat.Text += strChat;
            //inputChat.Clear();//입력창 clear

        }
        
        private void sendChat_Click(object sender, EventArgs e)
        {
            char[] lobbyChat = new char[128];
            string strlobbyChat = inputChat.Text;
            int i;

            lobbyChat[0] = 'A';
            lobbyChat[1] = 'M';
            lobbyChat[2] = '_';
            lobbyChat[3] = 'L';
            lobbyChat[4] = 'O';
            lobbyChat[5] = 'B';
            lobbyChat[6] = 'B';
            lobbyChat[7] = 'Y';
            lobbyChat[8] = '_';
            lobbyChat[9] = 'C';
            lobbyChat[10] = 'H';
            lobbyChat[11] = 'A';
            lobbyChat[12] = 'T';
            lobbyChat[13] = '#';

            for (i = 21; i < strlobbyChat.Length + 21; i++)//# 제거
            {
                lobbyChat[i] = strlobbyChat[i - 21];
                if (lobbyChat[i] == '#')
                    lobbyChat[i] = ' ';
            }

            lobbyChat[i] = '#';
            Socket.SendMessage(lobbyChat);
            inputChat.Clear();//입력창 clear
        }

        private void outputChat_TextChanged(object sender, EventArgs e)
        {
            /*
            //outputChat.Text += "\n";
            outputChat.Text = inputChat.Text;
            inputChat.Clear();*/
        }

        private void inputChat_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)//엔터 누르면 자동으로 넘어가게 함
            {
                sendChat_Click(null, null);
                inputChat.Clear();//입력창 clear
                }

        }
        public void StopSound()
        {
            LobbySound.Stop();
        }
        public void Kill_Thread()
        {
            m_hThread.Abort();
        }

        private void RoomList_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void LobbyUserRank_Click(object sender, EventArgs e)
        {

        }

        private void LobbyUesrPoint_Click(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void userMakeSubject_MouseClick(object sender, MouseEventArgs e)
        {//333
            UserMakeSubject From=new UserMakeSubject(Socket,Login);

            From.Owner = this;
            Login.Opacity = 0.9;
            From.StartPosition = FormStartPosition.Manual;
            From.Location = new Point(200, 200);
            From.ShowDialog(); //켜진 창 끝날때까지 부모창 사용 불가
        }


    }
}