package org.example;

import lombok.AllArgsConstructor;

import java.sql.*;

@AllArgsConstructor
public class CountryDAO {
    private Connection conn;

    public void createCountry(Country country) throws SQLException {
        String insert = "INSERT INTO countries (name, code, continent) VALUES (?, ?, ?)";
        try (PreparedStatement ps = conn.prepareStatement(insert)) {
            ps.setString(1, country.getName());
            ps.setString(2, country.getCode());
            ps.setString(3, country.getContinent());
            ps.executeUpdate();
        }
    }

    public Country findById(int id) throws SQLException {
        String select = "SELECT * FROM countries WHERE id = ?";
        try (PreparedStatement ps = conn.prepareStatement(select)) {
            ps.setInt(1, id);
            try (ResultSet rs = ps.executeQuery()) {
                if (rs.next()) {
                    return new Country(
                            rs.getInt("id"),
                            rs.getString("name"),
                            rs.getString("code"),
                            rs.getString("continent")
                    );
                }
            }
        }
        return null;
    }

    public Country findByName(String name) throws SQLException {
        String select = "SELECT * FROM countries WHERE name = ?";
        try (PreparedStatement ps = conn.prepareStatement(select)) {
            ps.setString(1, name);
            try (ResultSet rs = ps.executeQuery()) {
                if (rs.next()) {
                    return new Country(
                            rs.getInt("id"),
                            rs.getString("name"),
                            rs.getString("code"),
                            rs.getString("continent")
                    );
                }
            }
        }
        return null;
    }
}
