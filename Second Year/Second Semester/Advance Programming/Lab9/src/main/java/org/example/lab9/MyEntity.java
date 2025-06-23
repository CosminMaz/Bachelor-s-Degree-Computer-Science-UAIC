package org.example.lab9;
import jakarta.persistence.*;

@Entity
@NamedQuery(
        name = "MyEntity.findByName",
        query = "SELECT e FROM MyEntity e WHERE e.name LIKE :name"
)
public class MyEntity {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String name;

    public MyEntity() {}
    public MyEntity(String name) {
        this.name = name;
    }

    public Long getId() { return id; }
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
}

