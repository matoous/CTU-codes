package bicycle;

import javax.xml.transform.Source;
import java.net.SocketPermission;

/**
 * Created by root on 3/15/17.
 */
public class Bicycle {
    public int getCadence() {
        return cadence;
    }

    public void setCadence(int cadence) {
        this.cadence = cadence;
    }

    public int getGear() {
        return gear;
    }

    public void setGear(int gear) {
        this.gear = gear;
    }

    public int getSpeed() {
        return speed;
    }

    public void setSpeed(int speed) {
        this.speed = speed;
    }

    private int cadence;
    private int gear;
    private int speed;

    public void printDescription(){
        System.out.println("Bike is in gear " + this.gear + " with a cadence of " + this.cadence + " and traveling at speed of " + this.speed);
    }

    public Bicycle(int cadence, int gear, int speed){
        this.cadence = cadence;
        this.gear = gear;
        this.speed = speed;
    }

    public void visit(BicycleService bs){
        System.out.println("Going to service");
        bs.accept(this);
    }
}
