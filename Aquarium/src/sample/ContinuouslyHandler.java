package sample;

import java.io.BufferedReader;
import java.io.IOException;

public class ContinuouslyHandler implements Runnable {

    Console console;

    String message;
    BufferedReader in;

    public ContinuouslyHandler (Console c, BufferedReader in) {
        this.in = in;
        console = c;
    }

    public void run() {

        while(true){
            try {
                message = in.readLine();
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

            } catch (IOException e) {e.printStackTrace();}
        }
    }

}
