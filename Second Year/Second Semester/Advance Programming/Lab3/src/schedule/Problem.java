package schedule;

import airport.Airport;
import airport.Flight;

import java.util.Set;

public abstract class Problem {
    protected Airport airport;
    protected Set<Flight> flights;

    public Problem(Airport air, Set<Flight> flights){
        this.airport = air;
        this.flights = flights;
    }

    public abstract void simpleSchedule();

}
