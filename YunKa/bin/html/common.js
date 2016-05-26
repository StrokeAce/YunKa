var TQGF = {
   //matchPIC: new RegExp("((?:http|https|ftp|mms|rtsp)://(&(?=amp;)|[A-Za-z0-9\./=\?%_~@#:;\+\-])+\.(gif|jpg|png|bmp|jpeg))", "ig"),
    //matchURL: new RegExp("((?:http|https|ftp|mms|rtsp)://(&(?=amp;)|[A-Za-z0-9\./=\?%_~!*@&#:;\+\-])+)","ig"),
    FormatMsg: function (strmsg) {
        //strmsg = "WxImage:http://mmbiz.qpic.cn/mmbiz/NhdoVI1ibD0GfTnAFQx0ZnQiciaNNCGTylLqccVY8EpCVKqiabqP4iaz6zCsT5mTEJtUPLQZ1ibkQqdPDzBcHS5IP08Q/0";
        //strmsg = "WxVoice:https://api.weixin.qq.com/cgi-bin/media/get?access_token=VeO00TTgaQcWI6idtmccwiNAFLzV5BInLdho0xHM9vUvpaGiu0Ed4wq9WZXN8Nrx0Ywj56wIEYjHr7WqaF5jePANiiEDX2DxS-4wvTWYNgA&media_id=Wz6H2Ndq_8Kxx7-dsN5_k8HRbOFM37X0WnWCeTbJR-KhWLVcWo_7aRS4TQK23K65";
        //strmsg = "WxVoice:d:\\Wz6H2Ndq_8Kxx7-dsN5_k8HRbOFM37X0WnWCeTbJR-KhWLVcWo_7aRS4TQK23K65";
        var strret = strmsg;
        var matchImage = /((?:http|https|ftp|mms|rtsp):\/\/(&(?=amp;)|[A-Za-z0-9\.\/=\?%_~@#:;\+\-])+\.(gif|jpg|png|bmp|jpeg))/ig;
        var matchWxImage = /WxImage:\s*?((?:http|https|ftp|mms|rtsp):\/\/(&(?=amp;)|[A-Za-z0-9\.\/=&\?%_~@#:;\+\-])+)/ig;
        var matchWxAudio = /WxVoice:\s*?(.+)/ig;
        var matchURL = /((?:http|https|ftp|mms|rtsp):\/\/(&(?=amp;)|[A-Za-z0-9\.\/=\?%_~!*@&#:;\+\-])+)/ig;
       /*  if (matchWxImage.test(strmsg)) {
            strret = strret.replace(matchWxImage, "<a class='msglink' target=_blank href=$1><img style='border: 2px dotted gray;width:66%' src=$1></a>");
        } else if (matchWxAudio.test(strmsg)) {
            strret = strret.replace(/\\/g, "\\\\");
            strret = strret.replace(/ /g, "&#32;");
            //strret = strret.replace(matchWxAudio, "<IMG src=voice.png onclick=amr.PlayAudio('$1');>");
            strret = strret.replace(matchWxAudio, "<IMG src=voice.png onclick=window.navigate('app:1005@'+'$1');>");
        }else if (matchImage.test(strmsg)) {
            strret = strret.replace(matchImage, "<a class='msglink' target=_blank href=$1><img style='border: 2px dotted gray;width:66%' src=$1></a>");
        }  else if (matchURL.test(strmsg)) {
            strret = strret.replace(matchURL, "<a class='msglink' target=_blank href=$1>$1</a>");
        }*/
        return strret;
    },
    GetReplaceText: function (strmsg, strarr) {
        var retstr = strmsg;
        if (strarr instanceof Array) {
            for (var i = 0; i < strarr.length; i++) {
                if (strarr[i][0] == strmsg) {
                    retstr = strarr[i][1];
                    break;
                }
            }
        }
        return retstr;
    },
    GetSelectText: function () {
        if (window.getSelection) {
            return window.getSelection().toString();
        } else {
            if (document.getSelection) {
                return document.getSelection();
            } else {
                if (document.selection) {
                    return document.selection.createRange().text;
                }
            }
        }
        return "";
    },
    AudioStr: function ($1) {
        var str = $1.replace(/&lt;/, "");
        str = str.replace(/&gt;/, "");
        str = str.replace(/\\/g, "\\\\");
        str = "<IMG src=\"voice.png\" onclick=\"javascript:amr.PlayAudio('" + str + "');\">"
        return str;
    },
    ReplaceStringToFace: function (strmsg) {
        //strmsg = strmsg.replace(/收到微信用户发过来的语音:https:\/\/.+?media_id=(.{64})/ig, "<IMG src=\"voice.png\" onclick=\"javascript:amr.PlayAudio('$1');\">");
        //strmsg = strmsg.replace(/&lt;(.+?)&gt;/ig, this.AudioStr);
        var strret = strmsg.replace(/_tq_face_(\d{1,3})_/ig, "<IMG src=\"../Faces/$1.gif\">");
        return strret;
    },
    EncodeHtmlTags: function (strmsg) {
        var strret = strmsg;
        //strret = strret.replace(/&/ig, "&amp;");
        //strret = strret.replace(/</ig, "&lt;");
        //strret = strret.replace(/>/ig, "&gt;");
    //strret = strret.replace(/\'/ig, "&apos;");
    //strret = strret.replace(/\"/ig, "&quot;");
        //strret = strret.replace(/\n/ig, "<br>");
        return strret;
    },
    FacesIndexStr: function ($1) {
        var fid = $1.replace(/\\</, "");
        fid = fid.replace(/>/, "");
        var idx = fid;
        var retstr = "_tq_face_" + idx + "_";
        return retstr;
    },
    SaveHtmlElement: function (strmsg) {
        var strret = strmsg.replace(/\\<(\d{1,3})>/ig, "_tq_face_$1_");
        return strret;
    }
};

var TQHtmlCheck = {
    allstr: [],
    stag: "<![TQHTML[",
    etag: "]]>",
    repl: "",
    get: function () {
        var len = TQHtmlCheck.allstr.length;
        var retval = TQHtmlCheck.repl;
        for (var i = 0; i < len; i++) {
            retval = retval.replace("_tq_html_code_" + i + "_", TQHtmlCheck.allstr[i]);
        }
        return retval;
    },
    replace: function (str) {
        var retval = false, repstr = "";
        var spos = 0, epos = 0, ppos = 0, idx = 0;
        var f = false;
        TQHtmlCheck.repl = "";
        do {
            f = false;
            spos = str.indexOf(TQHtmlCheck.stag, epos);
            if (spos != -1) {
                var ss = spos + TQHtmlCheck.stag.length;
                epos = str.indexOf(TQHtmlCheck.etag, ss);
                var sublen = epos - ss;
                if (sublen >= 0) {
                    repstr = "_tq_html_code_" + idx + "_";
                    TQHtmlCheck.allstr[idx++] = str.substr(ss, sublen);
                    epos += TQHtmlCheck.etag.length;
                    f = true;
                    retval = true;
                }
            }
            if (f) {
                TQHtmlCheck.repl += str.substr(ppos, spos - ppos);
                TQHtmlCheck.repl += repstr;
                ppos = epos;
            } else {
                TQHtmlCheck.repl += str.substr(epos);
            }
        } while (spos != -1 && epos != -1 && epos < str.length);
        return retval;
    }
};

String.prototype.trim = function () { return this.replace(/^\s+|\s+$/g, ""); }
String.prototype.ltrim = function () { return this.replace(/^\s+/, ""); }
String.prototype.rtrim = function () { return this.replace(/\s+$/, ""); }

