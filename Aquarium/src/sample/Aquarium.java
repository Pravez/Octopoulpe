package sample;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.Collection;

public class Aquarium extends Application {

    int timer = 0;
    ArrayList<Fish> fishes;

    final Pane root = new Pane();

    @Override
    public void start(Stage primaryStage) throws Exception{

        //final URL imageURL = getClass().getResource("magicarpe.png");
        //final Image image = new Image(imageURL.toExternalForm());

        fishes = new ArrayList<Fish>();

        Fish f = new Fish(0, 0, 100, 100, "magicarpe");
        fishes.add(f);

        //root = new Pane();
        root.getChildren().setAll(getAllViews(0));

        //Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Aquarium");
        primaryStage.setScene(new Scene(root, 600, 550));

        primaryStage.show();

        new AnimationTimer() {
            @Override
            public void handle(long now) {
                timer++;

                if (timer%25 == 0) {
                    root.getChildren().setAll(getAllViews(1));
                }
                if (timer%50 == 0)
                    root.getChildren().setAll(getAllViews(0));

                if (timer == 100) {
                    fishes.get(0).setGoal(100, 100);
                }
                if (timer == 200 || timer == 600) {
                    //fishes.get(0).setGoal(0, 0);
                    addFish("magicarpe", 350, 200, 100, 100);
                    //primaryStage.setScene(new Scene(root, 600, 550));
                    //timer=0;
                }
                if (timer == 400 || timer == 800) {
                    if (timer == 800) {
                  //removeFish(fishes.get(1));
                  timer = 0;
                    }
                }

                for (Fish f : fishes) {
                    f.update(timer);
                }
            }
        }.start();

    }

    public Collection<ImageView> getAllViews(int nb) {
        ArrayList<ImageView> res = new ArrayList<ImageView>();
        for(Fish f : fishes) {
            res.add(f.get_View(nb));
        }

        return res;
    }

    public void addFish(String name, int x, int y, int w, int h) {
              Fish f = new Fish(x, y, w, h, name);
              fishes.add(f);
              root.getChildren().setAll(getAllViews(0));
    }

    public void removeFish(Fish f) {
              fishes.remove(f);
    }

    public static void main(String[] args) {
        launch(args);
    }
}
