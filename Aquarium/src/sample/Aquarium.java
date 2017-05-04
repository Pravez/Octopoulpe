package sample;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.URL;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.*;

public class Aquarium extends Application {

    //config attributes
    private String id;
    private long pingTimeslice; //in millisecondes
    private String imagesURL;
    protected long timeElpased = 0;
    protected long timeSinceStart = 0;

    //animation attributes
    private int timer = 0;
    private long previousTime = 0;

    //display attributes
    protected int width = 650;
    protected int height = 400;
    private final Pane aquarium = new Pane();
    private ImageView background;

    //general attributes
    private ArrayList<Fish> fishes;
    private Console console;
    private boolean hasNew=false;

    //others
    protected Path logsPath;
    private int konamiCode = 0;

    @Override
    public void start(Stage primaryStage) throws Exception{

        fishes = new ArrayList<Fish>();
        logsPath = Paths.get(System.getProperty("user.dir") + "/src/sample/logs.txt");
        try {
            Files.write(logsPath, Arrays.asList("> Demarrage de l'application...\n..\n.\n"), Charset.forName("UTF-8"));
        } catch (IOException e) {System.out.println("Probleme lors de l'ecriture des logs : " + e.toString());}

        //Creation of the console window
        console = new Console(this, 400, 400);
        console.show();
        config();
        console.suggestInput("hello in as " + id);

        //initialisation of the aquarium
        initView();
        primaryStage.setResizable(false);
        primaryStage.setTitle("Aquarium");
        primaryStage.setMaxHeight(height);
        primaryStage.setMaxWidth(width);
        primaryStage.setScene(new Scene(aquarium, width, height));

        Platform.setImplicitExit(false);
        primaryStage.setOnCloseRequest(event -> {
            console.parser.communicator.handler.stop();
            console.logOut();
            System.exit(1);
        });
        primaryStage.show();

        initKonami();

        handler();
    }

    private void initView () {
        final URL url = getClass().getResource("Images/bg.png");
        final Image bg = new Image(url.toExternalForm());
        background =  new ImageView(bg);
        background.setFitHeight(height);
        background.setFitWidth(width);
        aquarium.getChildren().setAll(background);
        aquarium.getChildren().addAll(getAllViews(0));
    }

    protected void writeLogs(String s) {
        try {
            Files.write(logsPath, Arrays.asList("> " + s), Charset.forName("UTF-8"), StandardOpenOption.APPEND);
        } catch (IOException e) {System.out.println("Probleme lors de l'ecriture des logs : " + e.toString());}
    }

    private void handler() {
        new AnimationTimer() {
            @Override
            public void handle(long now) {
                timer++;;

                if (console.threadIsOver() || !console.parser.communicator.isConnected()) {
                    writeLogs("Arret du programme.");
                    console.logOut();
                    System.exit(1);
                }

                if (hasNew) {
                    hasNew = false;
                    aquarium.getChildren().remove(1, aquarium.getChildren().size()); //remove all except background
                    aquarium.getChildren().addAll(getAllViews(1));
                }

                if (timeElpased >= pingTimeslice*1000000) {
                    timeElpased = 0;
                    writeLogs("Timeout bientot atteint : on ping.\n");
                    console.parser.communicator.send("ping 12345");
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
                    timeSinceStart+=(now / 1000000) - previousTime;
                    for (Fish f : fishes) {
                        f.update((now / 1000000) - previousTime);
                    }
                }
                previousTime = now / 1000000;
            }
        }.start();
    }

    private void config() {
        //TODO : On suppose que le config est correctement fait  et complete ?
            try {
                List<String> lines = Files.readAllLines(Paths.get(System.getProperty("user.dir") + "/src/sample/affichage.cfg"), StandardCharsets.ISO_8859_1);

                String address = new String();
                int port = 0;
                for (String s : lines) {
                    if (!s.isEmpty() && s.charAt(0) != '#') {
                        String[] words = s.split(" ");
                        switch (words[0]) {
                            case "controller-address":
                                address = words[2];
                                break;
                            case "id":
                                id = words[2];
                                break;
                            case "controller-port":
                                port = Integer.parseInt(words[2]);
                                break;
                            case "display-timeout-value":
                                pingTimeslice = Integer.parseInt(words[2])*1000;
                                break;
                            case "resources":
                                imagesURL = words[2];
                                break;
                            default:
                                break;
                        }
                    }
                }
                writeLogs("On configure :\n ID="+id+"\n Adresse du serveur="+address+"\n Port="+port+"\n Timeout par défaut="+pingTimeslice+" millisecondes\n Repertoire des images="+imagesURL+"\n");
                console.parser.communicator.config(address, port);
            } catch (IOException e) {
                System.out.println("Exception lors de la lecture de affichage.cfg : " + e.toString());
                writeLogs("Probleme lors de la lecture du fichier de configuration.\n");
            }
        }

    public void setFishSize(String name, int w, int h) {
        for (Fish f: fishes) {
            if (f.getName().contentEquals(name)) {
                f.setSize(w*width/100, h*height/100);
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
        String s = new String(fishes.size() + " poisson(s) trouve(s)" + System.lineSeparator());

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
                else if (ke.getCode() == KeyCode.DOWN && (konamiCode == 2 || konamiCode == 3)) {
                    konamiCode++;
                }
                else if (ke.getCode() == KeyCode.LEFT && (konamiCode == 4 || konamiCode == 6)) {
                    konamiCode++;
                }
                else if (ke.getCode() == KeyCode.RIGHT && (konamiCode == 5 || konamiCode == 7)){
                    konamiCode++;
                }
                else if (ke.getCode() == KeyCode.B && konamiCode == 8 ){
                    konamiCode++;
                }
                else if (ke.getCode() == KeyCode.A&& konamiCode == 9 ){
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
        ArrayList<ImageView> res = new ArrayList<ImageView>();
        for(Fish f : fishes) {
            res.add(f.get_View(nb));
        }
        return res;
    }

    public void addFish(Fish f) {
        hasNew = true; //to know that another thread add a fish (just the main thread can add it at the view)
        fishes.add(f);
    }

    public void addFish(String name, int x, int y, int w, int h) {
              Fish f = new Fish(x*width/100, y*height/100, w*width/100, h*height/100, name);
              fishes.add(f);
              hasNew = true;
    }

    public void setStarted(String name) {
        for (Fish f: fishes) {
            if (f.getName().contentEquals(name)) {
                System.out.println("FOUND FISH : " + name);
                f.setStarted(true);
            }
        }
    }

    public void removeFish(String name) {
        Fish toRemove = null;
        for (Fish f : fishes) {
            if (f.getName().equalsIgnoreCase(name)) {
                toRemove = f;
            }
        }
        if (toRemove != null)
            fishes.remove(toRemove);
    }

    public void setGoal(String name, int x, int y, long d) {
        for (Fish f : fishes ) {
            if (f.getName().equalsIgnoreCase(name)) {
                writeLogs("Nouveau but pour le poisson "+ name+ " : aller en "+x+"x"+y+ " en " + d + " millisecondes/n");
                f.setGoal(x*width/100, y*height/100, d, timeSinceStart/timer);
            }
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}
