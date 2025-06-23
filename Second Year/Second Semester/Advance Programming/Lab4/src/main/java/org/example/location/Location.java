package org.example.location;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

@AllArgsConstructor
@Getter
@Setter
public class Location implements  Comparable<Location>{
    private String name;
    private LocationType type;



    @Override
    public int compareTo(Location other) {
        return this.name.compareTo(other.name);
    }

    @Override
    public String toString() {
        return this.name + " ( " + this.type + ") ";
    }

}