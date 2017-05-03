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
    private BufferedReader in;
    protected BufferedReader inContinuously;

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
                System.out.println("Try to connect at " + InetAddress.getLocalHost().toString().split("/")[1] + ", with port = " + port);
                //socket = new Socket(InetAddress.getByName(InetAddress.getLocalHost().toString().split("/")[1]), 2009); //TODO : change ADDRESS and port
                socket = new Socket(InetAddress.getLocalHost().toString().split("/")[1], port);
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                inContinuously = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                connected = true;
            } catch (IOException e) {
                connected = false;
                //System.out.println("Probleme de connexion : " + e.toString());
            }
        }
        System.out.println("CONNECTE !!!!!!!!!");
    }

    public void getAswGoal() {
        if (isConnected()) {
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
                    if (!console.aquarium.hasFish(n))
                        console.aquarium.addFish(n, x, y, w, h);
                    else {
                        console.aquarium.setFishSize(n, w, h);
                        console.aquarium.setGoal(n, x, y, time);
                    }
                    System.out.println("DEBUG : FIN GOAL");
                }
            }catch (IOException e) {
                System.out.println("DEBUG : Exception in send !!"); }
        }
        System.out.println("FINI");
    }

    public void getAswHello() {
        if (isConnected()) {
            boolean answered = false;
            while (!answered) {
                try {
                    String message = in.readLine();
                    console.display.appendText(message + System.lineSeparator());
                    console.setId(message.split(" ")[2]);
                    answered=true;
                } catch (IOException e) {
                    System.out.println("DEBUG : Exception in send !!");
                    answered=false;
                }
            }
        }
    }

    public void getAswAdd(String name, int x, int y, int w, int h, String model) {
        if (isConnected()) {
            boolean answered = false;
            while (!answered) {
                try {
                    String message = in.readLine();
                    console.display.appendText(message + System.lineSeparator());
                    if (message.split(" ")[1].contentEquals("OK"))
                        console.aquarium.addFish(name, x, y, w, h);
                    answered=true;
                } catch (IOException e) {
                    System.out.println("DEBUG : Exception in send !!");
                    answered=false;
                }
            }
        }
    }

    public void getAswStart(String n) {
        if (isConnected()) {
            boolean answered = false;
            while (!answered) {
                try {
                    String message = in.readLine();
                    console.aquarium.setStarted(n);
                    answered=true;
                } catch (IOException e) {
                    System.out.println("DEBUG : Exception in send !!");
                    answered=false;
                }
            }
        }
    }

    public void getAswDel(String name) {
        if (isConnected()) {
            boolean answered = false;
            while (!answered) {
                try {
                    String message = in.readLine();
                    console.display.appendText(message + System.lineSeparator());
                    if (message.split(" ")[1].contentEquals("Fish"));
                    console.aquarium.removeFish(name);
                    answered=true;
                } catch (IOException e) {
                    System.out.println("DEBUG : Exception in send !!");
                    answered=false;
                }
            }
        }
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
