import QtQuick 2.9
import QtQuick.Controls.Material 2.2
import QtQuick.Controls 2.2

Item {
    id: trackerVisualizer
    property var model
    property var index
    property var channelCount
    anchors.fill: parent
    signal toggleChannel(int index, bool isActive)

    ListView {
        id: trackerListView
        interactive: false
        clip: true
        anchors.fill: parent
        model: parent.model
        currentIndex: parent.index
        orientation: ListView.Vertical
        preferredHighlightBegin: 50
        preferredHighlightEnd: 50
        highlightRangeMode: ListView.ApplyRange
        highlightMoveDuration: 0

        headerPositioning: ListView.OverlayHeader
        header: Rectangle {
            height: 50
            z: 2
            color: Material.background
            width: parent.width
            ListView {
                interactive: false
                anchors.fill: parent
                model: channelCount
                orientation: ListView.Horizontal
                delegate: Button {
                    flat: true
                    width: 40
                    height: 50
                    checkable: true
                    checked: true
                    text: index
                    onPressed: trackerVisualizer.toggleChannel(index, checked)
                }
            }
        }

        // The rows
        delegate: Rectangle {
            id: delegateRect
            height: 40
            color: Material.background
            width: parent.width
            ListView {
                interactive: false
                anchors.fill: parent
                model: modelData;
                orientation: ListView.Horizontal
                delegate: Rectangle {
                    color: Material.background
                    width: 40
                    height: 40
                    Text {
                        text: modelData;
                        anchors.fill: parent
                        color: delegateRect.ListView.isCurrentItem ? Material.foreground : Material.accent
                    }
                }
            }
        }

    }
}
