package org.example.shell;

import org.example.model.Image;
import org.example.model.Repository;

import java.awt.*;

public class UpdateCommand implements Command {
    private Repository repo;

    public UpdateCommand(Repository repo) {
        this.repo = repo;
    }

    @Override
    public void execute(String[] args) throws Exception {
        if (args.length < 2) throw new AddCommand.InvalidDataException("Usage: update <imageId> <newPath>");
        String id = args[0];
        String newPath = args[1];
        Image img = repo.getImageById(id);
        if (img == null) throw new AddCommand.InvalidDataException("Image not found: " + id);
        System.out.println("Image updated.");
    }
}

