import QtQuick 1.1
import com.meego 1.0

Rectangle {
    width: parent.width
    height: 40
    color: "#D0D0D0"

    property bool addPrefix: false

    Label {
        text: (addPrefix ? qsTr("Platform") + " " : "") + section
        anchors.fill: parent
        anchors.rightMargin: 16
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 18
        font.bold: true
    }
}
