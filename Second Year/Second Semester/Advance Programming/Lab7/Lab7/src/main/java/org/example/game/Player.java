package org.example.game;

import org.example.word.Bag;
import org.example.word.Tile;
import org.example.word.WordDictionary;

import java.util.*;

public class Player implements Runnable {
    private final String name;
    private final Bag bag;
    private final Board board;
    private final WordDictionary dictionary;
    private final Random random = new Random();

    public Player(String name, Bag bag, Board board, WordDictionary dictionary) {
        this.name = name;
        this.bag = bag;
        this.board = board;
        this.dictionary = dictionary;
    }

    @Override
    public void run() {
        List<Tile> hand = bag.extractTiles(7);
        while (!hand.isEmpty()) {
            String word = tryToFormWord(hand);
            if (word != null) {
                int score = word.chars()
                        .map(c -> hand.stream().filter(t -> t.getLetter() == c).findFirst().get().getPoints())
                        .sum();
                board.submitWord(name, word, score);
                hand.removeIf(t -> word.indexOf(t.getLetter()) >= 0);
                hand.addAll(bag.extractTiles(word.length()));
            } else {
                hand.clear(); // discard and redraw
                hand.addAll(bag.extractTiles(7));
            }
        }
        System.out.println(name + " has finished playing.");
    }

    private String tryToFormWord(List<Tile> hand) {
        StringBuilder sb = new StringBuilder();
        for (Tile tile : hand) sb.append(tile.getLetter());
        String letters = sb.toString();
        // Try permutations
        for (int i = 7; i >= 2; i--) {
            List<String> permutations = getPermutations(letters, i);
            for (String word : permutations) {
                if (this.dictionary.isValidWord(word)) return word;
            }
        }
        return null;
    }

    private List<String> getPermutations(String input, int length) {
        Set<String> results = new HashSet<>();
        permute("", input, length, results);
        return new ArrayList<>(results);
    }

    private void permute(String prefix, String remaining, int length, Set<String> results) {
        if (prefix.length() == length) {
            results.add(prefix);
            return;
        }
        for (int i = 0; i < remaining.length(); i++) {
            permute(prefix + remaining.charAt(i),
                    remaining.substring(0, i) + remaining.substring(i + 1), length, results);
        }
    }
}