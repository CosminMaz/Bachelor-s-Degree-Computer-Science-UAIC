package org.example.lab11.dbs;

import lombok.AllArgsConstructor;
import lombok.Data;

@AllArgsConstructor
@Data
public class City {
    private int id;
    private String name;
    private String country;
    private boolean capital;
    private double latitude;
    private double longitude;
}
