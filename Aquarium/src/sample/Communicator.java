package sample;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

public class Communicator {

    private Console console;
    private Socket socket;
    protected BufferedReader in;
    private Thread reicever;

    public Communicator(Console c) {
        console = c;
    }

    public boolean isConnected() {
        return (socket != null && socket.isConnected());
    }

    public void config(String address, int port) {
        boolean connected = false;
        while (!connected) {
            try {
                System.out.println("DEBUG : Try to connect at " + InetAddress.getByName(address) + ", with adress = " + address + " and with port = " + port);
                socket = new Socket(InetAddress.getByName(InetAddress.getLocalHost().toString().split("/")[1]), port);
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                connected = true;
            } catch (IOException e) {
                connected = false;
            }
        }
        System.out.println("DEBUG : Connexion etablie");
        reicever = new Thread(new ReceiveHandler(console, in));
        reicever.start();
    }

    public void send(String s) {
        System.out.println("DEBUG : On envoie " + s);
        if (isConnected()) {
            try {
                PrintWriter out =  new PrintWriter(socket.getOutputStream());
                console.aquarium.timeElpased = 0;
                out.println(s);
                out.flush();
            } catch (IOException e) {System.out.println("DEBUG : Exception in send !!");}
        }
    }

}
