package sample;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.awt.*;
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

    public Aquarium aquarium;

    private ToolBar toolbar;
    private String id;

    protected Parser parser;

    //Display
    Pane entry;
    VBox vb;
    protected TextArea display;
    private TextField input;
    private ComboBox comboBox;


    protected final List<String> history;
    protected int historyCount = 0;

    private int width;
    private int height;

    Thread continuously;

    ArrayList<Fish> toHandle;

    public Console(Aquarium a, int w, int h) {
        aquarium = a;
        this.setTitle("Console");
        entry = new Pane();

        toHandle = new ArrayList<Fish>();

        display = new TextArea();
        display.setEditable(false);
        history = new ArrayList<>();

        initTab();
        initInput();

        comboBox = new ComboBox();
        comboBox.getItems().addAll("addFish PoissonRouge at 50x50, 15x15, RandomWayPoint",
                "addFish PoissonClown at 50x50, 10x10, VerticalWayPoint",
                "addFish PoissonNain at 50x50, 10x10, HorizontalWayPoint",
                "startFish PoissonRouge",
                "delFish PoissonRouge",
                "getFishes",
                "getFishesContinuously",
                "stopSendContinuously");
        comboBox.setPromptText("Quick order");
        comboBox.setEditable(true);
        comboBox.setMinWidth(w);
        comboBox.setMaxWidth(w);
        comboBox.addEventHandler(KeyEvent.KEY_RELEASED, keyEvent -> {
            switch (keyEvent.getCode()) {
                case ENTER:
                    String action = input.getText();
                    history.add(action);
                    historyCount++;
                    display.appendText("> " + action + System.lineSeparator());
                    input.clear();

                    System.out.println("DEBUG : On a rentree : " + action);
                    parser.parser(action);
                    break;
            }});

        comboBox.valueProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue ov, String t, String t1) {
                input.setText(t1);
            }
        });


        vb = new VBox();
        display.setMinHeight(h-input.getHeight()-toolbar.getHeight()-50); //-10 for the height of windows itself
        display.setMaxHeight(h-input.getHeight()-toolbar.getHeight()-50);
        input.setPrefColumnCount(20);
        vb.getChildren().addAll(toolbar, display, input);
        entry.getChildren().add(vb);

        this.setScene(new Scene(entry, w, h));
        parser = new Parser(this, display);
    }

    public void addFishToHandle(String n, int x, int y, int w, int h) {
        System.out.println("DEBUG : on ajoute le poisson " + n + " avec pos=" + x + "/" + y + " et w=" + w + " et h=" +h);
        toHandle.add(new Fish(x*aquarium.width/100, y*aquarium.height/100, w*aquarium.width/100, h*aquarium.height/100, n));
    }

    public void removeFishToHandle(Fish f) {
        toHandle.remove(f);
    }

    public void popFishToHandle(String n) {
        Fish res = null;
        for (Fish f : toHandle) {
            System.out.println("DEBUG : ON PASSE PAR " + f.toString());
            if (f.getName().contentEquals(n)) {
                System.out.println("DEBUG : ON POP " + f.toString());
                aquarium.addFish(f);
                res = f;
            }
        }
        if (res != null)
            toHandle.remove(res);
    }

    public boolean threadIsOver() {
        return (parser.communicator.reicever != null && parser.communicator.reicever.getState() == Thread.State.TERMINATED);
    }

    public void setId(String id) {
        this.id = id;
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

                    System.out.println("DEBUG : On a rentree : " + action);
                    parser.parser(action);
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

    public void logOut() {
        parser.communicator.logOut();
        this.close();
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

        CheckBox cb = new CheckBox("Quick order");
        cb.selectedProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                //cb.setSelected(!newValue);
                if (newValue) {
                    vb.getChildren().remove(input);
                    vb.getChildren().add(comboBox);
                    entry.getChildren().setAll(vb);
                }
                else {
                    vb.getChildren().remove(comboBox);
                    vb.getChildren().add(input);
                    entry.getChildren().setAll(vb);
                }
            }
        });

        toolbar = new ToolBar(tab3, new Separator(), tab1, new Separator(), tab2, new Separator(), cb);
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
}