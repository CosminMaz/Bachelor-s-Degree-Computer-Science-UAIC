package Homework;

import java.util.Vector;

public class Homework {
    final int k, n;
    int[][] matrix;

    /*
      k-clique: a set of vertices that induces a complete graph (Noted w);
      stable set: is s subset of G where is no edge between its vertices (Noted: alpha);

      Dependence between clique and stable set: w(G) = alpha(G-);

      To check both the properties:
      k <= n / 2 + 1, if  k is odd
      k <= n / 2, otherwise
    */

    Homework(Bonus comp){
        this.n = comp.n;
        this.k = comp.k;
        this.matrix = new int[n][n];
        for(int i = 0; i < this.n; ++i){
            for(int j = 0; j < this.n; ++j){
                if(i == j){
                    this.matrix[i][j] = 0;
                } else if(comp.matrix[i][j] == 0){
                    this.matrix[i][j] = 1;
                } else {
                    this.matrix[i][j] = 0;
                }
            }
        }
    }

    public Homework(int n, int k) {
        if(k <= 0 || n <= 0){
            System.out.println("Invalid arguments: k and n must be positive");
            System.exit(-1);
        }

        if(k > n){
            System.out.println("Invalid arguments: k must be less than or equal to n");
            System.exit(-1);
        }

        this.k = k;
        this.n = n;
        this.matrix = new int[n][n];

        switch (this.k % 2){
            case 0:
                if(this.k <= this.n / 2 ){
                    generateGraph();
                } else {
                    generateRandomGraph();
                }
                break;
           case 1:
               if(this.k <= this.n / 2 + 1){
                   generateGraph();
               } else {
                   generateRandomGraph();
               }
               break;
           default:
               System.out.println("How can a number not be even or odd?");
               System.exit(-1);
        }
    }

    void generateGraph(){
        System.out.println("The graph will have both properties.");
        int randomIndex;
        //Make the stable set
        Vector<Integer> stableSet = new Vector<>(k);

        for(int i = 0; i < k; i++){
            do{
                randomIndex = (int)(Math.random()* n);
            } while(stableSet.contains(randomIndex));
            stableSet.add(randomIndex);
        }

        for(int i = 0; i < k; i++){
            for(int j = i + 1; j < k; j++){
                this.matrix[stableSet.get(i)][stableSet.get(j)] = 1;
                this.matrix[stableSet.get(j)][stableSet.get(i)] = 1;
            }
        }


        //Make the clique
        Vector<Integer> cliqueVertices = new Vector<>(k);
        //If k = n / 2 or k = n / 2 + 1 => one node from stable must be in clique
        //else 50% to one node from stable set to be in clique

        if( k == n / 2 || k == n / 2 + 1){
            cliqueVertices.add(stableSet.get((int)(Math.random()* k)));
        } else {
            if((int)(Math.random()* 1) % 2 == 0){
                cliqueVertices.add(stableSet.get((int)(Math.random()* k)));
            }
        }

        //Forming clique set
        while(cliqueVertices.size() != k){
            do {
                randomIndex = (int)(Math.random() * n);
            }while(cliqueVertices.contains(randomIndex) && stableSet.contains(randomIndex));
            cliqueVertices.add(randomIndex);
        }

        //Adding random vertices without breaking the properties
        //Maximum nr of vertices is: [n(n - 1) - k(k-1)]/2
        int numberOfRandomEdges = (int)(Math.random() * (n * (n-1) - k * (k - 1)) / 2);
        for(int i = 0; i < numberOfRandomEdges; ++i){
            int firstVertex = (int)(Math.random() * n);
            int secondVertex;
            do{
                secondVertex = (int)(Math.random() * n);
            }while(secondVertex == firstVertex || (stableSet.contains(secondVertex) && stableSet.contains(firstVertex)));
            this.matrix[firstVertex][secondVertex] = 1;
            this.matrix[secondVertex][firstVertex] = 1;
        }

    }

    void generateRandomGraph(){
        System.out.println("The graph won't have both properties.");
        int numberOfRandomEdges = (int)(Math.random() * (n * (n-1) - k * (k - 1)) / 2);
        for(int i = 0; i < numberOfRandomEdges; ++i){
            int firstVertex = (int)(Math.random() * n);
            int secondVertex;
            do{
                secondVertex = (int)(Math.random() * n);
            }while(secondVertex == firstVertex);
            this.matrix[firstVertex][secondVertex] = 1;
            this.matrix[secondVertex][firstVertex] = 1;
        }

    }

    public void printMatrix(){
        StringBuilder matrixString = new StringBuilder();
        matrixString.append("  ");
        for(int i = 0; i < n; i++){
            matrixString.append("  ");
            matrixString.append(i);
            matrixString.append(" ");
        }
        matrixString.append("\n");
        matrixString.append("  ");
        matrixString.append("\u2500".repeat(Math.max(0, n * 4)));
        matrixString.append("\n");

        for(int i = 0; i < n; ++i){
            matrixString.append(i);
            matrixString.append(" \u2502 ");
            for(int j = 0; j < n; ++j){
                matrixString.append(this.matrix[i][j]);
                if(j == n - 1){
                    matrixString.append(" \u2502 ");
                } else {
                    matrixString.append(" \u254E ");
                }

            }
            matrixString.append("\n");
            matrixString.append("  ");
            matrixString.append("\u2500".repeat(Math.max(0, n * 4)));
            matrixString.append("\n");
        }
        System.out.println(matrixString);
    }

    int printNumberOfEdges(){
        int numberOfEdges = 0;
        for(int i = 0; i < n; i++){
            for(int j = i + 1; j < n; j++){
                if(this.matrix[i][j] == 1){
                    numberOfEdges++;
                }
            }
        }
        System.out.println("Number of edges is: " + numberOfEdges + ".");
        return numberOfEdges;
    }

    public void printMinMaxDegreeOfVertices(){
        int minDegree = Integer.MAX_VALUE;
        int maxDegree = Integer.MIN_VALUE;
        int degree;

        for(int i = 0; i < n; i++){
            degree = 0;
            for(int j = 0; j < n; j++){
                degree += this.matrix[i][j];
            }

            if(degree < minDegree){
                minDegree = degree;
            }

            if(degree > maxDegree){
                maxDegree = degree;
            }
        }
        System.out.println("\u0394(G) = " + maxDegree);
        System.out.println("\u03B4(G) = " + minDegree);
    }

    public void verifyNumberOfEdges(){
        int sumOfDegrees = 0;

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                sumOfDegrees += this.matrix[i][j];
            }
        }
        if(sumOfDegrees == 2 * printNumberOfEdges()){
            System.out.println("Sum of degrees is equal with 2 * number of edges.");
        } else {
            System.out.println("Sum of degrees is NOT equal with 2 * number of edges.");
        }
    }

}
