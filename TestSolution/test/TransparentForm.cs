using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace test
{
	public class TransparentForm : CSWinFormLayeredWindow.PerPixelAlphaForm
	{
		private Image image;

        private Point position;

        private int time = 0;
        private Size size;

        //attribute for moving
        private Point goal = new Point(-1, -1); // =-1 if no goal

        public TransparentForm(Size size)
		{
			InitializeComponent();
            Bitmap test = new Bitmap(Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe2.png")), size);
            this.size = size;
            this.SelectBitmap(test);

            Console.WriteLine(Path.GetDirectoryName(Environment.CurrentDirectory));
			//image = Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.poulpy.png"));
			position.X = 0;
			position.Y = 0;

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
            /*MakeTransparent_Example1(e);
            Console.WriteLine(Path.GetDirectoryName(Environment.CurrentDirectory));
            time++;
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
                this.SelectBitmap(new Bitmap(Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe3.png")), size));
            }
            if (time == 100)
            {
                time = 0;
                this.SelectBitmap(new Bitmap(Image.FromStream(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("test.images.magicarpe2.png")), size));
            }

            if (goal.X != -1 && goal.Y != -1)
            {
                if (position.X == 500)
                    this.position.X = 0;
                else if (position.X < goal.X)
                    this.position.X = this.position.X + 1;
                else if (position.X > goal.X)
                    this.position.X = this.position.X - 1;

                if (position.Y == 600)
                    this.position.Y = 0;
                else if (position.Y < goal.Y)
                    this.position.Y = this.position.Y + 1;
                else if (position.Y > goal.X)
                    this.position.Y = this.position.Y - 1;

                if (position.Equals(goal))
                    goal = new Point(-1, -1);
            }
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
            this.Location = this.position;
            this.Update();
            this.Refresh();
        }

        public void move(Point p)
        {
            this.goal = p;
        }

    }
}
