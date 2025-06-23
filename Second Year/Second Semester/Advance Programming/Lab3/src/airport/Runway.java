package airport;

import java.util.ArrayList;
import java.util.List;

public class Runway {
    private int number;
    private List<Flight> flights = new ArrayList<>();

    public Runway(int n){
        this.number = n;
    }

    public int getNumber() {
        return this.number;
    }

    public List<Flight> getFlights() {
        return this.flights;
    }
}
