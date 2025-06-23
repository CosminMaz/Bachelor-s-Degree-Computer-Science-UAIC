package ClientApplication;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Objects;
import java.util.Random;
import java.util.Scanner;

public class GameClient {

    static final int[][] directions = {
            {-1, 0}, {-1, 1}, {0, -1},
            {0, 1}, {1, -1}, {1, 0}
    };

    static boolean[][] visited;

    public static boolean existsPath(char[][] board) {
        visited = new boolean[11][11];
        for(int j = 0; j < 11; j++) {
            if (board[0][j] == 'B' || board[0][j] == 'N') {
                if(dfsPossibleBlue(board, 0, j, visited)) {
                    return true;
                }
            }
        }
        return false;
    }

    public static boolean dfsPossibleBlue(char[][] board, int x, int y, boolean[][] visited) {
        if(x == 10) return true;

        visited[x][y] = true;

        for(int[] direction : directions) {
            int newX = x + direction[0];
            int newY = y + direction[1];

            if(newX >= 0 && newX <= 10 && newY >= 0 && newY <= 10 && !visited[newX][newY] &&
                    (board[newX][newY] == 'B' || board[newX][newY] == 'N')) {
                if(dfsPossibleBlue(board, newX, newY, visited)) {
                    return true;
                }
            }
        }
        return false;
    }

    public static boolean hasRedWon(char[][] board) {
        visited = new boolean[11][11];
        for (int i = 0; i < board.length; i++) {
            if(board[i][0] == 'R' && !visited[i][0]) {
                if(dfs(board, i, 0, 'R')) {
                    return true;
                }
            }
        }
        return false;
    }

    public static boolean hasBlueWon(char[][] board) {
        visited = new boolean[11][11];
        for (int j = 0; j < board.length; j++) {
            if(board[0][j] == 'B' && !visited[0][j]) {
                if(dfs(board, 0, j, 'B')) {
                    return true;
                }
            }
        }
        return false;
    }

    public static boolean dfs(char[][] board, int x, int y, char color) {
        if(color == 'R' && y == 10) return true;
        if(color == 'B' && x == 10) return true;

        visited[x][y] = true;

        for(int[] direction : directions) {
            int newX = x + direction[0];
            int newY = y + direction[1];
            if (newX >= 0 && newY >= 0 && newX < 11 && newY < 11 && board[newX][newY] == color && !visited[newX][newY]) {
                if(dfs(board, newX, newY, color)) {
                    return true;
                }
            }
        }
        return false;
    }

    public static void showTable(char[][] table) {
        System.out.println("Table: ");
        for (char[] chars : table) {
            for (char aChar : chars) {
                System.out.print(aChar + " ");
            }
            System.out.println();
        }
    }

    public static void main(String [] args) throws IOException{
        String serverAddress = "127.0.0.1";
        final int PORT = 8100;
        Scanner scanner = new Scanner(System.in);
        String color = "not assigned";
        char[][] table = {
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
                {'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'},
        };
        try {
            Socket socket = new Socket(serverAddress, PORT);
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            while(true) {
                switch (color) {
                    case "not assigned" -> {
                        System.out.print("Enter your command: ");
                        String request = scanner.nextLine();

                        if (Objects.equals(request, "exit")) {
                            System.out.println("Client Application Stopped.");
                            return;
                        }

                        out.println(request);
                        out.flush();

                        String response = in.readLine();
                        System.out.println(response);
                        if (Objects.equals(response, "you created a game, your color is red") || Objects.equals(response, "you joined a game, your color is blue")) {
                            if (!response.isEmpty()) {
                                String[] words = response.split(" ");
                                color = words[words.length - 1];
                            }
                        } else if (Objects.equals(response, "stop")) {
                            socket.close();
                            return;
                        }else if(Objects.equals(response, "you created a game with AI")) {
                            color = "AI";
                        } else {
                            System.out.println(response);
                        }
                    }
                    case "red" -> {
                        showTable(table);

                        System.out.print("Enter your command: ");
                        String request = scanner.nextLine();

                        if (Objects.equals(request, "exit")) {
                            System.out.println("Client Application Stopped.");
                            return;
                        } else {
                            String[] words = request.split(" ");
                            String move = words[words.length - 1];
                            int rand = move.charAt(0) - 'A';
                            int coloana = Integer.parseInt(move.substring(1));
                            table[rand][coloana] = 'R';
                        }

                        showTable(table);
                        if(hasRedWon(table)) {
                            System.out.println("Red wins");
                            out.println("You lost");
                            out.flush();
                            return;
                        }

                        out.println(request);
                        out.flush();

                        String response = in.readLine();
                        System.out.println(response);
                        if (Objects.equals(response, "stop")) {
                            socket.close();
                            return;
                        }else if(Objects.equals(response, "2 You lost") || Objects.equals(response, "2 You won")) {
                            return;
                        } else {
                            String[] words = response.split(" ");
                            String move = words[words.length - 1];
                            int rand = move.charAt(0) - 'A';
                            int coloana = Integer.parseInt(move.substring(1));
                            table[rand][coloana] = 'B';
                        }

                        showTable(table);
                        if(hasBlueWon(table)) {
                            System.out.println("Blue wins");
                            out.println("You won");
                            out.flush();
                            return;
                        }
                    }
                    case "blue" -> {
                        String response = in.readLine();
                        System.out.println(response);
                        if (Objects.equals(response, "exit")) {
                            System.out.println("Client Application Stopped.");
                            return;
                        }else if(Objects.equals(response, "1 You lost") || Objects.equals(response, "1 You won")) {
                            return;
                        } else {
                            String[] words = response.split(" ");
                            String move = words[words.length - 1];
                            int rand = move.charAt(0) - 'A';
                            int coloana = Integer.parseInt(move.substring(1));
                            table[rand][coloana] = 'R';
                        }

                        showTable(table);
                        if(hasRedWon(table)) {
                            System.out.println("Red wins");
                            out.println("You won");
                            out.flush();
                            return;
                        }

                        System.out.print("Enter your command: ");
                        String request = scanner.nextLine();

                        if (Objects.equals(request, "exit")) {
                            System.out.println("Client Application Stopped.");
                            return;
                        } else {
                            String[] words = request.split(" ");
                            String move = words[words.length - 1];
                            int rand = move.charAt(0) - 'A';
                            int coloana = Integer.parseInt(move.substring(1));
                            table[rand][coloana] = 'B';
                        }

                        showTable(table);
                        if(hasBlueWon(table)) {
                            System.out.println("Blue wins");
                            out.println("You lost");
                            out.flush();
                            return;
                        }

                        out.println(request);
                        out.flush();
                    }
                    case "AI" -> {
                        int turn = 0; // turn = 0, tura jucatorului, turn = 1 tura AI
                        System.out.println("AM intrat pe caz AI");
                        while(true) {
                            if (turn == 0) {
                                showTable(table);
                                if (hasRedWon(table)) {
                                    System.out.println("You lost");
                                    return;
                                }
                                System.out.print("Enter your command: ");
                                String request = scanner.nextLine();

                                if (Objects.equals(request, "exit")) {
                                    System.out.println("Client Application Stopped.");
                                    return;
                                } else {
                                    String[] words = request.split(" ");
                                    String move = words[words.length - 1];
                                    int rand = move.charAt(0) - 'A';
                                    int coloana = Integer.parseInt(move.substring(1));
                                    table[rand][coloana] = 'R';
                                }

                                showTable(table);
                                if (hasRedWon(table)) {
                                    System.out.println("Red wins");
                                    return;
                                }
                                turn++;
                            } else {
                                Random r = new Random();
                                int rand = r.nextInt(11);
                                int coloana = r.nextInt(11);
                                while (table[rand][coloana] == 'R') {
                                    rand = r.nextInt(11);
                                    coloana = r.nextInt(11);
                                }
                                table[rand][coloana] = 'B';
                                if (existsPath(table)) {
                                    System.out.println("AI: Exista cel putin un path invingator pentru mine");
                                } else {
                                    System.out.println("AI: Nu mai exista niciun path invingator pentru mine");
                                }
                                turn--;
                            }
                        }
                    }
                }
            }
        } catch (UnknownHostException e){
            System.err.println(" 🤨🤨🤨🤔 " + e);
        }

    }
}
