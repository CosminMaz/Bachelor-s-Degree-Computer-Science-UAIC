package Homework;

import java.util.Vector;

public class Bonus extends Homework {
    Vector<Integer> neighbors; /* R */
    Vector<Integer> vertices; /* P */
    Vector<Integer> edges; /* X */
    Boolean isSizeOfK = false;
    public Bonus(int n, int k){
        super(n, k);
        this.neighbors = new Vector<>(n);
        this.vertices = new Vector<>(n);
        this.edges = new Vector<>(n);

        for(int i = 0; i < n; i++){
            this.vertices.add(i);
        }
    }

    Bonus(Bonus comp){
        super(comp);
        this.neighbors = new Vector<>(n);
        this.vertices = new Vector<>(n);
        this.edges = new Vector<>(n);

        for(int i = 0; i < n; i++){
            this.vertices.add(i);
        }
    }

    public void BronKerbosch1(){
        if(this.neighbors.size() == this.k){
            isSizeOfK = true;
        }

        for(int i : this.vertices){
            Vector<Integer> tempNeighbors = new Vector<>(n); /* R */
            Vector<Integer> tempVertices = new Vector<>(n); /* P */
            Vector<Integer> tempEdges = new Vector<>(n); /* X */
            tempNeighbors.addAll(this.neighbors);
            tempVertices.addAll(this.vertices);
            tempEdges.addAll(this.edges);
            tempNeighbors.add(i);

            Vector<Integer> nodeNeighbors = searchNeighbours(i);
            Vector<Integer> P = new Vector<>(n);
            Vector<Integer> X = new Vector<>(n);
            for(int j : tempVertices){
                if(nodeNeighbors.contains(j)){
                    P.add(j);
                }
            }
            for(int j : tempEdges){
                if(nodeNeighbors.contains(j)){
                    X.add(j);
                }
            }
            recursiveBronKerbosch1(tempNeighbors, P, X);
            tempNeighbors.removeElement(i);
            edges.add(i);
        }
        if(isSizeOfK){
            System.out.println("The graph has at least one clique of size k.");
        } else {
            System.out.println("The graph does not have a clique of size k.");
        }
    }

    void recursiveBronKerbosch1(Vector<Integer> R, Vector<Integer> P, Vector<Integer> X){
        if(R.size() == this.k){
            isSizeOfK = true;
        }

        if(R.size() < this.k  && P.isEmpty() && X.isEmpty()){
           return;
        }

        for(int i : P){
            Vector<Integer> tempNeighbors = new Vector<>(n); /* R */
            Vector<Integer> tempVertices = new Vector<>(n); /* P */
            Vector<Integer> tempEdges = new Vector<>(n); /* X */
            tempNeighbors.addAll(R);
            tempVertices.addAll(P);
            tempEdges.addAll(X);
            tempNeighbors.add(i);

            Vector<Integer> nodeNeighbors = searchNeighbours(i);

            Vector<Integer> tempP = new Vector<>(n);
            Vector<Integer> tempX = new Vector<>(n);
            for(int j : tempVertices){
                if(nodeNeighbors.contains(j)){
                    tempP.add(j);
                }
            }
            for(int j : tempEdges){
                if(nodeNeighbors.contains(j)){
                    tempX.add(j);
                }
            }
            recursiveBronKerbosch1(tempNeighbors, tempP, tempX);
            tempNeighbors.removeElement(i);
        }
    }

    Vector<Integer> searchNeighbours(int node){
        Vector<Integer> neighbors = new Vector<>(n);
        for(int i = 0; i < n; i++){
            if(this.matrix[node][i] == 1){
                neighbors.add(i);
            }
        }
        return neighbors;
    }

    public void searchStableSet(){
        Bonus complementary = new Bonus(this);
        complementary.BronKerbosch1Complementary();
    }

    void BronKerbosch1Complementary(){
        if(this.neighbors.size() == this.k){
            isSizeOfK = true;
        }

        for(int i : this.vertices){
            Vector<Integer> tempNeighbors = new Vector<>(n); /* R */
            Vector<Integer> tempVertices = new Vector<>(n); /* P */
            Vector<Integer> tempEdges = new Vector<>(n); /* X */
            tempNeighbors.addAll(this.neighbors);
            tempVertices.addAll(this.vertices);
            tempEdges.addAll(this.edges);
            tempNeighbors.add(i);

            Vector<Integer> nodeNeighbors = searchNeighbours(i);
            Vector<Integer> P = new Vector<>(n);
            Vector<Integer> X = new Vector<>(n);
            for(int j : tempVertices){
                if(nodeNeighbors.contains(j)){
                    P.add(j);
                }
            }
            for(int j : tempEdges){
                if(nodeNeighbors.contains(j)){
                    X.add(j);
                }
            }
            recursiveBronKerbosch1(tempNeighbors, P, X);
            tempNeighbors.removeElement(i);
            edges.add(i);
        }
        if(isSizeOfK){
            System.out.println("The graph has at least one stable set of size k.");
        } else {
            System.out.println("The graph does not have a stable set of size k.");
        }
    }

}
