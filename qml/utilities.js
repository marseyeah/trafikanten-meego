.pragma library

function jsonToDate(date) {
    return new Date(parseInt(date));
}

function formatTrafDate(refDate, date) {
    var trafTime = jsonToDate(date);
    var currTime = jsonToDate(refDate);
    var diff = trafTime.getTime() - currTime.getTime();
    var diffSec = Math.round(diff / 1000);
    if (diffSec < -1)
        return "";
    else if (diffSec < 45)
        return "0";
    else if (diffSec <= 105)
        return "1 min";
    else if (diffSec <= 165)
        return "2 min";
    else if (diffSec <= 225)
        return "3 min";
    else if (diffSec <= 285)
        return "4 min";
    else if (diffSec <= 345)
        return "5 min";
    else if (diffSec <= 405)
        return "6 min";
    else if (diffSec <= 465)
        return "7 min";
    else if (diffSec <= 525)
        return "8 min";
    else if (diffSec <= 585)
        return "9 min";
    else
        return Qt.formatDateTime(trafTime, "hh:mm");
}

function lTrim(value) {

    var re = /\s*((\S+\s*)*)/;
    return value.replace(re, "$1");

}

function rTrim(value) {

    var re = /((\s*\S+)*)\s*/;
    return value.replace(re, "$1");

}

function trim(value) {

    return lTrim(rTrim(value));

}

function ucWords(str) {
    var arrStr = str.split(" ");
    var strOut = '';

    for (var i=0;i<arrStr.length;i++)
    {
        var firstChar = arrStr[i].substring(0,1);
        var remainChar = arrStr[i].substring(1);
        firstChar = firstChar.toUpperCase();
        remainChar = remainChar.toLowerCase();
        strOut += firstChar + remainChar + ' ';
    }

    return strOut.substring(0, strOut.length - 1);
}

function stopNameSubPart(stopName, part) {
    var hasP = stopName.indexOf('(') != -1;
    var hasB = stopName.indexOf('[') != -1;
    var hasSeveralPart = hasP || hasB;
    if (!hasSeveralPart && part > 0 || part > 1)
        return "";

    var splits;
    if (hasP)
        splits = stopName.split('(');
    else
        splits = stopName.split('[')

    return ucWords(splits[part].replace(/(\)|])/i, ""));
}

function getTransportIcon(type) {
    var icon = "";
    var t = parseInt(type);
    switch (t) {
    case 1:
    case 2:
    icon = "buss_gray.png";
    break;
    case 4:
    case 6:
    icon = "tog_gray.png";
    break;
    case 5:
    icon = "boat_gray.png";
    break;
    case 7:
    icon = "trykk_gray.png";
    break;
    case 8:
    icon = "tbane_gray.png";
    break;
    }

    if (icon.length > 0)
        icon = "images/" + icon;

    return icon;
}
