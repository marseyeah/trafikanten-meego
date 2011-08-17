import QtQuick 1.1
import com.meego 1.0

Rectangle {
    property alias text: title.text
    property alias secondaryText: title2.text
    property alias textAnchors: title.anchors
    property alias secondaryTextAnchors: title2.anchors

    width: parent ? parent.width : 0
    height: 64
    color: "#1A85DE"
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter

    Column {
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 74
        anchors.verticalCenter: parent.verticalCenter

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            id: title
            verticalAlignment: Text.AlignVCenter
            color: "white"
            font.pixelSize: 28
            elide: Text.ElideRight
        }
        Label {
            id: title2
            anchors.left: text.length > 0 ? parent.left : undefined
            anchors.right: text.length > 0 ? parent.right : undefined
            verticalAlignment: Text.AlignVCenter
            color: "#383838"
            font.pixelSize: 20
            elide: Text.ElideRight
        }
    }
}
