package persons;

import java.time.LocalDate;
import java.util.Objects;

public class Teacher extends Person {

    public Teacher(String name, LocalDate birthDate) {
        super(name, birthDate);
    }

    @Override
    public String toString() {
        return "Teacher: " + this.getName() + ", birth date " + this.getBirthDate();
    }


    public boolean equals(Teacher otherTeacher) {
        return Objects.equals(this.getName(), otherTeacher.getName());
    }

}
