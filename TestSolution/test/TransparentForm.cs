using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace test
{
	public class TransparentForm : Form
	{
		private Image image;
		private int x;
		private int y;

		public TransparentForm()
		{
			InitializeComponent();
			Console.WriteLine(Path.GetDirectoryName(Environment.CurrentDirectory));
			image = Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.poulpy.png"));
			x = 0;
			y = 0;

			Application.Idle += TickWhileIdle;
		
		}

		private void InitializeComponent()
		{
			this.ControlBox = false;
			this.FormBorderStyle = FormBorderStyle.None;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.TopMost = true;
		}

		protected override void OnPaintBackground(PaintEventArgs e)
		{
			e.Graphics.DrawImage(this.image, 0, 0, image.Width, image.Height);
		}

		private void evolve()
		{
			if (x == 500)
				this.x = 0;
			else
				this.x = this.x + 1;


			if (y == 600)
				this.y = 0;
			else
				this.y = this.y + 1;
		}

		public void TickWhileIdle(object sender, EventArgs e)
		{
			evolve();
			this.Location = new Point(x, y);
			this.Update();
			this.Refresh();
		}

	}
}
