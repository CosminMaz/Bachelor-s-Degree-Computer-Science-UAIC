package org.example.main;

import com.github.javafaker.Faker;
import org.example.homework.Homework;
import org.example.location.Location;
import org.example.location.LocationType;

import java.util.*;
import java.util.stream.Collectors;

public class  Main {
    public static void main(String[] args) {

        /*
        Location location1 = new Location("Location1", LocationType.FRIENDLY);
        Location location2 = new Location("Location2", LocationType.ENEMY);
        Location location3 = new Location("Location3", LocationType.NEUTRAL);
        Location location4 = new Location("Location4", LocationType.FRIENDLY);
        Location location5 = new Location("Location5", LocationType.FRIENDLY);
        Location location6 = new Location("Location6", LocationType.ENEMY);

        List<Location> locations = Arrays.asList(location1, location2, location3, location4, location5, location6);

        TreeSet<Location> friendlyLocations = locations.stream()
                .filter(loc -> loc.getType() == LocationType.FRIENDLY)
                .collect(Collectors.toCollection(TreeSet::new));

        System.out.println("Friendly locations");
        friendlyLocations.forEach(System.out::println);

        LinkedList<Location> enemyLocations = locations.stream()
                .filter(loc -> loc.getType() == LocationType.ENEMY)
                .sorted(Comparator.comparing(Location::getType).thenComparing(Location::getName))
                .collect(Collectors.toCollection(LinkedList::new));

        System.out.println("\nEnemy locations");
        enemyLocations.forEach(System.out::println);
        */

        System.out.println("--------------------------------");

        Homework robotPathfinding = new Homework();
        robotPathfinding.printSortedLocations();
        robotPathfinding.computeShortestPaths();
        robotPathfinding.runLargeScaleTests();



    }
}
