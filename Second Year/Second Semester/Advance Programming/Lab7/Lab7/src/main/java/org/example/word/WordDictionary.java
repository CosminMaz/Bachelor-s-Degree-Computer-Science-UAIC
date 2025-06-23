package org.example.word;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class WordDictionary {
    private final Set<String> words = new HashSet<>();

    public WordDictionary(List<String> wordList) {
        words.addAll(wordList);
    }

    public boolean isValidWord(String word) {
        return words.contains(word.toUpperCase());
    }
}