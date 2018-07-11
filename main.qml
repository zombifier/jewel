import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.2
import Jewel.MusicController 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Jewel Tracker Player")

    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.primary: Material.Purple
    header: ToolBar {
        RowLayout {
            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: false
            ToolButton {
                anchors.leftMargin: 50
                id: openButton
                text: "Open"
                onClicked: fileDialog.open()
            }
            Label {
                text: "Currently playing:"
            }
            Label {
                id: songName
                color: Material.accent
            }
        }
    }
    footer: ToolBar {
        background: Rectangle {
            implicitHeight: 40
            color: Material.background
        }
        RowLayout {
            Layout.alignment: Qt.AlignBottom
            Layout.fillHeight: false
            Label {
                text: "Select subsong: "
            }
            SpinBox {
                id: subsongSpinBox
                editable: true
            }
            Label {
                text: "Tempo factor: "
            }
            SpinBox {
                id: tempoSpinbox
                editable: true
                from: 1
                value: 100
                to: 400
                stepSize: 10

                property int decimals: 2
                property real realValue: value / 100

                validator: DoubleValidator {
                    bottom: Math.min(tempoSpinbox.from, tempoSpinbox.to)
                    top:  Math.max(tempoSpinbox.from, tempoSpinbox.to)
                }

                textFromValue: function(value, locale) {
                    return Number(value / 100).toLocaleString(locale, 'f', tempoSpinbox.decimals)
                }

                valueFromText: function(text, locale) {
                    return Number.fromLocaleString(locale, text) * 100
                }
            }
        }

    }


    MusicController {
        id: musicController
    }

    FileDialog {
        id: fileDialog
        title: "Choose tracker to play"
        onAccepted: {
            console.log(fileDialog.fileUrl);
            musicController.openFile(fileDialog.fileUrl);
            songName.text = fileDialog.fileUrl;
            subsongSpinBox.value = 0;
            subsongSpinBox.to = musicController.getNumSubsongs() - 1;
            subsongSpinBox.valueModified.connect(function() {
                musicController.selectSubsong(subsongSpinBox.value);
            });
            tempoSpinbox.value = 100;
            tempoSpinbox.valueModified.connect(function () {
                musicController.setTempoFactor(tempoSpinbox.value);
            });
            trackerVisualizer.channelCount = musicController.getNumChannels();
            close();
        }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent

        RowLayout {
            RoundButton {
                id: playButton
                anchors.left: parent.left
                text: musicController.isPlaying ? "Pause" : "Play"
                onClicked: musicController.isPlaying ? musicController.pause() : musicController.play()
            }
            Slider {
                id: progressSlider
                Layout.fillWidth: true
                value: musicController.songPosition
                to: musicController.songDuration
            }
        }
        RowLayout {
            TrackerVisualizer {
                id: trackerVisualizer
                Layout.alignment: Layout.Center
                model: musicController.currentPatternData
                index: musicController.currentRow
                onToggleChannel: musicController.toggleChannel(index, isActive);
            }
        }

    }
}
