package persons;

import projects.Project;

import java.time.LocalDate;

public class Student extends Person {
    private int registrationNumber;
    private Project[] preferredProjects;

    public Student(String name, LocalDate birthDate, int registrationNumber) {
        super(name, birthDate);
        this.registrationNumber = registrationNumber;
    }

    public Project getPrefferedProject(int n){
        return this.preferredProjects[n];
    }

    public int getRegistrationNumber() {
        return registrationNumber;
    }

    public void setRegistrationNumber(int registrationNumber) {
        this.registrationNumber = registrationNumber;
    }

    //It needs to be a shallow copy
    public void setPreferredProjects(Project[] projects) {
        this.preferredProjects = projects;
    }

    @Override
    public String toString() {
        return "Student: " + this.getName() + ", birth date " + this.getBirthDate() + " " + this.preferredProjects[0].toString() + " " + this.preferredProjects[1].toString();
    }


    public boolean equals(Student otherStudent) {
        if(otherStudent == null || !(otherStudent instanceof Student)){
            return false;
        }
        return this.registrationNumber == otherStudent.getRegistrationNumber();

    }

}
