import QtQuick 1.1
import QtMobility.location 1.2
import com.meego 1.0
import "utilities.js" as Utilities

Item {
    id: root
    property int selectedStopID
    property string selectedStopName
    property alias searching: spinner.running
    property alias count: listview.count
    signal stopSelected

    function updatePosition()
    {
        trafikanten.nearbySearch = Qt.point(0, 0);
        positionSource.active = true;
    }

    BusyIndicator {
        id: spinner
        y: 0.2 * parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        platformStyle: BusyIndicatorStyle { size: "medium" }
        running: trafikanten.waitingForData || positionSource.active
        visible: running
    }

    PositionSource {
        id: positionSource
        updateInterval: 0
        active: false
        onPositionChanged: { trafikanten.nearbySearch = Qt.point(position.coordinate.latitude, position.coordinate.longitude); active = false; }
     }

    ListView {
        id: listview
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: parent.width
        clip: true

        model: trafikanten.nearbySearchResult
        delegate: StopListDelegate { }
    }

    ScrollDecorator { flickableItem: listview }
}
