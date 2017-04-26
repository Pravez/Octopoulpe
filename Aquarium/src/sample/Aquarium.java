package sample;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.Collection;

public class Aquarium extends Application {

    private int timer = 0;
    private int konamiCode = 0;
    private ArrayList<Fish> fishes;

    private final Pane aquarium = new Pane();

    @Override
    public void start(Stage primaryStage) throws Exception{

        //tmp : initialisation of fish
        fishes = new ArrayList<Fish>();
        Fish f = new Fish(0, 0, 100, 100, "magicarpe");
        fishes.add(f);

        //initialisation of the aquarium
        aquarium.getChildren().setAll(getAllViews(0));
        primaryStage.setTitle("Aquarium");
        primaryStage.setScene(new Scene(aquarium, 600, 550));
        primaryStage.show();

        //Creation of the console window
        Console console = new Console(this, 400, 400);
        console.show();

        //KONAMI CODE
        initKonami();

        new AnimationTimer() {
            @Override
            public void handle(long now) {
                timer++;

                if (timer%25 == 0) {
                    aquarium.getChildren().setAll(getAllViews(1));
                }
                if (timer%50 == 0)
                    aquarium.getChildren().setAll(getAllViews(0));

                if (timer == 400 || timer == 800) {
                    if (timer == 800) {
                  timer = 0;
                    }
                }
                for (Fish f : fishes) {
                    f.update(timer);
                }
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
              aquarium.getChildren().setAll(getAllViews(0));
    }

    public void removeFish(String name) {
        for (Fish f : fishes ) {
            if (f.getName() == name)
                System.out.println("On supprime " + name);
                fishes.remove(f);
        }
    }

    public void setGoal(String name, int x, int y) {
        for (Fish f : fishes ) {
            if (f.getName() == name)
                System.out.println("On met le goal " + x + "/" + y + " a " + name);
                f.setGoal(x, y);
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}
