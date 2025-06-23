package org.example;

import lombok.AllArgsConstructor;

import java.sql.*;

@AllArgsConstructor
public class CityDAO {
    private Connection con;

    public void createCity(City city) throws SQLException {
        String insert = "INSERT INTO cities (name, country, capital, latitude, longitude) VALUES (?, ?, ?, ?, ?)";
        try (PreparedStatement ps = con.prepareStatement(insert)) {
            ps.setString(1, city.getName());
            ps.setString(2, city.getCountry());
            ps.setBoolean(3, city.isCapital());
            ps.setDouble(4, city.getLatitude());
            ps.setDouble(5, city.getLongitude());
            ps.executeUpdate();
        }
    }

    public City findByID(int id) throws SQLException {
        String select = "SELECT * FROM cities WHERE id = ?";
        try (PreparedStatement ps = con.prepareStatement(select)) {
            ps.setInt(1, id);
            try (ResultSet rs = ps.executeQuery()) {
                if (rs.next()) {
                    return new City(
                    rs.getInt("id"),
                    rs.getString("name"),
                    rs.getString("country"),
                    rs.getBoolean("capital"),
                    rs.getDouble("latitude"),
                    rs.getDouble("longitude")
                    );
                }
            }
        }
        return null;
    }
}
