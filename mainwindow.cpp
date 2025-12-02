#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QItemSelectionModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initStudentTab();
    initTeacherTab();
    initMatchesTab();

    seedDummyStudentData();
    seedDummyTeacherData();
    computeStudentProfile();
    refreshStudentCoursesTable();
    refreshTeacherTable();

    connect(ui->btnUploadTranscript, &QPushButton::clicked,
            this, &MainWindow::onUploadTranscriptClicked);
    connect(ui->btnLoadCatalog, &QPushButton::clicked,
            this, &MainWindow::onLoadCatalogClicked);

    connect(ui->btnLoadSyllabi, &QPushButton::clicked,
            this, &MainWindow::onLoadSyllabiClicked);
    connect(ui->btnLoadReviews, &QPushButton::clicked,
            this, &MainWindow::onLoadReviewsClicked);

    connect(ui->tblTeachers->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, [this](const QModelIndex &current, const QModelIndex &){
                Q_UNUSED(current);
                onTeacherSelectionChanged();
            });

    connect(ui->btnFindMatch, &QPushButton::clicked,
            this, &MainWindow::onFindMatchClicked);
}

MainWindow::~MainWindow()
{
    qDeleteAll(m_teachers);
    delete ui;
}

void MainWindow::initStudentTab()
{
    ui->tblStudentCourses->setColumnCount(3);
    QStringList headers;
    headers << "Course" << "Grade" << "Style Tags";
    ui->tblStudentCourses->setHorizontalHeaderLabels(headers);
    ui->tblStudentCourses->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    updateStudentProfileBars(0, 0, 0, 0);
}

void MainWindow::initTeacherTab()
{
    ui->tblTeachers->setColumnCount(3);
    QStringList headers;
    headers << "Name" << "Course" << "Style Tags";
    ui->tblTeachers->setHorizontalHeaderLabels(headers);
    ui->tblTeachers->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    TeacherProfile dummy;
    updateTeacherProfileBars(&dummy);
}

void MainWindow::initMatchesTab()
{
    ui->tblMatches->setColumnCount(4);
    QStringList headers;
    headers << "Rank" << "Professor" << "Match Score" << "Why It Fits";
    ui->tblMatches->setHorizontalHeaderLabels(headers);
    ui->tblMatches->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::seedDummyStudentData()
{
    m_courses.clear();

    CourseRecord c1;
    c1.code = "ENC1101";
    c1.grade = 4.0;
    c1.styleTags = "writing-heavy, discussion";
    m_courses.push_back(c1);

    CourseRecord c2;
    c2.code = "MAC2311";
    c2.grade = 3.0;
    c2.styleTags = "exam-heavy, lecture-based";
    m_courses.push_back(c2);

    CourseRecord c3;
    c3.code = "COP2006";
    c3.grade = 4.0;
    c3.styleTags = "project-based, hands-on";
    m_courses.push_back(c3);
}

void MainWindow::seedDummyTeacherData()
{
    qDeleteAll(m_teachers);
    m_teachers.clear();

    m_teachers.push_back(new TeacherProfile(
        "Dr. Rivera", "MAC2311", "lecture-heavy, guided notes",
        0.85, 0.30, 0.10, 0.70));

    m_teachers.push_back(new TeacherProfile(
        "Prof. Chen", "MAC2311", "interactive, Q&A driven",
        0.60, 0.85, 0.20, 0.50));

    m_teachers.push_back(new ExpertTeacher(
        "Dr. Patel", "ENC1101", "writing-intensive, seminar",
        0.40, 0.60, 0.90, 0.30, 0.15));
}

void MainWindow::computeStudentProfile()
{
    double writing = 0.75;
    double interactive = 0.55;
    double exam = 0.30;
    double lecture = 0.60;

    updateStudentProfileBars(writing, interactive, exam, lecture);
}

void MainWindow::updateStudentProfileBars(double writing,
                                          double interactive,
                                          double exam,
                                          double lecture)
{
    ui->barWriting->setRange(0, 100);
    ui->barInteractive->setRange(0, 100);
    ui->barExam->setRange(0, 100);
    ui->barLecture->setRange(0, 100);

    ui->barWriting->setValue(static_cast<int>(writing * 100));
    ui->barInteractive->setValue(static_cast<int>(interactive * 100));
    ui->barExam->setValue(static_cast<int>(exam * 100));
    ui->barLecture->setValue(static_cast<int>(lecture * 100));
}

void MainWindow::updateTeacherProfileBars(TeacherBase *baseTeacher)
{
    auto *tp = dynamic_cast<TeacherProfile*>(baseTeacher);
    if (!tp) return;

    ui->barTeachLecture->setRange(0, 100);
    ui->barTeachInteractive->setRange(0, 100);
    ui->barTeachWriting->setRange(0, 100);
    ui->barTeachExams->setRange(0, 100);

    ui->barTeachLecture->setValue(tp->getLecture() * 100);
    ui->barTeachInteractive->setValue(tp->getInteractive() * 100);
    ui->barTeachWriting->setValue(tp->getWriting() * 100);
    ui->barTeachExams->setValue(tp->getExams() * 100);
}

void MainWindow::refreshStudentCoursesTable()
{
    ui->tblStudentCourses->setRowCount(m_courses.size());
    for (int row = 0; row < m_courses.size(); ++row) {
        const auto &c = m_courses[row];
        ui->tblStudentCourses->setItem(row, 0,
                                       new QTableWidgetItem(c.code));
        ui->tblStudentCourses->setItem(row, 1,
                                       new QTableWidgetItem(QString::number(c.grade, 'f', 1)));
        ui->tblStudentCourses->setItem(row, 2,
                                       new QTableWidgetItem(c.styleTags));
    }
}

void MainWindow::refreshTeacherTable()
{
    ui->tblTeachers->setRowCount(m_teachers.size());
    for (int row = 0; row < m_teachers.size(); ++row) {
        auto *t = m_teachers[row];
        ui->tblTeachers->setItem(row, 0,
                                 new QTableWidgetItem(t->getName()));
        ui->tblTeachers->setItem(row, 1,
                                 new QTableWidgetItem(t->getCourse()));
        ui->tblTeachers->setItem(row, 2,
                                 new QTableWidgetItem(t->getTags()));
    }
}

void MainWindow::refreshMatchesTable()
{
    ui->tblMatches->setRowCount(m_matches.size());
    for (int row = 0; row < m_matches.size(); ++row) {
        const auto &m = m_matches[row];
        ui->tblMatches->setItem(row, 0,
                                new QTableWidgetItem(QString::number(row + 1)));
        ui->tblMatches->setItem(row, 1,
                                new QTableWidgetItem(m.professor));
        ui->tblMatches->setItem(row, 2,
                                new QTableWidgetItem(QString::number(m.score * 100.0, 'f', 1) + "%"));
        ui->tblMatches->setItem(row, 3,
                                new QTableWidgetItem(m.why));
    }
}

void MainWindow::recomputeMatches(const QString &courseCode)
{
    m_matches.clear();

    for (auto *t : m_teachers) {
        if (!courseCode.isEmpty() &&
            t->getCourse().compare(courseCode, Qt::CaseInsensitive) != 0)
            continue;

        MatchRow mr;
        mr.professor  = t->getName();
        mr.courseCode = t->getCourse();
        mr.score      = t->computeScore();
        mr.why        = "Computed using virtual function scoring.";
        m_matches.push_back(mr);
    }

    std::sort(m_matches.begin(), m_matches.end(),
              [](const MatchRow &a, const MatchRow &b){
                  return a.score > b.score;
              });

    refreshMatchesTable();
}

void MainWindow::onUploadTranscriptClicked()
{
    QMessageBox::information(this, "Not Implemented",
                             "Upload Transcript clicked (placeholder).");
}

void MainWindow::onLoadCatalogClicked()
{
    QMessageBox::information(this, "Not Implemented",
                             "Load Course Catalog clicked (placeholder).");
}

void MainWindow::onLoadSyllabiClicked()
{
    QMessageBox::information(this, "Not Implemented",
                             "Load Syllabi Folder clicked (placeholder).");
}

void MainWindow::onLoadReviewsClicked()
{
    QMessageBox::information(this, "Not Implemented",
                             "Load Reviews CSV clicked (placeholder).");
}

void MainWindow::onTeacherSelectionChanged()
{
    int row = ui->tblTeachers->currentRow();
    if (row < 0 || row >= m_teachers.size())
        return;

    updateTeacherProfileBars(m_teachers[row]);
}

void MainWindow::onFindMatchClicked()
{
    const QString courseCode = ui->editCourseSearch->text().trimmed();
    recomputeMatches(courseCode);
}
