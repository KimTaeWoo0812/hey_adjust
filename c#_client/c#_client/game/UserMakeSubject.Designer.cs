namespace game
{
    partial class UserMakeSubject
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(UserMakeSubject));
            this.cancel = new System.Windows.Forms.Button();
            this.make = new System.Windows.Forms.Button();
            this.problem = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // cancel
            // 
            this.cancel.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("cancel.BackgroundImage")));
            this.cancel.Location = new System.Drawing.Point(196, 144);
            this.cancel.Name = "cancel";
            this.cancel.Size = new System.Drawing.Size(100, 45);
            this.cancel.TabIndex = 42;
            this.cancel.Text = "취소";
            this.cancel.UseVisualStyleBackColor = true;
            this.cancel.Click += new System.EventHandler(this.cancel_Click);
            // 
            // make
            // 
            this.make.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("make.BackgroundImage")));
            this.make.Location = new System.Drawing.Point(65, 144);
            this.make.Name = "make";
            this.make.Size = new System.Drawing.Size(100, 45);
            this.make.TabIndex = 41;
            this.make.Text = "추가";
            this.make.UseVisualStyleBackColor = true;
            this.make.Click += new System.EventHandler(this.make_Click);
            // 
            // problem
            // 
            this.problem.Location = new System.Drawing.Point(135, 60);
            this.problem.Name = "problem";
            this.problem.Size = new System.Drawing.Size(124, 25);
            this.problem.TabIndex = 39;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label1.Location = new System.Drawing.Point(92, 63);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 15);
            this.label1.TabIndex = 38;
            this.label1.Text = "문제";
            // 
            // UserMakeSubject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(345, 241);
            this.Controls.Add(this.cancel);
            this.Controls.Add(this.make);
            this.Controls.Add(this.problem);
            this.Controls.Add(this.label1);
            this.Name = "UserMakeSubject";
            this.Text = "UserMakeSubject";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cancel;
        private System.Windows.Forms.Button make;
        private System.Windows.Forms.TextBox problem;
        private System.Windows.Forms.Label label1;
    }
}