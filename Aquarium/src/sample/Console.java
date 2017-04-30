package sample;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import static javafx.scene.control.Alert.AlertType.INFORMATION;

public class Console extends Stage {

    private Aquarium aquarium;
    private ToolBar toolbar;

    private Socket socket;
    private BufferedReader in;
    private BufferedReader inContinuously;
    private String id;

    private TextArea display;
    private TextField input;
    protected final List<String> history;
    protected int historyCount = 0;

    private int width;
    private int height;

    Thread continuously;

    public Console(Aquarium a, int w, int h) {
        aquarium = a;
        this.setTitle("Console");
        Pane entry = new Pane();

        display = new TextArea();
        display.setEditable(false);

        history = new ArrayList<>();

        initTab();
        initInput();

        VBox vb = new VBox();
        display.setMinHeight(h-input.getHeight()-toolbar.getHeight()-50); //-10 for the height of windows itself
        display.setMaxHeight(h-input.getHeight()-toolbar.getHeight()-50);
        input.setPrefColumnCount(20);
        vb.getChildren().addAll(toolbar, display, input);
        entry.getChildren().add(vb);

        this.setScene(new Scene(entry, w, h));
    }

    public void suggestInput(String s) {
        input.setText(s);
    }
    private void initInput() {
        input = new TextField ();
        input.addEventHandler(KeyEvent.KEY_RELEASED, keyEvent -> {
            switch (keyEvent.getCode()) {
                case ENTER:
                    String action = input.getText();
                    history.add(action);
                    historyCount++;
                    display.appendText("> " + action + System.lineSeparator());
                    input.clear();

                    System.out.println("DEBUG : On a rentré : " + action);
                    parser(action);
                    break;
                case UP:
                    if (historyCount == 0) {
                        break;
                    }
                    historyCount--;
                    input.setText(history.get(historyCount));
                    input.selectAll();
                    break;
                case DOWN:
                    if (historyCount == history.size() - 1) {
                        historyCount = history.size();
                        input.clear();
                        break;
                    }
                    if (historyCount < history.size()) {
                        historyCount++;
                        input.setText(history.get(historyCount - 1));
                        input.selectAll();
                    }
                    break;
                default:
                    historyCount = history.size();
                    break;
            }
        });
    }

    public void config(String address, int port) {
        boolean connected = false;
        while (!connected) {
            try {
                System.out.println("Try to connect at " + InetAddress.getLocalHost().toString().split("/")[1] + ", with port = " + port);
                socket = new Socket(InetAddress.getByName(InetAddress.getLocalHost().toString().split("/")[1]), 2009); //TODO : change ADDRESS and port
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                inContinuously = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                connected = true;
            } catch (IOException e) {
                connected = false;
                //System.out.println("Problème de connexion : " + e.toString());
            }
        }
        System.out.println("CONNECTE !!!!!!!!!");
    }

    private void initTab() {
        //TODO : Automatic listing with file ?
        Label tab1 = new Label("Help");
        tab1.setOnMouseClicked(new EventHandler<MouseEvent>() {
            public void handle(MouseEvent e) {
                if (e.getButton() == MouseButton.PRIMARY) {
                    Alert alert = new Alert(INFORMATION);
                    System.out.println("DEBUG : HELP");
                    alert.setHeaderText("Commands available : ");
                    alert.setTitle("Help");
                    alert.setHeight(200);
                    alert.setContentText("Here the command available : " + System.lineSeparator()
                            + " - hello [in as ID]" + System.lineSeparator()
                            + " - status " + System.lineSeparator()
                            + " - startFish name " + System.lineSeparator()
                            + " - addFish name x y w h modelMoving " + System.lineSeparator()
                            + " - delFish name " + System.lineSeparator()
                            + " - setGoal name x y delay" + System.lineSeparator()
                            + " - getFishes " + System.lineSeparator()
                            + " - getFishesContinuously " + System.lineSeparator() );

                    alert.showAndWait().ifPresent(response -> {
                        if (response == ButtonType.OK) {
                            alert.close();
                        }
                    });
                }
            }
        });

        Label tab2 = new Label("About");
        tab2.setOnMouseClicked(new EventHandler<MouseEvent>() {
            public void handle(MouseEvent e) {
                if (e.getButton() == MouseButton.PRIMARY) {
                    Alert alert = new Alert(INFORMATION);
                    System.out.println("DEBUG : ABOUT");
                    alert.setHeaderText("About the software");
                    alert.setTitle("About");
                    alert.setHeight(200);
                    alert.setContentText("This software was made for a student's project as part of ENSEIRB-MATMECA's studies." + System.lineSeparator()
                            + " Contributors : " + System.lineSeparator()
                            + " Paul Breton " + System.lineSeparator()
                            + " Paul Gaulier " + System.lineSeparator()
                            + " Louise Mouret" + System.lineSeparator()
                            + " Laurie-Anne Parant " + System.lineSeparator()
                            + " Nicolas Vidal " + System.lineSeparator());

                    alert.showAndWait().ifPresent(response -> {
                        if (response == ButtonType.OK) {
                            alert.close();
                        }
                    });
                }
            }
        });

        Label tab3 = new Label("Fishes");
        tab3.setOnMouseClicked(new EventHandler<MouseEvent>() {
            public void handle(MouseEvent e) {
                if (e.getButton() == MouseButton.PRIMARY) {
                    Alert alert = new Alert(INFORMATION);
                    System.out.println("DEBUG : FISH");
                    alert.setHeaderText("Fishes available : ");
                    alert.setTitle("Fishes");
                    alert.setHeight(200);
                    alert.setContentText("Here the fishes available : " + System.lineSeparator() +getFishesAvailable() );

                    alert.showAndWait().ifPresent(response -> {
                        if (response == ButtonType.OK) {
                            alert.close();
                        }
                    });
                }
            }
        });

        toolbar = new ToolBar(tab3, new Separator(), tab1, new Separator(), tab2);
    }

    private String getFishesAvailable() {
        //TODO : use function in class Fish ?
            File directory = new File(  System.getProperty("user.dir") + "/src/sample/Images/");
            System.out.println("DEBUG : Directory " + directory.toString() + " exists : " + directory.exists());
            String[] listFiles = directory.list();
            String res = new String("");
            for(int i=0;i<listFiles.length;i++) {
                listFiles[i] = listFiles[i].substring(0, listFiles[i].indexOf("."));
                if (!listFiles[i].equalsIgnoreCase("bg") && !listFiles[i].equalsIgnoreCase("bb") && !listFiles[i].contains("2")) { //to avaoid background
                    res += "- " + listFiles[i] + System.lineSeparator();
                }
            }
            return res;
    }

    private boolean checkMobilityModel(String m) {
        try {
            List<String> lines = Files.readAllLines(Paths.get(System.getProperty("user.dir") + "/src/sample/MobilityModel.txt"), Charset.defaultCharset());

            for (String s : lines) {
                System.out.println("DEBUG : dans fichier, ligne :" + s);
                if (m.contentEquals(s))
                    return true;
            }
        } catch (IOException e) {System.out.println("DEBUG : DIDN'T FOUND FILE !");}

        return false;
    }

    private void parser(String action) {
        String[] args = action.split (" |\\, ");
        switch (args[0]) {
            case "hello" :
                if (args.length == 4 || args.length == 1) {
                    send(action);
                    getAswHello();
                }
                else
                    display.appendText("< NOK. Usage : 'hello' or 'hello in as ID'" + System.lineSeparator());
                break;
            case "status" :
                if (args.length == 1) {
                    if (socket != null && socket.isConnected()) {
                        display.appendText("< OK : Connecté au contrôleur, " + aquarium.toString() + System.lineSeparator());
                    }
                    else
                        display.appendText("< NOK : Connexion non trouvée."  + aquarium.toString() + System.lineSeparator());
                }
                else
                    display.appendText("< NOK. The command 'status' doesn't expect arguments." + System.lineSeparator());
                break;
            case "addFish":
                if (args.length == 6) {
                    try {
                        if (!checkMobilityModel(args[5]))
                            display.appendText("< NOK : modèle de mobilité non supporté" + System.lineSeparator());
                        else {
                            send(action);
                            getAswAdd(args[1], Integer.parseInt(args[3].split("x")[0]), Integer.parseInt(args[3].split("x")[1]), Integer.parseInt(args[4].split("x")[0]), Integer.parseInt(args[4].split("x")[1]), args[5]);
                            display.appendText("< OK" + System.lineSeparator());
                        }
                    } catch (NumberFormatException e) {
                        display.appendText("< NOK ! " + e.getMessage().split("\"")[1] + " is supposed to be an integer." + System.lineSeparator());
                    }
                }
                else
                    display.appendText("< NOK. Usage : addFish 'name' at 'x'x'y', 'w'x'h', 'mobilityModel'" + System.lineSeparator());
                break;
            case "startFish":
                if (args.length == 2) {
                    if (aquarium.hasFish(args[1])) {
                        send(action);
                        getAswStart(args[1]);
                        display.appendText("< OK" + System.lineSeparator());
                    }
                    else
                        display.appendText("< NOK : Poisson inexistant" + System.lineSeparator());
                }
                else
                    display.appendText("< NOK. Usage : 'startFish name'" + System.lineSeparator());
                break;
            case "delFish":
                if (args.length == 2) {
                    if (aquarium.hasFish(args[1])) {
                        send(action);
                        getAswDel(args[1]);
                        display.appendText("< OK" + System.lineSeparator());
                    }
                    else
                        display.appendText("< NOK : Poisson inexistant" + System.lineSeparator());
                }
                else
                    display.appendText("< Wrong syntax ! Usage : 'delFish name'" + System.lineSeparator());
                break;
            case "getFishes":
                if (args.length == 1) {
                    send(action);
                    getAswGoal();
                    display.appendText("< OK" + System.lineSeparator());
                }
                else
                    display.appendText("< NOK. Usage : 'getFishes'" + System.lineSeparator());
                break;
            case "getFishesContinuously":
                if (args.length == 1) {
                    send(action);
                    display.appendText("< OK" + System.lineSeparator());
                    //continuously = new Thread(new ContinuouslyHandler(inContinuously, aquarium));
                    //continuously.run();
                    System.out.println("DEBUG : ON SORT DE CETTE MERDE");
                }
                else
                    display.appendText("< NOK. Usage : 'getFishesContinuously'" + System.lineSeparator());
                break;
            case "setGoal" :
                if (args.length == 5) {
                    if (aquarium.hasFish(args[1])) {

                        try {
                            aquarium.setGoal(args[1], Integer.parseInt(args[2]), Integer.parseInt(args[3]), Integer.parseInt(args[4]));
                            display.appendText("< OK" + System.lineSeparator());
                        }
                        catch (NumberFormatException e) {display.appendText("< NOK ! " + e.getMessage().split("\"")[1] + " is supposed to be an integer." + System.lineSeparator());}
                    }
                    else
                        display.appendText("< NOK : Poisson inexistant" + System.lineSeparator());
                }
                else
                    display.appendText("< NOK. Usage : 'setGoal name x y delay'" + System.lineSeparator());
                break;
            default:
                display.appendText("< NOK. Commande introuvable. (Les commandes sont disponible dans l'onglet)." + System.lineSeparator());
                break;
        }
    }

    public void getAswGoal() {
        if (socket != null && socket.isConnected()) {
            try {
                String message = in.readLine();
                System.out.println("DEBUG : ON ESSAI DE RECUPERER UN GOAL : " + message);
                String[] args = message.split(" |\\[|\\]|\\,");
                for (String s : args)
                    System.out.println("DEBUG : ON A DANS ARGS: " + s);
                for (int i = 2; i < args.length; i = i + 7) {
                    String n = args[i];
                    int x = Integer.parseInt(args[i + 2].split("x")[0]);
                    int y = Integer.parseInt(args[i + 2].split("x")[1]);
                    int time = Integer.parseInt(args[i + 4]) * 1000;
                    int w = Integer.parseInt(args[i + 3].split("x")[0]);
                    int h = Integer.parseInt(args[i + 3].split("x")[0]);
                    System.out.println("DEBUG : ON A W = : " + w + " ET H = " + h);
                    if (!aquarium.hasFish(n))
                        aquarium.addFish(n, x, y, w, h);
                    else {
                        aquarium.setFishSize(n, w, h);
                        aquarium.setGoal(n, x, y, time);
                    }
                    System.out.println("DEBUG : FIN GOAL");
                }
            }catch (IOException e) {
                System.out.println("DEBUG : Exception in send !!"); }
        }
        System.out.println("FINI");
    }

    public void getAswHello() {
        if (socket != null && socket.isConnected()) {
            boolean answered = false;
            while (!answered) {
                try {
                    String message = in.readLine();
                    display.appendText(message + System.lineSeparator());
                    id = message.split(" ")[2];
                    answered=true;
                } catch (IOException e) {
                    System.out.println("DEBUG : Exception in send !!");
                    answered=false;
                }
            }
        }
    }

    public void getAswAdd(String name, int x, int y, int w, int h, String model) {
        if (socket != null && socket.isConnected()) {
            boolean answered = false;
            while (!answered) {
                try {
                    String message = in.readLine();
                    display.appendText(message + System.lineSeparator());
                    if (message.split(" ")[3].contentEquals("successfully"));
                        aquarium.addFish(name, x, y, w, h);
                    answered=true;
                } catch (IOException e) {
                    System.out.println("DEBUG : Exception in send !!");
                    answered=false;
                }
            }
        }
    }

    public void getAswStart(String n) {
        if (socket != null && socket.isConnected()) {
            boolean answered = false;
            while (!answered) {
                try {
                    String message = in.readLine();
                    aquarium.setStarted(n);
                    answered=true;
                } catch (IOException e) {
                    System.out.println("DEBUG : Exception in send !!");
                    answered=false;
                }
            }
        }
    }

    public void getAswDel(String name) {
        if (socket != null && socket.isConnected()) {
            boolean answered = false;
            while (!answered) {
                try {
                    String message = in.readLine();
                    display.appendText(message + System.lineSeparator());
                    if (message.split(" ")[1].contentEquals("Fish"));
                            aquarium.removeFish(name);
                    answered=true;
                } catch (IOException e) {
                    System.out.println("DEBUG : Exception in send !!");
                    answered=false;
                }
            }
        }
    }

    public void send(String s) {
        if (socket != null && socket.isConnected()) {
            try {
                PrintWriter out =  new PrintWriter(socket.getOutputStream());
                out.println(s);
                out.flush();
            } catch (IOException e) {System.out.println("DEBUG : Exception in send !!");}
        }
    }
}