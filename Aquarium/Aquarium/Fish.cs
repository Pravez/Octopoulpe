using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace Aquarium
{
    class Fish
    {
        private int time = 0;
        public int width;
        public int height;
        
        // Animation representing the fish
        public Texture2D Texture1;
        public Texture2D Texture2;

        // Position of the Fish relative to the upper left side of the screen
        public Vector2 Position;
        public Vector2 Goal;

        // State of the fish
        public bool Displayed;


        // Get the width of the fish
        public int Width
        {
            get { return this.width; }
        }

        // Get the height of the fish
        public int Height
        {
            get { return this.height; }
        }

        public void Initialize(Texture2D texture1, Texture2D texture2, Vector2 position, int width, int height)
        {
            Texture1 = texture1;
            Texture2 = texture2;
            this.width = width;
            this.height = height;
            Position = position;
            Displayed = false;
            Goal = new Vector2(0, 0);
        }

        public void setGoal(Vector2 goal)
        {
            this.Goal = goal;
        }

        public void Update()
        {
            if (Goal.X != -1 && Goal.Y != -1)
            {
                if (Position.X > Goal.X)
                    Position.X = Position.X - 1;
                else if (Position.X < Goal.X)
                    Position.X = Position.X + 1;

                if (Position.Y > Goal.Y)
                    Position.Y = Position.Y - 1;
                else if (Position.Y < Goal.Y)
                    Position.Y = Position.Y + 1;

                if (Position.Equals(Goal))
                    Goal = new Vector2(-1, -1);
            }

            time = (time + 1) % 20;

        }

        public void Draw(SpriteBatch spriteBatch)
        {
            if (time < 10)
                spriteBatch.Draw(Texture1, new Rectangle((int)Position.X, (int)Position.Y, width, height), null, Color.White, 0f, Vector2.Zero, SpriteEffects.None, 0f);
            else
                spriteBatch.Draw(Texture2, new Rectangle((int)Position.X, (int)Position.Y, width, height), null, Color.White, 0f, Vector2.Zero, SpriteEffects.None, 0f);
        }
    }
}
