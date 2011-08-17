import QtQuick 1.1
import com.meego 1.0
import "utilities.js" as Utilities

Rectangle {
    id: delegateRoot
    height: 72;
    width: parent.width
    color: "#E0E1E2"
    visible: modelData.name.length > 0

    Rectangle {
        id: separator
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 16
        height: 2
        visible: index !== 0
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#BABABA" }
            GradientStop { position: 0.49; color: "#BABABA" }
            GradientStop { position: 0.5; color: "#FFFFFF" }
            GradientStop { position: 1.0; color: "#FFFFFF" }
        }
    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: separator.bottom
        height: 70

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left; anchors.leftMargin: 16
            anchors.right: iconRow.left; anchors.rightMargin: 16

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                text: Utilities.stopNameSubPart(modelData.name, 0)
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            Label {
                anchors.left: text.length > 0 ? parent.left : undefined
                anchors.right: text.length > 0 ? parent.right : undefined
                text: Utilities.stopNameSubPart(modelData.name, 1)
                color: "#8c8c8c"
                font.pixelSize: 20
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        Rectangle {
            id: overlay
            anchors.fill: parent
            color: "#22000000"
            opacity: 0
            Behavior on opacity { NumberAnimation { duration: 50 } }
        }

        Row {
            id: iconRow
            anchors.right: arrow.left;
            anchors.rightMargin: 3
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            Image {
                source: "images/tog_gray.png"
                smooth: true
                visible: modelData.hasTrain
            }
            Image {
                source: "images/tbane_gray.png"
                smooth: true
                visible: modelData.hasMetro
            }
            Image {
                source: "images/trykk_gray.png"
                smooth: true
                visible: modelData.hasTram
            }
            Image {
                source: "images/buss_gray.png"
                smooth: true
                visible: modelData.hasBus
            }
            Image {
                source: "images/boat_gray.png"
                smooth: true
                visible: modelData.hasBoat
            }
        }

        Image {
            id: arrow
            source: "image://theme/icon-s-common-next"
            anchors.right: parent.right; anchors.rightMargin: 16
            anchors.verticalCenter: parent.verticalCenter
        }

        MouseArea {
            anchors.fill: parent
            onPressed: overlay.opacity = 1
            onReleased: overlay.opacity = 0
            onPositionChanged: overlay.opacity = 0
            onClicked: {
                root.selectedStopID = modelData.stopID;
                root.selectedStopName = modelData.name;
                root.stopSelected();
            }
        }
    }
}
