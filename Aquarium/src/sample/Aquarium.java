package sample;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;

public class Aquarium extends Application {

    int timer = 0;
    ArrayList<Fish> fishes;

    @Override
    public void start(Stage primaryStage) throws Exception{

        //final URL imageURL = getClass().getResource("magicarpe.png");
        //final Image image = new Image(imageURL.toExternalForm());

        fishes = new ArrayList<Fish>();

        Fish f = new Fish(0, 0, 100, 100, "magicarpe");
        fishes.add(f);
        /*final ImageView imageView = new ImageView(image);
        imageView.setFitHeight(100);
        imageView.setFitWidth(100);*/

        final Pane root = new Pane();
        root.getChildren().setAll(fishes.get(0).get_View(0));

        //Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Aquarium");
        primaryStage.setScene(new Scene(root, 600, 550));

        primaryStage.show();

        new AnimationTimer() {
            @Override
            public void handle(long now) {
                timer++;

                if (timer%25 == 0) {
                    root.getChildren().setAll(fishes.get(0).get_View(1));
                }
                if (timer%50 == 0)
                    root.getChildren().setAll(fishes.get(0).get_View(0));

                if (timer == 100) {
                    fishes.get(0).setGoal(100, 100);
                }
                if (timer == 200) {
                    //fishes.get(0).setGoal(0, 0);
                    timer=0;
                }

                for (Fish f : fishes) {
                    f.update(timer);
                }
            }
        }.start();

    }


    public static void main(String[] args) {
        launch(args);
    }
}
