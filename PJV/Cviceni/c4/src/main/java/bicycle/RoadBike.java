package bicycle;

/**
 * Created by root on 3/15/17.
 */
public class RoadBike extends Bicycle {
    private int tireWidth;

    public RoadBike(int cadence, int gear, int speed, int tireWidth) {
        super(cadence, gear, speed);
        this.tireWidth = tireWidth;
    }

    @Override
    public void printDescription(){
        System.out.println("Bike is in gear " + super.getGear() + " with a cadence of " + super.getCadence() + " and traveling at speed of " + super.getSpeed() + " tire width: " + this.tireWidth);
    }

    public void visit(BicycleService bs){
        System.out.println("Road going to service");
        bs.accept(this);
    }
}
