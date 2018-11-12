package bicycle;

/**
 * Created by root on 3/15/17.
 */
public class TestBikes {
    public static void main(String[] args) {
        Bicycle bike1 = new MountainBike(4,50,120,"Dual");
        Bicycle bike2 = new RoadBike(120,10,80,12);
        Bicycle bike3 = new Bicycle(30,2,30);
        bike1.printDescription();
        bike2.printDescription();
        bike3.printDescription();

        System.out.println("__ BS __ \n");
        BicycleService bs = new BicycleService();
        bike1.visit(bs);
        bike2.visit(bs);
        bike3.visit(bs);

        System.out.println("\n\n__ MBS __ \n");
        BicycleService mbs = new MountainBikeService();
        bike1.visit(mbs);
        bike2.visit(mbs);
        bike3.visit(mbs);

        System.out.println("\n\n__ RBS __ \n");
        BicycleService rbs = new RoadBikeService();
        bike1.visit(rbs);
        bike2.visit(rbs);
        bike3.visit(rbs);
    }
}
