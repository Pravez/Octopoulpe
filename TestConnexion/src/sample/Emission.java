package sample;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;


public class Emission implements Runnable {

    private PrintWriter out;
    private String message = null;
    private Scanner sc = null;

    public Emission(PrintWriter out) {
        System.out.println("DEBUG : ON CREE NOTRE EMISSION");
        this.out = out;
    }


    public void run() {
        System.out.println("DEBUG : ON RUN NOTRE EMISSION");

        sc = new Scanner(System.in);
    }
}