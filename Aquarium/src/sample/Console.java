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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
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

    private TextArea display;
    private TextField input;
    protected final List<String> history;
    protected int historyCount = 0;

    private int width;
    private int height;

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
                    parser(action.split(" "));
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

        toolbar = new ToolBar(tab1, new Separator(), tab2 );
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

    private void parser(String[] args) {
        if (args[0].equalsIgnoreCase("addFish")) {
            if (args.length == 7) {
                //TODO : check for new name if it already exists
                try {
                    if (! checkMobilityModel(args[6]))
                        display.appendText("< NOK : modèle de mobilité non supporté" + System.lineSeparator());
                    else {
                        aquarium.addFish(args[1], Integer.parseInt(args[2]), Integer.parseInt(args[3]), Integer.parseInt(args[4]), Integer.parseInt(args[5]), args[6]);
                        display.appendText("< OK" + System.lineSeparator());
                    }
                } catch (NumberFormatException e) {display.appendText("< NOK ! " + e.getMessage().split("\"")[1] + " is supposed to be an integer." + System.lineSeparator());}

            }
            else
                display.appendText("< NOK. Usage : 'addFish name x y w h modelMoving'" + System.lineSeparator());
        }
        else if (args[0].equalsIgnoreCase("delFish")) {
            if (args.length == 2) {
                //TODO : for every display ?
                if (aquarium.hasFish(args[1])) {
                    aquarium.removeFish(args[1]);
                    display.appendText("< OK" + System.lineSeparator());
                }
                else
                    display.appendText("< NOK : Poisson inexistant" + System.lineSeparator());
            }
            else
                display.appendText("< Wrong syntax ! Usage : 'delFish name'" + System.lineSeparator());
        }
        else if (args[0].equalsIgnoreCase("status")) {
            if (args.length == 1) {
                if (socket != null && socket.isConnected()) {
                    display.appendText("< OK : Connecté au contrôleur, " + aquarium.toString());
                }
                else
                   display.appendText("< NOK : Connexion non trouvée " + System.lineSeparator());
            }
            else
                display.appendText("< NOK. The command 'status' doesn't expect arguments." + System.lineSeparator());
        }
        else if (args[0].equalsIgnoreCase("startFish")) {
            //TODO : Check if name exists (juste in the display ?)
            if (args.length == 2) {
                if (aquarium.hasFish(args[1])) {
                    //TODO : actually start fish :3
                    display.appendText("< OK" + System.lineSeparator());
                }
                else
                    display.appendText("< NOK : Poisson inexistant" + System.lineSeparator());
                System.out.println("DEBUG : Want to start the fish " + args[1]);

            }
            else
                display.appendText("< NOK. Usage : 'startFish name'" + System.lineSeparator());
        }
        //TODO : remove the setGoal ?
        else if (args[0].equalsIgnoreCase("setGoal")) {
            //TODO : switch it to a receive order : go to ...
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
        }
        else if (args[0].equalsIgnoreCase("hello")) {
            if (args.length == 4 || args.length == 1) {
                //TODO : send ID to Controler with TCP
                Boolean response = true;
                if (response)
                    display.appendText("< greeting " + args[3] + System.lineSeparator());
                else
                    display.appendText("< no greeting" + System.lineSeparator());
            }
            else
                display.appendText("< NOK. Usage : 'hello' or 'hello in as ID'" + System.lineSeparator());
        }
        else if (args[0].equalsIgnoreCase("getFishes")) {
            if (args.length == 1) {
                //TODO : send demand to Controler with TCP : String[] fishes
                display.appendText("< connexion not established yet " + System.lineSeparator());
            }
            else
                display.appendText("< NOK. Usage : 'getFishes'" + System.lineSeparator());
        }
        else if (args[0].equalsIgnoreCase("getFishesContinuously")) {
            if (args.length == 1) {
                //TODO : send demand to Controler with TCP : String[] fishes
                display.appendText("< connexion not established yet " + System.lineSeparator());
            }
            else
                display.appendText("< NOK. Usage : 'getFishesContinuously'" + System.lineSeparator());
        }
        else
            display.appendText("< NOK. Please read the help." + System.lineSeparator());
    }
}
