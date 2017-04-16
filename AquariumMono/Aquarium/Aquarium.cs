using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Collections.Generic;

namespace Aquarium
{
    /// <summary>
    /// This is the main type for your game.
    /// </summary>
    public class Aquarium : Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        List<Fish> fishes;

        public Aquarium()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // Initialize the aquarium class
            fishes = new List<Fish>();

            AddFish("magicarpe2", new Vector2(0,0), new Vector2(100, 100));
            AddFish("magicarpe2", new Vector2(50, 50), new Vector2(100, 100));
            //fishes.Add(new Fish());
            //fishes.Add(new Fish());

            base.Initialize();

            fishes[0].setGoal(new Vector2(400, 350));
            fishes[1].setGoal(new Vector2(465, 165));
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);
            // Load the player resources

            Vector2 playerPosition = new Vector2(25, 25);

           /* foreach (Fish f in fishes)
            {
                f.Initialize(Content.Load<Texture2D>("magicarpe2"), Content.Load<Texture2D>("magicarpe3"), playerPosition, 100, 100);
            }*/
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// game-specific content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            foreach (Fish f in fishes)
            {
                f.Update();
            }
            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            // Start drawing
            spriteBatch.Begin();

            spriteBatch.Draw(Content.Load<Texture2D>("bg"), new Rectangle(0, 0, graphics.PreferredBackBufferWidth, graphics.PreferredBackBufferHeight), null, Color.White, 0f, Vector2.Zero, SpriteEffects.None, 0f);
            // Draw the fishes
            foreach (Fish f in fishes)
            {
                f.Draw(spriteBatch);
            }

            // Stop drawing
            spriteBatch.End();

            base.Draw(gameTime);
        }

        protected void AddFish(string name, Vector2 position, Vector2 size)
        {
            fishes.Add(new Fish());
            fishes[fishes.Count-1].Initialize(Content.Load<Texture2D>(name), Content.Load<Texture2D>(name + "_"), position, (int)size.X, (int)size.Y);
        }

        protected void DeleteFish(string name)
        {

        }
    }
}
