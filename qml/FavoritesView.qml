import QtQuick 1.1
import com.meego 1.0
import "utilities.js" as Utilities

Page {
    id: root
    orientationLock: PageOrientation.LockPortrait

    onVisibleChanged: { if (visible) trafikanten.favorites.refreshData() }

    ViewHeader {
        id: header
        text: qsTr("Favorites")

        BusyIndicator {
            id: spinner
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.verticalCenter: parent.verticalCenter
            platformStyle: BusyIndicatorStyle { size: "medium" }
            running: visible
            visible: !refreshButton.visible && lineList.count > 0
        }

        Button {
            id: refreshButton
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.verticalCenter: parent.verticalCenter
            height: 42; width: height
            iconSource: "image://theme/icon-s-refresh"
            onClicked: trafikanten.favorites.refreshData()
            visible: !trafikanten.favorites.waitingForData && lineList.count > 0
        }
    }

    ListView {
        id: lineList
        clip: true
        cacheBuffer: height * 5
        width: parent.width
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        section.property: "stopName"
        section.criteria: ViewSection.FullString
        section.delegate: StopSectionDelegate { }

        model: trafikanten.favorites.data
        delegate: StopDataDelegate { }
    }

    ScrollDecorator { flickableItem: lineList }

    Label {
        visible: lineList.count === 0
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -210
        color: "#8c8c8c"
        font.pixelSize: 32
        text: qsTr("No favorites")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
