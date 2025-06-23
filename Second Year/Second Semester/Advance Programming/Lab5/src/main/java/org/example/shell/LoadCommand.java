package org.example.shell;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.core.type.TypeReference;
import org.example.model.Image;
import org.example.model.Repository;


import java.io.File;
import java.util.List;

public class LoadCommand implements Command {
    private Repository repo;

    public LoadCommand(Repository repo) {
        this.repo = repo;
    }

    @Override
    public void execute(String[] args) throws Exception {
        if (args.length < 1) throw new AddCommand.InvalidDataException("Usage: load <file.json>");
        ObjectMapper mapper = new ObjectMapper();
        List<Image> images = mapper.readValue(new File(args[0]), new TypeReference<List<Image>>() {});
        repo.setImages(images);
        System.out.println("Loaded " + images.size() + " images.");
    }
}
