package bicycle;

/**
 * Created by root on 3/15/17.
 */
public class BicycleService {

    public BicycleService(){}

    public void accept(Bicycle b){
        System.out.println("Fixing bike");
    }

    void accept(MountainBike b){
        System.out.println("Fixing bike");
    }

    void accept(RoadBike b){
        System.out.println("Fixing bike");
    }
}
