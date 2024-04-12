#include "mainwindow.h"

// qt elements
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QIcon>
#include <QToolBar>
#include <QToolButton>
#include <QMenuBar>

// FMOD
/*#include <conio.h>
#include "inc/fmod.h"*/

void onPlay(); void onNext(); void onPrevious(); void onPause(); void open();
int main(int argc, char *argv[])
{
    // application
    QApplication app(argc, argv);
    app.setOrganizationDomain("slshptt.vercel.app");
    app.setApplicationName("potatoMusic");
    app.setApplicationVersion("0.0.7");

    // main widget
    QWidget *widget = new QWidget;
    widget->setWindowTitle("potatoMusic");
    widget->setMinimumWidth(350);
    widget->setMinimumHeight(150);

    // toolbar
    QToolBar *toolbar = new QToolBar;
    toolbar->addAction("Open");

    // icons
    QIcon iforward = QIcon::fromTheme("media-skip-forward");
    QIcon ibackward = QIcon::fromTheme("media-skip-backward");
    QIcon ipause = QIcon::fromTheme("media-playback-pause");
    QIcon iplay = QIcon::fromTheme("media-playback-start");
    QIcon iicon = QIcon::fromTheme("audio-headphones");

    // app icon
    widget->setWindowIcon(iicon);
    app.setWindowIcon(iicon);

    // buttons
    QPushButton *prev = new QPushButton(ibackward, "");
    QPushButton *play = new QPushButton(iplay, "");
    QPushButton *next = new QPushButton(iforward, "");

    QPushButton *placeholder = new QPushButton("placeholder");

    // connecting
    QObject::connect(prev, &QPushButton::clicked, onPrevious);
    QObject::connect(play, &QPushButton::clicked, onPlay);
    QObject::connect(next, &QPushButton::clicked, onNext);

    // layout
    QHBoxLayout *playout = new QHBoxLayout(widget); // for padding
    playout->setMenuBar(toolbar);

    QGridLayout *glayout = new QGridLayout; // for grid
    playout->addStretch(1);

    playout->addLayout(glayout);

    glayout->addWidget(placeholder, 0, 0, 1, 3);
    glayout->addWidget(prev, 1, 0);
    glayout->addWidget(play, 1, 1);
    glayout->addWidget(next, 1, 2);

    playout->addStretch(1);

    // run app
    widget->show();
    return app.exec();
}

void onPlay()
{
    QMessageBox msgBox;
    msgBox.setText("play");
    msgBox.exec();
}
void onNext()
{
    QMessageBox msgBox;
    msgBox.setText("next");
    msgBox.exec();
}
void onPrevious()
{
    QMessageBox msgBox;
    msgBox.setText("previous");
    msgBox.exec();
}
void onPause()
{
    QMessageBox msgBox;
    msgBox.setText("pause");
    msgBox.exec();
}
