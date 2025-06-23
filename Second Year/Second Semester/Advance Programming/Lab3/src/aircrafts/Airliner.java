package aircrafts;

public class Airliner extends Aircraft{
    final private int numberOfPassengers;

    public Airliner(String number, String model, int passengers){
        super(number, model);
        this.numberOfPassengers = passengers;
    }

    public int getNumberOfPassengers(){
        return this.numberOfPassengers;
    }

    @Override
    public boolean isCargoCapable() {
        return false;
    }

    @Override
    public boolean isPassengerCapable() {
        return true;
    }
}
