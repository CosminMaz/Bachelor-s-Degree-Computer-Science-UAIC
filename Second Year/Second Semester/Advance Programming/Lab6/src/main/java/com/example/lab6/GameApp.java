package com.example.lab6;

import javafx.application.Application;
import javafx.embed.swing.SwingFXUtils;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.SnapshotParameters;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.*;
import javafx.scene.image.WritableImage;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import javax.imageio.ImageIO;
import java.io.*;
import java.util.*;

public class GameApp extends Application {

    private final int canvasWidth = 800;
    private final int canvasHeight = 600;

    private Canvas canvas;
    private GraphicsContext gc;
    private List<Point> dots = new ArrayList<>();
    private List<Line> lines = new ArrayList<>();
    private Point selectedDot = null;
    private int currentPlayer = 0; // 0 = blue, 1 = red
    private TextField dotCountField;

    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("Dot Game Full");

        BorderPane root = new BorderPane();

        // Top: Configuration Panel
        HBox configPanel = new HBox(10);
        configPanel.setPadding(new Insets(10));
        configPanel.setStyle("-fx-background-color: #e0e0e0;");
        Label dotLabel = new Label("Number of Dots:");
        dotCountField = new TextField("10");
        Button generateButton = new Button("New Game");
        generateButton.setOnAction(e -> generateDots());
        configPanel.getChildren().addAll(dotLabel, dotCountField, generateButton);

        // Center: Canvas
        canvas = new Canvas(canvasWidth, canvasHeight);
        gc = canvas.getGraphicsContext2D();
        Pane canvasPane = new Pane(canvas);
        canvasPane.setStyle("-fx-background-color: white;");
        canvas.addEventHandler(MouseEvent.MOUSE_CLICKED, this::handleCanvasClick);

        // Bottom: Control Panel
        HBox controlPanel = new HBox(10);
        controlPanel.setPadding(new Insets(10));
        controlPanel.setStyle("-fx-background-color: #d0d0d0;");
        Button saveButton = new Button("Save");
        Button loadButton = new Button("Load");
        Button exportButton = new Button("Export PNG");
        Button scoreButton = new Button("Compare Score");
        Button exitButton = new Button("Exit");

        saveButton.setOnAction(e -> saveGame());
        loadButton.setOnAction(e -> loadGame());
        exportButton.setOnAction(e -> exportToPNG());
        scoreButton.setOnAction(e -> compareWithBestScore());
        exitButton.setOnAction(e -> primaryStage.close());

        controlPanel.getChildren().addAll(saveButton, loadButton, exportButton, scoreButton, exitButton);

        // Layout
        root.setTop(configPanel);
        root.setCenter(canvasPane);
        root.setBottom(controlPanel);

        primaryStage.setScene(new Scene(root));
        primaryStage.show();

        generateDots();
    }

    private void generateDots() {
        try {
            int numberOfDots = Integer.parseInt(dotCountField.getText());
            dots.clear();
            lines.clear();
            Random rand = new Random();
            for (int i = 0; i < numberOfDots; i++) {
                double x = 20 + rand.nextDouble() * (canvasWidth - 40);
                double y = 20 + rand.nextDouble() * (canvasHeight - 40);
                dots.add(new Point(x, y));
            }
            drawBoard();
        } catch (NumberFormatException ex) {
            System.out.println("Invalid number of dots.");
        }
    }

    private void handleCanvasClick(MouseEvent event) {
        for (Point dot : dots) {
            if (dot.contains(event.getX(), event.getY())) {
                if (selectedDot == null) {
                    selectedDot = dot;
                } else if (selectedDot != dot) {
                    Line line = new Line(selectedDot, dot, currentPlayer);
                    if (!lines.contains(line)) {
                        lines.add(line);
                        currentPlayer = 1 - currentPlayer; // switch player
                    }
                    selectedDot = null;
                } else {
                    selectedDot = null;
                }
                break;
            }
        }
        drawBoard();
    }

    private void drawBoard() {
        gc.clearRect(0, 0, canvasWidth, canvasHeight);
        for (Line line : lines) {
            gc.setStroke(line.player == 0 ? Color.BLUE : Color.RED);
            gc.setLineWidth(2);
            gc.strokeLine(line.a.x, line.a.y, line.b.x, line.b.y);
        }
        for (Point dot : dots) {
            gc.setFill(Color.BLACK);
            gc.fillOval(dot.x - 4, dot.y - 4, 8, 8);
        }
        if (selectedDot != null) {
            gc.setStroke(Color.GREEN);
            gc.strokeOval(selectedDot.x - 6, selectedDot.y - 6, 12, 12);
        }
    }

    private void compareWithBestScore() {
        double[] scores = new double[2];
        for (Line line : lines) {
            scores[line.player] += line.length();
        }
        double mstScore = computeMSTScore();

        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("Score Comparison");
        alert.setHeaderText("Score Comparison");
        alert.setContentText(String.format("Player Blue: %.2f\nPlayer Red: %.2f\nOptimal (MST): %.2f",
                scores[0], scores[1], mstScore));
        alert.show();
    }

    private double computeMSTScore() {
        List<Edge> allEdges = new ArrayList<>();
        for (int i = 0; i < dots.size(); i++) {
            for (int j = i + 1; j < dots.size(); j++) {
                allEdges.add(new Edge(dots.get(i), dots.get(j)));
            }
        }
        Collections.sort(allEdges);

        UnionFind uf = new UnionFind(dots.size());
        double totalWeight = 0;
        for (Edge edge : allEdges) {
            int i = dots.indexOf(edge.a);
            int j = dots.indexOf(edge.b);
            if (uf.union(i, j)) {
                totalWeight += edge.length;
            }
        }
        return totalWeight;
    }

    private void exportToPNG() {
        WritableImage image = new WritableImage(canvasWidth, canvasHeight);
        canvas.snapshot(new SnapshotParameters(), image);
        File file = new File("game_board.png");
        try {
            ImageIO.write(SwingFXUtils.fromFXImage(image, null), "png", file);
            System.out.println("Image saved.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void saveGame() {
        try (ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream("game.ser"))) {
            out.writeObject(new GameState(dots, lines, currentPlayer));
            System.out.println("Game saved.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void loadGame() {
        try (ObjectInputStream in = new ObjectInputStream(new FileInputStream("game.ser"))) {
            GameState state = (GameState) in.readObject();
            this.dots = state.dots;
            this.lines = state.lines;
            this.currentPlayer = state.currentPlayer;
            drawBoard();
            System.out.println("Game loaded.");
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    // Point Class
    private static class Point {
        double x, y;

        Point(double x, double y) { this.x = x; this.y = y; }

        boolean contains(double px, double py) {
            return Math.hypot(x - px, y - py) <= 6;
        }
    }

    // Line Class
    private static class Line {
        Point a, b;
        int player;

        Line(Point a, Point b, int player) {
            if (a.hashCode() <= b.hashCode()) {
                this.a = a;
                this.b = b;
            } else {
                this.a = b;
                this.b = a;
            }
            this.player = player;
        }

        double length() {
            return Math.hypot(a.x - b.x, a.y - b.y);
        }

        @Override
        public boolean equals(Object o) {
            if (!(o instanceof Line other)) return false;
            return a.equals(other.a) && b.equals(other.b);
        }

        @Override
        public int hashCode() {
            return Objects.hash(a, b);
        }
    }

    // Edge Class (for MST)
    private static class Edge implements Comparable<Edge> {
        Point a, b;
        double length;

        Edge(Point a, Point b) {
            this.a = a;
            this.b = b;
            this.length = Math.hypot(a.x - b.x, a.y - b.y);
        }

        @Override
        public int compareTo(Edge o) {
            return Double.compare(this.length, o.length);
        }
    }

    // Union-Find for MST
    private static class UnionFind {
        int[] parent;

        UnionFind(int size) {
            parent = new int[size];
            Arrays.fill(parent, -1);
        }

        int find(int x) {
            if (parent[x] < 0) return x;
            return parent[x] = find(parent[x]);
        }

        boolean union(int x, int y) {
            int px = find(x), py = find(y);
            if (px == py) return false;
            parent[px] = py;
            return true;
        }
    }

    // Serializable GameState
    private static class GameState {
        List<Point> dots;
        List<Line> lines;
        int currentPlayer;

        GameState(List<Point> dots, List<Line> lines, int currentPlayer) {
            this.dots = new ArrayList<>(dots);
            this.lines = new ArrayList<>(lines);
            this.currentPlayer = currentPlayer;
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}
