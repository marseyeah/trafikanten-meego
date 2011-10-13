import QtQuick 1.1
import com.meego 1.0

Dialog {
    id: myDialog
    property string titleText
    property string contentText
    property string bodyText

    platformStyle: DialogStyle {
        leftMargin: 44
        rightMargin: 44
    }

    title: Column {
        spacing: 16
        anchors.left: parent.left
        anchors.right: parent.right

        Label {
            id: titleField
            color: "white"
            font.bold: true
            anchors.left: parent.left
            anchors.right: parent.right
            wrapMode: Text.Wrap
            text: myDialog.titleText
        }

        Rectangle {
            height: 2
            anchors.left: parent.left
            anchors.right: parent.right
            color: "white"
        }
    }

    content: ListView {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 600
        clip: true

        model: 1
        delegate: Column {
            id: contentColumn
            anchors.left: parent.left
            anchors.right: parent.right

            Item {
                height: 16
                width: parent.width
            }
            Label {
                id: contentField
                color: "white"
                anchors.left: parent.left
                anchors.right: parent.right
                wrapMode: Text.Wrap
                textFormat: Text.RichText
                font.pixelSize: 20
                text: myDialog.contentText
            }
            Item {
                height: 16
                width: parent.width
            }
            Label {
                id: bodyField
                color: "white"
                anchors.left: text.length > 0 ? parent.left : undefined
                anchors.right: text.length > 0 ? parent.right : undefined
                wrapMode: Text.Wrap
                textFormat: Text.RichText
                font.pixelSize: 20
                text: myDialog.bodyText
            }
            Item {
                height: bodyField.text.length > 0 ? 16 : 0
                width: parent.width
            }
        }
    }

    buttons: ButtonRow {
        style: ButtonStyle {
            pressedBackground: "image://theme/color9-meegotouch-button-inverted-background-pressed" + (position ? "-" + position : "")
            disabledBackground: "image://theme/color9-meegotouch-button-inverted-background-disabled" + (position ? "-" + position : "")
            checkedBackground: "image://theme/color9-meegotouch-button-inverted-background-selected" + (position ? "-" + position : "")
            checkedDisabledBackground: "image://theme/color9-meegotouch-button-inverted-background-disabled-selected" + (position ? "-" + position : "")
        }
        anchors.horizontalCenter: parent.horizontalCenter
        Button {
            text: qsTr("Close"); onClicked: myDialog.accept()
        }
    }
}
