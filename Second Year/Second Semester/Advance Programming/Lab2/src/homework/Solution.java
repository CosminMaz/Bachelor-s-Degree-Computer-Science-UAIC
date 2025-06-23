package homework;

import persons.Student;

import java.util.Arrays;
import java.util.Objects;

public class Solution extends Problem{
    private boolean hallCompatible;

    public Solution(int n){
        super(n);
        this.hallCompatible = true;
    }

    public void greedySolution(){
        for(int i = 0; i < this.getAddedStudents(); ++i){
            for(int j = 0; j < this.getAddedProjects(); ++j){
                if((this.students[i].getPrefferedProject(0) == this.projects[j] || this.students[i].getPrefferedProject(1) == this.projects[j]) && this.projects[i] != null){
                    this.projects[j] = null;
                    break;
                }
                System.out.println("Can't solve the problem");
                return;
            }
        }
        System.out.println("Can solve the problem");
    }

    public void hallSolution(){
        Student[] temp = new Student[this.getAddedStudents()];
        temp = this.students;
        hallSolution(this.getAddedStudents(), 0, temp, 0);
        if(this.hallCompatible){
            System.out.println("It's possible to verify with Hall.");
        } else {
            System.out.println("It's not possible to verify with Hall.");
        }
    }

    public void hallSolution(int n, int index, Student[]current, int size){
        if(size >= this.getAddedStudents()){
            return;
        }

        int uniqueProjects = 0;
        String[] unique = new String[this.getAddedProjects()];
        for(int i = 0; i < this.getAddedProjects(); ++i){
            unique[i] = "";
        }

        for(int i = 0; i < size; ++i){
            for(int k = 0; k <= 1; ++k){
                boolean found = false;
                for(int j = 0; j < uniqueProjects; j++){
                    System.out.println(current[i].getPrefferedProject(k).getProjectName() + " " + unique[j]);
                    if(current[i].getPrefferedProject(k).getProjectName().equals(unique[j])){
                        found = true;
                        //break;
                    }
                }
                if(!found){
                    unique[uniqueProjects] = current[i].getPrefferedProject(k).getProjectName();
                    uniqueProjects++;
                }
            }
        }

        if(size > uniqueProjects){
            this.hallCompatible = false;
        }

        for(int i = index; i < this.getAddedStudents(); ++i){
            current[i] = this.students[i];
            hallSolution(this.getAddedStudents(), i + 1, current, size + 1);
        }
    }
}
