import QtQuick 1.0

Item {
    property real contentOpacity: 0.3

    Rectangle {
        anchors.fill: parent
        color: "#E0E1E2"
    }

    Text {
        id: logo
        text: "Trafikanten"
        y: 350
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#8c8c8c"
        font.family: "Trebuchet MS, Arial"
        font.pixelSize: 35
        font.bold: true
        opacity: contentOpacity
    }

    Image {
        anchors.top: logo.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter:parent.horizontalCenter
        source: "images/logo_banner.png"
        opacity: contentOpacity
    }
}
