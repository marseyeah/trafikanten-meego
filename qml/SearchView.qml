import QtQuick 1.1
import com.meego 1.0

Page {
    id: searchItem
    orientationLock: PageOrientation.LockPortrait
    pageStack: parent

    ViewHeader {
        id: header
        text: qsTr("Search")
    }

    StopSearchBox {
        id: searchBox
        width: parent.width
        anchors.top: header.bottom;
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        placeholderText: qsTr("Stop")

        onStopSelected: pageStack.push(Qt.resolvedUrl("TimetableView.qml"), { stopID: searchBox.selectedStopID, stopName: searchBox.selectedStopName })
    }
}
