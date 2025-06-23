package airport;

import aircrafts.Aircraft;

import java.time.LocalTime;

public class Flight {
    public LocalTime[] landingInterval;
    private Aircraft aircraft;

    public Flight(LocalTime[] localTimes, Aircraft air){
        if(localTimes.length != 2){
            System.out.println("Wrong Intervals");
            return;
        }

        if(localTimes[0].isAfter(localTimes[1])){
            System.out.println("Wrong Intervals");
            return;
        }
        this.landingInterval = localTimes;
        this.aircraft = air;
    }

}
