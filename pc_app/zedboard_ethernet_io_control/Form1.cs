using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace zedboard_ethernet_io_control
{
    public partial class Form1 : Form
    {
        NetworkHelper netHelper;

        public Form1()
        {
            InitializeComponent();

            netHelper = new NetworkHelper();

            this.groupBoxJA.Enabled = false;
            this.groupBoxJB.Enabled = false;
            this.groupBoxJC.Enabled = false;
            this.groupBoxJD.Enabled = false;

            this.labelConnectionState.Text = "Not Connected";
            this.labelStatus.Text = "Idle";
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            string ipAddress = this.textBoxIPAddress.Text;
            int port = Convert.ToInt32(this.textBoxPort.Text);

            if (netHelper.Connect(ipAddress, port))
            {
                MessageBox.Show("Connected!");

                this.labelConnectionState.Text = "Connected";
                this.labelStatus.Text = "Idle";

                this.groupBoxJA.Enabled = true;
                this.groupBoxJB.Enabled = true;
                this.groupBoxJC.Enabled = true;
                this.groupBoxJD.Enabled = true;
            }
            else
            {
                MessageBox.Show("Could not connect to Zedboard.  Check your setup and try again.");
            }
            
        }

        private uint getValue()
        {
            uint value = 0;

            // yes, this is ugly ... get over it.

            // PMOD JA
            //
            // JA 1-4
            if (checkBoxJA1.Checked)
                value += (1 << 0);
            if (checkBoxJA2.Checked)
                value += (1 << 1);
            if (checkBoxJA3.Checked)
                value += (1 << 2);
            if (checkBoxJA4.Checked)
                value += (1 << 3);
            // JA 7-10
            if (checkBoxJA7.Checked)
                value += (1 << 4);
            if (checkBoxJA8.Checked)
                value += (1 << 5);
            if (checkBoxJA9.Checked)
                value += (1 << 6);
            if (checkBoxJA10.Checked)
                value += (1 << 7);

            // PMOD JB
            //
            // JB 1-4
            if (checkBoxJB1.Checked)
                value += (1 << 8);
            if (checkBoxJB2.Checked)
                value += (1 << 9);
            if (checkBoxJB3.Checked)
                value += (1 << 10);
            if (checkBoxJB4.Checked)
                value += (1 << 11);
            // JB 7-10
            if (checkBoxJB7.Checked)
                value += (1 << 12);
            if (checkBoxJB8.Checked)
                value += (1 << 13);
            if (checkBoxJB9.Checked)
                value += (1 << 14);
            if (checkBoxJB10.Checked)
                value += (1 << 15);

            // PMOD JC
            //
            // JC 1-4
            if (checkBoxJC1.Checked)
                value += (1 << 16);
            if (checkBoxJC2.Checked)
                value += (1 << 17);
            if (checkBoxJC3.Checked)
                value += (1 << 18);
            if (checkBoxJC4.Checked)
                value += (1 << 19);
            // JC 7-10
            if (checkBoxJC7.Checked)
                value += (1 << 20);
            if (checkBoxJC8.Checked)
                value += (1 << 21);
            if (checkBoxJC9.Checked)
                value += (1 << 22);
            if (checkBoxJC10.Checked)
                value += (1 << 23);

            // PMOD JD
            //
            // JD 1-4
            if (checkBoxJD1.Checked)
                value += (1 << 24);
            if (checkBoxJD2.Checked)
                value += (1 << 25);
            if (checkBoxJD3.Checked)
                value += (1 << 26);
            if (checkBoxJD4.Checked)
                value += (1 << 27);
            // JD 7-10
            if (checkBoxJD7.Checked)
                value += (1 << 28);
            if (checkBoxJD8.Checked)
                value += (1 << 29);
            if (checkBoxJD9.Checked)
                value += (1 << 30);
            if (checkBoxJD10.Checked)
                value += (uint)(Math.Pow(2,31));

            return value;
        }

        private void buttonDisconnect_Click(object sender, EventArgs e)
        {
            if (netHelper.IsConnected())
            {
                netHelper.Close();

                this.groupBoxJA.Enabled = false;
                this.groupBoxJB.Enabled = false;
                this.groupBoxJC.Enabled = false;
                this.groupBoxJD.Enabled = false;

                this.labelConnectionState.Text = "Not Connected";
                this.labelStatus.Text = "Idle";
            }
        }

        private void buttonSendValue_Click(object sender, EventArgs e)
        {
            if (netHelper.IsConnected())
            {
                netHelper.WriteWord(this.getValue());

                MessageBox.Show("Word Sent!");
            }
            else
            {
                MessageBox.Show("You are not connected to the Zedboard.  You must be connected to send data.");

                this.groupBoxJA.Enabled = false;
                this.groupBoxJB.Enabled = false;
                this.groupBoxJC.Enabled = false;
                this.groupBoxJD.Enabled = false;

                this.labelConnectionState.Text = "Not Connected";
                this.labelStatus.Text = "Idle";
            }
        }

    }
}
