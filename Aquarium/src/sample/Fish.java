package sample;

import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

import java.awt.*;
import java.io.File;
import java.net.URL;

public class Fish {

    private ImageView view1;
    private ImageView view2;

    private int x;
    private int y;

    private String name;

    private Point goal;

    public Fish(int x, int y, int w, int h, String s) {
        this.x = x;
        this.y = y;
        name = s;
        goal = new Point(-1, -1);

        initImage(s, w, h);
    }

    private String[] listImage(){
        File directory = new File(  System.getProperty("user.dir") + "/src/sample/Images/");
        System.out.println("DEBUG : Directory " + directory.toString() + " exists : " + directory.exists());
        String[] listFiles = directory.list();

        for(int i=0;i<listFiles.length;i++) {
            listFiles[i] = listFiles[i].substring(0, listFiles[i].indexOf("."));
            System.out.println("DEBUG : fichier : " + listFiles[i]);
        }
        return listFiles;
    }

    private void initImage(String s, int w, int h) {
        int index = s.indexOf('_');
        String pictureFile = (index == -1) ? new String(s) : new String(s.substring(0, index));
        Boolean exist = false;
        System.out.println("DEBUG : FIIIILE : " + pictureFile);
        String [] listFiles = listImage();
        for (String f: listFiles) {
            System.out.println("DEBUG : file in directory : " + f);
             if (f.equalsIgnoreCase(pictureFile)) {
              System.out.println("DEBUG : NO EQUAAAL");
              exist = true;
             }
        }
        if (! exist)
            pictureFile = new String("poulpy"); //Default image

        System.out.println("DEBUG : Images/" + pictureFile + ".png");

        final URL url1 = getClass().getResource( "Images/" + pictureFile + ".png");
        final URL url2 = getClass().getResource("Images/" + pictureFile + "2.png");
        final Image img1 = new Image(url1.toExternalForm());
        final Image img2 = new Image(url2.toExternalForm());

        view1 =  new ImageView(img1);
        view2 =  new ImageView(img2);
        view1.setFitHeight(h);
        view1.setFitWidth(w);
        view2.setFitHeight(h);
        view2.setFitWidth(w);
    }

    public void setPosition(int new_x, int new_y) {
        x = new_x;
        y = new_y;

        view1.setX(x);
        view1.setY(y);
        view2.setX(x);
        view2.setY(y);
    }

    public String getName() {
        return name;
    }

    public void setGoal(int x, int y) {
        goal.x = x;
        goal.y = y;
    }

    public ImageView get_View(int nb) {
        if (nb == 0)
            return view1;
        else
            return view2;
    }

    public void update(int timer) {
        if (! goal.equals(new Point(-1,-1))) {
            if (goal.x > x)
                x++;
            else if (goal.x < x)
                x--;

            if (goal.y > y)
                y++;
            else if (goal.y < y)
                y--;

            setPosition(x, y);

            if (goal.equals(new Point(x, y))) {
                goal.x = -1;
                goal.y = -1;
            }
        }
    }
}
