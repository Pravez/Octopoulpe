package sample;

import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.SwipeEvent;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import static javafx.scene.input.KeyCode.ENTER;

public class ScenarioWindow extends Stage {

    private Accordion accordion;
    private TextField input;
    private int height;

    public ScenarioWindow(TextField tf) {
        input = tf;

        accordion = new Accordion();
        createPanes();

        final StackPane root = new StackPane();
        root.getChildren().add(accordion);
        height = accordion.getPanes().size()*40>300?accordion.getPanes().size()*40:300;
        final Scene scene = new Scene(root, 300, height);
        this.setScene(scene);
        this.setTitle("Scenario");
    }

    private void createPanes() {
        try {
            List<String> lines = Files.readAllLines(Paths.get(System.getProperty("user.dir") + "scenarios.txt"), StandardCharsets.UTF_8);
            for (int i = 0; i < lines.size(); ++i) {
                if (lines.get(i).charAt(0) == '>') {
                    String title = lines.get(i).substring(2, lines.get(i).length());
                    String steps = "";
                    i++;
                    while (i < lines.size() && lines.get(i).charAt(0) != '>') {
                        steps += lines.get(i) + "\n";
                        i++;
                    }
                    createPane(title, steps);
                    i--;
                }
            }

        } catch (IOException e) {
            System.out.println("Exception when opening scenarios");
        }
    }

    private void createPane(String title, String steps) {
        ScenarioWindow w = this;
        VBox vb = new VBox();
        Button b = createButton(steps);
        Label l = new Label(steps);
        vb.getChildren().addAll(l, b);

        TitledPane tp = new TitledPane(title, vb);
        tp.setOnMouseClicked(new EventHandler<MouseEvent>() {

            public void handle(MouseEvent e) {
                if (tp.isExpanded()) {
                    int nbLine = steps.split("\n").length;
                    w.setHeight(300 + nbLine * 10);
                }
                else
                    w.setHeight(height);
            }
        });
        accordion.getPanes().add(tp);
    }

    private Button createButton(String steps) {
        ScenarioWindow w = this;
        Button b = new Button("Lancer scenario");
        String[] step = steps.split("\n");
        b.setOnMouseClicked(new EventHandler<MouseEvent>() {
            public void handle(MouseEvent e) {

                    new Thread(new Runnable() {
                      @Override
                      public void run() {
                          for (String s: step ) {
                              input.setText(s);
                              input.fireEvent(new KeyEvent(KeyEvent.KEY_RELEASED, "Enter", "Key Enter Pressed", ENTER, false, false, false, false));
                              try {
                                  Thread.sleep(2000);
                              } catch (InterruptedException exception) {
                                  System.out.println("Exception sleeping : " + exception.toString());
                              }
                          }
                      }
                    }).start();
                w.close();
            }
        });
        return b;
    }
}