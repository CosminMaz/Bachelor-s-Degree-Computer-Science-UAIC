package org.example.shell;

import org.example.model.Image;
import org.example.model.Repository;

import java.time.LocalDate;
import java.util.Collections;

public class AddCommand implements Command {
    private Repository repo;

    public AddCommand(Repository repo) {
        this.repo = repo;
    }

    @Override
    public void execute(String[] args) throws Exception {
        if (args.length < 2) throw new InvalidDataException("Not enough arguments.");
        repo.addImage(new Image(args[0], LocalDate.parse(args[1]), Collections.singletonList(args[2]), args[3])); // String name, LocalDate date, List<String> tags, String fileLocation
        System.out.println("Image added.");
    }

    public static class InvalidCommandException extends Exception {
        public InvalidCommandException(String message) {
            super(message);
        }
    }

    public static class InvalidDataException extends Exception {
        public InvalidDataException(String message) {
            super(message);
        }
    }


}

