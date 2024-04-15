#include "mainwindow.h"

// qt elements
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QIcon>
#include <QToolBar>
#include <QToolButton>
#include <QMenuBar>
#include <QLabel>
#include <QtMultimedia/QMediaPlayer>
#include <QInputDialog>
#include <QMessageBox>

// FMOD
/*#include <conio.h>
#include "inc/fmod.h"*/

void onPlay(); void onNext(); void onPrevious(); void onPause(); void open(); void setVolume();
int main(int argc, char *argv[]) {
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

    // icons
    QIcon iforward = QIcon::fromTheme("media-skip-forward");
    QIcon ibackward = QIcon::fromTheme("media-skip-backward");
    QIcon ipause = QIcon::fromTheme("media-playback-pause");
    QIcon iplay = QIcon::fromTheme("media-playback-start");
    QIcon iicon = QIcon::fromTheme("audio-headphones");
    QIcon ivolume = QIcon::fromTheme("audio-volume-high");
    QIcon iopen = QIcon::fromTheme("document-open");
    QIcon ilist = QIcon::fromTheme("view-list");

    // toolbar
    QToolBar *toolbar = new QToolBar;

    QAction *openmusic = new QAction("Open");
    QAction *viewmusic = new QAction("View playlist");
    QAction *asetv = new QAction("Set volume");

    openmusic->setIcon(iopen);
    viewmusic->setIcon(ilist);
    asetv->setIcon(ivolume);

    toolbar->addAction(openmusic);
    toolbar->addAction(viewmusic);
    toolbar->addSeparator();
    toolbar->addAction(asetv);


    // app icon
    widget->setWindowIcon(iicon);
    app.setWindowIcon(iicon);

    // buttons
    QPushButton *bprev = new QPushButton(ibackward, "");
    QPushButton *bplay = new QPushButton(iplay, "");
    QPushButton *bnext = new QPushButton(iforward, "");

    // image
    QLabel *songimage = new QLabel();
    QImage tmpimage;
    tmpimage.load("/home/user/test.png");

    QImage image = tmpimage.scaled(QSize(250, 250));
    songimage->setPixmap(QPixmap::fromImage(image));

    // labels
    QFont big;
    big.setPointSize(14);

    QLabel *songname = new QLabel();
    songname->setText("testo songo");
    songname->setAlignment(Qt::AlignCenter);
    songname->setFont(big);
    songname->setFixedWidth(250);

    QLabel *songauthor = new QLabel();
    songauthor->setText("potatiotato");
    songauthor->setAlignment(Qt::AlignCenter);
    songauthor->setFixedWidth(250);

    // connecting
    QObject::connect(bprev, &QPushButton::clicked, onPrevious);
    QObject::connect(bplay, &QPushButton::clicked, onPlay);
    QObject::connect(bnext, &QPushButton::clicked, onNext);
    QObject::connect(asetv, &QAction::triggered, setVolume);

    // player
   /*auto player = new QMediaPlayer;
    auto audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    player->setSource(QUrl::fromLocalFile("/home/user/Music/Medlyak.mp3"));
    audioOutput->setVolume(50);
    player->play();*/

    //QMediaPlayer player;
    //QAudioOutput audioOutput; // chooses the default audio routing
    //player.setAudioOutput(&audioOutput);
    //player.setSource(QUrl::fromLocalFile("/home/user/Music/Medlyak.mp3"));
    //audioOutput.setVolume(50);
    //player.play();



    // layout
    QHBoxLayout *playout = new QHBoxLayout(widget); // for hpadding
    QVBoxLayout *vlayout = new QVBoxLayout; // for vpadding
    QGridLayout *glayout = new QGridLayout; // for grid
    playout->setMenuBar(toolbar);

    // do hpadding
    playout->addStretch(1);
    playout->addLayout(vlayout);
    playout->addStretch(1);

    // do vpadding
    vlayout->addStretch(1);
    vlayout->addLayout(glayout);
    vlayout->addStretch(1);

    // add main widgets
    glayout->addWidget(bprev, 1, 0);
    glayout->addWidget(bplay, 1, 1);
    glayout->addWidget(bnext, 1, 2);

    glayout->addWidget(songimage, 0, 0, 1, 3);
    glayout->addWidget(songname, 2, 0, 1, 3);
    glayout->addWidget(songauthor, 3, 0, 1, 3);

    // run app
    widget->show();
    return app.exec();
}

void onPlay() {
    QMessageBox msgBox;
    msgBox.setText("play");
    msgBox.exec();
}
void onNext() {
    QMessageBox msgBox;
    msgBox.setText("next");
    msgBox.exec();
}
void onPrevious() {
    QMessageBox msgBox;
    msgBox.setText("previous");
    msgBox.exec();
}
void onPause() {
    QMessageBox msgBox;
    msgBox.setText("pause");
    msgBox.exec();
}

void setVolume() {
    QInputDialog *dialog = new QInputDialog();
    dialog->setWindowTitle("0-100");
    dialog->show();
}
