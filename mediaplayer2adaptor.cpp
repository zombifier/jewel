#include "mediaplayer2adaptor.h"
#include <QMimeDatabase>
#include <QMimeType>
#include <QGuiApplication>

MediaPlayer2Adaptor::MediaPlayer2Adaptor(QObject *parent) : QDBusAbstractAdaptor(parent)
{
}

bool MediaPlayer2Adaptor::CanQuit() {
    return true;
}

bool MediaPlayer2Adaptor::CanRaise() {
    return true;
}

bool MediaPlayer2Adaptor::HasTrackList() {
    return false;
}

QString MediaPlayer2Adaptor::Identity() {
    return QString("Jewel Tracker Player");
}

QString MediaPlayer2Adaptor::DesktopEntry() {
    return QString("jewel");
}

QStringList MediaPlayer2Adaptor::SupportedUriSchemes() {
    return QStringList{"file"};
}

QStringList MediaPlayer2Adaptor::SupportedMimeTypes() {
    QMimeDatabase mimeDatabase;
    QStringList mimeList;
    bool hasDefault = false;
    for (std::string extension : openmpt::get_supported_extensions()) {
        QString name = "name.";
        name.append(extension);
        QMimeType mimeType = mimeDatabase.mimeTypeForFile(name, QMimeDatabase::MatchExtension);
        if (mimeType.isDefault()) {
            if (!hasDefault) {
                mimeList.append(mimeType.name());
                hasDefault = true;
            }
        }
        else {
            mimeList.append(mimeType.name());
        }
    }
    return mimeList;
}

void MediaPlayer2Adaptor::Raise() {
    emit MusicController::getInstance()->raiseWindow();
}

void MediaPlayer2Adaptor::Quit() {
    QGuiApplication::exit();
}
