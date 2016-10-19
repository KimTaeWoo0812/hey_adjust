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
    public partial class gameRoom : Form
    {
        private Info InfoData;
        private CSocket Socket;
        private Thread Room_hThread;
        private Thread picture_Thread;//영상 스레드
        private bool CheckNoChat;
        login Login;
        private char[] message = new char[128];
        public int peopleCount;
        private int[] peopleCountArr = new int[10];//나간사람확인위해
        private bool StartThread;
        private bool RoomExitThread = false;
        private char[] userNum = new char[5];
        public int hostOfRoom;//방장 인지? 1은 방장
        //public char[] RightTopic = new char[30];//정답 저장
        public string RightTopic;
        public int timeOutCount=0;//시간아웃으로 끝난건지 체크
        public int StreamSocketStart = 0;//영상 전송 시작부분
        private int ntopic = 0;//토픽에 따른 화면 전환을 위해
        Stopwatch Timer = new Stopwatch();//시간 측정
        CvCapture capture;
        IplImage image = new IplImage(320, 240, BitDepth.U8, 3);
        public bool RoomImageThread = false;//영상 쓰레드
        CStreamSocket StreamSocket;
        System.Media.SoundPlayer gameRoomSound;
        
        public gameRoom(CSocket socket, login _login)
        {
            CheckForIllegalCrossThreadCalls = false;

            InitializeComponent();
            InfoData = null;
            Socket = socket;
            Login = _login;
            Room_hThread = new Thread(new ThreadStart(RoomGetInfo));
            Room_hThread.Start();
            RoomPeopleSendToServer();//있는 사용자들 전부 받기
            //QuitLobby();
            _login.SetRoom(this);
            CheckNoChat = false;
            StartThread = false;
            hostOfRoom = 0;
            choiceSubject.Visible = false;
            StreamSocket = null;
            gameRoomSound = new System.Media.SoundPlayer(socket.MusicPath + "..\\gameRoom\\" + "gameRoom.wav");
            gameRoomSound.PlayLooping();//노래 재생
        }
        public void HostChack()
        {
            hostOfRoom = 1;
            picture_Thread = new Thread(new ThreadStart(PictureForHost));
            RoomReady.Text = "Game Start!";
            choiceSubject.Visible = true;
            beforPicture.Visible = false;
            if (initCamere())
                picture_Thread.Start();
            else
                MessageBox.Show("캠 연동 실패!");
        }
        private void RoomGetInfo()
        {
            while (true)
            {
                if (RoomExitThread) break;
                if (StartThread)
                {
                    if (CheckNoChat == false)
                    {
                        CheckNoChat = true;
                        while (!Socket.hasMessage())
                            if (RoomExitThread) break;
                        if (RoomExitThread) break;
                        char[] order = new char[30];
                        message = Socket.GetMessage();
                        for (int i = 0; i < 20; i++)
                            order[i] = message[i];

                        if (order[8] == 'U' && order[9] == 'S' && order[10] == 'E')
                        {
                            RoomPeople();//사용자들 정보 받기
                        }
                        else if (order[8] == 'C' && order[9] == 'H' && order[10] == 'A')
                        {
                            RoomLookAtChat();//채팅
                        }
                        else if (order[13] == 'I' && order[14] == 'N' && order[15] == 'F' && order[16] == 'O')
                        {
                            RoomInof();//방이름 등 방 정보
                        }
                        else if (order[8] == 'S' && order[9] == 'T' && order[10] == 'A' && order[11] == 'R' && order[12] == 'T')
                        {
                            if (hostOfRoom == 1 && message[21] == '0')
                                MessageBox.Show("모두 준비를 해야 시작할 수 있습니다!");//방장이 시작 눌렀을때
                            else if (message[21] == '1')
                            {
                                //게임 시작
                                StartGame();
                            }
                        }
                        else if (order[8] == 'O' && order[9] == 'U' && order[10] == 'T')
                        {
                            MessageBox.Show("방장이 나갔습니다! 방에서 나가주세요!");
                            break;
                        }
                        else if (order[8] == 'C' && order[9] == 'A' && order[10] == 'T' && order[11] == 'E' && order[12] == 'G' && order[13] == 'O' && order[14] == 'R' && order[15] == 'Y')
                        {
                            GetSubject();//서버로부터 주제 받기
                        }
                        else if (order[8] == 'R' && order[9] == 'I' && order[10] == 'G' && order[11] == 'H' && order[12] == 'T')
                        {
                            RightCount();//맞춘 갯수
                        }
                        else if (order[8] == 'S' && order[9] == 'U' && order[10] == 'C' && order[11] == 'C' && order[12] == 'E' && order[13] == 'S' && order[14] == 'S')
                        {
                            SuccessPeople();//맞춘 사람
                        }
                        else if (order[13] == 'T' && order[14] == 'O' && order[15] == 'P' && order[16] == 'I' && order[17] == 'C')
                        {
                            getTopic();//문제
                        }
                        else if (order[8] == 'E' && order[9] == 'N' && order[10] == 'D')
                        {
                            EndGame();//문제 다 맞추고 게임 끝날때
                        }
                        else if (order[8] == 'T' && order[9] == 'I' && order[10] == 'M' && order[11] == 'E' && order[12] == '_' && order[13] == 'O' && order[13] == 'U' && order[14] == 'T')
                        {
                            TimeOutGame();

                        }
                        CheckNoChat = false;
                    }
                }
            }

        }
        public void StartGame()//게임 시작
        {
            beforPicture.Visible = false;
            StopSound();
            switch (choiceSubject.SelectedIndex)
            {
                case 0:
                    gameRoomSound = new System.Media.SoundPlayer(Socket.MusicPath + "..\\animal\\" + "1animal.wav");//동물
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\animal.jpg");
                    gameRoomSound.PlayLooping();//노래 재생
                    break;
                case 1:
                    gameRoomSound = new System.Media.SoundPlayer(Socket.MusicPath + "..\\comic\\" + "comic.wav");//만화
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\comic.jpg");
                    gameRoomSound.PlayLooping();//노래 재생
                    break;
                case 2:
                    gameRoomSound = new System.Media.SoundPlayer(Socket.MusicPath + "..\\sports\\" + "sports.wav");//스포츠
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\sports.jpg");
                    gameRoomSound.PlayLooping();//노래 재생
                    break;
                case 3:
                    gameRoomSound = new System.Media.SoundPlayer(Socket.MusicPath + "..\\movie\\" + "1movie.wav");//영화
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\movie.jpg");
                    gameRoomSound.PlayLooping();//노래 재생
                    break;
                case 4:
                    gameRoomSound = new System.Media.SoundPlayer(Socket.MusicPath + "..\\userGame\\" + "userGame.wav");//사용자 정의
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\movie.jpg");
                    gameRoomSound.PlayLooping();//노래 재생
                    break;
            }
            switch (ntopic)
            {
                case 0:
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\animal.jpg");
                    break;
                case 1://만화
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\comic.jpg");
                    break;
                case 2:
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\sports.jpg");
                    break;
                case 3:
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\movie.jpg");
                    break;
                case 4:
                    this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\userGame.jpg");
                    break;
            }
            
            people2Ready.Text = string.Empty;
            people3Ready.Text = string.Empty;
            people4Ready.Text = string.Empty;
            people5Ready.Text = string.Empty;
            people6Ready.Text = string.Empty;
            people7Ready.Text = string.Empty;
            people8Ready.Text = string.Empty;

            StreamSocketStart = 1;
            MessageBox.Show("시작");
            if (hostOfRoom != 1)
                PictureForMemberThread_Start();
            Timer.Reset();
            Timer.Start();
        }
        private void StopSound()//노래 정지
        {
            gameRoomSound.Stop();
        }
        private void EndGame()//문제 다 맞추고 게임 끝날때
        {
            if (hostOfRoom != 1)
                beforPicture.Visible = true;
            this.BackgroundImage = Image.FromFile(Socket.MusicPath + "\\Room.png");
            StopSound();
            gameRoomSound = new System.Media.SoundPlayer(Socket.MusicPath + "..\\gameRoom\\" + "gameRoom.wav");
            gameRoomSound.PlayLooping();//노래 재생

            people2count.Text = string.Empty;
            people3count.Text = string.Empty;
            people4count.Text = string.Empty;
            people5count.Text = string.Empty;
            people6count.Text = string.Empty;
            people7count.Text = string.Empty;
            people8count.Text = string.Empty;
            topic.Text = string.Empty;
            StreamSocketStart = 0;
            Timer.Stop();

            if (timeOutCount == 1)//시간초과로 끝날때
            {
                if (hostOfRoom != 1)
                    beforPicture.Visible = true;

                char[] timeOut = new char[128];
                StopSound();
                timeOut[0] = 'A';
                timeOut[1] = 'M';
                timeOut[2] = '_';
                timeOut[3] = 'G';
                timeOut[4] = 'A';
                timeOut[5] = 'M';
                timeOut[6] = 'E';
                timeOut[7] = '_';
                timeOut[8] = 'T';
                timeOut[9] = 'I';
                timeOut[10] = 'M';
                timeOut[11] = 'E';
                timeOut[12] = '_';
                timeOut[13] = 'O';
                timeOut[14] = 'U';
                timeOut[15] = 'T';
                timeOut[16] = '#';
                topic.Text = string.Empty;

                Socket.SendMessage(timeOut);
                timeOutCount = 0;
            }
            MessageBox.Show("게임 끝");
        }
        private void TimeOutGame()//시간 초과로 게임 끝날때
        {
            Timer.Stop();

        }





        private void getTopic()//문제
        {
            string strtopic = "문제: ";
            byte[] byteTopic = new byte[100];
            int i;

            for (i = 21; message[i] != 65533&&i<128; i++)
                strtopic += message[i];

            if (hostOfRoom == 1)
            {
                topic.Text = strtopic;//방장은 문제 프린터
            }
            else//방원은 정답만 저장.
            {
                RightTopic = string.Empty;
                for (i = 21; message[i] != 65533; i++)
                    RightTopic += message[i];
            }
            Update();
        }

        private void SuccessPeople() //맞춘 사람
        {
            string strSuccessPeople = string.Empty;

            for (int i = 21; message[i] != '#'; i++)
                strSuccessPeople += message[i];

            MessageBox.Show(strSuccessPeople + "님이 정답을 맞추셨습니다!", "정답");
        }


        private void RightCount()//맞춘 갯수
        {
            string strRightCount = string.Empty;

            strRightCount += message[41];
            //if(message[21]=='1')//방장
            // {
            //   people1count.Text = strRightCount;
            //}
            if (message[21] == '2')
            {
                people2count.Text = strRightCount;
            }
            else if (message[21] == '3')
            {
                people3count.Text = strRightCount;
            }
            else if (message[21] == '4')
            {
                people4count.Text = strRightCount;
            }
            else if (message[21] == '5')
            {
                people5count.Text = strRightCount;
            }
            else if (message[21] == '6')
            {
                people6count.Text = strRightCount;
            }
            else if (message[21] == '7')
            {
                people7count.Text = strRightCount;
            }
            else if (message[21] == '8')
            {
                people8count.Text = strRightCount;
            }
            //Update();
        }



        private void RoomPeopleSendToServer()//서버에 전송
        {
            char[] messageRoomPeople = new char[15];

            messageRoomPeople[0] = 'A';
            messageRoomPeople[1] = 'M';
            messageRoomPeople[2] = '_';
            messageRoomPeople[3] = 'R';
            messageRoomPeople[4] = 'O';
            messageRoomPeople[5] = 'O';
            messageRoomPeople[6] = 'M';
            messageRoomPeople[7] = '_';
            messageRoomPeople[8] = 'I';
            messageRoomPeople[9] = 'N';
            messageRoomPeople[10] = 'F';
            messageRoomPeople[11] = 'O';
            messageRoomPeople[12] = '#';
            peopleCount = 0;
            Socket.SendMessage(messageRoomPeople);


        }
        private void RoomPeople()//사람들 정보 받음
        {
            string strPeopleNum = string.Empty;
            string strPeopleName = string.Empty;
            string strPeoplePoint = string.Empty;
            string strPeopleRank = string.Empty;
            string strPeopleReady = string.Empty;
            int i;

            strPeopleNum += message[21];//이게 0이면 그만받음

            if (strPeopleNum[0] == '0')
            {
                peopleCount = 0;
                for (i = 1; i < 8; i++)//나간사람 지우기
                {
                    if (peopleCountArr[i] == 0)
                    {
                        switch (i)
                        {
                            case 0:
                                people1.Visible = false;
                                /*
                                people1Name.Text = string.Empty;
                                people1Point.Text = string.Empty;
                                people1Rank.Text = string.Empty;
                                people1Ready.Text = string.Empty;
                                people1NameLabel.Text = string.Empty;
                                people1PointLabel.Text = string.Empty;
                                people1RankLabel.Text = string.Empty;
                                //people1count.Text = string.Empty;
                                //people1Corict.Text = string.Empty;
                                 * */
                                break;
                            case 1:
                                people2.Visible = false;
                                /*
                                people2Name.Text = string.Empty;
                                people2Point.Text = string.Empty;
                                people2Rank.Text = string.Empty;
                                people2Ready.Text = string.Empty;
                                people2NameLabel.Text = string.Empty;
                                people2PointLabel.Text = string.Empty;
                                people2RankLabel.Text = string.Empty;
                                people2count.Text = string.Empty;
                                people2Corict.Text = string.Empty;
                                 * */
                                break;
                            case 2:
                                people3.Visible = false;
                                /*
                                people3Name.Text = string.Empty;
                                people3Point.Text = string.Empty;
                                people3Rank.Text = string.Empty;
                                people3Ready.Text = string.Empty;
                                people3NameLabel.Text = string.Empty;
                                people3PointLabel.Text = string.Empty;
                                people3RankLabel.Text = string.Empty;
                                people3count.Text = string.Empty;
                                people3Corict.Text = string.Empty;
                                 * */
                                break;
                            case 3:
                                people4.Visible = false;
                                /*
                                people4Name.Text = string.Empty;
                                people4Point.Text = string.Empty;
                                people4Rank.Text = string.Empty;
                                people4Ready.Text = string.Empty;
                                people4NameLabel.Text = string.Empty;
                                people4PointLabel.Text = string.Empty;
                                people4RankLabel.Text = string.Empty;
                                people4count.Text = string.Empty;
                                people4Corict.Text = string.Empty;
                                 * */
                                break;
                            case 4:
                                people5.Visible = false;
                                /*
                                people5Name.Text = string.Empty;
                                people5Point.Text = string.Empty;
                                people5Rank.Text = string.Empty;
                                people5Ready.Text = string.Empty;
                                people5NameLabel.Text = string.Empty;
                                people5PointLabel.Text = string.Empty;
                                people5RankLabel.Text = string.Empty;
                                people5count.Text = string.Empty;
                                people5Corict.Text = string.Empty;
                                 * */
                                break;
                            case 5:
                                people6.Visible = false;
                                /*
                                people6Name.Text = string.Empty;
                                people6Point.Text = string.Empty;
                                people6Rank.Text = string.Empty;
                                people6Ready.Text = string.Empty;
                                people6NameLabel.Text = string.Empty;
                                people6PointLabel.Text = string.Empty;
                                people6RankLabel.Text = string.Empty;
                                people6count.Text = string.Empty;
                                people6Corict.Text = string.Empty;
                                 * */
                                break;
                            case 6:
                                people7.Visible = false;
                                /*
                                people7Name.Text = string.Empty;
                                people7Point.Text = string.Empty;
                                people7Rank.Text = string.Empty;
                                people7Ready.Text = string.Empty;
                                people7NameLabel.Text = string.Empty;
                                people7PointLabel.Text = string.Empty;
                                people7RankLabel.Text = string.Empty;
                                people7count.Text = string.Empty;
                                people7Corict.Text = string.Empty;
                                 * */
                                break;
                            case 7:
                                people8.Visible = false;
                                /*
                                people8Name.Text = string.Empty;
                                people8Point.Text = string.Empty;
                                people8Rank.Text = string.Empty;
                                people8Ready.Text = string.Empty;
                                people8NameLabel.Text = string.Empty;
                                people8PointLabel.Text = string.Empty;
                                people8RankLabel.Text = string.Empty;
                                people8count.Text = string.Empty;
                                people8Corict.Text = string.Empty;
                                 * */
                                break;
                        }
                    }
                    else if (peopleCountArr[i] == 1)
                    {
                        switch (i)
                        {
                            case 0:
                                people1NameLabel.Text = "별  명";
                                people1PointLabel.Text = "포인트";
                                people1RankLabel.Text = "랭  킹";
                                people1.Visible = true;
                                break;
                            case 1:
                                people2NameLabel.Text = "별  명";
                                people2PointLabel.Text = "포인트";
                                people2RankLabel.Text = "랭  킹";
                                people2Corict.Text = "맞춘 갯수 :";
                                people2.Visible = true;
                                break;
                            case 2:
                                people3NameLabel.Text = "별  명";
                                people3PointLabel.Text = "포인트";
                                people3RankLabel.Text = "랭  킹";
                                people3Corict.Text = "맞춘 갯수 :";
                                people3.Visible = true;
                                break;
                            case 3:
                                people4NameLabel.Text = "별  명";
                                people4PointLabel.Text = "포인트";
                                people4RankLabel.Text = "랭  킹";
                                people4Corict.Text = "맞춘 갯수 :";
                                people4.Visible = true;
                                break;
                            case 4:
                                people5NameLabel.Text = "별  명";
                                people5PointLabel.Text = "포인트";
                                people5RankLabel.Text = "랭  킹";
                                people5Corict.Text = "맞춘 갯수 :";
                                people5.Visible = true;
                                break;
                            case 5:
                                people6NameLabel.Text = "별  명";
                                people6PointLabel.Text = "포인트";
                                people6RankLabel.Text = "랭  킹";
                                people6Corict.Text = "맞춘 갯수 :";
                                people6.Visible = true;
                                break;
                            case 6:
                                people7NameLabel.Text = "별  명";
                                people7PointLabel.Text = "포인트";
                                people7RankLabel.Text = "랭  킹";
                                people7Corict.Text = "맞춘 갯수 :";
                                people7.Visible = true;
                                break;
                            case 7:
                                people8NameLabel.Text = "별  명";
                                people8PointLabel.Text = "포인트";
                                people8RankLabel.Text = "랭  킹";
                                people8Corict.Text = "맞춘 갯수 :";
                                people8.Visible = true;
                                break;
                        }
                    }
                }

                for (i = 0; i < 8; i++)
                    peopleCountArr[i] = 0;

                Update();
                return;
            }

            for (i = 21; message[i] != '#'; i++)//순서 저장
                peopleCount = message[i] - 49;

            peopleCountArr[peopleCount] = 1;

            for (i = 41; message[i] != '#'; i++)//이름 저장
                strPeopleName += message[i];

            for (i = 61; message[i] != '#'; i++)//포인트 저장
                strPeoplePoint += message[i];

            for (i = 81; message[i] != '#'; i++)//랭크 저장
                strPeopleRank += message[i];

            for (i = 101; message[i] != '#'; i++)//레디했나?
                strPeopleReady += message[i];

            //스트링  초기화하고 자리에 정보 저장
            {
                switch (peopleCount)
                {
                    case 0:
                        people1Name.Text = strPeopleName;
                        people1Point.Text = strPeoplePoint;
                        people1Rank.Text = strPeopleRank;
                        if (strPeopleReady == "1")
                            people1Ready.Text = "Ready";
                        else
                            people1Ready.Text = string.Empty;

                        break;
                    case 1:
                        people2Name.Text = strPeopleName;
                        people2Point.Text = strPeoplePoint;
                        people2Rank.Text = strPeopleRank;
                        if (strPeopleReady == "1")
                            people2Ready.Text = "Ready";
                        else
                            people2Ready.Text = string.Empty;

                        break;
                    case 2:
                        people3Name.Text = strPeopleName;
                        people3Point.Text = strPeoplePoint;
                        people3Rank.Text = strPeopleRank;
                        if (strPeopleReady == "1")
                            people3Ready.Text = "Ready";
                        else
                            people3Ready.Text = string.Empty;

                        break;
                    case 3:
                        people4Name.Text = strPeopleName;
                        people4Point.Text = strPeoplePoint;
                        people4Rank.Text = strPeopleRank;
                        if (strPeopleReady == "1")
                            people4Ready.Text = "Ready";
                        else
                            people4Ready.Text = string.Empty;

                        break;
                    case 4:
                        people5Name.Text = strPeopleName;
                        people5Point.Text = strPeoplePoint;
                        people5Rank.Text = strPeopleRank;
                        if (strPeopleReady == "1")
                            people5Ready.Text = "Ready";
                        else
                            people5Ready.Text = string.Empty;

                        break;
                    case 5:
                        people6Name.Text = strPeopleName;
                        people6Point.Text = strPeoplePoint;
                        people6Rank.Text = strPeopleRank;
                        if (strPeopleReady == "1")
                            people6Ready.Text = "Ready";
                        else
                            people6Ready.Text = string.Empty;

                        break;
                    case 6:
                        people7Name.Text = strPeopleName;
                        people7Point.Text = strPeoplePoint;
                        people7Rank.Text = strPeopleRank;
                        if (strPeopleReady == "1")
                            people7Ready.Text = "Ready";
                        else
                            people7Ready.Text = string.Empty;

                        break;
                    case 7:
                        people8Name.Text = strPeopleName;
                        people8Point.Text = strPeoplePoint;
                        people8Rank.Text = strPeopleRank;
                        if (strPeopleReady == "1")
                            people8Ready.Text = "Ready";
                        else
                            people8Ready.Text = string.Empty;

                        break;
                }
                strPeopleName = string.Empty;
                strPeoplePoint = string.Empty;
                strPeopleRank = string.Empty;
                strPeopleReady = string.Empty;
                Update();
                StartThread = true;
            }
            peopleCount++;

        }

        public void GoThread()
        {
            StartThread = true;
        }


        private void RoomLookAtChat()//서버로부터 텍스트 내용 받음
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

            if (RoomOutputChat.Text != string.Empty)
                RoomOutputChat.Text += "\r\n";
            RoomOutputChat.AppendText(strChat);
            //RoomInputChat.Clear();//입력창 clear

        }
        private void RoomSendChat_Click(object sender, EventArgs e)
        {
            char[] lobbyChat = new char[128];
            string strlobbyChat = RoomInputChat.Text;
            int i,temp=0;

            if (RightTopic != null&&RightTopic.Length == strlobbyChat.Length)
            {
                for (i = 0; i < RightTopic.Length; i++)
                {
                    if (RightTopic != strlobbyChat)
                    {
                        temp = 1;
                        break;
                    }
                }
                if (temp == 1)
                    return;
            
            //if (string.Compare(RightTopic, strlobbyChat) == 0)//정답 확인
            
                char[] gameSccessMessage = new char[128];
                gameSccessMessage[0] = 'A';
                gameSccessMessage[1] = 'M';
                gameSccessMessage[2] = '_';
                gameSccessMessage[3] = 'G';
                gameSccessMessage[4] = 'A';
                gameSccessMessage[5] = 'M';
                gameSccessMessage[6] = 'E';
                gameSccessMessage[7] = '_';
                gameSccessMessage[8] = 'S';
                gameSccessMessage[9] = 'U';
                gameSccessMessage[10] = 'C';
                gameSccessMessage[11] = 'C';
                gameSccessMessage[12] = 'E';
                gameSccessMessage[13] = 'S';
                gameSccessMessage[14] = 'S';
                gameSccessMessage[15] = '#';
                Socket.SendMessage(gameSccessMessage);
                return;
            }
            


            lobbyChat[0] = 'A';
            lobbyChat[1] = 'M';
            lobbyChat[2] = '_';
            lobbyChat[3] = 'R';
            lobbyChat[4] = 'O';
            lobbyChat[5] = 'O';
            lobbyChat[6] = 'M';
            lobbyChat[7] = '_';
            lobbyChat[8] = 'C';
            lobbyChat[9] = 'H';
            lobbyChat[10] = 'A';
            lobbyChat[11] = 'T';
            lobbyChat[12] = '#';

            for (i = 21; i < strlobbyChat.Length + 21; i++)//# 제거
            {
                lobbyChat[i] = strlobbyChat[i - 21];
                if (lobbyChat[i] == '#')
                    lobbyChat[i] = ' ';
            }
            lobbyChat[i] = '#';
            Socket.SendMessage(lobbyChat);
            RoomInputChat.Clear();
        }
        private void RoomInputChat_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)//엔터 누르면 자동으로 넘어가게 함
            {
                RoomSendChat_Click(null, null);
                RoomInputChat.Clear();//입력창 clear
            }
        }

        public void SetInfoData(Info data)//사용자 데이타 받아오기
        {
            InfoData = data;

            //데이터 넘기기
            char[] szData = InfoData.OutputName();//이름
            string strData = string.Empty;
            for (int i = 0; i < szData.Length; ++i)
                strData += szData[i];

            gameRoomUserName.Text = strData;

            szData = InfoData.OutputPoint();//포인트
            strData = string.Empty;
            for (int i = 0; i < szData.Length; ++i)
                strData += szData[i];

            gameRoomUesrPoint.Text = strData;

            szData = InfoData.OutputRank();//순위
            strData = string.Empty;
            for (int i = 0; i < szData.Length; ++i)
                strData += szData[i];

            gameRoomUserRank.Text = strData;
        }

        public void RoomInof()//방이름, 최대인원
        {//ip
            int i;
            roomNum.Text = string.Empty;
            roomName.Text = string.Empty;
            roomMax.Text = string.Empty;
            roomPeople.Text = string.Empty;

            for (i = 21; message[i] != '#'; i++)//방번호 저장
                roomNum.Text += message[i];

            if (StreamSocket == null)   //123
            {
                StreamSocket = new CStreamSocket();
                //StreamSocket.Initialize("210.118.75.146", 9190, Convert.ToInt32(roomNum.Text));
                StreamSocket.Initialize("192.168.159.1", 9190, Convert.ToInt32(roomNum.Text));
                //StreamSocket.Initialize("192.168.0.69", 9190, Convert.ToInt32(roomNum.Text));//내 데스크탑
                //socket.Initialize("192.168.0.64", 7788);
            }

            
            for (i = 41; message[i] != '#'; i++)//방제목 저장
                roomName.Text += message[i];

            int j;
            for (j = 50; message[j] != '#'; j++) ;

            roomMax.Text += message[j-1];//최대인원 저장

            for (j = 70; message[j] != '#'; j++) ;

            roomPeople.Text += message[j-1];//현재인원 저장


            //roomName.Text = RoomName;
            //roomMax.Text = RoomMax;
            //roomNum.Text = strRoomNum;
            Update();
        }

        public void outAtRoom_Click(object sender, EventArgs e)
        {//나가기
            //123123
            //gameEnd lobby = new gameEnd(Socket, Login);
            StopSound();
            RoomExitThread = true;

            //Room_hThread.Join();
            //Room_hThread.Abort();

            gameEnd lobby = new gameEnd(Socket, Login);
            char[] quitMessageToCerver = new char[15];
            quitMessageToCerver[0] = 'A';
            quitMessageToCerver[1] = 'M';
            quitMessageToCerver[2] = '_';
            quitMessageToCerver[3] = 'R';
            quitMessageToCerver[4] = 'O';
            quitMessageToCerver[5] = 'O';
            quitMessageToCerver[6] = 'M';
            quitMessageToCerver[7] = '_';
            quitMessageToCerver[8] = 'P';
            quitMessageToCerver[9] = 'R';
            quitMessageToCerver[10] = 'E';
            quitMessageToCerver[11] = 'V';
            quitMessageToCerver[12] = '#';

            Timer.Stop();
            Login.SetRoom(this);
            lobby.SetInfoData(InfoData);
            lobby.Owner = Login;
            lobby.TopLevel = false;
            Login.Controls.Add(lobby);
            Login.IsMdiContainer = true;
            Socket.SendMessage(quitMessageToCerver);
            lobby.Show();
            StreamSocket.Release();//스트리밍 소켓 종료
            this.Close();
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        public void roomMax_Click(object sender, EventArgs e)
        {

        }

        private void label21_Click(object sender, EventArgs e)
        {

        }
        public void Kill_RoomThread()
        {
            Room_hThread.Abort();
        }

        private void RoomReady_Click(object sender, EventArgs e)//레디 클릭
        {
            char[] readyMessageToCerver = new char[15];

            if (hostOfRoom == 0)
            {
                readyMessageToCerver[0] = 'A';
                readyMessageToCerver[1] = 'M';
                readyMessageToCerver[2] = '_';
                readyMessageToCerver[3] = 'R';
                readyMessageToCerver[4] = 'O';
                readyMessageToCerver[5] = 'O';
                readyMessageToCerver[6] = 'M';
                readyMessageToCerver[7] = '_';
                readyMessageToCerver[8] = 'R';
                readyMessageToCerver[9] = 'E';
                readyMessageToCerver[10] = 'A';
                readyMessageToCerver[11] = 'D';
                readyMessageToCerver[12] = 'Y';
                readyMessageToCerver[13] = '#';


                if (RoomReady.Text == "READY")
                {
                    RoomReady.Text = "준비완료!";
                }
                else if (RoomReady.Text == "준비완료!")
                {
                    RoomReady.Text = "READY";
                }
                Socket.SendMessage(readyMessageToCerver);
            }
            if (hostOfRoom == 1 && choiceSubject.SelectedIndex == -1)
            {
                MessageBox.Show("카테고리를 선택하세요!!");
                return;
            }
            if (hostOfRoom == 1)//방장이 레디 클릭
            {
                readyMessageToCerver[0] = 'A';
                readyMessageToCerver[1] = 'M';
                readyMessageToCerver[2] = '_';
                readyMessageToCerver[3] = 'R';
                readyMessageToCerver[4] = 'O';
                readyMessageToCerver[5] = 'O';
                readyMessageToCerver[6] = 'M';
                readyMessageToCerver[7] = '_';
                readyMessageToCerver[8] = 'S';
                readyMessageToCerver[9] = 'T';
                readyMessageToCerver[10] = 'A';
                readyMessageToCerver[11] = 'R';
                readyMessageToCerver[12] = 'T';
                readyMessageToCerver[13] = '#';

                Socket.SendMessage(readyMessageToCerver);

            }
        }

        //unsafe
        private void PictureForHost()//방장 영상 스레드
        {
            while (true)
            {
                if (RoomExitThread == true)
                    break;

                image = capture.QueryFrame();

                //try{
                if (image == null) continue;
                    pictureBox1.Image = image.ToBitmap();
                //}catch(ArgumentException){
               //     continue;
               // }

                if (StreamSocketStart == 1)
                {
                    StreamSocket.ResetFlag();
                    StreamSocket.SendMessage(image);

                    while (!StreamSocket.getFlag())
                        if (RoomExitThread == true)
                            break;
                }

                //Bitmap bitmap = (Bitmap)BitmapConverter.ToBitmap(image);

                //시작 잡을 것
                /*
                System.IO.MemoryStream BITMAPMST = new MemoryStream();
                bitmap.Save(BITMAPMST, System.Drawing.Imaging.ImageFormat.Bmp);

                System.IO.MemoryStream JPEGMST = new MemoryStream();
                bitmap.Save(JPEGMST, System.Drawing.Imaging.ImageFormat.Jpeg);

                byte[] bTemp = BITMAPMST.ToArray();
                //스트리밍 서버로 바이트 배열 실시간 전송
                StreamSocket.SendMessage(image);
                */
                //pictureBoxIpl1.ImageIpl = image;
            }
        }
        
        public void PictureForMemberThread_Start()//맴버 영상 스레드 시작
        {
            picture_Thread = new Thread(new ThreadStart(PictureForMember));
            picture_Thread.Start();
        }
        private void PictureForMember()//맴버 영상 스레드
        {
            while (true)
            {
                if (StreamSocket == null && RoomExitThread != true) continue;

                if (RoomExitThread == true)
                    break;
                while (!StreamSocket.hasMessage())
                    if (RoomExitThread == true)
                        break;
                //서버로 부터 데이터 받기

                //pictureBoxIpl1.Image = StreamSocket.getImage();

                if (StreamSocket.hasImage())
                {
                    pictureBox1.Image = StreamSocket.getImage();
                    StreamSocket.ResetFlag();
                }

                StreamSocket.SendFlag();
            }
        }
        private void gameRoom_Load(object sender, EventArgs e)
        {
        }

        private void gameRoom_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (timer1 != null)
                timer1.Enabled = false;
            if (capture != null)
                capture.Dispose();
        }

        private bool initCamere()
        {
            try
            {
                capture = CvCapture.FromCamera(CaptureDevice.DShow, 0);
                capture.SetCaptureProperty(CaptureProperty.FrameWidth, 320);
                capture.SetCaptureProperty(CaptureProperty.FrameHeight, 240);

                return true;
            }
            catch
            {
                return false;
            }
        }

        private void StartTimer()
        {
            timer1.Interval = 33;
            timer1.Enabled = true;
        }



        private void choiceSubject_SelectedIndexChanged(object sender, EventArgs e)//방장 주제선택
        {
            int choice;
            char[] charChoice = new char[128];

            choice = choiceSubject.SelectedIndex;

            //약속어 넣어야댐
            charChoice[0] = 'A';
            charChoice[1] = 'M';
            charChoice[2] = '_';
            charChoice[3] = 'R';
            charChoice[4] = 'O';
            charChoice[5] = 'O';
            charChoice[6] = 'M';
            charChoice[7] = '_';
            charChoice[8] = 'C';
            charChoice[9] = 'A';
            charChoice[10] = 'T';
            charChoice[11] = 'E';
            charChoice[12] = 'G';
            charChoice[13] = 'O';
            charChoice[14] = 'R';
            charChoice[15] = 'Y';
            charChoice[16] = '#';


            charChoice[21] = (char)(choice + 48);
            charChoice[22] = '#';
            if (charChoice[21] == '0')
            {
                Socket.SendMessage(charChoice);
            }
            else if (charChoice[21] == '1')
            {
                Socket.SendMessage(charChoice);
            }
            else if (charChoice[21] == '2')
            {
                Socket.SendMessage(charChoice);
            }
            else if (charChoice[21] == '3')
            {
                Socket.SendMessage(charChoice);
            }
            else if (charChoice[21] == '4')
            {
                Socket.SendMessage(charChoice);
            }
        }
        private void GetSubject()
        {
            if (message[21] == '0')
            {
                printSubject.Text = "동물";
                ntopic = 0;
            }
            else if (message[21] == '1')
            {
                printSubject.Text = "만화";
                ntopic = 1;
            }
            else if (message[21] == '2')
            {
                printSubject.Text = "스포츠";
                ntopic = 2;
            }
            else if (message[21] == '3')
            {
                printSubject.Text = "영화";
                ntopic = 3;
            }
            else if (message[21] == '4')
            {
                printSubject.Text = "사용자 정의 게임";
                ntopic = 4;
            }
            Update();
        }




        private void people4_Paint(object sender, PaintEventArgs e)
        {

        }

        private void WatchToTime_Tick(object sender, EventArgs e)//타이머
        {
            TimeSpan Time = Timer.Elapsed;

            if (300 - Time.Seconds == 0)
            {
                timeOutCount = 1;
                EndGame();
                Timer.Stop();
                return;
            }

            gameTimer.Text = Convert.ToString(300 - ((Time.Minutes * 60) + Time.Seconds)) + "초";

        }

        private void pictureBoxIpl1_Click_1(object sender, EventArgs e)
        {

        }

        private void RoomInputChat_TextChanged(object sender, EventArgs e)
        {

        }


    }
}
