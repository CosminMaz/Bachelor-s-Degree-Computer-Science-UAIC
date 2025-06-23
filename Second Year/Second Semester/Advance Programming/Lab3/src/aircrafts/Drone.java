package aircrafts;

public class Drone extends Aircraft {
    final private int batteryCapacity;

    public Drone(String number, String model, int batteryCapacity){
        super(number, model);
        this.batteryCapacity = batteryCapacity;
    }

    public int getBatteryCapacity(){
        return this.batteryCapacity;
    }

    @Override
    public boolean isCargoCapable() {
        return true;
    }

    @Override
    public boolean isPassengerCapable(){
        return false;
    }

}
