package sample;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

import java.net.URL;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Timer;
import java.util.TimerTask;

public class Aquarium extends Application {

    private int timer = 0;
    private long previousTime = 0;

    private int konamiCode = 0;
    private ArrayList<Fish> fishes;
    private ImageView background;

    private int width = 630;
    private int height = 300;

    private final Pane aquarium = new Pane();

    @Override
    public void start(Stage primaryStage) throws Exception{

        fishes = new ArrayList<Fish>();

        //initialisation of the aquarium
        final URL url = getClass().getResource("Images/bg.png");
        final Image bg = new Image(url.toExternalForm());
        background =  new ImageView(bg);
        background.setFitHeight(height);
        background.setFitWidth(width);
        aquarium.getChildren().setAll(background);

        aquarium.getChildren().addAll(getAllViews(0));
        primaryStage.setTitle("Aquarium");
        primaryStage.setScene(new Scene(aquarium, width, height));
        primaryStage.show();

        //Creation of the console window
        Console console = new Console(this, 400, 400);
        console.show();

        //KONAMI CODE
        initKonami();


        new AnimationTimer() {
            @Override
            public void handle(long now) {
                System.out.println("DEBUG : now = " + (now/1000000));
                timer++;

                if (timer%25 == 0) {
                    aquarium.getChildren().remove(1, aquarium.getChildren().size()); //remove all except background
                    aquarium.getChildren().addAll(getAllViews(1));
                }
                if (timer%50 == 0) {
                    aquarium.getChildren().remove(1, aquarium.getChildren().size() );
                    aquarium.getChildren().addAll(getAllViews(0));
                }

                if (timer == 400 || timer == 800) {
                    if (timer == 800) {
                        timer = 0;
                    }
                }
                if (previousTime != 0) {
                    System.out.println("DEBUG : previousTime = " +  previousTime);
                    System.out.println("DEBUG : elapsed = " + (now/1000000 - previousTime));
                    for (Fish f : fishes) {
                        f.update((now/1000000) - previousTime);
                    }
                }

                previousTime = now/1000000;
            }
        }.start();
    }

    private void initKonami() {
        aquarium.requestFocus();
        aquarium.setOnKeyReleased(new EventHandler<KeyEvent>(){
            public void handle(KeyEvent ke){
                if (ke.getCode() == KeyCode.UP && (konamiCode == 0 || konamiCode == 1)) {
                    konamiCode++;
                }
                else if (ke.getCode() == KeyCode.DOWN&& (konamiCode == 2 || konamiCode == 3)) {
                    konamiCode++;
                }
                else if (ke.getCode() == KeyCode.LEFT&& (konamiCode == 4 || konamiCode == 6)) {
                    konamiCode++;
                }
                else if (ke.getCode() == KeyCode.RIGHT&& (konamiCode == 5 || konamiCode == 7)){
                    konamiCode++;
                }
                else if (ke.getCode() == KeyCode.B&& konamiCode == 8 ){
                    konamiCode++;
                }
                else if (ke.getCode() == KeyCode.A&& konamiCode == 9 ){
                    System.out.println("KO-KO-KO-KONAMI COOOOOOOOOODE !!!");
                    konamiCode=0;
                }
            }
        });
    }

    public Collection<ImageView> getAllViews(int nb) {
        //TODO : if the list is empty ?

        ArrayList<ImageView> res = new ArrayList<ImageView>();
        for(Fish f : fishes) {
            res.add(f.get_View(nb));
        }

        return res;
    }

    public void addFish(String name, int x, int y, int w, int h) {
              Fish f = new Fish(x, y, w, h, name);
              fishes.add(f);
              aquarium.getChildren().add(f.get_View(0));
    }

    public void removeFish(String name) {
        Fish toRemove = null;
        for (Fish f : fishes) {
            if (f.getName().equalsIgnoreCase(name)) {
                System.out.println("DEBUG : On supprime " + name);
                toRemove = f;
            }
        }
        if (toRemove != null)
            fishes.remove(toRemove);
    }

    public void setGoal(String name, int x, int y, long d) {
        for (Fish f : fishes ) {
            if (f.getName().equalsIgnoreCase(name)) {
                System.out.println("On met le goal " + x + "/" + y + " a " + name + " en le faisant en " + d + "millisecondes" );
                f.setGoal(x, y, d);
            }
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}
