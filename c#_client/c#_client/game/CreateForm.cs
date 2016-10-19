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
    public partial class CreateForm : Form
    {
        private CSocket Socket;
        login Login;
        public CreateForm(CSocket socket, login _login)
        {
            InitializeComponent();
            Socket = socket;
            Login = _login;
            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            UpdateStyles();
        }

        private void btnCreate_Click(object sender, EventArgs e)
        {
            /*
            string strID = txtID.Text;
            string strPassword = textBox2.Text;

            MessageBox.Show(strID, "테스트");
            MessageBox.Show(strPassword, "테스트2");
            */
            char[] kor = new char[30];

            for (int i = 0; i < createName.Text.Length; i++)
                kor[i] = createName.Text[i];


            if (0xAC00 <= kor[0] && kor[0] <= 0xD7A3 || (0x3131 <= kor[0] && kor[0] <= 0x318E))//한글
            {
                MessageBox.Show("닉네임은 영어로 해주세요!!");
                return;
            }
            if (createID.Text == string.Empty)
            {
                MessageBox.Show("아이디를 입력하세요");
                return;
            }

            else if (createPW.Text == string.Empty)
            {
                MessageBox.Show("패스워드를 입력하세요");
                return;
            }

            else if (createPWchack.Text != createPW.Text)
            {
                MessageBox.Show("패스워드가 맞지 않습니다");
                return;
            }

            else if (createName.Text == string.Empty)
            {
                MessageBox.Show("이름을 입력하세요");
                return;
            }

            {// #이 있는지 확인
                int i,again=0;
                for (i = 0; i < createID.Text.Length; i++)// #이 있는지 확인
                {
                    if (createID.Text[i] == '#')
                    {
                        again = 1;
                        break;
                    }
                }
                for (i = 0; i < createPW.Text.Length; i++)// #이 있는지 확인
                {
                    if (createPW.Text[i] == '#')
                    {
                        again = 1;
                        break;
                    }
                }
                for (i = 0; i < createName.Text.Length; i++)// #이 있는지 확인
                {
                    if (createName.Text[i] == '#')
                    {
                        again = 1;
                        break;
                    }
                }
                if (again == 1)
                {
                    MessageBox.Show("# 문자를 쓸 수 없습니다!");
                    return;
                }
            }
            if (createID.Text.Length > 15 || createID.Text.Length < 5)
            {
                MessageBox.Show("ID는 5자 이상 15자 이하 입니다!");
                return;
            }
            else if (createPW.Text.Length > 15 || createPW.Text.Length < 5)
            {
                MessageBox.Show("PW는 5자 이상 15자 이하 입니다!");
                return;
            }
            else if (createName.Text.Length > 15)
            {
                MessageBox.Show("15자 이하 입니다!!");
                return;
            }



            else
            {
                SendInfoToCerver();
                GetOrderFromCerver();
                //MessageBox.Show("이름을 입력하세요");
            }
        }

        




        private void SendInfoToCerver()//주기
        {
            char[] InfoOfUser = new char[128];
            int i;

            

            InfoOfUser[0] = 'A';
            InfoOfUser[1] = 'M';
            InfoOfUser[2] = '_';
            InfoOfUser[3] = 'L';
            InfoOfUser[4] = 'O';
            InfoOfUser[5] = 'G';
            InfoOfUser[6] = 'I';
            InfoOfUser[7] = 'N';
            InfoOfUser[8] = '_';
            InfoOfUser[9] = 'C';
            InfoOfUser[10] = 'R';
            InfoOfUser[11] = 'E';
            InfoOfUser[12] = 'A';
            InfoOfUser[13] = 'T';
            InfoOfUser[14] = 'E';
            InfoOfUser[15] = '#';

            for (i = 21; i < createID.Text.Length + 21; i++)//ID 받기
                InfoOfUser[i] = createID.Text[i - 21];
            InfoOfUser[i] = '#';

            for (i = 41; i < createPW.Text.Length + 41; i++)//PW 받기
                InfoOfUser[i] = createPW.Text[i - 41];
            InfoOfUser[i] = '#';

            for (i = 61; i < createName.Text.Length + 61; i++)//이름 받기
                InfoOfUser[i] = createName.Text[i - 61];
            InfoOfUser[i] = '#';
            Socket.SendMessage(InfoOfUser);//서버에 보내기
        }

        private void GetOrderFromCerver()//받기
        {
            char[] getOrder = new char[30];

            while (!Socket.hasMessage());
            getOrder = Socket.GetMessage();



            if (getOrder[21] == '1')
            {
                MessageBox.Show("회원가입 완료!");
                Login.Opacity = 1;
                this.Close();
            }
            else
                MessageBox.Show("회원가입 실패!");
        }

        private void cancleCreate_Click(object sender, EventArgs e)
        {
            Login.Opacity = 1;
            this.Close();
        }
    }
}
