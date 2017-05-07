package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.Pane;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import javafx.scene.layout.VBox;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class Main extends Application {

    private TextArea display;

    //Connexion
    private Socket socket;
    private BufferedReader in;
    PrintWriter out;

    public static ServerSocket ss = null;
    public static Thread t;

    @Override
    public void start(Stage primaryStage) {
        //connexion


        VBox vb = new VBox();
        Button b = new Button("Lancer scenario");
        Label l = new Label("Explication du second scenario\n blablablabla\nblablabla\n");
        vb.getChildren().addAll(l, b);

        final TitledPane titledPane1 = new TitledPane("Scenario 1", vb);
        final TitledPane titledPane2 = new TitledPane("Scenario 2", vb);
        final TitledPane titledPane3 = new TitledPane("Scenario 3", new Button("Bouton 3"));
        final TitledPane titledPane4 = new TitledPane("Scenario 4", new Button("Bouton 4"));
        final Accordion accordion = new Accordion();
        accordion.getPanes().setAll(titledPane1, titledPane2, titledPane3, titledPane4);
        accordion.setExpandedPane(titledPane1);
        final StackPane root = new StackPane();
        root.getChildren().add(accordion);
        final Scene scene = new Scene(root, 300, 250);
        primaryStage.setTitle("Test de l'accordéon");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    public void connexion() {
        boolean connected = false;
        while (!connected) {
            try {
                System.out.println("Try to connect at " + InetAddress.getLocalHost().toString().split("/")[1] + ", with port = " + 15);
                socket = new Socket(InetAddress.getByName(InetAddress.getLocalHost().toString().split("/")[1]), 15);
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                out = new PrintWriter(socket.getOutputStream());
                connected = true;
                System.out.println("Connected at " + socket.getInetAddress());
            } catch (IOException e) {
                connected = false;
                //System.out.println("Problème de connexion");
            }
        }
        System.out.println("CONNECTE !!!!!!!!!");
    }

    public static void main(String[] args) {

        try {
            ss = new ServerSocket(2009);
            System.out.println("Le serveur est à l'écoute du port "+ss.getLocalPort());

            t = new Thread(new Accepter_connexion(ss));
            t.start();

        } catch (IOException e) {
            System.err.println("Le port "+ss.getLocalPort()+" est déjà utilisé !");
        }

        launch(args);

    }
}
