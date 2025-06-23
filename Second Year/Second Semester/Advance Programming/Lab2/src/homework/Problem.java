package homework;

import persons.Person;
import persons.Student;
import projects.Project;

import java.time.LocalDate;

public class Problem {
    final Project[] projects;
    final Student[] students;
    private int addedProjects;
    private int addedStudents;

    public Problem(int n){
        this.projects = new Project[n];
        this.students = new Student[n];
        this.addedProjects = 0;
        this.addedStudents = 0;
    }

    public void addProject(Project project){
        if(this.addedProjects < this.projects.length){
            this.projects[this.addedProjects] = project;
            this.addedProjects++;
        }
    }

    public void addStudent(Student student){
        this.students[this.addedStudents] = student;
        this.addedStudents++;
    }

    public int getAddedStudents(){
        return this.addedStudents;
    }

    public int getAddedProjects(){
        return this.addedProjects;
    }

    public Person[] returnAllParsonsInvolved(){
        Person[] involvedPersons = new Person[this.addedProjects + this.addedProjects];

        for(int i = 0; i < this.addedStudents; ++i){
            involvedPersons[i] = this.students[i];
        }

        for(int i = 0; i < this.addedProjects; ++i){
            involvedPersons[i + this.addedStudents] = this.projects[i].getTeacherProposal();
        }

        return  involvedPersons;
    }

    public void printAll(){
        for (Project project : this.projects) {
            System.out.println(project);
        }

        for (Student student : this.students) {
            System.out.println(student);
        }
    }

}
