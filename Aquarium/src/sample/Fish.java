package sample;

import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

import java.awt.*;
import java.net.URL;

/**
 * Created by nicvidal on 23/04/17.
 */
public class Fish {

    private ImageView view1;
    private ImageView view2;

    private int x;
    private int y;

    private Point goal;
    //private int height;
    //private int width;

    public Fish(int x, int y, int w, int h, String s) {
        //width = w;
        //height = h;
        this.x = x;
        this.y = y;
        goal = new Point(-1, -1);

        int index = s.indexOf('_');
        String pictureFile = (index == -1) ? new String(s) : new String(s.substring(0, index)) ;

        final URL url1 = getClass().getResource( pictureFile + ".png");
        final URL url2 = getClass().getResource(pictureFile + "2.png");
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
