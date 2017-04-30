package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TextArea;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

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
    public void start(Stage primaryStage) throws Exception{

        Pane entry = new Pane();
        System.out.println("ADDRESS : " + InetAddress.getLocalHost());
        //connexion();

        display = new TextArea();
        display.setEditable(false);
        entry.getChildren().add(display);

        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Fake controler");
        primaryStage.setScene(new Scene(entry, 300, 275));
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
                System.out.println("Problème de connexion");
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
