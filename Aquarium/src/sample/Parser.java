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
    public Thread continuously;

    public Parser(Console c, TextArea ta) {
        console = c;
        console.display = ta;
        communicator = new Communicator(console);
    }

    synchronized public void parser(String action) {
        String[] args = action.split (" |\\, ");
        switch (args[0]) {
            case "hello" :
                if (args.length == 4 || args.length == 1) {
                    communicator.send(action);
                }
                else
                    console.display.appendText("< NOK. Usage : 'hello' or 'hello in as ID'" + System.lineSeparator());
                break;
            case "log" :
                if (args.length == 2 && args[1].contentEquals("out")) {
                    communicator.send(action);
                }
                else
                    console.display.appendText("< NOK. Usage : 'log out'" + System.lineSeparator());
                break;
            case "status" :
                if (args.length == 1) {
                    if (communicator.isConnected()) {
                        console.display.appendText("< OK : Connecte au controleur, " + console.aquarium.toString());
                    }
                    else
                        console.display.appendText("< NOK : Connexion non trouvee."  + console.aquarium.toString());
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
                            //console.addFishToHandle(args[1], Integer.parseInt(args[3].split("x")[0]), Integer.parseInt(args[3].split("x")[1]), Integer.parseInt(args[4].split("x")[0]), Integer.parseInt(args[4].split("x")[1]));
                            communicator.handler.addToHistory(action);
                            //while (!communicator.orderHistory.contains(action));
                            communicator.send(action);
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
                        communicator.handler.addToHistory(action);
                        communicator.send(action);
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
                        communicator.handler.addToHistory(action);
                        communicator.send(action);
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
                }
                else
                    console.display.appendText("< NOK. Usage : 'getFishes'" + System.lineSeparator());
                break;
            case "getFishesContinuously":
                if (args.length == 1) {
                    communicator.send(action);
                }
                else
                    console.display.appendText("< NOK. Usage : 'getFishesContinuously'" + System.lineSeparator());
                break;
            default:
                console.display.appendText("< NOK. Commande introuvable. (Les commandes sont disponible dans l'onglet)." + System.lineSeparator());
                break;
        }
    }

    private boolean checkMobilityModel(String m) {
        try {
            List<String> lines = Files.readAllLines(Paths.get(System.getProperty("user.dir") + "MobilityModels.txt"), Charset.defaultCharset());

            for (String s : lines) {
                if (m.contentEquals(s))
                    return true;
            }
        } catch (IOException e) {System.out.println("Exception in searching file : " + e.toString()); console.aquarium.writeLogs("Exception lors de l'ouverture du fichier de modeles de mobilite.\n");}

        return false;
    }
}
