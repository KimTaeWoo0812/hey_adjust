using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace game
{

    public class Info
    {
        char[] UserName;
        char[] UserPoint;
        char[] UserRank;

        public Info()
        {
            UserName = new char[20];
            UserPoint = new char[20];
            UserRank = new char[20];
        }

        public void InputName(char[] data)
        {
            UserName = data;
        }

        public void InputPoint(char[] data)
        {
            UserPoint = data;
        }

        public void InputRank(char[] data)
        {
            UserRank = data;
        }

        public char[] OutputName()
        {
            return UserName;
        }

        public char[] OutputPoint()
        {
            return UserPoint;
        }

        public char[] OutputRank()
        {
            return UserRank;
        }
    }
}
