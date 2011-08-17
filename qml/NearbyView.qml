import QtQuick 1.1
import com.meego 1.0

Page {
    id: view
    orientationLock: PageOrientation.LockPortrait
    pageStack: parent

    onVisibleChanged: {
        if (visible && nearby.count === 0)
            nearby.updatePosition();
    }

    ViewHeader {
        id: header
        text: qsTr("Nearby")

        Button {
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.verticalCenter: parent.verticalCenter
            height: 42; width: height
            iconSource: "image://theme/icon-s-refresh"
            onClicked: nearby.updatePosition()
            opacity: enabled ? 1.0 : 0.5
            enabled: !nearby.searching
        }
    }

    NearbyListView {
        id: nearby
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        onStopSelected: pageStack.push(Qt.resolvedUrl("TimetableView.qml"), { stopID: nearby.selectedStopID, stopName: nearby.selectedStopName })
    }
}
