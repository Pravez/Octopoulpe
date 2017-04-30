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

import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;

public class Aquarium extends Application {

    //config attributes
    private String id;
    private int pingTimeslice;
    private String imagesURL;
    private int timeElpased = 0;

    //animation attributes
    private int timer = 0;
    private long previousTime = 0;

    //display attributes
    private int width = 650;
    private int height = 400;
    private final Pane aquarium = new Pane();
    private ImageView background;

    //general attributes
    private ArrayList<Fish> fishes;
    private Console console;

    //others
    private int konamiCode = 0;

    @Override
    public void start(Stage primaryStage) throws Exception{

        String test = "list [PoissonRouge at 90x4,10x4,5] [PoissonClown at 20x80,12x6,5]";

        for(String s:test.split(" |\\[|\\]|\\,"))
            System.out.println(s);

        while(true) {
            if (false)
                break;
        } ;

        //Creation of the console window
        console = new Console(this, 400, 400);
        console.show();
        config();
        console.suggestInput("hello in as " + id);

        fishes = new ArrayList<Fish>();

        //initialisation of the aquarium
        final URL url = getClass().getResource("Images/bg.png");
        final Image bg = new Image(url.toExternalForm());
        background =  new ImageView(bg);
        background.setFitHeight(height);
        background.setFitWidth(width);
        aquarium.getChildren().setAll(background);

        primaryStage.setResizable(false);

        aquarium.getChildren().addAll(getAllViews(0));
        primaryStage.setTitle("Aquarium");
        primaryStage.setMaxHeight(height);
        primaryStage.setMaxWidth(width);
        primaryStage.setScene(new Scene(aquarium, width, height));
        primaryStage.show();

        initKonami();

        handler();
    }

    private void handler() {
        new AnimationTimer() {
            @Override
            public void handle(long now) {
                //System.out.println("DEBUG : now = " + (now/1000000));
                timer++;
                console.checkMessage();

                if (timeElpased >= pingTimeslice) {
                    timeElpased = 0;
                    console.send("ping 12345");
                }

                if (timer%15 == 0) {
                    aquarium.getChildren().remove(1, aquarium.getChildren().size()); //remove all except background
                    aquarium.getChildren().addAll(getAllViews(1));
                }
                if (timer%30 == 0) {
                    aquarium.getChildren().remove(1, aquarium.getChildren().size() );
                    aquarium.getChildren().addAll(getAllViews(0));
                }

                if (previousTime != 0) {
                    timeElpased += previousTime;
                    for (Fish f : fishes) {
                        f.update((now/1000000) - previousTime);
                    }
                }

                previousTime = now/1000000;
            }
        }.start();
    }

    private void config() {
        //TODO : On suppose que le config est correctement fait  et complété ?
            try {
                List<String> lines = Files.readAllLines(Paths.get(System.getProperty("user.dir") + "/src/sample/affichage.cfg"), StandardCharsets.ISO_8859_1);

                String address = new String();
                int port = 0;
                for (String s : lines) {
                    if (!s.isEmpty() && s.charAt(0) != '#') {
                        String[] words = s.split(" ");
                        System.out.println("DEBUG : FIRST WORD " + words[0]);
                        switch (words[0]) {
                            case "controller-address":
                                System.out.println("DEBUG : ADDRESS " + words[2]);
                                address = words[2];
                                break;
                            case "id":
                                System.out.println("DEBUG : ID " + words[2]);
                                id = words[2];
                                break;
                            case "controller-port":
                                System.out.println("DEBUG : PORT " + words[2]);
                                port = Integer.parseInt(words[2]);
                                break;
                            case "display-timeout-value":
                                System.out.println("DEBUG : TIMESLICE " + words[2]);
                                pingTimeslice = Integer.parseInt(words[2]);
                                break;
                            case "resources":
                                System.out.println("DEBUG : RESSOURCE " + words[2]);
                                imagesURL = words[2];
                                break;
                            default:
                                break;
                        }
                    }
                }
                console.config(address, port);
            } catch (IOException e) {
                System.out.println("DEBUG : DIDN'T FOUND FILE ! Exception : " + e.toString());
            }
        }

    public void setFishSize(String name, int w, int h) {
        for (Fish f: fishes) {
            if (f.getName().contentEquals(name)) {
                f.setSize(w, h);
            }
        }
    }

    public boolean hasFish(String name) {
        for (Fish f: fishes) {
            if (f.getName().contentEquals(name))
                return true;
        }
        return false;
    }

    public String toString() {
        String s = new String(fishes.size() + " poisson(s) trouvé(s)" + System.lineSeparator());

        for (Fish f: fishes) {
            s += "Fish " + f.toString(width, height) + System.lineSeparator();
        }

        return s;
    }

    public int getNbFishes() {
        return fishes.size();
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
                    System.out.println("DEBUG : KONAMI CODE !!!");
                    final URL url = getClass().getResource("Images/bb.png");
                    final Image bg = new Image(url.toExternalForm());
                    background =  new ImageView(bg);
                    background.setFitHeight(height);
                    background.setFitWidth(width);
                    aquarium.getChildren().setAll(background); //remove all except background
                    aquarium.getChildren().addAll(getAllViews(0));

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

    public void addFish(String name, int x, int y, int w, int h, String movingModel) {
              Fish f = new Fish(x*width/100, y*height/100, w*width/100, h*height/100, name, movingModel);
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
                f.setGoal(x*width/100, y*height/100, d);
            }
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}
