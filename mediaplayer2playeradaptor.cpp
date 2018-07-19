#include "mediaplayer2playeradaptor.h"

MediaPlayer2PlayerAdaptor::MediaPlayer2PlayerAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent)
{
}

QString MediaPlayer2PlayerAdaptor::PlaybackStatus() {
    if (MusicController::getInstance()->isPlaying()) {
        return "Playing";
    }
    else {
        return "Paused";
    }
}

QString MediaPlayer2PlayerAdaptor::LoopStatus() {
    return "Track";
}

double MediaPlayer2PlayerAdaptor::Rate() {
    return MusicController::getInstance()->getTempoFactor();
}

bool MediaPlayer2PlayerAdaptor::Shuffle() {
    return false;
}

QVariantMap MediaPlayer2PlayerAdaptor::Metadata() {
}

QString MediaPlayer2PlayerAdaptor::PlaybackStatus() {
}
