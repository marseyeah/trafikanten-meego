import QtQuick 1.1
import com.meego 1.0

Image {
    property alias text: title.text
    property alias secondaryText: title2.text
    property alias textAnchors: title.anchors
    property alias secondaryTextAnchors: title2.anchors

    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
    source: "image://theme/color9-meegotouch-view-header-fixed"

    Column {
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 74
        anchors.verticalCenter: parent.verticalCenter
        spacing: -4

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
            color: "#D8D8D8"
            font.pixelSize: 20
            elide: Text.ElideRight
        }
    }
}
