package aircrafts;

public class Freighter extends Aircraft {
    final private int maximumPayLoad;

    public Freighter(String number, String model, int maximumPayLoad){
        super(number, model);
        this.maximumPayLoad = maximumPayLoad;
    }

    public int getMaximumPayLoad(){
        return this.maximumPayLoad;
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
