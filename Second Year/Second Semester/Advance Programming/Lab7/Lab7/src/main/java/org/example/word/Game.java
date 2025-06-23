import org.example.game.Board;
import org.example.game.Player;
import org.example.word.Bag;
import org.example.word.WordDictionary;

import java.util.Dictionary;

public static void main(String[] args) throws InterruptedException {
    Bag bag = new Bag();
    WordDictionary dictionary = new WordDictionary(Arrays.asList("HELLO", "WORLD", "JAVA", "CODE", "THREAD", "SYNC", "LOCK", "BAG", "GAME"));
    Board board = new Board();

    List<Thread> players = new ArrayList<>();
    for (int i = 1; i <= 4; i++) {
        Thread t = new Thread(new Player("Player" + i, bag, board, dictionary));
        players.add(t);
        t.start();
    }

    for (Thread t : players) t.join();

    board.displayResults();
    System.out.println("\nWinner: " + board.getWinner());
}