(function e(t, n, r) {
    function s(o, u) {
        if (!n[o]) {
            if (!t[o]) {
                var a = typeof require == "function" && require;
                if (!u && a) return a(o, !0);
                if (i) return i(o, !0);
                var f = new Error("Cannot find module '" + o + "'");
                throw f.code = "MODULE_NOT_FOUND", f
            }
            var l = n[o] = {
                exports: {}
            };
            t[o][0].call(l.exports, function(e) {
                var n = t[o][1][e];
                return s(n ? n : e)
            }, l, l.exports, e, t, n, r)
        }
        return n[o].exports
    }
    var i = typeof require == "function" && require;
    for (var o = 0; o < r.length; o++) s(r[o]);
    return s
})({
    1: [function(require, module, exports) {
        module.exports = {
            assetsRoot: "https://js-assets.aiv-cdn.net/playback/web_player",
            dependencies: {
                ATVWebPlayer: {
                    name: "ATVWebPlayer",
                    version: "1.0.201022.0",
                    crc32: "415969317",
                    forceUpdate: !1
                },
                ATVWebCascadesPlayer: {
                    name: "ATVWebCascadesPlayer",
                    version: "1.2.200957.0",
                    crc32: "-1577748719",
                    forceUpdate: !0
                },
                DVWebGlobalPlayer: {
                    name: "DVWebGlobalPlayer",
                    version: "1.0.193.0",
                    crc32: !1,
                    forceUpdate: !1
                },
                ATVWebDiagnosticsRenderer: {
                    name: "ATVWebDiagnosticsRenderer",
                    version: "1.0.200259.0",
                    crc32: !1,
                    forceUpdate: !1
                }
            }
        };

    }, {}],
    2: [function(require, module, exports) {
        "use strict";
        var Features;
        ! function(e) {
            e[e.volume = 0] = "volume"
        }(Features || (Features = {})), module.exports = Features;

    }, {}],
    3: [function(require, module, exports) {
        "use strict";
        var TinyMetricsReporter = function() {
            function e(e) {
                this.REGION_LIST = [{
                    tld: "amazon.com",
                    fls: "na",
                    marketplace: "us",
                    marketplaceId: "ATVPDKIKX0DER"
                }, {
                    tld: "amazon.de",
                    fls: "eu",
                    marketplace: "de",
                    marketplaceId: "A1PA6795UKMFR9"
                }, {
                    tld: "amazon.co.uk",
                    fls: "eu",
                    marketplace: "uk",
                    marketplaceId: "A1F83G8C2ARO7P"
                }, {
                    tld: "amazon.co.jp",
                    fls: "fe",
                    marketplace: "jp",
                    marketplaceId: "A1VC38T7YXB528"
                }], this.region = this.REGION_LIST[0];
                for (var t = 0, r = this.REGION_LIST; t < r.length; t++) {
                    var o = r[t];
                    if ((e ? e : location.hostname).indexOf(o.tld) > -1) {
                        this.region = o;
                        break
                    }
                }
            }
            return e.prototype.reportAppLoad = function(e) {
                this.report("AppLoadTime", e + ":u=ms")
            }, e.prototype.reportAppLoadError = function() {
                this.report("AppLoadError", "1")
            }, e.prototype.reportAppLoadCacheHit = function() {
                this.report("AppLoadCacheHit", "1")
            }, e.prototype.reportAppLoadCacheMiss = function() {
                this.report("AppLoadCacheMiss", "1")
            }, e.prototype.report = function(e, t) {
                (new Image).src = "https://fls-" + this.region.fls + ".amazon.com/1/action-impressions/1/OP/aiv-web-player/action/HTML5Player:." + e + "@v=" + t + "?marketplace=" + this.region.marketplace + "&marketplaceId=" + this.region.marketplaceId + "&requestId=1&service=AIVWebPlayer&session=" + ("" + Math.random()).substr(2)
            }, e
        }();
        module.exports = TinyMetricsReporter;

    }, {}],
    4: [function(require, module, exports) {
        "use strict";
        var ATVWebResourceLoading = require(14),
            JavaScriptLoader = ATVWebResourceLoading.JavaScriptLoader,
            ScriptType = ATVWebResourceLoading.ScriptType,
            Features = require(2),
            TinyMetricsReporter = require(3),
            AppLoaderConfig = require(1),
            WebPlayerLoader = function() {
                function e(e, t, i, r) {
                    void 0 === e && (e = AppLoaderConfig), void 0 === t && (t = new JavaScriptLoader), void 0 === i && (i = new TinyMetricsReporter), void 0 === r && (r = location.search);
                    var o = this;
                    if (this.Features = Features, this.onList = [], this.onceList = [], this.getFeatureList = [], r)
                        for (var n = r.substr(1).split("&"), a = 0; a < n.length; a++) {
                            var s = n[a].split("=");
                            if (2 === s.length) {
                                var p = s[0];
                                if (e.dependencies[p]) {
                                    var c = decodeURIComponent(s[1]).split(";");
                                    if (2 !== c.length) throw new Error("crc-32 not supplied for " + p);
                                    e.dependencies[p].version = c[0], e.dependencies[p].crc32 = c[1]
                                }
                            }
                        }
                    var h = e.dependencies.ATVWebPlayer,
                        l = {
                            crc32: h.crc32,
                            url: h.name=="ATVWebPlayer"?"qrc:///scripts/amazon/ATVWebPlayer.js": e.assetsRoot + "/" + h.name + "/" + h.version + "/js/" + h.name + ".js",
                            name: h.name,
                            type: ScriptType.JAVASCRIPT,
                            version: h.version
                        };
                        console.log("DEBUG 300: " + h.name);
                    t.get(l, function(t) {
                        t.cached ? i.reportAppLoadCacheHit() : i.reportAppLoadCacheMiss(), o.webPlayer = new window.ATVWebPlayer(e), o.tryWebPlayer(), i.reportAppLoad(t.totalMs)
                    }, function(e) {}, function(e) {
                        if (window.ueLogError) try {
                            window.ueLogError(e, {
                                attribution: "web_player_loader"
                            })
                        } catch (e) {}
                        i.reportAppLoadError()
                    })
                }
                return e.prototype.initialize = function(e) {
                    this.tryConfig(e)
                }, e.prototype.prepare = function(e, t) {
                    this.tryConfig(e, t)
                }, e.prototype.play = function(e, t, i) {
                    this.tryConfig(e, t, i)
                }, e.prototype.on = function(e, t) {
                    this.webPlayer ? this.webPlayer.on(e, t) : this.appendListener(this.onList, e, t)
                }, e.prototype.once = function(e, t) {
                    this.webPlayer ? this.webPlayer.once(e, t) : this.appendListener(this.onceList, e, t)
                }, e.prototype.off = function(e, t) {
                    this.webPlayer ? this.webPlayer.off(e, t) : (this.removeListener(this.onList, e, t), this.removeListener(this.onceList, e, t))
                }, e.prototype.getFeature = function(e, t, i) {
                    this.webPlayer ? this.webPlayer.getFeature(e, t, i) : this.getFeatureList.push({
                        feature: e,
                        onAvailable: t,
                        onError: i
                    })
                }, e.prototype.appendListener = function(e, t, i) {
                    e.push({
                        event: t,
                        callback: i
                    })
                }, e.prototype.removeListener = function(e, t, i) {
                    for (var r = 0; r < e.length; r++) e[r].event === t && e[r].callback === i && (e[r] = null)
                }, e.prototype.tryConfig = function(e, t, i) {
                    void 0 === e && (e = null), void 0 === t && (t = null), void 0 === i && (i = null), this.playerConfig = e, this.contentConfig = t, this.playbackConfig = i, this.tryWebPlayer()
                }, e.prototype.attachEvents = function(e, t) {
                    for (var i = 0; i < e.length; i++) {
                        var r = e[i];
                        r && t(r.event, r.callback)
                    }
                }, e.prototype.attachGetFeatures = function() {
                    for (var e = 0, t = this.getFeatureList; e < t.length; e++) {
                        var i = t[e];
                        this.webPlayer.getFeature(i.feature, i.onAvailable, i.onError)
                    }
                    this.getFeatureList = []
                }, e.prototype.tryWebPlayer = function() {
                    this.webPlayer && (this.attachEvents(this.onList, this.webPlayer.on), this.attachEvents(this.onceList, this.webPlayer.once), this.attachGetFeatures(), this.onList = [], this.onceList = [], this.playbackConfig ? this.webPlayer.play(this.playerConfig, this.contentConfig, this.playbackConfig) : this.contentConfig ? this.webPlayer.prepare(this.playerConfig, this.contentConfig) : this.playerConfig && this.webPlayer.initialize(this.playerConfig))
                }, e
            }();
        module.exports = WebPlayerLoader;

    }, {
        "1": 1,
        "14": 14,
        "2": 2,
        "3": 3
    }],
    5: [function(require, module, exports) {
        "use strict";
        var Loader = require(4);
        module.exports = window.WebPlayer = Loader;
    }, {
        "4": 4
    }],
    6: [function(require, module, exports) {
        "use strict";
        var IndexedDBStorage = function() {
            function t(e, n) {
                void 0 === n && (n = window);
                var a = this;
                this.VERSION = 1, this.pendingDbOperations = [], this.tryInitialize = function(e) {
                    a.database || !t.isSupported(a.windowRef) ? e() : (a.pendingDbOperations.push(e), a.dbOpenRequest || a.initialize(function() {
                        for (var t = 0; t < a.pendingDbOperations.length; t++) {
                            var e = a.pendingDbOperations[t];
                            "function" == typeof e && a.pendingDbOperations[t]()
                        }
                        a.dbOpenRequest = null, a.pendingDbOperations = []
                    }))
                }, this.initialize = function(t) {
                    try {
                        var e = a.windowRef.indexedDB.open(a.dbName, a.VERSION);
                        e.onsuccess = function(e) {
                            try {
                                a.database = e.target.result
                            } catch (t) {}
                            t()
                        }, e.onupgradeneeded = function(t) {
                            t.oldVersion < 1 && (a.database = t.target.result, a.database.createObjectStore(a.dbName))
                        }, e.onerror = function(e) {
                            e.preventDefault(), t()
                        }
                    } catch (e) {
                        t()
                    }
                }, this.dbName = e, this.windowRef = n
            }
            return t.prototype.get = function(t, e) {
                var n = this;
                this.tryInitialize(function() {
                    try {
                        if (!n.database) return e(null);
                        var a = (new Date).getTime(),
                            i = n.database.transaction([n.dbName], "readonly").objectStore(n.dbName).get(t);
                        i.onsuccess = function(t) {
                            try {
                                var n = t.target.result;
                                e(n, (new Date).getTime() - a)
                            } catch (t) {
                                e(null)
                            }
                        }, i.onerror = function() {
                            e(null)
                        }
                    } catch (t) {
                        e(null)
                    }
                })
            }, t.prototype.set = function(t, e) {
                var n = this;
                this.tryInitialize(function() {
                    try {
                        n.database && n.database.transaction([n.dbName], "readwrite").objectStore(n.dbName).put(e, t)
                    } catch (t) {}
                })
            }, t.prototype.clear = function() {
                var t = this;
                this.tryInitialize(function() {
                    try {
                        t.database && t.database.transaction([t.dbName], "readwrite").objectStore(t.dbName).clear()
                    } catch (t) {}
                })
            }, t.isSupported = function(t) {
                return void 0 === t && (t = window), "undefined" != typeof t.indexedDB
            }, t
        }();
        module.exports = IndexedDBStorage;

    }, {}],
    7: [function(require, module, exports) {
        "use strict";
        var IndexedDBStorage = require(6),
            index = {
                IndexedDBStorage: IndexedDBStorage
            };
        module.exports = index;

    }, {
        "6": 6
    }],
    8: [function(require, module, exports) {
        "use strict";
        var ATVWebIndexedDBStorage = require(7),
            IndexedDBStorage = ATVWebIndexedDBStorage.IndexedDBStorage,
            JavaScriptLoader = require(9),
            ScriptType = require(11),
            SimpleURLAcquirer = require(12),
            BaseCachingScriptLoader = function() {
                function r(r, o, t, i) {
                    void 0 === r && (r = new JavaScriptLoader), void 0 === o && (o = new SimpleURLAcquirer), void 0 === t && (t = new IndexedDBStorage("ScriptStore"));
                    var n = this;
                    this.SCRIPT_TYPES_XHR_REQUIRED = [ScriptType.JSON], this.XHR_TIMEOUT_MS = 12e4, this.XHR_RETRIES = 2, this.loadJsonFromString = function(r, o, t) {
                        try {
                            var i = (new Date).getTime();
                            t.payload = JSON.parse(o), t.parseMs = (new Date).getTime() - i, t.totalMs = n.calculateTotalLoadMs(t), r.onLoad(t)
                        } catch (o) {
                            n.onError(r, o)
                        }
                    }, this.loadJavaScriptFromString = function(r, o, t) {
                        try {
                            var i = new Blob([o], {
                                    type: "application/javascript"
                                }),
                                e = {
                                    url: n.createObjectURL(i),
                                    name: r.scriptInfo.name,
                                    type: r.scriptInfo.type,
                                    version: r.scriptInfo.version
                                };
                            n.javaScriptLoader.get(e, function(o) {
                                t.parseMs = o.downloadMs, t.totalMs = n.calculateTotalLoadMs(t), r.onLoad(t)
                            }, r.onNonFatal, function(o) {
                                return n.onError(r, o)
                            })
                        } catch (o) {
                            n.onError(r, o)
                        }
                    }, this.loadJavaScriptFromOrigin = function(r) {
                        n.javaScriptLoader.get(r.scriptInfo, r.onLoad, r.onNonFatal, r.onFatal, !0)
                    }, this.loadJsonFromOrigin = function(r) {
                        n.urlAcquirer.get(r.scriptInfo.url, r.scriptInfo.crc32, n.XHR_TIMEOUT_MS, n.XHR_RETRIES, function(o, t) {
                            n.loadFromString(r, o, t)
                        }, function(o) {
                            r.onNonFatal(o)
                        }, function(o) {
                            r.onFatal(o)
                        })
                    }, this.javaScriptLoader = r, this.urlAcquirer = o, this.offlineStorage = t, this.loadFromStringFunctions = {}, this.loadFromStringFunctions[ScriptType.JAVASCRIPT] = this.loadJavaScriptFromString, this.loadFromStringFunctions[ScriptType.JSON] = this.loadJsonFromString, this.loadFromOriginFunctions = {}, this.loadFromOriginFunctions[ScriptType.JAVASCRIPT] = this.loadJavaScriptFromOrigin, this.loadFromOriginFunctions[ScriptType.JSON] = this.loadJsonFromOrigin, !this.isDefined(i) && this.isDefined(window.URL) ? this.createObjectURL = URL.createObjectURL : this.createObjectURL = i
                }
                return r.prototype.getContent = function(r, o) {
                    var t = this.requiresUrlAcquirer(r.scriptInfo.type);
                    t && !this.urlAcquirer.isSupported() ? r.onFatal(new Error("URLAcquirer not supported but " + ScriptType[r.scriptInfo.type] + " requires it.")) : !t && this.onlySupportsOrigin() ? this.loadFromOrigin(r) : this.offlineStorage.get(r.scriptInfo.name, o)
                }, r.prototype.loadFromString = function(r, o, t) {
                    var i = this.loadFromStringFunctions[r.scriptInfo.type];
                    return this.isDefined(i) ? void i(r, o, t) : void r.onFatal(new Error("No loadFromString function found for " + ScriptType[r.scriptInfo.type]))
                }, r.prototype.loadFromOrigin = function(r) {
                    var o = this.loadFromOriginFunctions[r.scriptInfo.type];
                    return this.isDefined(o) ? void o(r) : void r.onFatal(new Error("No loadFromOrigin function found for " + ScriptType[r.scriptInfo.type]))
                }, r.prototype.acquireNewAsset = function(r, o, t) {
                    var i = this;
                    void 0 === t && (t = !0), this.urlAcquirer.get(r.scriptInfo.url, r.scriptInfo.crc32, this.XHR_TIMEOUT_MS, this.XHR_RETRIES, function(n, e) {
                        t && i.offlineStorage.set(r.scriptInfo.name, {
                            version: r.scriptInfo.version,
                            data: n
                        }), o && i.loadFromString(r, n, e)
                    }, function(o, t) {
                        r.onNonFatal(o, t)
                    }, function(t, n) {
                        o && i.onError(r, t, n)
                    })
                }, r.prototype.onError = function(r, o, t) {
                    r.onNonFatal(o, t), this.loadFromOrigin(r)
                }, r.prototype.requiresUrlAcquirer = function(r) {
                    return this.SCRIPT_TYPES_XHR_REQUIRED.indexOf(r) !== -1
                }, r.prototype.onlySupportsOrigin = function() {
                    return !(this.isDefined(window.Blob) && this.urlAcquirer.isSupported())
                }, r.prototype.isDefined = function(r) {
                    return "undefined" != typeof r
                }, r.prototype.nonNumberToZero = function(r) {
                    return "number" == typeof r ? r : 0
                }, r.prototype.calculateTotalLoadMs = function(r) {
                    return this.nonNumberToZero(r.cacheLoadMs) + this.nonNumberToZero(r.latencyMs) + this.nonNumberToZero(r.downloadMs) + this.nonNumberToZero(r.verifyMs) + this.nonNumberToZero(r.parseMs)
                }, r
            }();
        module.exports = BaseCachingScriptLoader;

    }, {
        "11": 11,
        "12": 12,
        "7": 7,
        "9": 9
    }],
    9: [function(require, module, exports) {
        "use strict";
        var JavaScriptLoader = function() {
            function t(t, e, r) {
                void 0 === t && (t = 12e4), void 0 === e && (e = 2), void 0 === r && (r = document);
                var o = this;
                this.scriptErrorCount = 0, this.scriptLoaded = !1, this.onScriptError = function(t, e, r) {
                    t && (t.onload = t.onerror = t.onreadystatechange = null), clearTimeout(o.scriptTimerId), o.objectExists(o.scriptInfo.name) || (++o.scriptErrorCount < o.maxAttempts ? (o.onNonFatal(e), o.setScript(r)) : o.onFatal(e))
                }, this.scriptTimeoutMs = t, this.maxAttempts = e, this.documentInstance = r
            }
            return t.prototype.get = function(t, e, r, o, i) {
                void 0 === i && (i = !1), this.scriptInfo = t, this.onLoad = e, this.onNonFatal = r, this.onFatal = o, this.scriptErrorCount = 0, this.scriptLoaded = !1, this.bustCache = i, clearTimeout(this.scriptTimerId), this.setScript(this.scriptInfo.url)
            }, t.prototype.setScript = function(t) {
                var e = this,
                    r = this.documentInstance.createElement("script");
                this.scriptTimerId = setTimeout(function() {
                    e.onScriptError(r, new Error("Script load timed out. " + e.scriptInfo.name + " for " + t), t)
                }, this.scriptTimeoutMs);
                var o = (new Date).getTime();
                r.setAttribute("crossorigin", "anonymous"), r.onload = r.onreadystatechange = function() {
                    if (!e.scriptLoaded && ("undefined" == typeof r.readyState || "loaded" === r.readyState || "complete" === r.readyState))
                        if (e.objectExists(e.scriptInfo.name)) {
                            clearTimeout(e.scriptTimerId), e.scriptLoaded = !0;
                            var i = (new Date).getTime() - o;
                            r.onload = r.onerror = r.onreadystatechange = null, e.onLoad({
                                cached: !1,
                                downloadMs: i,
                                totalMs: i
                            })
                        } else e.onScriptError(r, new Error("Script loaded but missing result object. Expected " + e.scriptInfo.name + " for " + t), t)
                }, r.onerror = function() {
                    e.onScriptError(r, new Error("Script onerror fired: " + e.scriptInfo.name + " for " + t), t)
                };
                var i = this.bustCache ? "?t=" + (new Date).getTime() : "";
                r.src = t + i, this.documentInstance.getElementsByTagName("head")[0].appendChild(r)
            }, t.prototype.objectExists = function(t) {
                return "undefined" != typeof window[t]
            }, t
        }();
        module.exports = JavaScriptLoader;

    }, {}],
    10: [function(require, module, exports) {
        "use strict";
        var __extends = this && this.__extends || function() {
                var t = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(t, e) {
                    t.__proto__ = e
                } || function(t, e) {
                    for (var o in e) e.hasOwnProperty(o) && (t[o] = e[o])
                };
                return function(e, o) {
                    function n() {
                        this.constructor = e
                    }
                    t(e, o), e.prototype = null === o ? Object.create(o) : (n.prototype = o.prototype, new n)
                }
            }(),
            BaseCachingScriptLoader = require(8),
            LazyCachingScriptLoader = function(t) {
                function e(e, o, n, r) {
                    return t.call(this, e, o, n, r) || this
                }
                return __extends(e, t), e.prototype.get = function(t, e, o, n) {
                    var r = this,
                        a = {
                            scriptInfo: t,
                            onLoad: e,
                            onNonFatal: o,
                            onFatal: n
                        };
                    this.getContent(a, function(e, o) {
                        e ? (r.loadFromString(a, e.data, {
                            cached: !0,
                            cacheLoadMs: o
                        }), e.version !== t.version && r.acquireNewAsset(a, !1)) : r.acquireNewAsset(a, !0)
                    })
                }, e
            }(BaseCachingScriptLoader);
        module.exports = LazyCachingScriptLoader;

    }, {
        "8": 8
    }],
    11: [function(require, module, exports) {
        "use strict";
        var ScriptType;
        ! function(p) {
            p[p.JAVASCRIPT = 0] = "JAVASCRIPT", p[p.JSON = 1] = "JSON"
        }(ScriptType || (ScriptType = {})), module.exports = ScriptType;

    }, {}],
    12: [function(require, module, exports) {
        "use strict";
        var crc32 = require(15),
            X_SERVER_IP = "X-Server-IP",
            SimpleURLAcquirer = function() {
                function e(e, t) {
                    void 0 === e && (e = XMLHttpRequest), void 0 === t && (t = crc32.str);
                    var n = this;
                    this.makeXHRRequest = function(e, t, r, o, a, i, s, d) {
                        void 0 === d && (d = 0);
                        var u = "",
                            c = new n.XHR,
                            l = function(l) {
                                l || (l = {
                                    type: "unknown error"
                                });
                                var f = new Error("XHR " + l.type + " for " + e + ". Status: " + c.status + ", readyState: " + c.readyState),
                                    p = {
                                        attempt: d,
                                        cloudfrontRequestID: n.getHeaderIfAvailable("x-amz-cf-id", u, c),
                                        contentLength: l.contentLength,
                                        contentLengthHeader: n.getHeaderIfAvailable("Content-Length", u, c),
                                        contentHash: l.contentHash,
                                        serverIP: n.getHeaderIfAvailable(X_SERVER_IP, u, c)
                                    };
                                o > 0 ? (i(f, p), n.makeXHRRequest(e, t, r, --o, a, i, s, d + 1)) : s(f, p)
                            },
                            f = n.now(),
                            p = f;
                        c.addEventListener("readystatechange", function() {
                            2 === c.readyState && (p = n.now())
                        }), c.addEventListener("load", function() {
                            if (u = c.getAllResponseHeaders(), 200 === c.status) {
                                var e = c.responseText,
                                    r = n.now(),
                                    o = !0,
                                    i = null,
                                    s = "(not checked)";
                                if (t) {
                                    var d = n.now();
                                    s = "" + n.toCRC32(e), o = s === t, i = n.now() - d
                                }
                                if (o) a(e, {
                                    cached: !1,
                                    latencyMs: p - f,
                                    downloadMs: r - p,
                                    verifyMs: i,
                                    serverIP: n.getHeaderIfAvailable(X_SERVER_IP, u, c)
                                });
                                else {
                                    var v = e ? e.length : 0,
                                        R = v > 0 ? "[" + e.substr(0, 15) + "]" : "(empty)";
                                    l({
                                        type: "verify failed " + R,
                                        contentLength: v,
                                        contentHash: s
                                    })
                                }
                            } else l({
                                type: "bad response"
                            })
                        }), c.addEventListener("error", l), c.addEventListener("timeout", l);
                        var v = 0 === d ? "" : "?t=" + n.now();
                        c.open("GET", e + v), c.timeout = r, c.send()
                    }, this.XHR = e, this.toCRC32 = t
                }
                return e.prototype.isSupported = function() {
                    return "withCredentials" in new this.XHR
                }, e.prototype.get = function(e, t, n, r, o, a, i) {
                    this.makeXHRRequest(e, t, n, r, o, a, i)
                }, e.prototype.now = function() {
                    return (new Date).getTime()
                }, e.prototype.getHeaderIfAvailable = function(e, t, n) {
                    return t.indexOf(e) !== -1 ? n.getResponseHeader(e) : null
                }, e
            }();
        module.exports = SimpleURLAcquirer;

    }, {
        "15": 15
    }],
    13: [function(require, module, exports) {
        "use strict";
        var __extends = this && this.__extends || function() {
                var t = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(t, r) {
                    t.__proto__ = r
                } || function(t, r) {
                    for (var n in r) r.hasOwnProperty(n) && (t[n] = r[n])
                };
                return function(r, n) {
                    function o() {
                        this.constructor = r
                    }
                    t(r, n), r.prototype = null === n ? Object.create(n) : (o.prototype = n.prototype, new o)
                }
            }(),
            BaseCachingScriptLoader = require(8),
            TransparentCachingScriptLoader = function(t) {
                function r(r, n, o, e) {
                    return t.call(this, r, n, o, e) || this
                }
                return __extends(r, t), r.prototype.get = function(t, r, n, o, e) {
                    var i = this;
                    void 0 === e && (e = !0);
                    var a = {
                        scriptInfo: t,
                        onLoad: r,
                        onNonFatal: n,
                        onFatal: o
                    };
                    e ? this.getContent(a, function(r, n) {
                        r && r.version === t.version ? i.loadFromString(a, r.data, {
                            cached: !0,
                            cacheLoadMs: n
                        }) : i.acquireNewAsset(a, !0)
                    }) : this.onlySupportsOrigin() ? this.loadFromOrigin(a) : this.acquireNewAsset(a, !0, !1)
                }, r
            }(BaseCachingScriptLoader);
        module.exports = TransparentCachingScriptLoader;

    }, {
        "8": 8
    }],
    14: [function(require, module, exports) {
        "use strict";
        var LazyCachingScriptLoader = require(10),
            TransparentCachingScriptLoader = require(13),
            JavaScriptLoader = require(9),
            ScriptType = require(11),
            exportedAPI = {
                LazyCachingScriptLoader: LazyCachingScriptLoader,
                TransparentCachingScriptLoader: TransparentCachingScriptLoader,
                JavaScriptLoader: JavaScriptLoader,
                ScriptType: ScriptType
            };
        module.exports = exportedAPI;

    }, {
        "10": 10,
        "11": 11,
        "13": 13,
        "9": 9
    }],
    15: [function(require, module, exports) {
        var CRC32;
        ! function(r) {
            "undefined" == typeof DO_NOT_EXPORT_CRC ? "object" == typeof exports ? r(exports) : "function" == typeof define && define.amd ? define(function() {
                var e = {};
                return r(e), e
            }) : r(CRC32 = {}) : r(CRC32 = {})
        }(function(r) {
            function e() {
                for (var r = 0, e = new Array(256), n = 0; 256 != n; ++n) r = n, r = 1 & r ? -306674912 ^ r >>> 1 : r >>> 1, r = 1 & r ? -306674912 ^ r >>> 1 : r >>> 1, r = 1 & r ? -306674912 ^ r >>> 1 : r >>> 1, r = 1 & r ? -306674912 ^ r >>> 1 : r >>> 1, r = 1 & r ? -306674912 ^ r >>> 1 : r >>> 1, r = 1 & r ? -306674912 ^ r >>> 1 : r >>> 1, r = 1 & r ? -306674912 ^ r >>> 1 : r >>> 1, r = 1 & r ? -306674912 ^ r >>> 1 : r >>> 1, e[n] = r;
                return "undefined" != typeof Int32Array ? new Int32Array(e) : e
            }

            function n(r) {
                if (r.length > 32768 && a) return f(new Buffer(r));
                for (var e = -1, n = r.length - 1, t = 0; t < n;) e = u[255 & (e ^ r.charCodeAt(t++))] ^ e >>> 8, e = u[255 & (e ^ r.charCodeAt(t++))] ^ e >>> 8;
                return t === n && (e = e >>> 8 ^ u[255 & (e ^ r.charCodeAt(t))]), e ^ -1
            }

            function t(r) {
                if (r.length > 1e4) return f(r);
                for (var e = -1, n = 0, t = r.length - 3; n < t;) e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])];
                for (; n < t + 3;) e = e >>> 8 ^ u[255 & (e ^ r[n++])];
                return e ^ -1
            }

            function f(r) {
                for (var e = -1, n = 0, t = r.length - 7; n < t;) e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])], e = e >>> 8 ^ u[255 & (e ^ r[n++])];
                for (; n < t + 7;) e = e >>> 8 ^ u[255 & (e ^ r[n++])];
                return e ^ -1
            }

            function o(r) {
                for (var e, n, t = -1, f = 0, o = r.length; f < o;) e = r.charCodeAt(f++), e < 128 ? t = t >>> 8 ^ u[255 & (t ^ e)] : e < 2048 ? (t = t >>> 8 ^ u[255 & (t ^ (192 | e >> 6 & 31))], t = t >>> 8 ^ u[255 & (t ^ (128 | 63 & e))]) : e >= 55296 && e < 57344 ? (e = (1023 & e) + 64, n = 1023 & r.charCodeAt(f++), t = t >>> 8 ^ u[255 & (t ^ (240 | e >> 8 & 7))], t = t >>> 8 ^ u[255 & (t ^ (128 | e >> 2 & 63))], t = t >>> 8 ^ u[255 & (t ^ (128 | n >> 6 & 15 | (3 & e) << 4))], t = t >>> 8 ^ u[255 & (t ^ (128 | 63 & n))]) : (t = t >>> 8 ^ u[255 & (t ^ (224 | e >> 12 & 15))], t = t >>> 8 ^ u[255 & (t ^ (128 | e >> 6 & 63))], t = t >>> 8 ^ u[255 & (t ^ (128 | 63 & e))]);
                return t ^ -1
            }
            r.version = "0.4.0";
            var u = e(),
                a = "undefined" != typeof Buffer;
            r.table = u, r.bstr = n, r.buf = t, r.str = o
        });

    }, {}]
}, {}, [5]);