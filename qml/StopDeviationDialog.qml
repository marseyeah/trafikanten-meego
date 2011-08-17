import QtQuick 1.1
import com.meego 1.0

Dialog {
    id: myDialog
    property alias titleText: titleTrans.query
    property alias contentText: contentTrans.query
    property alias bodyText: bodyTrans.query

    Translator {
        id: titleTrans
    }
    Translator {
        id: contentTrans
    }
    Translator {
        id: bodyTrans
    }

    title: Column {
        spacing: 16
        anchors.left: parent.left
        anchors.right: parent.right

        Label {
            id: titleField
            text: titleTrans.result
            color: "white"
            font.bold: true
            anchors.left: parent.left
            anchors.right: parent.right
            wrapMode: Text.Wrap
        }

        Rectangle {
            height: 2
            anchors.left: parent.left
            anchors.right: parent.right
            color: "white"
        }
    }

    content: Column {
        anchors.left: parent.left
        anchors.right: parent.right

        Item {
            height: 16
            width: parent.width
        }
        Label {
            id: contentField
            text: contentTrans.result
            color: "white"
            anchors.left: parent.left
            anchors.right: parent.right
            wrapMode: Text.Wrap
            textFormat: Text.RichText
            font.pixelSize: 20
        }
        Item {
            height: 16
            width: parent.width
        }
        Label {
            id: bodyField
            text: bodyTrans.result
            color: "white"
            anchors.left: text.length > 0 ? parent.left : undefined
            anchors.right: text.length > 0 ? parent.right : undefined
            wrapMode: Text.Wrap
            textFormat: Text.RichText
            font.pixelSize: 20
        }
        Item {
            height: bodyField.text.length > 0 ? 16 : 0
            width: parent.width
        }
    }

    buttons: ButtonRow {
        style: ButtonStyle { }
        anchors.horizontalCenter: parent.horizontalCenter
        Button {text: qsTr("Close"); onClicked: myDialog.accept()}
    }
}
