#include "musiccontroller.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <QDebug>
#include <portaudio.h>

MusicController* MusicController::instance = 0;

MusicController::MusicController(QObject *parent) : QObject(parent)
{
    instance = this;
    portaudio::AutoSystem portaudio_initializer;
    portaudio_initializer.initialize();
    portaudio::System & portaudio = portaudio::System::instance();
    portaudio::DirectionSpecificStreamParameters outputstream_parameters(portaudio.defaultOutputDevice(), 2, portaudio::FLOAT32, false, portaudio.defaultOutputDevice().defaultHighOutputLatency(), 0);
    portaudio::StreamParameters stream_parameters(portaudio::DirectionSpecificStreamParameters::null(), outputstream_parameters, samplerate, paFramesPerBufferUnspecified, paNoFlag);
    stream = new portaudio::MemFunCallbackStream<MusicController>(stream_parameters, *this, &MusicController::streamCallback);
    stream->start();
    qDebug() << "PortAudio initialized.";
}

MusicController* MusicController::getInstance() {
    return instance;
}

/** Opens a module for playback.
 * Returns true if successful, false if not
 */
bool MusicController::openFile(QUrl fileName) {
    try {
        std::ifstream file(fileName.toLocalFile().toStdString(), std::ios::binary);
        mod = new openmpt::module_ext(file);
        interactive = static_cast<openmpt::ext::interactive*>(mod->get_interface(openmpt::ext::interactive_id));
        // we fill the pattern content 3d array
        rawPatternContent = new QVector<QVector<QStringList>>();
        for (std::int32_t patternCount = 0; patternCount < mod->get_num_patterns(); patternCount++) {
            QVector<QStringList> pattern;
            for (std::int32_t rowCount = 0; rowCount < mod->get_pattern_num_rows(patternCount); rowCount++) {
                QStringList row;
                for (std::int32_t channelCount = 0; channelCount < mod->get_num_channels(); channelCount++) {
                    row.append(QString::fromStdString(mod->format_pattern_row_channel_command(patternCount, rowCount, channelCount, openmpt::module::command_index::command_note)));
                }
                pattern.append(row);
            }
            rawPatternContent->append(pattern);
        }
        patternNum = mod->get_current_pattern();
        rowNum = mod->get_current_row();
        emit durationChanged();
        emit currentPatternChanged();
        emit currentRowChanged();
        mod->set_repeat_count(-1);
        return true;
    } catch (openmpt::exception &e) {
        qDebug() << "Error opening file:" << e.what();
        return false;
    }
}

void MusicController::play() {
    if (stream->isStopped() && mod != nullptr) {
        stream->start();
    }
    if (!playing) {
        playing = true;
        emit isPlayingChanged();
    }
}

void MusicController::pause() {
    if (!stream->isStopped()) {
        playing = false;
        emit isPlayingChanged();
    }
}

int MusicController::streamCallback(const void *inputBuffer, void *outputBuffer, unsigned long numFrames,
                       const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags) {

    float* leftBuffer = ((float**)outputBuffer)[0];
    float* rightBuffer = ((float**)outputBuffer)[1];
    if (playing) {
        std::size_t count = mod->read(samplerate, numFrames, leftBuffer, rightBuffer);
        emit positionChanged();
        int newRow = mod->get_current_row();
        int newPattern = mod->get_current_pattern();
        if (patternNum != newPattern) {
            patternNum = newPattern;
            emit currentPatternChanged();
        }
        if (rowNum != newRow) {
            rowNum = newRow;
            emit currentRowChanged();
        }
        if (count == 0) {
            mod->set_position_seconds(0);
            emit positionChanged();
            emit isPlayingChanged();
            playing = false;
            return 1; // playback finished
        }
    } else {
        // WRITE ZEROS INTO BUFFERS
        std::fill(leftBuffer, leftBuffer + numFrames, 0);
        std::fill(rightBuffer, rightBuffer + numFrames, 0);
    }
    return 0;
}

double MusicController::getDurationSeconds() {
    if (mod == nullptr) return 0;
    else return mod->get_duration_seconds();
}

void MusicController::setPositionSeconds(double pos) {
    if (mod != nullptr && mod->get_position_seconds() != pos) {
        mod->set_position_seconds(pos);
        emit positionChanged();
    }
}

double MusicController::getPositionSeconds() {
    if (mod == nullptr) return 0;
    else return mod->get_position_seconds();
}

int MusicController::getNumSubsongs() { return mod->get_num_subsongs(); }

int MusicController::getNumChannels() { return mod->get_num_channels(); }

bool MusicController::isPlaying() {
    return playing;
}

int MusicController::currentRow() {
    return rowNum;
}

void MusicController::toggleChannel(int index, bool active) {
    interactive->set_channel_mute_status(index, active);
}

void MusicController::setTempoFactor(int value) {
    double factor = (double) value / (double) 100;
    interactive->set_tempo_factor(factor);
}

QVariantList MusicController::currentPatternData() {
    QVariantList list;
    if (mod != nullptr) {
        foreach(QStringList q, rawPatternContent->at(patternNum)) {
            foreach(QString s, q) {
            }
            list.append(QVariant::fromValue(q));
        }
    }
    return list;
}

void MusicController::selectSubsong(int num) {
    if (num < mod->get_num_subsongs()) mod->select_subsong(num);
    // also notify the slider that the values changed
    emit positionChanged();
    emit durationChanged();
    rowNum = mod->get_current_row();
    patternNum = mod->get_current_pattern();
    emit currentRowChanged();
    emit currentPatternChanged();
}
QString MusicController::songName() {
    return QString::fromStdString(mod->get_subsong_names().at(mod->get_selected_subsong()));
}

QVariantList MusicController::testModel() {
    QVariantList list;
    foreach(QStringList q, rawPatternContent->at(0)) {
        foreach(QString s, q) {
        }
        list.append(QVariant::fromValue(q));
    }
    return list;
}

MusicController::~MusicController() {
    this->pause();
}
