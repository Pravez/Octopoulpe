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

public class Aquarium extends Application {

    int timer = 0;

    @Override
    public void start(Stage primaryStage) throws Exception{

        final URL imageURL = getClass().getResource("magicarpe.png");
        final Image image = new Image(imageURL.toExternalForm());

        final ImageView imageView = new ImageView(image);
        imageView.setFitHeight(100);
        imageView.setFitWidth(100);

        final Pane rooti = new Pane();
        rooti.getChildren().setAll(imageView);

        //Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Hello World");
        primaryStage.setScene(new Scene(rooti, 300, 275));

        primaryStage.show();

        new AnimationTimer() {
            @Override
            public void handle(long now) {
                timer++;
                if (timer == 100) {
                    imageView.setX(100);
                    imageView.setY(100);
                }
                if (timer == 200) {
                    imageView.setX(0);
                    imageView.setY(0);
                    timer=0;
                }
            }
                /*final double width = 0.5 * primaryStage.getWidth();
                final double height = 0.5 * primaryStage.getHeight();
                final double radius = Math.sqrt(2) * Math.max(width, height);
                for (int i=0; i<STAR_COUNT; i++) {
                    final Node node = nodes[i];
                    final double angle = angles[i];
                    final long t = (now - start[i]) % 2000000000;
                    final double d = t * radius / 2000000000.0;
                    node.setTranslateX(Math.cos(angle) * d + width);
                    node.setTranslateY(Math.sin(angle) * d + height);
                }*/
        }.start();

    }


    public static void main(String[] args) {
        launch(args);
    }
}
