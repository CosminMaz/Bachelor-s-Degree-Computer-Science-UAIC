package org.example.lab11.dbs;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

@AllArgsConstructor
@Getter
@Setter
public class Country {
    private int id;
    private String name;
    private String code;
    private String continent;

}