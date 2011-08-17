import QtQuick 1.1
import com.meego 1.0
import "utilities.js" as Utilities

Item {
    id: root
    property int selectedStopID
    property string selectedStopName
    property alias count: listview.count
    signal stopSelected

    ListView {
        id: listview
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: parent.width
        clip: true

        model: trafikanten.recentSearches
        delegate: StopListDelegate { }
    }

    ScrollDecorator { flickableItem: listview }
}
