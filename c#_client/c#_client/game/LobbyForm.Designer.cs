namespace game
{
    partial class gameEnd
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(gameEnd));
            this.UserList = new System.Windows.Forms.ListBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.LobbyUserRank = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.LobbyUesrPoint = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.LobbyUserName = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.sendChat = new System.Windows.Forms.Button();
            this.outputChat = new System.Windows.Forms.TextBox();
            this.inputChat = new System.Windows.Forms.TextBox();
            this.makeRoom_Click = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.RoomList = new System.Windows.Forms.ListBox();
            this.listView1 = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.userMakeSubject = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // UserList
            // 
            this.UserList.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.UserList.FormattingEnabled = true;
            this.UserList.ItemHeight = 15;
            this.UserList.Location = new System.Drawing.Point(768, 92);
            this.UserList.Name = "UserList";
            this.UserList.ScrollAlwaysVisible = true;
            this.UserList.Size = new System.Drawing.Size(206, 499);
            this.UserList.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("groupBox1.BackgroundImage")));
            this.groupBox1.Controls.Add(this.LobbyUserRank);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.LobbyUesrPoint);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.LobbyUserName);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Font = new System.Drawing.Font("MDSol", 9F);
            this.groupBox1.Location = new System.Drawing.Point(768, 608);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(206, 169);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "유저정보";
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // LobbyUserRank
            // 
            this.LobbyUserRank.AutoSize = true;
            this.LobbyUserRank.Font = new System.Drawing.Font("MDSol", 9F);
            this.LobbyUserRank.Image = ((System.Drawing.Image)(resources.GetObject("LobbyUserRank.Image")));
            this.LobbyUserRank.Location = new System.Drawing.Point(78, 102);
            this.LobbyUserRank.Name = "LobbyUserRank";
            this.LobbyUserRank.Size = new System.Drawing.Size(23, 15);
            this.LobbyUserRank.TabIndex = 6;
            this.LobbyUserRank.Text = "llll";
            this.LobbyUserRank.Click += new System.EventHandler(this.LobbyUserRank_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("MDSol", 9F);
            this.label5.Image = ((System.Drawing.Image)(resources.GetObject("label5.Image")));
            this.label5.Location = new System.Drawing.Point(20, 102);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 15);
            this.label5.TabIndex = 5;
            this.label5.Text = "랭  킹";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // LobbyUesrPoint
            // 
            this.LobbyUesrPoint.AutoSize = true;
            this.LobbyUesrPoint.Font = new System.Drawing.Font("MDSol", 9F);
            this.LobbyUesrPoint.Image = ((System.Drawing.Image)(resources.GetObject("LobbyUesrPoint.Image")));
            this.LobbyUesrPoint.Location = new System.Drawing.Point(78, 69);
            this.LobbyUesrPoint.Name = "LobbyUesrPoint";
            this.LobbyUesrPoint.Size = new System.Drawing.Size(23, 15);
            this.LobbyUesrPoint.TabIndex = 4;
            this.LobbyUesrPoint.Text = "llll";
            this.LobbyUesrPoint.Click += new System.EventHandler(this.LobbyUesrPoint_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("MDSol", 9F);
            this.label3.Image = ((System.Drawing.Image)(resources.GetObject("label3.Image")));
            this.label3.Location = new System.Drawing.Point(15, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(52, 15);
            this.label3.TabIndex = 3;
            this.label3.Text = "포인트";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // LobbyUserName
            // 
            this.LobbyUserName.AutoSize = true;
            this.LobbyUserName.Font = new System.Drawing.Font("MDSol", 9F);
            this.LobbyUserName.Image = ((System.Drawing.Image)(resources.GetObject("LobbyUserName.Image")));
            this.LobbyUserName.Location = new System.Drawing.Point(78, 34);
            this.LobbyUserName.Name = "LobbyUserName";
            this.LobbyUserName.Size = new System.Drawing.Size(23, 15);
            this.LobbyUserName.TabIndex = 1;
            this.LobbyUserName.Text = "llll";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("MDSol", 9F);
            this.label1.Image = ((System.Drawing.Image)(resources.GetObject("label1.Image")));
            this.label1.Location = new System.Drawing.Point(20, 34);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "별  명";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // sendChat
            // 
            this.sendChat.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("sendChat.BackgroundImage")));
            this.sendChat.Font = new System.Drawing.Font("MDSol", 9F);
            this.sendChat.Location = new System.Drawing.Point(676, 785);
            this.sendChat.Name = "sendChat";
            this.sendChat.Size = new System.Drawing.Size(90, 29);
            this.sendChat.TabIndex = 2;
            this.sendChat.Text = "보내기";
            this.sendChat.UseVisualStyleBackColor = true;
            this.sendChat.Click += new System.EventHandler(this.sendChat_Click);
            this.sendChat.KeyDown += new System.Windows.Forms.KeyEventHandler(this.inputChat_KeyDown);
            // 
            // outputChat
            // 
            this.outputChat.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.outputChat.Location = new System.Drawing.Point(12, 608);
            this.outputChat.Multiline = true;
            this.outputChat.Name = "outputChat";
            this.outputChat.ReadOnly = true;
            this.outputChat.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.outputChat.Size = new System.Drawing.Size(750, 171);
            this.outputChat.TabIndex = 3;
            this.outputChat.TextChanged += new System.EventHandler(this.outputChat_TextChanged);
            // 
            // inputChat
            // 
            this.inputChat.Location = new System.Drawing.Point(12, 786);
            this.inputChat.Name = "inputChat";
            this.inputChat.Size = new System.Drawing.Size(658, 25);
            this.inputChat.TabIndex = 4;
            this.inputChat.KeyDown += new System.Windows.Forms.KeyEventHandler(this.inputChat_KeyDown);
            // 
            // makeRoom_Click
            // 
            this.makeRoom_Click.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("makeRoom_Click.BackgroundImage")));
            this.makeRoom_Click.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.makeRoom_Click.Font = new System.Drawing.Font("MDSol", 12F);
            this.makeRoom_Click.Location = new System.Drawing.Point(611, 25);
            this.makeRoom_Click.Name = "makeRoom_Click";
            this.makeRoom_Click.Size = new System.Drawing.Size(151, 61);
            this.makeRoom_Click.TabIndex = 8;
            this.makeRoom_Click.Text = "방만들기";
            this.makeRoom_Click.UseVisualStyleBackColor = true;
            this.makeRoom_Click.Click += new System.EventHandler(this.makeRoom_Click_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.label7.Font = new System.Drawing.Font("MDSol", 10F);
            this.label7.Location = new System.Drawing.Point(831, 74);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(76, 17);
            this.label7.TabIndex = 11;
            this.label7.Text = "유저목록";
            // 
            // RoomList
            // 
            this.RoomList.Font = new System.Drawing.Font("MDSol", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.RoomList.FormattingEnabled = true;
            this.RoomList.ItemHeight = 20;
            this.RoomList.Location = new System.Drawing.Point(12, 119);
            this.RoomList.Name = "RoomList";
            this.RoomList.Size = new System.Drawing.Size(750, 464);
            this.RoomList.TabIndex = 17;
            this.RoomList.MouseClick += new System.Windows.Forms.MouseEventHandler(this.RoomList_MouseDoubleClick);
            this.RoomList.SelectedIndexChanged += new System.EventHandler(this.RoomList_SelectedIndexChanged);
            this.RoomList.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.RoomList_MouseDoubleClick);
            // 
            // listView1
            // 
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
            this.listView1.Font = new System.Drawing.Font("MDSol", 12F);
            this.listView1.Location = new System.Drawing.Point(12, 92);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(750, 28);
            this.listView1.TabIndex = 18;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "방번";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "방제";
            this.columnHeader2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.columnHeader2.Width = 400;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "참가인원";
            this.columnHeader3.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.columnHeader3.Width = 90;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "시작여부";
            this.columnHeader4.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.columnHeader4.Width = 90;
            // 
            // userMakeSubject
            // 
            this.userMakeSubject.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("userMakeSubject.BackgroundImage")));
            this.userMakeSubject.Font = new System.Drawing.Font("MDSol", 9F);
            this.userMakeSubject.Location = new System.Drawing.Point(809, 786);
            this.userMakeSubject.Name = "userMakeSubject";
            this.userMakeSubject.Size = new System.Drawing.Size(165, 30);
            this.userMakeSubject.TabIndex = 19;
            this.userMakeSubject.Text = "사용자 게임 추가";
            this.userMakeSubject.UseVisualStyleBackColor = true;
            this.userMakeSubject.MouseClick += new System.Windows.Forms.MouseEventHandler(this.userMakeSubject_MouseClick);
            // 
            // gameEnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(990, 850);
            this.ControlBox = false;
            this.Controls.Add(this.userMakeSubject);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.RoomList);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.makeRoom_Click);
            this.Controls.Add(this.inputChat);
            this.Controls.Add(this.outputChat);
            this.Controls.Add(this.sendChat);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.UserList);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "gameEnd";
            this.Text = "LobbyForm";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox UserList;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button sendChat;
        private System.Windows.Forms.TextBox inputChat;
        private System.Windows.Forms.Label LobbyUserName;
        private System.Windows.Forms.Label LobbyUserRank;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label LobbyUesrPoint;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button makeRoom_Click;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox outputChat;
        private System.Windows.Forms.ListBox RoomList;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.Button userMakeSubject;
    }
}