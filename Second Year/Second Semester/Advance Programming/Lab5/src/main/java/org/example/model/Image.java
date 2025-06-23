package org.example.model;

import lombok.AllArgsConstructor;

import java.util.List;
import java.time.LocalDate;

public record Image(String name, LocalDate date, List<String> tags, String fileLocation) {
}
