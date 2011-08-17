import QtQuick 1.0

QtObject {
    property string result: ""
    property string query: ""
    property string srcLang: "nb"
    property string targetLang: trafikanten.language
    property string url: "https://www.googleapis.com/language/translate/v2?key=AIzaSyBNTQBGkbM0X-1jkYbL6rx0OEuN0T2SN9c&"

    function translate() {
        if (query.length === 0 || srcLang.length === 0 || targetLang.length === 0) return;

        if (srcLang == targetLang) {
            result = query;
            return;
        }

        var req = url + "q=" + query + "&source=" + srcLang + "&target=" + targetLang;
        var xhr = new XMLHttpRequest;
        xhr.open("GET", req);
        xhr.onreadystatechange = function() {
            if (xhr.readyState == XMLHttpRequest.DONE) {
                var obj = JSON.parse(xhr.responseText);
                result = obj.data.translations[0].translatedText;
            }
        }
        xhr.send();
    }

    onQueryChanged: translate();
    onSrcLangChanged: translate();
    onTargetLangChanged: translate();
}
