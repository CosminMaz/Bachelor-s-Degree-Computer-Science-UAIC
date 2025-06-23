package org.example.word;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

@AllArgsConstructor
@Setter
@Getter
public class Tile {
    char letter;
    int points;

    @Override
    public String toString() {
        return letter + "(" + points + ")";
    }

}
