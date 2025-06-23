package airport;

import java.util.ArrayList;
import java.util.List;

public class Airport {
    private List<Flight> flights = new ArrayList<>();
    private List<Runway> runways = new ArrayList<>();

    public Airport(Runway[] run){
        this.runways = List.of(run);
    }

    public List<Runway> getRunways(){
        return this.runways;
    }
}
