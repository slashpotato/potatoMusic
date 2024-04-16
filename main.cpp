#include "mainwindow.h"

// qt
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>
#include <QToolBar>
#include <QLabel>
#include <QtMultimedia/QMediaPlayer>
#include <QInputDialog>
#include <QAudioOutput>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

// taglib
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>

// defining global elements
QMediaPlayer* player = nullptr;
QAudioOutput* audioOutput = nullptr;
QPushButton* bplay = nullptr;
QImage* songimage = nullptr;
QImage* tmpimage = nullptr;
QLabel *songImageLabel = nullptr;

bool nowPaused = false;

void onNext(); void onPrevious(); void onPause(); void open(); void setVolume(); void open();

// main
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
    QIcon iopen = QIcon::fromTheme("folder-saved-search");
    QIcon ilist = QIcon::fromTheme("view-list");

    // toolbar
    QToolBar *toolbar = new QToolBar;

    QAction *openmusic = new QAction("Open");
    QAction *viewmusic = new QAction("View playlist");
    QAction *asetv = new QAction("Set volume");

    openmusic->setIcon(iopen);
    viewmusic->setIcon(ilist);
    asetv->setIcon(ivolume);

    openmusic->setToolTip("Open file");
    viewmusic->setToolTip("View playlist");
    asetv->setToolTip("Set player volume (0-100)");

    toolbar->addAction(openmusic);
    toolbar->addAction(viewmusic);
    toolbar->addSeparator();
    toolbar->addAction(asetv);

    // app icon
    widget->setWindowIcon(iicon);
    app.setWindowIcon(iicon);

    // buttons
    QPushButton *bprev = new QPushButton(ibackward, "");
    bplay = new QPushButton(iplay, "");
    QPushButton *bnext = new QPushButton(iforward, "");

    // image
    songImageLabel = new QLabel;

    // labels
    QFont big;
    big.setPointSize(14);

    QLabel *songname = new QLabel();
    songname->setText("title");
    songname->setAlignment(Qt::AlignCenter);
    songname->setFont(big);
    songname->setFixedWidth(250);

    QLabel *songauthor = new QLabel();
    songauthor->setText("author");
    songauthor->setAlignment(Qt::AlignCenter);
    songauthor->setFixedWidth(250);

    // player
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput; // chooses the default audio routing
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(1);

    // connecting
    QObject::connect(bprev, &QPushButton::clicked, onPrevious);
    QObject::connect(bplay, &QPushButton::clicked, onPause);
    QObject::connect(bnext, &QPushButton::clicked, onNext);
    QObject::connect(asetv, &QAction::triggered, setVolume);
    QObject::connect(openmusic, &QAction::triggered, open);

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

    glayout->addWidget(songImageLabel, 0, 0, 1, 3);
    glayout->addWidget(songname, 2, 0, 1, 3);
    glayout->addWidget(songauthor, 3, 0, 1, 3);

    // run app
    widget->show();
    return app.exec();
}

// media buttons
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
    QIcon ipause = QIcon::fromTheme("media-playback-pause");
    QIcon iplay = QIcon::fromTheme("media-playback-start");

    if (nowPaused == true) {
        player->play();
        bplay->setIcon(ipause);
        nowPaused = false;
    } else {
        player->pause();
        bplay->setIcon(iplay);
        nowPaused = true;
    }
}

// action buttons
void setVolume() {
    bool ok;
    int min = 0, max = 100, value = 100, step = 10;
    float value_int = QInputDialog::getInt(nullptr, "0-100", "Enter new volume:", value, min, max, step, &ok);
    float res = value_int / 100;
    audioOutput->setVolume(res);
}

void open() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters({"Audio Files (*.mp3 *.ogg *.wav *.flac *.opus *.aac *.m4a *.mid *.midi)", "Any files (*)"});

    if (dialog.exec()) {
        QStringList fileNames = dialog.selectedFiles();
        QString filePath = fileNames.first();
        player->setSource(filePath);

        // Получение изображения альбома используя TagLib API
        TagLib::MPEG::File file(filePath.toStdString().c_str());
        TagLib::ID3v2::Tag *tag = file.ID3v2Tag();
        TagLib::ID3v2::FrameList frameList = tag->frameListMap()["APIC"];
        if (!frameList.isEmpty()) {
            TagLib::ID3v2::AttachedPictureFrame *coverImg = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());
            if (coverImg) {
                QByteArray imageData(coverImg->picture().data(), coverImg->picture().size());
                QPixmap pixmap;
                pixmap.loadFromData(imageData);
                if (!pixmap.isNull()) {
                    songImageLabel->setPixmap(pixmap.scaled(200, 200, Qt::KeepAspectRatio));
                }
            }
        }
    }

    if (nowPaused) {
        onPause();
    }
}
