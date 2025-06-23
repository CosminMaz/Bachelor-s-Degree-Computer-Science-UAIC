package aircrafts;

import capable.CargoCapable;
import capable.PassengerCapable;

public abstract class Aircraft implements PassengerCapable, CargoCapable {
    private final String tailNumber;
    private final String model;

    public Aircraft(String number, String model){
        this.tailNumber = number;
        this.model = model;
    }

    public String getTailNumber(){
        return this.tailNumber;
    }

    public String getModel() {
        return this.model;
    }

}
