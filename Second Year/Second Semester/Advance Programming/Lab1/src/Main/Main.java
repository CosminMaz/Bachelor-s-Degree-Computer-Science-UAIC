package Main;

import Compulsory.Compulsory;
import Homework.*;


public class Main {
    public static void main(String[] args) {
        Compulsory compulsory = new Compulsory();

        if(args.length != 2){
            System.out.println("Usage: java Lab1.Main <int> <int>");
            System.exit(-1);
        }

        long t1 = System.currentTimeMillis();
        Homework homework = new Homework(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
        homework.printMatrix();
        homework.verifyNumberOfEdges();
        homework.printMinMaxDegreeOfVertices();
        long t2 = System.currentTimeMillis();
        System.out.println("Time taken: " + (t2 - t1) / 1000  + " seconds");

        Bonus bonus = new Bonus(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
        bonus.BronKerbosch1();
        bonus.searchStableSet();
    }
}