import QtQuick 1.1
import com.meego 1.0
import "utilities.js" as Utilities

Item {
    id: root
    property string selectedStopName: ""
    property alias placeholderText: input.placeholderText
    property int selectedStopID: 0

    signal stopSelected

    ListView {
        id: listview
        anchors.top: separator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        anchors.bottom: parent.bottom
        clip: true

        model: trafikanten.realTimeSearchResult
        delegate: StopListDelegate { }
        onMovementStarted: listview.focus = true
    }

    ScrollDecorator { flickableItem: listview }

    Label {
        visible: listview.count === 0 && input.text.length > 0 && !trafikanten.waitingForData
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: separator.bottom
        anchors.topMargin: 32
        color: "#8c8c8c"
        font.pixelSize: 32
        text: qsTr("No results")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    TextField {
        id: input
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 16
        anchors.top: parent.top
        inputMethodHints: Qt.ImhNoPredictiveText
        platformStyle: TextFieldStyle {
            backgroundSelected: "image://theme/color9-meegotouch-textedit-background-selected"
        }
        Keys.onReturnPressed: listview.focus = true

        Image {
            id: editIcon
            source: input.text.length > 0 ? "image://theme/icon-m-input-clear" : "image://theme/icon-m-common-search"
            anchors.right: parent.right
            anchors.rightMargin: (parent.height - height) / 2
            anchors.verticalCenter: parent.verticalCenter
            opacity: iconMouseArea.pressed && input.text.length > 0 ? 0.5 : 1.0
            visible: !spinner.visible
        }

        BusyIndicator {
            id: spinner
            anchors.right: parent.right
            anchors.rightMargin: (parent.height - height) / 2
            anchors.verticalCenter: parent.verticalCenter
            platformStyle: BusyIndicatorStyle { size: "small" }
            running: trafikanten.waitingForData && !root.parent.pageStack.busy
            visible: running
        }

        MouseArea {
            id: iconMouseArea
            anchors.fill: editIcon
            onClicked: { input.text = ""; input.forceActiveFocus() }
            z: 2
            enabled: !spinner.visible
        }

        onTextChanged: {
            selectedStopID = 0
            trafikanten.realTimeSearch = Utilities.trim(text);
        }

        onFocusChanged: {
            if (focus) {
                if (text.length > 0) {
                    trafikanten.realTimeSearch = Utilities.trim(text);
                }
            }
        }
    }

    Separator {
        id: separator
        anchors.top: input.bottom
        anchors.topMargin: 16
        width:  parent.width
    }
}
