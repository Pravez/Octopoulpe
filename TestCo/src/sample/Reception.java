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

        while(true){
            try {
                message = in.readLine();
                if (!message.contentEquals("ping 12345")) {
                    System.out.println("Message : "+ message);
                    String[] args = message.split(" |\\[|\\]|\\,");
                    switch(args[0]){
                        case "addFish":
                            out.println("< Fish m successfully added");
                            out.flush();
                            break;
                        case "delFish":
                            out.println("< Fish "+ args[1] + " removed !");
                            out.flush();
                            break;
                        case "startFish":
                            out.println("< Fish " + args[1] + " started");
                            out.flush();
                            break;
                        case "getFishes":
                            out.println("list [m at 0x0,10x10,5] [tst at 90x90,10x10,5] ");
                            out.flush();
                            break;
                        case "getFishesContinuously":
                            System.out.println("WE DETECT THE GETFISHESCONTINUOUSLY");
                            int i=0;
                            while(true){
                                i++;
                                try {
                                    Thread.sleep(2000);
                                    System.out.println("ON ENVOI");
                                    /*if (i%2 == 0)
                                        out.println("list [m at 0x0,10x10,5]");
                                    else
                                        out.println("list [m at 100x100,10x10,5]");
                                    out.flush();*/

                                    if (false)
                                        break;

                                } catch(InterruptedException e) {System.out.println("DEBUG : Exception --> " + e.toString());}
                            }
                            break;
                        case "hello":
                            if (args.length == 4) {
                                out.println("< greeting " + args[3]);
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
