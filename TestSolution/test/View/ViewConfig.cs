using System;
using System.Net;

namespace test
{
	// The ViewConfig class provides the informations given by the affichage.cfg file
	public class ViewConfig
	{
		// Attributes
		private IPAddress controllerAddress; // The IP address of the controller
		private String id; // The identifier of the view
		private int controllerPort; // The TCP port number listened by the controller
		private int displayTimeoutValue; // The default value for transmitting a ping
		private String imgDirectory; // The directory where are the fish pictures

		// The constructor which uses the file "affichage.cfg" to collect the informations
		public ViewConfig ()
		{
			// TO DO
			// Choose default values

			// Opening the file
			System.IO.StreamReader file = new System.IO.StreamReader("/home/lparant/S8-projects/Octopoulpe/TestSolution/test/View/affichage.cfg");

			// Reading the file line by line
			String line;
			String[] infos;
			String[] separator = new string[] {" = "};
			while((line = file.ReadLine()) != null)
			{
				// Ignoring the comment, empty and header lines
				if (!(line.StartsWith ("<") || line.StartsWith ("#") || line.Length == 0))
				{
					infos = line.Split (separator, StringSplitOptions.None);

					// Filling the attributes
					if (infos [0] == "controller-adress")
						controllerAddress = IPAddress.Parse(infos [1]);
					if (infos [0] == "id")
						id = infos [1];
					if (infos [0] == "controller-port")
						controllerPort = Int32.Parse(infos [1]);
					if (infos [0] == "display-timeout-value")
						displayTimeoutValue = Int32.Parse(infos [1]);
					if (infos [0] == "resources")
						imgDirectory = infos [1];
				}
			}

			// Closing the file
			file.Close();
		}

		// Getters
		// Controller IP address getter
		public IPAddress getControllerAddress()
		{
			return controllerAddress;
		}

		// Identifier getter
		public String getIdentifier()
		{
			return id;
		}

		// TCP port number getter
		public int getControllerPort()
		{
			return controllerPort;
		}

		// Default value for ping getter
		public int getDisplayTimeoutValue()
		{
			return displayTimeoutValue;
		}

		// Pictures directory getter
		public String getImgDirectory()
		{
			return imgDirectory;
		}
	}
}

