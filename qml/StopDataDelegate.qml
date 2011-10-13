import QtQuick 1.1
import com.meego 1.0
import "utilities.js" as Utilities

Column {
    id: delegateRoot
    width: parent.width

    DeviModel {
        id: deviationModel
        mLine: modelData.lineName
    }

    Separator {
        id: separator
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 16
        visible: delegateRoot.ListView.previousSection == delegateRoot.ListView.section
    }

    Rectangle {
        id: rect
        width: parent.width;
        height: 80
        color: "#E0E1E2"

        Image {
            id: lineNum
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            source: "images/icon-common.png"

            Label {
                text: modelData.lineName.length > 0 ? modelData.lineName : "?"
                anchors.fill: parent
                color: "white"
                font.pixelSize: text.length < 3 ? 34 : 26
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        property bool isFavorite: trafikanten.favorites.isFavorite(modelData.stopId, modelData.lineName, modelData.destination, modelData.platformName)

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: lineNum.right
            anchors.leftMargin: 16
            anchors.right: parent.right
            spacing: 3

            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                height: Math.max(favoriteIcon.height, destinationLabel.height)

                Image {
                    id: favoriteIcon
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 16
                    source: rect.isFavorite ? "image://theme/icon-s-common-favorite-mark" : "image://theme/icon-s-common-favorite-unmark"

                    MouseArea {
                        anchors.centerIn: parent
                        width: parent.width * 1.7
                        height: parent.height * 1.7
                        onClicked: {
                            if (rect.isFavorite)
                                trafikanten.favorites.removeFavorite(modelData.stopId, modelData.lineName, modelData.destination, modelData.platformName);
                            else
                                trafikanten.favorites.addFavorite(modelData.stopId, root.stopName, modelData.lineName, modelData.destination, modelData.platformName, modelData.transportation);
                            rect.isFavorite = !rect.isFavorite
                        }
                    }
                }

                Image {
                    id: transportType
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: destinationLabel.left
                    anchors.leftMargin: destinationLabel.paintedWidth + 16
                    source: Utilities.getTransportIcon(modelData.transportation)
                }

                Label {
                    id: destinationLabel
                    text: modelData.destination
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: favoriteIcon.left
                    anchors.rightMargin: 16 * 2 + transportType.width
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }

            ListView {
                id: timeview
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 16
                height: 30
                orientation: ListView.Horizontal
                spacing: 14
                clip: true

                model: modelData.departureTimes
                delegate: Component {
                    Label {
                        height: timeview.height
                        font.bold: true
                        color: "#285ab3"
                        verticalAlignment: Text.AlignTop
                        font.pixelSize: 22
                        text: (modelData.monitored ? "" : "ca ") + (rawText == "0" ? qsTr("now") : rawText)
                        property string rawText: Utilities.formatTrafDate(modelData.referenceTime, modelData.departureTime)
                    }
                }
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        height: deviationModel.count > 0 ? 27 : 0
        color: "#E0E1E2"
        clip: true
        Behavior on height { NumberAnimation { } }

        MouseArea {
            id: deviButton
            anchors.fill: parent
            onClicked: {
                mainPage.deviationDialog.titleText = deviationModel.get(0).title
                mainPage.deviationDialog.contentText = deviationModel.get(0).description
                mainPage.deviationDialog.bodyText = deviationModel.get(0).body
                mainPage.deviationDialog.open()
            }
        }

        Label {
            anchors.left: parent.left
            anchors.leftMargin: lineNum.width + 32
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.top: parent.top
            anchors.topMargin: -7
            font.pixelSize: 22
            font.family: "Nokia Pure Text Light"
            color: "#285ab3"
            opacity: deviButton.pressed ? 0.3 : 1.0
            elide: Text.ElideRight
            text: deviationModel.count > 0 ? deviationModel.get(0).title : ""
        }
    }
}
