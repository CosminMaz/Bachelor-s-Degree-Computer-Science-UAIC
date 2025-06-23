package org.example.game;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Board {
    private final Map<String, Integer> scores = new ConcurrentHashMap<>();

    public synchronized void submitWord(String player, String word, int score) {
        scores.merge(player, score, Integer::sum);
        System.out.println(player + " submitted \"" + word + "\" for " + score + " points.");
    }

    public void displayResults() {
        System.out.println("\n=== Final Scores ===");
        scores.forEach((player, score) -> System.out.println(player + ": " + score));
    }

    public String getWinner() {
        return scores.entrySet().stream()
                .max(Map.Entry.comparingByValue())
                .map(Map.Entry::getKey)
                .orElse("No one");
    }
}