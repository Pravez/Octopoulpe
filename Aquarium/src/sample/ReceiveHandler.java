package sample;

import java.io.BufferedReader;
import java.io.IOException;
import java.net.SocketException;
import java.util.LinkedList;

public class ReceiveHandler implements Runnable {

    Console console;
    Communicator communicator;

    String message;
    BufferedReader in;

    boolean running;
    protected LinkedList<String> orderHistory;

    public ReceiveHandler(Console c, BufferedReader in) {
        this.in = in;
        running = true;
        console = c;
        communicator = console.parser.communicator;
        orderHistory = new LinkedList<String>();
    }

    public void addToHistory(String s) {
        orderHistory.add(s);
    }

    public void stop() {
        running = false;
    }

    synchronized private void handleOK() {
        while (orderHistory.isEmpty()) {System.out.println("DEBUG : On attend, c'est vide.");};
        String order = orderHistory.pop();
        String[] argsOrder = order.split (" |\\, ");
        switch (argsOrder[0]) {
            case "addFish":
                console.aquarium.addFish(argsOrder[1], Integer.parseInt(argsOrder[3].split("x")[0]), Integer.parseInt(argsOrder[3].split("x")[1]), Integer.parseInt(argsOrder[4].split("x")[0]), Integer.parseInt(argsOrder[4].split("x")[1]));
                break;
            case "startFish":
                console.aquarium.setStarted(argsOrder[1]);
                break;
            case "delFish":
                console.aquarium.removeFish(argsOrder[1]);
                break;
            default:
                break;
        }
    }

    public void run() {
        while(running){
            try {
                try {
                    message = in.readLine();

                console.aquarium.writeLogs("On recoit : " + message);
                String[] args = message.split(" |\\[|\\]|\\,");
                for (String s : args)
                switch (args[0]) {
                    case "bye":
                        running = false;
                        break;
                    case "list":
                        handleGoal(args);
                        break;
                    case "no":
                        running = false;
                        break;
                    case "greeting":
                        console.setId(args[1]);
                        break;
                    case "NOK":
                        console.display.appendText(message + System.lineSeparator());
                        break;
                    case "OK":
                        handleOK();
                        break;
                    default:
                        break;
                }

                if (args[0] != "pong") {
                    console.display.appendText("< " + message + System.lineSeparator());
                }
                } catch (SocketException e) {running = false;}
            } catch (IOException e) {e.printStackTrace();}
        }
    }

    public void handleGoal(String[] args) {

        for (int i = 2; i < args.length; i = i + 7) {
            String n = args[i];
            int x = Integer.parseInt(args[i + 2].split("x")[0]);
            int y = Integer.parseInt(args[i + 2].split("x")[1]);
            int time = Integer.parseInt(args[i + 4]) * 1000;
            int w = Integer.parseInt(args[i + 3].split("x")[0]);
            int h = Integer.parseInt(args[i + 3].split("x")[0]);
            if (!console.aquarium.hasFish(n))
                console.aquarium.addFish(n, x, y, w, h);
            else {
                console.aquarium.setFishSize(n, w, h);
                console.aquarium.setGoal(n, x, y, time);
            }
        }
    }


}
