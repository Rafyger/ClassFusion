#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "teacherbase.h"
#include "teacherprofile.h"
#include "expertteacher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


struct CourseRecord {
    QString code;
    double grade;
    QString styleTags;
};

struct MatchRow {
    QString professor;
    QString courseCode;
    double score;
    QString why;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onUploadTranscriptClicked();
    void onLoadCatalogClicked();

    void onLoadSyllabiClicked();
    void onLoadReviewsClicked();
    void onTeacherSelectionChanged();

    void onFindMatchClicked();

private:
    Ui::MainWindow *ui;

    QVector<CourseRecord>   m_courses;
    QVector<TeacherBase*>   m_teachers;
    QVector<MatchRow>       m_matches;

    void initStudentTab();
    void initTeacherTab();
    void initMatchesTab();

    void seedDummyStudentData();
    void seedDummyTeacherData();
    void computeStudentProfile();
    void recomputeMatches(const QString &courseCode);

    void refreshStudentCoursesTable();
    void refreshTeacherTable();
    void refreshMatchesTable();

    void updateStudentProfileBars(double writing, double interactive,
                                  double exam, double lecture);
    void updateTeacherProfileBars(TeacherBase *baseTeacher);
};

#endif
