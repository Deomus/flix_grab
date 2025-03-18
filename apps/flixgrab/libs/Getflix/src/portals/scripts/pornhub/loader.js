(function() {
    var e = this,
        t = e._,
        n = Array.prototype,
        o = Object.prototype,
        r = Function.prototype,
        a = n.push,
        i = n.slice,
        s = (n.concat, o.toString),
        l = o.hasOwnProperty,
        u = Array.isArray,
        c = Object.keys,
        f = (r.bind, Object.create, function(e) {
            return e instanceof f ? e : this instanceof f ? void(this._wrapped = e) : new f(e)
        });
    "undefined" != typeof exports ? ("undefined" != typeof module && module.exports && (exports = module.exports = f), exports._ = f) : e._ = f, f.VERSION = "1.8.2";
    var p = function(a, i, e) {
            if (void 0 === i) return a;
            switch (null == e ? 3 : e) {
                case 1:
                    return function(e) {
                        return a.call(i, e)
                    };
                case 2:
                    return function(e, t) {
                        return a.call(i, e, t)
                    };
                case 3:
                    return function(e, t, n) {
                        return a.call(i, e, t, n)
                    };
                case 4:
                    return function(e, t, n, r) {
                        return a.call(i, e, t, n, r)
                    }
            }
            return function() {
                return a.apply(i, arguments)
            }
        },
        d = function(e, t, n) {
            return null == e ? f.identity : f.isFunction(e) ? p(e, t, n) : f.isObject(e) ? f.matcher(e) : f.property(e)
        };
    f.iteratee = function(e, t) {
        return d(e, t, 1 / 0)
    };
    var h = function(l, u) {
            return function(e) {
                var t = arguments.length;
                if (t < 2 || null == e) return e;
                for (var n = 1; n < t; n++)
                    for (var r = arguments[n], a = l(r), i = a.length, o = 0; o < i; o++) {
                        var s = a[o];
                        u && void 0 !== e[s] || (e[s] = r[s])
                    }
                return e
            }
        },
        y = Math.pow(2, 53) - 1,
        m = function(e) {
            var t = e && e.length;
            return "number" == typeof t && 0 <= t && t <= y
        };
    f.each = f.forEach = function(e, t, n) {
        var r, a;
        if (t = p(t, n), m(e))
            for (r = 0, a = e.length; r < a; r++) t(e[r], r, e);
        else {
            var i = f.keys(e);
            for (r = 0, a = i.length; r < a; r++) t(e[i[r]], i[r], e)
        }
        return e
    }, f.map = f.collect = function(e, t, n) {
        t = d(t, n);
        for (var r = !m(e) && f.keys(e), a = (r || e).length, i = Array(a), o = 0; o < a; o++) {
            var s = r ? r[o] : o;
            i[o] = t(e[s], s, e)
        }
        return i
    }, f.find = f.detect = function(e, t, n) {
        var r;
        if (void 0 !== (r = m(e) ? f.findIndex(e, t, n) : f.findKey(e, t, n)) && -1 !== r) return e[r]
    }, f.filter = f.select = function(e, r, t) {
        var a = [];
        return r = d(r, t), f.each(e, function(e, t, n) {
            r(e, t, n) && a.push(e)
        }), a
    }, f.reject = function(e, t, n) {
        return f.filter(e, f.negate(d(t)), n)
    }, f.every = f.all = function(e, t, n) {
        t = d(t, n);
        for (var r = !m(e) && f.keys(e), a = (r || e).length, i = 0; i < a; i++) {
            var o = r ? r[i] : i;
            if (!t(e[o], o, e)) return !1
        }
        return !0
    }, f.some = f.any = function(e, t, n) {
        t = d(t, n);
        for (var r = !m(e) && f.keys(e), a = (r || e).length, i = 0; i < a; i++) {
            var o = r ? r[i] : i;
            if (t(e[o], o, e)) return !0
        }
        return !1
    }, f.contains = f.includes = f.include = function(e, t, n) {
        return m(e) || (e = f.values(e)), 0 <= f.indexOf(e, t, "number" == typeof n && n)
    }, f.pluck = function(e, t) {
        return f.map(e, f.property(t))
    }, f.max = function(e, r, t) {
        var n, a, i = -1 / 0,
            o = -1 / 0;
        if (null == r && null != e)
            for (var s = 0, l = (e = m(e) ? e : f.values(e)).length; s < l; s++) n = e[s], i < n && (i = n);
        else r = d(r, t), f.each(e, function(e, t, n) {
            a = r(e, t, n), (o < a || a === -1 / 0 && i === -1 / 0) && (i = e, o = a)
        });
        return i
    }, f.sortBy = function(e, r, t) {
        return r = d(r, t), f.pluck(f.map(e, function(e, t, n) {
            return {
                value: e,
                index: t,
                criteria: r(e, t, n)
            }
        }).sort(function(e, t) {
            var n = e.criteria,
                r = t.criteria;
            if (n !== r) {
                if (r < n || void 0 === n) return 1;
                if (n < r || void 0 === r) return -1
            }
            return e.index - t.index
        }), "value")
    }, f.size = function(e) {
        return null == e ? 0 : m(e) ? e.length : f.keys(e).length
    };
    var g = function(e, t, n, r) {
        for (var a = [], i = 0, o = r || 0, s = e && e.length; o < s; o++) {
            var l = e[o];
            if (m(l) && (f.isArray(l) || f.isArguments(l))) {
                t || (l = g(l, t, n));
                var u = 0,
                    c = l.length;
                for (a.length += c; u < c;) a[i++] = l[u++]
            } else n || (a[i++] = l)
        }
        return a
    };

    function _(i) {
        return function(e, t, n) {
            t = d(t, n);
            for (var r = null != e && e.length, a = 0 < i ? 0 : r - 1; 0 <= a && a < r; a += i)
                if (t(e[a], a, e)) return a;
            return -1
        }
    }
    f.flatten = function(e, t) {
        return g(e, t, !1)
    }, f.object = function(e, t) {
        for (var n = {}, r = 0, a = e && e.length; r < a; r++) t ? n[e[r]] = t[r] : n[e[r][0]] = e[r][1];
        return n
    }, f.indexOf = function(e, t, n) {
        var r = 0,
            a = e && e.length;
        if ("number" == typeof n) r = n < 0 ? Math.max(0, a + n) : n;
        else if (n && a) return e[r = f.sortedIndex(e, t)] === t ? r : -1;
        if (t != t) return f.findIndex(i.call(e, r), f.isNaN);
        for (; r < a; r++)
            if (e[r] === t) return r;
        return -1
    }, f.findIndex = _(1), f.findLastIndex = _(-1);
    f.negate = function(e) {
        return function() {
            return !e.apply(this, arguments)
        }
    };
    var v = !{
            toString: null
        }.propertyIsEnumerable("toString"),
        b = ["valueOf", "isPrototypeOf", "toString", "propertyIsEnumerable", "hasOwnProperty", "toLocaleString"];

    function P(e, t) {
        var n = b.length,
            r = e.constructor,
            a = f.isFunction(r) && r.prototype || o,
            i = "constructor";
        for (f.has(e, i) && !f.contains(t, i) && t.push(i); n--;)(i = b[n]) in e && e[i] !== a[i] && !f.contains(t, i) && t.push(i)
    }
    f.keys = function(e) {
        if (!f.isObject(e)) return [];
        if (c) return c(e);
        var t = [];
        for (var n in e) f.has(e, n) && t.push(n);
        return v && P(e, t), t
    }, f.allKeys = function(e) {
        if (!f.isObject(e)) return [];
        var t = [];
        for (var n in e) t.push(n);
        return v && P(e, t), t
    }, f.invert = function(e) {
        for (var t = {}, n = f.keys(e), r = 0, a = n.length; r < a; r++) t[e[n[r]]] = n[r];
        return t
    }, f.functions = f.methods = function(e) {
        var t = [];
        for (var n in e) f.isFunction(e[n]) && t.push(n);
        return t.sort()
    }, f.extend = h(f.allKeys), f.extendOwn = f.assign = h(f.keys), f.findKey = function(e, t, n) {
        t = d(t, n);
        for (var r, a = f.keys(e), i = 0, o = a.length; i < o; i++)
            if (t(e[r = a[i]], r, e)) return r
    }, f.pick = function(e, t, n) {
        var r, a, i = {},
            o = e;
        if (null == o) return i;
        f.isFunction(t) ? (a = f.allKeys(o), r = p(t, n)) : (a = g(arguments, !1, !1, 1), r = function(e, t, n) {
            return t in n
        }, o = Object(o));
        for (var s = 0, l = a.length; s < l; s++) {
            var u = a[s],
                c = o[u];
            r(c, u, o) && (i[u] = c)
        }
        return i
    }, f.omit = function(e, t, n) {
        if (f.isFunction(t)) t = f.negate(t);
        else {
            var r = f.map(g(arguments, !1, !1, 1), String);
            t = function(e, t) {
                return !f.contains(r, t)
            }
        }
        return f.pick(e, t, n)
    }, f.defaults = h(f.allKeys, !0), f.clone = function(e) {
        return f.isObject(e) ? f.isArray(e) ? e.slice() : f.extend({}, e) : e
    }, f.isEmpty = function(e) {
        return null == e || (m(e) && (f.isArray(e) || f.isString(e) || f.isArguments(e)) ? 0 === e.length : 0 === f.keys(e).length)
    }, f.isElement = function(e) {
        return !(!e || 1 !== e.nodeType)
    }, f.isArray = u || function(e) {
        return "[object Array]" === s.call(e)
    }, f.isObject = function(e) {
        var t = typeof e;
        return "function" === t || "object" === t && !!e
    }, f.each(["Arguments", "Function", "String", "Number", "Date", "RegExp", "Error"], function(t) {
        f["is" + t] = function(e) {
            return s.call(e) === "[object " + t + "]"
        }
    }), f.isArguments(arguments) || (f.isArguments = function(e) {
        return f.has(e, "callee")
    }), "function" != typeof /./ && "object" != typeof Int8Array && (f.isFunction = function(e) {
        return "function" == typeof e || !1
    }), f.isNaN = function(e) {
        return f.isNumber(e) && e !== +e
    }, f.isBoolean = function(e) {
        return !0 === e || !1 === e || "[object Boolean]" === s.call(e)
    }, f.isNull = function(e) {
        return null === e
    }, f.isUndefined = function(e) {
        return void 0 === e
    }, f.has = function(e, t) {
        return null != e && l.call(e, t)
    }, f.noConflict = function() {
        return e._ = t, this
    }, f.property = function(t) {
        return function(e) {
            return null == e ? void 0 : e[t]
        }
    }, f.random = function(e, t) {
        return null == t && (t = e, e = 0), e + Math.floor(Math.random() * (t - e + 1))
    };
    var x = {
            "&": "&amp;",
            "<": "&lt;",
            ">": "&gt;",
            '"': "&quot;",
            "'": "&#x27;",
            "`": "&#x60;"
        },
        M = f.invert(x),
        S = function(t) {
            var n = function(e) {
                    return t[e]
                },
                e = "(?:" + f.keys(t).join("|") + ")",
                r = RegExp(e),
                a = RegExp(e, "g");
            return function(e) {
                return e = null == e ? "" : "" + e, r.test(e) ? e.replace(a, n) : e
            }
        };
    f.escape = S(x), f.unescape = S(M);
    var T = 0;
    f.uniqueId = function(e) {
        var t = ++T + "";
        return e ? e + t : t
    }, f.templateSettings = {
        evaluate: /<%([\s\S]+?)%>/g,
        interpolate: /<%=([\s\S]+?)%>/g,
        escape: /<%-([\s\S]+?)%>/g
    };
    var w = /(.)^/,
        O = {
            "'": "'",
            "\\": "\\",
            "\r": "r",
            "\n": "n",
            "\u2028": "u2028",
            "\u2029": "u2029"
        },
        H = /\\|'|\r|\n|\u2028|\u2029/g,
        k = function(e) {
            return "\\" + O[e]
        };
    f.template = function(i, e, t) {
        !e && t && (e = t), e = f.defaults({}, e, f.templateSettings);
        var n = RegExp([(e.escape || w).source, (e.interpolate || w).source, (e.evaluate || w).source].join("|") + "|$", "g"),
            o = 0,
            s = "__p+='";
        i.replace(n, function(e, t, n, r, a) {
            return s += i.slice(o, a).replace(H, k), o = a + e.length, t ? s += "'+\n((__t=(" + t + "))==null?'':_.escape(__t))+\n'" : n ? s += "'+\n((__t=(" + n + "))==null?'':__t)+\n'" : r && (s += "';\n" + r + "\n__p+='"), e
        }), s += "';\n", e.variable || (s = "with(obj||{}){\n" + s + "}\n"), s = "var __t,__p='',__j=Array.prototype.join,print=function(){__p+=__j.call(arguments,'');};\n" + s + "return __p;\n";
        try {
            var r = new Function(e.variable || "obj", "_", s)
        } catch (e) {
            throw e.source = s, e
        }
        var a = function(e) {
                return r.call(this, e, f)
            },
            l = e.variable || "obj";
        return a.source = "function(" + l + "){\n" + s + "}", a
    }, f.chain = function(e) {
        var t = f(e);
        return t._chain = !0, t
    };
    f.mixin = function(t) {
        f.each(f.functions(t), function(e) {
            var r = f[e] = t[e];
            f.prototype[e] = function() {
                var e, t, n = [this._wrapped];
                return a.apply(n, arguments), e = this, t = r.apply(f, n), e._chain ? f(t).chain() : t
            }
        })
    }, f.mixin(f), f.prototype.value = function() {
        return this._wrapped
    }, f.prototype.valueOf = f.prototype.toJSON = f.prototype.value, f.prototype.toString = function() {
        return "" + this._wrapped
    }, "function" == typeof define && define.amd && define("underscore", [], function() {
        return f
    })
}).call(this), __ = _.noConflict(), "object" != typeof JSON && (JSON = {}),
    function() {
        "use strict";
        var rx_one = /^[\],:{}\s]*$/,
            rx_two = /\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g,
            rx_three = /"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g,
            rx_four = /(?:^|:|,)(?:\s*\[)+/g,
            rx_escapable = /[\\\"\u0000-\u001f\u007f-\u009f\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,
            rx_dangerous = /[\u0000\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,
            gap, indent, meta, rep;

        function f(e) {
            return e < 10 ? "0" + e : e
        }

        function this_value() {
            return this.valueOf()
        }

        function quote(e) {
            return rx_escapable.lastIndex = 0, rx_escapable.test(e) ? '"' + e.replace(rx_escapable, function(e) {
                var t = meta[e];
                return "string" == typeof t ? t : "\\u" + ("0000" + e.charCodeAt(0).toString(16)).slice(-4)
            }) + '"' : '"' + e + '"'
        }

        function str(e, t) {
            var n, r, a, i, o, s = gap,
                l = t[e];
            switch (l && "object" == typeof l && "function" == typeof l.toJSON && (l = l.toJSON(e)), "function" == typeof rep && (l = rep.call(t, e, l)), typeof l) {
                case "string":
                    return quote(l);
                case "number":
                    return isFinite(l) ? String(l) : "null";
                case "boolean":
                case "null":
                    return String(l);
                case "object":
                    if (!l) return "null";
                    if (gap += indent, o = [], "[object Array]" === Object.prototype.toString.apply(l)) {
                        for (i = l.length, n = 0; n < i; n += 1) o[n] = str(n, l) || "null";
                        return a = 0 === o.length ? "[]" : gap ? "[\n" + gap + o.join(",\n" + gap) + "\n" + s + "]" : "[" + o.join(",") + "]", gap = s, a
                    }
                    if (rep && "object" == typeof rep)
                        for (i = rep.length, n = 0; n < i; n += 1) "string" == typeof rep[n] && (a = str(r = rep[n], l)) && o.push(quote(r) + (gap ? ": " : ":") + a);
                    else
                        for (r in l) Object.prototype.hasOwnProperty.call(l, r) && (a = str(r, l)) && o.push(quote(r) + (gap ? ": " : ":") + a);
                    return a = 0 === o.length ? "{}" : gap ? "{\n" + gap + o.join(",\n" + gap) + "\n" + s + "}" : "{" + o.join(",") + "}", gap = s, a
            }
        }
        "function" != typeof Date.prototype.toJSON && (Date.prototype.toJSON = function() {
            return isFinite(this.valueOf()) ? this.getUTCFullYear() + "-" + f(this.getUTCMonth() + 1) + "-" + f(this.getUTCDate()) + "T" + f(this.getUTCHours()) + ":" + f(this.getUTCMinutes()) + ":" + f(this.getUTCSeconds()) + "Z" : null
        }, Boolean.prototype.toJSON = this_value, Number.prototype.toJSON = this_value, String.prototype.toJSON = this_value), "function" != typeof JSON.stringify && (meta = {
            "\b": "\\b",
            "\t": "\\t",
            "\n": "\\n",
            "\f": "\\f",
            "\r": "\\r",
            '"': '\\"',
            "\\": "\\\\"
        }, JSON.stringify = function(e, t, n) {
            var r;
            if (indent = gap = "", "number" == typeof n)
                for (r = 0; r < n; r += 1) indent += " ";
            else "string" == typeof n && (indent = n);
            if ((rep = t) && "function" != typeof t && ("object" != typeof t || "number" != typeof t.length)) throw new Error("JSON.stringify");
            return str("", {
                "": e
            })
        }), "function" != typeof JSON.parse && (JSON.parse = function(text, reviver) {
            var j;

            function walk(e, t) {
                var n, r, a = e[t];
                if (a && "object" == typeof a)
                    for (n in a) Object.prototype.hasOwnProperty.call(a, n) && (void 0 !== (r = walk(a, n)) ? a[n] = r : delete a[n]);
                return reviver.call(e, t, a)
            }
            if (text = String(text), rx_dangerous.lastIndex = 0, rx_dangerous.test(text) && (text = text.replace(rx_dangerous, function(e) {
                    return "\\u" + ("0000" + e.charCodeAt(0).toString(16)).slice(-4)
                })), rx_one.test(text.replace(rx_two, "@").replace(rx_three, "]").replace(rx_four, ""))) return j = eval("(" + text + ")"), "function" == typeof reviver ? walk({
                "": j
            }, "") : j;
            throw new SyntaxError("JSON.parse")
        })
    }();
var store = {};
try {
    var storage = window.localStorage;
    storage.setItem("storage_test", 1), storage.removeItem("storage_test")
} catch (e) {
    var storage = "";
    storage = new function() {
        function n(e) {
            var t, n, r, a, i;
            e = encodeURIComponent(JSON.stringify(e)), t = "player_storage", n = e, i = (r = 365) ? ((a = new Date).setTime(a.getTime() + 24 * r * 60 * 60 * 1e3), "; expires=" + a.toGMTString()) : "", document.cookie = t + "=" + n + i + "; path=/"
        }
        var e, r = (e = function(e) {
            var t, n, r = e + "=",
                a = document.cookie.split(";");
            for (t = 0; t < a.length; t++)
                if (0 == (n = a[t].trim()).indexOf(r)) return n.substring(r.length, n.length);
            return null
        }("player_storage")) ? JSON.parse(decodeURIComponent(e)) : {};
        return {
            length: 0,
            getItem: function(e) {
                return void 0 === r[e] ? null : r[e]
            },
            removeItem: function(e) {
                delete r[e], this.length--, n(r)
            },
            setItem: function(e, t) {
                r[e] = t + "", this.length++, n(r)
            }
        }
    }
}
store.has = function(e) {
    return void 0 !== store.get(e)
}, store.serialize = function(e) {
    return JSON.stringify(e)
}, store.deserialize = function(t) {
    if ("string" == typeof t) try {
        return JSON.parse(t)
    } catch (e) {
        return t || void 0
    }
}, store.remove = function(e) {
    try {
        storage.removeItem(e)
    } catch (e) {}
}, store.set = function(e, t) {
    if (void 0 === t) return store.remove(e);
    try {
        storage.setItem(e, store.serialize(t))
    } catch (e) {
        return !1
    }
    return t
}, store.get = function(e, t) {
    try {
        var n = store.deserialize(storage.getItem(e))
    } catch (e) {}
    return void 0 === n ? t : n
}, window.store = store;
var __, MHP1138 = {
    initLoader: function(e) {
        MHP1138.loader.initLoader(e)
    },
    createPlayer: function(e, t) {
        this.playerReady ? MHP1138.player.createPlayer(e, t) : MHP1138.players[e] = t
    },
    playerType: function(e) {
        return MHP1138.player.playerType(e)
    },
    isPlaying: function(e) {
        return MHP1138.player.isPlaying(e)
    },
    destroyPlayer: function(e, t) {
        MHP1138.player.destroyPlayer(e, t)
    },
    seek: function(e, t, n) {
        __.isBoolean(n) || (n = !0), MHP1138.player.seek(e, t, n)
    },
    play: function(e) {
        MHP1138.player.play(e)
    },
    pause: function(e) {
        MHP1138.player.pause(e)
    },
    setMute: function(e, t) {
        MHP1138.player.setMute(e, t)
    },
    getVolume: function(e) {
        return MHP1138.player.getVolume(e)
    },
    setVolume: function(e, t) {
        MHP1138.player.setVolume(e, t)
    },
    getCurrentTime: function(e) {
        return MHP1138.player.getCurrentTime(e)
    },
    exitFullscreen: function(e) {
        MHP1138.player.exitFullscreen(e)
    },
    inspect: function() {
        return MHP1138.player.inspect()
    },
    playerStatus: function() {
        return MHP1138.player.playerStatus()
    },
    getPlayerIds: function() {
        return MHP1138.player.getPlayerIds()
    },
    checkIE: function() {
        var e = -1;
        if ("Microsoft Internet Explorer" == navigator.appName) {
            var t = navigator.userAgent;
            null != new RegExp("MSIE ([0-9]{1,}[.0-9]{0,})").exec(t) && (e = parseFloat(RegExp.$1))
        }
        return e
    },
    loader: null,
    player: null,
    detector: null,
    choosenPlayer: {
        playerName: "",
        version: "stable"
    },
    skins: {},
    symbols: "abcdefghijklmnopqrstuvwxyz.|-1234567890",
    skinsMarkup: {},
    loadedMasterCSS: !1,
    players: {},
    playerReady: !1,
    isBasic: !1,
    loaderVersion: "3.10.3",
    appVersions: {},
    flashPlayer_eventFunctionCreated: !1,
    isOldIE: !1,
    isNoFullScreenIE: function() {
        return this.checkIE() < 11 && !(-1 === this.checkIE()) && !(0 === this.checkIE())
    },
    minified: !0,
    addFileModeToPath: function(e) {
        return this.minified && (e = e.substr(0, e.lastIndexOf(".")) + ".min" + e.substr(e.lastIndexOf("."))), e
    },
    log: function(e) {
        this.outputConsoleLog("mhp1138: " + e, "log")
    },
    warn: function(e) {
        this.outputConsoleLog("mhp1138: " + e, "warn")
    },
    error: function(e) {
        this.outputConsoleLog("mhp1138: v.3.10.3 -- o_O --\x3e " + e, "error")
    },
    outputConsoleLog: function(e, t) {
        this.isOldIE || this.minified || console[t](e)
    },
    pathToFileName: function(e) {
        return e.replace(/^.*[\\\/]/, "")
    },
    getParameterByName: function(e) {
        if (null == (t = RegExp("[#&]" + e + "=([^&]*)").exec(window.location.hash))) var t = RegExp("[?&]" + e + "=([^&]*)").exec(window.location.search);
        return null != t && decodeURIComponent(t[1].replace(/\+/g, " "))
    }
};
MHP1138.loader = function() {
    window.location.href.match(/pornhub/i);
    var m = {
            playerUsageOrder: [],
            playerVersionArray: [{
                playerName: "HTML5Player",
                version: "3.10.3"
            }, {
                playerName: "HTML5PlayerMobile",
                version: "3.10.3"
            }, {
                playerName: "4Play",
                version: "1.6.11"
            }, {
                playerName: "oldFlash",
                version: "3.2.7"
            }, {
                playerName: "imagePlayer",
                version: "1.2.3"
            }, {
                playerName: "basicHTML5",
                version: "1.5.0"
            }],
            pathToBuildFolder: "//ss.phncdn.com/html5player/",
            embeds: {
                enabled: !1,
                mediaType: "image"
            },
            fallbackFunction: function() {},
            supportTest: {
                basicHTML5: {
                    fileToSupport: ["h264"]
                },
                HTML5Player: {
                    fileToSupport: ["h264"]
                },
                "4Play": {
                    minFlashVersion: "10.2"
                },
                oldFlash: {
                    minFlashVersion: "9.0"
                },
                HTML5PlayerMobile: {
                    fileToSupport: ["h264"]
                }
            }
        },
        g = {};

    function _(e) {
        return __.all(m.supportTest[e].fileToSupport, function(e) {
            return __.videoFormatIsSupported(e)
        })
    }

    function t(e) {
        MHP1138.getParameterByName("debug") && (MHP1138.minified = !1), __.isObject(e.playerDefaultSettings) && (g = e.playerDefaultSettings), m = __.deepExtend(m, __.omit(e, "playerDefaultSettings")), MHP1138.loaderSettings = m,
            function() {
                MHP1138.detector.init(m.deviceType);
                var e = function() {
                        var r = [];
                        0 < m.playerUsageOrder.length ? __.each(m.playerUsageOrder, function(e) {
                            var n = e.split("_"),
                                t = __.findKey(m.playerVersionArray, function(e, t) {
                                    return 0 <= e.playerName.indexOf(n[0])
                                });
                            r.push(m.playerVersionArray[t])
                        }) : r = m.playerVersionArray;
                        return r
                    }(),
                    t = m.embeds.enabled;
                MHP1138.appVersions = e;
                var n = MHP1138.detector.getBrowser(),
                    r = "safari" === n.name && n.version <= 7,
                    a = !1;
                try {
                    a = Boolean(new ActiveXObject("ShockwaveFlash.ShockwaveFlash"))
                } catch (e) {
                    a = void 0 !== navigator.mimeTypes["application/x-shockwave-flash"]
                }
                var i = "0.0";
                if (a) {
                    var i = function() {
                        try {
                            try {
                                var e = new ActiveXObject("ShockwaveFlash.ShockwaveFlash.6");
                                try {
                                    e.AllowScriptAccess = "always"
                                } catch (e) {
                                    return "6,0,0"
                                }
                            } catch (e) {}
                            return new ActiveXObject("ShockwaveFlash.ShockwaveFlash").GetVariable("$version").replace(/\D+/g, ",").match(/^,?(.+),?$/)[1]
                        } catch (e) {
                            try {
                                if (navigator.mimeTypes["application/x-shockwave-flash"].enabledPlugin) return (navigator.plugins["Shockwave Flash 2.0"] || navigator.plugins["Shockwave Flash"]).description.replace(/\D+/g, ",").match(/^,?(.+),?$/)[1]
                            } catch (e) {}
                        }
                        return "0,0,0"
                    }().replace(/,/gi, ".")
                }
                var o = MHP1138.detector,
                    s = 537 <= checkWebKit() && 36 <= getChromeVersion() && !o.isWindows();
                var l = "video" === m.embeds.mediaType,
                    u = o.isIos() && v()[0] < 8 && !o.isWindows(),
                    c = o.isIos() && v()[0] < 10 && !o.isWindows(),
                    f = (o.isIos() && 9 < v()[0] || o.isMacos() && ("ontouchstart" in window || 0 < navigator.MaxTouchPoints || 0 < navigator.msMaxTouchPoints)) && !o.isWindows(),
                    p = s && !o.isFirefox() || f;
                if (MHP1138.iOSversion = v()[0], MHP1138.choosenPlayer = __.find(e, function(e) {
                        switch (e.playerName) {
                            case "4Play":
                                return _(e.playerName) && a && !o.isSmartTV() && !o.isTablet() && !o.isMobile();
                            case "imagePlayer":
                                return t && (o.isMobile() && (!l || l && !p && !c) || o.isTablet() && u);
                            case "basicHTML5":
                            case "basicHTML5Phub":
                                return MHP1138.isBasic = !0, !MHP1138.isOldIE;
                            case "HTML5Player":
                                return _(e.playerName) && o.isDesktop() && !MHP1138.isNoFullScreenIE() && !o.isPlaystation() && !o.isXbox() && !r && !o.isSmartTV() && !o.isTablet();
                            case "HTML5PlayerMobile":
                                return _(e.playerName) && o.isMobile() && !o.isIe() && !o.isPlaystation() && !o.isXbox() && p && (!t || t && l) || o.isTablet() && !o.isIe() && (s || o.isFirefox() && !o.isIos() || o.isFirefox() && t || o.isIos() && 6 < v()[0]);
                            case "oldFlash":
                                return parseFloat(i) >= parseFloat(m.supportTest[e.playerName].minFlashVersion) && !o.isMobile() && !o.isTablet()
                        }
                    }), MHP1138.getParameterByName("forcePlayer")) {
                    var d = MHP1138.getParameterByName("forcePlayer");
                    if (-1 == d.indexOf("_")) MHP1138.choosenPlayer = {
                        playerName: d,
                        version: "stable"
                    };
                    else {
                        var h = d.split("_");
                        __.isArray(h) && 2 == h.length && h[0].length && h[1].length && (MHP1138.choosenPlayer = {
                            playerName: h[0],
                            version: h[1]
                        })
                    }
                }
                if (__.isUndefined(MHP1138.choosenPlayer)) {
                    var y = m.fallbackFunction.toString().match(/\{([\s\S]*)\}/m)[1];
                    return y = y.replace(/^\s*\/\/.*$/gm, ""), methodBodyLength = y.replace(/^\s+|\s+$/g, "").length, !__.isUndefined(m.fallbackFunction) && __.isFunction(m.fallbackFunction) && 0 < methodBodyLength ? m.fallbackFunction() : __.videoFormatIsSupported("h264") || MHP1138.isOldIE ? alert("please download flash") : alert("please either install flash or H.264 decoder to support player")
                }
                MHP1138.player.initPlayer(g, MHP1138.choosenPlayer.playerName)
            }()
    }

    function v() {
        if (/iP(hone|od|ad)/.test(navigator.userAgent)) {
            var e = navigator.appVersion.match(/OS (\d+)_(\d+)_?(\d+)?/);
            return [parseInt(e[1]), parseInt(e[2]), parseInt(e[3] || 0)]
        }
        return [0, 0, 0]
    }
    return {
        initLoader: function(e) {
            t(e)
        },
        testFormats: _,
        scriptLoader: function(e, t, n) {
            ! function(t, n, r) {
                var a, e = document,
                    i = e.getElementsByTagName("head")[0];
                if ("HTML5Player" == MHP1138.choosenPlayer.playerName && (void 0 !== m.adaptive && !1 === m.adaptive || (t = t.replace("HTML5Player.", "HTML5Player.adaptive."))), "HTML5PlayerMobile" == MHP1138.choosenPlayer.playerName && (void 0 !== m.adaptive && !1 === m.adaptive || "safari" == MHP1138.detector.getBrowser().name || (t = t.replace("HTML5PlayerMobile.", "HTML5PlayerMobile.adaptive."))), "js" == n)(a = e.createElement("script")).src = t, a.type = "text/javascript";
                else {
                    if ("css" != n) return MHP1138.error("Invalid file format passed to scriptLoader function.");
                    (a = e.createElement("link")).rel = "stylesheet", a.type = "text/css", a.href = t
                }
                a.async = !0, a.onload = a.onreadystatechange = function(e) {
                    a.readyState && !/loaded|complete/.test(a.readyState) || (a.onload = a.onreadystatechange = null, a.parentNode && "css" !== n && a.parentNode.removeChild(a), a = null, r && r(200, "success", MHP1138.pathToFileName(t), n))
                }, i.insertBefore(a, i.firstChild)
            }(e, t, n)
        },
        pathToBuildFolder: function() {
            return m.pathToBuildFolder
        }
    }
}(), MHP1138.player = function() {
    var e = {
            actionTags: "",
            appId: "10829",
            autoPlay: !1,
            campaignName: "",
            siteName: "",
            clickUrl: "",
            closedCaptions: "",
            forgetUserAfter: 86400,
            imageHeight: 0,
            imageUrl: "",
            imageWidth: 0,
            mediaPriority: "hls",
            mediaDefinition: [],
            onNth: 1,
            overlays: [],
            overlayTextAd: {
                displayDuration: 45,
                displayText: "",
                linkUrl: "",
                showDelay: 5
            },
            poster: "",
            profileAdUrl: "",
            skippable: !1,
            skipDelay: 0,
            skipDelayMessage: "",
            skipMessage: "",
            clickableAreas: {
                video: !1,
                link: !0
            },
            cta: {
                image: "",
                title: "",
                subtitle: "",
                button: "",
                url: "",
                expand: !1,
                timer: !0
            },
            titleCta: {
                mobileText: "",
                pcText: "",
                link: "",
                tracking: ""
            },
            adaptive: {
                defaultBandwidthEstimate: 15e5,
                prebufferGoal: 120,
                bandwidthDowngradeTarget: .75,
                bandwidthUpgradeTarget: .7,
                switchInterval: 2
            },
            shownOnlyInFullScreen: !1,
            thumbs: {
                urlPattern: "",
                format: "5x5",
                type: "normal",
                cdnType: "regular",
                preload: !1,
                progressive: !1,
                async: !1,
                samplingFrequency: "0",
                thumbWidth: "160",
                thumbHeight: "90",
                vertical: !1,
                crop: !1
            },
            timing: "main",
            title: "",
            trackUrl: "",
            type: "video",
            duration: 0,
            vertical: !1,
            videoUnavailable: !1,
            videoUnavailableMessage: "",
            videoUrl: "",
            watchPageUrl: "",
            weight: 1,
            campaignWeight: 1,
            user_accept_language: ""
        },
        o = {
            autoplay: !1,
            autoplayAds: !0,
            hotspots: {
                enabled: !0,
                chartColor: "#635b58",
                chartHeight: "30px",
                data: []
            },
            nextVideo: {
                thumb: "",
                duration: "",
                title: "",
                isHD: !1,
                nextUrl: ""
            },
            seekPreview: {
                enabled: !0,
                blur: 3,
                showInBufferedZone: !1
            },
            locale: "en",
            quickSetup: !1,
            env: "",
            siteId: null,
            seekParams: {},
            startOffset: 0,
            productUrl: "",
            referrerUrl: "",
            videoPreload: "none",
            maxSeeksInProgress: 2,
            monitorBandwidth: !0,
            viewedRequestTimeout: 60,
            viewedRequestURL: "",
            vastParserErrorReportURL: "https://www.ezfuck.com/vast/?vast=",
            isVr: !1,
            htmlSettings: {
                skin: "default",
                notAvailableMessage: "%NOT_AVAILABLE%",
                videoErrorMessage: "%ERROR_OCCURED%",
                skipMessage: "%SKIP_AD%",
                skipDelayMessage: "%SKIP_TIMER%",
                adsTrackUrl: "https://etahub.com/events?app_id=%APPID%&splayer=%PLAYER%&eventName=%ACTION%&ssite=%SITE%&scampaign=%CAMPAIGN%&ssiteName=%SITENAME%&platform=%PLATFORM%&adTitle=%ADTITLE%",
                listedTrackUrl: "https://etahub.com/events?app_id=10940&eventName=%ACTION%&ssiteName=%SITENAME%&ssite=%SITE%&platform=%PLATFORM%"
            },
            eventTracking: {
                enabled: !0,
                cdn: "",
                videoId: "",
                url: "https://etahub.com/events?app_id=10896&ssiteName=%SITENAME%&platform=%PLATFORM%&featureName=%NAME%&featureValue=%VALUE%"
            },
            embeds: {
                enabled: !1,
                logoEnabled: !0,
                imageUrl: "",
                watchHD: !1,
                utmRedirect: {
                    logo: !0,
                    title: !0,
                    relatedBtns: !0,
                    thumbs: !0,
                    videoArea: !0
                },
                redirect: {
                    onFullscreen: !1,
                    onMenu: !1,
                    videoArea: !0,
                    logoUrl: "",
                    relatedUrl: "",
                    mostViewedUrl: "",
                    mftuUrl: "",
                    topRatedUrl: ""
                }
            },
            mainRoll: e,
            defaultRollObject: e,
            menu: {
                url: "",
                deferredLoad: !0,
                related: !0,
                topRated: !0,
                mostViewed: !0,
                mftu: !0,
                showOnPause: !1,
                showOnPost: !1,
                linkColor: null
            },
            features: {
                autoplay: !0,
                ccMenu: !0,
                chromecast: !0,
                cinema: !1,
                fullscreen: !0,
                iosFullscreen: !0,
                iosAutoFullscreen: !1,
                fakeFullscreen: !1,
                hotspots: !0,
                logo: !1,
                mute: !0,
                nextVideo: !0,
                options: !0,
                share: !1,
                speed: !0,
                tooltips: !0,
                topBar: !1,
                volume: !0,
                grid: !0,
                alternativeOptionsMenu: !0,
                embedCode: "",
                themeColor: "#f6921e",
                ignorePreferences: !1,
                hideControlsTimeout: 2,
                qualitiesPerSlide: 4,
                doubleTapSeek: !0,
                removeProgressive: !0,
                qualityAutoWrap: !0,
                minIOSforPreroll: 12,
                basicPrerolls: !1
            },
            flashSettings: {
                appId: 0,
                allowfullscreen: !0,
                wmode: "opaque",
                quality: "high",
                allowScriptAccess: "always",
                bgcolor: "#000",
                paths: {
                    oldFlash: {
                        SWF: ""
                    },
                    fourPlay: {
                        SWF: "",
                        skins: []
                    }
                },
                cdnProvider: "",
                htmlPauseRoll: null,
                htmlPostRoll: null,
                postRollUrl: "",
                pauseRollUrl: "",
                extraFlashvars: {
                    oldFlash: {},
                    fourPlay: {}
                }
            },
            closedCaptionsStyle: {
                size: "1.0",
                bg: "0.75",
                color: "white",
                bg_color: "black"
            },
            events: {}
        },
        s = {},
        l = window,
        t = !1,
        u = {
            eventType: ["onPlay", "onPause", "onEnd", "showPreRoll", "hidePreRoll", "hidePreRollUI", "showPostRoll", "showPauseRoll", "onPreRollTimeout", "onPreRollNoAdsFound", "onFrequencyCap", "hideRoll", "onVastParserError", "onShare", "onBtnPlay", "onBtnBigPlay", "onBtnReplay", "onBtnPause", "expandPlayer", "collapsePlayer", "horizontalPlayer", "verticalPlayer", "horizontalThumbnails", "verticalThumbnails", "onQualityChange", "onQualityUpsell", "onAdaptiveQualityChange", "onDestroy", "onVolumeChange", "onVolumeSlider", "onMute", "onUnmute", "onAutoplayChange", "onPlayPromise", "onPlaybackRateChange", "onHotspotsChange", "onTextTrackChange", "onWatchHd", "onTextTrackStyleChange", "onSeek", "onForwardRewindSeek", "onDurationChange", "onTimeChange", "onPlaylistCountdown", "onFullscreen", "chromecastAvailable", "onCasting", "onAirplay", "onKeyboard", "onWaiting", "onBuffer", "onVideoReady", "onReady", "onRedirect", "onRelatedGridClick", "copyUrlVideo", "copyUrlVideoTime", "copyEmbed", "onVrError", "lowBandwidth", "overlaysReady", "thumbnailsReady", "fragmentChanged", "onMediaError", "onNextVideo", "onHelp", "onLooped"],
            events: {},
            playerId: null,
            initDispatcher: function() {
                __.each(this.eventType, function(e) {
                    u.events[e] = []
                })
            },
            subscribe: function(e, t, n) {
                this.events[e] && this.events[e].push({
                    playerId: t,
                    callback: n
                })
            },
            unsubscribe: function(e, t, n) {
                for (var r = this.events[e].length; r--;) this.events[e][r].playerId == t && (this.events[e].splice(r, 1), n && n(e, t))
            },
            fireEvent: function(t, n, r) {
                var a = this;
                __.each(this.events[t], function(e) {
                    if (e.playerId == n) {
                        try {
                            e.callback(n, t, r)
                        } catch (e) {
                            console.log("FireEvent error handler ", e)
                        }
                        a.playerId = n
                    }
                })
            }
        };

    function c() {
        var e = MHP1138.choosenPlayer.playerName;
        return "HTML5PlayerMobile" === e && (e = "HTML5Player"), "mhp1138_playerPlugin_" + e
    }

    function f() {
        __.each(s, function(e) {
            n(e.playerId)
        }), s = null, __.purge(document.body);
        var e = c();
        l[e] = null, __.each(MHP1138.player, function(e, t) {
            MHP1138.player[t] = null
        }), __.each(MHP1138.loader, function(e, t) {
            MHP1138.loader[t] = null
        }), o = null, __.each(MHP1138, function(e, t) {
            MHP1138[t] = null
        }), MHP1138 = null
    }

    function p(e, t) {
        var n, r, a = __.deepExtend({}, o, t),
            i = c();
        "basicHTML5" == MHP1138.choosenPlayer.playerName && (a.mainRoll.mediaDefinition = __.removeAdaptiveMedia(a.mainRoll.mediaDefinition)), s[e] = new l[i], n = e, r = a, __.each(u.eventType, function(e) {
            __.isFunction(r.events[e]) && u.subscribe(e, n, r.events[e])
        }), s[e].initPlayer(e, a)
    }

    function n(n, e) {
        __.isObject(s[n]) && (__.each(u.eventType, function(e) {
            u.unsubscribe(e, n)
        }), u.playerId == n && (u.playerId = null), s[n].destroy(function() {
            t && __.each(s[n], function(e, t) {
                s[n][t] = null
            }), delete s[n], e && e()
        }))
    }
    return {
        initPlayer: function(e, t) {
            o = __.deepExtend(o, e), u.initDispatcher();
            var n = "",
                r = "stable/";
            switch (t) {
                case "basicHTML5":
                    n = "basicHtml5/";
                    break;
                case "basicHTML5Phub":
                    n = "basicHtml5Phub/";
                    break;
                case "HTML5Player":
                case "HTML5PlayerTablet":
                case "HTML5PlayerMobile":
                    n = "html5/";
                    break;
                case "4Play":
                    n = "4play/";
                    break;
                case "oldFlash":
                    n = "oldFlash/";
                    break;
                case "imagePlayer":
                    n = "imagePlayer/"
            }
            "stable" !== MHP1138.choosenPlayer.version && (r = MHP1138.choosenPlayer.version + "/");
            var a = MHP1138.loader.pathToBuildFolder() + n + r + "mhp1138.player." + MHP1138.choosenPlayer.playerName + (MHP1138.minified ? ".min" : "") + ".js";
            MHP1138.loader.scriptLoader(a, "js", function() {
                MHP1138.playerReady = !0, __.size(MHP1138.players) && __.each(MHP1138.players, function(e, t) {
                    p(t, e)
                })
            });
            var i = MHP1138.detector.getBrowser();
            "ie" == i.name && i.version <= 9 && (l.addEventListener ? l.addEventListener("beforeunload", f) : l.attachEvent("onunload", f))
        },
        createPlayer: p,
        eventsDispatcher: u,
        playerType: MHP1138.choosenPlayer.playerName,
        fireEvent: function(e, t, n) {
            u.fireEvent(e, t, n)
        },
        subscribeToEvent: function(e, t, n) {
            u.subscribe(e, t, n)
        },
        unsubscribeToEvent: function(e, t, n) {
            u.unsubscribe(e, t, n)
        },
        getConfigsObject: function(e) {
            return !!__.isObject(s[e]) && s[e].createConfigJson()
        },
        sendFlashEventToPlayer: function(e, t) {
            __.isObject(s[e]) && s[e].catchAndFilterEvent(t)
        },
        destroyPlayer: function(e, t) {
            n(e, t)
        },
        seek: function(e, t, n) {
            __.isObject(s[e]) && s[e].seek(t, n)
        },
        isPlaying: function(e) {
            return !!__.isObject(s[e]) && s[e].isPlaying()
        },
        isReady: function(e) {
            return !!__.isObject(s[e]) && s[e].isReady()
        },
        play: function(e) {
            __.isObject(s[e]) && s[e].play()
        },
        pause: function(e) {
            __.isObject(s[e]) && s[e].pause()
        },
        setMute: function(e, t) {
            __.isObject(s[e]) && s[e].setMute(t)
        },
        getVolume: function(e) {
            return !!__.isObject(s[e]) && s[e].getVolume()
        },
        setVolume: function(e, t) {
            __.isObject(s[e]) && s[e].setVolume(t)
        },
        getCurrentTime: function(e) {
            return !!__.isObject(s[e]) && s[e].getCurrentTime(e)
        },
        exitFullscreen: function(e) {
            __.isObject(s[e]) && s[e].fullscreen && s[e].fullscreen.exit()
        },
        setPoster: function(e, t) {
            __.isObject(s[e]) && s[e].setPoster(t)
        },
        setActionTags: function(e, t) {
            __.isObject(s[e]) && s[e].setActionTags(t)
        },
        setQuality: function(e, t) {
            __.isObject(s[e]) && s[e].setQuality(t)
        },
        setThumbs: function(e, t) {
            __.isObject(s[e]) && s[e].setThumbs(t)
        },
        inspect: function() {
            var n = {};
            return __.each(s, function(e, t) {
                n[t] = {
                    settings: "oldFlash" == MHP1138.choosenPlayer.playerName || "4Play" == MHP1138.choosenPlayer.playerName ? MHP1138.player.getConfigsObject(t) : e.settings
                }
            }), n
        },
        getPlayerIds: function() {
            var n = [];
            return __.each(s, function(e, t) {
                n.push(t)
            }), n
        },
        getPlayers: function() {
            return s
        },
        playerStatus: function() {
            var n = {};
            return __.each(s, function(e, t) {
                void 0 !== e.getCurrentState ? n[e.playerId] = e.getCurrentState() : n[e.playerId] = {
                    currentState: "",
                    flags: {}
                }
            }), n
        }
    }
}(), __.mixin({
    deepExtend: function(n) {
        var r = /#{\s*?_\s*?}/,
            e = Array.prototype.slice;
        return __.each(e.call(arguments, 1), function(e) {
            for (var t in e)
                if (__.isUndefined(n[t]) || __.isFunction(n[t]) || __.isNull(e[t]) || __.isDate(e[t])) n[t] = e[t];
                else if (__.isString(e[t]) && r.test(e[t])) __.isString(n[t]) && (n[t] = e[t].replace(r, n[t]));
            else if (__.isArray(n[t]) || __.isArray(e[t])) {
                if (!__.isArray(n[t]) || !__.isArray(e[t])) throw new Error("Trying to combine an array with a non-array (" + t + ")");
                n[t] = __.reject(__.deepExtend(__.clone(n[t]), e[t]), function(e) {
                    return __.isNull(e)
                })
            } else if (__.isObject(n[t]) || __.isObject(e[t])) {
                if (!__.isObject(n[t]) || !__.isObject(e[t])) throw new Error("Trying to combine an object with a non-object (" + t + ")");
                n[t] = __.deepExtend(__.clone(n[t]), e[t])
            } else n[t] = e[t]
        }), n
    }
}), __.mixin({
    videoFormatIsSupported: function(n) {
        var e = document.createElement("video"),
            t = !1,
            r = __.find({
                ogg: 'video/ogg; codecs="theora"',
                h264: 'video/mp4; codecs="avc1.42E01E"',
                webm: 'video/webm; codecs="vp8, vorbis"',
                vp9: 'video/webm; codecs="vp9"',
                hls: 'application/x-mpegURL; codecs="avc1.42E01E"'
            }, function(e, t) {
                return t == n
            });
        if (r) try {
            (t = !!e.canPlayType) && (t = e.canPlayType(r).replace(/^no$/, ""))
        } catch (e) {}
        return !!t
    }
}), __.mixin({
    isPath: function(e) {
        return /.*(\/.*|\..*)+/.test(e)
    }
}), __.mixin({
    isURL: function(e) {
        return regexp = /^(?:(?:https?|ftp):\/\/)?(?:(?!(?:10|127)(?:\.\d{1,3}){3})(?!(?:169\.254|192\.168)(?:\.\d{1,3}){2})(?!172\.(?:1[6-9]|2\d|3[0-1])(?:\.\d{1,3}){2})(?:[1-9]\d?|1\d\d|2[01]\d|22[0-3])(?:\.(?:1?\d{1,2}|2[0-4]\d|25[0-5])){2}(?:\.(?:[1-9]\d?|1\d\d|2[0-4]\d|25[0-4]))|(?:(?:[a-z\u00a1-\uffff0-9]-*)*[a-z\u00a1-\uffff0-9]+)(?:\.(?:[a-z\u00a1-\uffff0-9]-*)*[a-z\u00a1-\uffff0-9]+)*(?:\.(?:[a-z\u00a1-\uffff]{2,})))(?::\d{2,5})?(?:\/\S*)?$/, !!regexp.test(e)
    }
});
var mhp_prefix = "mhp1138_";

function androidVersion() {
    var e = navigator.userAgent;
    return parseFloat(e.match(/Android\s+([\d\.]+)/)[1])
}

function checkWebKit() {
    var e = /AppleWebKit\/([\d.]+)/.exec(navigator.userAgent);
    return e ? parseFloat(e[1]) : null
}

function getChromeVersion() {
    var e = navigator.userAgent.match(/Chrom(e|ium)\/([0-9]+)\./);
    return !!e && parseInt(e[2], 10)
}
__.mixin(__.isUndefined(document.documentElement.classList) ? {
    addClass: function(e, t) {
        -1 == t.indexOf(mhp_prefix) && (t = mhp_prefix + t), __.hasClass(e, t) || (e.className += (e.className ? " " : "") + t)
    },
    removeClass: function(e, t) {
        -1 == t.indexOf(mhp_prefix) && (t = mhp_prefix + t), __.hasClass(e, t) && (e.className = e.className.replace(new RegExp("(^|\\s)*" + t + "(\\s|$)*", "g"), ""))
    },
    hasClass: function(e, t) {
        return -1 == t.indexOf(mhp_prefix) && (t = mhp_prefix + t), new RegExp("(^|\\s)" + t + "(\\s|$)").test(e.className)
    },
    toggleClass: function(e, t) {
        -1 == t.indexOf(mhp_prefix) && (t = mhp_prefix + t), (__.hasClass(e, t) ? __.removeClass : __.addClass)(e, t)
    }
} : {
    addClass: function(e, t) {
        e && e.classList ? (-1 == t.indexOf(mhp_prefix) && (t = mhp_prefix + t), e.classList.add(t)) : console.log("addClass: broken CSS selector", e, t)
    },
    removeClass: function(e, t) {
        e && e.classList ? (-1 == t.indexOf(mhp_prefix) && (t = mhp_prefix + t), e.classList.remove(t)) : console.log("removeClass: broken CSS selector", e, t)
    },
    hasClass: function(e, t) {
        if (e && e.classList) return -1 == t.indexOf(mhp_prefix) && (t = mhp_prefix + t), e.classList.contains(t);
        console.log("hasClass: broken CSS selector", e, t)
    },
    toggleClass: function(e, t) {
        e && e.classList ? (-1 == t.indexOf(mhp_prefix) && (t = mhp_prefix + t), e.classList.toggle(t)) : console.log("toggleClass: broken CSS selector", e, t)
    }
}), __.mixin({
    filterVideoResolution: function(e) {
        var t = [];
        return __.each(e, function(e) {
            __.isString(e.videoUrl) && e.videoUrl.length && __.isPath(e.videoUrl) && (!e.format || e.format && "hls" !== e.format && "dash" !== e.format) && t.push(e)
        }), t
    },
    removeAdaptiveMedia: function(e, t) {
        return t = t || ["hls", "dash"], __.isString(t) && (t = [t]), __.filter(e, function(e) {
            return !(0 <= __.indexOf(t, e.format))
        })
    },
    filterMediaSources: function(e) {
        var n = [],
            r = {},
            a = "";
        e = __.sortBy(e, function(e) {
            return "dash" == e.format ? 9999 : "hls" == e.format ? 9998 : -parseFloat(String(e.quality).replace(/[_p]/gi, ".")) - ("upsell" == e.format ? 1e4 : 0)
        }), e = __.filter(e, function(e) {
            return __.isPath(e.videoUrl) || "upsell" == e.format
        }), __.each(e, function(e) {
            var t = e.quality;
            n.push(t), "upsell" != (r[t] = e).format && e.defaultQuality && (a = t)
        }), a || (a = __.max(r, function(e) {
            return "dash" == e.format || "hls" == e.format ? 1 : parseFloat(String(e.quality).replace(/[_p]/gi, ".") * ("upsell" == e.format ? -1 : 1))
        }).quality);
        for (var t = 0; t < e.length; t++) e[t].quality == a && (e[t].defaultQuality = !0);
        return e
    },
    selectedQualitySource: function(e) {
        for (var t = {}, n = 0; n < e.length; n++) !0 === e[n].defaultQuality && (t = e[n]);
        return t
    }
}), __.mixin({
    purge: function(e) {
        var t, n, r, a = e.attributes;
        if (a)
            for (t = a.length - 1; 0 <= t; t -= 1) "function" == typeof e[r = a[t].name] && (e[r] = null);
        if (a = e.childNodes)
            for (n = a.length, t = 0; t < n; t += 1) __.purge(e.childNodes[t])
    }
}), __.mixin({
    capitalize: function(e, t) {
        e = "" + e;
        var n = t ? e.slice(1).toLowerCase() : e.slice(1);
        return e.charAt(0).toUpperCase() + n
    }
}), MHP1138.detector = function() {
    var n = ["desktop", "mobile", "tablet", "tv"],
        o = null,
        s = null,
        t = {},
        l = {},
        u = {
            init: function(e) {
                t = e, ~__.indexOf(n, t) && (o = e, s = navigator.userAgent);
                var t;
                __.each(l[o], function(e, t) {
                    u["is" + __.capitalize(t)] = function() {
                        return !!s.match(l[o][t])
                    }
                }), __.each(l.os, function(e, t) {
                    u["is" + __.capitalize(t)] = function() {
                        return !!s.match(l.os[t])
                    }
                }), __.each(n, function(e) {
                    u["is" + __.capitalize(e)] = function() {
                        return e == o
                    }
                })
            },
            getBrowser: r,
            generateClassList: function(e) {
                ! function(e) {
                    var t = r();
                    __.isUndefined(t.name) || __.addClass(e, t.name);
                    __.isUndefined(t.type) || __.addClass(e, t.type);
                    __.isUndefined(t.version) || __.addClass(e, t.name + t.version);
                    __.isUndefined(t.os) || __.addClass(e, t.os)
                }(e)
            }
        };

    function r() {
        if (__.isEmpty(t)) {
            var i = [],
                n = {
                    name: "undefined",
                    version: ""
                };
            __.each(l[o], function(e, t) {
                if (!i[0] && u["is" + __.capitalize(t)]()) {
                    var n = s.match(l[o][t]);
                    if (i = [t, n[1] || ""], "safari" == t) {
                        var r = s.match(/(?:Version\/|Safari\/|Safari )(\d+\.\d+\.*\d*)/gi),
                            a = r && r[0] ? r[0].split("/")[1] : "";
                        i = [t, a]
                    }
                }
            }), __.each(l.os, function(e, t) {
                "undefined" == n.name && u["is" + __.capitalize(t)]() && (n = {
                    name: t,
                    version: s.match(e)[2] || ""
                })
            }), t = {
                name: i[0],
                type: o,
                version: (e = i[1], __.isUndefined(e) ? e : e.replace("_", ".").split(".")[0]),
                fullVersion: i[1],
                os: n.name,
                osVersion: n.version
            }
        }
        var e;
        return t
    }
    return l.desktop = {
        ie: /(?:WOW64.+rv:|MSIE\s)(\d+\.*\d*)(?!.*Firefox)/,
        edge: /(?:Edge\/)(\d+\.\d+\.*\d*)/,
        chrome: /(?:Chrome\/)(\d+\.\d+\.\d+\.*\d+)(?!.*OPR)/,
        safari: /(?:AppleWebKit\/|Version\/|Safari\/|Safari )(\d+\.\d+\.*\d*)/,
        firefox: /(?:[Ff]irefox(?:\/| |\(|)(\d*\.\d\.*\d*\.*\d*)*)/,
        opera: /(?:OPR\/|Opera(?:\s|\/))(\d+\.\d+\.*\d*\.*\d*)/
    }, l.mobile = {
        edge: l.desktop.edge,
        ie: /(?:(?:IEM|iem)obile|MSIE|Edge)(?:\/|\s)(\d+\.*\d*)/,
        chrome: /(?:CriOS\/|Chrome\/)(\d+\.\d+\.\d+\.*\d+)(?!.*OPR)/,
        safari: l.desktop.safari,
        opera: l.desktop.opera,
        firefox: /(?:Firefox|Fennec|FxiOS)(?:\/)((\d*\.\d\.*\d*\.*\d*))/,
        uc: /(?:UCBrowser\/|UCWEB\/)(\d+\.\d+\.*\d*\.*\d*)/
    }, l.tablet = {
        firefox: l.mobile.firefox,
        ie: !1,
        edge: l.desktop.edge,
        chrome: l.mobile.chrome,
        safari: l.desktop.safari,
        opera: l.desktop.opera
    }, l.os = {
        windowsPhone: /(Windows Phone) O?S?\s?(\d+\.?\d?)/,
        ios: /(iPhone|iPad|iPod)+.+OS (\d+)/,
        macos: /Mac OS X/i,
        windows: /Windows/i,
        android: /(Android);? (\d+\.?\d?)?/i,
        playstation: /playstation/i,
        xbox: /xbox/i,
        smartTV: /TV/i,
        samsungNative: /(SAMSUNG|SM)/
    }, u
}();
//# sourceMappingURL=mhp1138.loader.min.js.map