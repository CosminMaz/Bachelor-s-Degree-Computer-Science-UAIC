package schedule;

import airport.Airport;
import airport.Flight;
import airport.Runway;

import java.util.*;

public class Solution extends Problem {
    private Map<Runway, Flight>[] schedule;
    private int added = 0;

    public Solution(Airport air, Set<Flight> flights){
        super(air, flights);
        this.schedule = new HashMap[flights.size() + 1];
        for (int i = 0; i <= flights.size(); i++) {
            this.schedule[i] = new HashMap<>();
        }
    }

    public void simpleSchedule(){

        for(Flight flight : this.flights){
            boolean accepted = false;
            for(Runway run : this.airport.getRunways()){
                boolean found = false;
                for(Flight f : run.getFlights()){
                    if(flight.landingInterval[0].compareTo(f.landingInterval[0]) > 0 && flight.landingInterval[0].compareTo(f.landingInterval[1]) < 0){
                        found = true;
                    }
                    if(flight.landingInterval[1].compareTo(f.landingInterval[0]) > 0 && flight.landingInterval[1].compareTo(f.landingInterval[1]) < 0){
                        found = true;
                    }
                }
                if(!found){
                    accepted = true;
                    if (this.added <= flights.size()) {
                        this.schedule[this.added].put(run, flight);
                        this.added++;
                    }
                }
            }
            if(!accepted){
                System.out.println("Can't find a solution");
                return;
            }
        }
        System.out.println("Can find a solution");
    }


    //*****************

    public void optimazedSchedule() {
        List<Runway> runways = new ArrayList<>(airport.getRunways());
        List<Flight> flightList = new ArrayList<>(flights);

        if (runways.isEmpty()) {
            System.out.println("No runways available.");
            return;
        }

        // Sort flights by earliest landing time
        flightList.sort(Comparator.comparing(f -> f.landingInterval[0]));

        // Assign flights in a round-robin manner to balance load
        int runwayIndex = 0;
        for (int i = 0; i < flightList.size(); i++) {
            Flight flight = flightList.get(i);
            schedule[i].put(runways.get(runwayIndex), flight);
            runwayIndex = (runwayIndex + 1) % runways.size();
        }

        // Check if scheduling is equitable
        int minFlights = Integer.MAX_VALUE, maxFlights = Integer.MIN_VALUE;
        Map<Runway, Integer> runwayCount = new HashMap<>();

        for (Map<Runway, Flight> map : schedule) {
            for (Runway r : map.keySet()) {
                runwayCount.put(r, runwayCount.getOrDefault(r, 0) + 1);
            }
        }

        for (int count : runwayCount.values()) {
            minFlights = Math.min(minFlights, count);
            maxFlights = Math.max(maxFlights, count);
        }

        if (maxFlights - minFlights > 1) {
            System.out.println("Equitable scheduling not possible with current runways.");
            int requiredRunways = (int) Math.ceil((double) flights.size() / minFlights);
            System.out.println("Minimum additional runways needed: " + (requiredRunways - runways.size()));
        } else {
            System.out.println("Equitable scheduling successful.");
            printSchedule();
        }
    }

    private void printSchedule() {
        for (int i = 0; i < schedule.length; i++) {
            if (schedule[i] != null && !schedule[i].isEmpty()) {
                System.out.println("Schedule " + i + ": " + schedule[i]);
            }
        }
    }



}
