using System;
using System.Drawing;
using System.Windows;
using System.Windows.Forms;


namespace test
{
	class MainClass
	{

		public static void Main(string[] args)
		{
			Console.WriteLine("Hello World!");
            
			TransparentForm form = new TransparentForm();
			form.Show();
            //form.CreateBitmapAtRuntime();
            Application.EnableVisualStyles();
			Application.Run(form);

        }
	}
}
