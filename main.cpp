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
#include <QFileInfo>
#include <QStringList>
#include <QSlider>

// taglib
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/tpropertymap.h>
#include <taglib/mpegfile.h>
#include <taglib/flacfile.h>
#include <taglib/oggfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/wavfile.h>
#include <taglib/mp4file.h>
#include <taglib/opusfile.h>

#include <queue>

QString version = "0.1.2";

class MusicQueue
{
public:
    void addToQueue(const QString &filePath)
    {
        queue.push_back(filePath); // Используем push_back для добавления элемента в конец очереди
    }

    QString removeFromQueue()
    {
        if (!queue.empty())
        {
            QString filePath = queue.front();
            queue.pop_front(); // Используем pop_front для удаления элемента из начала очереди
            return filePath;
        }
        return QString();
    }

    QStringList getQueue() const
    {
        return QStringList(queue.begin(), queue.end());
    }

    bool isQueueEmpty() const
    {
        return queue.empty();
    }


private:
    std::deque<QString> queue;
};

MusicQueue musicQueue;

QSlider* createSlider(QWidget *parent) {
    QSlider *slider = new QSlider(Qt::Horizontal, parent);
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setValue(0);
    return slider;
}

// defining global elements
QMediaPlayer *player = nullptr;
QAudioOutput *audioOutput = nullptr;
QPushButton *bplay = nullptr;
QImage *songimage = nullptr;
QImage *tmpimage = nullptr;
QLabel *songImageLabel = nullptr;
QLabel *songname = nullptr;
QLabel *songauthor = nullptr;
QSlider *slider = nullptr;

int thumbScale = 300;
bool nowPaused = false;

void onNext();
void onPrevious();
void onPause();
void open();
void setVolume();
void open();
void ohelp();
void opref();
void viewMusic();

void setTrackPositionFun(int position) {
    qint64 duration = player->duration();
    if (duration > 0) {
        qint64 newPosition = (position * duration) / 100;
        player->setPosition(newPosition);
    }
}

void onPositionChanged(qint64 position) {
    qreal pos = static_cast<qreal>(position);
    qreal duration = static_cast<qreal>(player->duration());

    qreal progressPercentage = (pos / duration) * 100.0;

    if (!slider->isSliderDown()) {
       slider->setValue(progressPercentage);
    }

    if (position >= duration) {
       onPause();
    }
}

// main
int main(int argc, char *argv[])
{
    // application
    QApplication app(argc, argv);
    app.setOrganizationDomain("slshptt.vercel.app");
    app.setApplicationName("potatoMusic");
    app.setApplicationVersion(version);

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
    QIcon ilist = QIcon::fromTheme("open-menu");
    QIcon imore = QIcon::fromTheme("view-more");
    QIcon ihelp = QIcon::fromTheme("help-contents");

    // toolbar
    QToolBar *toolbar = new QToolBar;

    QAction *openmusic = new QAction("Open");
    QAction *viewmusic = new QAction("View playlist");
    QAction *asetv = new QAction("Set volume");
    QAction *tpref = new QAction("Preferences");
    QAction *thelp = new QAction("Help");

    openmusic->setIcon(iopen);
    viewmusic->setIcon(ilist);
    asetv->setIcon(ivolume);
    tpref->setIcon(imore);
    thelp->setIcon(ihelp);

    openmusic->setToolTip("Open file");
    viewmusic->setToolTip("View playlist");
    asetv->setToolTip("Set volume level");
    tpref->setToolTip("Preferences");
    thelp->setToolTip("Help");

    toolbar->addAction(openmusic);
    toolbar->addAction(viewmusic);
    toolbar->addSeparator();
    toolbar->addAction(asetv);
    toolbar->addAction(tpref);
    toolbar->addAction(thelp);

    openmusic->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    asetv->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_V));

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
    big.setPointSize(19);
    QFont med;
    big.setPointSize(17);

    songname = new QLabel();
    songname->setText("title");
    songname->setAlignment(Qt::AlignCenter);
    songname->setFont(big);
    songname->setFixedWidth(thumbScale);

    songauthor = new QLabel();
    songauthor->setText("author");
    songauthor->setAlignment(Qt::AlignCenter);
    songauthor->setFont(med);
    songauthor->setFixedWidth(thumbScale);

    // player
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput; // chooses the default audio routing
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(1);

    // slider
    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setValue(0);

    // connecting
    QObject::connect(bprev, &QPushButton::clicked, onPrevious);
    QObject::connect(bplay, &QPushButton::clicked, onPause);
    QObject::connect(bnext, &QPushButton::clicked, onNext);
    QObject::connect(asetv, &QAction::triggered, setVolume);
    QObject::connect(tpref, &QAction::triggered, opref);
    QObject::connect(thelp, &QAction::triggered, ohelp);
    QObject::connect(openmusic, &QAction::triggered, open);
    QObject::connect(viewmusic, &QAction::triggered, viewMusic);
    QObject::connect(slider, &QSlider::sliderMoved, setTrackPositionFun);
    QObject::connect(player, &QMediaPlayer::positionChanged, onPositionChanged);

    // layout
    QHBoxLayout *playout = new QHBoxLayout(widget); // for hpadding
    QVBoxLayout *vlayout = new QVBoxLayout;         // for vpadding
    QGridLayout *glayout = new QGridLayout;         // for grid
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
    glayout->addWidget(slider, 2, 0, 1, 3);
    glayout->addWidget(songname, 3, 0, 1, 3);
    glayout->addWidget(songauthor, 4, 0, 1, 3);

    // run app
    widget->show();
    widget->resize(widget->width(), widget->height() + thumbScale);
    return app.exec();
}

// media buttons
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
    QIcon ipause = QIcon::fromTheme("media-playback-pause");
    QIcon iplay = QIcon::fromTheme("media-playback-start");

    if (nowPaused == true)
    {
        player->play();
        bplay->setIcon(ipause);
        nowPaused = false;
    }
    else
    {
        player->pause();
        bplay->setIcon(iplay);
        nowPaused = true;
    }
}

// action buttons
void viewMusic() {
    QString queueList;
    for (const QString &song : musicQueue.getQueue()) {
        queueList += song + "\n";
    }
    QMessageBox::information(nullptr, "Queue", queueList);
}

void setVolume()
{
    bool ok;
    int min = 0, max = 100, value = 100, step = 10;
    float value_int = QInputDialog::getInt(nullptr, "0-100", "Enter new volume:", value, min, max, step, &ok);
    float res = value_int / 100;
    audioOutput->setVolume(res);
}

void ohelp()
{
    QMessageBox msgBox;
    msgBox.setText("potatoMusic v" + version + "\n\nShortcuts:\nCtrl+O - open\nCtrl+V - set volume level\n\nhttps://github.com/slashpotato/potatoMusic");
    msgBox.exec();
}
void opref()
{
    //QMessageBox msgBox;
    //msgBox.setText();
    //msgBox.exec();
}

void open()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters({"Audio Files (*.mp3 *.ogg *.wav *.flac *.opus *.m4a)", "Any files (*)"});

    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        QString filePath = fileNames.first();
        for (const QString& filePath : fileNames)
        {
            // Добавить каждый выбранный файл в очередь воспроизведения
            musicQueue.addToQueue(filePath);
        }
        // Если очередь не пуста и воспроизведение не начато, начать воспроизведение
        if (!musicQueue.isQueueEmpty() && !player->isPlaying()) {
            QString firstSong = musicQueue.removeFromQueue();
            player->setSource(firstSong);

            // get album art
            QFileInfo fileInfo(filePath);
            QString ext = fileInfo.suffix().toLower();

            if (nowPaused)
            {
                onPause();
            }

            if (ext == "mp3")
            {
                TagLib::MPEG::File mp3File(filePath.toUtf8().constData());
                if (!mp3File.isValid())
                {
                    QMessageBox::warning(nullptr, "Error", "Invalid MP3 file.");
                    return;
                }

                const TagLib::ID3v2::FrameList &frameList = mp3File.ID3v2Tag()->frameListMap()["APIC"];
                if (!frameList.isEmpty())
                {
                    TagLib::ID3v2::AttachedPictureFrame *frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());
                    if (frame)
                    {
                        TagLib::ByteVector imageData = frame->picture();
                        QPixmap pixmap;
                        pixmap.loadFromData(reinterpret_cast<const uchar *>(imageData.data()), imageData.size());
                        if (!pixmap.isNull())
                        {
                            songImageLabel->setPixmap(pixmap.scaled(thumbScale, thumbScale, Qt::KeepAspectRatio));
                        }
                    }
                }

                QString title = QString::fromStdString(mp3File.tag()->title().toCString(true));
                QString artist = QString::fromStdString(mp3File.tag()->artist().toCString(true));
                songname->setText(title);
                songauthor->setText(artist);
            }
            else if (ext == "flac")
            {
                TagLib::FLAC::File flacFile(filePath.toUtf8().constData());
                if (!flacFile.isValid())
                {
                    QMessageBox::warning(nullptr, "Error", "Invalid FLAC file.");
                    return;
                }

                const TagLib::List<TagLib::FLAC::Picture *> &pictureList = flacFile.pictureList();
                if (!pictureList.isEmpty())
                {
                    const TagLib::FLAC::Picture *picture = pictureList.front();
                    if (picture)
                    {
                        TagLib::ByteVector imageData = picture->data();
                        QPixmap pixmap;
                        pixmap.loadFromData(reinterpret_cast<const uchar *>(imageData.data()), imageData.size());
                        if (!pixmap.isNull())
                        {
                            songImageLabel->setPixmap(pixmap.scaled(thumbScale, thumbScale, Qt::KeepAspectRatio));
                        }
                    }
                }

                QString title = QString::fromStdString(flacFile.tag()->title().toCString(true));
                QString artist = QString::fromStdString(flacFile.tag()->artist().toCString(true));
                songname->setText(title);
                songauthor->setText(artist);
            }
            else if (ext == "ogg" || ext == "wav" || ext == "m4a" || ext == "opus")
            {
                TagLib::FileRef fileRef(filePath.toUtf8().constData());
                if (!fileRef.isNull() && fileRef.tag())
                {
                    TagLib::Tag *tag = fileRef.tag();
                    QString title = QString::fromStdString(tag->title().toCString(true));
                    QString artist = QString::fromStdString(tag->artist().toCString(true));
                    songname->setText(title);
                    songauthor->setText(artist);

                    TagLib::PropertyMap properties = tag->properties();
                    if (properties.contains("WM/Picture"))
                    {
                        TagLib::String pictureData = properties["WM/Picture"].toString();
                        QString pictureDataQString = QString::fromStdString(pictureData.to8Bit(true).data());
                        QPixmap pixmap;
                        pixmap.loadFromData(reinterpret_cast<const uchar *>(pictureDataQString.toUtf8().constData()), pictureDataQString.length());
                        if (!pixmap.isNull())
                        {
                            songImageLabel->setPixmap(pixmap.scaled(thumbScale, thumbScale, Qt::KeepAspectRatio));
                        }
                    }
                }
            }
            else
            {
                QMessageBox::warning(nullptr, "Warning", "Unsupported file format: metadata won't be loaded, but most likely played."); // :)
                return;
            }
        }
    }
}
