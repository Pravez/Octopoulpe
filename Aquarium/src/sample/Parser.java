package sample;

import javafx.scene.control.TextArea;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class Parser {

    protected Console console;
    protected Communicator communicator;

    public Parser(Console c, TextArea ta) {
        console = c;
        console.display = ta;
        communicator = new Communicator(console);
    }

    public void parser(String action) {
        String[] args = action.split (" |\\, ");
        switch (args[0]) {
            case "hello" :
                if (args.length == 4 || args.length == 1) {
                    communicator.send(action);
                    communicator.getAswHello();
                }
                else
                    console.display.appendText("< NOK. Usage : 'hello' or 'hello in as ID'" + System.lineSeparator());
                break;
            case "log" :
                if (args.length == 2 && args[1] == "out") {
                    communicator.send(action);
                }
                else
                    console.display.appendText("< NOK. Usage : 'log out'" + System.lineSeparator());
                break;
            case "status" :
                if (args.length == 1) {
                    if (communicator.isConnected()) {
                        console.display.appendText("< OK : Connecte au contrôleur, " + console.aquarium.toString() + System.lineSeparator());
                    }
                    else
                        console.display.appendText("< NOK : Connexion non trouvee."  + console.aquarium.toString() + System.lineSeparator());
                }
                else
                    console.display.appendText("< NOK. The command 'status' doesn't expect arguments." + System.lineSeparator());
                break;
            case "addFish":
                if (args.length == 6) {
                    try {
                        if (!checkMobilityModel(args[5]))
                            console.display.appendText("< NOK : modele de mobilite non supporte" + System.lineSeparator());
                        else {
                            communicator.send(action);
                            communicator.getAswAdd(args[1], Integer.parseInt(args[3].split("x")[0]), Integer.parseInt(args[3].split("x")[1]), Integer.parseInt(args[4].split("x")[0]), Integer.parseInt(args[4].split("x")[1]), args[5]);
                            console.display.appendText("< OK" + System.lineSeparator());
                        }
                    } catch (NumberFormatException e) {
                        console.display.appendText("< NOK ! " + e.getMessage().split("\"")[1] + " is supposed to be an integer." + System.lineSeparator());
                    }
                }
                else
                    console.display.appendText("< NOK. Usage : addFish 'name' at 'x'x'y', 'w'x'h', 'mobilityModel'" + System.lineSeparator());
                break;
            case "startFish":
                if (args.length == 2) {
                    if (console.aquarium.hasFish(args[1])) {
                        communicator.send(action);
                        communicator.getAswStart(args[1]);
                        console.display.appendText("< OK" + System.lineSeparator());
                    }
                    else
                        console.display.appendText("< NOK : Poisson inexistant" + System.lineSeparator());
                }
                else
                    console.display.appendText("< NOK. Usage : 'startFish name'" + System.lineSeparator());
                break;
            case "delFish":
                if (args.length == 2) {
                    if (console.aquarium.hasFish(args[1])) {
                        communicator.send(action);
                        communicator.getAswDel(args[1]);
                        console.display.appendText("< OK" + System.lineSeparator());
                    }
                    else
                        console.display.appendText("< NOK : Poisson inexistant" + System.lineSeparator());
                }
                else
                    console.display.appendText("< Wrong syntax ! Usage : 'delFish name'" + System.lineSeparator());
                break;
            case "getFishes":
                if (args.length == 1) {
                    communicator.send(action);
                    communicator.getAswGoal();
                    console.display.appendText("< OK" + System.lineSeparator());
                }
                else
                    console.display.appendText("< NOK. Usage : 'getFishes'" + System.lineSeparator());
                break;
            case "getFishesContinuously":
                if (args.length == 1) {
                    communicator.send(action);
                    console.display.appendText("< OK" + System.lineSeparator());
                    //continuously = new Thread(new ContinuouslyHandler(inContinuously, aquarium));
                    //continuously.run();
                    System.out.println("DEBUG : ON SORT DE CETTE MERDE");
                }
                else
                    console.display.appendText("< NOK. Usage : 'getFishesContinuously'" + System.lineSeparator());
                break;
            case "setGoal" :
                if (args.length == 5) {
                    if (console.aquarium.hasFish(args[1])) {

                        try {
                            console.aquarium.setGoal(args[1], Integer.parseInt(args[2]), Integer.parseInt(args[3]), Integer.parseInt(args[4]));
                            console.display.appendText("< OK" + System.lineSeparator());
                        }
                        catch (NumberFormatException e) {console.display.appendText("< NOK ! " + e.getMessage().split("\"")[1] + " is supposed to be an integer." + System.lineSeparator());}
                    }
                    else
                        console.display.appendText("< NOK : Poisson inexistant" + System.lineSeparator());
                }
                else
                    console.display.appendText("< NOK. Usage : 'setGoal name x y delay'" + System.lineSeparator());
                break;
            default:
                console.display.appendText("< NOK. Commande introuvable. (Les commandes sont disponible dans l'onglet)." + System.lineSeparator());
                break;
        }
    }

    private boolean checkMobilityModel(String m) {
        try {
            List<String> lines = Files.readAllLines(Paths.get(System.getProperty("user.dir") + "/src/sample/MobilityModel.txt"), Charset.defaultCharset());

            for (String s : lines) {
                System.out.println("DEBUG : dans fichier, ligne :" + s);
                if (m.contentEquals(s))
                    return true;
            }
        } catch (IOException e) {System.out.println("DEBUG : DIDN'T FOUND FILE !");}

        return false;
    }
}
