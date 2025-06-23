package org.example.lab11.dbs;

import java.io.BufferedReader;
import java.io.FileReader;
import java.sql.Connection;

public class CityImporter {

    private Connection conn;

    public CityImporter(Connection conn) {
        this.conn = conn;
    }

    public void importFromCSV(String filepath) {
        try(BufferedReader br = new BufferedReader(new FileReader(filepath))) {
            String line = br.readLine();
            CityDAO cityDAO = new CityDAO(conn);

            while((line = br.readLine()) != null) {
                String[] parts = line.split(",", -1);

                String name = parts[1];
                String country = parts[0];
                boolean capital = true;
                double latitude = Double.parseDouble(parts[2]);
                double longitude = Double.parseDouble(parts[3]);
                City city = new City(0, name, country, capital, latitude, longitude);
                cityDAO.createCity(city);
            }

            System.out.println("Import complet!");

        }catch (Exception e) {
            e.printStackTrace();
        }
    }

}
