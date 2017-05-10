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

/**
 * class that represent a view, a part of the global aquarium
 */
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

    /**
     * allow to start the application
     * @param primaryStage
     * @throws Exception
     */
    @Override
    public void start(Stage primaryStage) throws Exception{

        fishes = new ArrayList<Fish>();
        logsPath = Paths.get(System.getProperty("user.dir") + "/logs.txt");
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
        primaryStage.setX(100);
        primaryStage.setY(100);
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

    /**
     * initialise the elements of the view
     */
    private void initView () {
        final URL url = getClass().getResource("Images/bg.png");
        final Image bg = new Image(url.toExternalForm());
        background =  new ImageView(bg);
        background.setFitHeight(height);
        background.setFitWidth(width);
        aquarium.getChildren().setAll(background);
        aquarium.getChildren().addAll(getAllViews(0));
    }

    /**
     * write a string in the log file
     * @param s logs to write
     */
    protected void writeLogs(String s) {
        try {
            Files.write(logsPath, Arrays.asList("> " + s), Charset.forName("UTF-8"), StandardOpenOption.APPEND);
        } catch (IOException e) {System.out.println("Probleme lors de l'ecriture des logs : " + e.toString());}
    }

    /**
     * define the severals actions to handle each tick
     */
    private void handler() {
        new AnimationTimer() {
            @Override
            public void handle(long now) {
                timer++;;

                if (console.threadIsOver() ){//|| !console.parser.communicator.isConnected()) {
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

    /**
     * configure the aquarium thanks to the aquarium.cfg
     */
    private void config() {
            try {
                List<String> lines = Files.readAllLines(Paths.get(System.getProperty("user.dir") + "/resources/affichage.cfg"), StandardCharsets.ISO_8859_1);

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
                writeLogs("On configure :\n ID="+id+"\n Adresse du serveur="+address+"\n Port="+port+"\n Timeout par defaut="+pingTimeslice+" millisecondes\n Repertoire des images="+imagesURL+"\n");
                //console.parser.communicator.config(address, port);
            } catch (IOException e) {
                System.out.println("Exception lors de la lecture de affichage.cfg : " + e.toString());
                writeLogs("Probleme lors de la lecture du fichier de configuration.\n");
            }
        }

    /**
     * update the size of the fish with corresponding name
     * @param name name of the fish to update
     * @param w the new width
     * @param h the new height
     */
    public void setFishSize(String name, int w, int h) {
        for (Fish f: fishes) {
            if (f.getName().contentEquals(name)) {
                f.setSize(w*width/100, h*height/100);
            }
        }
    }

    /**
     * allow to know with the aquarium has a fish
     * @param name name of the fish we seacrh
     * @return 1 if it contains the fish, 0 else
     */
    public boolean hasFish(String name) {
        for (Fish f: fishes) {
            if (f.getName().contentEquals(name))
                return true;
        }
        return false;
    }

    /**
     * allow to get a description of the aquarium
     * @return a description of the aquarium in string
     */
    public String toString() {
        String s = new String(fishes.size() + " poisson(s) trouve(s)" + System.lineSeparator());

        for (Fish f: fishes) {
            s += "Fish " + f.toString(width, height) + System.lineSeparator();
        }

        return s;
    }

    /**
     * allow to know how many fishes the aquarium contains
     * @return the number of fish in the aquarium
     */
    public int getNbFishes() {
        return fishes.size();
    }

    /**
     * initialise the elements for the konami code
     */
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

    /**
     * allow to get the view of each fish in the aquarium
     * @param nb specify the view we want (two views for animation)
     * @return a collection of all the views of fishes
     */
    public Collection<ImageView> getAllViews(int nb) {
        ArrayList<ImageView> res = new ArrayList<ImageView>();
        for(Fish f : fishes) {
            res.add(f.get_View(nb));
        }
        return res;
    }

    /**
     * add a fish in the aquarium
     * @param f the fish we want to add
     */
    public void addFish(Fish f) {
        hasNew = true; //to know that another thread add a fish (just the main thread can add it at the view)
        fishes.add(f);
    }

    /**
     * add a fish in the aquarium
     * @param name name of the new fish
     * @param x position in x of the new fish
     * @param y position in y of the new fish
     * @param w width of the new fish
     * @param h height of the new fish
     * @param started a boolean to know with the new fish is already started or not
     */
    public void addFish(String name, int x, int y, int w, int h, boolean started) {
              Fish f = new Fish(x*width/100, y*height/100, w*width/100, h*height/100, name);
              f.setStarted(started);
              fishes.add(f);
              hasNew = true;
    }

    /**
     * start a fish
     * @param name name of the fish we want to start
     */
    public void setStarted(String name) {
        for (Fish f: fishes) {
            if (f.getName().contentEquals(name)) {
                System.out.println("FOUND FISH : " + name);
                f.setStarted(true);
            }
        }
    }

    /**
     * remove a fish
     * @param name name of the fish we want to remove
     */
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

    /**
     * set a new goal to a fish
     * @param name name of the fish we want to work on
     * @param x position in x of the goal
     * @param y position in y of the goal
     * @param d duration expected for the goal
     */
    public void setGoal(String name, int x, int y, long d) {
        for (Fish f : fishes ) {
            if (f.getName().equalsIgnoreCase(name)) {
                writeLogs("Nouveau but pour le poisson "+ name+ " : aller en "+x+"x"+y+ " en " + d + " millisecondes/n");
                f.setGoal(x*width/100, y*height/100, d, timeSinceStart/timer);
            }
        }
    }

    /**
     * remove all the fishes that were not updated
     * @param fishesUpdated ArrayList of all the names of fish that were updated
     */
    public void removeNonUpdated(ArrayList<String> fishesUpdated) {
        ArrayList<Fish> toRemove = new ArrayList<Fish>();
        for( Fish f : fishes) {
            if (!fishesUpdated.contains(f.getName()))
                toRemove.add(f);
        }

        for (Fish f: toRemove){
            fishes.remove(f);
        }

    }

    /**
     * main function
     * @param args no args expected
     */
    public static void main(String[] args) {
        launch(args);
    }
}
