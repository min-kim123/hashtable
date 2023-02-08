#ifndef STUDENT_H
#define STUDENT_H
#include <iostream>

class Student {
public:
    Student(char* newfirst, char* newlast, int newID, float newGPA);


    void setFirst(char newfirst[80]);
    void setLast(char newlast[80]);
    void setID(int newID);
    void setGPA(float newGPA);

    char* getFirst();
    char* getLast();
    int getID();
    float getGPA();

private:
    char firstname[80];
    char lastname[80];
    int id;
    float gpa;
};

#endif