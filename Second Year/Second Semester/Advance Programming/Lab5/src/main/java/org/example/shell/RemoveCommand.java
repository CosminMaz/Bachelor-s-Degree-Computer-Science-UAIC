package org.example.shell;

import org.example.model.Repository;

public class RemoveCommand implements Command {
    private Repository repo;

    public RemoveCommand(Repository repo) {
        this.repo = repo;
    }

    @Override
    public void execute(String[] args) throws Exception {
        if (args.length < 1) throw new AddCommand.InvalidDataException("Usage: remove <imageId>");
        String id = args[0];
        boolean removed = repo.removeImageById(id);
        if (removed) {
            System.out.println("Image removed.");
        } else {
            throw new AddCommand.InvalidDataException("Image with ID '" + id + "' not found.");
        }
    }
}

