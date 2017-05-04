package sample;

import java.io.BufferedReader;
import java.io.IOException;

public class ReceiveHandler implements Runnable {

    Console console;

    String message;
    BufferedReader in;

    public ReceiveHandler(Console c, BufferedReader in) {
        this.in = in;
        console = c;
    }

    public void run() {
        boolean running = true;
        while(running){
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
                        //TODO : what we can do if no greeting ?
                        break;
                    case "greeting":
                        console.setId(args[1]);
                        break;
                    case "NOK":
                        console.display.appendText(message + System.lineSeparator());
                        break;
                    case "OK":
                        if (args.length > 4) {
                            if (args[5].contentEquals("added")) {
                                console.popFishToHandle(args[3]);
                            }
                            if (args[5].contentEquals("started")) {
                                console.aquarium.setStarted(args[3]);
                            }
                            if (args[5].contentEquals("removed")) {
                                console.aquarium.removeFish(args[3]);
                            }
                        }
                        break;
                    default:
                        break;
                }

                if (args[0] != "pong") {
                    console.display.appendText("< " + message + System.lineSeparator());
                }

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
