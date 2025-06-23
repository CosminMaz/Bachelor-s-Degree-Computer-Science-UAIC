package org.example.main;

import org.example.model.Image;
import org.example.model.Repository;
import org.example.shell.Shell;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        Image image1 = new Image("Cat-photo", LocalDate.parse("2026-04-15"), new ArrayList<String>(Arrays.asList("Cat", "LeChat")), "D:\\photo.png");

        List<Image> imagesList = new ArrayList<>();
        imagesList.add(image1);

        Repository repo = new Repository(imagesList);

        repo.displayImage(image1);

        System.out.println("Welcome to the Image Repository Shell!");
        System.out.println("Type a command (add, remove, update, load, save, report, exit)");

        Shell shell = new Shell();
        shell.run();

        System.out.println("Goodbye!");




    }
}