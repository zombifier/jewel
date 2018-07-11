#ifndef MUSICCONTROLLER_H
#define MUSICCONTROLLER_H

#include <QObject>
#include <QUrl>
#include <QVariantList>
#include <QVector>
#include <QStringList>
#include <libopenmpt/libopenmpt.hpp>
#include <libopenmpt/libopenmpt_ext.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>

/**
 * @brief This class provides an interface for QML classes to control music playback
 * as well as get info about the track.
 * Right now it is a superclass that does pretty much everything (mostly because
 * the module class of libopenmpt also does pretty much everything). We may split it
 * into multiple subclasses further down the line, but the code is pretty readable as-is.
 */
class MusicController : public QObject
{
    Q_OBJECT
public:
    explicit MusicController(QObject *parent = nullptr);
    Q_PROPERTY (double songPosition READ getPositionSeconds WRITE setPositionSeconds NOTIFY positionChanged)
    Q_PROPERTY (double songDuration READ getDurationSeconds NOTIFY durationChanged)
    Q_PROPERTY (bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY (QVariant currentPatternData READ currentPatternData NOTIFY currentPatternChanged)
    Q_PROPERTY (int currentRow READ currentRow NOTIFY currentRowChanged)
    Q_INVOKABLE bool openFile(QUrl fileName);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE QString songName();
    Q_INVOKABLE int getNumSubsongs();
    Q_INVOKABLE void selectSubsong(int num);
    Q_INVOKABLE int getNumChannels();
    Q_INVOKABLE void toggleChannel(int index, bool active);
    Q_INVOKABLE void setTempoFactor(int value);
    Q_INVOKABLE QVariantList testModel();
    double getPositionSeconds();
    void setPositionSeconds(double pos);
    double getDurationSeconds();
    bool isPlaying();
    QVariantList currentPatternData();
    int currentRow();
signals:
    void positionChanged();
    void durationChanged();
    void isPlayingChanged();
    void currentPatternChanged();
    void currentRowChanged();
private:
    openmpt::module_ext *mod = nullptr;
    openmpt::ext::interactive *interactive;
    // the following two variables are used to avoid unnecessary order/pattern changed signals
    std::int32_t rowNum = 0;
    std::int32_t patternNum = 0;
    QVector<QVector<QStringList>> *rawPatternContent;
    portaudio::MemFunCallbackStream<MusicController> *stream;
    const std::int32_t samplerate = 48000;
    int streamCallback(const void *inputBuffer, void *outputBuffer, unsigned long numFrames,
                       const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
};

#endif // MUSICCONTROLLER_H
