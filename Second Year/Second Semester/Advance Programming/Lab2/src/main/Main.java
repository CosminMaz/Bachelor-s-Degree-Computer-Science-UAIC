package main;

import homework.Problem;
import homework.Solution;
import persons.Student;
import persons.Teacher;
import projects.Project;
import projects.ProjectType;

import java.time.LocalDate;

public class Main {
    public static void main(String[] args) {

        /* Compulsory
        Student student1 = new Student("John", LocalDate.of(1990, 1, 1), 123);
        System.out.println(student1);

        Teacher teacher1 = new Teacher("JonBonjovi", LocalDate.of(1966, 1, 4));
        System.out.println(teacher1);

        Project project1 = new Project("TestDetest", ProjectType.THEORETICAL, teacher1);
        System.out.println(project1);
        */

        //Student student1 = new Student("Cossi", LocalDate.of(2004, 4, 15), 231136);

        int instanceSize = (int)(Math.random() * 20);
        Solution problem = new Solution(instanceSize);

        for(int i = 0; i < instanceSize - 1; ++i){
            Student student = new Student("Cossi" + i, LocalDate.of(2004, 4, 15), 231136);

            Teacher teacher = new Teacher("Teacher" + i, LocalDate.of(1966, 1, 4));

            int i2 = (int)(Math.random() * 20);
            int i3 = (int)(Math.random() * 20);

            Project project1 = new Project("Project" + i2, ProjectType.THEORETICAL, teacher);
            Project project2 = new Project("Project" + i3, ProjectType.PRACTICAL, teacher);
            student.setPreferredProjects(new Project[]{project1, project2});
            problem.addProject(project1);
            problem.addProject(project2);
            problem.addStudent(student);
        }

        problem.printAll();
        //problem.greedySolution();
        problem.hallSolution();
    }
}