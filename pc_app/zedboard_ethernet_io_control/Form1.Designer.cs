namespace zedboard_ethernet_io_control
{
    partial class Form1
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxIPAddress = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBoxJA = new System.Windows.Forms.GroupBox();
            this.checkBoxJA10 = new System.Windows.Forms.CheckBox();
            this.checkBoxJA9 = new System.Windows.Forms.CheckBox();
            this.checkBoxJA8 = new System.Windows.Forms.CheckBox();
            this.checkBoxJA7 = new System.Windows.Forms.CheckBox();
            this.checkBoxJA4 = new System.Windows.Forms.CheckBox();
            this.checkBoxJA3 = new System.Windows.Forms.CheckBox();
            this.checkBoxJA2 = new System.Windows.Forms.CheckBox();
            this.checkBoxJA1 = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.labelConnectionState = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBoxJB = new System.Windows.Forms.GroupBox();
            this.checkBoxJB10 = new System.Windows.Forms.CheckBox();
            this.checkBoxJB9 = new System.Windows.Forms.CheckBox();
            this.checkBoxJB8 = new System.Windows.Forms.CheckBox();
            this.checkBoxJB7 = new System.Windows.Forms.CheckBox();
            this.checkBoxJB4 = new System.Windows.Forms.CheckBox();
            this.checkBoxJB3 = new System.Windows.Forms.CheckBox();
            this.checkBoxJB2 = new System.Windows.Forms.CheckBox();
            this.checkBoxJB1 = new System.Windows.Forms.CheckBox();
            this.groupBoxJC = new System.Windows.Forms.GroupBox();
            this.checkBoxJC10 = new System.Windows.Forms.CheckBox();
            this.checkBoxJC9 = new System.Windows.Forms.CheckBox();
            this.checkBoxJC8 = new System.Windows.Forms.CheckBox();
            this.checkBoxJC7 = new System.Windows.Forms.CheckBox();
            this.checkBoxJC4 = new System.Windows.Forms.CheckBox();
            this.checkBoxJC3 = new System.Windows.Forms.CheckBox();
            this.checkBoxJC2 = new System.Windows.Forms.CheckBox();
            this.checkBoxJC1 = new System.Windows.Forms.CheckBox();
            this.groupBoxJD = new System.Windows.Forms.GroupBox();
            this.checkBoxJD10 = new System.Windows.Forms.CheckBox();
            this.checkBoxJD9 = new System.Windows.Forms.CheckBox();
            this.checkBoxJD8 = new System.Windows.Forms.CheckBox();
            this.checkBoxJD7 = new System.Windows.Forms.CheckBox();
            this.checkBoxJD4 = new System.Windows.Forms.CheckBox();
            this.checkBoxJD3 = new System.Windows.Forms.CheckBox();
            this.checkBoxJD2 = new System.Windows.Forms.CheckBox();
            this.checkBoxJD1 = new System.Windows.Forms.CheckBox();
            this.buttonSendValue = new System.Windows.Forms.Button();
            this.labelStatus = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.buttonDisconnect = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBoxJA.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBoxJB.SuspendLayout();
            this.groupBoxJC.SuspendLayout();
            this.groupBoxJD.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.buttonDisconnect);
            this.groupBox1.Controls.Add(this.buttonConnect);
            this.groupBox1.Controls.Add(this.textBoxPort);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textBoxIPAddress);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(222, 123);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Connection Settings";
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(6, 85);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(96, 23);
            this.buttonConnect.TabIndex = 4;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(79, 50);
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(98, 20);
            this.textBoxPort.TabIndex = 3;
            this.textBoxPort.Text = "7000";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 53);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Port:";
            // 
            // textBoxIPAddress
            // 
            this.textBoxIPAddress.Location = new System.Drawing.Point(79, 23);
            this.textBoxIPAddress.Name = "textBoxIPAddress";
            this.textBoxIPAddress.Size = new System.Drawing.Size(98, 20);
            this.textBoxIPAddress.TabIndex = 1;
            this.textBoxIPAddress.Text = "192.168.1.10";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "IP Address:";
            // 
            // groupBoxJA
            // 
            this.groupBoxJA.Controls.Add(this.checkBoxJA10);
            this.groupBoxJA.Controls.Add(this.checkBoxJA9);
            this.groupBoxJA.Controls.Add(this.checkBoxJA8);
            this.groupBoxJA.Controls.Add(this.checkBoxJA7);
            this.groupBoxJA.Controls.Add(this.checkBoxJA4);
            this.groupBoxJA.Controls.Add(this.checkBoxJA3);
            this.groupBoxJA.Controls.Add(this.checkBoxJA2);
            this.groupBoxJA.Controls.Add(this.checkBoxJA1);
            this.groupBoxJA.Location = new System.Drawing.Point(12, 141);
            this.groupBoxJA.Name = "groupBoxJA";
            this.groupBoxJA.Size = new System.Drawing.Size(86, 235);
            this.groupBoxJA.TabIndex = 1;
            this.groupBoxJA.TabStop = false;
            this.groupBoxJA.Text = "PMOD JA";
            // 
            // checkBoxJA10
            // 
            this.checkBoxJA10.AutoSize = true;
            this.checkBoxJA10.Location = new System.Drawing.Point(15, 201);
            this.checkBoxJA10.Name = "checkBoxJA10";
            this.checkBoxJA10.Size = new System.Drawing.Size(59, 17);
            this.checkBoxJA10.TabIndex = 8;
            this.checkBoxJA10.Text = "PIN 10";
            this.checkBoxJA10.UseVisualStyleBackColor = true;
            // 
            // checkBoxJA9
            // 
            this.checkBoxJA9.AutoSize = true;
            this.checkBoxJA9.Location = new System.Drawing.Point(15, 178);
            this.checkBoxJA9.Name = "checkBoxJA9";
            this.checkBoxJA9.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJA9.TabIndex = 7;
            this.checkBoxJA9.Text = "PIN 9";
            this.checkBoxJA9.UseVisualStyleBackColor = true;
            // 
            // checkBoxJA8
            // 
            this.checkBoxJA8.AutoSize = true;
            this.checkBoxJA8.Location = new System.Drawing.Point(15, 155);
            this.checkBoxJA8.Name = "checkBoxJA8";
            this.checkBoxJA8.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJA8.TabIndex = 6;
            this.checkBoxJA8.Text = "PIN 8";
            this.checkBoxJA8.UseVisualStyleBackColor = true;
            // 
            // checkBoxJA7
            // 
            this.checkBoxJA7.AutoSize = true;
            this.checkBoxJA7.Location = new System.Drawing.Point(15, 132);
            this.checkBoxJA7.Name = "checkBoxJA7";
            this.checkBoxJA7.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJA7.TabIndex = 5;
            this.checkBoxJA7.Text = "PIN 7";
            this.checkBoxJA7.UseVisualStyleBackColor = true;
            // 
            // checkBoxJA4
            // 
            this.checkBoxJA4.AutoSize = true;
            this.checkBoxJA4.Location = new System.Drawing.Point(15, 97);
            this.checkBoxJA4.Name = "checkBoxJA4";
            this.checkBoxJA4.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJA4.TabIndex = 4;
            this.checkBoxJA4.Text = "PIN 4";
            this.checkBoxJA4.UseVisualStyleBackColor = true;
            // 
            // checkBoxJA3
            // 
            this.checkBoxJA3.AutoSize = true;
            this.checkBoxJA3.Location = new System.Drawing.Point(15, 74);
            this.checkBoxJA3.Name = "checkBoxJA3";
            this.checkBoxJA3.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJA3.TabIndex = 3;
            this.checkBoxJA3.Text = "PIN 3";
            this.checkBoxJA3.UseVisualStyleBackColor = true;
            // 
            // checkBoxJA2
            // 
            this.checkBoxJA2.AutoSize = true;
            this.checkBoxJA2.Location = new System.Drawing.Point(15, 51);
            this.checkBoxJA2.Name = "checkBoxJA2";
            this.checkBoxJA2.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJA2.TabIndex = 2;
            this.checkBoxJA2.Text = "PIN 2";
            this.checkBoxJA2.UseVisualStyleBackColor = true;
            // 
            // checkBoxJA1
            // 
            this.checkBoxJA1.AutoSize = true;
            this.checkBoxJA1.Location = new System.Drawing.Point(15, 28);
            this.checkBoxJA1.Name = "checkBoxJA1";
            this.checkBoxJA1.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJA1.TabIndex = 1;
            this.checkBoxJA1.Text = "PIN 1";
            this.checkBoxJA1.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.labelStatus);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.labelConnectionState);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Location = new System.Drawing.Point(240, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(222, 123);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            // 
            // labelConnectionState
            // 
            this.labelConnectionState.AutoSize = true;
            this.labelConnectionState.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelConnectionState.Location = new System.Drawing.Point(25, 29);
            this.labelConnectionState.Name = "labelConnectionState";
            this.labelConnectionState.Size = new System.Drawing.Size(168, 25);
            this.labelConnectionState.TabIndex = 1;
            this.labelConnectionState.Text = "Not Connected";
            this.labelConnectionState.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 16);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(92, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "Connection State:";
            // 
            // groupBoxJB
            // 
            this.groupBoxJB.Controls.Add(this.checkBoxJB10);
            this.groupBoxJB.Controls.Add(this.checkBoxJB9);
            this.groupBoxJB.Controls.Add(this.checkBoxJB8);
            this.groupBoxJB.Controls.Add(this.checkBoxJB7);
            this.groupBoxJB.Controls.Add(this.checkBoxJB4);
            this.groupBoxJB.Controls.Add(this.checkBoxJB3);
            this.groupBoxJB.Controls.Add(this.checkBoxJB2);
            this.groupBoxJB.Controls.Add(this.checkBoxJB1);
            this.groupBoxJB.Location = new System.Drawing.Point(134, 141);
            this.groupBoxJB.Name = "groupBoxJB";
            this.groupBoxJB.Size = new System.Drawing.Size(86, 235);
            this.groupBoxJB.TabIndex = 6;
            this.groupBoxJB.TabStop = false;
            this.groupBoxJB.Text = "PMOD JB";
            // 
            // checkBoxJB10
            // 
            this.checkBoxJB10.AutoSize = true;
            this.checkBoxJB10.Location = new System.Drawing.Point(15, 201);
            this.checkBoxJB10.Name = "checkBoxJB10";
            this.checkBoxJB10.Size = new System.Drawing.Size(59, 17);
            this.checkBoxJB10.TabIndex = 8;
            this.checkBoxJB10.Text = "PIN 10";
            this.checkBoxJB10.UseVisualStyleBackColor = true;
            // 
            // checkBoxJB9
            // 
            this.checkBoxJB9.AutoSize = true;
            this.checkBoxJB9.Location = new System.Drawing.Point(15, 178);
            this.checkBoxJB9.Name = "checkBoxJB9";
            this.checkBoxJB9.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJB9.TabIndex = 7;
            this.checkBoxJB9.Text = "PIN 9";
            this.checkBoxJB9.UseVisualStyleBackColor = true;
            // 
            // checkBoxJB8
            // 
            this.checkBoxJB8.AutoSize = true;
            this.checkBoxJB8.Location = new System.Drawing.Point(15, 155);
            this.checkBoxJB8.Name = "checkBoxJB8";
            this.checkBoxJB8.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJB8.TabIndex = 6;
            this.checkBoxJB8.Text = "PIN 8";
            this.checkBoxJB8.UseVisualStyleBackColor = true;
            // 
            // checkBoxJB7
            // 
            this.checkBoxJB7.AutoSize = true;
            this.checkBoxJB7.Location = new System.Drawing.Point(15, 132);
            this.checkBoxJB7.Name = "checkBoxJB7";
            this.checkBoxJB7.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJB7.TabIndex = 5;
            this.checkBoxJB7.Text = "PIN 7";
            this.checkBoxJB7.UseVisualStyleBackColor = true;
            // 
            // checkBoxJB4
            // 
            this.checkBoxJB4.AutoSize = true;
            this.checkBoxJB4.Location = new System.Drawing.Point(15, 97);
            this.checkBoxJB4.Name = "checkBoxJB4";
            this.checkBoxJB4.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJB4.TabIndex = 4;
            this.checkBoxJB4.Text = "PIN 4";
            this.checkBoxJB4.UseVisualStyleBackColor = true;
            // 
            // checkBoxJB3
            // 
            this.checkBoxJB3.AutoSize = true;
            this.checkBoxJB3.Location = new System.Drawing.Point(15, 74);
            this.checkBoxJB3.Name = "checkBoxJB3";
            this.checkBoxJB3.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJB3.TabIndex = 3;
            this.checkBoxJB3.Text = "PIN 3";
            this.checkBoxJB3.UseVisualStyleBackColor = true;
            // 
            // checkBoxJB2
            // 
            this.checkBoxJB2.AutoSize = true;
            this.checkBoxJB2.Location = new System.Drawing.Point(15, 51);
            this.checkBoxJB2.Name = "checkBoxJB2";
            this.checkBoxJB2.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJB2.TabIndex = 2;
            this.checkBoxJB2.Text = "PIN 2";
            this.checkBoxJB2.UseVisualStyleBackColor = true;
            // 
            // checkBoxJB1
            // 
            this.checkBoxJB1.AutoSize = true;
            this.checkBoxJB1.Location = new System.Drawing.Point(15, 28);
            this.checkBoxJB1.Name = "checkBoxJB1";
            this.checkBoxJB1.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJB1.TabIndex = 1;
            this.checkBoxJB1.Text = "PIN 1";
            this.checkBoxJB1.UseVisualStyleBackColor = true;
            // 
            // groupBoxJC
            // 
            this.groupBoxJC.Controls.Add(this.checkBoxJC10);
            this.groupBoxJC.Controls.Add(this.checkBoxJC9);
            this.groupBoxJC.Controls.Add(this.checkBoxJC8);
            this.groupBoxJC.Controls.Add(this.checkBoxJC7);
            this.groupBoxJC.Controls.Add(this.checkBoxJC4);
            this.groupBoxJC.Controls.Add(this.checkBoxJC3);
            this.groupBoxJC.Controls.Add(this.checkBoxJC2);
            this.groupBoxJC.Controls.Add(this.checkBoxJC1);
            this.groupBoxJC.Location = new System.Drawing.Point(255, 141);
            this.groupBoxJC.Name = "groupBoxJC";
            this.groupBoxJC.Size = new System.Drawing.Size(86, 235);
            this.groupBoxJC.TabIndex = 9;
            this.groupBoxJC.TabStop = false;
            this.groupBoxJC.Text = "PMOD JC";
            // 
            // checkBoxJC10
            // 
            this.checkBoxJC10.AutoSize = true;
            this.checkBoxJC10.Location = new System.Drawing.Point(15, 201);
            this.checkBoxJC10.Name = "checkBoxJC10";
            this.checkBoxJC10.Size = new System.Drawing.Size(59, 17);
            this.checkBoxJC10.TabIndex = 8;
            this.checkBoxJC10.Text = "PIN 10";
            this.checkBoxJC10.UseVisualStyleBackColor = true;
            // 
            // checkBoxJC9
            // 
            this.checkBoxJC9.AutoSize = true;
            this.checkBoxJC9.Location = new System.Drawing.Point(15, 178);
            this.checkBoxJC9.Name = "checkBoxJC9";
            this.checkBoxJC9.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJC9.TabIndex = 7;
            this.checkBoxJC9.Text = "PIN 9";
            this.checkBoxJC9.UseVisualStyleBackColor = true;
            // 
            // checkBoxJC8
            // 
            this.checkBoxJC8.AutoSize = true;
            this.checkBoxJC8.Location = new System.Drawing.Point(15, 155);
            this.checkBoxJC8.Name = "checkBoxJC8";
            this.checkBoxJC8.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJC8.TabIndex = 6;
            this.checkBoxJC8.Text = "PIN 8";
            this.checkBoxJC8.UseVisualStyleBackColor = true;
            // 
            // checkBoxJC7
            // 
            this.checkBoxJC7.AutoSize = true;
            this.checkBoxJC7.Location = new System.Drawing.Point(15, 132);
            this.checkBoxJC7.Name = "checkBoxJC7";
            this.checkBoxJC7.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJC7.TabIndex = 5;
            this.checkBoxJC7.Text = "PIN 7";
            this.checkBoxJC7.UseVisualStyleBackColor = true;
            // 
            // checkBoxJC4
            // 
            this.checkBoxJC4.AutoSize = true;
            this.checkBoxJC4.Location = new System.Drawing.Point(15, 97);
            this.checkBoxJC4.Name = "checkBoxJC4";
            this.checkBoxJC4.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJC4.TabIndex = 4;
            this.checkBoxJC4.Text = "PIN 4";
            this.checkBoxJC4.UseVisualStyleBackColor = true;
            // 
            // checkBoxJC3
            // 
            this.checkBoxJC3.AutoSize = true;
            this.checkBoxJC3.Location = new System.Drawing.Point(15, 74);
            this.checkBoxJC3.Name = "checkBoxJC3";
            this.checkBoxJC3.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJC3.TabIndex = 3;
            this.checkBoxJC3.Text = "PIN 3";
            this.checkBoxJC3.UseVisualStyleBackColor = true;
            // 
            // checkBoxJC2
            // 
            this.checkBoxJC2.AutoSize = true;
            this.checkBoxJC2.Location = new System.Drawing.Point(15, 51);
            this.checkBoxJC2.Name = "checkBoxJC2";
            this.checkBoxJC2.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJC2.TabIndex = 2;
            this.checkBoxJC2.Text = "PIN 2";
            this.checkBoxJC2.UseVisualStyleBackColor = true;
            // 
            // checkBoxJC1
            // 
            this.checkBoxJC1.AutoSize = true;
            this.checkBoxJC1.Location = new System.Drawing.Point(15, 28);
            this.checkBoxJC1.Name = "checkBoxJC1";
            this.checkBoxJC1.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJC1.TabIndex = 1;
            this.checkBoxJC1.Text = "PIN 1";
            this.checkBoxJC1.UseVisualStyleBackColor = true;
            // 
            // groupBoxJD
            // 
            this.groupBoxJD.Controls.Add(this.checkBoxJD10);
            this.groupBoxJD.Controls.Add(this.checkBoxJD9);
            this.groupBoxJD.Controls.Add(this.checkBoxJD8);
            this.groupBoxJD.Controls.Add(this.checkBoxJD7);
            this.groupBoxJD.Controls.Add(this.checkBoxJD4);
            this.groupBoxJD.Controls.Add(this.checkBoxJD3);
            this.groupBoxJD.Controls.Add(this.checkBoxJD2);
            this.groupBoxJD.Controls.Add(this.checkBoxJD1);
            this.groupBoxJD.Location = new System.Drawing.Point(375, 141);
            this.groupBoxJD.Name = "groupBoxJD";
            this.groupBoxJD.Size = new System.Drawing.Size(86, 235);
            this.groupBoxJD.TabIndex = 9;
            this.groupBoxJD.TabStop = false;
            this.groupBoxJD.Text = "PMOD JD";
            // 
            // checkBoxJD10
            // 
            this.checkBoxJD10.AutoSize = true;
            this.checkBoxJD10.Location = new System.Drawing.Point(15, 201);
            this.checkBoxJD10.Name = "checkBoxJD10";
            this.checkBoxJD10.Size = new System.Drawing.Size(59, 17);
            this.checkBoxJD10.TabIndex = 8;
            this.checkBoxJD10.Text = "PIN 10";
            this.checkBoxJD10.UseVisualStyleBackColor = true;
            // 
            // checkBoxJD9
            // 
            this.checkBoxJD9.AutoSize = true;
            this.checkBoxJD9.Location = new System.Drawing.Point(15, 178);
            this.checkBoxJD9.Name = "checkBoxJD9";
            this.checkBoxJD9.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJD9.TabIndex = 7;
            this.checkBoxJD9.Text = "PIN 9";
            this.checkBoxJD9.UseVisualStyleBackColor = true;
            // 
            // checkBoxJD8
            // 
            this.checkBoxJD8.AutoSize = true;
            this.checkBoxJD8.Location = new System.Drawing.Point(15, 155);
            this.checkBoxJD8.Name = "checkBoxJD8";
            this.checkBoxJD8.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJD8.TabIndex = 6;
            this.checkBoxJD8.Text = "PIN 8";
            this.checkBoxJD8.UseVisualStyleBackColor = true;
            // 
            // checkBoxJD7
            // 
            this.checkBoxJD7.AutoSize = true;
            this.checkBoxJD7.Location = new System.Drawing.Point(15, 132);
            this.checkBoxJD7.Name = "checkBoxJD7";
            this.checkBoxJD7.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJD7.TabIndex = 5;
            this.checkBoxJD7.Text = "PIN 7";
            this.checkBoxJD7.UseVisualStyleBackColor = true;
            // 
            // checkBoxJD4
            // 
            this.checkBoxJD4.AutoSize = true;
            this.checkBoxJD4.Location = new System.Drawing.Point(15, 97);
            this.checkBoxJD4.Name = "checkBoxJD4";
            this.checkBoxJD4.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJD4.TabIndex = 4;
            this.checkBoxJD4.Text = "PIN 4";
            this.checkBoxJD4.UseVisualStyleBackColor = true;
            // 
            // checkBoxJD3
            // 
            this.checkBoxJD3.AutoSize = true;
            this.checkBoxJD3.Location = new System.Drawing.Point(15, 74);
            this.checkBoxJD3.Name = "checkBoxJD3";
            this.checkBoxJD3.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJD3.TabIndex = 3;
            this.checkBoxJD3.Text = "PIN 3";
            this.checkBoxJD3.UseVisualStyleBackColor = true;
            // 
            // checkBoxJD2
            // 
            this.checkBoxJD2.AutoSize = true;
            this.checkBoxJD2.Location = new System.Drawing.Point(15, 51);
            this.checkBoxJD2.Name = "checkBoxJD2";
            this.checkBoxJD2.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJD2.TabIndex = 2;
            this.checkBoxJD2.Text = "PIN 2";
            this.checkBoxJD2.UseVisualStyleBackColor = true;
            // 
            // checkBoxJD1
            // 
            this.checkBoxJD1.AutoSize = true;
            this.checkBoxJD1.Location = new System.Drawing.Point(15, 28);
            this.checkBoxJD1.Name = "checkBoxJD1";
            this.checkBoxJD1.Size = new System.Drawing.Size(53, 17);
            this.checkBoxJD1.TabIndex = 1;
            this.checkBoxJD1.Text = "PIN 1";
            this.checkBoxJD1.UseVisualStyleBackColor = true;
            // 
            // buttonSendValue
            // 
            this.buttonSendValue.Location = new System.Drawing.Point(12, 386);
            this.buttonSendValue.Name = "buttonSendValue";
            this.buttonSendValue.Size = new System.Drawing.Size(450, 27);
            this.buttonSendValue.TabIndex = 10;
            this.buttonSendValue.Text = "Send Value to Zedboard";
            this.buttonSendValue.UseVisualStyleBackColor = true;
            this.buttonSendValue.Click += new System.EventHandler(this.buttonSendValue_Click);
            // 
            // labelStatus
            // 
            this.labelStatus.AutoSize = true;
            this.labelStatus.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelStatus.Location = new System.Drawing.Point(25, 86);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(50, 25);
            this.labelStatus.TabIndex = 3;
            this.labelStatus.Text = "Idle";
            this.labelStatus.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 73);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(40, 13);
            this.label5.TabIndex = 2;
            this.label5.Text = "Status:";
            // 
            // buttonDisconnect
            // 
            this.buttonDisconnect.Location = new System.Drawing.Point(117, 85);
            this.buttonDisconnect.Name = "buttonDisconnect";
            this.buttonDisconnect.Size = new System.Drawing.Size(96, 23);
            this.buttonDisconnect.TabIndex = 5;
            this.buttonDisconnect.Text = "Disconnect";
            this.buttonDisconnect.UseVisualStyleBackColor = true;
            this.buttonDisconnect.Click += new System.EventHandler(this.buttonDisconnect_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(473, 426);
            this.Controls.Add(this.buttonSendValue);
            this.Controls.Add(this.groupBoxJD);
            this.Controls.Add(this.groupBoxJC);
            this.Controls.Add(this.groupBoxJB);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBoxJA);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Zedboard Ethernet IO Control Software";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBoxJA.ResumeLayout(false);
            this.groupBoxJA.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBoxJB.ResumeLayout(false);
            this.groupBoxJB.PerformLayout();
            this.groupBoxJC.ResumeLayout(false);
            this.groupBoxJC.PerformLayout();
            this.groupBoxJD.ResumeLayout(false);
            this.groupBoxJD.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxIPAddress;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBoxJA;
        private System.Windows.Forms.CheckBox checkBoxJA1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label labelConnectionState;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox checkBoxJA10;
        private System.Windows.Forms.CheckBox checkBoxJA9;
        private System.Windows.Forms.CheckBox checkBoxJA8;
        private System.Windows.Forms.CheckBox checkBoxJA7;
        private System.Windows.Forms.CheckBox checkBoxJA4;
        private System.Windows.Forms.CheckBox checkBoxJA3;
        private System.Windows.Forms.CheckBox checkBoxJA2;
        private System.Windows.Forms.GroupBox groupBoxJB;
        private System.Windows.Forms.CheckBox checkBoxJB10;
        private System.Windows.Forms.CheckBox checkBoxJB9;
        private System.Windows.Forms.CheckBox checkBoxJB8;
        private System.Windows.Forms.CheckBox checkBoxJB7;
        private System.Windows.Forms.CheckBox checkBoxJB4;
        private System.Windows.Forms.CheckBox checkBoxJB3;
        private System.Windows.Forms.CheckBox checkBoxJB2;
        private System.Windows.Forms.CheckBox checkBoxJB1;
        private System.Windows.Forms.GroupBox groupBoxJC;
        private System.Windows.Forms.CheckBox checkBoxJC10;
        private System.Windows.Forms.CheckBox checkBoxJC9;
        private System.Windows.Forms.CheckBox checkBoxJC8;
        private System.Windows.Forms.CheckBox checkBoxJC7;
        private System.Windows.Forms.CheckBox checkBoxJC4;
        private System.Windows.Forms.CheckBox checkBoxJC3;
        private System.Windows.Forms.CheckBox checkBoxJC2;
        private System.Windows.Forms.CheckBox checkBoxJC1;
        private System.Windows.Forms.GroupBox groupBoxJD;
        private System.Windows.Forms.CheckBox checkBoxJD10;
        private System.Windows.Forms.CheckBox checkBoxJD9;
        private System.Windows.Forms.CheckBox checkBoxJD8;
        private System.Windows.Forms.CheckBox checkBoxJD7;
        private System.Windows.Forms.CheckBox checkBoxJD4;
        private System.Windows.Forms.CheckBox checkBoxJD3;
        private System.Windows.Forms.CheckBox checkBoxJD2;
        private System.Windows.Forms.CheckBox checkBoxJD1;
        private System.Windows.Forms.Button buttonSendValue;
        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button buttonDisconnect;
    }
}

