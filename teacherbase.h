#ifndef TEACHERBASE_H
#define TEACHERBASE_H

#include <QString>

class TeacherBase
{
public:
    virtual ~TeacherBase() {}

    virtual QString getName() const = 0;
    virtual QString getCourse() const = 0;
    virtual QString getTags() const = 0;

    // Runtime polymorphism
    virtual double computeScore() const = 0;
};

#endif // TEACHERBASE_H
