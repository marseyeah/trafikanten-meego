import QtQuick 1.1
import com.meego 1.0

Page {
    id: view
    orientationLock: PageOrientation.LockPortrait
    pageStack: parent

    ViewHeader {
        id: header
        text: qsTr("Recents")
    }

    RecentsListView {
        id: recents
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        onStopSelected: pageStack.push(Qt.resolvedUrl("TimetableView.qml"), { stopID: recents.selectedStopID, stopName: recents.selectedStopName })
    }

    Label {
        visible: recents.count === 0
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -210
        color: "#8c8c8c"
        font.pixelSize: 32
        text: qsTr("No recents")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

}
