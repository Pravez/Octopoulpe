package sample;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import static javafx.scene.control.Alert.AlertType.INFORMATION;

public class Console extends Stage {

    private Aquarium aquarium;

    private TextArea display;
    private TextField input;
    private ToolBar toolbar;

    private int width;
    private int height;

    public Console(Aquarium a, int w, int h) {
        aquarium = a;
        this.setTitle("Console");
        Pane entry = new Pane();

        display = new TextArea();
        display.setEditable(false);

        Label onglet1 = new Label("Help");
        onglet1.setOnMouseClicked(new EventHandler<MouseEvent>() {
            public void handle(MouseEvent e) {
                if (e.getButton() == MouseButton.PRIMARY) {
                    //Tooltip tooltip = new Tooltip("You need help ? Sad !");
                    Alert alert = new Alert(INFORMATION);
                    System.out.println("DEBUG : ALERT");
                    alert.show();
                    alert.showAndWait().ifPresent(response -> {
                        if (response == ButtonType.OK) {
                            alert.close();
                        }
                    });
                }
            }

        });

        Label onglet2 = new Label("About");

        toolbar = new ToolBar(onglet1, new Separator(), onglet2 );

        //Label label1 = new Label("Choose your action : ");
        input = new TextField ();
        input.setOnAction(new EventHandler<ActionEvent>() {
            public void handle(ActionEvent ak) {

                String action = input.getText();
                display.appendText("> " + action + System.lineSeparator());
                input.clear();

                System.out.println("DEBUG : On a rentré : " + action);
                parser(action.split(" "));
            }

        });

        VBox vb = new VBox();
        display.setMinHeight(h-input.getHeight()-toolbar.getHeight());
        display.setMaxHeight(h-input.getHeight()-toolbar.getHeight());
        input.setPrefColumnCount(20);
        vb.getChildren().addAll(toolbar, display, input);
        entry.getChildren().add(vb);

        this.setScene(new Scene(entry, w, h));
    }

    private void parser(String[] args) {
        if (args[0].equalsIgnoreCase("addFish")) {
            //TODO : check type of arguments
            if (args.length == 6) {
                aquarium.addFish(args[1], Integer.parseInt(args[2]), Integer.parseInt(args[3]), Integer.parseInt(args[4]), Integer.parseInt(args[5]));
                display.appendText("< OK" + System.lineSeparator());
            }
            else
                display.appendText("< Wrong syntax ! Usage : 'addFish name x y w h'" + System.lineSeparator());
        }
        else if (args[0].equalsIgnoreCase("delFish")) {
            if (args.length == 2) {
                aquarium.removeFish(args[1]);
                display.appendText("< OK" + System.lineSeparator());
            }
            else
                display.appendText("< Wrong syntax ! Usage : 'delFish name'" + System.lineSeparator());
        }
        else if (args[0].equalsIgnoreCase("status")) {
            if (args.length == 1) {
                display.appendText("< OK : Connecté à ... " + System.lineSeparator());
            }
            else
                display.appendText("< Wrong syntax ! The command 'status' doesn't expect arguments." + System.lineSeparator());
        }
        else if (args[0].equalsIgnoreCase("startFish")) {
            if (args.length == 2)
                System.out.println("DEBUG : Want to start the fish " + args[1]);
            else
                display.appendText("< Wrong syntax ! Usage : 'startFish name'" + System.lineSeparator());
        }
        //TODO : remove the setGoal ?
        else if (args[0].equalsIgnoreCase("setGoal")) {
            if (args.length == 4) {
                aquarium.setGoal(args[1], Integer.parseInt(args[2]), Integer.parseInt(args[3]));
                display.appendText("< OK" + System.lineSeparator());
            }
            else
                display.appendText("< Wrong syntax ! Usage : 'setGoal name x y'" + System.lineSeparator());
        }
        else
            display.appendText("< Wrong Syntax ! Please read the help." + System.lineSeparator());
    }


}
