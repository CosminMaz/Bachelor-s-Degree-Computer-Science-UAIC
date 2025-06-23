package org.example.lab9;

import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.Persistence;

public class DBSingleton {
    private static final EntityManagerFactory emf =
            Persistence.createEntityManagerFactory("myPU");

    public static EntityManagerFactory getEntityManagerFactory() {
        return emf;
    }

    public static void shutdown() {
        emf.close();
    }
}