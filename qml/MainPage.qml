import QtQuick 1.1
import com.meego 1.0

Page {
    id: mainPage

    property alias deviationDialog: deviDialog

    tools: commonTools
    orientationLock: PageOrientation.LockPortrait

    Background {
        anchors.fill: parent
    }

    StopDeviationDialog {
        id: deviDialog
    }

    TabGroup {
        id: tabGroup

        Component.onCompleted: {
            var tab = trafikanten.lastTab;
            if (tab == "nearby")
                currentTab = nearbyTab;
            else if (tab == "favorites")
                currentTab = favoritesTab;
            else if (tab == "recents")
                currentTab = recentsTab;
            else
                currentTab = searchTab;
        }
        onCurrentTabChanged: trafikanten.lastTab = currentTab.name;

        PageStack {
            id: searchTab
            property string name: "search"
            Component.onCompleted: push(Qt.resolvedUrl("SearchView.qml"))
            onVisibleChanged: { if (!visible) pop() }
        }
        PageStack {
            id: nearbyTab
            property string name: "nearby"
            Component.onCompleted: push(Qt.resolvedUrl("NearbyView.qml"))
            onVisibleChanged: { if (!visible) pop() }
        }
        FavoritesView {
            id: favoritesTab
            property string name: "favorites"
        }
        PageStack {
            id: recentsTab
            property string name: "recents"
            Component.onCompleted: push(Qt.resolvedUrl("RecentsView.qml"))
            onVisibleChanged: { if (!visible) pop() }
        }
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            iconId: enabled ? "toolbar-back" : "toolbar-back-dimmed";
            onClicked: { tabGroup.currentTab.pop(); }
            enabled: tabGroup.currentTab.depth > 1
        }

        ButtonRow {
            TabButton {
                tab: searchTab
                iconSource: "image://theme/icon-m-toolbar-search"
            }
            TabButton {
                tab: nearbyTab
                iconSource: "images/toolbar-nearby.png"
            }
            TabButton {
                tab: favoritesTab
                iconSource: "image://theme/icon-m-toolbar-favorite-mark"
            }
            TabButton {
                tab: recentsTab
                iconSource: "image://theme/icon-m-toolbar-history"
            }
        }

        Item {
            width: 80; height: 64

            Image {
                anchors.centerIn: parent
                source: menuMouseArea.pressed ? "image://theme/icon-m-toolbar-view-menu-dimmed" : "image://theme/icon-m-toolbar-view-menu";
            }

            MouseArea {
                id: menuMouseArea
                anchors.fill: parent
                onClicked: { myMenu.open() }
            }
        }
    }

    Menu {
        id: myMenu
        MenuLayout {
            MenuItem {text: qsTr("About") + " Trafikanten"; onClicked: { aboutDialog.open() } }
        }
    }

    AboutDialog {
        id: aboutDialog
    }
}
