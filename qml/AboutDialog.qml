import QtQuick 1.1
import com.meego 1.0

Dialog {
    id: genericDialog

    property string titleText: qsTr("About") + " Trafikanten"

    property Style platformStyle: SelectionDialogStyle {}

    //private
    property bool __drawFooterLine: false

    title: Item {
        id: header
        height: genericDialog.platformStyle.titleBarHeight

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Item {
            id: labelField

            anchors.fill:  parent

            Item {
                id: labelWrapper
                anchors.left: labelField.left
                anchors.right: labelField.left

                anchors.bottom:  parent.bottom
                anchors.bottomMargin: genericDialog.platformStyle.titleBarLineMargin

                //anchors.verticalCenter: labelField.verticalCenter

                height: titleLabel.height

                Label {
                    id: titleLabel
                    x: genericDialog.platformStyle.titleBarIndent
                    font.pixelSize: 32
                    text: genericDialog.titleText
                    color: genericDialog.platformStyle.commonLabelColor
                }

            }
        }

        Rectangle {
            id: headerLine

            anchors.left: parent.left
            anchors.right: parent.right

            anchors.bottom:  header.bottom

            height: 1

            color: "#4D4D4D"
        }

    }

    content: Column {
        width: parent.width

        Item {
            width: parent.width
            height: 38
        }

        Row {
            spacing: 16

            Image {
                source: "qrc:/Trafikanten.png"
                anchors.verticalCenter: parent.verticalCenter
            }

            Column {
                Label {
                    text: qsTr("Version") + " 1.2.0"
                    font.pixelSize: 22
                    color: genericDialog.platformStyle.commonLabelColor
                }

                Label {
                    text: "Copyright \u00a9 Trafikanten AS"
                    font.pixelSize: 22
                    color: genericDialog.platformStyle.commonLabelColor
                }

                Item { width: parent.width; height: 16 }

                Label {
                    text: qsTr("Developed by") + " Yoann Lopes"
                    font.pixelSize: 22
                    color: genericDialog.platformStyle.commonLabelColor
                }

                Label {
                    text: "<yoann.lopes@gmail.com>"
                    font.pixelSize: 22
                    color: genericDialog.platformStyle.commonLabelColor
                }
            }
        }

        Item {
            width: parent.width
            height: 38
        }

        Label {
            text: qsTr("For more information, please visit")
            font.pixelSize: 22
            color: genericDialog.platformStyle.commonLabelColor
        }
        Label {
            text: "<a href='http://www.trafikanten.no'>http://www.trafikanten.no</a>"
            font.pixelSize: 22
            color: genericDialog.platformStyle.commonLabelColor
            onLinkActivated: Qt.openUrlExternally("http://www.trafikanten.no")
        }
    }

    buttons: Item {
        id: buttonColFiller
        width: parent.width
        height: childrenRect.height

        anchors.top: parent.top

        Button {
            id: acceptButton
            text: qsTr("Close")
            onClicked: accept()
            __dialogButton: true
            platformStyle: ButtonStyle {inverted: true}
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 38
        }
    }

}
