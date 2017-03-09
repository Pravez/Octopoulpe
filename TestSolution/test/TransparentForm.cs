using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace test
{
	public class TransparentForm : CSWinFormLayeredWindow.PerPixelAlphaForm
	{
		private Image image;
		private int x;
		private int y;
        private int time = 0;
		public TransparentForm()
		{
			InitializeComponent();
            Bitmap test = new Bitmap(Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe2.png")));
           
            this.SelectBitmap(test);

            Console.WriteLine(Path.GetDirectoryName(Environment.CurrentDirectory));
			//image = Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.poulpy.png"));
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
            //this.BackColor = System.Drawing.Color.Transparent;
		}

		protected override void OnPaintBackground(PaintEventArgs e)
        {
            //MakeTransparent_Example1(e);
            //Console.WriteLine(Path.GetDirectoryName(Environment.CurrentDirectory));
            /*time++;
            this.Refresh();
            if (time<100)
            {
                Bitmap test = new Bitmap(Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe3.png")));
                test.MakeTransparent(Color.Transparent);
                //pictureBox1.Image = (Image)test;
                image = Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe3.png"));
                e.Graphics.DrawImage(this.image, 0, 0, image.Width, image.Height);
            }
            else if (time <200)
            {
                time = 0;
                image = Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe2.png"));
                e.Graphics.DrawImage(this.image, 0, 0, image.Width, image.Height);
                Bitmap test = new Bitmap(Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe2.png")));
                test.MakeTransparent(Color.Transparent);
                //pictureBox1.Image = (Image)test;
            }
            this.Refresh();*/

        }

		private void evolve()
		{
            time = time +1;
            if (time == 50)
            {                
                this.SelectBitmap(new Bitmap(Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe3.png"))));
            }
            if (time == 100)
            {
                time = 0;
                this.SelectBitmap(new Bitmap(Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe2.png"))));
            }


            if (x == 500)
				this.x = 0;
			else
				this.x = this.x + 1;


			if (y == 600)
				this.y = 0;
			else
				this.y = this.y + 1;
		}

        System.Diagnostics.Stopwatch stopWatch = System.Diagnostics.Stopwatch.StartNew();

        readonly TimeSpan TargetElapsedTime = TimeSpan.FromTicks(TimeSpan.TicksPerSecond / 60);
        readonly TimeSpan MaxElapsedTime = TimeSpan.FromTicks(TimeSpan.TicksPerSecond / 10);

        TimeSpan accumulatedTime;
        TimeSpan lastTime;


        void Tick(object sender, EventArgs e)
        {
            TimeSpan currentTime = stopWatch.Elapsed;
            TimeSpan elapsedTime = currentTime - lastTime;
            lastTime = currentTime;

            if (elapsedTime > MaxElapsedTime)
            {
                elapsedTime = MaxElapsedTime;
            }

            accumulatedTime += elapsedTime;

            bool updated = false;

            while (accumulatedTime >= TargetElapsedTime)
            {
                Update();

                accumulatedTime -= TargetElapsedTime;
                updated = true;
            }

            if (updated)
            {
                Invalidate();
            }
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
