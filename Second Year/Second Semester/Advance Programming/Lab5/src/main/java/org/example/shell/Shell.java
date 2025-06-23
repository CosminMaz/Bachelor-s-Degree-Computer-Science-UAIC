package org.example.shell;

import org.example.model.Repository;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class Shell {
    private Repository repo = new Repository();
    private Map<String, Command> commands = new HashMap<>();

    public Shell() {
        commands.put("add", new AddCommand(repo));
        commands.put("remove", new RemoveCommand(repo));
        commands.put("update", new UpdateCommand(repo));
        commands.put("load", new LoadCommand(repo));
        commands.put("save", new SaveCommand(repo));
        commands.put("report", new ReportCommand(repo));
    }

    public void run() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.print("> ");
            String line = scanner.nextLine();
            if ("exit".equalsIgnoreCase(line)) break;
            String[] tokens = line.trim().split("\\s+");
            if (tokens.length == 0) continue;
            String cmdName = tokens[0];
            Command cmd = commands.get(cmdName);
            if (cmd == null) {
                System.out.println("Invalid command.");
                continue;
            }
            try {
                cmd.execute(Arrays.copyOfRange(tokens, 1, tokens.length));
            } catch (Exception e) {
                System.err.println("Error: " + e.getMessage());
            }
        }
    }
}
