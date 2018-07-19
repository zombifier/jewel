#ifndef MEDIAPLAYER2ADAPTOR_H
#define MEDIAPLAYER2ADAPTOR_H

#include <QString>
#include <QStringList>
#include <QtDBus/QDBusAbstractAdaptor>
#include "musiccontroller.h"

class MediaPlayer2Adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
    Q_PROPERTY(bool CanQuit READ CanQuit)
    //Q_PROPERTY(bool Fullscreen READ Fullscreen WRITE setFullscreen)
    //Q_PROPERTY(bool CanSetFullscreen READ CanSetFullscreen)
    Q_PROPERTY(bool CanRaise READ CanRaise)
    Q_PROPERTY(bool HasTrackList READ HasTrackList)
    Q_PROPERTY(QString Identity READ Identity)
    Q_PROPERTY(QString DesktopEntry READ DesktopEntry)
    Q_PROPERTY(QStringList SupportedUriSchemes READ SupportedUriSchemes)
    Q_PROPERTY(QStringList SupportedMimeTypes READ SupportedMimeTypes)
public:
    explicit MediaPlayer2Adaptor(QObject *parent = nullptr);
    bool CanQuit();
    bool CanRaise();
    bool HasTrackList();
    QString Identity();
    QString DesktopEntry();
    QStringList SupportedUriSchemes();
    QStringList SupportedMimeTypes();
public slots:
    void Raise();
    void Quit();
};

#endif // MEDIAPLAYER2ADAPTOR_H
