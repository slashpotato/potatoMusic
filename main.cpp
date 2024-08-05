#include "mainwindow.h"
#include <queue>
#include <iostream>

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
#include <QDialog>
#include <QDialogButtonBox>
#include <QFont>
#include <QDirIterator>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QImage>
#include <QByteArray>
#include <QProcess>

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
#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <taglib/mp4coverart.h>

// version
QString version = "0.4.1";

// deleted queue
class RemovedTracksQueue {
public:
    void addToQueue(const QString &filePath) {
        queue.push_back(filePath);
    }

    QString removeFromQueue() {
        if (!queue.empty()) {
            QString filePath = queue.front();
            queue.pop_front();
            return filePath;
        }
        return QString();
    }

    QStringList getQueue() const {
        return QStringList(queue.begin(), queue.end());
    }

    bool isQueueEmpty() const {
        return queue.empty();
    }

private:
    std::deque<QString> queue;
};

// Глобальный объект очереди для удаленных треков
RemovedTracksQueue removedTracksQueue;

// queue
class MusicQueue
{
public:
    void addToQueue(const QString &filePath)
    {
        queue.push_back(filePath);
    }

    /*void restoreLastRemovedTrack()
    {
        if (!removedTracks.empty()) {
            QString lastRemovedTrack = removedTracks.back();
            removedTracks.pop_back();
            queue.push_back(lastRemovedTrack);
        }
    }*/

    QString removeFromQueue()
    {
        if (!queue.empty())
        {
            QString filePath = queue.front();
            queue.pop_front();
            removedTracksQueue.addToQueue(filePath);
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

// slider
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
QPushButton *bmix = nullptr;
QPushButton *bcon = nullptr;
QImage *songimage = nullptr;
QImage *tmpimage = nullptr;
QLabel *songImageLabel = nullptr;
QLabel *songname = nullptr;
QLabel *songauthor = nullptr;
QSlider *slider = nullptr;
QLabel *elapsedTimeLabel = nullptr;
QLabel *totalTimeLabel = nullptr;

int thumbScale = 299;
bool nowPaused = false;

void onNext();
void onPrevious();
void onPause();
void open();
void setVolume();
void ohelp();
void opref();
void playNextTrack();
void viewTracks();

// funs
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

    QTime elapsedTime(0, 0);
    elapsedTime = elapsedTime.addMSecs(position);
    elapsedTimeLabel->setText(elapsedTime.toString("mm:ss"));



    if (position >= duration) {
       playNextTrack();
    }
}

// fonts
QFont fbig;
QFont fmed;

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

    fbig.setPointSize(19);
    fmed.setPointSize(14);

    // icons
    QIcon iforward = QIcon::fromTheme("media-skip-forward");
    QIcon ibackward = QIcon::fromTheme("media-skip-backward");
    QIcon ipause = QIcon::fromTheme("media-playback-pause", QIcon(":/icons/pause.svg"));
    QIcon iplay = QIcon::fromTheme("media-playback-start", QIcon(":/icons/play.svg"));
    QIcon iicon = QIcon::fromTheme("audio-headphones", QIcon(":/icons/headphones.png"));
    QIcon ivolume = QIcon::fromTheme("audio-volume-high", QIcon(":/icons/audio.png"));
    QIcon iopen = QIcon::fromTheme("folder-saved-search", QIcon(":/icons/folder.png"));
    QIcon ilist = QIcon::fromTheme("open-menu", QIcon(":/icons/menu.png"));
    QIcon imore = QIcon::fromTheme("settings-symbolic", QIcon(":/icons/settings.png"));
    QIcon ihelp = QIcon::fromTheme("help-contents", QIcon(":/icons/help.png"));

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
    bprev->setShortcut(QKeySequence(Qt::Key_MediaPrevious));
    bplay->setShortcut(QKeySequence(Qt::Key_Space));
    bnext->setShortcut(QKeySequence(Qt::Key_MediaNext));
    bplay->setShortcut(QKeySequence(Qt::Key_MediaTogglePlayPause));

    // image
    songImageLabel = new QLabel;

    // labels
    songname = new QLabel();
    songname->setText("title");
    songname->setAlignment(Qt::AlignCenter);
    songname->setFont(fbig);
    songname->setFixedWidth(thumbScale);

    songauthor = new QLabel();
    songauthor->setText("author");
    songauthor->setAlignment(Qt::AlignCenter);
    songauthor->setFont(fmed);
    songauthor->setFixedWidth(thumbScale);

    elapsedTimeLabel = new QLabel("00:00");
    totalTimeLabel = new QLabel("00:00");

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

    // tray
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(widget);
    trayIcon->setIcon(QIcon::fromTheme("headphones-symbolic")); // Установите иконку из ресурсов
    trayIcon->show();

    QMenu *trayMenu = new QMenu(widget);
    QAction *playPauseAction = new QAction("Play/Pause", widget);
    QObject::connect(playPauseAction, &QAction::triggered, bplay, &QPushButton::click);
    trayMenu->addAction(playPauseAction);
    trayIcon->setContextMenu(trayMenu);

    // connecting
    QObject::connect(bprev, &QPushButton::clicked, onPrevious);
    QObject::connect(bplay, &QPushButton::clicked, onPause);
    QObject::connect(bnext, &QPushButton::clicked, onNext);
    QObject::connect(asetv, &QAction::triggered, setVolume);
    QObject::connect(tpref, &QAction::triggered, opref);
    QObject::connect(thelp, &QAction::triggered, ohelp);
    QObject::connect(openmusic, &QAction::triggered, open);
    QObject::connect(viewmusic, &QAction::triggered, viewTracks);
    QObject::connect(slider, &QSlider::sliderMoved, setTrackPositionFun);
    QObject::connect(player, &QMediaPlayer::positionChanged, onPositionChanged);

    // layout
    QHBoxLayout *playout = new QHBoxLayout(widget); // for hpadding
    QVBoxLayout *vlayout = new QVBoxLayout;         // for vpadding
    QGridLayout *glayout = new QGridLayout;         // for grid
    QHBoxLayout *slayout = new QHBoxLayout;         // for slider
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
    glayout->addLayout(slayout, 2, 0, 1, 3);
    glayout->addWidget(songname, 3, 0, 1, 3);
    glayout->addWidget(songauthor, 4, 0, 1, 3);

    slayout->addWidget(elapsedTimeLabel);
    slayout->addWidget(slider);
    slayout->addWidget(totalTimeLabel);

    // run app
    widget->show();
    widget->resize(widget->width(), widget->height() + thumbScale);
    widget->setMinimumHeight(widget->height());
    widget->setMinimumWidth(widget->width());
    return app.exec();
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

// get track meta with ffmpeg

QPixmap extractCoverArt(const QString &filePath) {
    QStringList args;
    args << "-i" << filePath << "-an" << "-vcodec" << "copy" << "-f" << "image2pipe" << "-";

    QProcess process;
    process.start("ffmpeg", args);
    process.waitForFinished();

    QByteArray imageData = process.readAllStandardOutput();
    QImage image = QImage::fromData(imageData);

    if (image.isNull()) {
        return QPixmap();
    }

    QPixmap pixmap = QPixmap::fromImage(image.scaled(thumbScale, thumbScale, Qt::KeepAspectRatio));
    return pixmap;
}

// load ( and play ) track

void play(const QString &filePath)
{
    player->setSource(filePath);

    QFileInfo fileInfo(filePath);
    QString ext = fileInfo.suffix().toLower();

    if (nowPaused)
    {
        onPause();
    }
    onPause();
    onPause();

    qint64 duration = player->duration();
    qint64 secondsTotal = duration / 1000;
    int minutes = secondsTotal / 60;
    int seconds = secondsTotal % 60;
    QString timeString = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
    totalTimeLabel->setText(timeString);

    // image. p.s. i hate this part i spent most time on it

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
    else
    {
        TagLib::FileRef fileRef(filePath.toUtf8().constData());
        if (!fileRef.isNull() && fileRef.tag())
        {
            TagLib::Tag *tag = fileRef.tag();
            QString title = QString::fromStdString(tag->title().toCString(true));
            QString artist = QString::fromStdString(tag->artist().toCString(true));
            songname->setText(title);
            songauthor->setText(artist);

            QPixmap coverArt = extractCoverArt(filePath);
            if (!coverArt.isNull()) {
                songImageLabel->setPixmap(coverArt.scaled(thumbScale, thumbScale, Qt::KeepAspectRatio));
            } else {
                songImageLabel->setText("No cover art available");
            }
        }
    }
}

void open()
{
    QMenu menu;
    QAction *openFilesAction = menu.addAction("Open Files");
    QAction *openDirectoryAction = menu.addAction("Open Directory");

    QAction *selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == openFilesAction)
    {
        // Обработка выбора "Open Files"
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::ExistingFiles);
        dialog.setViewMode(QFileDialog::Detail);
        dialog.setNameFilters({"Audio Files (*.mp3 *.ogg *.wav *.flac *.opus *.m4a)", "Any files (*)"});

        dialog.setOption(QFileDialog::DontUseNativeDialog, true);
        dialog.setOption(QFileDialog::ReadOnly, true);

        if (dialog.exec())
        {
            QStringList fileNames = dialog.selectedFiles();
            musicQueue = MusicQueue();

            for (const QString& fileName : fileNames)
            {
                QFileInfo fileInfo(fileName);
                if (fileInfo.isFile()) {
                    musicQueue.addToQueue(fileName);
                } else if (fileInfo.isDir()) {
                    QDirIterator it(fileName, QStringList() << "*.mp3" << "*.ogg" << "*.wav" << "*.flac" << "*.opus" << "*.m4a", QDir::Files, QDirIterator::Subdirectories);
                    while (it.hasNext()) {
                        QString filePath = it.next();
                        musicQueue.addToQueue(filePath);
                    }
                }
            }

            if (!musicQueue.isQueueEmpty() && !player->isPlaying())
            {
                playNextTrack();
            }
        }
    }
    else if (selectedAction == openDirectoryAction)
    {
        // Обработка выбора "Open Directory"
        QString directoryPath = QFileDialog::getExistingDirectory(nullptr, "Open Directory", QDir::homePath());

        if (!directoryPath.isEmpty())
        {
            QDirIterator it(directoryPath, QStringList() << "*.mp3" << "*.ogg" << "*.wav" << "*.flac" << "*.opus" << "*.m4a", QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                QString filePath = it.next();
                musicQueue.addToQueue(filePath);
            }

            if (!musicQueue.isQueueEmpty() && !player->isPlaying())
            {
                playNextTrack();
            }
        }
    }
}

void playNextTrack()
{
    if (!musicQueue.isQueueEmpty())
    {
        QString nextSong = musicQueue.removeFromQueue();
        //removedTracksList.addTrack(nextSong);
        play(nextSong);
    }
    else
    {
        QMessageBox::information(nullptr, "Queue", "Queue is empty.");
    }
    onPause();
    onPause();
}

// media buttons
void onNext()
{
    playNextTrack();
}

void onPrevious()
{
    if (!removedTracksQueue.isQueueEmpty()) {
        QString lastRemovedTrack = removedTracksQueue.removeFromQueue();
        musicQueue.addToQueue(lastRemovedTrack);
        play(lastRemovedTrack);
    } else {
        QMessageBox::information(nullptr, "Removed Tracks", "No tracks to restore.");
    }
}

void viewTracks()
{
    QMenu menu;
    QAction *viewQueueAction = menu.addAction("View Queue");
    QAction *viewRemovedTracksAction = menu.addAction("View Removed Tracks");

    QAction *selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == viewQueueAction)
    {
        // Обработка выбора "View Queue"
        QStringList queueList;
        for (const QString &song : musicQueue.getQueue())
        {
            // Добавление треков из основной очереди в список
            QString title;
            QString artist;
            TagLib::FileRef file(song.toUtf8().constData());
            if (!file.isNull() && file.tag())
            {
                TagLib::Tag *tag = file.tag();
                title = QString::fromStdString(tag->title().toCString(true));
                if (title.isEmpty())
                {
                    QFileInfo fileInfo(song);
                    title = fileInfo.fileName();
                }
            }
            else
            {
                QFileInfo fileInfo(song);
                title = fileInfo.fileName();
            }
            queueList.append(title);
        }
        QString queueText = queueList.join("\n");
        QMessageBox msgBox;
        msgBox.setText(queueText);
        msgBox.setMaximumWidth(500);
        msgBox.setWindowTitle("Queue");
        msgBox.exec();
    }
    else if (selectedAction == viewRemovedTracksAction)
    {
        QStringList removedTracksList = removedTracksQueue.getQueue();
        QString removedTracksText;

        for (const QString &filePath : removedTracksList)
        {
            TagLib::FileRef file(filePath.toUtf8().constData());
            if (!file.isNull() && file.tag())
            {
                TagLib::Tag *tag = file.tag();
                QString title = QString::fromStdString(tag->title().toCString(true));
                if (title.isEmpty())
                {
                    QFileInfo fileInfo(filePath);
                    title = fileInfo.fileName();
                }
                removedTracksText += title + "\n";
            }
            else
            {
                QFileInfo fileInfo(filePath);
                removedTracksText += fileInfo.fileName() + "\n";
            }
        }

        QMessageBox msgBox;
        msgBox.setText(removedTracksText);
        msgBox.setMaximumWidth(500);
        msgBox.setWindowTitle("Removed Tracks");
        msgBox.exec();
    }
}

void setVolume()
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(100);
    if (audioOutput->volume() == 0) {
        slider->setValue(audioOutput->volume() * 100.0);
    } else {
        slider->setValue(audioOutput->volume() * 100.0 +1);
    }

    QLabel *slidervalue = new QLabel(QString::number(slider->value()) + "%");

    QDialog dialog;
    dialog.setWindowTitle("Set Volume");
    dialog.setFixedSize(300, 70);
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QHBoxLayout *hlayout = new QHBoxLayout;
    layout->addLayout(hlayout);
    hlayout->addWidget(slider);
    hlayout->addWidget(slidervalue);
    layout->addStretch(1);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    layout->addWidget(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QObject::connect(slider, &QSlider::valueChanged, [&slidervalue](int value) {
        slidervalue->setText(QString::number(value) + "%");
    });

    if (dialog.exec() == QDialog::Accepted) {
        float volume = slider->value() / 100.0;
        audioOutput->setVolume(volume);
    }

    delete slider;
}

void ohelp()
{
    QMessageBox msgBox;
    msgBox.setText("potatoMusic v" + version + "\n\nShortcuts:\nCtrl+O - open\nCtrl+V - set volume level\n\nhttps://github.com/slashpotato/potatoMusic");
    msgBox.exec();
}
void opref()
{
    QMessageBox msgBox;
    msgBox.setText("");
    msgBox.exec();
}
