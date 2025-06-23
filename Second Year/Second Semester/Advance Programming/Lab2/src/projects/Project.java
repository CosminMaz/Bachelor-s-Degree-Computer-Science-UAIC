package projects;

import persons.Teacher;

public class Project {
    private String projectName;
    private ProjectType projectType;
    private final Teacher teacherProposal;

    public Project(String projectName, ProjectType projectType, Teacher teacherProposal) {
        this.projectName = projectName;
        this.projectType = projectType;
        //Needs to be a shallow copy
        this.teacherProposal = teacherProposal;
    }

    public String getProjectName() {
        return projectName;
    }

    public ProjectType getProjectType() {
        return projectType;
    }

    public Teacher getTeacherProposal() {
        return teacherProposal;
    }

    public void setProjectName(String projectName) {
        this.projectName = projectName;
    }

    public void setProjectType(ProjectType projectType) {
        this.projectType = projectType;
    }

    @Override
    public String toString() {
        return "Project: " + projectName + " Type: " + projectType + ", proposed by: " + teacherProposal.getName() + "\n";
    }

}
