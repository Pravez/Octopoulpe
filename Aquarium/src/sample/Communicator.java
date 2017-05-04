package sample;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class Communicator {

    private Console console;
    private Socket socket;
    protected BufferedReader in;
    protected Thread receiver;
    protected ReceiveHandler handler;

    protected LinkedList<String> orderHistory;

    public Communicator(Console c) {
        console = c;
        orderHistory = new LinkedList<String>();
    }

    public boolean isConnected() {
        return (socket != null && socket.isConnected());
    }

    public void config(String address, int port) {
        boolean connected = false;
        while (!connected) {
            try {
                console.aquarium.writeLogs("Tentative de connexion sur " + InetAddress.getByName(address) + "sur le port = " + port + "\n");
                socket = new Socket(InetAddress.getByName(address), port);
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                connected = true;
            } catch (IOException e) {
                connected = false;
            }
        }
        console.aquarium.writeLogs("Connexion etablie.\n");
        handler = new ReceiveHandler(console, in);
        receiver = new Thread(handler);
        receiver.start();
    }

    synchronized public void logOut() {
        try {
            if (socket != null) {
                socket.close();
            }
        }catch (IOException e) {System.out.println("Exception : " + e.toString()); console.aquarium.writeLogs("Exception lors de la fermeture de socket\n");}
         }

    public void send(String s) {
        console.aquarium.writeLogs("On envoie au serveur : " +s+"\n");
        if (isConnected()) {
            try {
                PrintWriter out =  new PrintWriter(socket.getOutputStream());
                console.aquarium.timeElpased = 0;
                out.println(s);
                out.flush();
            } catch (IOException e) {System.out.println("Exception in send !!");console.aquarium.writeLogs("Exception lors de l'envoi.");}
        }
    }

}
