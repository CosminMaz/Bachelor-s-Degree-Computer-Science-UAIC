package org.example.word;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class Bag {
    private final List<Tile> tiles = new ArrayList<>();
    private final Random random = new Random();

    public Bag() {
        for (char c = 'A'; c <= 'Z'; c++) {
            int points = random.nextInt(10) + 1;
            for (int i = 0; i < 10; i++) {
                tiles.add(new Tile(c, points));
            }
        }
        Collections.shuffle(tiles);
    }

    public synchronized List<Tile> extractTiles(int count) {
        List<Tile> extracted = new ArrayList<>();
        for (int i = 0; i < count && !tiles.isEmpty(); i++) {
            extracted.add(tiles.removeLast());
        }
        return extracted;
    }

    public synchronized boolean isEmpty() {
        return tiles.isEmpty();
    }


}
