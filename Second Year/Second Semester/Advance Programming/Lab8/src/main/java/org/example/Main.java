package org.example;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;


public class Main {
    public static void main(String[] args) {
        try (Connection conn = DBSingleton.getInstance().getConnection();
             Statement stmt = conn.createStatement()) {

            String createTableContinents = """
                CREATE TABLE IF NOT EXISTS continents (
                    id SERIAL PRIMARY KEY,
                    name VARCHAR(50)
                )
                """;

            String createTableCountries = """
                CREATE TABLE IF NOT EXISTS countries (
                    id SERIAL PRIMARY KEY,
                    name VARCHAR(50),
                    code VARCHAR(50),
                    continent VARCHAR(50)
                )
                """;

            String createTableCities= """
                    CREATE TABLE IF NOT EXISTS cities (
                    id SERIAL PRIMARY KEY,
                    country VARCHAR(50),
                    name VARCHAR(50),
                    capital BOOLEAN,
                    latitude DOUBLE PRECISION,
                    longitude DOUBLE PRECISION
                    )
                    """;

            stmt.execute(createTableContinents);
            System.out.println("Continents table created");

            stmt.execute(createTableCountries);
            System.out.println("Countries table created");

            stmt.execute(createTableCities);
            System.out.println("Cities table created");

            CountryDAO countryDAO = new CountryDAO(conn);
            ContinentDAO continentDAO = new ContinentDAO(conn);

            Continent europa = new Continent(0, "Europa");
            continentDAO.createContinent(europa);

            Country romania = new Country(0, "Romania", "RO", "Europa");
            countryDAO.createCountry(romania);

            Continent findContinent = continentDAO.findById(1);
            System.out.println("Continent: " + findContinent);

            Country findCountry = countryDAO.findById(1);
            System.out.println("Country: " + findCountry);

            CityImporter importer = new CityImporter(conn);
            importer.importFromCSV("concap.csv");

            String selectRandomCities = "SELECT id, name, latitude, longitude FROM cities ORDER BY RANDOM() LIMIT 2";

            ResultSet rs = stmt.executeQuery(selectRandomCities);

            double lat1 = 0, lon1 = 0, lat2 = 0, lon2 = 0;

            if(rs.next()) {
                lat1 = rs.getDouble("latitude");
                lon1 = rs.getDouble("longitude");
            }

            if(rs.next()) {
                lat2 = rs.getDouble("latitude");
                lon2 = rs.getDouble("longitude");
            }

            double lat1Rad = Math.toRadians(lat1);
            double lon1Rad = Math.toRadians(lon1);
            double lat2Rad = Math.toRadians(lat2);
            double lon2Rad = Math.toRadians(lon2);

            double deltaLat = lat2Rad - lat1Rad;
            double deltaLon = lon2Rad - lon1Rad;

            double a = Math.sin(deltaLat / 2) * Math.sin(deltaLat / 2) +
                    Math.cos(lat1Rad) * Math.cos(lat2Rad) *
                            Math.sin(deltaLon / 2) * Math.sin(deltaLon / 2);

            double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

            System.out.println("Distanta este : " + c * 6371);


        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}