!function (r) {
    "use strict";
    function e(r, e, t) {
        for (var n in e)
            !0 !== t && r.hasOwnProperty(n) || (r[n] = e[n]);
    }
    var t = "/", n = "node_modules", i = { _registry: {}, _parsedEntryPoints: [], _lastRegistered: "", _bind: function (r, e, t) {
        return function (n) {
            var i = [].slice.call(arguments).concat(t);
            return r.apply(e, i);
        };
    }, shallowCopy: e, _log: { prepare: function (r, e) {
        var t = "[Codex] " + r;
        return e ? (e.msg = t, e) : t;
    }, warn: function (e, t) {
        r.C.config.logger.warn(this.prepare(e, t));
    }, error: function (e, t) {
        r.C.config.logger.error(this.prepare(e, t));
    } }, _createBoundRequire: function (r) {
        var e = this, t = e._bind, n = t(e.require, e, r);
        return n.ensure = t(e.requireAsync, e, r), n;
    }, _buildNmPath: function (r, e, i) {
        var o = e.split(t), s = o.lastIndexOf(n) + 2;
        o = o.slice(0, s).concat([n, r]);
        for (var a = 0; a < i; a++)
            s = o.lastIndexOf(n, s - 1);
        var u = o.slice(0, s).join(t);
        return "" !== u && (u += t), u += n + t + r;
    }, _normalize: function (r, e) {
        return -1 === r.indexOf("./") ? this._normalizeNodeModulePath(r, e) : this._normalizeFilePath(r, e);
    }, _normalizeNodeModulePath: function (r, e) {
        var t = { module: r, requiredBy: e };
        if (!this.config || !this.config.nmEntryPoints)
            return r;
        var i, o = this.config.nmEntryPoints, s = o[r];
        if (!s)
            return this._log.error("No entry points found!", t), r;
        if (1 === s.length)
            return s[0];
        if (1 === e.split(n).length)
            return i = s.filter(function (r) {
                return 2 === r.split(n).length;
            });
        for (var a = 0; !i;) {
            for (var u = this._buildNmPath(r, e, a), f = 0; f < s.length; f++)
                0 === s[f].indexOf(u) && (i = s[f]);
            a++;
        }
        return i;
    }, _normalizeFilePath: function (r, e) {
        for (var n = r.split(t), i = e.split(t), o = i.slice(0, -1), s = [], a = 0; a < n.length; a++)
            "." === n[a] ? s = o : ".." === n[a] ? (o.pop(), s = o) : s.push(n[a]);
        return s = s.join(t);
    }, _findEntry: function (r) {
        for (var e = this._registry, t = [r + "/index.js", r + ".js", r], n = t.length; n--;)
            if (e[t[n]])
                return { entry: e[t[n]], normalizedName: t[n] };
        return { entry: null, normalizedName: r };
    }, register: function (r, e) {
        var t = this._registry;
        if ("function" == typeof e)
            if (t[r]) {
                var n = "Module " + r + " already loaded, skipping!";
                this._log.warn(n, { module: r });
            }
            else
                t[r] = { invoke: e };
        this._lastRegistered = r;
    }, require: function (r, e) {
        var t, n, i = e ? this._normalize(r, e) : r;
        if (n = this._findEntry(i), t = n.entry, i = n.normalizedName, t) {
            if (!t.exports) {
                var o = e ? this._registry[e] : null, s = { exports: {}, id: i, parent: o, entrypoint: !o, invoke: t.invoke };
                this._registry[i] = t = s, s.invoke(this._createBoundRequire(i), s, s.exports);
            }
            return t.exports;
        }
        var a = { module: i, requiredBy: e };
        return !0 === this.config.stub ? { stub: !0 } : void this._log.error("MODULE NOT FOUND!", a);
    }, requireAsync: function (e, t, n) {
        var i = this;
        if (!r.C.fetch)
            throw new Error("FETCH NOT IMPLEMENTED!");
        if (!r.C.Client)
            throw new Error("CODEX-CLIENT NOT AVAILABLE!");
        if (!r.C.config)
            throw new Error("CONFIG NOT AVAILABLE!");
        var o = r.C.config;
        if (!o.namespace || !o.version || !o.id)
            throw new Error("CONFIG MISSING BUILD-TIME CONTEXT");
        if (!o.truthMap && o.truths)
            throw new Error("TRUTH MAP NOT AVAILABLE!");
        if (!o.client)
            throw new Error("CONFIG.CLIENT NOT SET!");
        for (var s = e instanceof Array ? e : [e], a = 0; a < s.length; a++)
            s[a] = i._normalize(s[a], n);
        var u = i._parsedEntryPoints, f = { type: "js", files: s, excludeFiles: u, namespace: o.namespace, version: o.version, id: o.id };
        o.truthMap && o.truths && (f.truthMap = o.truthMap, f.truths = o.truths);
        var l = new r.C.Client(o.client), h = l.getUrl(f);
        r.C.fetch(h, function () {
            i._parsedEntryPoints = u.concat(s), t();
        });
    }, kickoff: function (r) {
        var e = r;
        if (!e && "" === this._lastRegistered)
            return void this._log.error("NO MODULES REGISTERED!");
        e || (e = this._lastRegistered), e = e instanceof Array ? e : [e];
        for (var t = 0; t < e.length; t++)
            this._parsedEntryPoints.push(e[t]), this.require(e[t]);
    } };
    r.Codex && r.Codex.config ? e(r.Codex, i, !1) : (r.Codex = i, r.Codex.config = {}), r.C = r.Codex, r.C.r = r.C.register, r.C.k = r.C.kickoff;
}(window);
!function (o) {
    "use strict";
    var e = { stub: !0, logger: console };
    if (!o || !o.C)
        throw new Error("[Codex] Codex bootstrap not loaded!");
    var r = { id: "js", namespace: "webui", version: "0.0.1-akira-js-mk-v023848d6" };
    o.C.shallowCopy(o.C.config, r, !0), o.C.shallowCopy(o.C.config, e, !1);
}(window);
!function (t) {
    "use strict";
    if ("object" != typeof t.Codex)
        throw new Error("[Codex] Codex client shim requires global.Codex!");
    t.Codex.Client = function t(o) {
        if (!(this instanceof t))
            return new t(o);
        var e = this;
        if (e._stack = o.hasOwnProperty("stack") ? o.stack.toUpperCase() : "", e._urlEncodedCodexVersion = encodeURIComponent("^") + e.constants.MAJOR_VERSION_SEMVER, e._port = o.port, e._customHost = function () {
            var t = "";
            return o.hasOwnProperty("host") && (t = e._trimSlashes(o.host), o.hasOwnProperty("prefixPath") && (t += "/" + e._trimSlashes(o.prefixPath))), t;
        }(), e._protocol = o.protocol || "https://", "" === e._stack)
            throw new Error("`stack` is required!");
        if (!e.constants.STACKS.hasOwnProperty(e._stack))
            throw new Error(e._stack + " is an unsupported stack!");
        if (o.hasOwnProperty("prefixPath") && !o.hasOwnProperty("host"))
            throw new Error("`prefixPath` requires `host` value!");
    }, t.Codex.Client.create = t.Codex.Client, t.Codex.Client.prototype = { _resolveHost: function (t) {
        var o = this, e = o.constants.HOST[o._stack], n = void 0 === t || Boolean(t);
        return o._stack !== o.constants.STACKS.PROD && "" !== o._customHost && (e = o._customHost), o._stack === o.constants.STACKS.PROD && !0 === n && (e = o._customHost || o.constants.HOST.CDN), e;
    }, getUrl: function (t) {
        var o = this, e = o.constants.NONE, n = "", s = o._protocol + o._resolveHost(t.cdn);
        return t.truths && t.truths.length > 0 && (e = t.truths.map(function (o) {
            return t.truthMap.kv[o];
        }).join("")), t.shimFlags && (!0 === t.shimFlags.bootstrap && (n += "b"), !0 === t.shimFlags.client && (n += "c"), !0 === t.shimFlags.kickoffLastOnly ? n += "l" : !0 === t.shimFlags.kickoff && (n += "k")), [s + (o._port ? ":" + o._port : ""), o._urlEncodedCodexVersion, o.constants.BASE_URL, t.namespace, t.version, t.id || t.type, t.type, o._encodeEntryPoints(t.files), t.hasOwnProperty("truthMap") ? t.truthMap.length || 0 : o.constants.NONE, e, n || o.constants.NONE, !1 !== t.resolveConditions ? "true" : "false", t.excludeFiles && t.excludeFiles.length > 0 ? o._encodeEntryPoints(t.excludeFiles) : o.constants.NONE].map(function (t) {
            return o._trimSlashes(t.toString());
        }).join("/");
    }, _encodeEntryPoints: function (t) {
        var o = this, e = [];
        return "object" == typeof t ? (e = t.map(function (t) {
            return t.replace(/\//g, o.constants.URL_SLASH_CHAR);
        }), e.join(",")) : t.replace(/\//g, o.constants.URL_SLASH_CHAR);
    }, _trimSlashes: function (t) {
        return t.replace(/^\/+|\/+$/g, "");
    }, constants: { HOST: { DEVELOPMENT: "127.0.0.1", PRBUILDER: "codex-prbuilder.netflix.com", TEST: "codex-test.netflix.com", PROD: "codex-prod.netflix.com", CDN: "codex.nflxext.com" }, STACKS: { DEVELOPMENT: "DEVELOPMENT", PRBUILDER: "PRBUILDER", TEST: "TEST", PROD: "PROD" }, BASE_URL: "truthBundle", NONE: "none", URL_SLASH_CHAR: "%7C", MAJOR_VERSION_SEMVER: "2.0.0" } };
}(window);
C.r("7a", function (t, c, i) {
    "use strict";
});
!function (o) {
    "use strict";
    if (!(o && o.C && o.C.k))
        throw new Error("[Codex] Codex bootstrap not loaded!");
    o.C.k();
}(window);
C.r("7e", function (e, t, n) {
    "use strict";
    if ("undefined" != typeof window && ("undefined" == typeof global && (window.global = {}), "undefined" == typeof process && (window.process = { env: {} })), "undefined" != typeof Intl && Intl.__disableRegExpRestore && Intl.__disableRegExpRestore(), window && window.netflix && window.netflix.reactContext && window.netflix.reactContext.models && window.netflix.reactContext.models.codexClient && window.netflix.reactContext.models.codexClient.data && window.netflix.reactContext.models.codexClient.data.obfuscatedTruths) {
        Codex.fetch = function (e, t) {
            var n = document.getElementsByTagName("script")[0], o = document.createElement("script"), d = !1;
            o.src = e, o.type = "text/javascript", o.onload = o.onreadystatechange = function () {
                d || this.readyState && "complete" !== this.readyState || (d = !0, t());
            }, n && n.parentNode && n.parentNode.insertBefore(o, n);
        };
        var o = window.netflix.reactContext.models.codexClient.data, d = o.obfuscatedTruths, i = { kv: {} }, a = d.length;
        if (a) {
            i.length = d[0].length;
            for (var c = 0; c < a; ++c) {
                var l = d[c];
                i.kv[l] = l;
            }
        }
        Codex.config.client = o.config, Codex.config.truths = d, Codex.config.truthMap = i;
    }
});
!function (o) {
    "use strict";
    if (!(o && o.C && o.C.k))
        throw new Error("[Codex] Codex bootstrap not loaded!");
    o.C.k();
}(window);
