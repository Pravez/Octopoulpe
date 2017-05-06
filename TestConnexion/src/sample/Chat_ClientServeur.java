package sample;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;


public class Chat_ClientServeur implements Runnable {

    private Socket socket = null;
    private BufferedReader in = null;
    private PrintWriter out = null;
    private Thread t3, t4;


    public Chat_ClientServeur(Socket s){

        socket = s;
    }
    public void run() {

        try {
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            out = new PrintWriter(socket.getOutputStream());

            Emission em = new Emission(out);

            Thread t3 = new Thread(new Reception(in, out));
            t3.start();

        } catch (IOException e) {
            System.err.println("S'est déconnecté ");
        }
    }
}
