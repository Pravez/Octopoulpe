package sample;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.Collection;

public class Aquarium extends Application {

    int timer = 0;
    int konamiCode = 0;
    ArrayList<Fish> fishes;

    final Pane aquarium = new Pane();

    //final Pane entry = new Pane();
    //Stage Entry;
    @Override
    public void start(Stage primaryStage) throws Exception{

        //tmp initialisation of fish

        fishes = new ArrayList<Fish>();
        Fish f = new Fish(0, 0, 100, 100, "magicarpe");
        fishes.add(f);

        //initialisation of the aquarium
        aquarium.getChildren().setAll(getAllViews(0));
        primaryStage.setTitle("Aquarium");
        primaryStage.setScene(new Scene(aquarium, 600, 550));

        primaryStage.show();

        Stage console = new Stage();
        console.setTitle("Console");
        Pane entry = new Pane();

        Label label1 = new Label("Choose your action : ");
        TextField textField = new TextField ();
        textField.setOnAction(new EventHandler<ActionEvent>() {
            public void handle(ActionEvent ak) {
                System.out.println();
                String action = textField.getText();
                String[] args = action.split(" ");
                System.out.println("On a rentré : " + action);
                System.out.println("Vérification du parse : " + args[0] + " et " + args[1]);
                if (args[0].equalsIgnoreCase("addFish")) {
                    System.out.println("On veut : " + args[1] + ", " + args[2] + ", " + args[3] + ", "  + args[4] + ", "  + args[5]);
                    addFish(args[1], Integer.parseInt(args[2]), Integer.parseInt(args[3]), Integer.parseInt(args[4]), Integer.parseInt(args[5]));
                }
                else if (args[0].equalsIgnoreCase("removeFish")) {
                    removeFish(Integer.parseInt(args[1]));
                }
            }
        });

        HBox hb = new HBox();
        hb.getChildren().addAll(label1, textField);
        hb.setSpacing(10);
        entry.getChildren().add(hb);

        console.setScene(new Scene(entry, 200, 200));

        console.show();

        //KONAMI CODE
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

        new AnimationTimer() {
            @Override
            public void handle(long now) {
                timer++;

                if (timer%25 == 0) {
                    aquarium.getChildren().setAll(getAllViews(1));
                }
                if (timer%50 == 0)
                    aquarium.getChildren().setAll(getAllViews(0));

                if (timer == 100) {
                    fishes.get(0).setGoal(100, 100);
                }
                if (timer == 200 || timer == 600) {
                    //addFish("magicarpe_2", 350, 200, 100, 100);
                }
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
              aquarium.getChildren().setAll(getAllViews(0));
    }

    public void removeFish(int index) {
              fishes.remove(index);
    }

    public static void main(String[] args) {
        launch(args);
    }
}
