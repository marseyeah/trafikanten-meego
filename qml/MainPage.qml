import QtQuick 1.1
import com.meego 1.0

Page {
    id: mainPage
    tools: commonTools
    orientationLock: PageOrientation.LockPortrait

    Background {
        anchors.fill: parent
    }

    TabGroup {
        id: tabGroup

        currentTab: searchTab

        PageStack {
            id: searchTab
            Component.onCompleted: push(Qt.resolvedUrl("SearchView.qml"))
            onVisibleChanged: { if (!visible) pop() }
        }
        PageStack {
            id: nearbyTab
            Component.onCompleted: push(Qt.resolvedUrl("NearbyView.qml"))
            onVisibleChanged: { if (!visible) pop() }
        }
        FavoritesView {
            id: favoritesTab
        }
        PageStack {
            id: recentsTab
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
    }
}
