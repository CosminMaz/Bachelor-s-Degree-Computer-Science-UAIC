package main;

import aircrafts.Aircraft;
import aircrafts.Airliner;
import aircrafts.Drone;
import aircrafts.Freighter;
import airport.Airport;
import airport.Flight;
import airport.Runway;
import schedule.Solution;

import java.time.LocalTime;
import java.time.chrono.ChronoLocalDateTime;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Main {
    public static void main(String[] args) {
        //Set<Aircraft> aircraftSet = new HashSet<>();
        Airliner airliner1 = new Airliner("r123", "123", 43);
        /*
        if(airliner1.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Airliner airliner2 = new Airliner("r12233", "1223", 45);
        /*
        if(airliner2.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Airliner airliner3 = new Airliner("r1245343", "1323", 433);

        /*
        if(airliner3.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Airliner airliner4 = new Airliner("r153423", "1213", 413);
        /*
        if(airliner4.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Freighter freighter1 = new Freighter("f12312", "23", 32);
        /*
        if(freighter1.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Freighter freighter2 = new Freighter("f12312", "23", 32);
        /*
        if(freighter2.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
         Freighter freighter3 = new Freighter("f12312", "23", 32);
        /*
         if(freighter3.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Freighter freighter4 = new Freighter("f12312", "23", 32);
        /*
        if(freighter4.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Drone drone1 = new Drone("2131", "76", 2);
        /*
        if(drone1.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Drone drone2 = new Drone("2134", "79", 5);
        /*
        if(drone2.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Drone drone3 = new Drone("2135", "78", 4);
        /*
        if(drone3.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        Drone drone4 = new Drone("2138", "77", 3);
        /*
        if(drone4.isCargoCapable()){
            aircraftSet.add(airliner1);
        }
        */
        LocalTime[] timeList = new LocalTime[2];
        timeList[0] = LocalTime.of(9,30);
        timeList[1] = LocalTime.of(11, 5);
        Flight flight1 = new Flight(timeList, drone1);

        timeList[0] = LocalTime.of(12,30);
        timeList[1] = LocalTime.of(13, 5);
        Flight flight2 = new Flight(timeList, drone1);

        timeList[0] = LocalTime.of(14,30);
        timeList[1] = LocalTime.of(15, 5);
        Flight flight3 = new Flight(timeList, drone1);


        Runway runway1 = new Runway(1);
        Runway runway2 = new Runway(2);
        Runway runway3 = new Runway(3);
        Runway runway4 = new Runway(4);

        Airport airport = new Airport(new Runway[]{runway1, runway2, runway3, runway4});

        Set<Flight> flightSet = new HashSet<>();
        flightSet.add(flight1);
        flightSet.add(flight2);
        flightSet.add(flight3);
        Solution solution = new Solution(airport, flightSet);

        //solution.simpleSchedule();
        solution.optimazedSchedule();
    }

}