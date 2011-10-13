import QtQuick 1.1
import com.meego 1.0
import "utilities.js" as Utilities

Page {
    id: root
    orientationLock: PageOrientation.LockPortrait
    property int stopID: 0
    property string stopName: ""

    onStopIDChanged: {
        if (status == PageStatus.Active)
            trafikanten.realTimeDataQuery = stopID
    }
    onVisibleChanged: { if (!visible) trafikanten.realTimeDataQuery = 0 }
    onStatusChanged: {
        if (status == PageStatus.Active)
            trafikanten.realTimeDataQuery = stopID
    }

    DeviModel {
        id: deviationModel
        mStop: root.stopID

        onCountChanged: {
            if  (count > 0) {
                deviDialog.titleText = deviationModel.get(0).title
                deviDialog.contentText = deviationModel.get(0).description
                deviDialog.bodyText = deviationModel.get(0).body
            }
        }
    }

    StopDeviationDialog {
        id: deviDialog
    }

    ViewHeader {
        id: header
        text: Utilities.stopNameSubPart(stopName, 0)
        secondaryText: Utilities.stopNameSubPart(stopName, 1)

        Button {
            id: refreshButton
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.verticalCenter: parent.verticalCenter
            height: 42; width: height
            iconSource: "image://theme/icon-s-refresh"
            onClicked: {
                var id = stopID;
                stopID = 0;
                stopID = id;
            }
            opacity: enabled ? 1.0 : 0.5
            enabled: !trafikanten.waitingForData
        }
    }

    BusyIndicator {
        id: spinner
        y: 0.2 * parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        platformStyle: BusyIndicatorStyle { size: "medium" }
        running: trafikanten.waitingForData
        visible: running
    }

    Label {
        visible: lineList.count === 0 && !trafikanten.waitingForData && root.status == PageStatus.Active && deviationModel.count == 0
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -210
        color: "#8c8c8c"
        font.pixelSize: 32
        text: qsTr("No departures found")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    ListView {
        id: lineList
        clip: true
        cacheBuffer: height * 5
        width: parent.width
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        section.property: "platformName"
        section.criteria: ViewSection.FullString
        section.delegate: StopSectionDelegate { addPrefix: true }

        model: trafikanten.realTimeDataResult
        delegate: StopDataDelegate { }

        header: Rectangle {
            width: lineList.width
            height: (root.status == PageStatus.Active || root.status == PageStatus.Deactivating) && deviationModel.count > 0  && !trafikanten.waitingForData ? (deviTitle.height + deviDesc.height + 16) : 0
            color: "#E0E1E2"
            clip: true

            Rectangle {
                id: overlay
                anchors.fill: parent
                color: "#22000000"
                opacity: deviMouseArea.pressed ? 1.0 : 0.0
                Behavior on opacity { NumberAnimation { duration: 50 } }
            }

            Label {
                id: deviTitle
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.top: parent.top
                anchors.topMargin: 8
                wrapMode: Text.WordWrap
                font.bold: true
                font.pixelSize: 22
                text: deviationModel.count > 0 ? deviationModel.get(0).title : ""
            }
            Label {
                id: deviDesc
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.top: deviTitle.bottom
                wrapMode: Text.WordWrap
                font.pixelSize: 22
                textFormat: Text.RichText
                text: deviationModel.count > 0 ? (deviationModel.get(0).description) : ""
            }

            MouseArea {
                id: deviMouseArea
                anchors.fill: parent
                onClicked: deviDialog.open()
            }
        }
    }

    ScrollDecorator { flickableItem: lineList }
}
