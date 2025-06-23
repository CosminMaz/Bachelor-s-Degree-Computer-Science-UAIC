package org.example.lab11.dbs;

import java.sql.Connection;
import java.sql.SQLException;


import com.zaxxer.hikari.HikariConfig;
import com.zaxxer.hikari.HikariDataSource;

public class DBSingleton {
    private static DBSingleton instance;
    private final HikariDataSource dataSource;

    private DBSingleton() {
        HikariConfig config = new HikariConfig();
        config.setJdbcUrl("jdbc:postgresql://localhost:5432/postgres"); // numele bazei tale
        config.setUsername("postgres");
        config.setPassword("andrei");

        // Optional: configurări pentru performanță
        config.setMaximumPoolSize(10);
        config.setMinimumIdle(2);
        config.setIdleTimeout(30000);
        config.setConnectionTimeout(30000);
        config.setMaxLifetime(1800000);

        dataSource = new HikariDataSource(config);
        System.out.println("HikariCP init complet.");
    }

    public static DBSingleton getInstance() {
        if (instance == null) {
            instance = new DBSingleton();
        }
        return instance;
    }

    public Connection getConnection() throws SQLException {
        return dataSource.getConnection();
    }
}
