package org.example;

import lombok.AllArgsConstructor;

import java.sql.*;

@AllArgsConstructor
public class ContinentDAO {
    private Connection conn;

    public void createContinent(Continent continent) throws SQLException {
        String sql = "INSERT INTO continents (name) VALUES (?)";
        try (PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setString(1, continent.getName());
            ps.executeUpdate();
        }
    }

    public Continent findById(int id) throws SQLException {
        String sql = "SELECT * FROM continents WHERE id = ?";
        try (PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setInt(1, id);
            try (ResultSet rs = ps.executeQuery()) {
                if (rs.next()) {
                    return new Continent(
                            rs.getInt("id"),
                            rs.getString("name")
                    );
                }
            }
        }
        return null;
    }

    public Continent findByName(String name) throws SQLException {
        String sql = "SELECT * FROM continents WHERE name = ?";
        try (PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setString(1, name);
            try (ResultSet rs = ps.executeQuery()) {
                if (rs.next()) {
                    return new Continent(
                            rs.getInt("id"),
                            rs.getString("name")
                    );
                }
            }
        }
        return null;
    }
}
