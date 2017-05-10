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

/**
 * class that define the communicator, the element that will allow the TCP communication with the socket
 */
public class Communicator {

    private Console console;
    private Socket socket;
    protected BufferedReader in;
    protected Thread receiver;
    protected ReceiveHandler handler;


    /**
     * constructor of the communicator
     * @param c console that will use the communicator
     */
    public Communicator(Console c) {
        console = c;
    }

    /**
     * check if the socket is connected
     * @return 1 if the socket is connected, 0 else
     */
    public boolean isConnected() {
        return (socket != null && socket.isConnected());
    }

    /**
     * configure the connexion
     * @param address address of ther server we want to connect with
     * @param port port on which we want to connect
     */
    public void config(String address, int port) {
        boolean connected = false;
        while (!connected) {
            try {
                console.aquarium.writeLogs("Tentative de connexion sur " + InetAddress.getByName(InetAddress.getLocalHost().toString().split("/")[1]) + " sur le port = " + port + "\n");

                socket = new Socket(InetAddress.getByName(InetAddress.getLocalHost().toString().split("/")[1].substring(1)), port);
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

    /**
     * allow to log out from the connexion
     */
    synchronized public void logOut() {
        try {
            if (socket != null) {
                socket.close();
            }
        }catch (IOException e) {System.out.println("Exception : " + e.toString()); console.aquarium.writeLogs("Exception lors de la fermeture de socket\n");}
         }

    /**
     * send a message on the socket
     * @param s message to send
     */
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
