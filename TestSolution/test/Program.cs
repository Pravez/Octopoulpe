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
			Console.WriteLine("Welcome Octopouple !!");

            Size size = new Size(100, 50);
			TransparentForm form = new TransparentForm(size);
			form.Show();
            //form.CreateBitmapAtRuntime();
            form.move(new Point(30, 450));
            Application.EnableVisualStyles();
			Application.Run(form);

        }
	}
}
