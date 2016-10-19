namespace game
{
    partial class setupRoom
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(setupRoom));
            this.inputRoomName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.makeRoom = new System.Windows.Forms.Button();
            this.cancelsetupRoom = new System.Windows.Forms.Button();
            this.RoomMax = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // inputRoomName
            // 
            this.inputRoomName.Location = new System.Drawing.Point(133, 69);
            this.inputRoomName.Name = "inputRoomName";
            this.inputRoomName.Size = new System.Drawing.Size(124, 25);
            this.inputRoomName.TabIndex = 4;
            this.inputRoomName.TextChanged += new System.EventHandler(this.inputRoomName_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label1.Location = new System.Drawing.Point(84, 72);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 15);
            this.label1.TabIndex = 3;
            this.label1.Text = "방제목";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label2.Location = new System.Drawing.Point(84, 121);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(37, 15);
            this.label2.TabIndex = 5;
            this.label2.Text = "인원";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // makeRoom
            // 
            this.makeRoom.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("makeRoom.BackgroundImage")));
            this.makeRoom.Location = new System.Drawing.Point(61, 224);
            this.makeRoom.Name = "makeRoom";
            this.makeRoom.Size = new System.Drawing.Size(100, 45);
            this.makeRoom.TabIndex = 7;
            this.makeRoom.Text = "만들기";
            this.makeRoom.UseVisualStyleBackColor = true;
            this.makeRoom.Click += new System.EventHandler(this.setRoomName_Click);
            // 
            // cancelsetupRoom
            // 
            this.cancelsetupRoom.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("cancelsetupRoom.BackgroundImage")));
            this.cancelsetupRoom.Location = new System.Drawing.Point(192, 224);
            this.cancelsetupRoom.Name = "cancelsetupRoom";
            this.cancelsetupRoom.Size = new System.Drawing.Size(100, 45);
            this.cancelsetupRoom.TabIndex = 8;
            this.cancelsetupRoom.Text = "취소";
            this.cancelsetupRoom.UseVisualStyleBackColor = true;
            this.cancelsetupRoom.Click += new System.EventHandler(this.cancelsetupRoom_Click);
            // 
            // RoomMax
            // 
            this.RoomMax.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.RoomMax.ForeColor = System.Drawing.SystemColors.WindowText;
            this.RoomMax.FormattingEnabled = true;
            this.RoomMax.Items.AddRange(new object[] {
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8"});
            this.RoomMax.Location = new System.Drawing.Point(133, 121);
            this.RoomMax.Name = "RoomMax";
            this.RoomMax.Size = new System.Drawing.Size(124, 23);
            this.RoomMax.TabIndex = 37;
            this.RoomMax.SelectedIndexChanged += new System.EventHandler(this.makeRoom_SelectedIndexChanged);
            // 
            // setupRoom
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(362, 296);
            this.Controls.Add(this.RoomMax);
            this.Controls.Add(this.cancelsetupRoom);
            this.Controls.Add(this.makeRoom);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.inputRoomName);
            this.Controls.Add(this.label1);
            this.Name = "setupRoom";
            this.Text = "setupRoom";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox inputRoomName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button makeRoom;
        private System.Windows.Forms.Button cancelsetupRoom;
        private System.Windows.Forms.ComboBox RoomMax;
    }
}