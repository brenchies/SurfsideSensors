
namespace AQS_DEMO
{
    partial class AQS_Demo_Form
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
            this.components = new System.ComponentModel.Container();
            this.serialPortAQS = new System.IO.Ports.SerialPort(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.panel4 = new System.Windows.Forms.Panel();
            this.panel5 = new System.Windows.Forms.Panel();
            this.panel6 = new System.Windows.Forms.Panel();
            this.panel7 = new System.Windows.Forms.Panel();
            this.panel8 = new System.Windows.Forms.Panel();
            this.panel9 = new System.Windows.Forms.Panel();
            this.panel10 = new System.Windows.Forms.Panel();
            this.lblPMS1_1_0 = new System.Windows.Forms.Label();
            this.lblPMS1_2_5 = new System.Windows.Forms.Label();
            this.lblPMS1_10_0 = new System.Windows.Forms.Label();
            this.label16lblPMS2_10_0 = new System.Windows.Forms.Label();
            this.lblPMS2_2_5 = new System.Windows.Forms.Label();
            this.lblPMS2_1_0 = new System.Windows.Forms.Label();
            this.lblHum = new System.Windows.Forms.Label();
            this.lblTemp = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            this.panel10.SuspendLayout();
            this.SuspendLayout();
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(511, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(295, 32);
            this.label1.TabIndex = 0;
            this.label1.Text = "Air Quality Monitoring ";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.Color.White;
            this.label2.Location = new System.Drawing.Point(268, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 32);
            this.label2.TabIndex = 1;
            this.label2.Text = "PMS1";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.Color.White;
            this.label3.Location = new System.Drawing.Point(564, 48);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(92, 32);
            this.label3.TabIndex = 2;
            this.label3.Text = "PMS2";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.Color.Transparent;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.White;
            this.label4.Location = new System.Drawing.Point(12, 41);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(95, 32);
            this.label4.TabIndex = 3;
            this.label4.Text = "Temp:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.BackColor = System.Drawing.Color.Transparent;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.White;
            this.label5.Location = new System.Drawing.Point(12, 82);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(89, 32);
            this.label5.TabIndex = 4;
            this.label5.Text = "Hum: ";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.BackColor = System.Drawing.Color.Transparent;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.ForeColor = System.Drawing.Color.White;
            this.label7.Location = new System.Drawing.Point(14, 28);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(105, 32);
            this.label7.TabIndex = 7;
            this.label7.Text = "PM1.0:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.BackColor = System.Drawing.Color.Transparent;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.ForeColor = System.Drawing.Color.White;
            this.label8.Location = new System.Drawing.Point(14, 69);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(105, 32);
            this.label8.TabIndex = 8;
            this.label8.Text = "PM2.5:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.BackColor = System.Drawing.Color.Transparent;
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.ForeColor = System.Drawing.Color.White;
            this.label9.Location = new System.Drawing.Point(14, 110);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(121, 32);
            this.label9.TabIndex = 9;
            this.label9.Text = "PM10.0:";
            this.label9.Click += new System.EventHandler(this.label9_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.BackColor = System.Drawing.Color.Transparent;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.ForeColor = System.Drawing.Color.White;
            this.label10.Location = new System.Drawing.Point(16, 110);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(121, 32);
            this.label10.TabIndex = 12;
            this.label10.Text = "PM10.0:";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.BackColor = System.Drawing.Color.Transparent;
            this.label11.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.ForeColor = System.Drawing.Color.White;
            this.label11.Location = new System.Drawing.Point(16, 69);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(105, 32);
            this.label11.TabIndex = 11;
            this.label11.Text = "PM2.5:";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.BackColor = System.Drawing.Color.Transparent;
            this.label12.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.ForeColor = System.Drawing.Color.White;
            this.label12.Location = new System.Drawing.Point(16, 28);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(105, 32);
            this.label12.TabIndex = 10;
            this.label12.Text = "PM1.0:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.BackColor = System.Drawing.Color.Transparent;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.ForeColor = System.Drawing.Color.White;
            this.label6.Location = new System.Drawing.Point(865, 48);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(103, 32);
            this.label6.TabIndex = 13;
            this.label6.Text = "SHT31";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // panel1
            // 
            this.panel1.BackgroundImage = global::AQS_DEMO.Properties.Resources.beautiful_color_ui_gradients_backgrounds_bourbon;
            this.panel1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1334, 63);
            this.panel1.TabIndex = 14;
            // 
            // panel2
            // 
            this.panel2.BackgroundImage = global::AQS_DEMO.Properties.Resources.beautiful_color_ui_gradients_backgrounds_bourbon;
            this.panel2.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel2.Controls.Add(this.lblPMS1_10_0);
            this.panel2.Controls.Add(this.lblPMS1_2_5);
            this.panel2.Controls.Add(this.lblPMS1_1_0);
            this.panel2.Controls.Add(this.label7);
            this.panel2.Controls.Add(this.label8);
            this.panel2.Controls.Add(this.label9);
            this.panel2.Location = new System.Drawing.Point(187, 83);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(262, 158);
            this.panel2.TabIndex = 15;
            // 
            // panel3
            // 
            this.panel3.BackgroundImage = global::AQS_DEMO.Properties.Resources.beautiful_color_ui_gradients_backgrounds_bourbon;
            this.panel3.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel3.Controls.Add(this.label16lblPMS2_10_0);
            this.panel3.Controls.Add(this.label12);
            this.panel3.Controls.Add(this.lblPMS2_2_5);
            this.panel3.Controls.Add(this.label11);
            this.panel3.Controls.Add(this.lblPMS2_1_0);
            this.panel3.Controls.Add(this.label10);
            this.panel3.Location = new System.Drawing.Point(487, 83);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(262, 158);
            this.panel3.TabIndex = 16;
            // 
            // panel4
            // 
            this.panel4.BackgroundImage = global::AQS_DEMO.Properties.Resources.beautiful_color_ui_gradients_backgrounds_bourbon;
            this.panel4.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel4.Controls.Add(this.label4);
            this.panel4.Controls.Add(this.lblHum);
            this.panel4.Controls.Add(this.label5);
            this.panel4.Controls.Add(this.lblTemp);
            this.panel4.Location = new System.Drawing.Point(796, 83);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(262, 158);
            this.panel4.TabIndex = 17;
            // 
            // panel5
            // 
            this.panel5.BackColor = System.Drawing.Color.Transparent;
            this.panel5.BackgroundImage = global::AQS_DEMO.Properties.Resources.surfsidescience_min_logo_machine;
            this.panel5.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel5.Location = new System.Drawing.Point(24, 288);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(149, 85);
            this.panel5.TabIndex = 18;
            // 
            // panel6
            // 
            this.panel6.BackColor = System.Drawing.Color.Transparent;
            this.panel6.BackgroundImage = global::AQS_DEMO.Properties.Resources.RESEMBID_Logo_Redesign_FINAL_Full_Colour_Stacked;
            this.panel6.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel6.Location = new System.Drawing.Point(207, 288);
            this.panel6.Name = "panel6";
            this.panel6.Size = new System.Drawing.Size(181, 85);
            this.panel6.TabIndex = 19;
            // 
            // panel7
            // 
            this.panel7.BackColor = System.Drawing.Color.Transparent;
            this.panel7.BackgroundImage = global::AQS_DEMO.Properties.Resources.GFDRR_Primary_Logo_BW_Shade;
            this.panel7.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel7.Location = new System.Drawing.Point(427, 288);
            this.panel7.Name = "panel7";
            this.panel7.Size = new System.Drawing.Size(373, 85);
            this.panel7.TabIndex = 20;
            // 
            // panel8
            // 
            this.panel8.BackColor = System.Drawing.Color.Transparent;
            this.panel8.BackgroundImage = global::AQS_DEMO.Properties.Resources.ef_logo22;
            this.panel8.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel8.Location = new System.Drawing.Point(835, 304);
            this.panel8.Name = "panel8";
            this.panel8.Size = new System.Drawing.Size(190, 66);
            this.panel8.TabIndex = 20;
            // 
            // panel9
            // 
            this.panel9.BackColor = System.Drawing.Color.Transparent;
            this.panel9.BackgroundImage = global::AQS_DEMO.Properties.Resources.Daco_2892920;
            this.panel9.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel9.Location = new System.Drawing.Point(1055, 285);
            this.panel9.Name = "panel9";
            this.panel9.Size = new System.Drawing.Size(211, 85);
            this.panel9.TabIndex = 20;
            this.panel9.Paint += new System.Windows.Forms.PaintEventHandler(this.panel9_Paint);
            // 
            // panel10
            // 
            this.panel10.BackColor = System.Drawing.Color.Transparent;
            this.panel10.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel10.Controls.Add(this.label2);
            this.panel10.Controls.Add(this.panel5);
            this.panel10.Controls.Add(this.panel9);
            this.panel10.Controls.Add(this.panel8);
            this.panel10.Controls.Add(this.panel2);
            this.panel10.Controls.Add(this.panel7);
            this.panel10.Controls.Add(this.panel3);
            this.panel10.Controls.Add(this.panel6);
            this.panel10.Controls.Add(this.panel4);
            this.panel10.Controls.Add(this.label3);
            this.panel10.Controls.Add(this.label6);
            this.panel10.Location = new System.Drawing.Point(30, 105);
            this.panel10.Name = "panel10";
            this.panel10.Size = new System.Drawing.Size(1292, 396);
            this.panel10.TabIndex = 21;
            // 
            // lblPMS1_1_0
            // 
            this.lblPMS1_1_0.AutoSize = true;
            this.lblPMS1_1_0.BackColor = System.Drawing.Color.Transparent;
            this.lblPMS1_1_0.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPMS1_1_0.ForeColor = System.Drawing.Color.White;
            this.lblPMS1_1_0.Location = new System.Drawing.Point(141, 28);
            this.lblPMS1_1_0.Name = "lblPMS1_1_0";
            this.lblPMS1_1_0.Size = new System.Drawing.Size(105, 32);
            this.lblPMS1_1_0.TabIndex = 10;
            this.lblPMS1_1_0.Text = "PM1.0:";
            this.lblPMS1_1_0.Click += new System.EventHandler(this.label13_Click);
            // 
            // lblPMS1_2_5
            // 
            this.lblPMS1_2_5.AutoSize = true;
            this.lblPMS1_2_5.BackColor = System.Drawing.Color.Transparent;
            this.lblPMS1_2_5.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPMS1_2_5.ForeColor = System.Drawing.Color.White;
            this.lblPMS1_2_5.Location = new System.Drawing.Point(141, 69);
            this.lblPMS1_2_5.Name = "lblPMS1_2_5";
            this.lblPMS1_2_5.Size = new System.Drawing.Size(105, 32);
            this.lblPMS1_2_5.TabIndex = 11;
            this.lblPMS1_2_5.Text = "PM1.0:";
            // 
            // lblPMS1_10_0
            // 
            this.lblPMS1_10_0.AutoSize = true;
            this.lblPMS1_10_0.BackColor = System.Drawing.Color.Transparent;
            this.lblPMS1_10_0.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPMS1_10_0.ForeColor = System.Drawing.Color.White;
            this.lblPMS1_10_0.Location = new System.Drawing.Point(141, 110);
            this.lblPMS1_10_0.Name = "lblPMS1_10_0";
            this.lblPMS1_10_0.Size = new System.Drawing.Size(105, 32);
            this.lblPMS1_10_0.TabIndex = 12;
            this.lblPMS1_10_0.Text = "PM1.0:";
            // 
            // label16lblPMS2_10_0
            // 
            this.label16lblPMS2_10_0.AutoSize = true;
            this.label16lblPMS2_10_0.BackColor = System.Drawing.Color.Transparent;
            this.label16lblPMS2_10_0.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label16lblPMS2_10_0.ForeColor = System.Drawing.Color.White;
            this.label16lblPMS2_10_0.Location = new System.Drawing.Point(141, 110);
            this.label16lblPMS2_10_0.Name = "label16lblPMS2_10_0";
            this.label16lblPMS2_10_0.Size = new System.Drawing.Size(105, 32);
            this.label16lblPMS2_10_0.TabIndex = 15;
            this.label16lblPMS2_10_0.Text = "PM1.0:";
            // 
            // lblPMS2_2_5
            // 
            this.lblPMS2_2_5.AutoSize = true;
            this.lblPMS2_2_5.BackColor = System.Drawing.Color.Transparent;
            this.lblPMS2_2_5.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPMS2_2_5.ForeColor = System.Drawing.Color.White;
            this.lblPMS2_2_5.Location = new System.Drawing.Point(141, 69);
            this.lblPMS2_2_5.Name = "lblPMS2_2_5";
            this.lblPMS2_2_5.Size = new System.Drawing.Size(105, 32);
            this.lblPMS2_2_5.TabIndex = 14;
            this.lblPMS2_2_5.Text = "PM1.0:";
            // 
            // lblPMS2_1_0
            // 
            this.lblPMS2_1_0.AutoSize = true;
            this.lblPMS2_1_0.BackColor = System.Drawing.Color.Transparent;
            this.lblPMS2_1_0.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPMS2_1_0.ForeColor = System.Drawing.Color.White;
            this.lblPMS2_1_0.Location = new System.Drawing.Point(141, 28);
            this.lblPMS2_1_0.Name = "lblPMS2_1_0";
            this.lblPMS2_1_0.Size = new System.Drawing.Size(105, 32);
            this.lblPMS2_1_0.TabIndex = 13;
            this.lblPMS2_1_0.Text = "PM1.0:";
            // 
            // lblHum
            // 
            this.lblHum.AutoSize = true;
            this.lblHum.BackColor = System.Drawing.Color.Transparent;
            this.lblHum.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblHum.ForeColor = System.Drawing.Color.White;
            this.lblHum.Location = new System.Drawing.Point(135, 82);
            this.lblHum.Name = "lblHum";
            this.lblHum.Size = new System.Drawing.Size(105, 32);
            this.lblHum.TabIndex = 17;
            this.lblHum.Text = "PM1.0:";
            // 
            // lblTemp
            // 
            this.lblTemp.AutoSize = true;
            this.lblTemp.BackColor = System.Drawing.Color.Transparent;
            this.lblTemp.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTemp.ForeColor = System.Drawing.Color.White;
            this.lblTemp.Location = new System.Drawing.Point(135, 41);
            this.lblTemp.Name = "lblTemp";
            this.lblTemp.Size = new System.Drawing.Size(105, 32);
            this.lblTemp.TabIndex = 16;
            this.lblTemp.Text = "PM1.0:";
            // 
            // AQS_Demo_Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::AQS_DEMO.Properties.Resources._065_Burning_Spring;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(1334, 589);
            this.Controls.Add(this.panel10);
            this.Controls.Add(this.panel1);
            this.DoubleBuffered = true;
            this.Name = "AQS_Demo_Form";
            this.Text = "AQS_Demo";
            this.Load += new System.EventHandler(this.AQS_Demo_Form_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.panel10.ResumeLayout(false);
            this.panel10.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.IO.Ports.SerialPort serialPortAQS;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.Panel panel6;
        private System.Windows.Forms.Panel panel7;
        private System.Windows.Forms.Panel panel8;
        private System.Windows.Forms.Panel panel9;
        private System.Windows.Forms.Panel panel10;
        private System.Windows.Forms.Label lblPMS1_10_0;
        private System.Windows.Forms.Label lblPMS1_2_5;
        private System.Windows.Forms.Label lblPMS1_1_0;
        private System.Windows.Forms.Label label16lblPMS2_10_0;
        private System.Windows.Forms.Label lblPMS2_2_5;
        private System.Windows.Forms.Label lblPMS2_1_0;
        private System.Windows.Forms.Label lblHum;
        private System.Windows.Forms.Label lblTemp;
    }
}

