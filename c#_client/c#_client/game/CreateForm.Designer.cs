namespace game
{
    partial class CreateForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CreateForm));
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.createID = new System.Windows.Forms.TextBox();
            this.createPW = new System.Windows.Forms.TextBox();
            this.btnCreate = new System.Windows.Forms.Button();
            this.cancleCreate = new System.Windows.Forms.Button();
            this.createPWchack = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.createName = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label1.Location = new System.Drawing.Point(103, 52);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(20, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "ID";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label2.Location = new System.Drawing.Point(92, 85);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(31, 15);
            this.label2.TabIndex = 1;
            this.label2.Text = "PW";
            // 
            // createID
            // 
            this.createID.Location = new System.Drawing.Point(152, 52);
            this.createID.Name = "createID";
            this.createID.Size = new System.Drawing.Size(141, 25);
            this.createID.TabIndex = 2;
            // 
            // createPW
            // 
            this.createPW.Location = new System.Drawing.Point(152, 85);
            this.createPW.Name = "createPW";
            this.createPW.Size = new System.Drawing.Size(141, 25);
            this.createPW.TabIndex = 3;
            // 
            // btnCreate
            // 
            this.btnCreate.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnCreate.BackgroundImage")));
            this.btnCreate.Location = new System.Drawing.Point(71, 257);
            this.btnCreate.Name = "btnCreate";
            this.btnCreate.Size = new System.Drawing.Size(100, 45);
            this.btnCreate.TabIndex = 4;
            this.btnCreate.Text = "회원가입";
            this.btnCreate.UseVisualStyleBackColor = true;
            this.btnCreate.Click += new System.EventHandler(this.btnCreate_Click);
            // 
            // cancleCreate
            // 
            this.cancleCreate.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("cancleCreate.BackgroundImage")));
            this.cancleCreate.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cancleCreate.Location = new System.Drawing.Point(193, 257);
            this.cancleCreate.Name = "cancleCreate";
            this.cancleCreate.Size = new System.Drawing.Size(100, 45);
            this.cancleCreate.TabIndex = 5;
            this.cancleCreate.Text = "취소";
            this.cancleCreate.UseVisualStyleBackColor = true;
            this.cancleCreate.Click += new System.EventHandler(this.cancleCreate_Click);
            // 
            // createPWchack
            // 
            this.createPWchack.Location = new System.Drawing.Point(152, 125);
            this.createPWchack.Name = "createPWchack";
            this.createPWchack.Size = new System.Drawing.Size(141, 25);
            this.createPWchack.TabIndex = 7;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label3.Location = new System.Drawing.Point(48, 128);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(81, 15);
            this.label3.TabIndex = 6;
            this.label3.Text = "PW 재입력";
            // 
            // createName
            // 
            this.createName.Location = new System.Drawing.Point(152, 165);
            this.createName.Name = "createName";
            this.createName.Size = new System.Drawing.Size(141, 25);
            this.createName.TabIndex = 9;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label4.Location = new System.Drawing.Point(92, 168);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 15);
            this.label4.TabIndex = 8;
            this.label4.Text = "이름";
            // 
            // CreateForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(372, 343);
            this.Controls.Add(this.createName);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.createPWchack);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cancleCreate);
            this.Controls.Add(this.btnCreate);
            this.Controls.Add(this.createPW);
            this.Controls.Add(this.createID);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "CreateForm";
            this.Text = "CreateForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox createID;
        private System.Windows.Forms.TextBox createPW;
        private System.Windows.Forms.Button btnCreate;
        private System.Windows.Forms.Button cancleCreate;
        private System.Windows.Forms.TextBox createPWchack;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox createName;
        private System.Windows.Forms.Label label4;
    }
}