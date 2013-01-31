using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;

namespace zedboard_ethernet_io_control
{
    class NetworkHelper
    {
        TcpClient serverConnection;
        NetworkStream netStream;

        public bool IsConnected()
        {
            bool connected = false;

            try
            {
                connected = serverConnection.Connected;
            }
            catch
            {
                // nothing to do
            }

            return connected;
        }

        /// <summary>
        /// Connects to the Zedboard at the specified ip address on the specified port
        /// </summary>
        /// <param name="ipaddress">IP Address of the Zedboard</param>
        /// <param name="port">Port to talk to the board on</param>
        /// <returns>True if successful</returns>
        public bool Connect(string ipaddress, int port)
        {
            bool status = false;

            // try and connect to the zedboard
            //try
            //{

                // this is a work around because TcpClient doesn't have a connection timeout.
                // this sets it to 5 seconds, then returns
                /*
                using (serverConnection = new TcpClient())
                {
                    
                    IAsyncResult ar = serverConnection.BeginConnect(ipaddress, port, null, null);
                    System.Threading.WaitHandle wh = ar.AsyncWaitHandle;
                    try
                    {
                        if (!ar.AsyncWaitHandle.WaitOne(TimeSpan.FromSeconds(3), false))
                        {
                            serverConnection.Close();

                            return false;
                            //throw new TimeoutException();
                        }

                        //serverConnection.EndConnect(ar);
                    }
                    finally
                    {
                        wh.Close();
                    }
                } 
                */

                serverConnection = new TcpClient(ipaddress, port);

                //serverConnection.Connect();

                // attach to the network stream
                netStream = serverConnection.GetStream();

                // record our success
                status = serverConnection.Connected;
            //}
            //catch (SocketException)
            //{
            //    MessageBox.Show("Unable to connect to server");
            //}

            

            return status;

        }

        /// <summary>
        /// Write a 4 byte word (uint) to the Zedboard
        /// </summary>
        /// <param name="word">The word to be sent to the zedboard</param>
        /// <returns>True if successful</returns>
        public bool WriteWord(uint word)
        {
            bool success = false;

            try
            {
                // make sure we are connected, and if we are send our word
                if (serverConnection.Connected != true)
                {
                    MessageBox.Show("Please connect to the Zedboard before writing to it.");
                }
                else
                {
                    // get the bytes from the word
                    byte[] bytes = BitConverter.GetBytes(word);
                    // send the word over the interface
                    netStream.Write(bytes, 0, bytes.Length);
                }
            }
            catch (SocketException)
            {
                MessageBox.Show("An error occurred while trying to write to the Zedboard, please check your setup.");
            }

            return success;
        }
        
        /// <summary>
        /// This function simply closes the socket connection
        /// </summary>
        public void Close()
        {
            try
            {
                // close the connection
                serverConnection.Close();
            }
            catch
            {
                // do something here ...
            }
        }
    }

}
