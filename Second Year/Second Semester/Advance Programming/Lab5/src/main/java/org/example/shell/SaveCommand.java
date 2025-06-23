package org.example.shell;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.example.model.Repository;

import java.io.File;

public class SaveCommand implements Command {
    private Repository repo;

    public SaveCommand(Repository repo) {
        this.repo = repo;
    }

    @Override
    public void execute(String[] args) throws Exception {
        if (args.length < 1) throw new AddCommand.InvalidDataException("Usage: save <file.json>");
        ObjectMapper mapper = new ObjectMapper();
        mapper.writerWithDefaultPrettyPrinter().writeValue(new File(args[0]), repo.getImages());
        System.out.println("Saved " + repo.getImages().size() + " images to " + args[0]);
    }
}
