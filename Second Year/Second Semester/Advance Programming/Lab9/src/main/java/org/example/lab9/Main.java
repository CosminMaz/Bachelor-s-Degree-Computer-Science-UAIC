package org.example.lab9;

import java.util.List;

public class Main {
    public static void main(String[] args) {
        EntityRepository repo = new EntityRepository();

        repo.create(new MyEntity("Alice"));
        repo.create(new MyEntity("Bob"));
        repo.create(new MyEntity("Alicia"));

        MyEntity found = repo.findById(1L);
        System.out.println("Found by ID: " + found.getName());

        List<MyEntity> matches = repo.findByName("Ali%");
        for (MyEntity e : matches) {
            System.out.println("Matched: " + e.getName());
        }

        DBSingleton.shutdown();
    }
}