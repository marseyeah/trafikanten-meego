import QtQuick 1.0

XmlListModel {
    property string mLine: ""
    property string mStop: ""

    function getCurrentTime()
    {
        var currdate = new Date();
        var month = parseInt(currdate.getMonth());
        ++month;
        var day = parseInt(currdate.getDate());
        var hour = parseInt(currdate.getHours());
        var minutes = parseInt(currdate.getMinutes());
        return currdate.getFullYear() + "-"
                + (month < 10 ? "0" + month : month) + "-"
                + (day < 10 ? "0" + day : day) + "T"
                + (hour < 10 ? "0" + hour : hour) + ":"
                + (minutes < 10 ? "0" + minutes : minutes);
    }

    id: feedModel
    source: "http://devi.trafikanten.no/rss.aspx?v=1.1&show=filter"
            + (mLine.length > 0 ? "&linename=" + mLine : "")
            + (mStop.length > 0 ? "&stop=" + mStop : "")
            + "&datetime=" + getCurrentTime()
    query: "/rss/channel/item"
    XmlRole { name: "title"; query: "title/string()" }
    XmlRole { name: "description"; query: "description/string()" }
    XmlRole { name: "body"; query: "body/string()" }
}
