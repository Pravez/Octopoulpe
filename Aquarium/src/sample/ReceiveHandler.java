package sample;

import java.io.BufferedReader;
import java.io.IOException;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.LinkedList;

/**
 * class that define the receiver, a thread that will read on the socket and handle the response of the server
 */
public class ReceiveHandler implements Runnable {

    Console console;
    Communicator communicator;

    String message;
    BufferedReader in;

    boolean running;
    protected LinkedList<String> orderHistory;

    /**
     * constructor of the receiver
     * @param c console that will use the receiver
     * @param in buffered reader on which we will read
     */
    public ReceiveHandler(Console c, BufferedReader in) {
        this.in = in;
        running = true;
        console = c;
        communicator = console.parser.communicator;
        orderHistory = new LinkedList<String>();
    }

    /**
     * add an order to the history
     * @param s order to add
     */
    public void addToHistory(String s) {
        orderHistory.add(s);
    }

    /**
     * too stop the execution of the thread
     */
    public void stop() {
        running = false;
    }

    /**
     * function to handle a response saying OK
     */
    synchronized private void handleOK() {
        while (orderHistory.isEmpty()) {};
        String order = orderHistory.pop();
        String[] argsOrder = order.split (" |\\, ");
        switch (argsOrder[0]) {
            case "addFish":
                console.aquarium.addFish(argsOrder[1], Integer.parseInt(argsOrder[3].split("x")[0]), Integer.parseInt(argsOrder[3].split("x")[1]), Integer.parseInt(argsOrder[4].split("x")[0]), Integer.parseInt(argsOrder[4].split("x")[1]), false);
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

    /**
     * run function of the thread, that will read on the socket and will handle the response
     */
    public void run() {
        while(running){
            try {
                try {
                    message = in.readLine();

		if (message != null)
		{
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
                            if (!orderHistory.isEmpty())
                                orderHistory.pop();
                        	break;
                    	case "OK":
                        	handleOK();
                        	break;
                    	default:
                        	break;
                }

                if (args[0] != "pong") {
                    console.writeDisplay(message);
                }
		}
                } catch (SocketException e) {running = false;}
            } catch (IOException e) {e.printStackTrace();}
        }
    }

    /**
     * handle the response "list" and update the aquarium in consequence
     * @param args arguments of the command we received
     */
    public void handleGoal(String[] args) {

        ArrayList<String> fishesUpdated = new ArrayList<String>();

        for (int i = 2; i < args.length; i = i + 7) {
            String n = args[i];
            fishesUpdated.add(n);
            int x = Integer.parseInt(args[i + 2].split("x")[0]);
            int y = Integer.parseInt(args[i + 2].split("x")[1]);
            int time = Integer.parseInt(args[i + 4]) * 1000;
            int w = Integer.parseInt(args[i + 3].split("x")[0]);
            int h = Integer.parseInt(args[i + 3].split("x")[0]);
            if (!console.aquarium.hasFish(n))
                console.aquarium.addFish(n, x, y, w, h, true);
            else {
                console.aquarium.setFishSize(n, w, h);
                console.aquarium.setGoal(n, x, y, time);
            }
        }
        console.aquarium.removeNonUpdated(fishesUpdated);
    }


}
