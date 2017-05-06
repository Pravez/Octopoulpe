package sample;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;


public class Reception implements Runnable {

    private BufferedReader in;
    private PrintWriter out;
    private String message = null;

    private Emission emission;

    public Reception(BufferedReader in, PrintWriter pw){
        out = pw;
        this.in = in;
    }

    public void run() {
        int tour=0;

        while(true){
            tour++;
            try {
                message = in.readLine();
                if (!message.contentEquals("ping 12345")) {
                    System.out.println("Message : "+ message);
                    String[] args = message.split(" |\\[|\\]|\\,");
                    switch(args[0]){
                        case "log":
                            out.println("bye");
                            out.flush();
                            break;
                        case "addFish":
                            out.println("OK");
                            out.flush();
                            break;
                        case "delFish":
                            out.println("OK");
                            out.flush();
                            break;
                        case "startFish":
                            out.println("OK");
                            out.flush();
                            break;
                        case "getFishes":
                            if (tour%2 == 0)
                                out.println("list [PoissonRouge at 80x80,15x15,2]");
                            else
                                out.println("list [PoissonRouge at 35x35,15x15,2]");
                            out.flush();
                            break;
                        case "getFishesContinuously":
                            int i=0;
                            while(true){
                                i++;
                                try {
                                    out.println("list [PoissonRouge at -10x-10,15x15,4]");
                                    out.flush();
                                    Thread.sleep(4010);

                                    out.println("list [PoissonRouge at 110x110,15x15,3] [PoissonNain at -10x-10,10x10,2]");
                                    out.flush();
                                    Thread.sleep(4010);

                                    out.println("list [PoissonRouge at 70x70,15x15,3] [PoissonNain at 40x40,10x10,3]");
                                    out.flush();
                                    Thread.sleep(4010);

                                    out.println("list [PoissonNain at 60x60,10x10,3]");
                                    out.flush();
                                    Thread.sleep(4010);

                                    if (false)
                                        break;

                                } catch(InterruptedException e) {System.out.println("DEBUG : Exception --> " + e.toString());}
                            }
                            break;
                        case "stopSendContinuously":
                            out.println("OK : end of transaction");
                            out.flush();
                            break;
                        case "hello":
                            if (args.length == 4) {
                                out.println("greeting " + args[3]);
                                out.flush();
                            }
                            break;
                        default:
                            break;
                    }
                }

            } catch (IOException e) {

                e.printStackTrace();
            }
        }
    }

}
