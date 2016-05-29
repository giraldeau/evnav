#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
class QLabel;
class QScrollArea;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool loadFile(const QString &path);

private slots:
    void open();

private:
    void createActions();

    Ui::MainWindow *ui;
    QImage m_image;
    QLabel *m_label;
    QScrollArea *m_scrollArea;

};

#endif // MAINWINDOW_H
