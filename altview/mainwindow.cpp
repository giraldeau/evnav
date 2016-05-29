#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>
#include <QLabel>
#include <QScrollArea>
#include <QFileDialog>
#include <QMenu>
#include <QRgb>
#include <QAction>
#include <QPixmap>

#include <elevationtile.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_label(new QLabel),
    m_scrollArea(new QScrollArea)
{
    ui->setupUi(this);
    m_label->setBackgroundRole(QPalette::Base);
    m_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_label->setScaledContents(true);

    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(m_label);
    m_scrollArea->setVisible(false);
    setCentralWidget(m_scrollArea);
    createActions();
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    QAction *exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}

void MainWindow::open()
{
    QFileDialog dialog(this, tr("Open File"));
    while(dialog.exec() == QDialog::Accepted &&
          !loadFile(dialog.selectedFiles().first())) {}
}

bool MainWindow::loadFile(const QString &path)
{
    ElevationTile tile(this);
    tile.load(path);

    QImage img(tile.width(), tile.height(), QImage::Format_Grayscale8);
    img.fill(Qt::white);

    QRgb value;
    for (int y = 0; y < tile.height(); y++) {
        for (int x = 0; x < tile.width(); x++) {
            int alt = tile.valueAt(x, y);
            alt = qBound(0, alt, 255);
            value = qRgb(alt, alt, alt);
            img.setPixel(x, y, value);
        }
    }

    m_label->setPixmap(QPixmap::fromImage(img));
    m_scrollArea->setVisible(true);
    m_label->adjustSize();
    return true;
}
