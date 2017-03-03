using System;
using System.Net.Sockets;

namespace test
{
	public class Connect
	{
		public Socket sock;

		public Connect ()
		{
			sock = new Socket (
				AddressFamily.InterNetwork,
				SocketType.Stream,
				ProtocolType.Tcp);
		}
	}
}

