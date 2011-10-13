import QtQuick 1.1
import com.meego 1.0

Sheet {
    id: myDialog
    property string titleText
    property string contentText
    property string bodyText

    buttons: SheetButton {
        id: acceptButton
        objectName: "acceptButton"
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter
        platformStyle: SheetButtonAccentStyle {
            background: "image://theme/color9-meegotouch-sheet-button-accent-inverted-background"
            pressedBackground: "image://theme/color9-meegotouch-sheet-button-accent-inverted-background-pressed"
            disabledBackground: "image://theme/color9-meegotouch-sheet-button-accent-inverted-background-disabled"
        }
        text: qsTr("Close")
        onClicked: myDialog.close()
    }

    content: ListView {
        id: contentView
        anchors.fill: parent
        anchors.margins: 16

        model: 1
        delegate: Column {
            id: contentColumn
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                id: titleField
                font.bold: true
                anchors.left: parent.left
                anchors.right: parent.right
                wrapMode: Text.Wrap
                text: myDialog.titleText
            }
            Item {
                height: 16
                width: parent.width
            }
            Label {
                id: contentField
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
                anchors.left: text.length > 0 ? parent.left : undefined
                anchors.right: text.length > 0 ? parent.right : undefined
                wrapMode: Text.Wrap
                textFormat: Text.RichText
                font.pixelSize: 20
                text: myDialog.bodyText
            }
        }

        ScrollDecorator {
            anchors.rightMargin: -16
            anchors.topMargin: -16
            flickableItem: contentView
        }
    }
}
