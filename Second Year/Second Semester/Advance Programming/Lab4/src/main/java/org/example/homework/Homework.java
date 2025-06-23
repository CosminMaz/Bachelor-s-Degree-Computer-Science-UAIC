package org.example.homework;

import com.github.javafaker.Faker;
import org.example.location.Location;
import org.example.location.LocationType;
import org.jgrapht.Graph;
import org.jgrapht.alg.shortestpath.DijkstraShortestPath;
import org.jgrapht.graph.DefaultWeightedEdge;
import org.jgrapht.graph.SimpleWeightedGraph;

import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Homework {
    private final List<Location> locations;
    private final Graph<Location, DefaultWeightedEdge> graph;
    private final Map<LocationPair, RouteStatistics> routeStatisticsMap;

    // Data structure to store test results
    private final List<TestResult> testResults = new ArrayList<>();

    public Homework() {
        this(10); // Default to 10 locations
    }

    public Homework(int numberOfLocations) {
        Faker faker = new Faker();
        locations = new ArrayList<>();
        Random random = new Random();

        // Generate random locations
        for (int i = 0; i < numberOfLocations; i++) {
            locations.add(new Location(faker.address().cityName(), LocationType.values()[random.nextInt(3)]));
        }

        // Initialize graph
        graph = new SimpleWeightedGraph<>(DefaultWeightedEdge.class);
        locations.forEach(graph::addVertex);

        // Add random weighted edges
        for (int i = 0; i < locations.size() - 1; i++) {
            for (int j = i + 1; j < locations.size(); j++) {
                if (random.nextBoolean()) { // Decide if there's a vertex
                    DefaultWeightedEdge edge = graph.addEdge(locations.get(i), locations.get(j));
                    graph.setEdgeWeight(edge, 1 + random.nextDouble() * 10); // Random travel time
                }
            }
        }

        // Initialize route statistics map
        routeStatisticsMap = new HashMap<>();
        computeAllRouteStatistics();
    }

    // Helper class to store location pairs
    private static class LocationPair {
        private final Location source;
        private final Location destination;

        public LocationPair(Location source, Location destination) {
            this.source = source;
            this.destination = destination;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            LocationPair that = (LocationPair) o;
            return Objects.equals(source, that.source) &&
                    Objects.equals(destination, that.destination);
        }

        @Override
        public int hashCode() {
            return Objects.hash(source, destination);
        }
    }

    // Class to store route statistics
    public static class RouteStatistics {
        private final int friendlyCount;
        private final int enemyCount;
        private final int neutralCount;
        private final double totalWeight;

        public RouteStatistics(int friendlyCount, int enemyCount, int neutralCount, double totalWeight) {
            this.friendlyCount = friendlyCount;
            this.enemyCount = enemyCount;
            this.neutralCount = neutralCount;
            this.totalWeight = totalWeight;
        }

        @Override
        public String toString() {
            return String.format("Friendly: %d, Enemy: %d, Neutral: %d, Total Weight: %.2f",
                    friendlyCount, enemyCount, neutralCount, totalWeight);
        }
    }

    // Class to store test results
    private static class TestResult {
        private final int numberOfLocations;
        private final long computationTimeMs;
        private final int totalRoutesComputed;

        
        public TestResult(int numberOfLocations, long computationTimeMs, int totalRoutesComputed) {
            this.numberOfLocations = numberOfLocations;
            this.computationTimeMs = computationTimeMs;
            this.totalRoutesComputed = totalRoutesComputed;
        }
    }

    private void computeAllRouteStatistics() {
        DijkstraShortestPath<Location, DefaultWeightedEdge> dijkstra = new DijkstraShortestPath<>(graph);

        for (int i = 0; i < locations.size(); i++) {
            for (int j = i + 1; j < locations.size(); j++) {
                Location source = locations.get(i);
                Location destination = locations.get(j);

                var path = dijkstra.getPath(source, destination);
                if (path != null) {
                    int friendly = 0;
                    int enemy = 0;
                    int neutral = 0;

                    for (Location loc : path.getVertexList()) {
                        switch (loc.getType()) {
                            case FRIENDLY -> friendly++;
                            case ENEMY -> enemy++;
                            case NEUTRAL -> neutral++;
                        }
                    }

                    RouteStatistics stats = new RouteStatistics(
                            friendly, enemy, neutral, path.getWeight());

                    routeStatisticsMap.put(new LocationPair(source, destination), stats);
                }
            }
        }
    }

    public void printRouteStatistics() {
        System.out.println("\nRoute Statistics:");
        routeStatisticsMap.forEach((pair, stats) ->
                System.out.printf("%s -> %s: %s%n",
                        pair.source, pair.destination, stats));
    }

    public void runLargeScaleTests() {
        int[] sizes = {100, 200};

        for (int size : sizes) {
            long startTime = System.currentTimeMillis();

            // Create a large problem instance
            Homework largeInstance = new Homework(size);

            // Compute all route statistics (done in constructor)
            long endTime = System.currentTimeMillis();

            int totalRoutes = largeInstance.routeStatisticsMap.size();
            testResults.add(new TestResult(size, endTime - startTime, totalRoutes));

            System.out.printf("Test completed for %d locations. Time: %d ms, Routes computed: %d%n",
                    size, endTime - startTime, totalRoutes);
        }

        printTestStatistics();
    }

    private void printTestStatistics() {
        System.out.println("\nTest Statistics:");

        System.out.println("\nAverage computation time per location:");
        testResults.stream()
                .collect(Collectors.groupingBy(
                        result -> result.numberOfLocations,
                        Collectors.averagingLong(result -> result.computationTimeMs)
                ))
                .forEach((size, avgTime) ->
                        System.out.printf("%d locations: %.2f ms%n", size, avgTime));

        System.out.println("\nRoutes computed per location:");
        testResults.stream()
                .collect(Collectors.groupingBy(
                        result -> result.numberOfLocations,
                        Collectors.averagingDouble(result -> (double)result.totalRoutesComputed / result.numberOfLocations)
                ))
                .forEach((size, avgRoutes) ->
                        System.out.printf("%d locations: %.2f routes per location%n", size, avgRoutes));

        System.out.println("\nPerformance trend:");
        double trend = IntStream.range(0, testResults.size() - 1)
                .mapToDouble(i -> (double)testResults.get(i + 1).computationTimeMs / testResults.get(i).computationTimeMs)
                .average()
                .orElse(0);
        System.out.printf("On average, each size increase leads to %.2fx computation time increase%n", trend);
    }

    // Existing methods remain unchanged
    public void printSortedLocations() {
        TreeSet<Location> neutralLocations = locations.stream()
                .filter(loc -> loc.getType() == LocationType.NEUTRAL)
                .collect(Collectors.toCollection(TreeSet::new));

        System.out.println("Neutral Locations:");
        neutralLocations.forEach(System.out::println);

        TreeSet<Location> friendlyLocations = locations.stream()
                .filter(loc -> loc.getType() == LocationType.FRIENDLY)
                .collect(Collectors.toCollection(TreeSet::new));

        System.out.println("\nFriendly Locations:");
        friendlyLocations.forEach(System.out::println);

        LinkedList<Location> enemyLocations = locations.stream()
                .filter(loc -> loc.getType() == LocationType.ENEMY)
                .sorted(Comparator.comparing(Location::getType).thenComparing(Location::getName))
                .collect(Collectors.toCollection(LinkedList::new));

        System.out.println("\nEnemy Locations:");
        enemyLocations.forEach(System.out::println);
    }

    public void computeShortestPaths() {
        Location start = locations.getFirst();
        DijkstraShortestPath<Location, DefaultWeightedEdge> dijkstra = new DijkstraShortestPath<>(graph);
        System.out.println("\nShortest paths from " + start + ":");
        locations.forEach(destination -> {
            if (!start.equals(destination)) {
                System.out.println("To " + destination + ": " + dijkstra.getPathWeight(start, destination));
            }
        });
    }

    public static void main(String[] args) {
        Homework homework = new Homework();
        homework.printSortedLocations();
        homework.computeShortestPaths();
        homework.printRouteStatistics();

        // Run large scale tests
        homework.runLargeScaleTests();
    }
}