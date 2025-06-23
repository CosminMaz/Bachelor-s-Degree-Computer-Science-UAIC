package org.example.shell;

public interface Command {
    void execute(String[] args) throws Exception;
}
