package bicycle;

/**
 * Created by root on 3/15/17.
 */
public class MountainBike extends Bicycle {
    String suspension;

    public MountainBike(int gear, int cadence, int speed, String suspension){
        super(cadence, gear, speed);
        this.suspension = suspension;
    }

    @Override
    public void printDescription(){
        System.out.println("Bike is in gear " + super.getGear() + " with a cadence of " + super.getCadence() + " and traveling at speed of " + super.getSpeed() + " suspension " + this.suspension);
    }

    public void visit(BicycleService bs){
        System.out.println("MB going to service");
        bs.accept(this);
    }
}
