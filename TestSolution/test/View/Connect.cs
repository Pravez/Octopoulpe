﻿using System;
using System.Net;
using System.Net.Sockets;	

namespace test
{	
	// The Connect class connects a client to a server at the address and port specified in the affichage.cfg file
	public class Connect
	{
		public Socket sock;
		public ViewConfig vc;

		// Opens a connection with the server specified in the affichage.cfg file
		public Connect ()
		{
			sock = new Socket (
				AddressFamily.InterNetwork,
				SocketType.Stream,
				ProtocolType.Tcp);

			vc = new ViewConfig ();

			IPAddress addr = vc.getControllerAddress();
			int port = vc.getControllerPort();

			IPEndPoint iep = new IPEndPoint (addr, port);
			sock.Connect (iep);
		}

		// Shuts down the connection
		public void Disconnect ()
		{
			sock.Shutdown (SocketShutdown.Send);
			sock.Close ();
		}

		// Sends data to the server
		public void PushValue (byte[] buffer)
		{
			sock.Send (buffer);
		}

		// Receives data from the server
		public byte[] PullValue ()
		{
			byte[] buffer = new byte[512];
			sock.Receive (buffer);
			return buffer;
		}
	}
}
