(window.webpackJsonpBrowse = window.webpackJsonpBrowse || []).push([[13], {
    "/pwX": function(e, t, n) {
        "use strict";
        var a = n("wx14")
          , i = n("q1tI")
          , r = n.n(i)
          , o = n("eV9I")
          , c = r.a.createElement;
        t.a = function(e) {
            return function(t) {
                return c(o.a.Consumer, null, (function(n) {
                    var i = n.cacheKey
                      , r = n.updateCacheKey;
                    return c(e, Object(a.a)({
                        cacheKey: i,
                        updateCacheKey: r
                    }, t))
                }
                ))
            }
        }
    },
    "/x/M": function(e, t, n) {
        "use strict";
        (function(e) {
            var a = n("1OyB")
              , i = n("vuIU")
              , r = n("md7G")
              , o = n("foSv")
              , c = n("JX7q")
              , l = n("Ji7U")
              , s = n("rePB")
              , u = n("q1tI")
              , d = n.n(u)
              , f = n("v/+M")
              , m = n("6kA8")
              , p = n("J+Nd")
              , h = n("mwIZ")
              , b = n.n(h)
              , v = n("D1y2")
              , g = n.n(v)
              , y = n("Scth")
              , O = n.n(y)
              , _ = n("TSYQ")
              , j = n.n(_)
              , I = n("qzj+")
              , w = (n("9xXN"),
            d.a.createElement)
              , T = "h3oBrowse"
              , S = "welcomeModal"
              , E = function(t) {
                function n() {
                    var t, i;
                    Object(a.a)(this, n);
                    for (var l = arguments.length, u = new Array(l), d = 0; d < l; d++)
                        u[d] = arguments[d];
                    return i = Object(r.a)(this, (t = Object(o.a)(n)).call.apply(t, [this].concat(u))),
                    Object(s.a)(Object(c.a)(i), "state", {
                        show: !1
                    }),
                    Object(s.a)(Object(c.a)(i), "onHide", (function() {
                        var t = i.props
                          , n = t.next
                          , a = t.version;
                        try {
                            localStorage.setItem("_hulu_welcome_modal", a)
                        } catch (r) {
                            O.a.error("fail to set _hulu_welcome_modal to localStorage due to error ".concat(r))
                        }
                        g()(e, [T, S], a),
                        n()
                    }
                    )),
                    i
                }
                return Object(l.a)(n, t),
                Object(i.a)(n, [{
                    key: "componentDidMount",
                    value: function() {
                        var e = this.props.next;
                        this.showWelcome(e)
                    }
                }, {
                    key: "showWelcome",
                    value: function(t) {
                        var n = this.props.version
                          , a = 0;
                        try {
                            a = localStorage.getItem("_hulu_welcome_modal") || 0
                        } catch (r) {
                            O.a.error("fail to get _hulu_welcome_modal from localStorage due to error ".concat(r))
                        }
                        0 === a && (a = b()(e, [T, S], 0));
                        var i = parseFloat(n) > parseFloat(a);
                        i || t(),
                        this.setState({
                            show: i
                        })
                    }
                }, {
                    key: "render",
                    value: function() {
                        var e, t = this.props, n = t.hasLiveTV, a = t.isNoah;
                        return e = n ? "LevelFour.welcome_modal.live_stream" : a ? "LevelFour.welcome_modal.noah" : "LevelFour.welcome_modal.default",
                        this.state.show ? w(f.a, {
                            className: "WelcomeModal",
                            role: "dialog",
                            "aria-labelledby": "WelcomeModal__title"
                        }, w("h1", {
                            id: "WelcomeModal__title",
                            className: "WelcomeModal__title"
                        }, w(p.view, {
                            from: "".concat(e, ".title"),
                            isHTML: !0
                        })), w("span", {
                            className: "WelcomeModal__subtitle"
                        }, w(p.view, {
                            from: "".concat(e, ".subTitle"),
                            isHTML: !0
                        })), w("div", {
                            className: "WelcomeModal__bullets"
                        }, w(p.view, {
                            from: "".concat(e, ".points"),
                            isHTML: !0
                        })), w("div", {
                            className: "WelcomeModal__prompt"
                        }, w(p.view, {
                            from: "".concat(e, ".body"),
                            isHTML: !0
                        })), w(m.a, {
                            className: j()("WelcomeModal__cta", Object(I.a)("WelcomeModal", "cta")),
                            buttonStyle: "cta",
                            onClick: this.onHide
                        }, w(p.view, {
                            from: "".concat(e, ".ctaText")
                        })), w("div", {
                            className: "WelcomeModal__footnote"
                        }, w(p.view, {
                            from: "".concat(e, ".footNote"),
                            isHTML: !0
                        }))) : null
                    }
                }]),
                n
            }(d.a.Component);
            t.a = E
        }
        ).call(this, n("3r9c"))
    },
    "0f81": function(e, t, n) {
        "use strict";
        (function(e) {
            var a = n("q1tI")
              , i = n.n(a)
              , r = n("O7qf")
              , o = n("H9OJ")
              , c = n("Pn51")
              , l = n("m4ty")
              , s = (n("jSP2"),
            n("ZoaF"))
              , u = n("GDm6")
              , d = n("SgFH")
              , f = i.a.createElement
              , m = function(t) {
                var n = t.id
                  , a = t.type
                  , i = t.upsellPackageId
                  , r = {
                    impression: [{
                        id: n,
                        type: a
                    }],
                    collectionMetricsParams: t.collectionMetricsParams,
                    selectionTrackingId: t.selectionTrackingId
                };
                i && (r.upsellImpression = [{
                    id: n,
                    type: a,
                    upsellPackageId: i
                }]),
                u.g.trackImpression(e.store.getState, r)
            }
              , p = function(e) {
                var t = e.list
                  , n = e.offsiteItems
                  , i = {
                    collectionIndex: 0,
                    collectionName: d.COLLECTION_NAME.OFFISTE,
                    pageName: d.SEARCH_PAGE_NAME.INSTANT
                }
                  , u = {
                    collectionIndex: 0,
                    collectionName: d.COLLECTION_NAME.TOP_RESULTS,
                    pageName: d.SEARCH_PAGE_NAME.INSTANT
                };
                return f(a.Fragment, null, f(c.a, null), f("ul", {
                    className: "InstantSearch",
                    role: "listbox",
                    id: "instantSearch"
                }, t.map((function(e, t) {
                    return f(s.a, {
                        className: "InstantSearch__Option",
                        tag: "li",
                        role: "option",
                        key: o.g(e),
                        id: o.g(e),
                        onEnter: m,
                        metricsParams: {
                            id: o.g(e),
                            type: o.o(e),
                            selectionTrackingId: e.selectionTrackingId,
                            upsellPackageId: o.r(e),
                            collectionMetricsParams: u
                        }
                    }, f(r.a, {
                        asset: e,
                        index: t,
                        collectionMetricsParams: u
                    }))
                }
                )), f(l.a, {
                    list: n,
                    collectionMetricsParams: i
                })))
            };
            p.defaultProps = {
                list: [],
                offsiteItems: []
            },
            t.a = p
        }
        ).call(this, n("3r9c"))
    },
    "2kvc": function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.ECB = void 0;
        t.ECB = {
            encrypt: function(e, t) {
                for (var n = 4 * e._blocksize, a = n - t.length % n, i = 0; i < a; i++)
                    t.push(a);
                for (var r = 0; r < t.length; r += n)
                    e._encryptblock(t, r)
            },
            decrypt: function(e, t, n) {
                for (var a = 4 * e._blocksize, i = 0; i < t.length; i += a)
                    e._decryptblock(t, i);
                if (!n)
                    for (var r = t[t.length - 1], o = 0; o < r; o++)
                        t.pop()
            }
        }
    },
    "5nWE": function(e, t, n) {
        "use strict";
        (function(e) {
            n.d(t, "a", (function() {
                return o
            }
            ));
            var a = n("Scth")
              , i = n.n(a)
              , r = function(t) {
                return {
                    storageExists: function() {
                        try {
                            return e[t].setItem("storagetest", 1),
                            e[t].removeItem("storagetest"),
                            !0
                        } catch (n) {
                            return !1
                        }
                    },
                    get: function(n) {
                        try {
                            var a = e[t].getItem(n);
                            return JSON.parse(a)
                        } catch (r) {
                            return void i.a.error(r)
                        }
                    },
                    set: function(n, a) {
                        try {
                            e[t].setItem(n, JSON.stringify(a))
                        } catch (r) {
                            return void i.a.error(r)
                        }
                    }
                }
            }
              , o = r("localStorage");
            r("sessionStorage")
        }
        ).call(this, n("3r9c"))
    },
    "5t+C": function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.actions = void 0;
        var a = function(e) {
            if (e && e.__esModule)
                return e;
            if (null === e || "object" !== typeof e && "function" !== typeof e)
                return {
                    default: e
                };
            var t = o();
            if (t && t.has(e))
                return t.get(e);
            var n = {}
              , a = Object.defineProperty && Object.getOwnPropertyDescriptor;
            for (var i in e)
                if (Object.prototype.hasOwnProperty.call(e, i)) {
                    var r = a ? Object.getOwnPropertyDescriptor(e, i) : null;
                    r && (r.get || r.set) ? Object.defineProperty(n, i, r) : n[i] = e[i]
                }
            n.default = e,
            t && t.set(e, n);
            return n
        }(n("P160"));
        t.actions = a;
        var i, r = (i = n("HCao")) && i.__esModule ? i : {
            default: i
        };
        function o() {
            if ("function" !== typeof WeakMap)
                return null;
            var e = new WeakMap;
            return o = function() {
                return e
            }
            ,
            e
        }
        t.view = r.default
    },
    "68yl": function(e, t, n) {
        "use strict";
        var a = n("rePB")
          , i = n("H9OJ")
          , r = n("1OyB")
          , o = n("vuIU")
          , c = n("md7G")
          , l = n("foSv")
          , s = n("JX7q")
          , u = n("Ji7U")
          , d = n("q1tI")
          , f = n.n(d)
          , m = n("p3iw")
          , p = n("GEYT")
          , h = n.n(p)
          , b = n("TSYQ")
          , v = n.n(b)
          , g = n("SgFH")
          , y = n("YYy2")
          , O = n("GfZ+")
          , _ = f.a.createElement
          , j = function(e) {
            function t(e) {
                var n;
                return Object(r.a)(this, t),
                (n = Object(c.a)(this, Object(l.a)(t).call(this, e))).onMouseEnter = n.onMouseEnter.bind(Object(s.a)(n)),
                n.onMouseLeave = n.onMouseLeave.bind(Object(s.a)(n)),
                n.onSearchClick = n.onSearchClick.bind(Object(s.a)(n)),
                n.trackSearchClickEvent = n.trackSearchClickEvent.bind(Object(s.a)(n)),
                n.trackTealiumEvent = n.trackTealiumEvent.bind(Object(s.a)(n)),
                n
            }
            return Object(u.a)(t, e),
            Object(o.a)(t, [{
                key: "onMouseEnter",
                value: function() {
                    this.props.setSelectIndex({
                        type: m.a.OFFSITE,
                        index: this.props.index
                    })
                }
            }, {
                key: "onMouseLeave",
                value: function() {
                    this.props.setSelectIndex({
                        type: m.a.OFFSITE,
                        index: void 0
                    })
                }
            }, {
                key: "onSearchClick",
                value: function(e) {
                    var t = this.props
                      , n = t.name
                      , a = t.addToRecentSearch
                      , i = t.getRecoResult
                      , r = t.clearClick;
                    i(),
                    a({
                        text: n
                    }),
                    r(),
                    e && e.preventDefault(),
                    this.trackSearchClickEvent(),
                    this.trackTealiumEvent()
                }
            }, {
                key: "trackTealiumEvent",
                value: function() {
                    var e = this.props.metricsInfo
                      , t = Object(y.d)(e.entityType, e.entityName);
                    Object(O.trackTealium)(t)
                }
            }, {
                key: "trackSearchClickEvent",
                value: function() {
                    var e = this.props
                      , t = e.index
                      , n = e.collectionMetricsParams
                      , a = e.metricsInfo
                      , i = e.selectionTrackingId;
                    (0,
                    e.trackClick)({
                        type: a.entityType,
                        id: a.entityId,
                        selectionTrackingId: i,
                        index: t,
                        targetType: g.SEARCH_TARGET_TYPE.ITEM,
                        collectionMetricsParams: n,
                        actionFrom: g.SEARCH_ACTION_PAGE_TYPE.SEARCH,
                        actionTo: g.SEARCH_ACTION_PAGE_TYPE.SEARCH_RECO
                    })
                }
            }, {
                key: "componentDidUpdate",
                value: function() {
                    var e = this.props
                      , t = e.selectIndex
                      , n = e.clickIndex
                      , a = e.index
                      , i = e.isMouseEnabled;
                    t.type !== m.a.OFFSITE || t.index !== a || i || h()(this.node, {
                        time: 300
                    }),
                    n.type === m.a.OFFSITE && n.index === a && this.onSearchClick()
                }
            }, {
                key: "render",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.highlightText
                      , a = t.displayType
                      , i = t.selectIndex
                      , r = t.index
                      , o = i.type === m.a.OFFSITE && i.index === r
                      , c = v()("Item__Title", {
                        "Item__Title--selected": o
                    });
                    return _("button", {
                        ref: function(t) {
                            return e.node = t
                        },
                        tabIndex: "-1",
                        onMouseEnter: this.onMouseEnter,
                        onMouseLeave: this.onMouseLeave,
                        onClick: this.onSearchClick,
                        className: "Item"
                    }, _("div", {
                        className: c
                    }, n.map((function(e) {
                        return _("span", {
                            key: e.text,
                            className: e.highlighted ? "Item__Title--Highlighted" : "Item__Title--Normal"
                        }, e.text)
                    }
                    ))), _("div", {
                        className: "Item__Type"
                    }, a))
                }
            }]),
            t
        }(d.Component)
          , I = n("Vg22")
          , w = n("nYuY")
          , T = n("GDm6");
        function S(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        t.a = Object(I.connect)((function(e, t) {
            var n = t.item
              , r = e[m.d];
            return function(e) {
                for (var t = 1; t < arguments.length; t++) {
                    var n = null != arguments[t] ? arguments[t] : {};
                    t % 2 ? S(Object(n), !0).forEach((function(t) {
                        Object(a.a)(e, t, n[t])
                    }
                    )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : S(Object(n)).forEach((function(t) {
                        Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                    }
                    ))
                }
                return e
            }({}, t, {
                highlightText: i.e(n),
                displayType: i.d(n),
                name: i.k(n),
                metricsInfo: i.j(n),
                clickIndex: r.clickIndex,
                isMouseEnabled: r.isMouseEnabled,
                selectIndex: r.selectIndex,
                selectionTrackingId: n.selectionTrackingId
            })
        }
        ), (function(e, t) {
            return {
                setSelectIndex: function(t) {
                    e(Object(w.q)(t))
                },
                clearClick: function() {
                    e(Object(w.b)())
                },
                getRecoResult: function() {
                    e(Object(w.j)(t.item))
                },
                addToRecentSearch: function(t) {
                    e(Object(w.a)(t))
                },
                trackClick: function(t) {
                    e(Object(T.h)(T.f.METRICS_CLICK, t))
                }
            }
        }
        ))(j)
    },
    "6qam": function(e, t) {
        e.exports = function(e) {
            return null === e
        }
    },
    "7jF1": function(e, t, n) {
        "use strict";
        var a = this && this.__importStar || function(e) {
            if (e && e.__esModule)
                return e;
            var t = {};
            if (null != e)
                for (var n in e)
                    Object.hasOwnProperty.call(e, n) && (t[n] = e[n]);
            return t.default = e,
            t
        }
          , i = this && this.__importDefault || function(e) {
            return e && e.__esModule ? e : {
                default: e
            }
        }
        ;
        Object.defineProperty(t, "__esModule", {
            value: !0
        });
        var r = a(n("q1tI"))
          , o = i(n("aeRQ"));
        t.useAppsFlyerBanner = function(e) {
            var t = e.bannerKey
              , n = e.onError;
            return r.useEffect((function() {
                if (!window.AF)
                    try {
                        o.default(window, document, "script", 0, "AF", "banners", {
                            banners: {
                                key: t
                            }
                        })
                    } catch (e) {
                        n && n(e)
                    }
            }
            ), []),
            [r.useCallback((function(e) {
                var n = e.bannerKey
                  , a = e.creativeId
                  , i = e.additionalParams;
                window.AF && window.AF("banners", "showBanner", {
                    key: void 0 !== n ? n : t,
                    creativeId: a,
                    additionalParams: i
                })
            }
            ), []), r.useCallback((function() {
                window.AF && window.AF("banners", "hideBanner")
            }
            ), [])]
        }
        ,
        t.default = t.useAppsFlyerBanner
    },
    "8B9+": function(e, t, n) {
        "use strict";
        (function(e) {
            var a = n("q1tI")
              , i = n.n(a)
              , r = n("H9OJ")
              , o = n("68yl")
              , c = (n("1wFk"),
            n("E+oP"))
              , l = n.n(c)
              , s = n("GDm6")
              , u = n("ZoaF")
              , d = n("J+Nd")
              , f = i.a.createElement
              , m = function(t) {
                var n = t.item
                  , a = {
                    collectionMetricsParams: t.collectionMetricsParams,
                    impression: [{
                        id: r.g(n),
                        type: r.i(n)
                    }],
                    selectionTrackingId: n.selectionTrackingId
                };
                s.g.trackImpression(e.store.getState, a)
            }
              , p = function(e) {
                var t = e.list
                  , n = e.collectionMetricsParams;
                return l()(t) ? null : f("div", {
                    className: "OffsiteView"
                }, f("h3", null, f(d.view, {
                    from: "search.offsiteSearch.title"
                })), t.map((function(e, t) {
                    var a = {
                        item: e,
                        collectionMetricsParams: n
                    };
                    return f(u.a, {
                        className: "InstantSearch__Option",
                        tag: "li",
                        role: "option",
                        id: r.g(e),
                        key: r.g(e),
                        onEnter: m,
                        metricsParams: a
                    }, f(o.a, {
                        item: e,
                        index: t,
                        collectionMetricsParams: n
                    }))
                }
                )))
            };
            p.defaultProps = {
                list: []
            },
            t.a = p
        }
        ).call(this, n("3r9c"))
    },
    Aqu8: function(e, t, n) {
        "use strict";
        (function(e) {
            var a = n("1OyB")
              , i = n("vuIU")
              , r = n("md7G")
              , o = n("foSv")
              , c = n("Ji7U")
              , l = n("rePB")
              , s = n("q1tI")
              , u = n.n(s)
              , d = n("aVhw")
              , f = n("oTX3")
              , m = n("t+qN")
              , p = n("cH2P")
              , h = n("/pwX")
              , b = u.a.createElement;
            function v(e, t) {
                var n = Object.keys(e);
                if (Object.getOwnPropertySymbols) {
                    var a = Object.getOwnPropertySymbols(e);
                    t && (a = a.filter((function(t) {
                        return Object.getOwnPropertyDescriptor(e, t).enumerable
                    }
                    ))),
                    n.push.apply(n, a)
                }
                return n
            }
            t.a = function(t) {
                var n = function(n) {
                    function s() {
                        var e;
                        return Object(a.a)(this, s),
                        (e = Object(r.a)(this, Object(o.a)(s).apply(this, arguments))).state = {},
                        e
                    }
                    return Object(c.a)(s, n),
                    Object(i.a)(s, [{
                        key: "getCallbacks",
                        value: function() {
                            return [this.props.initContent, d.b, d.g, this.props.initTileStates]
                        }
                    }, {
                        key: "componentDidUpdate",
                        value: function() {
                            var e = this.props.cacheKey;
                            this.props.dataReady && this.props.dataReady !== Object(m.b)(this.props.url) && (Object(d.a)(this.props.url, {
                                cacheKey: e
                            }, this.getCallbacks()),
                            this.props.setIsLoadingData())
                        }
                    }, {
                        key: "componentDidMount",
                        value: function() {
                            console.log("componentDidMount .....");
                            var t = this.props.cacheKey;
                            e.geo = this.props.geo,
                            Object(d.a)(this.props.url, {
                                cacheKey: t
                            }, this.getCallbacks()),
                            this.props.dataReady && this.props.dataReady === Object(m.b)(this.props.url) || this.props.setIsLoadingData()
                        }
                    }, {
                        key: "render",
                        value: function() {
                            if (this.props.dataReady === Object(m.b)(this.props.url))
                                return b(t, this.props);
                            if (!Object(p.a)()) {
                                var e = function(e) {
                                    for (var t = 1; t < arguments.length; t++) {
                                        var n = null != arguments[t] ? arguments[t] : {};
                                        t % 2 ? v(Object(n), !0).forEach((function(t) {
                                            Object(l.a)(e, t, n[t])
                                        }
                                        )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : v(Object(n)).forEach((function(t) {
                                            Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                                        }
                                        ))
                                    }
                                    return e
                                }({}, this.props.geo, {
                                    cacheKey: this.props.cacheKey
                                });
                                return b("link", {
                                    rel: "preload",
                                    href: Object(d.e)(this.props.url, e),
                                    as: "fetch",
                                    crossOrigin: "use-credentials",
                                    "data-testid": "preload-link"
                                })
                            }
                            return null
                        }
                    }]),
                    s
                }(s.Component);
                return Object(l.a)(n, "displayName", Object(f.b)("WithBackendComponent", t)),
                Object(h.a)(n)
            }
        }
        ).call(this, n("3r9c"))
    },
    ArxA: function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.Player = void 0;
        var a = function(e) {
            if (e && e.__esModule)
                return e;
            if (null === e || "object" !== typeof e && "function" !== typeof e)
                return {
                    default: e
                };
            var t = i();
            if (t && t.has(e))
                return t.get(e);
            var n = {}
              , a = Object.defineProperty && Object.getOwnPropertyDescriptor;
            for (var r in e)
                if (Object.prototype.hasOwnProperty.call(e, r)) {
                    var o = a ? Object.getOwnPropertyDescriptor(e, r) : null;
                    o && (o.get || o.set) ? Object.defineProperty(n, r, o) : n[r] = e[r]
                }
            n.default = e,
            t && t.set(e, n);
            return n
        }(n("5t+C"));
        function i() {
            if ("function" !== typeof WeakMap)
                return null;
            var e = new WeakMap;
            return i = function() {
                return e
            }
            ,
            e
        }
        t.Player = a
    },
    Bxes: function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.Crypto = void 0;
        var a = n("y81G")
          , i = n("2kvc")
          , r = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
          , o = {};
        t.Crypto = o;
        var c = o.util = {
            rotl: function(e, t) {
                return e << t | e >>> 32 - t
            },
            rotr: function(e, t) {
                return e << 32 - t | e >>> t
            },
            endian: function(e) {
                if (e.constructor == Number)
                    return 16711935 & c.rotl(e, 8) | 4278255360 & c.rotl(e, 24);
                for (var t = 0; t < e.length; t++)
                    e[t] = c.endian(e[t]);
                return e
            },
            randomBytes: function(e) {
                for (var t = []; e > 0; e--)
                    t.push(Math.floor(256 * Math.random()));
                return t
            },
            bytesToWords: function(e) {
                for (var t = [], n = 0, a = 0; n < e.length; n++,
                a += 8)
                    t[a >>> 5] |= (255 & e[n]) << 24 - a % 32;
                return t
            },
            wordsToBytes: function(e) {
                for (var t = [], n = 0; n < 32 * e.length; n += 8)
                    t.push(e[n >>> 5] >>> 24 - n % 32 & 255);
                return t
            },
            bytesToHex: function(e) {
                for (var t = [], n = 0; n < e.length; n++)
                    t.push((e[n] >>> 4).toString(16)),
                    t.push((15 & e[n]).toString(16));
                return t.join("")
            },
            hexToBytes: function(e) {
                for (var t = [], n = 0; n < e.length; n += 2)
                    t.push(parseInt(e.substr(n, 2), 16));
                return t
            },
            bytesToBase64: function(e) {
                if ("function" == typeof btoa)
                    return btoa(s.bytesToString(e));
                for (var t = [], n = 0; n < e.length; n += 3)
                    for (var a = e[n] << 16 | e[n + 1] << 8 | e[n + 2], i = 0; i < 4; i++)
                        8 * n + 6 * i <= 8 * e.length ? t.push(r.charAt(a >>> 6 * (3 - i) & 63)) : t.push("=");
                return t.join("")
            },
            base64ToBytes: function(e) {
                if ("function" == typeof atob)
                    return s.stringToBytes(atob(e));
                e = e.replace(/[^A-Z0-9+\/]/gi, "");
                for (var t = [], n = 0, a = 0; n < e.length; a = ++n % 4)
                    0 !== a && t.push((r.indexOf(e.charAt(n - 1)) & Math.pow(2, -2 * a + 8) - 1) << 2 * a | r.indexOf(e.charAt(n)) >>> 6 - 2 * a);
                return t
            }
        };
        o.mode = {},
        o.mode.ECB = i.ECB,
        o.mode.CBC7 = a.CBC7;
        var l = o.charenc = {};
        l.UTF8 = {
            stringToBytes: function(e) {
                return s.stringToBytes(unescape(encodeURIComponent(e)))
            },
            bytesToString: function(e) {
                return decodeURIComponent(escape(s.bytesToString(e)))
            }
        };
        var s = l.Binary = {
            stringToBytes: function(e) {
                for (var t = [], n = 0; n < e.length; n++)
                    t.push(255 & e.charCodeAt(n));
                return t
            },
            bytesToString: function(e) {
                for (var t = [], n = 0; n < e.length; n++)
                    t.push(String.fromCharCode(e[n]));
                return t.join("")
            }
        }
    },
    C3CF: function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.default = void 0;
        var a = n("Jhxl")
          , i = n("Bxes")
          , r = n("hEOo");
        const o = {
            DEEJAY_DEVICE_ID: 190,
            DEEJAY_KEY_VERSION: 1,
            DEEJAY_KEY: "6ebfc84f3c304217b20fd9a66cb5957f",
            REGION: "US",
            LANGUAGE: "en"
        };
        function c(e={}) {
            return Object.keys(e).sort().reduce((t,n)=>{
                const a = e[n];
                return a && t.push(encodeURIComponent(n) + "=" + encodeURIComponent(a)),
                t
            }
            , []).join("&")
        }
        var l = function(e, t) {
            function n(e) {
                try {
                    let t = function(e, t) {
                        const n = i.Crypto.util.hexToBytes(e)
                          , r = i.Crypto.util.hexToBytes("00000000000000000000000000000000").concat(n)
                          , o = a.AES.decrypt(r, t, {
                            mode: i.Crypto.mode.CBC7,
                            usePkcs7: !0
                        })
                          , c = i.Crypto.charenc.UTF8.bytesToString(o);
                        return JSON.parse(c)
                    }(e, i.Crypto.util.hexToBytes(o.DEEJAY_KEY));
                    return t.lastUpdate = new Date,
                    t
                } catch (t) {
                    throw console.log("Error parsing Deejay's raw config:", t),
                    t
                }
            }
            return function(e, t) {
                return fetch(e, {
                    method: "POST",
                    headers: {
                        "Content-type": "application/x-www-form-urlencoded"
                    },
                    credentials: "include",
                    body: c(t)
                }).then(e=>e.text()).then(e=>n(e)).catch(e=>(console.log("Error Deejay config:", e),
                Promise.reject(e)))
            }(`https://play.${e}/config`, function(e) {
                let t = Math.floor(1e6 * Math.random())
                  , n = `${o.DEEJAY_KEY},${o.DEEJAY_DEVICE_ID},${o.DEEJAY_KEY_VERSION},${t}`;
                return n = r.MD5.hexMD5(n),
                {
                    rv: t,
                    encrypted_nonce: n,
                    device: o.DEEJAY_DEVICE_ID,
                    device_id: e,
                    language: o.LANGUAGE,
                    region: o.REGION,
                    version: o.DEEJAY_KEY_VERSION,
                    badging: !0,
                    app_version: 1
                }
            }(t))
        };
        t.default = l
    },
    C4cC: function(e, t, n) {
        "use strict";
        var a = n("rePB")
          , i = n("hpN2")
          , r = n("VNN1")
          , o = n("xk4V")
          , c = n.n(o)
          , l = n("E+oP")
          , s = n.n(l)
          , u = n("p3iw");
        n.d(t, "b", (function() {
            return u.d
        }
        ));
        var d, f = n("P/G1"), m = n.n(f), p = n("H9OJ"), h = n("e7UM");
        function b(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function v(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = null != arguments[t] ? arguments[t] : {};
                t % 2 ? b(Object(n), !0).forEach((function(t) {
                    Object(a.a)(e, t, n[t])
                }
                )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : b(Object(n)).forEach((function(t) {
                    Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                }
                ))
            }
            return e
        }
        var g = {
            keywords: "",
            searchBarInputText: "",
            instantSearchData: null,
            ongoingRequest: !1,
            networkError: !1,
            recentSearchItems: [],
            zeroSearchData: [],
            fullTextSearchData: null,
            offsiteSearchData: null,
            recoData: null,
            queryTime: null,
            selectIndex: {
                type: u.a.ENTITY,
                index: void 0
            },
            clickIndex: {
                type: u.a.ENTITY,
                index: void 0
            },
            isMouseEnabled: !0,
            zeroSearchIsLoaded: !1,
            tileStates: null,
            searchId: "",
            searchSessionId: "",
            queryId: "",
            queryTag: []
        }
          , y = (d = {},
        Object(a.a)(d, r.m, (function(e, t) {
            return v({}, e, {
                instantSearchData: t,
                ongoingRequest: !1,
                isMouseEnabled: !1
            })
        }
        )),
        Object(a.a)(d, r.b, (function(e) {
            return v({}, e, {
                keywords: "",
                searchBarInputText: "",
                instantSearchData: null,
                fullTextSearchData: null,
                queryTag: [],
                recoData: null,
                networkError: !1
            })
        }
        )),
        Object(a.a)(d, r.n, (function(e, t) {
            var n = e.searchSessionId;
            return (s()(t) || s()(n)) && (n = c()()),
            v({}, e, {
                keywords: t,
                searchBarInputText: t,
                queryId: c()(),
                searchSessionId: n,
                fullTextSearchData: null,
                offsiteSearchData: null,
                queryTag: [],
                recoData: null
            })
        }
        )),
        Object(a.a)(d, r.r, (function(e, t) {
            return v({}, e, {
                searchBarInputText: t
            })
        }
        )),
        Object(a.a)(d, r.h, (function(e) {
            return v({}, e, {
                ongoingRequest: !0,
                networkError: !1,
                queryTime: (new Date).getTime()
            })
        }
        )),
        Object(a.a)(d, r.g, (function(e) {
            return v({}, e, {
                ongoingRequest: !1,
                networkError: !0
            })
        }
        )),
        Object(a.a)(d, r.k, (function() {
            return g
        }
        )),
        Object(a.a)(d, r.u, (function(e, t, n) {
            if (!Array.isArray(t))
                return e;
            var a = t
              , i = [{
                series: {
                    type: "series",
                    name: "TV Shows",
                    hubRelativeUrl: "/hub/tv",
                    id: "urn:hulu:hub:tv"
                }
            }, {
                movie: {
                    type: "movie",
                    name: "Movies",
                    hubRelativeUrl: "/hub/movies",
                    id: "urn:hulu:hub:movies"
                }
            }, {
                network: {
                    type: "network",
                    name: "Networks",
                    hubRelativeUrl: "/hub/networks",
                    id: "urn:hulu:hub:networks"
                }
            }, {
                sports_team: {
                    type: "sports_team",
                    name: "Sports Teams",
                    hubRelativeUrl: "/hub/sports",
                    id: "urn:hulu:hub:sports"
                }
            }, {
                genre: {
                    type: "genre",
                    name: "Genres",
                    hubRelativeUrl: null
                }
            }];
            if (!n.hasLiveTV) {
                var r = ["series", "movie", "genre"];
                i = i.filter((function(e) {
                    var t = Object.keys(e)[0];
                    return r.includes(t)
                }
                ))
            }
            var o = i.map((function(e) {
                return v({}, m()(e)[0], {
                    items: a.filter((function(t) {
                        return p.o(t) === Object.keys(e)[0]
                    }
                    ))
                })
            }
            ));
            return v({}, e, {
                zeroSearchData: o = o.filter((function(e) {
                    return !s()(e.items)
                }
                ))
            })
        }
        )),
        Object(a.a)(d, r.v, (function(e) {
            return v({}, e, {
                zeroSearchIsLoaded: !0
            })
        }
        )),
        Object(a.a)(d, r.l, (function(e, t) {
            if (!Array.isArray(t.result))
                return e;
            var n = t.result
              , a = !0
              , i = !1
              , r = void 0;
            try {
                for (var o, c = n[Symbol.iterator](); !(a = (o = c.next()).done); a = !0) {
                    var l = o.value;
                    l.type = l.category,
                    l.items = l.results.slice(0, 30),
                    l.name = l.category
                }
            } catch (s) {
                i = !0,
                r = s
            } finally {
                try {
                    a || null == c.return || c.return()
                } finally {
                    if (i)
                        throw r
                }
            }
            return n = n.filter((function(e) {
                return e.items.length > 0
            }
            )),
            v({}, e, {
                fullTextSearchData: {
                    keywords: t.keywords,
                    result: n
                },
                ongoingRequest: !1
            })
        }
        )),
        Object(a.a)(d, r.o, (function(e, t) {
            return v({}, e, {
                offsiteSearchData: t
            })
        }
        )),
        Object(a.a)(d, r.q, (function(e, t) {
            return v({}, e, {
                recoData: t
            })
        }
        )),
        Object(a.a)(d, r.t, (function(e, t) {
            return v({}, e, {
                tileStates: v({}, t)
            })
        }
        )),
        Object(a.a)(d, r.i, (function(e, t) {
            return v({}, e, {
                recentSearchItems: t
            })
        }
        )),
        Object(a.a)(d, r.f, (function(e) {
            return v({}, e, {
                searchId: c()()
            })
        }
        )),
        Object(a.a)(d, r.p, (function(e, t) {
            return v({}, e, {
                queryTag: t
            })
        }
        )),
        Object(a.a)(d, r.j, (function(e, t) {
            return v({}, e, {
                selectIndex: t
            })
        }
        )),
        Object(a.a)(d, r.c, (function(e, t) {
            return v({}, e, {
                clickIndex: t
            })
        }
        )),
        Object(a.a)(d, r.a, (function(e) {
            return v({}, e, {
                clickIndex: {
                    type: u.a.ENTITY,
                    index: void 0
                }
            })
        }
        )),
        Object(a.a)(d, r.d, (function(e) {
            return v({}, e, {
                isMouseEnabled: !1
            })
        }
        )),
        Object(a.a)(d, r.e, (function(e) {
            return v({}, e, {
                isMouseEnabled: !0
            })
        }
        )),
        Object(a.a)(d, r.s, h.n),
        d);
        t.a = Object(i.a)(g, y)
    },
    CVsM: function(e, t, n) {
        "use strict";
        var a = this && this.__importDefault || function(e) {
            return e && e.__esModule ? e : {
                default: e
            }
        }
        ;
        Object.defineProperty(t, "__esModule", {
            value: !0
        });
        var i = a(n("apDF"))
          , r = a(n("7jF1"));
        t.useAppsFlyerBanner = r.default,
        t.default = i.default
    },
    D1y2: function(e, t, n) {
        var a = n("FZoo");
        e.exports = function(e, t, n) {
            return null == e ? e : a(e, t, n)
        }
    },
    GEYT: function(e, t) {
        var n = "complete"
          , a = "canceled";
        function i(e, t, n) {
            e.self === e ? e.scrollTo(t, n) : (e.scrollLeft = t,
            e.scrollTop = n)
        }
        function r(e) {
            var t = e._scrollSettings;
            if (t) {
                var a = function(e, t, n) {
                    var a, i, r, o, c, l, s, u = e.getBoundingClientRect(), d = n && null != n.left ? n.left : .5, f = n && null != n.top ? n.top : .5, m = n && null != n.leftOffset ? n.leftOffset : 0, p = n && null != n.topOffset ? n.topOffset : 0, h = d, b = f;
                    if (t.self === t)
                        l = Math.min(u.width, t.innerWidth),
                        s = Math.min(u.height, t.innerHeight),
                        i = u.left + t.pageXOffset - t.innerWidth * h + l * h,
                        r = u.top + t.pageYOffset - t.innerHeight * b + s * b,
                        r -= p,
                        o = (i -= m) - t.pageXOffset,
                        c = r - t.pageYOffset;
                    else {
                        l = u.width,
                        s = u.height,
                        a = t.getBoundingClientRect();
                        var v = u.left - (a.left - t.scrollLeft)
                          , g = u.top - (a.top - t.scrollTop);
                        i = v + l * h - t.clientWidth * h,
                        r = g + s * b - t.clientHeight * b,
                        i = Math.max(Math.min(i, t.scrollWidth - t.clientWidth), 0),
                        r = Math.max(Math.min(r, t.scrollHeight - t.clientHeight), 0),
                        r -= p,
                        o = (i -= m) - t.scrollLeft,
                        c = r - t.scrollTop
                    }
                    return {
                        x: i,
                        y: r,
                        differenceX: o,
                        differenceY: c
                    }
                }(t.target, e, t.align)
                  , o = Date.now() - t.startTime
                  , c = Math.min(1 / t.time * o, 1);
                if (o > t.time && t.endIterations > 3)
                    return i(e, a.x, a.y),
                    e._scrollSettings = null,
                    t.end(n);
                t.endIterations++;
                var l = 1 - t.ease(c);
                if (i(e, a.x - a.differenceX * l, a.y - a.differenceY * l),
                o >= t.time)
                    return r(e);
                !function(e) {
                    if ("requestAnimationFrame"in window)
                        return window.requestAnimationFrame(e);
                    setTimeout(e, 16)
                }(r.bind(null, e))
            }
        }
        function o(e, t, n, i) {
            var o, c = !t._scrollSettings, l = t._scrollSettings, s = Date.now();
            function u(e) {
                t._scrollSettings = null,
                t.parentElement && t.parentElement._scrollSettings && t.parentElement._scrollSettings.end(e),
                i(e),
                t.removeEventListener("touchstart", o, {
                    passive: !0
                }),
                t.removeEventListener("wheel", o, {
                    passive: !0
                })
            }
            l && l.end(a),
            t._scrollSettings = {
                startTime: l ? l.startTime : Date.now(),
                endIterations: 0,
                target: e,
                time: n.time + (l ? s - l.startTime : 0),
                ease: n.ease,
                align: n.align,
                end: u
            },
            o = u.bind(null, a),
            t.addEventListener("touchstart", o, {
                passive: !0
            }),
            t.addEventListener("wheel", o, {
                passive: !0
            }),
            c && r(t)
        }
        function c(e) {
            return "pageXOffset"in e || (e.scrollHeight !== e.clientHeight || e.scrollWidth !== e.clientWidth) && "hidden" !== getComputedStyle(e).overflow
        }
        function l() {
            return !0
        }
        e.exports = function(e, t, a) {
            if (e) {
                "function" === typeof t && (a = t,
                t = null),
                t || (t = {}),
                t.time = isNaN(t.time) ? 1e3 : t.time,
                t.ease = t.ease || function(e) {
                    return 1 - Math.pow(1 - e, e / 2)
                }
                ;
                for (var i = e.parentElement, r = 0, s = t.validTarget || l, u = t.isScrollable; i; ) {
                    if (s(i, r) && (u ? u(i, c) : c(i)) && (r++,
                    o(e, i, t, d)),
                    !(i = i.parentElement)) {
                        r || a && a(n);
                        break
                    }
                    "BODY" === i.tagName && (i = (i = i.ownerDocument).defaultView || i.ownerWindow)
                }
            }
            function d(e) {
                --r || a && a(e)
            }
        }
    },
    HCao: function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.default = void 0;
        var a = u(n("q1tI"))
          , i = l(n("17x9"))
          , r = l(n("JZM8"))
          , o = u(n("P160"))
          , c = l(n("C3CF"));
        function l(e) {
            return e && e.__esModule ? e : {
                default: e
            }
        }
        function s() {
            if ("function" !== typeof WeakMap)
                return null;
            var e = new WeakMap;
            return s = function() {
                return e
            }
            ,
            e
        }
        function u(e) {
            if (e && e.__esModule)
                return e;
            if (null === e || "object" !== typeof e && "function" !== typeof e)
                return {
                    default: e
                };
            var t = s();
            if (t && t.has(e))
                return t.get(e);
            var n = {}
              , a = Object.defineProperty && Object.getOwnPropertyDescriptor;
            for (var i in e)
                if (Object.prototype.hasOwnProperty.call(e, i)) {
                    var r = a ? Object.getOwnPropertyDescriptor(e, i) : null;
                    r && (r.get || r.set) ? Object.defineProperty(n, i, r) : n[i] = e[i]
                }
            return n.default = e,
            t && t.set(e, n),
            n
        }
        function d(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function f(e, t, n) {
            return t in e ? Object.defineProperty(e, t, {
                value: n,
                enumerable: !0,
                configurable: !0,
                writable: !0
            }) : e[t] = n,
            e
        }
        const m = "https://player.hulu.com/site/dash/"
          , p = "https://player.hulu.com/site/dash/smoke/"
          , h = "https://player.huluqa.com/site/dash/"
          , b = {
            demo: "hulu-web-demo-player-version",
            prod: "hulu-web-prod-player-version",
            staging: "hulu-web-staging-player-version",
            smoke: "hulu-web-smoke-player-version"
        }
          , v = ["backToBrowse", "backToWatch", "closePlayer", "navigateTo", "showLoginForm", "getHitInfo", "refresh", "refreshUserLogin", "goToGuide", "showDetails"]
          , g = ["computerGuid", "latitude", "longitude", "profile", "subscription"]
          , y = {
            position: "absolute",
            height: "100%",
            width: "100%",
            boxSizing: "border-box"
        }
          , O = {
            playerHostUrl: "",
            huluBaseDomain: "",
            playerVersionKey: ""
        };
        class _ extends a.Component {
            constructor(e) {
                super(e),
                this.state = {
                    loadingError: !1
                },
                this._loadAndInitPlayer = this._loadAndInitPlayer.bind(this),
                this._loadPlayer = this._loadPlayer.bind(this),
                function() {
                    const e = window.HULU && window.HULU.env || "prod"
                      , t = "smoke" === e;
                    "staging" === e || "qa" === e ? (O.playerHostUrl = h,
                    O.huluBaseDomain = "huluqa.com",
                    O.playerVersionKey = b.staging) : t ? (O.playerHostUrl = p,
                    O.huluBaseDomain = "hulu.com",
                    O.playerVersionKey = b.smoke) : (O.playerHostUrl = m,
                    O.huluBaseDomain = "hulu.com",
                    O.playerVersionKey = b.prod)
                }()
            }
            componentDidMount() {
                const e = this.props
                  , t = e.config
                  , n = e.isNonSub
                  , a = (0,
                r.default)(this.props, g);
                this._loadAndInitPlayer(function(e) {
                    for (var t = 1; t < arguments.length; t++) {
                        var n = null != arguments[t] ? arguments[t] : {};
                        t % 2 ? d(n, !0).forEach((function(t) {
                            f(e, t, n[t])
                        }
                        )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : d(n).forEach((function(t) {
                            Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                        }
                        ))
                    }
                    return e
                }({}, a, {
                    isNonSub: Boolean(t.isNonSub || n),
                    enabledAdsOverride: t.enabledAdsOverride,
                    liveGuideEnabled: t.liveGuideEnabled,
                    isForcedNonSub: Boolean(n),
                    siteAPI: (0,
                    r.default)(this.props, v)
                })),
                o.updateCommonContext(a)
            }
            static getDerivedStateFromProps(e) {
                const t = (0,
                r.default)(e, g)
                  , n = (0,
                r.default)(e, v);
                return o.updateCommonContext(t),
                o.updateSiteApi(n),
                null
            }
            shouldComponentUpdate() {
                return !1
            }
            render() {
                return a.default.createElement("div", {
                    id: "dash-player-container",
                    style: y
                })
            }
            _loadPlayer(e, t, n) {
                var a;
                (a = `${e}load_player_build.js`,
                new Promise((e,t)=>{
                    let n = document.createElement("script");
                    n.type = "text/javascript",
                    n.src = a,
                    n.async = !1,
                    n.onload = e,
                    n.onerror = t,
                    n.onabort = t,
                    document.head.appendChild(n)
                }
                )).then(()=>
                {
                    console.log("loadSitePlayer ... ");
                    //console.log("e= " + e);
                    //console.log("t= " + t);
                    window.DashPlayer.loadSitePlayer(e, t);
                }                
                ).then(()=>{
                    console.log("Content config parameter: " + JSON.stringify(n));
                    const e = document.getElementById("dash-player-container");
                    let a = window.DashPlayer.getSitePlayer(e, n);
                    console.log("getSitePlayer a=" + JSON.stringify(a));
                    localStorage.setItem(O.playerVersionKey, t),
                    o.resolveInitPromise(a),
                    this.setState({
                        loadingError: !1
                    });
                    console.log("loadSitePlayer done. ");
                    o.startVideo2();
                }
                ).catch(error => {
                    console.error("loadSitePlayer err: " + JSON.stringify(error));
                    console.log(error.stack);
                    this.setState({
                        loadingError: !0
                    })
                }
                )
            }
            _loadAndInitPlayer(e={}) {
                console.log("loadAndInitPlayer ... " + JSON.stringify(e));
                const t = function(e="", t) 
                {
                    e = e.replace(/[[\]]/g, "\\$&");
                    const n = new RegExp("[?&]" + e + "(=([^&#]*)|&|#|$)","i").exec(t);
                    if (n && n[2])
                        return decodeURIComponent(n[2].replace(/\+/g, " "))
                }
                ("h5_cpv", location.href);
                t ? this._loadPlayer(O.playerHostUrl, t, e) : (0,
                c.default)(O.huluBaseDomain, e.computerGuid).then(t=>{
                    let n = (t.flags || {})[O.playerVersionKey];
                    n || (n = localStorage.getItem(O.playerVersionKey)),
                    n ? (n = n.trim(),
                    this._loadPlayer(O.playerHostUrl, n, e)) : this.setState({
                        loadingError: !0
                    })
                }
                ).catch(t=>{
                    console.error("loadAndInitPlayer err: " + JSON.stringify(t));
                    let n = localStorage.getItem(O.playerVersionKey);
                    n ? this._loadPlayer(O.playerHostUrl, n, e) : this.setState({
                        loadingError: !0
                    })
                }
                )
            }
        }
        _.propTypes = {
            computerGuid: i.default.string,
            latitude: i.default.number,
            longitude: i.default.number,
            eabId: i.default.string,
            isNonSub: i.default.boolean,
            config: i.default.object,
            backToBrowse: i.default.func.isRequired,
            backToWatch: i.default.func.isRequired,
            closePlayer: i.default.func.isRequired,
            navigateTo: i.default.func.isRequired,
            showLoginForm: i.default.func.isRequired,
            getHitInfo: i.default.func.isRequired,
            refresh: i.default.func.isRequired
        },
        _.defaultProps = {
            isNonSub: !1,
            latitude: -1,
            longitude: -1
        };
        var j = _;
        t.default = j
    },
    IH8g: function(e, t, n) {
        "use strict";
        n.d(t, "a", (function() {
            return o
        }
        )),
        n.d(t, "b", (function() {
            return c
        }
        ));
        var a = n("BN2M")
          , i = n("HOv+")
          , r = new Set
          , o = function(e) {
            var t = e.stateKey;
            Object(i.c)(t) ? a.d.trackCollectionImpression(e) : r.add(e)
        }
          , c = function() {
            Array.from(r).forEach((function(e) {
                a.d.trackCollectionImpression(e)
            }
            )),
            r.clear()
        }
    },
    Jc6e: function(e, t, n) {
        "use strict";
        (function(e) {
            var a = n("q1tI")
              , i = n.n(a)
              , r = n("iR1w")
              , o = n("mwIZ")
              , c = n.n(o)
              , l = n("WjpJ")
              , s = n.n(l)
              , u = (n("Bptr"),
            n("g/JB"))
              , d = n("4a2M")
              , f = n("VGA2")
              , m = i.a.createElement
              , p = function(e) {
                return Math.ceil(e / u.b) || 5
            }
              , h = Object(a.memo)((function() {
                var t = function(e) {
                    var t = e.data
                      , n = e.style;
                    return m("div", {
                        className: "LiveGuideChannelGridSkeleton__channel",
                        style: n
                    }, m("div", {
                        className: "LiveGuideChannelGridSkeleton__program LiveGuideChannelGridSkeleton__program--on-now"
                    }, m("div", {
                        className: "LiveGuideChannelGridSkeleton__channel-logo-container"
                    }, m("div", {
                        className: "LiveGuideChannelGridSkeleton__channel-logo"
                    }))), s()(p(t.breakpoint)).map((function(e) {
                        return m("div", {
                            key: "skeleton-program-".concat(e),
                            className: "LiveGuideChannelGridSkeleton__program"
                        })
                    }
                    )))
                };
                return m(d.a, null, (function(n) {
                    return m("div", {
                        className: "LiveGuideChannelGridSkeleton",
                        "data-testid": "live-guide-channel-grid-skeleton",
                        style: {
                            marginLeft: Object(f.a)(n, u.g),
                            "--cell-width": "".concat(u.b, "px")
                        }
                    }, m("div", {
                        className: "LiveGuideChannelGridSkeleton__time-blocks"
                    }, m("div", {
                        className: "LiveGuideChannelGridSkeleton__time-block LiveGuideChannelGridSkeleton__time-block--on-now"
                    }), s()(p(n)).map((function(e) {
                        return m("div", {
                            key: "skeleton-time-block-".concat(e),
                            className: "LiveGuideChannelGridSkeleton__time-block"
                        })
                    }
                    ))), m(r.a, {
                        className: "List",
                        height: c()(e, "window.innerHeight", 1e3),
                        itemCount: 50,
                        itemSize: u.a,
                        itemData: {
                            breakpoint: n
                        },
                        style: {
                            overflow: "hidden"
                        }
                    }, t))
                }
                ))
            }
            ));
            t.a = h
        }
        ).call(this, n("3r9c"))
    },
    Jhxl: function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.AES = void 0;
        for (var a = n("Bxes"), i = [99, 124, 119, 123, 242, 107, 111, 197, 48, 1, 103, 43, 254, 215, 171, 118, 202, 130, 201, 125, 250, 89, 71, 240, 173, 212, 162, 175, 156, 164, 114, 192, 183, 253, 147, 38, 54, 63, 247, 204, 52, 165, 229, 241, 113, 216, 49, 21, 4, 199, 35, 195, 24, 150, 5, 154, 7, 18, 128, 226, 235, 39, 178, 117, 9, 131, 44, 26, 27, 110, 90, 160, 82, 59, 214, 179, 41, 227, 47, 132, 83, 209, 0, 237, 32, 252, 177, 91, 106, 203, 190, 57, 74, 76, 88, 207, 208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2, 127, 80, 60, 159, 168, 81, 163, 64, 143, 146, 157, 56, 245, 188, 182, 218, 33, 16, 255, 243, 210, 205, 12, 19, 236, 95, 151, 68, 23, 196, 167, 126, 61, 100, 93, 25, 115, 96, 129, 79, 220, 34, 42, 144, 136, 70, 238, 184, 20, 222, 94, 11, 219, 224, 50, 58, 10, 73, 6, 36, 92, 194, 211, 172, 98, 145, 149, 228, 121, 231, 200, 55, 109, 141, 213, 78, 169, 108, 86, 244, 234, 101, 122, 174, 8, 186, 120, 37, 46, 28, 166, 180, 198, 232, 221, 116, 31, 75, 189, 139, 138, 112, 62, 181, 102, 72, 3, 246, 14, 97, 53, 87, 185, 134, 193, 29, 158, 225, 248, 152, 17, 105, 217, 142, 148, 155, 30, 135, 233, 206, 85, 40, 223, 140, 161, 137, 13, 191, 230, 66, 104, 65, 153, 45, 15, 176, 84, 187, 22], r = [], o = 0; o < 256; o++)
            r[i[o]] = o;
        var c = []
          , l = []
          , s = []
          , u = []
          , d = []
          , f = [];
        function m(e, t) {
            for (var n = 0, a = 0; a < 8; a++) {
                1 & t && (n ^= e);
                var i = 128 & e;
                e = e << 1 & 255,
                i && (e ^= 27),
                t >>>= 1
            }
            return n
        }
        for (o = 0; o < 256; o++)
            c[o] = m(o, 2),
            l[o] = m(o, 3),
            s[o] = m(o, 9),
            u[o] = m(o, 11),
            d[o] = m(o, 13),
            f[o] = m(o, 14);
        var p, h, b, v = [0, 1, 2, 4, 8, 16, 32, 64, 128, 27, 54];
        const g = {
            encrypt: function(e, t, n) {
                var i = a.Crypto.charenc.UTF8.stringToBytes(e)
                  , r = a.Crypto.util.randomBytes(4 * g._blocksize)
                  , o = t.constructor == String ? a.Crypto.PBKDF2(t, r, 32, {
                    asBytes: !0
                }) : t
                  , c = n && n.mode || a.Crypto.mode.OFB;
                return g._init(o),
                c.encrypt(g, i, r),
                n && "hex" === n.returnType ? {
                    iv: a.Crypto.util.bytesToHex(r),
                    ciphertext: a.Crypto.util.bytesToHex(i)
                } : {
                    iv: a.Crypto.util.bytesToBase64(r),
                    ciphertext: a.Crypto.util.bytesToBase64(i)
                }
            },
            decrypt: function(e, t, n) {
                var i, r;
                n.mode != a.Crypto.mode.ECB && (i = e.splice(0, 4 * g._blocksize)),
                r = t.constructor == String ? a.Crypto.PBKDF2(t, i, 32, {
                    asBytes: !0
                }) : t;
                var o = n && n.mode || a.Crypto.mode.OFB;
                return g._init(r),
                o.decrypt(g, e, i, n.usePkcs7),
                e
            },
            _blocksize: 4,
            _encryptblock: function(e, t) {
                var n, a, r = [[], [], [], []];
                for (n = 0; n < g._blocksize; n++)
                    for (a = 0; a < 4; a++)
                        r[n][a] = e[t + 4 * a + n];
                for (n = 0; n < 4; n++)
                    for (a = 0; a < 4; a++)
                        r[n][a] ^= b[a][n];
                for (var o = 1; o < h; o++) {
                    for (n = 0; n < 4; n++)
                        for (a = 0; a < 4; a++)
                            r[n][a] = i[r[n][a]];
                    for (r[1].push(r[1].shift()),
                    r[2].push(r[2].shift()),
                    r[2].push(r[2].shift()),
                    r[3].unshift(r[3].pop()),
                    a = 0; a < 4; a++) {
                        var s = r[0][a]
                          , u = r[1][a]
                          , d = r[2][a]
                          , f = r[3][a];
                        r[0][a] = c[s] ^ l[u] ^ d ^ f,
                        r[1][a] = s ^ c[u] ^ l[d] ^ f,
                        r[2][a] = s ^ u ^ c[d] ^ l[f],
                        r[3][a] = l[s] ^ u ^ d ^ c[f]
                    }
                    for (n = 0; n < 4; n++)
                        for (a = 0; a < 4; a++)
                            r[n][a] ^= b[4 * o + a][n]
                }
                for (n = 0; n < 4; n++)
                    for (a = 0; a < 4; a++)
                        r[n][a] = i[r[n][a]];
                for (r[1].push(r[1].shift()),
                r[2].push(r[2].shift()),
                r[2].push(r[2].shift()),
                r[3].unshift(r[3].pop()),
                n = 0; n < 4; n++)
                    for (a = 0; a < 4; a++)
                        r[n][a] ^= b[4 * h + a][n];
                for (n = 0; n < g._blocksize; n++)
                    for (a = 0; a < 4; a++)
                        e[t + 4 * a + n] = r[n][a]
            },
            _decryptblock: function(e, t) {
                var n, a, i = [[], [], [], []];
                for (n = 0; n < g._blocksize; n++)
                    for (a = 0; a < 4; a++)
                        i[n][a] = e[t + 4 * a + n];
                for (n = 0; n < 4; n++)
                    for (a = 0; a < 4; a++)
                        i[n][a] ^= b[4 * h + a][n];
                for (var o = 1; o < h; o++) {
                    for (i[1].unshift(i[1].pop()),
                    i[2].push(i[2].shift()),
                    i[2].push(i[2].shift()),
                    i[3].push(i[3].shift()),
                    n = 0; n < 4; n++)
                        for (a = 0; a < 4; a++)
                            i[n][a] = r[i[n][a]];
                    for (n = 0; n < 4; n++)
                        for (a = 0; a < 4; a++)
                            i[n][a] ^= b[4 * (h - o) + a][n];
                    for (a = 0; a < 4; a++) {
                        var c = i[0][a]
                          , l = i[1][a]
                          , m = i[2][a]
                          , p = i[3][a];
                        i[0][a] = f[c] ^ u[l] ^ d[m] ^ s[p],
                        i[1][a] = s[c] ^ f[l] ^ u[m] ^ d[p],
                        i[2][a] = d[c] ^ s[l] ^ f[m] ^ u[p],
                        i[3][a] = u[c] ^ d[l] ^ s[m] ^ f[p]
                    }
                }
                for (i[1].unshift(i[1].pop()),
                i[2].push(i[2].shift()),
                i[2].push(i[2].shift()),
                i[3].push(i[3].shift()),
                n = 0; n < 4; n++)
                    for (a = 0; a < 4; a++)
                        i[n][a] = r[i[n][a]];
                for (n = 0; n < 4; n++)
                    for (a = 0; a < 4; a++)
                        i[n][a] ^= b[a][n];
                for (n = 0; n < g._blocksize; n++)
                    for (a = 0; a < 4; a++)
                        e[t + 4 * a + n] = i[n][a]
            },
            _init: function(e) {
                p = e.length / 4,
                h = p + 6,
                g._keyexpansion(e)
            },
            _keyexpansion: function(e) {
                var t;
                for (b = [],
                t = 0; t < p; t++)
                    b[t] = [e[4 * t], e[4 * t + 1], e[4 * t + 2], e[4 * t + 3]];
                for (t = p; t < g._blocksize * (h + 1); t++) {
                    var n = [b[t - 1][0], b[t - 1][1], b[t - 1][2], b[t - 1][3]];
                    t % p === 0 ? (n.push(n.shift()),
                    n[0] = i[n[0]],
                    n[1] = i[n[1]],
                    n[2] = i[n[2]],
                    n[3] = i[n[3]],
                    n[0] ^= v[t / p]) : p > 6 && t % p === 4 && (n[0] = i[n[0]],
                    n[1] = i[n[1]],
                    n[2] = i[n[2]],
                    n[3] = i[n[3]]),
                    b[t] = [b[t - p][0] ^ n[0], b[t - p][1] ^ n[1], b[t - p][2] ^ n[2], b[t - p][3] ^ n[3]]
                }
            }
        };
        t.AES = g
    },
    O7qf: function(e, t, n) {
        "use strict";
        var a = n("rePB")
          , i = n("H9OJ")
          , r = n("1OyB")
          , o = n("vuIU")
          , c = n("md7G")
          , l = n("foSv")
          , s = n("JX7q")
          , u = n("Ji7U")
          , d = n("q1tI")
          , f = n.n(d)
          , m = n("GEYT")
          , p = n.n(m)
          , h = n("BN2M")
          , b = n("GfZ+")
          , v = n("t+qN")
          , g = n("GDm6")
          , y = n("p3iw")
          , O = n("TSYQ")
          , _ = n.n(O)
          , j = n("SgFH")
          , I = n("8xii")
          , w = n("YYy2")
          , T = f.a.createElement
          , S = function(e) {
            function t(e) {
                var n;
                return Object(r.a)(this, t),
                (n = Object(c.a)(this, Object(l.a)(t).call(this, e))).onSearchClick = n.onSearchClick.bind(Object(s.a)(n)),
                n.onMouseEnter = n.onMouseEnter.bind(Object(s.a)(n)),
                n.onMouseLeave = n.onMouseLeave.bind(Object(s.a)(n)),
                n.trackUserInteractionEvent = n.trackUserInteractionEvent.bind(Object(s.a)(n)),
                n.trackSearchClickEvent = n.trackSearchClickEvent.bind(Object(s.a)(n)),
                n.trackTealiumEvent = n.trackTealiumEvent.bind(Object(s.a)(n)),
                n
            }
            return Object(u.a)(t, e),
            Object(o.a)(t, [{
                key: "shouldComponentUpdate",
                value: function(e) {
                    return e.imageUrl !== this.props.imageUrl || e.highlightText !== this.props.highlightText
                }
            }, {
                key: "componentDidUpdate",
                value: function() {
                    var e = this.props
                      , t = e.selectIndex
                      , n = e.isMouseEnabled
                      , a = e.clickIndex
                      , i = e.index;
                    t.type !== y.a.ENTITY || t.index !== i || n || p()(this.node, {
                        time: 300
                    }),
                    a.type === y.a.ENTITY && a.index === i && this.onSearchClick()
                }
            }, {
                key: "getImageElement",
                value: function(e, t) {
                    if (e)
                        return T("img", {
                            className: "NormalImage",
                            src: e,
                            role: "presentation"
                        });
                    var n = t.map((function(e) {
                        return e.text
                    }
                    )).join("");
                    return T("div", {
                        className: "FallBackImage"
                    }, T("div", {
                        className: "FallBackImage__Text"
                    }, n))
                }
            }, {
                key: "getWatermarkElement",
                value: function(e) {
                    if (e)
                        return T("img", {
                            className: "BrandWatermark",
                            src: e,
                            role: "presentation"
                        })
                }
            }, {
                key: "getTitleElement",
                value: function(e) {
                    return T("div", {
                        className: "ListItem__Content__HighlightText"
                    }, e.map((function(e, t) {
                        return T("span", {
                            "aria-hidden": !0,
                            key: "".concat(t),
                            className: e.highlighted ? "ListItem__Content__HighlightText--Highlighted" : "ListItem__Content__HighlightText--Normal"
                        }, e.text)
                    }
                    )))
                }
            }, {
                key: "getUpsellImageElement",
                value: function(e) {
                    return e ? T("div", {
                        className: "UpsellImage"
                    }) : null
                }
            }, {
                key: "getUpsellActionElement",
                value: function(e) {
                    return e ? T(I.b, {
                        text: e
                    }) : null
                }
            }, {
                key: "render",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.title
                      , a = t.imageUrl
                      , i = t.highlightText
                      , r = t.displayType
                      , o = t.href
                      , c = t.brand
                      , l = t.isUpsell
                      , s = t.upsellActionText
                      , u = t.selectIndex
                      , d = t.index
                      , f = t.id
                      , m = u.index === d && u.type === y.a.ENTITY
                      , p = _()("ListItem", {
                        "ListItem--Selected": m
                    });
                    return T("a", {
                        ref: function(t) {
                            return e.node = t
                        },
                        tabIndex: "-1",
                        className: p,
                        href: o,
                        onClick: this.onSearchClick,
                        onMouseEnter: this.onMouseEnter,
                        onMouseLeave: this.onMouseLeave
                    }, T("div", {
                        className: "ListItem__Image"
                    }, this.getImageElement(a, i, f), this.getUpsellImageElement(l), this.getWatermarkElement(c)), T("div", {
                        className: "ListItem__Content"
                    }, T("div", {
                        className: "sr-only"
                    }, n), this.getTitleElement(i), this.getUpsellActionElement(s)), T("div", {
                        className: "ListItem__Type"
                    }, r))
                }
            }, {
                key: "onMouseEnter",
                value: function() {
                    this.props.setSelectIndex({
                        type: y.a.ENTITY,
                        index: this.props.index
                    })
                }
            }, {
                key: "onMouseLeave",
                value: function() {
                    this.props.setSelectIndex({
                        type: y.a.ENTITY,
                        index: void 0
                    })
                }
            }, {
                key: "onSearchClick",
                value: function(e) {
                    var t = this.props
                      , n = t.name
                      , a = t.isUpsell
                      , i = t.addRecentSearch
                      , r = t.clearClick
                      , o = t.discoverHref;
                    i({
                        text: n
                    }),
                    a ? Object(v.d)(this.props.href) : Object(v.c)(this.props.href, {}, o),
                    r(),
                    e && e.preventDefault(),
                    this.trackSearchClickEvent(),
                    this.trackUserInteractionEvent(),
                    this.trackTealiumEvent()
                }
            }, {
                key: "trackSearchClickEvent",
                value: function() {
                    var e = this.props
                      , t = e.index
                      , n = e.isUpsell
                      , a = e.upsellPackageId
                      , i = e.trackClick
                      , r = e.collectionMetricsParams
                      , o = e.metricsInfo
                      , c = e.selectionTrackingId
                      , l = {
                        type: o.entityType,
                        id: o.entityId,
                        index: t,
                        targetType: j.SEARCH_TARGET_TYPE.ITEM,
                        collectionMetricsParams: r,
                        actionFrom: j.SEARCH_ACTION_PAGE_TYPE.SEARCH,
                        actionTo: n ? j.SEARCH_ACTION_PAGE_TYPE.UPSELL_LEDGER : j.SEARCH_ACTION_PAGE_TYPE.DETAILS,
                        selectionTrackingId: c
                    };
                    a && (l.upsellPackageId = a),
                    i(l)
                }
            }, {
                key: "trackUserInteractionEvent",
                value: function() {
                    var e = this.props
                      , t = e.index
                      , n = e.eabId
                      , a = e.isUpsell
                      , i = e.metricsInfo;
                    if (!a) {
                        var r = {
                            actionSpecifier: "nav:details",
                            elementSpecifier: h.c.UI_ELEMENT.TILE,
                            displayName: i.entityName,
                            conditionalProperties: ["entity", "collection"],
                            entityData: {
                                entityId: i.entityId,
                                entityActionId: i.entityId,
                                entityType: i.entityType,
                                entityActionEabId: n,
                                entityActionType: h.c.ENTITY_ACTION_TYPE.BROWSE
                            },
                            collectionData: {
                                collectionId: g.a.USERINTERACTION_COLLECTION_ID,
                                collectionItemIndex: t,
                                collectionIndex: 0,
                                collectionSource: h.c.COLLECTION_SOURCE.SEARCH
                            }
                        };
                        Object(b.trackUserInteraction)(r)
                    }
                }
            }, {
                key: "trackTealiumEvent",
                value: function() {
                    var e = this.props.metricsInfo
                      , t = Object(w.d)(e.entityType, e.entityName);
                    Object(b.trackTealium)(t)
                }
            }]),
            t
        }(d.Component);
        S.defaultProps = {
            imageUrl: "",
            highlightText: [],
            displayType: "",
            href: "#"
        };
        var E = S
          , k = n("Vg22")
          , C = n("nYuY");
        function N(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        t.a = Object(k.connect)((function(e, t) {
            var n = t.asset
              , r = t.index
              , o = {};
            o.title = i.n(n),
            o.displayType = i.d(n),
            o.highlightText = i.e(n),
            o.imageUrl = i.h(n),
            o.id = i.g(n),
            o.brand = i.b(n),
            o.name = i.k(n),
            o.eabId = i.l(n);
            var c = i.r(n)
              , l = i.t(n);
            l ? (o.upsellPackageId = c,
            o.href = Object(I.d)({
                packageId: c,
                source: I.a.search,
                referringEntityId: i.g(n),
                detailUrl: i.q(n)
            })) : o.href = i.f(n),
            o.isUpsell = l,
            o.upsellActionText = i.p(n),
            o.metricsInfo = i.j(n),
            o.discoverHref = i.c(n);
            var s = e[y.d];
            return function(e) {
                for (var t = 1; t < arguments.length; t++) {
                    var n = null != arguments[t] ? arguments[t] : {};
                    t % 2 ? N(Object(n), !0).forEach((function(t) {
                        Object(a.a)(e, t, n[t])
                    }
                    )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : N(Object(n)).forEach((function(t) {
                        Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                    }
                    ))
                }
                return e
            }({}, t, {}, o, {
                index: r,
                selectIndex: s.selectIndex,
                clickIndex: s.clickIndex,
                isMouseEnabled: s.isMouseEnabled,
                selectionTrackingId: n.selectionTrackingId
            })
        }
        ), (function(e) {
            return {
                addRecentSearch: function(t) {
                    e(Object(C.a)(t))
                },
                trackClick: function(t) {
                    e(Object(g.h)(g.f.METRICS_CLICK, t))
                },
                setSelectIndex: function(t) {
                    e(Object(C.q)(t))
                },
                clearClick: function() {
                    e(Object(C.b)())
                }
            }
        }
        ))(E)
    },
    P160: function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.stopVideo = t.startVideo = t.switchMode = t.updateSiteApi = t.updateCommonContext = t.resolveInitPromise = void 0;
        let a = null
          , i = {}
          , r = {};
        const o = {
            promise: null,
            resolve: null
        };
        o.promise = new Promise(e=>o.resolve = e);
        t.resolveInitPromise = e=>{
            console.log("resolveInitPromise... "+ JSON.stringify(e));
            a = e,
            o.resolve()
        }
        ;
        t.updateCommonContext = e=>{
            console.log("updateCommonContext... ");
            Object.assign(r, e)
        }
        ;
        t.updateSiteApi = e=>{
            console.log("updateSiteApi... ");
            o.promise.then(()=>{
                a.setSiteApi(e)
            }
            )
        }
        ;
        t.switchMode = e=>{
            console.log("switchMode... ");
            o.promise.then(()=>{
                a.switchMode(e)
            }
            )
        }
        ;
        t.startVideo = (e,t,n)=>{
            console.log("startVideo... ");
            console.log("e=" + JSON.stringify(e));
            console.log("t=" + JSON.stringify(t));
            console.log("n=" + JSON.stringify(n));
            this.props = e;
            this.profile = t;
            this.source = n;
            c();
            let l = i;
            o.promise.then(()=>{
                l.canceled || a.playVideo(e, Object.assign({}, r, t, n))
            }
            )
        }
        ;
        t.startVideo2 = ()=>{
            console.log("startVideo2... ");
            e = this.props;
            t = this.profile;
            n = this.source;
            console.log("e=" + JSON.stringify(e));
            console.log("t=" + JSON.stringify(t));
            console.log("n=" + JSON.stringify(n));
            c();
            let l = i;
            o.promise.then(()=>{
                l.canceled || a.playVideo(e, Object.assign({}, r, t, n))
            }
            )
        }
        ;
        const c = ()=>{
            console.log("stopVideo... ");
            //return; // never stop this game
            i.canceled = !0,
            i = {},
            a && a.stopVideo()
        }
        ;
        t.stopVideo = c
    },
    Pn51: function(e, t, n) {
        "use strict";
        n.d(t, "a", (function() {
            return p
        }
        ));
        var a = n("Vg22")
          , i = n("nYuY")
          , r = n("p3iw")
          , o = n("q1tI")
          , c = n.n(o)
          , l = n("TSYQ")
          , s = n.n(l)
          , u = (n("+W/r"),
        n("J+Nd"))
          , d = c.a.createElement
          , f = function(e) {
            var t = e.keywords
              , n = e.selectIndex
              , a = e.userEmail
              , i = e.userType
              , o = e.didTapFullTextSearchEntry
              , c = e.onHover
              , l = s()("FullTextSearch__Entry", "cu-search-button", {
                "FullTextSearch__Entry--Selected": -1 === n.index && n.type === r.a.ENTITY
            });
            return d("button", {
                tabIndex: "-1",
                className: l,
                onClick: function() {
                    o(t, a, i)
                },
                onMouseEnter: c
            }, d("span", {
                className: "FullTextSearch__SearchIcon"
            }), d("span", null, d(u.view, {
                from: "search.fullTextSearch.searchFor",
                isHTML: !0,
                replace: {
                    KEYWORDS: t
                }
            })))
        }
          , m = n("kvot")
          , p = Object(a.connect)((function(e) {
            var t = e[r.d]
              , n = Object(m.b)(e);
            return {
                keywords: t.keywords,
                selectIndex: t.selectIndex,
                userEmail: n.userEmail,
                userType: n.userType
            }
        }
        ), (function(e) {
            return {
                didTapFullTextSearchEntry: function(t, n, a) {
                    e(Object(i.o)(t, n, a))
                },
                onHover: function() {
                    e(Object(i.q)({
                        type: r.a.ENTITY,
                        index: -1
                    }))
                }
            }
        }
        ))(f)
          , h = n("1OyB")
          , b = n("vuIU")
          , v = n("md7G")
          , g = n("foSv")
          , y = n("JX7q")
          , O = n("Ji7U")
          , _ = n("yV38")
          , j = n("m4ty")
          , I = n("SgFH")
          , w = n("eMJF")
          , T = c.a.createElement
          , S = function(e) {
            function t(e) {
                var n;
                return Object(h.a)(this, t),
                (n = Object(v.a)(this, Object(g.a)(t).call(this, e))).getSearchResultView = n.getSearchResultView.bind(Object(y.a)(n)),
                n.getNoResultView = n.getNoResultView.bind(Object(y.a)(n)),
                n
            }
            return Object(O.a)(t, e),
            Object(b.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.groups
                      , n = e.offsiteItems;
                    if (0 === t.length && 0 === n.length)
                        return null;
                    var a = {
                        collectionIndex: 0,
                        collectionName: I.COLLECTION_NAME.OFFISTE,
                        pageName: I.SEARCH_PAGE_NAME.FULL_TEXT
                    };
                    return T("div", {
                        className: "FullTextSearch"
                    }, t.length > 0 ? this.getSearchResultView() : this.getNoResultView(), T(j.a, {
                        list: n,
                        collectionMetricsParams: a
                    }))
                }
            }, {
                key: "getSearchResultView",
                value: function() {
                    return this.props.groups.map((function(e, t) {
                        return T(_.a, {
                            collection: e,
                            key: e.type,
                            requireDetailPage: e.type === r.c,
                            collectionMetricsParams: {
                                collectionName: e.type,
                                collectionIndex: t,
                                pageName: I.SEARCH_PAGE_NAME.FULL_TEXT
                            }
                        })
                    }
                    ))
                }
            }, {
                key: "getNoResultView",
                value: function() {
                    return T(w.a, null)
                }
            }]),
            t
        }(o.Component);
        S.defaultProps = {
            groups: [],
            offsiteItems: []
        };
        var E = S;
        t.b = E
    },
    RNiq: function(e, t, n) {
        "use strict";
        n.r(t);
        var a = n("o0o1")
          , i = n.n(a)
          , r = n("1OyB")
          , o = n("vuIU")
          , c = n("md7G")
          , l = n("foSv")
          , s = n("Ji7U")
          , u = n("q1tI")
          , d = n.n(u)
          , f = n("nOHt")
          , m = n.n(f)
          , p = n("t+qN")
          , h = n("b518")
          , b = n("FZ4G")
          , v = n("X/Cc")
          , g = n("Vg22")
          , y = n("CVsM")
          , O = n.n(y)
          , _ = n("Scth")
          , j = n.n(_)
          , I = d.a.createElement
          , w = function(e) {
            var t = e.bannerKey;
            return I(O.a, {
                bannerKey: t,
                onError: function(e) {
                    j.a.error("fail to initialize the AppsFlyer SDK script due to error ".concat(e))
                }
            })
        }
          , T = n("KYkD")
          , S = n("azIb")
          , E = n("RBFW")
          , k = n("qE4w")
          , C = n("rePB")
          , N = n("Ff2n")
          , P = n("TEMv")
          , D = n("JX7q")
          , x = n("wx14")
          , L = n("mwIZ")
          , R = n.n(L)
          , M = n("bNQv")
          , A = n.n(M)
          , G = n("TSYQ")
          , B = n.n(G)
          , F = n("inWk")
          , H = n("7Lze")
          , U = n("ZoaF")
          , V = n("A/H/")
          , z = n("ODXe")
          , W = n("cH2P")
          , Y = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    refreshId: null,
                    idleId: null,
                    idle: !1,
                    pageX: null,
                    pageY: null,
                    lastEventTime: null
                }),
                Object(C.a)(Object(D.a)(n), "_idle", (function() {
                    n._clearTimeouts(),
                    n.setState({
                        idle: !0
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "_handleEvent", (function(e) {
                    var t = n.props
                      , a = t.eventThrottleMillis
                      , i = t.refresh
                      , r = n.state
                      , o = r.pageX
                      , c = r.pageY
                      , l = r.idle
                      , s = r.lastEventTime
                      , u = +Date.now();
                    if ("mousemove" === e.type) {
                        if ("undefined" === typeof e.pageX || "undefined" === typeof e.pageY)
                            return;
                        if (e.pageX === o && e.pageY === c)
                            return;
                        if (u - s < a)
                            return
                    }
                    l && i(),
                    n._reset(),
                    n.setState({
                        pageX: e.pageX,
                        pageY: e.pageY,
                        idle: !1,
                        lastEventTime: u
                    })
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentWillMount",
                value: function() {
                    this.setState({
                        lastEventTime: +Date.now()
                    }),
                    this._bindEvents()
                }
            }, {
                key: "componentDidMount",
                value: function() {
                    this._reset()
                }
            }, {
                key: "componentDidUpdate",
                value: function(e) {
                    e !== this.props && (this._unbindEvents(),
                    this._reset(),
                    this._bindEvents())
                }
            }, {
                key: "componentWillUnmount",
                value: function() {
                    this._unbindEvents()
                }
            }, {
                key: "render",
                value: function() {
                    return null
                }
            }, {
                key: "_isEnabled",
                value: function() {
                    var e = this.props
                      , t = e.refreshIntervalSeconds
                      , n = e.enabled;
                    return t > 0 && n
                }
            }, {
                key: "_updateEventBinding",
                value: function(e, t, n, a) {
                    var i = [].concat(a)
                      , r = Object(z.a)(i, 2)
                      , o = r[0]
                      , c = r[1];
                    t.call(e, o, n, c)
                }
            }, {
                key: "_bindEvents",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.element
                      , a = t.events;
                    this._isEnabled() && n && a.forEach((function(t) {
                        return e._updateEventBinding(n, n.addEventListener, e._handleEvent, t)
                    }
                    ))
                }
            }, {
                key: "_unbindEvents",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.element
                      , a = t.events;
                    n && a.forEach((function(t) {
                        return e._updateEventBinding(n, n.removeEventListener, e._handleEvent, t)
                    }
                    ))
                }
            }, {
                key: "_reset",
                value: function() {
                    var e = this.props
                      , t = e.refreshIntervalSeconds
                      , n = e.idleTimeoutSeconds
                      , a = e.refresh;
                    this._clearTimeouts(),
                    this._isEnabled() && this.setState({
                        refreshId: setInterval(a, 1e3 * t),
                        idleId: setTimeout(this._idle, 1e3 * n)
                    })
                }
            }, {
                key: "_clearTimeouts",
                value: function() {
                    var e = this.state
                      , t = e.refreshId
                      , n = e.idleId;
                    clearInterval(t),
                    clearTimeout(n),
                    this.setState({
                        refreshId: null,
                        idleId: null
                    })
                }
            }]),
            t
        }(d.a.Component);
        Y.defaultProps = {
            element: Object(W.a)() ? document : null,
            events: ["mousemove", "keydown", ["wheel", {
                passive: !0
            }], "DOMMouseScroll", "mouseWheel", "mousedown", "touchstart", "touchmove", "MSPointerDown", "MSPointerMove"],
            refreshIntervalSeconds: 0,
            idleTimeoutSeconds: 0,
            eventThrottleMillis: 200,
            enabled: !0
        };
        var K = Y
          , q = n("IJXi")
          , J = n("peh1")
          , X = n("eWwf")
          , Z = n("HOv+")
          , Q = Object(J.createSelector)([Z.a], (function(e) {
            return {
                enabled: e !== X.b
            }
        }
        ))
          , $ = Object(P.a)(Object(q.a)("auto_refresh"), Object(g.connect)(Q))(K)
          , ee = n("Z8Nq")
          , te = n("IH8g")
          , ne = n("yrxK")
          , ae = n("/r3/")
          , ie = n("VYLy")
          , re = (n("TuZ4"),
        n("qzj+"))
          , oe = n("tK7F")
          , ce = d.a.createElement
          , le = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    fetchedAdditionalCollections: !1
                }),
                Object(C.a)(Object(D.a)(n), "onEnter", (function() {
                    var e = n.props
                      , t = e.fetchCollection
                      , a = e.collections
                      , i = e.cacheKey;
                    n.state.fetchedAdditionalCollections || (A()(a.slice(ne.a), (function(e) {
                        var n = R()(e, ["pagination", "next"], null)
                          , a = Object(ie.a)(n, {
                            cacheKey: i
                        });
                        t(a)
                    }
                    )),
                    n.setState({
                        fetchedAdditionalCollections: !0
                    }))
                }
                )),
                Object(C.a)(Object(D.a)(n), "setHalfwayTrigger", (function(e) {
                    n.hubLoadTriggerHalfway = e
                }
                )),
                Object(C.a)(Object(D.a)(n), "setBottomTrigger", (function(e) {
                    n.hubLoadTriggerBottom = e
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    !this.observer && this.hubLoadTriggerHalfway && this.hubLoadTriggerBottom && (this.observer = new V.a,
                    this.observer.add(1, this.hubLoadTriggerHalfway, this.onEnter, null),
                    this.observer.add(1, this.hubLoadTriggerBottom, this.onEnter, null))
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.refresh
                      , n = e.collections
                      , a = e.includeTitle
                      , i = e.title
                      , r = e.stateKey
                      , o = e.pageType
                      , c = e.metricsInfo
                      , l = e.isTopLevel
                      , s = this.state.fetchedAdditionalCollections;
                    return ce("div", {
                        className: "Hub"
                    }, a && i && ce(u.Fragment, null, ce(H.a, {
                        className: "Hub__navigation"
                    }, ce("h1", {
                        className: "Hub__title"
                    }, i))), ce(oe.a, {
                        title: "".concat(S.i, " | ").concat(i),
                        useOverrideTitle: l && a && i
                    }), ce(H.a, {
                        className: B()("Hub__collections", Object(re.a)("hub-collections", i))
                    }, ce(E.default, null, (function(e) {
                        return n.map((function(t, n) {
                            return function(e, t, n, a, i, r) {
                                var o = t.id
                                  , c = {
                                    collection: t,
                                    collectionIndex: e,
                                    artwork: Object(F.d)(t),
                                    pageType: a,
                                    stateKey: n
                                }
                                  , l = {
                                    onEnter: te.a,
                                    waitFor: 250,
                                    threshold: .5,
                                    metricsParams: Object(ee.a)(o, e, n, i)
                                }
                                  , s = Object(ae.a)(t, r);
                                return s ? ce(U.a, Object(x.a)({
                                    className: "Hub__collection",
                                    key: o
                                }, l), ce(s, Object(x.a)({}, c, {
                                    metricsInfo: i
                                }))) : null
                            }(n, t, r, o, c, e)
                        }
                        ))
                    }
                    ))), ce($, {
                        refresh: t
                    }), !s && ce("div", {
                        ref: this.setHalfwayTrigger,
                        className: "Hub__halfwayLoadTrigger"
                    }), !s && ce("div", {
                        ref: this.setBottomTrigger,
                        className: "Hub__bottomLoadTrigger"
                    }))
                }
            }]),
            t
        }(u.Component);
        le.defaultProps = {
            collections: [],
            includeTitle: !0
        };
        var se = le
          , ue = n("Aqu8")
          , de = n("FYOk")
          , fe = n("gKAa")
          , me = []
          , pe = Object(J.createSelector)([function(e) {
            return R()(e[v.b], "collections", me)
        }
        , fe.a, function(e) {
            return e[v.b].heimdallEntity
        }
        , function(e) {
            return e[v.b].name
        }
        , function(e) {
            return e[v.b].pageType
        }
        , function(e) {
            return e[v.b].dataReady
        }
        , function(e) {
            return e[v.b].metrics_info
        }
        ], (function(e, t, n, a, i, r, o) {
            return {
                dataReady: r,
                collections: e,
                includeTitle: (t.hub_config[n] || {}).includeTitle,
                title: a,
                stateKey: v.b,
                pageType: i,
                metricsInfo: o
            }
        }
        ));
        function he(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        var be = Object(P.a)(Object(g.connect)(pe, {
            initContent: b.initContent,
            setIsLoadingData: b.setIsLoadingData,
            initTileStates: b.appendTileStates,
            refresh: b.refresh,
            fetchCollection: b.fetchCollection
        }), Object(de.a)((function(e) {
            var t = e.heimdallEntity
              , n = e.heimdallEntityId
              , a = e.discoverHref;
            return function(e) {
                for (var t = 1; t < arguments.length; t++) {
                    var n = null != arguments[t] ? arguments[t] : {};
                    t % 2 ? he(Object(n), !0).forEach((function(t) {
                        Object(C.a)(e, t, n[t])
                    }
                    )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : he(Object(n)).forEach((function(t) {
                        Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                    }
                    ))
                }
                return e
            }({}, Object(N.a)(e, ["heimdallEntity", "heimdallEntityId", "discoverHref"]), {
                url: a ? ne.b.browseUrlWithDiscoverHref({
                    entity: t,
                    href: a
                }) : ne.b.default({
                    entity: t,
                    id: n
                })
            })
        }
        )), ue.a)(se)
          , ve = (n("OFe8"),
        d.a.createElement);
        function ge(e) {
            var t = e.index
              , n = "HubLoadingSkeleton__loading--".concat(t)
              , a = "HubLoadingSkeleton__regularRectangle ".concat(n)
              , i = "HubLoadingSkeleton__titleLine ".concat(n)
              , r = "HubLoadingSkeleton__subtitleLine ".concat(n);
            return ve("div", {
                className: "HubLoadingSkeleton__regularBlocks"
            }, ve("div", {
                className: a
            }, ve("div", {
                className: "HubLoadingSkeleton__regularDummy"
            }, ve("div", {
                className: "HubLoadingSkeleton__regularContent"
            }))), ve("div", {
                className: "HubLoadingSkeleton__text"
            }, ve("div", {
                className: r
            }), ve("div", {
                className: i
            })))
        }
        function ye(e) {
            var t = e.index
              , n = "HubLoadingSkeleton__portraitRectangle HubLoadingSkeleton__loading--".concat(t);
            return ve("div", {
                className: n
            }, ve("div", {
                className: "HubLoadingSkeleton__portraitDummy"
            }, ve("div", {
                className: "HubLoadingSkeleton__portraitContent"
            })))
        }
        var Oe = ve("div", {
            className: "HubLoadingSkeleton__portrait",
            "data-testid": "hub-loading-skeleton-portrait-collection"
        }, ve("div", {
            className: "HubLoadingSkeleton__portraitTitle"
        }), ve("div", {
            className: "HubLoadingSkeleton__portraitCollection"
        }, ve(ye, {
            index: 0
        }), ve(ye, {
            index: 1
        }), ve(ye, {
            index: 2
        }), ve(ye, {
            index: 3
        }), ve(ye, {
            index: 4
        }), ve(ye, {
            index: 5
        })))
          , _e = ve("div", {
            className: "HubLoadingSkeleton__simple",
            "data-testid": "hub-loading-skeleton-landscape-collection"
        }, ve("div", {
            className: "HubLoadingSkeleton__simpleTitle"
        }), ve("div", {
            className: "HubLoadingSkeleton__simpleCollection"
        }, ve(ge, {
            index: 0
        }), ve(ge, {
            index: 1
        }), ve(ge, {
            index: 2
        }), ve(ge, {
            index: 3
        }), ve(ge, {
            index: 4
        }), ve(ge, {
            index: 5
        }), ve(ge, {
            index: 6
        })))
          , je = function(e) {
            var t = e.isHomeHub
              , n = void 0 !== t && t;
            return ve(H.a, null, ve("div", {
                "data-testid": "hub-loading-skeleton",
                className: "HubLoadingSkeleton"
            }, n ? Oe : _e, _e, _e))
        }
          , Ie = n("UB5X")
          , we = n.n(Ie)
          , Te = n("5nWE")
          , Se = function(e, t, n, a) {
            var i = function() {
                return Math.floor(Date.now() / 1e3)
            }
              , r = Object(u.useState)({
                lastDismissalTime: null,
                dismissalCount: 0,
                initialAppOpenTime: null
            })
              , o = r[0]
              , c = o.lastDismissalTime
              , l = o.dismissalCount
              , s = o.initialAppOpenTime
              , d = r[1];
            return Object(u.useEffect)((function() {
                if (Te.a.storageExists()) {
                    var e = Te.a.get(a) || {};
                    !e.initialAppOpenTime && we()(n) && (e.initialAppOpenTime = i(),
                    Te.a.set(a, e)),
                    d({
                        lastDismissalTime: e.lastDismissalTime || 0,
                        dismissalCount: e.dismissalCount || 0,
                        initialAppOpenTime: e.initialAppOpenTime
                    })
                }
            }
            ), [a, n]),
            [null === c || l >= t || c + 86400 * e > i() || we()(n) && s + 86400 * n > i(), function() {
                var e = {
                    lastDismissalTime: i(),
                    dismissalCount: l + 1,
                    initialAppOpenTime: s
                };
                Te.a.set(a, e),
                d(e)
            }
            , function() {
                var e = {
                    lastDismissalTime: i(),
                    dismissalCount: t,
                    initialAppOpenTime: s
                };
                Te.a.set(a, e),
                d(e)
            }
            ]
        }
          , Ee = n("J+Nd")
          , ke = n("GfZ+")
          , Ce = n("rCRB")
          , Ne = n("+vIg")
          , Pe = n("OPC9")
          , De = (n("nT/b"),
        d.a.createElement)
          , xe = function(e) {
            var t = e.title
              , n = e.body
              , a = e.ctaCopy
              , i = e.ctaAction
              , r = e.dismissAction
              , o = e.ctaUserInteraction
              , c = e.dismissUserInteraction;
            return De(Pe.a, {
                className: "PwaActionModal",
                role: "dialog",
                "aria-labelledby": "PwaActionModal__title"
            }, De(Ce.a, {
                clickCancel: r,
                userInteractionData: c
            }), De("div", {
                className: "PwaActionModal__body"
            }, De("img", {
                className: "PwaActionModal__logo",
                src: "//assetshuluimcom-a.akamaihd.net/h3o/browse/HuluLogo_192x192.png",
                role: "presentation"
            }), De("div", {
                id: "PwaActionModal__title",
                className: "PwaActionModal__title"
            }, t), De("p", null, n), De(Ne.a, {
                buttonStyle: "cta",
                className: "PwaActionModal__install",
                onClick: i,
                userInteractionData: o
            }, a)))
        };
        xe.defaultProps = {
            daysUntilDismissalExpires: 0,
            maxDismissalCount: 0
        };
        var Le = xe
          , Re = (n("cuP3"),
        d.a.createElement)
          , Me = {
            PROMPT_DISMISS: {
                actionSpecifier: "add_to_hs:dismiss",
                displayName: "Not now",
                elementSpecifier: "add_to_hs:not_now_btn"
            },
            SHOW_NATIVE: {
                actionSpecifier: "add_to_hs:show_native",
                displayName: "Add",
                elementSpecifier: "add_to_hs:add_btn"
            },
            NATIVE_INSTALL: {
                actionSpecifier: "add_to_hs:native_install",
                displayName: "Install",
                elementSpecifier: "add_to_hs:install_btn"
            },
            NATIVE_DISMISS: {
                actionSpecifier: "add_to_hs:native_dismiss",
                displayName: "Cancel",
                elementSpecifier: "add_to_hs:cancel_btn"
            }
        }
          , Ae = function(e) {
            var t = e.daysUntilDismissalExpires
              , n = e.maxDismissalCount
              , a = Se(t, n, null, "_hulu_a2hs_banner")
              , r = Object(z.a)(a, 2)
              , o = r[0]
              , c = r[1]
              , l = function(e) {
                var t = Object(u.useState)(!1)
                  , n = t[0]
                  , a = t[1]
                  , r = Object(u.useRef)();
                Object(u.useEffect)((function() {
                    var e = function(e) {
                        e.preventDefault(),
                        Object(ke.trackTealium)({
                            event_name: "app_install_shown"
                        }),
                        r.current = e,
                        a(!0)
                    };
                    return window.addEventListener("beforeinstallprompt", e),
                    function() {
                        return window.removeEventListener("beforeinstallprompt", e)
                    }
                }
                ), []);
                return [n, function() {
                    var t;
                    return i.a.async((function(n) {
                        for (; ; )
                            switch (n.prev = n.next) {
                            case 0:
                                return Object(p.c)("/app/?utm_source=a2hs"),
                                r.current.prompt(),
                                n.next = 4,
                                i.a.awrap(r.current.userChoice);
                            case 4:
                                t = n.sent,
                                r.current = null,
                                e(),
                                Object(ke.trackTealium)({
                                    event_name: "app_install_outcome",
                                    outcome: t.outcome
                                }),
                                Object(ke.trackUserInteraction)("accepted" === t.outcome ? Me.NATIVE_INSTALL : Me.NATIVE_DISMISS);
                            case 9:
                            case "end":
                                return n.stop()
                            }
                    }
                    ))
                }
                ]
            }(c)
              , s = Object(z.a)(l, 2)
              , d = s[0]
              , f = s[1];
            return d && !o ? Re(Le, {
                title: Re(Ee.view, {
                    from: "installBanner.title"
                }),
                body: Re(Ee.view, {
                    from: "installBanner.body"
                }),
                ctaCopy: Re(Ee.view, {
                    from: "installBanner.add"
                }),
                ctaAction: f,
                dismissAction: c,
                ctaUserInteraction: Me.SHOW_NATIVE,
                dismissUserInteraction: Me.PROMPT_DISMISS
            }) : null
        };
        Ae.defaultProps = {
            daysUntilDismissalExpires: 0,
            maxDismissalCount: 0
        };
        var Ge = Object(q.a)("install_pwa_prompt")(Object(u.memo)(Ae))
          , Be = n("g/JB")
          , Fe = n("ArxA")
          , He = function(e) {
            var t = document.activeElement;
            switch (e.key) {
            case S.d.ARROW_RIGHT:
                e.preventDefault(),
                t.nextSibling && t.nextSibling.focus();
                break;
            case S.d.ARROW_LEFT:
                e.preventDefault(),
                t.previousSibling && t.previousSibling.focus()
            }
        }
          , Ue = n("BN2M")
          , Ve = n("9moD")
          , ze = n("i8i4")
          , We = n.n(ze)
          , Ye = n("iR1w");
        function Ke(e) {
            var t;
            t = "undefined" !== typeof window ? window : "undefined" !== typeof self ? self : global;
            var n = "undefined" !== typeof document && document.attachEvent;
            if (!n) {
                var a = function() {
                    var e = t.requestAnimationFrame || t.mozRequestAnimationFrame || t.webkitRequestAnimationFrame || function(e) {
                        return t.setTimeout(e, 20)
                    }
                    ;
                    return function(t) {
                        return e(t)
                    }
                }()
                  , i = function() {
                    var e = t.cancelAnimationFrame || t.mozCancelAnimationFrame || t.webkitCancelAnimationFrame || t.clearTimeout;
                    return function(t) {
                        return e(t)
                    }
                }()
                  , r = function(e) {
                    var t = e.__resizeTriggers__
                      , n = t.firstElementChild
                      , a = t.lastElementChild
                      , i = n.firstElementChild;
                    a.scrollLeft = a.scrollWidth,
                    a.scrollTop = a.scrollHeight,
                    i.style.width = n.offsetWidth + 1 + "px",
                    i.style.height = n.offsetHeight + 1 + "px",
                    n.scrollLeft = n.scrollWidth,
                    n.scrollTop = n.scrollHeight
                }
                  , o = function(e) {
                    if (!(e.target.className.indexOf("contract-trigger") < 0 && e.target.className.indexOf("expand-trigger") < 0)) {
                        var t = this;
                        r(this),
                        this.__resizeRAF__ && i(this.__resizeRAF__),
                        this.__resizeRAF__ = a((function() {
                            (function(e) {
                                return e.offsetWidth != e.__resizeLast__.width || e.offsetHeight != e.__resizeLast__.height
                            }
                            )(t) && (t.__resizeLast__.width = t.offsetWidth,
                            t.__resizeLast__.height = t.offsetHeight,
                            t.__resizeListeners__.forEach((function(n) {
                                n.call(t, e)
                            }
                            )))
                        }
                        ))
                    }
                }
                  , c = !1
                  , l = ""
                  , s = "animationstart"
                  , u = "Webkit Moz O ms".split(" ")
                  , d = "webkitAnimationStart animationstart oAnimationStart MSAnimationStart".split(" ")
                  , f = document.createElement("fakeelement");
                if (void 0 !== f.style.animationName && (c = !0),
                !1 === c)
                    for (var m = 0; m < u.length; m++)
                        if (void 0 !== f.style[u[m] + "AnimationName"]) {
                            l = "-" + u[m].toLowerCase() + "-",
                            s = d[m],
                            c = !0;
                            break
                        }
                var p = "resizeanim"
                  , h = "@" + l + "keyframes " + p + " { from { opacity: 0; } to { opacity: 0; } } "
                  , b = l + "animation: 1ms " + p + "; "
            }
            return {
                addResizeListener: function(a, i) {
                    if (n)
                        a.attachEvent("onresize", i);
                    else {
                        if (!a.__resizeTriggers__) {
                            var c = a.ownerDocument
                              , l = t.getComputedStyle(a);
                            l && "static" == l.position && (a.style.position = "relative"),
                            function(t) {
                                if (!t.getElementById("detectElementResize")) {
                                    var n = (h || "") + ".resize-triggers { " + (b || "") + 'visibility: hidden; opacity: 0; } .resize-triggers, .resize-triggers > div, .contract-trigger:before { content: " "; display: block; position: absolute; top: 0; left: 0; height: 100%; width: 100%; overflow: hidden; z-index: -1; } .resize-triggers > div { background: #eee; overflow: auto; } .contract-trigger:before { width: 200%; height: 200%; }'
                                      , a = t.head || t.getElementsByTagName("head")[0]
                                      , i = t.createElement("style");
                                    i.id = "detectElementResize",
                                    i.type = "text/css",
                                    null != e && i.setAttribute("nonce", e),
                                    i.styleSheet ? i.styleSheet.cssText = n : i.appendChild(t.createTextNode(n)),
                                    a.appendChild(i)
                                }
                            }(c),
                            a.__resizeLast__ = {},
                            a.__resizeListeners__ = [],
                            (a.__resizeTriggers__ = c.createElement("div")).className = "resize-triggers",
                            a.__resizeTriggers__.innerHTML = '<div class="expand-trigger"><div></div></div><div class="contract-trigger"></div>',
                            a.appendChild(a.__resizeTriggers__),
                            r(a),
                            a.addEventListener("scroll", o, !0),
                            s && (a.__resizeTriggers__.__animationListener__ = function(e) {
                                e.animationName == p && r(a)
                            }
                            ,
                            a.__resizeTriggers__.addEventListener(s, a.__resizeTriggers__.__animationListener__))
                        }
                        a.__resizeListeners__.push(i)
                    }
                },
                removeResizeListener: function(e, t) {
                    if (n)
                        e.detachEvent("onresize", t);
                    else if (e.__resizeListeners__.splice(e.__resizeListeners__.indexOf(t), 1),
                    !e.__resizeListeners__.length) {
                        e.removeEventListener("scroll", o, !0),
                        e.__resizeTriggers__.__animationListener__ && (e.__resizeTriggers__.removeEventListener(s, e.__resizeTriggers__.__animationListener__),
                        e.__resizeTriggers__.__animationListener__ = null);
                        try {
                            e.__resizeTriggers__ = !e.removeChild(e.__resizeTriggers__)
                        } catch (a) {}
                    }
                }
            }
        }
        var qe = function(e, t) {
            if (!(e instanceof t))
                throw new TypeError("Cannot call a class as a function")
        }
          , Je = function() {
            function e(e, t) {
                for (var n = 0; n < t.length; n++) {
                    var a = t[n];
                    a.enumerable = a.enumerable || !1,
                    a.configurable = !0,
                    "value"in a && (a.writable = !0),
                    Object.defineProperty(e, a.key, a)
                }
            }
            return function(t, n, a) {
                return n && e(t.prototype, n),
                a && e(t, a),
                t
            }
        }()
          , Xe = Object.assign || function(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = arguments[t];
                for (var a in n)
                    Object.prototype.hasOwnProperty.call(n, a) && (e[a] = n[a])
            }
            return e
        }
          , Ze = function(e, t) {
            if (!e)
                throw new ReferenceError("this hasn't been initialised - super() hasn't been called");
            return !t || "object" !== typeof t && "function" !== typeof t ? e : t
        }
          , Qe = function(e) {
            function t() {
                var e, n, a;
                qe(this, t);
                for (var i = arguments.length, r = Array(i), o = 0; o < i; o++)
                    r[o] = arguments[o];
                return n = a = Ze(this, (e = t.__proto__ || Object.getPrototypeOf(t)).call.apply(e, [this].concat(r))),
                a.state = {
                    height: a.props.defaultHeight || 0,
                    width: a.props.defaultWidth || 0
                },
                a._onResize = function() {
                    var e = a.props
                      , t = e.disableHeight
                      , n = e.disableWidth
                      , i = e.onResize;
                    if (a._parentNode) {
                        var r = a._parentNode.offsetHeight || 0
                          , o = a._parentNode.offsetWidth || 0
                          , c = window.getComputedStyle(a._parentNode) || {}
                          , l = parseInt(c.paddingLeft, 10) || 0
                          , s = parseInt(c.paddingRight, 10) || 0
                          , u = parseInt(c.paddingTop, 10) || 0
                          , d = parseInt(c.paddingBottom, 10) || 0
                          , f = r - u - d
                          , m = o - l - s;
                        (!t && a.state.height !== f || !n && a.state.width !== m) && (a.setState({
                            height: r - u - d,
                            width: o - l - s
                        }),
                        i({
                            height: r,
                            width: o
                        }))
                    }
                }
                ,
                a._setRef = function(e) {
                    a._autoSizer = e
                }
                ,
                Ze(a, n)
            }
            return function(e, t) {
                if ("function" !== typeof t && null !== t)
                    throw new TypeError("Super expression must either be null or a function, not " + typeof t);
                e.prototype = Object.create(t && t.prototype, {
                    constructor: {
                        value: e,
                        enumerable: !1,
                        writable: !0,
                        configurable: !0
                    }
                }),
                t && (Object.setPrototypeOf ? Object.setPrototypeOf(e, t) : e.__proto__ = t)
            }(t, e),
            Je(t, [{
                key: "componentDidMount",
                value: function() {
                    var e = this.props.nonce;
                    this._autoSizer && this._autoSizer.parentNode && this._autoSizer.parentNode.ownerDocument && this._autoSizer.parentNode.ownerDocument.defaultView && this._autoSizer.parentNode instanceof this._autoSizer.parentNode.ownerDocument.defaultView.HTMLElement && (this._parentNode = this._autoSizer.parentNode,
                    this._detectElementResize = Ke(e),
                    this._detectElementResize.addResizeListener(this._parentNode, this._onResize),
                    this._onResize())
                }
            }, {
                key: "componentWillUnmount",
                value: function() {
                    this._detectElementResize && this._parentNode && this._detectElementResize.removeResizeListener(this._parentNode, this._onResize)
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.children
                      , n = e.className
                      , a = e.disableHeight
                      , i = e.disableWidth
                      , r = e.style
                      , o = this.state
                      , c = o.height
                      , l = o.width
                      , s = {
                        overflow: "visible"
                    }
                      , d = {}
                      , f = !1;
                    return a || (0 === c && (f = !0),
                    s.height = 0,
                    d.height = c),
                    i || (0 === l && (f = !0),
                    s.width = 0,
                    d.width = l),
                    Object(u.createElement)("div", {
                        className: n,
                        ref: this._setRef,
                        style: Xe({}, s, r)
                    }, !f && t(d))
                }
            }]),
            t
        }(u.PureComponent);
        Qe.defaultProps = {
            onResize: function() {},
            disableHeight: !1,
            disableWidth: !1,
            style: {}
        };
        var $e = Qe;
        function et(e, t) {
            if (e.length !== t.length)
                return !1;
            for (var n = 0; n < e.length; n++)
                if (e[n] !== t[n])
                    return !1;
            return !0
        }
        var tt = function(e, t) {
            var n;
            void 0 === t && (t = et);
            var a, i = [], r = !1;
            return function() {
                for (var o = arguments.length, c = new Array(o), l = 0; l < o; l++)
                    c[l] = arguments[l];
                return r && n === this && t(c, i) ? a : (a = e.apply(this, c),
                r = !0,
                n = this,
                i = c,
                a)
            }
        }
          , nt = n("DzJC")
          , at = n.n(nt)
          , it = n("sEfC")
          , rt = n.n(it)
          , ot = n("TYy9")
          , ct = n.n(ot)
          , lt = n("aVhw")
          , st = (n("SsCe"),
        d.a.createElement)
          , ut = function(e) {
            var t = e.width
              , n = e.children
              , a = e.text
              , i = Object(u.useState)(!1)
              , r = i[0]
              , o = i[1]
              , c = Object(u.useState)({})
              , l = c[0]
              , s = c[1]
              , d = Object(u.useState)(null)
              , f = d[0]
              , m = d[1]
              , p = Object(u.useRef)();
            Object(u.useEffect)((function() {
                m(document.getElementById("h3o-live-tooltip"))
            }
            ), []);
            return st("div", {
                onMouseEnter: function() {
                    var e = {}
                      , n = p.current.getBoundingClientRect();
                    e.left = n.left + n.width / 2 - t / 2,
                    e.bottom = window.innerHeight - n.top - window.scrollY,
                    o(!0),
                    s(e)
                },
                onMouseLeave: function() {
                    o(!1)
                },
                className: "LiveGuideTooltip__trigger",
                "data-testid": "liveguide-tooltip-trigger",
                ref: p
            }, n, f && Object(ze.createPortal)(st("div", {
                className: "LiveGuideTooltip__container LiveGuideTooltip__container--liveGuide",
                "data-testid": "liveguide-tooltip-container",
                style: l
            }, st("div", {
                className: B()("LiveGuideTooltip__tooltip", "LiveGuideTooltip__tooltip--liveguide", {
                    "LiveGuideTooltip__tooltip--visible": r
                }),
                "data-testid": "liveguide-tooltip"
            }, st("span", {
                className: "LiveGuideTooltip__tooltipText LiveGuideTooltip__tooltipText--liveguide",
                "data-testid": "tooltip-text"
            }, a))), f))
        };
        ut.defaultProps = {
            width: 80
        };
        var dt = ut
          , ft = (n("0Z1l"),
        n("U3+y"))
          , mt = n("I8km")
          , pt = d.a.createElement
          , ht = 80
          , bt = 80
          , vt = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "channelRef", Object(u.createRef)()),
                Object(C.a)(Object(D.a)(n), "renderChannelLogo", (function(e, t) {
                    var n = [{
                        trim: "(0,0,0,0)"
                    }, {
                        resize: "".concat(ht, "x").concat(bt, "|max")
                    }, {
                        extent: "".concat(ht, "x").concat(bt)
                    }, {
                        format: "png"
                    }];
                    return t ? pt("img", {
                        className: "LiveGuideChannel__logo",
                        src: "".concat(t, "&operations=").concat(JSON.stringify(n)),
                        role: "presentation",
                        alt: e
                    }) : null
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleMouseEnter", (function() {
                    n.props.setFocusOnChannel(0)
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleMouseOut", (function() {
                    n.props.setFocusOnChannel(null)
                }
                )),
                Object(C.a)(Object(D.a)(n), "onClick", (function() {
                    var e = n.props
                      , t = e.channel
                      , a = e.playChannel
                      , i = t.channelEab
                      , r = t.channelId;
                    i && a(i, S.m.LIVE, r)
                }
                )),
                Object(C.a)(Object(D.a)(n), "getUserInteractionData", (function(e) {
                    var t = n.props
                      , a = t.channel
                      , i = t.collectionId
                      , r = a.channelName
                      , o = a.channelEab
                      , c = Object(lt.c)(o);
                    return Object(ft.g)({
                        title: r,
                        eabId: o,
                        entityId: c,
                        metricsParams: {
                            customElementSpecifier: e,
                            collectionId: i
                        }
                    })
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidUpdate",
                value: function() {
                    this.props.isFocused && this.channelRef.current.focus()
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.channel
                      , n = e.channelIndex
                      , a = e.forceHover
                      , i = e.style
                      , r = e.canSeeFirstProgram
                      , o = e.isFocused
                      , c = e.shouldBeFocusable
                      , l = e.totalNumRows
                      , s = e.currentlyPlayingChannel
                      , u = t.channelName
                      , d = t.channelLogo
                      , f = t.channelEab === s
                      , m = R()(t, "liveAndUpcomingContent.0.availability.isBlackout", !1)
                      , p = R()(t, "liveAndUpcomingContent.0.availability.isOffAir", !1)
                      , h = B()("LiveGuideChannel", {
                        "LiveGuideChannel--hover": a || o,
                        "LiveGuideChannel--playing": f,
                        "LiveGuideChannel--off-air": p
                    })
                      , b = "liveGuide.toolTipWithNetwork";
                    m ? b = "liveGuide.toolTipBlackout" : p && (b = "liveGuide.toolTipOffAir");
                    var v = n + 1;
                    return pt("div", {
                        className: h,
                        onMouseEnter: this.handleMouseEnter,
                        onMouseLeave: this.handleMouseOut,
                        "data-testid": "live-guide-channel-".concat(u),
                        style: i
                    }, f && pt("span", {
                        className: "LiveGuideChannel__playing-indicator"
                    }), m && !r && pt("div", {
                        className: "LiveGuideChannel__blackout-scrim"
                    }), pt(dt, {
                        text: pt(Ee.view, {
                            from: b,
                            replace: {
                                NETWORK: u || "Live"
                            }
                        }),
                        width: ht
                    }, pt(Ne.a, {
                        ref: this.channelRef,
                        className: "LiveGuideChannel__logo-background",
                        onClick: this.onClick,
                        disabled: m || p,
                        tabIndex: o || c ? "0" : "-1",
                        "data-testid": "live-guide-channel-button",
                        userInteractionData: this.getUserInteractionData(mt.SPECIFIER_TYPE.LIVE_GUIDE_NETWORK_LOGO)
                    }, pt("span", {
                        className: "sr-only"
                    }, pt(Ee.view, {
                        from: b,
                        replace: {
                            NETWORK: u || "Live"
                        }
                    }), pt(Ee.view, {
                        from: "liveGuide.channelA11yButtonText",
                        replace: {
                            INDEX: v,
                            TOTAL_COUNT: l,
                            NETWORK: u
                        }
                    })), this.renderChannelLogo(u, d))))
                }
            }]),
            t
        }(u.Component)
          , gt = n("vN+2")
          , yt = n.n(gt)
          , Ot = d.a.createElement;
        var _t = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "listRef", d.a.createRef()),
                Object(C.a)(Object(D.a)(n), "scrollOffsetInitial", n.props.scrollTo),
                Object(C.a)(Object(D.a)(n), "scrollOffsetFinal", 0),
                Object(C.a)(Object(D.a)(n), "handleScroll", (function(e) {
                    e.stopPropagation()
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    this.props.scrollTo && this.listRef.current.scrollTo(this.props.scrollTo)
                }
            }, {
                key: "componentDidUpdate",
                value: function(e) {
                    e.scrollTo !== this.props.scrollTo && this.initAnimation(this.props.scrollTo)
                }
            }, {
                key: "animate",
                value: function() {
                    var e = this;
                    requestAnimationFrame((function() {
                        var t = e.props
                          , n = t.duration
                          , a = t.onAnimationEnd
                          , i = performance.now() - e.animationStartTime
                          , r = e.scrollOffsetFinal - e.scrollOffsetInitial
                          , o = function(e, t) {
                            var n = Math.min(1, e / t);
                            return n * (2 - n)
                        }(i, n)
                          , c = e.scrollOffsetInitial + r * o;
                        e.listRef.current && e.listRef.current.scrollTo(c),
                        i < n ? e.animate() : (e.animationStartTime = void 0,
                        e.scrollOffsetInitial = e.scrollOffsetFinal,
                        a())
                    }
                    ))
                }
            }, {
                key: "initAnimation",
                value: function(e) {
                    this.animationStartTime || (this.scrollOffsetFinal = e,
                    this.animationStartTime = performance.now(),
                    this.animate(),
                    this.props.onAnimationStart())
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.style
                      , n = e.itemCount
                      , a = e.getItemSize
                      , i = e.width
                      , r = e.height
                      , o = e.children
                      , c = e.itemData
                      , l = e.onItemsRendered;
                    return Ot("div", {
                        onScroll: this.handleScroll,
                        className: "AnimatedRow"
                    }, Ot(Ye.b, {
                        ref: this.listRef,
                        style: t,
                        direction: "horizontal",
                        height: r,
                        itemCount: n,
                        itemSize: a,
                        itemData: c,
                        width: i,
                        onItemsRendered: l
                    }, o))
                }
            }]),
            t
        }(u.Component);
        _t.defaultProps = {
            scrollTo: 0,
            duration: 300,
            onAnimationEnd: yt.a,
            onAnimationStart: yt.a
        };
        var jt = _t
          , It = (n("1m1m"),
        d.a.createElement)
          , wt = function(e) {
            return "LiveGuideBadge ".concat(e.map((function(e) {
                return "LiveGuideBadge--".concat(e)
            }
            )).join(" "))
        }
          , Tt = function(e) {
            var t = e.text
              , n = e.icon
              , a = e.iconLabel
              , i = e.modifiers;
            return It("div", {
                className: wt(void 0 === i ? [] : i)
            }, n ? It("div", {
                className: "LiveGuideBadge__icon"
            }, It("span", {
                className: "sr-only"
            }, It(Ee.view, {
                from: a
            })), n) : It("div", {
                className: "LiveGuideBadge__text"
            }, t))
        }
          , St = n("+WDr")
          , Et = (n("3QXk"),
        n("8h9a"))
          , kt = n("Gte0")
          , Ct = n("a65k")
          , Nt = n("C3VV")
          , Pt = n("8Wld")
          , Dt = d.a.createElement
          , xt = function(e) {
            var t = e.content
              , n = e.withHoverPopout
              , a = e.playChannel
              , i = e.onDismissTile
              , r = void 0 === i ? function() {}
            : i
              , o = e.collectionId
              , c = e.index
              , l = e.collectionIndex
              , s = t.id
              , d = t.title
              , f = t.eab
              , m = t.type
              , p = t.metadata
              , h = t.description
              , b = t.artwork
              , v = t.availability
              , g = void 0 === v ? {} : v
              , y = t.detailsUrl
              , O = t.hasStartoverRights
              , _ = t.time
              , j = t.timeRemaining
              , I = t.channelInfo
              , w = void 0 === I ? {} : I
              , T = g.airStartDate
              , E = g.isBlackout
              , k = w.id
              , C = Object(u.useCallback)((function(e, t) {
                r("item_selected"),
                t && a(e, S.m.LIVE, k)
            }
            ), [r, a, k])
              , N = Object(u.useCallback)((function(e, t, n) {
                switch (e) {
                case Pt.a.DETAIL:
                    r("item_selected");
                    break;
                case Pt.a.STARTOVER:
                    r("item_selected"),
                    n && a(t, S.m.RESTART, k)
                }
            }
            ), [r, a, k]);
            if (!s)
                return Dt(Et.b, {
                    isLoading: !0
                });
            var P = j ? Dt(Ee.view, {
                from: "liveGuide.timeWithRemaining",
                replace: {
                    TIME: _,
                    REMAINING: j
                }
            }) : Object(kt.d)(T)
              , D = Object(Ct.b)({
                availability: g,
                duration: null
            })
              , L = Object(Nt.c)(f, d, m, O, y, E)
              , M = Object(ft.a)({
                title: d,
                eabId: f,
                entityId: s,
                entityType: m,
                metricsParams: {
                    customElementSpecifier: mt.SPECIFIER_TYPE.LIVE_GUIDE_DETAILS,
                    collectionId: o
                }
            })
              , A = Object(ft.g)({
                title: d,
                eabId: f,
                entityId: s,
                entityType: m,
                metricsParams: {
                    customElementSpecifier: mt.SPECIFIER_TYPE.LIVE_GUIDE_WATCH_LIVE,
                    collectionId: o
                }
            })
              , G = R()(w, "logo_url")
              , B = R()(w, "name");
            return Dt(Et.b, Object(x.a)({
                withHoverPopout: n,
                title: d,
                description: h,
                thumbnail: b,
                availability: g,
                detailsUrl: y,
                metadata: p,
                isTitleClickable: !0,
                menuActions: L,
                eabId: f,
                action: P,
                onTitleClick: r,
                detailsUrlUserInteractionData: M,
                watchUrlUserInteractionData: A,
                onClick: C,
                onActionMenuItemClick: N,
                brandWatermark: G,
                brandName: B,
                useExtent: !0,
                collectionId: o,
                index: c,
                collectionIndex: l
            }, D))
        }
          , Lt = (n("Hvr1"),
        d.a.createElement)
          , Rt = function(e) {
            var t = e.content
              , n = e.channelName
              , a = e.playChannel
              , i = e.collectionId
              , r = e.toggleDetailModal;
            return Lt(Pe.a, {
                className: "LiveGuideDetailModal",
                closeAction: function() {
                    r("user_dismissed")
                },
                "data-testid": "detail-modal"
            }, Lt(Ce.a, {
                clickCancel: function() {
                    r("user_dismissed")
                },
                title: Lt("span", {
                    className: "LiveGuideDetailModal__title"
                }, n)
            }), Lt(xt, {
                content: t,
                withHoverPopout: !1,
                onDismissTile: r,
                playChannel: a,
                collectionId: i
            }))
        }
          , Mt = d.a.createElement
          , At = [St.a, St.j, St.i, St.f]
          , Gt = function(e, t) {
            if (!t)
                return e;
            var n = Math.floor(t / 60)
              , a = t % 60
              , i = e ? Mt("span", {
                className: "LiveGuideProgram__time-extra"
            }, "(", e, ")") : null;
            return Mt(Ee.view, {
                from: n > 0 ? "liveGuide.remainingHours" : "liveGuide.remainingMins",
                replace: {
                    EXTRA: i,
                    HOURS_REMAINING: n,
                    MINS_REMAINING: a
                }
            })
        }
          , Bt = function(e) {
            var t = Object(u.useState)(!1)
              , n = t[0]
              , a = t[1]
              , i = Object(u.useState)(0)
              , r = i[0]
              , o = i[1]
              , c = e.content
              , l = e.contentIndex
              , s = e.forceHover
              , d = e.hideContent
              , f = e.hideBorder
              , m = e.floating
              , p = e.isFocused
              , h = e.setFocusOnChannel
              , b = c.title
              , v = c.time
              , g = c.timeRemaining
              , y = c.duration
              , O = c.channelGridBadges
              , _ = void 0 === O ? [] : O
              , j = c.availability
              , I = void 0 === j ? {} : j
              , w = c.metadata
              , T = void 0 === w ? [] : w
              , E = I.isBlackout
              , k = I.isLive
              , C = I.isUpcoming
              , N = I.isOffAir
              , P = At.filter((function(e) {
                return _[e]
            }
            )).map((function(e) {
                return _[e]
            }
            ))
              , D = {
                style: {
                    "--duration": "".concat(y)
                },
                className: B()("LiveGuideProgram", {
                    "LiveGuideProgram--first": 0 === l,
                    "LiveGuideProgram--hide": y <= Be.h,
                    "LiveGuideProgram--hover": s || p,
                    "LiveGuideProgram--no-border": f,
                    "LiveGuideProgram--off-air": N
                })
            };
            0 === l && (D.onMouseEnter = function() {
                return h(l)
            }
            ,
            D.onMouseLeave = function() {
                return h(null)
            }
            );
            var L = Object(u.useRef)(null);
            Object(u.useEffect)((function() {
                p && !d && L.current.focus()
            }
            ), [p, d]);
            var M = N ? Mt(Ee.view, {
                from: "liveGuide.nothingToAir"
            }) : b || Mt(Ee.view, {
                from: "titleFallback"
            })
              , A = Gt(v, g)
              , G = function(e) {
                if (!N) {
                    if (n) {
                        var t = (new Date).getTime() - r;
                        Ue.d.trackLiveGuideModalClose({
                            scenario: e,
                            modalOpenDuration: t
                        })
                    } else
                        Ue.d.trackLiveGuideModalOpen({
                            selectedEab: R()(c, "eab")
                        }),
                        o((new Date).getTime());
                    a(!n)
                }
            };
            return Mt(u.Fragment, null, n && Mt(Rt, Object(x.a)({}, e, {
                toggleDetailModal: G
            })), Mt("div", Object(x.a)({}, D, {
                onClick: G,
                onKeyDown: function(e) {
                    switch (e.key) {
                    case S.d.SPACE:
                    case S.d.ENTER:
                        e.preventDefault(),
                        G()
                    }
                },
                "data-testid": m ? "live-guide-program-floating-".concat(b) : "live-guide-program-".concat(b),
                role: !N && "button",
                tabIndex: p && !d ? 0 : -1,
                ref: L
            }), !d && Mt(u.Fragment, null, E && Mt("div", {
                className: "LiveGuideProgram__blackout-scrim",
                "data-testid": "blackout-scrim"
            }), Mt("span", {
                className: "sr-only"
            }, k && Mt(Ee.view, {
                from: "liveGuide.onNow"
            }), C && Mt(Ee.view, {
                from: "liveGuide.upcoming"
            }), M, A), Mt("div", {
                className: "LiveGuideProgram__content-container"
            }, Mt("div", {
                className: "LiveGuideProgram__badges",
                "data-testid": "live-guide-badges"
            }, P.map((function(e) {
                var t = e.type
                  , n = e.text
                  , a = e.icon
                  , i = e.iconLabel
                  , r = e.classNames;
                return Mt(Tt, {
                    key: t,
                    text: n,
                    icon: a,
                    iconLabel: i,
                    modifiers: r
                })
            }
            ))), Mt("div", {
                className: "LiveGuideProgram__content"
            }, Mt("div", {
                className: "LiveGuideProgram__time",
                "data-testid": "live-guide-program-time",
                "aria-hidden": !0
            }, A), Mt("div", {
                className: "LiveGuideProgram__title",
                "aria-hidden": !0,
                "data-testid": "live-guide-program-title"
            }, M), Mt("div", {
                className: "LiveGuideProgram__description",
                "data-testid": "live-guide-program-description"
            }, T.map((function(e) {
                return Mt("span", {
                    key: e,
                    className: "LiveGuideProgram__description--segment"
                }, e)
            }
            ))))))))
        }
          , Ft = n("FKXr")
          , Ht = (n("g1lf"),
        d.a.createElement);
        function Ut(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function Vt(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = null != arguments[t] ? arguments[t] : {};
                t % 2 ? Ut(Object(n), !0).forEach((function(t) {
                    Object(C.a)(e, t, n[t])
                }
                )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : Ut(Object(n)).forEach((function(t) {
                    Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                }
                ))
            }
            return e
        }
        var zt = 75
          , Wt = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    hoverAtContentIndex: null,
                    firstVisibleColumnIndex: 0,
                    isScrolling: !1
                }),
                Object(C.a)(Object(D.a)(n), "shouldComponentUpdate", Ye.c.bind(Object(D.a)(n))),
                Object(C.a)(Object(D.a)(n), "setIsHovering", (function(e) {
                    n.setState({
                        hoverAtContentIndex: e
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "renderProgram", (function(e) {
                    var t = e.index
                      , a = e.style
                      , i = e.data
                      , r = n.props
                      , o = r.row
                      , c = r.playChannel
                      , l = r.durationIndexToFocus
                      , s = r.collectionId
                      , u = o.liveAndUpcomingContent[t]
                      , d = Ht("div", {
                        className: "LiveGuideChannelGrid__infinite-program-buffer",
                        "data-testid": "live-guide-program-buffer"
                    });
                    return u && (d = Ht(Bt, {
                        content: u,
                        contentIndex: t,
                        channelName: o.channelName,
                        forceHover: n.state.hoverAtContentIndex === t,
                        setFocusOnChannel: n.setIsHovering,
                        playChannel: c,
                        hideContent: i.firstVisibleColumnIndex === t,
                        hideBorder: i.hideBorder,
                        floating: i.floating,
                        isFocused: Object(Ft.f)(u, l),
                        collectionId: s
                    })),
                    Ht("div", {
                        style: a
                    }, d)
                }
                )),
                Object(C.a)(Object(D.a)(n), "getItemSize", (function(e) {
                    var t = n.props.row.liveAndUpcomingContent[e];
                    return (t ? t.duration : n.props.paginationIncrement * Be.b) * Be.b
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleItemsRendered", (function(e) {
                    var t = e.visibleStartIndex
                      , a = e.visibleStopIndex
                      , i = n.props
                      , r = i.row
                      , o = i.updateEabsToFetch
                      , c = R()(r, "liveAndUpcomingContent", [])
                      , l = R()(r, "channelId");
                    t < c.length ? (n.setState({
                        firstVisibleColumnIndex: t
                    }),
                    o(c.slice(t, a + 1).map((function(e) {
                        return e.eab
                    }
                    )), l)) : n.setState({
                        firstVisibleColumnIndex: c.length - 1
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleRowAnimationStart", (function() {
                    n.setState({
                        isScrolling: !0
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleRowAnimationEnd", (function() {
                    n.setState({
                        isScrolling: !1
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "renderFloatingProgramBlock", (function() {
                    var e = n.state
                      , t = e.firstVisibleColumnIndex
                      , a = e.isScrolling
                      , i = n.props
                      , r = i.row.liveAndUpcomingContent
                      , o = i.timeBlocksIntoFuture
                      , c = i.paginationIncrement
                      , l = R()(r[t + 1], "durationBeforeProgram", 1 / 0)
                      , s = Math.abs(o * Be.b - l * Be.b);
                    if (o > 0 && s < zt)
                        return null;
                    var u = Math.abs((o - c) * Be.b - l * Be.b) - Be.b * c <= 0
                      , d = !(o > 0) || o > R()(r[t], "durationBeforeProgram");
                    return n.renderProgram({
                        index: t,
                        style: Vt({
                            position: "absolute",
                            top: 0,
                            left: Be.e,
                            zIndex: 1,
                            height: "100%"
                        }, u && a && {
                            opacity: 0,
                            transition: "opacity 0.2s ease"
                        }, {
                            width: s === 1 / 0 ? "200vw" : s
                        }),
                        data: {
                            floating: !0,
                            hideBorder: d
                        }
                    })
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.row
                      , n = e.rowIndex
                      , a = e.width
                      , i = e.itemCount
                      , r = e.style
                      , o = e.timeBlocksIntoFuture
                      , c = e.playChannel
                      , l = e.durationIndexToFocus
                      , s = e.forceHover
                      , u = e.shouldChannelBeFocusable
                      , d = e.totalNumRows
                      , f = e.currentlyPlayingChannel
                      , m = e.collectionId
                      , p = this.state
                      , h = p.firstVisibleColumnIndex
                      , b = p.hoverAtContentIndex
                      , v = p.isScrolling
                      , g = this.props.row.liveAndUpcomingContent.slice(-1).pop().airingEnd ? i : 1 / 0
                      , y = B()("LiveGuideRow", {
                        "LiveGuideRow--hover": s
                    })
                      , O = 0 === b
                      , _ = l === Be.c
                      , j = R()(t, "liveAndUpcomingContent.0.availability.isOffAir", !1);
                    return Ht("div", {
                        className: y,
                        style: r,
                        "data-testid": v ? "live-guide-row-scrolling" : "live-guide-row"
                    }, Ht("span", {
                        className: B()("LiveGuideRow__channel-and-first-program", {
                            "LiveGuideRow__channel-and-first-program--hover": O,
                            "LiveGuideRow__channel-and-first-program--off-air": j
                        }),
                        "data-testid": "live-guide-channel-and-first-program"
                    }, Ht(vt, {
                        channel: t,
                        channelIndex: n,
                        forceHover: 0 === b,
                        isFocused: _,
                        shouldBeFocusable: u,
                        setFocusOnChannel: this.setIsHovering,
                        playChannel: c,
                        canSeeFirstProgram: 0 === h,
                        totalNumRows: d,
                        currentlyPlayingChannel: f,
                        collectionId: m,
                        isCurrentProgramOffAir: j
                    }), this.renderFloatingProgramBlock()), Ht(jt, {
                        style: {
                            overflow: "hidden",
                            marginLeft: Be.e,
                            width: "auto"
                        },
                        height: 112,
                        itemCount: g,
                        itemData: {
                            firstVisibleColumnIndex: h
                        },
                        getItemSize: this.getItemSize,
                        width: a,
                        scrollTo: o * Be.b,
                        onItemsRendered: this.handleItemsRendered,
                        onAnimationEnd: this.handleRowAnimationEnd,
                        onAnimationStart: this.handleRowAnimationStart
                    }, this.renderProgram))
                }
            }]),
            t
        }(u.Component)
          , Yt = n("VGA2")
          , Kt = n("4a2M")
          , qt = n("6wAU")
          , Jt = n("E+oP")
          , Xt = n.n(Jt)
          , Zt = n("J2m7")
          , Qt = n.n(Zt)
          , $t = n("xkGf")
          , en = function(e) {
            return e[v.b].liveGuideStartTime
        }
          , tn = function(e, t) {
            return R()(e[v.b], ["liveGuideEndTimes", t.collectionId])
        }
          , nn = function(e) {
            return e[v.b].liveGuideProgramStates
        }
          , an = function(e) {
            return e[v.b].programsDetail
        }
          , rn = Object(J.createSelector)([en, tn], (function(e, t) {
            if (!e || !t)
                return [];
            var n = new Date(e)
              , a = n.getMinutes() < 30
              , i = Math.ceil((t - e) / S.h)
              , r = Array.from(new Array(2 * i), (function(t, i) {
                var r = new Date(e);
                return 0 === i ? n : (a ? (r.setUTCHours(n.getUTCHours() + i / 2),
                r.setUTCMinutes(i % 2 ? 30 : 0)) : (r.setUTCHours(n.getUTCHours() + (i + 1) / 2),
                r.setUTCMinutes(i % 2 ? 0 : 30)),
                r)
            }
            ));
            return r.slice(0, -1).map((function(e, t) {
                return {
                    startTime: e,
                    endTime: r[t + 1]
                }
            }
            ))
        }
        ))
          , on = n("NJMr")
          , cn = Object(J.createSelector)([function(e, t) {
            var n = Qt()(e[v.b].collections, (function(e) {
                return e.collectionId === t.collectionId
            }
            ));
            return R()(n, "channels", [])
        }
        , function(e, t) {
            return R()(e[v.b], ["channelListings", t.collectionId], {})
        }
        , en, tn, an, nn], (function(e, t, n, a, i, r) {
            return e.map((function(e) {
                var o = e.channelId
                  , c = e.channelLogo
                  , l = e.channelName
                  , s = R()(t, [o, "programs"], []);
                return {
                    channelId: o,
                    channelName: l,
                    channelEab: Object(Ft.d)(s),
                    channelLogo: c,
                    liveAndUpcomingContent: function(e, t, n, a, i) {
                        var r = new Date(t)
                          , o = new Date(n)
                          , c = []
                          , l = 0;
                        return e.forEach((function(e) {
                            var t = Object(Ft.g)(e, a, i, r, o);
                            t.durationBeforeProgram = l,
                            t.channelGridBadges = Object(Ct.c)(t.availability, t.hasStartoverRights),
                            t.airingEnd && t.airingEnd <= r || t.duration <= 0 || (c.push(t),
                            l += t.duration)
                        }
                        )),
                        c
                    }(s, n, a, i, r)
                }
            }
            )).filter((function(e) {
                return !Xt()(e.liveAndUpcomingContent)
            }
            ))
        }
        ))
          , ln = Object(J.createSelector)([rn, en], (function(e, t) {
            return e.map((function(e) {
                var n = e.startTime;
                return {
                    timeBlockText: $t.f.format(n),
                    paginationText: sn(new Date(t), n),
                    rawStartTimeOfBlock: n
                }
            }
            ))
        }
        ))
          , sn = function(e, t) {
            var n = new Date(e);
            if (n.setDate(e.getDate() + 1),
            e.toDateString() === t.toDateString())
                return t.getHours() > 17 ? {
                    siteConfigkey: "tonight"
                } : {
                    siteConfigkey: "today"
                };
            if (n.toDateString() === t.toDateString())
                return {
                    siteConfigkey: "tomorrow"
                };
            var a = $t.a.format(t)
              , i = $t.e.format(t);
            return {
                siteConfigkey: "futureDate",
                replaceText: "".concat(a, " ").concat(i)
            }
        }
          , un = Object(J.createSelector)([cn, ln, function(e) {
            return e[v.b].isLoadingLiveGuide
        }
        , en, function(e) {
            return e[on.b].eabId
        }
        ], (function(e, t, n, a, i) {
            return {
                channels: e,
                timeBlocks: t,
                isLoadingLiveGuide: n,
                startTimeOfGuide: a,
                currentlyPlayingChannel: i
            }
        }
        ))
          , dn = (n("uYol"),
        d.a.createElement);
        function fn(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        var mn = Object(P.a)(Object(q.a)("auto_refresh"), Object(g.connect)())((function(e) {
            var t, n, a = e.isOnNow, i = e.refreshLiveGuide, r = e.liveGuideOnNowRefreshIntervalSeconds, o = e.liveGuideOtherRefreshIntervalSeconds, c = e.liveGuideOnNowReDrawIntervalSeconds, l = e.liveGuideIdleTimeoutSeconds, s = e.redrawLiveGuide;
            if (i)
                if (a) {
                    t = r + (Math.floor(601 * Math.random()) + -300),
                    n = i
                } else
                    t = o,
                    n = function() {
                        Object(p.f)("/live")
                    }
                    ;
            else
                t = c,
                n = function() {
                    s((new Date).getTime())
                }
                ;
            var u = function(e) {
                for (var t = 1; t < arguments.length; t++) {
                    var n = null != arguments[t] ? arguments[t] : {};
                    t % 2 ? fn(Object(n), !0).forEach((function(t) {
                        Object(C.a)(e, t, n[t])
                    }
                    )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : fn(Object(n)).forEach((function(t) {
                        Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                    }
                    ))
                }
                return e
            }({}, e, {
                refreshIntervalSeconds: t,
                enabled: !0,
                refresh: n,
                idleTimeoutSeconds: l
            });
            return dn(K, Object(x.a)({}, u, {
                "data-testid": "auto-refresh-view"
            }))
        }
        ))
          , pn = d.a.createElement
          , hn = function(e) {
            var t = e.timeBlocksIntoFuture
              , n = e.isLoadingLiveGuide
              , a = e.paginateForward
              , i = e.paginateBackward
              , r = e.paginateToBeginning
              , o = 30 * e.paginationIncrement / 60;
            return pn("div", {
                className: "LiveGuideChannelGrid__pagination-container",
                "data-testid": "pagination"
            }, pn("button", {
                className: "LiveGuideChannelGrid__pagination LiveGuideChannelGrid__pagination--back",
                "data-testid": "pagination-back",
                onClick: i,
                disabled: t <= 0
            }, pn("span", {
                className: "sr-only"
            }, pn(Ee.view, {
                from: "liveGuide.paginationBack",
                replace: {
                    TIME: o
                }
            }))), pn("button", {
                className: "LiveGuideChannelGrid__paginationButton",
                onClick: r,
                "data-testid": "paginate-to-beginning"
            }, pn("span", {
                className: "sr-only"
            }, pn(Ee.view, {
                from: "liveGuide.paginationNow"
            })), pn("div", {
                className: B()("LiveGuideChannelGrid__roundIndicator", {
                    "LiveGuideChannelGrid__roundIndicator--dark": 0 !== t
                })
            })), pn("button", {
                className: "LiveGuideChannelGrid__pagination LiveGuideChannelGrid__pagination--forward",
                "data-testid": "pagination-forward",
                onClick: a,
                disabled: n
            }, pn("span", {
                className: "sr-only"
            }, pn(Ee.view, {
                from: "liveGuide.paginationForward",
                replace: {
                    TIME: o
                }
            }))))
        }
          , bn = n("Yx7S")
          , vn = d.a.createElement;
        function gn(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function yn(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = null != arguments[t] ? arguments[t] : {};
                t % 2 ? gn(Object(n), !0).forEach((function(t) {
                    Object(C.a)(e, t, n[t])
                }
                )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : gn(Object(n)).forEach((function(t) {
                    Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                }
                ))
            }
            return e
        }
        var On = 1e3
          , _n = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "initialScrollHandled", !1),
                Object(C.a)(Object(D.a)(n), "state", {
                    timeBlocksIntoFuture: 0,
                    firstVisibleRowIndex: 0,
                    lastVisibleRowIndex: 0,
                    programsToFetch: {},
                    focusedRowIndex: 0,
                    focusedDurationIndex: Be.c,
                    hasSetFocusInGrid: !1
                }),
                Object(C.a)(Object(D.a)(n), "listRef", d.a.createRef()),
                Object(C.a)(Object(D.a)(n), "refreshLiveGuide", (function() {
                    var e = n.props
                      , t = e.geo
                      , a = e.collectionId;
                    (0,
                    e.fetchLiveGuideListings)(t, a, Object(bn.b)(Date.now()), !0)
                }
                )),
                Object(C.a)(Object(D.a)(n), "renderUpcomingTimeBlocks", (function(e) {
                    var t = e.index
                      , a = e.style
                      , i = B()("LiveGuideChannelGrid__time", {
                        "LiveGuideChannelGrid__time--first": 0 === t
                    })
                      , r = n.props.timeBlocks[t]
                      , o = R()(r, "timeBlockText", "");
                    return vn("div", {
                        className: i,
                        key: t,
                        style: a
                    }, o)
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleTimeBlocksRendered", (function(e) {
                    var t = e.visibleStopIndex
                      , a = e.visibleStartIndex
                      , i = n.props
                      , r = i.liveGuideMaxDistanceSeen
                      , o = i.timeBlocks
                      , c = i.setLiveGuideMaxDistanceSeen
                      , l = i.startTimeOfGuide
                      , s = t - a
                      , u = o[t].rawStartTimeOfBlock - l;
                    u > r && c(u),
                    n.setState({
                        paginationIncrement: s
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "renderRow", (function(e) {
                    var t = e.index
                      , a = e.style
                      , i = e.data
                      , r = n.props
                      , o = r.playChannelInPIP
                      , c = r.currentlyPlayingChannel
                      , l = r.collectionId
                      , s = n.state
                      , u = s.focusedRowIndex
                      , d = s.focusedDurationIndex
                      , f = s.hasSetFocusInGrid
                      , m = s.paginationIncrement
                      , p = i.width
                      , h = i.timeBlocksIntoFuture
                      , b = i.channels
                      , v = i.startTimeOfGuide
                      , g = b[t]
                      , y = !f && u === t
                      , O = f && u === t ? d : null;
                    return vn(Wt, {
                        width: p,
                        style: a,
                        row: g,
                        rowIndex: t,
                        itemCount: g.liveAndUpcomingContent.length,
                        timeBlocksIntoFuture: h,
                        playChannel: o,
                        updateEabsToFetch: n.updateEabsToFetch,
                        durationIndexToFocus: O,
                        forceHover: u === t,
                        shouldChannelBeFocusable: y,
                        key: v + t,
                        totalNumRows: b.length,
                        currentlyPlayingChannel: c,
                        collectionId: l,
                        paginationIncrement: m
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleItemsRendered", (function(e) {
                    var t = e.visibleStartIndex
                      , a = e.visibleStopIndex
                      , i = n.props
                      , r = i.currentlyPlayingChannel
                      , o = i.trackLiveGuideShown
                      , c = i.channels
                      , l = i.setInitiatedCollections
                      , s = i.initiatedCollections
                      , u = i.collectionId
                      , d = c.findIndex((function(e) {
                        return e.channelEab === r
                    }
                    ))
                      , f = d > 0 ? d : 0;
                    o(),
                    l(yn({}, s, Object(C.a)({}, u, !0))),
                    !n.initialScrollHandled && f > 0 && (window.scrollTo(0, f * Be.a),
                    n.initialScrollHandled = !0),
                    n.setState((function(e) {
                        return {
                            firstVisibleRowIndex: t,
                            lastVisibleRowIndex: a,
                            focusedRowIndex: e.focusedRowIndex < t || e.focusedRowIndex > a ? t + 1 : e.focusedRowIndex
                        }
                    }
                    ))
                }
                )),
                Object(C.a)(Object(D.a)(n), "getItemSize", (function(e) {
                    return 0 === e ? 2 * Be.b : Be.b
                }
                )),
                Object(C.a)(Object(D.a)(n), "getMemoizedItemData", tt((function(e, t, n, a) {
                    return {
                        timeBlocksIntoFuture: e,
                        width: t,
                        channels: n,
                        startTimeOfGuide: a
                    }
                }
                ))),
                Object(C.a)(Object(D.a)(n), "getCurrentlyViewedTimeText", (function(e, t) {
                    if (0 === t)
                        return vn(Ee.view, {
                            from: "liveGuide.onNow"
                        });
                    var n = e[t - 1]
                      , a = R()(n, "paginationText.siteConfigkey")
                      , i = R()(n, "paginationText.replaceText");
                    return a ? vn(Ee.view, {
                        from: "liveGuide.".concat(a),
                        replace: {
                            TEXT: i
                        }
                    }) : null
                }
                )),
                Object(C.a)(Object(D.a)(n), "paginateForward", at()((function() {
                    n.setState((function(e) {
                        return {
                            timeBlocksIntoFuture: e.timeBlocksIntoFuture + e.paginationIncrement
                        }
                    }
                    ), n.potentiallyFetchMoreData)
                }
                ), On)),
                Object(C.a)(Object(D.a)(n), "paginateBackward", at()((function() {
                    n.setState((function(e) {
                        var t = e.timeBlocksIntoFuture - e.paginationIncrement;
                        return {
                            timeBlocksIntoFuture: t < 0 ? 0 : t
                        }
                    }
                    )),
                    n.state.timeBlocksIntoFuture - n.state.paginationIncrement === 0 && n.potentiallyRedrawLiveGuide()
                }
                ), On)),
                Object(C.a)(Object(D.a)(n), "potentiallyFetchMoreData", (function() {
                    var e = n.props
                      , t = e.timeBlocks;
                    (0,
                    e.fetchLiveGuideListings)(e.geo, e.collectionId, t[n.state.timeBlocksIntoFuture].rawStartTimeOfBlock.getTime())
                }
                )),
                Object(C.a)(Object(D.a)(n), "paginateToBeginning", (function() {
                    n.setState({
                        timeBlocksIntoFuture: 0
                    }),
                    n.potentiallyRedrawLiveGuide()
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleScroll", (function() {
                    n.listRef.current && n.listRef.current.scrollTo(window.scrollY)
                }
                )),
                Object(C.a)(Object(D.a)(n), "updateEabsToFetch", (function(e, t) {
                    n.setState((function(n) {
                        return {
                            programsToFetch: yn({}, n.programsToFetch, Object(C.a)({}, t, e))
                        }
                    }
                    ), n.fetchProgramsDetail)
                }
                )),
                Object(C.a)(Object(D.a)(n), "fetchProgramsDetail", rt()((function() {
                    var e = n.state
                      , t = e.programsToFetch
                      , a = e.firstVisibleRowIndex
                      , i = e.lastVisibleRowIndex
                      , r = n.props
                      , o = r.channels
                      , c = r.fetchLiveGuideProgramsDetail
                      , l = r.fetchLiveGuideMeStates
                      , s = ct()(o.slice(a, i + 1).map((function(e) {
                        return t[e.channelId]
                    }
                    )));
                    c(s),
                    l(s)
                }
                ), 200)),
                Object(C.a)(Object(D.a)(n), "updateFocusedRow", (function(e) {
                    var t = n.props.channels;
                    n.setState((function(n) {
                        var a = n.focusedRowIndex + e;
                        return (a < 0 || a >= t.length) && (a = e < 0 ? 0 : t.length - 1),
                        e < 0 && a < n.firstVisibleRowIndex + 1 && window.scrollTo(0, window.scrollY - 4 * Be.a),
                        {
                            focusedRowIndex: a,
                            hasSetFocusInGrid: !0
                        }
                    }
                    ))
                }
                )),
                Object(C.a)(Object(D.a)(n), "incrementDurationIndex", at()((function() {
                    var e = n.props.channels;
                    n.setState((function(t) {
                        var n = R()(e, [t.focusedRowIndex, "liveAndUpcomingContent"], [])
                          , a = Object(Ft.e)(t.focusedDurationIndex, n);
                        return {
                            focusedDurationIndex: a,
                            timeBlocksIntoFuture: Math.floor(a),
                            hasSetFocusInGrid: !0
                        }
                    }
                    ), n.potentiallyFetchMoreData)
                }
                ), On)),
                Object(C.a)(Object(D.a)(n), "decrementDurationIndex", at()((function() {
                    var e = n.props.channels;
                    n.setState((function(t) {
                        var a = R()(e, [t.focusedRowIndex, "liveAndUpcomingContent"], [])
                          , i = Object(Ft.b)(t.focusedDurationIndex, a)
                          , r = i === Be.c ? 0 : Math.floor(i);
                        return 0 === r && n.potentiallyRedrawLiveGuide(),
                        {
                            focusedDurationIndex: i,
                            timeBlocksIntoFuture: r,
                            hasSetFocusInGrid: !0
                        }
                    }
                    ))
                }
                ), On)),
                Object(C.a)(Object(D.a)(n), "setDurationIndexToNow", (function() {
                    n.setState({
                        focusedDurationIndex: Be.c,
                        timeBlocksIntoFuture: 0,
                        hasSetFocusInGrid: !0
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleKeyDownInGrid", (function(e) {
                    var t = n.props.isLoadingLiveGuide;
                    switch (e.key) {
                    case S.d.ARROW_RIGHT:
                        e.preventDefault(),
                        t || n.incrementDurationIndex();
                        break;
                    case S.d.ARROW_LEFT:
                        e.preventDefault(),
                        n.decrementDurationIndex();
                        break;
                    case S.d.HOME:
                        e.preventDefault(),
                        n.setDurationIndexToNow();
                        break;
                    case S.d.ARROW_DOWN:
                        e.preventDefault(),
                        n.updateFocusedRow(1);
                        break;
                    case S.d.ARROW_UP:
                        e.preventDefault(),
                        n.updateFocusedRow(-1);
                        break;
                    case S.d.PAGE_DOWN:
                        e.preventDefault(),
                        n.updateFocusedRow(5);
                        break;
                    case S.d.PAGE_UP:
                        e.preventDefault(),
                        n.updateFocusedRow(-5)
                    }
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    var e = this.props
                      , t = e.geo
                      , n = e.collectionId;
                    (0,
                    e.fetchLiveGuideListings)(t, n, e.startTimeOfGuide),
                    window.addEventListener("scroll", this.handleScroll)
                }
            }, {
                key: "componentWillUnmount",
                value: function() {
                    window.removeEventListener("scroll", this.handleScroll)
                }
            }, {
                key: "render",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.paginationControlsNode
                      , a = t.redrawLiveGuide
                      , i = t.isLoadingLiveGuide
                      , r = t.timeBlocks
                      , o = this.state
                      , c = o.timeBlocksIntoFuture
                      , l = o.paginationIncrement;
                    return vn("div", {
                        className: "LiveGuideChannelGrid__container"
                    }, n && Object(ze.createPortal)(vn(hn, {
                        timeBlocksIntoFuture: c,
                        timeBlocks: r,
                        isLoadingLiveGuide: i,
                        paginateForward: this.paginateForward,
                        paginateBackward: this.paginateBackward,
                        paginateToBeginning: this.paginateToBeginning,
                        paginationIncrement: l
                    }), n), vn(Kt.a, null, (function(t) {
                        return vn($e, {
                            disableHeight: !0,
                            style: {
                                width: "100%"
                            }
                        }, (function(n) {
                            var a = n.width;
                            return e.renderChannelsGrid(t, a)
                        }
                        ))
                    }
                    )), vn(mn, {
                        refreshLiveGuide: this.refreshLiveGuide,
                        isOnNow: 0 === c
                    }), vn(mn, {
                        redrawLiveGuide: a
                    }))
                }
            }, {
                key: "renderChannelsGrid",
                value: function(e, t) {
                    var n = this.state.timeBlocksIntoFuture
                      , a = this.props
                      , i = a.timeBlocks
                      , r = a.channels
                      , o = a.collectionId
                      , c = a.startTimeOfGuide
                      , l = Object(Yt.a)(e, Be.g);
                    if (i.length)
                        return vn("div", {
                            className: "LiveGuideChannelGrid",
                            onKeyDown: this.handleKeyDownInGrid,
                            "data-testid": "channel-grid"
                        }, vn("div", {
                            className: "LiveGuideChannelGrid__time-banner",
                            style: {
                                marginLeft: "".concat(l, "px")
                            },
                            "aria-hidden": !0,
                            "data-testid": "time-banner"
                        }, vn("div", {
                            className: B()("LiveGuideChannelGrid__on-now", {
                                "LiveGuideChannelGrid__on-now--dark": 0 !== n
                            }),
                            "data-testid": "current-time"
                        }, this.getCurrentlyViewedTimeText(i, n)), vn("div", {
                            className: "LiveGuideChannelGrid__upcoming-times"
                        }, vn(jt, {
                            style: {
                                overflow: "hidden"
                            },
                            height: 46,
                            width: t - l - Be.e,
                            getItemSize: this.getItemSize,
                            itemCount: i.length,
                            scrollTo: Be.b * this.state.timeBlocksIntoFuture,
                            onItemsRendered: this.handleTimeBlocksRendered
                        }, this.renderUpcomingTimeBlocks))), vn(Ye.a, {
                            ref: this.listRef,
                            key: o,
                            height: window.innerHeight,
                            style: {
                                height: "100%",
                                overflow: "hidden",
                                marginLeft: "".concat(l, "px")
                            },
                            width: t - l,
                            itemSize: Be.a,
                            itemCount: r.length,
                            itemData: this.getMemoizedItemData(n, t, r, c),
                            onItemsRendered: this.handleItemsRendered
                        }, this.renderRow))
                }
            }, {
                key: "potentiallyRedrawLiveGuide",
                value: function() {
                    var e = (new Date).getTime();
                    this.props.startTimeOfGuide + S.o <= e && this.props.redrawLiveGuide(e)
                }
            }]),
            t
        }(u.Component)
          , jn = Object(g.connect)((function(e, t) {
            return un(e, t)
        }
        ), {
            fetchLiveGuideListings: b.fetchLiveGuideListings,
            fetchLiveGuideProgramsDetail: b.fetchLiveGuideProgramsDetail,
            fetchLiveGuideMeStates: b.fetchLiveGuideMeStates,
            playChannelInPIP: qt.playChannelInPIP,
            redrawLiveGuide: b.redrawLiveGuide
        })(_n)
          , In = Object(J.createSelector)([function(e, t) {
            var n = Qt()(e[v.b].collections, (function(e) {
                return e.collectionId === t.collectionId
            }
            ));
            return R()(n, "genre")
        }
        , function(e, t) {
            return R()(e[v.b], ["genrePrograms", t.collectionId], [])
        }
        , nn, an, rn, en, tn], (function(e, t, n, a, i, r, o) {
            for (var c = new Date(r), l = new Date(o), s = i.reduce((function(t, n) {
                var a = n.startTime
                  , i = n.endTime;
                return t[a] = {
                    name: a <= c ? "".concat(e, " on now") : Object(Ft.c)(a, i),
                    programs: []
                },
                t
            }
            ), {}), u = 0, d = 0; u < t.length && d < i.length; ) {
                var f = i[d]
                  , m = t[u];
                if (new Date(m.airingStart) < f.endTime) {
                    var p = Object(Ft.g)(m, a, n, c, l);
                    s[f.startTime].programs.push(p),
                    u++
                } else
                    d++
            }
            var h = i.map((function(e) {
                var t = e.startTime;
                return s[t]
            }
            )).filter((function(e) {
                return e.programs.length > 0
            }
            ));
            return {
                genre: e,
                buckets: h
            }
        }
        ))
          , wn = n("8yiw")
          , Tn = n("h0o4")
          , Sn = (n("dVNW"),
        d.a.createElement)
          , En = function(e, t, n) {
            return function(a, i) {
                return {
                    eab: a.eab,
                    id: a.id,
                    content: Sn("div", {
                        className: "LiveGuideGenreCollection__item"
                    }, Sn(xt, {
                        content: a,
                        collectionId: t,
                        withHoverPopout: !0,
                        playChannel: e,
                        collectionIndex: n,
                        index: i
                    }))
                }
            }
        }
          , kn = Object(g.connect)((function(e, t) {
            return In(e, t)
        }
        ), {
            fetchLiveGuideGenrePrograms: b.fetchLiveGuideGenrePrograms,
            playChannelInPIP: qt.playChannelInPIP
        })((function(e) {
            var t = e.buckets
              , n = void 0 === t ? [] : t
              , a = e.fetchLiveGuideGenrePrograms
              , i = e.playChannelInPIP
              , r = e.collectionId
              , o = e.geo
              , c = Object(u.useContext)(Kt.b)
              , l = Object(Yt.a)(c, Tn.a);
            return Object(u.useEffect)((function() {
                a(o, r)
            }
            ), []),
            Sn("div", {
                className: "LiveGuideGenreCollection"
            }, Sn(H.a, null, n.map((function(e, t) {
                var n = e.name
                  , a = e.programs;
                return Sn("div", {
                    key: n,
                    className: "LiveGuideGenreCollection__bucket",
                    "data-testid": "".concat(n, "-bucket")
                }, Sn("h2", {
                    className: "LiveGuideGenreCollection__bucket-name",
                    id: Object(F.b)(n)
                }, n), Sn(wn.a, {
                    className: "LiveGuideGenreCollection__slider",
                    items: a.map(En(i, r, t)),
                    itemsPerPage: l,
                    hasGenericTiles: !0,
                    collectionTitle: n
                }))
            }
            ))), Sn(mn, {
                isOnNow: !0,
                refreshLiveGuide: function() {
                    return a(o, r)
                }
            }))
        }
        ))
          , Cn = n("lYDw")
          , Nn = (n("mYXy"),
        n("A7B/"))
          , Pn = n("BkJZ")
          , Dn = n("Jc6e")
          , xn = d.a.createElement
          , Ln = Object(g.connect)((function(e) {
            var t = new Date(R()(e[v.b], "liveGuideStartTime"));
            return {
                collections: R()(e[v.b], "collections", []),
                guideReadableStartTime: $t.f.format(t),
                playerType: R()(e[on.b], "type", "")
            }
        }
        ), {
            fetchLiveGuideCollections: b.fetchLiveGuideCollections,
            playChannelInPIP: qt.playChannelInPIP
        })((function(e) {
            var t = e.collections
              , n = e.geo
              , a = e.fetchLiveGuideCollections
              , r = e.playChannelInPIP
              , o = e.guideReadableStartTime
              , c = e.playerType
              , l = function() {
                var e = Object(u.useState)(null)
                  , t = e[0]
                  , n = e[1];
                return [Object(u.useCallback)((function(e) {
                    null !== e && n(e)
                }
                ), []), t]
            }()
              , s = Object(z.a)(l, 2)
              , d = s[0]
              , f = s[1]
              , p = Object(u.useState)(0)
              , h = p[0]
              , b = p[1]
              , v = Object(u.useState)(!1)
              , g = v[0]
              , y = v[1]
              , O = Object(u.useState)({})
              , _ = O[0]
              , j = O[1]
              , I = Object(u.useState)((new Date).getTime())
              , w = I[0]
              , T = I[1]
              , E = Object(u.useState)(0)
              , k = E[0]
              , C = E[1]
              , N = Object(u.useContext)(Ve.a).registerPerformanceMark
              , P = Object(u.useContext)(Cn.a).cacheKey
              , D = R()(t, [h], {})
              , x = D.collectionId
              , L = void 0 === x || D.collectionType === Be.d.TIME_GRID && !_[D.collectionId]
              , M = Object(u.useCallback)((function(e) {
                if (Object(F.k)(e)) {
                    Pn.d(mt.FEATURE_TAGS.live_guide_v2),
                    Ue.d.trackLiveGuideShown({
                        paintDuration: 0,
                        isFirstLoad: !1
                    }),
                    Ue.d.trackLiveGuideImpression({
                        collectionId: R()(t, [h, "collectionId"])
                    });
                    var n = (new Date).getTime();
                    T(n)
                } else {
                    var a = (new Date).getTime() - w;
                    Ue.d.trackLiveGuideClosed({
                        liveGuideOpenDuration: a,
                        liveGuideMaxDistanceSeen: k
                    })
                }
            }
            ), [t, h, w, k]);
            Object(u.useEffect)((function() {
                return m.a.events.on("routeChangeStart", M),
                function() {
                    m.a.events.off("routeChangeStart", M)
                }
            }
            ), [M]),
            Object(u.useEffect)((function() {
                a(n, P)
            }
            ), [a, n, P]),
            Object(u.useEffect)((function() {
                c !== S.l.MODAL && function() {
                    var e, t, a;
                    i.a.async((function(o) {
                        for (; ; )
                            switch (o.prev = o.next) {
                            case 0:
                                return o.next = 2,
                                i.a.awrap(Object(Nn.a)(ne.b.lastPlayedChannel(n)));
                            case 2:
                                e = o.sent,
                                t = R()(e, "bundle.eab_id"),
                                a = R()(e, "bundle.channel_id"),
                                t && r(t, S.m.LIVE, a);
                            case 6:
                            case "end":
                                return o.stop()
                            }
                    }
                    ))
                }()
            }
            ), []),
            Object(u.useEffect)((function() {
                window.scrollTo(0, 0)
            }
            ), []),
            Object(u.useEffect)((function() {
                x && (window.scrollTo(0, 0),
                Ue.d.trackLiveGuideImpression({
                    collectionId: x
                }))
            }
            ), [x]),
            Object(u.useEffect)((function() {
                if (c === S.l.MODAL) {
                    var e = R()(Fe.Player, "actions.switchMode", null);
                    e && e("modal")
                } else if (c === S.l.PAGE) {
                    var t = (new Date).getTime() - w;
                    Ue.d.trackLiveGuideClosed({
                        liveGuideOpenDuration: t,
                        liveGuideMaxDistanceSeen: k
                    })
                }
            }
            ), [c]);
            return xn("div", {
                className: "LiveGuide",
                "aria-busy": L
            }, xn("span", {
                className: "sr-only"
            }, xn(Ee.view, {
                from: "liveGuide.introText",
                replace: {
                    TIME: o
                }
            })), xn("div", {
                className: "LiveGuide__header-container"
            }, xn(H.a, null, xn("div", {
                className: "LiveGuide__header"
            }, t.length > 0 && xn("div", {
                className: "LiveGuide__filters",
                role: "tablist",
                "aria-label": "Live Guide Filters",
                onKeyDown: He
            }, t.map((function(e, t) {
                return xn("button", {
                    key: "".concat(e.collectionName, "-filter"),
                    role: "tab",
                    className: B()("LiveGuide__filter-item", {
                        "LiveGuide__filter-item--selected": t === h
                    }),
                    onClick: function() {
                        return b(t)
                    },
                    "aria-controls": "".concat(e.collectionName, "-tab"),
                    id: e.collectionName,
                    "aria-selected": t === h,
                    tabIndex: t === h ? 0 : -1
                }, e.collectionName)
            }
            ))), xn("div", {
                className: "LiveGuide__secondary-header-controls",
                ref: d
            })))), xn("div", {
                className: "LiveGuide__content",
                role: "tabpanel",
                id: "".concat(D.collectionName, "-tab"),
                "aria-labelledby": D.collectionName
            }, L && xn(Dn.a, null), D.collectionType === Be.d.TIME_GRID && xn(jn, {
                key: D.collectionId,
                collectionId: D.collectionId,
                geo: n,
                paginationControlsNode: f,
                trackLiveGuideShown: function() {
                    if (!g) {
                        var e = N("firstMeaningfulPaint", {
                            origin: "LiveGuide",
                            resetOnRouteChange: !0
                        });
                        Ue.d.trackLiveGuideShown({
                            paintDuration: e,
                            isFirstLoad: !0
                        }),
                        y(!0)
                    }
                },
                liveGuideMaxDistanceSeen: k,
                setLiveGuideMaxDistanceSeen: C,
                setInitiatedCollections: j,
                initiatedCollections: _
            }), D.collectionType === Be.d.GENRE && xn(kn, {
                key: D.collectionId,
                collectionId: D.collectionId,
                geo: n
            })))
        }
        ))
          , Rn = n("lSCD")
          , Mn = n.n(Rn)
          , An = d.a.createElement
          , Gn = function(e) {
            var t = e.daysUntilDismissalExpires
              , n = e.maxDismissalCount
              , a = e.minDaysUntilShow
              , i = Se(t, n, a, "_hulu_ms_rate_prompt")
              , r = Object(z.a)(i, 3)
              , o = r[0]
              , c = r[1]
              , l = r[2]
              , s = function() {
                var e = Object(u.useState)(!1)
                  , t = e[0]
                  , n = e[1];
                return Object(u.useEffect)((function() {
                    if ("undefined" !== typeof window.Windows) {
                        var e = R()(window.Windows, "Services.Store.StoreContext.getDefault")
                          , t = Mn()(e) ? e() : {}
                          , a = R()(t, "requestRateAndReviewAppAsync");
                        Mn()(a) && n(!0)
                    }
                }
                ), []),
                t
            }()
              , d = Object(u.useCallback)((function() {
                window.Windows.Services.Store.StoreContext.getDefault().requestRateAndReviewAppAsync(),
                l()
            }
            ), [l]);
            return !o && s ? An(Le, {
                title: An(Ee.view, {
                    from: "ratingPrompt.title"
                }),
                body: An(Ee.view, {
                    from: "ratingPrompt.body",
                    isHTML: !0
                }),
                ctaCopy: An(Ee.view, {
                    from: "ratingPrompt.cta"
                }),
                dismissAction: c,
                ctaAction: d
            }) : null
        };
        Gn.defaultProps = {
            daysUntilDismissalExpires: 0,
            maxDismissalCount: 0,
            minDaysUntilShow: 0
        };
        var Bn = Object(q.a)("microsoftPwaRatePrompt")(Object(u.memo)(Gn))
          , Fn = n("6kA8")
          , Hn = (n("h2bZ"),
        n("SgbN"))
          , Un = d.a.createElement
          , Vn = Un(u.Fragment, null, Un("span", {
            className: "sr-only"
        }, Un(Ee.view, {
            from: "actionMenuConfig.addMyStuff"
        })), Un(Hn.l, null))
          , zn = function(e) {
            var t = e.hasManageDvr
              , n = e.openManageDVR;
            return Un("div", {
                className: "MyStuffBanner"
            }, Un("div", {
                className: "MyStuffBanner__background"
            }), Un(H.a, {
                className: "MyStuffBanner__container"
            }, Un("div", {
                className: "MyStuffBanner__text-container"
            }, Un("div", {
                className: "MyStuffBanner__text"
            }, Un("h1", {
                className: "MyStuffBanner__title"
            }, Un(Ee.view, {
                from: "myStuffConfig.banner.title"
            })), Un("p", {
                className: "MyStuffBanner__description"
            }, Un(Ee.view, {
                from: "myStuffConfig.banner.description",
                replace: {
                    ICON: Vn
                }
            })))), t && Un(Fn.a, {
                buttonStyle: "cta",
                "data-testid": "manage-dvr-button",
                onClick: n
            }, Un(Ee.view, {
                from: "myStuffConfig.banner.actionText"
            }))))
        }
          , Wn = d.a.createElement
          , Yn = function(e) {
            var t = e.footerTextPath
              , n = e.totalSelectedItems
              , a = e.handleAccept
              , i = e.handleCancel
              , r = !!n
              , o = 1 === n
              , c = Wn(Ee.view, o ? {
                from: "".concat(t, ".totalSelectedTextSingular")
            } : {
                from: "".concat(t, ".totalSelectedText")
            })
              , l = Wn(Ee.view, {
                from: "".concat(t, ".selectActionRemove")
            })
              , s = Wn(Ee.view, {
                from: "".concat(t, ".selectActionCancel")
            });
            return Wn("div", {
                className: "MyStuffFooter__content"
            }, Wn("div", {
                className: "MyStuffFooter__select-videos-container",
                "aria-live": "polite",
                "aria-atomic": "true"
            }, r && function(e, t) {
                return Wn("div", {
                    className: "MyStuffFooter__selected-videos"
                }, Wn("div", {
                    className: "MyStuffFooter__selected-quantity"
                }, e), Wn("div", {
                    className: "MyStuffFooter__selected-text"
                }, t))
            }(n, c)), r && Wn("div", {
                className: "MyStuffFooter__actions"
            }, Wn(Fn.a, {
                onClick: i,
                className: "MyStuffFooter__btn MyStuffFooter__btn--cancel",
                buttonStyle: "inverse"
            }, s), Wn(Fn.a, {
                onClick: a,
                className: "MyStuffFooter__btn MyStuffFooter__btn--accept",
                buttonStyle: "cta"
            }, l)))
        }
          , Kn = d.a.createElement
          , qn = function(e) {
            var t = e.totalSelectedItems
              , n = e.handleAccept
              , a = e.handleCancel
              , i = e.footerTextPath
              , r = 1 === t
              , o = Kn(Ee.view, r ? {
                from: "".concat(i, ".confirmTitleSingular")
            } : {
                from: "".concat(i, ".confirmTitle")
            })
              , c = Kn(Ee.view, {
                from: "".concat(i, ".confirmSubtitle")
            })
              , l = Kn(Ee.view, {
                from: "".concat(i, ".confirmActionYes")
            })
              , s = Kn(Ee.view, {
                from: "".concat(i, ".confirmActionCancel")
            });
            return Kn("div", {
                className: "MyStuffFooter__content"
            }, Kn("div", {
                className: "MyStuffFooter__message"
            }, Kn("div", {
                className: "MyStuffFooter__message-title"
            }, o), Kn("div", {
                className: "MyStuffFooter__message-subtitle"
            }, c)), Kn("div", {
                className: "MyStuffFooter__actions"
            }, Kn(Fn.a, {
                onClick: a,
                className: "MyStuffFooter__btn MyStuffFooter__btn--cancel",
                buttonStyle: "inverse"
            }, s), Kn(Fn.a, {
                onClick: n,
                className: "MyStuffFooter__btn MyStuffFooter__btn--accept",
                buttonStyle: "cta"
            }, l)))
        }
          , Jn = (n("VyN/"),
        n("YYy2"))
          , Xn = d.a.createElement
          , Zn = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    showConfirmRemove: !1
                }),
                Object(C.a)(Object(D.a)(n), "showConfirmation", (function() {
                    n.setState({
                        showConfirmRemove: !0
                    });
                    var e = n.props
                      , t = e.totalSelectedItems
                      , a = e.manageDVR
                      , i = Object(Jn.b)(a, S.n.my_stuff_step.remove_confirm);
                    Object(ke.trackTealium)({
                        item_count: t,
                        event_name: i
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleCancel", (function() {
                    var e = n.props
                      , t = e.handleCancel
                      , a = e.totalSelectedItems
                      , i = e.manageDVR
                      , r = n.state.showConfirmRemove;
                    t();
                    var o = "";
                    o = r ? Object(Jn.b)(i, S.n.my_stuff_step.cancel_confirm) : Object(Jn.b)(i, S.n.my_stuff_step.cancel),
                    Object(ke.trackTealium)({
                        item_count: a,
                        event_name: o
                    }),
                    n.resetFooter()
                }
                )),
                Object(C.a)(Object(D.a)(n), "handleRemove", (function() {
                    var e = n.props
                      , t = e.handleRemove
                      , a = e.totalSelectedItems
                      , i = e.manageDVR
                      , r = Object(Jn.b)(i, S.n.my_stuff_step.remove);
                    t(),
                    Object(ke.trackTealium)({
                        item_count: a,
                        event_name: r
                    }),
                    n.resetFooter()
                }
                )),
                Object(C.a)(Object(D.a)(n), "resetFooter", (function() {
                    n.setState({
                        showConfirmRemove: !1
                    })
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentWillReceiveProps",
                value: function(e) {
                    e.totalSelectedItems !== this.props.totalSelectedItems && this.resetFooter()
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.totalSelectedItems
                      , n = e.footerTextPath
                      , a = this.state.showConfirmRemove
                      , i = B()("MyStuffFooter", {
                        "MyStuffFooter--selected": t
                    });
                    return Xn("div", {
                        className: i
                    }, a ? Xn(qn, {
                        totalSelectedItems: t,
                        handleAccept: this.handleRemove,
                        handleCancel: this.handleCancel,
                        footerTextPath: n
                    }) : Xn(Yn, {
                        totalSelectedItems: t,
                        handleAccept: this.showConfirmation,
                        handleCancel: this.handleCancel,
                        footerTextPath: n
                    }))
                }
            }]),
            t
        }(d.a.Component);
        Zn.defaultProps = {
            totalSelectedItems: 0
        };
        var Qn = Zn
          , $n = n("qPyV")
          , ea = n.n($n)
          , ta = n("OFL0")
          , na = n.n(ta)
          , aa = n("Czfj")
          , ia = n("5YYZ")
          , ra = n("TqH5")
          , oa = n("wquh")
          , ca = n("B7ci")
          , la = d.a.createElement;
        function sa(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        var ua = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    return la("div", {
                        className: "Tile__content"
                    }, this.getActionElement(), this.getTitleElement())
                }
            }, {
                key: "getActionElement",
                value: function() {
                    var e = this.props
                      , t = e.action
                      , n = e.actionHover;
                    if (t)
                        return n ? la("div", {
                            className: "Tile__action-container"
                        }, la("div", {
                            "aria-hidden": !0,
                            className: "Tile__action Tile__action--base"
                        }, t), la("div", {
                            className: "Tile__action Tile__action--hover"
                        }, n)) : la("div", {
                            "aria-hidden": !0,
                            className: "Tile__action"
                        }, t)
                }
            }, {
                key: "getTitleElement",
                value: function() {
                    var e = this.props
                      , t = e.title
                      , n = e.detailsUrl
                      , a = e.isTitleClickable
                      , i = e.detailsUrlUserInteractionData
                      , r = e.tealiumTileData
                      , o = e.discoverHref
                      , c = function(e) {
                        for (var t = 1; t < arguments.length; t++) {
                            var n = null != arguments[t] ? arguments[t] : {};
                            t % 2 ? sa(Object(n), !0).forEach((function(t) {
                                Object(C.a)(e, t, n[t])
                            }
                            )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : sa(Object(n)).forEach((function(t) {
                                Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                            }
                            ))
                        }
                        return e
                    }({}, R()(r, "detailsLink"), {
                        show_select: t
                    })
                      , l = la(ca.a, {
                        lineHeightEm: 1.3,
                        maxLines: 1
                    }, t);
                    return a && null !== n ? la(oa.a, {
                        className: "Tile__title Tile__title--clickable",
                        href: n,
                        discoverHref: o,
                        userInteractionData: i,
                        tealiumData: c
                    }, l) : la("h3", {
                        className: "Tile__title"
                    }, l)
                }
            }]),
            t
        }(u.Component)
          , da = n("uEca")
          , fa = n("RIwu")
          , ma = n("6xjU")
          , pa = n("X4z9")
          , ha = n("b1ih")
          , ba = n("nWNM")
          , va = n("hhJA")
          , ga = d.a.createElement;
        function ya(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function Oa(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = null != arguments[t] ? arguments[t] : {};
                t % 2 ? ya(Object(n), !0).forEach((function(t) {
                    Object(C.a)(e, t, n[t])
                }
                )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : ya(Object(n)).forEach((function(t) {
                    Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                }
                ))
            }
            return e
        }
        var _a = function(e, t) {
            return t ? Object(ba.b)(e, 1200) : Object(ba.b)(e, 600)
        }
          , ja = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.availability
                      , n = e.watchUrl
                      , a = e.detailsUrl
                      , i = e.detailsUrlUserInteractionData
                      , r = e.watchUrlUserInteractionData
                      , o = e.eabId
                      , c = e.collectionId
                      , l = e.isReadOnly
                      , s = e.channelId
                      , d = e.tealiumTileData
                      , f = e.title
                      , m = e.discoverHref
                      , p = Object(lt.c)(o)
                      , h = t && t.isPlayable
                      , b = t && t.isLive
                      , v = h ? n : a
                      , g = Oa({}, h ? R()(d, "playable") : R()(d, "nonPlayable"), {
                        show_select: f
                    })
                      , y = h ? r : i
                      , O = {
                        entityId: p,
                        collectionId: c,
                        channelId: s,
                        eabId: o,
                        isPlayable: h,
                        isLive: b
                    }
                      , _ = ga(u.Fragment, null, ga("div", {
                        className: "Tile__spacer"
                    }), this.getImageElement(), this.getImageScrimElement(), this.getRollOverElement(), this.getWatermarkElement());
                    return l ? ga("div", {
                        className: "Tile__thumbnail"
                    }, _) : ga(oa.a, {
                        className: "Tile__thumbnail Tile__thumbnail--with-hover",
                        href: v,
                        discoverHref: m,
                        onClick: this.props.onClick,
                        userInteractionData: y,
                        videoData: O,
                        tealiumData: g
                    }, _)
                }
            }, {
                key: "getControlsElement",
                value: function() {
                    var e = this.props
                      , t = e.availability
                      , n = e.hideControlIcon
                      , a = e.isRetina
                      , i = e.isReadOnly
                      , r = e.useSmallerIcons;
                    if (t && !i) {
                        var o = t.isPlayable
                          , c = B()("Tile__icon", {
                            "Tile__icon--retina": a,
                            "Tile__icon--small": r
                        });
                        return ga("div", {
                            className: "Tile__controls"
                        }, !n && ga("span", {
                            className: c
                        }, ga(o ? pa.a : ha.a, null)))
                    }
                }
            }, {
                key: "getRollOverImageElement",
                value: function() {
                    var e = this.props
                      , t = e.thumbnailHover
                      , n = e.isRetina
                      , a = e.altText;
                    return ga("div", null, t && ga(va.a, {
                        className: "Tile__rollover-image",
                        src: _a(t, n),
                        alt: a
                    }))
                }
            }, {
                key: "getRollOverElement",
                value: function() {
                    if (!this.props.isReadOnly)
                        return ga("div", {
                            className: "Tile__rollover"
                        }, this.getRollOverImageElement(), this.getControlsElement())
                }
            }, {
                key: "getImageElement",
                value: function() {
                    var e = this.props
                      , t = e.thumbnail
                      , n = e.isRetina
                      , a = e.altText;
                    return t ? ga(va.a, {
                        className: "Tile__image",
                        src: _a(t, n),
                        alt: a
                    }) : ga("div", {
                        className: "Tile__placeholder-image"
                    }, ga("span", {
                        role: "img",
                        "aria-label": a
                    }))
                }
            }, {
                key: "getImageScrimElement",
                value: function() {
                    var e = this.props
                      , t = e.thumbnail
                      , n = e.title;
                    return t ? ga("div", {
                        className: "Tile__scrim Tile__scrim--thumbnail"
                    }) : ga("div", {
                        className: "Tile__scrim"
                    }, ga("div", {
                        className: "Tile__scrim-text"
                    }, ga(ca.a, null, n)))
                }
            }, {
                key: "getWatermarkElement",
                value: function() {
                    var e = this.props
                      , t = e.brandWatermark
                      , n = e.brandName;
                    if (t)
                        return ga("img", {
                            className: "Tile__brand",
                            src: Object(ba.a)({
                                url: t
                            }),
                            alt: n
                        })
                }
            }]),
            t
        }(u.Component);
        ja.defaultProps = {
            hideControlIcon: !1,
            isRetina: !1,
            isReadOnly: !1
        };
        var Ia = ja
          , wa = d.a.createElement
          , Ta = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.statusBadge
                      , n = e.timeBadge
                      , a = e.unWatchedBadge;
                    return wa("div", {
                        className: "Tile__thumbnail-container"
                    }, wa(Ia, this.props), t ? this.getBadge(t, "status") : null, n ? this.getBadge(n, "time") : null, a ? this.getBadge(a, "unwatched") : null, this.getStatusBarElement())
                }
            }, {
                key: "getBadge",
                value: function(e, t) {
                    var n = e.text
                      , a = e.classNames
                      , i = "Tile__badge Tile__badge--".concat(t);
                    return wa("div", {
                        className: i
                    }, wa(ma.a, {
                        text: n,
                        modifiers: a
                    }))
                }
            }, {
                key: "getStatusBarElement",
                value: function() {
                    var e = this.props.availability;
                    if (e) {
                        var t = e.isLive
                          , n = e.isPlayable
                          , a = e.progress;
                        if (n) {
                            var i = t ? fa.b : fa.a;
                            return wa(da.a, {
                                className: "Tile__statusBar",
                                progressClass: i,
                                progress: a
                            })
                        }
                    }
                }
            }]),
            t
        }(u.Component);
        Ta.defaultProps = {
            hideControlIcon: !1,
            isRetina: !1,
            isReadOnly: !1
        };
        var Sa = Ta
          , Ea = (n("s8m1"),
        d.a.createElement)
          , ka = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {}),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props.darkTheme
                      , t = B()("Tile", {
                        "Tile--dark": e
                    });
                    return Ea("div", {
                        className: t
                    }, Ea(Sa, this.props), Ea(ua, this.props))
                }
            }]),
            t
        }(u.Component)
          , Ca = Object(P.a)(ia.a, ra.a, aa.a)
          , Na = Ca(ka)
          , Pa = (Ca(ua),
        Ca(Ia),
        d.a.createElement)
          , Da = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    return Pa("div", {
                        className: "NetworkTile__content"
                    }, this.getTitleElement())
                }
            }, {
                key: "getTitleElement",
                value: function() {
                    var e = this.props
                      , t = e.title
                      , n = e.detailsUrl
                      , a = e.detailsUrlUserInteractionData
                      , i = e.isReadOnly
                      , r = e.tealiumTileData
                      , o = R()(r, "detailsLink");
                    return i ? Pa("div", {
                        className: "NetworkTile__title"
                    }, Pa(ca.a, null, t)) : Pa(oa.a, {
                        className: "NetworkTile__title NetworkTile__title--clickable",
                        href: n,
                        userInteractionData: a,
                        tealiumData: o
                    }, Pa(ca.a, null, t))
                }
            }]),
            t
        }(u.Component)
          , xa = (n("Picv"),
        d.a.createElement)
          , La = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.detailsUrl
                      , n = e.detailsUrlUserInteractionData
                      , a = e.isReadOnly
                      , i = e.tealiumTileData
                      , r = R()(i, "nonPlayable");
                    return a ? xa("div", {
                        className: "NetworkTile__thumbnail",
                        style: {
                            backgroundColor: this.getBackgroundColor()
                        }
                    }, this.getImageElement()) : xa(oa.a, {
                        className: "NetworkTile__thumbnail NetworkTile__thumbnail--with-hover",
                        href: t,
                        onClick: this.props.onClick,
                        userInteractionData: n,
                        style: {
                            backgroundColor: this.getBackgroundColor()
                        },
                        tealiumData: r
                    }, this.getImageElement())
                }
            }, {
                key: "getBackgroundColor",
                value: function() {
                    var e = this.props
                      , t = e.brandThumbnail
                      , n = e.color
                      , a = R()(t, "accent.hue", 0);
                    return n || (a > 0 ? "hsl(".concat(a, ",50%,40%)") : void 0)
                }
            }, {
                key: "getImageElement",
                value: function() {
                    var e = this.props
                      , t = e.altText
                      , n = e.brandThumbnail;
                    if (!n.path)
                        return xa("span", {
                            role: "img",
                            "aria-label": t
                        });
                    var a = Object(ba.a)({
                        url: n.path,
                        large: !0
                    });
                    return xa("img", {
                        className: "NetworkTile__image",
                        src: a,
                        role: "presentation",
                        alt: t
                    })
                }
            }]),
            t
        }(u.Component);
        La.defaultProps = {
            brandThumbnail: {}
        };
        var Ra = La
          , Ma = d.a.createElement
          , Aa = function(e) {
            return Ma("div", {
                className: "NetworkTile"
            }, Ma(Ra, e), Ma(Da, e))
        }
          , Ga = function(e, t) {
            var n = R()(t, ["asset", "id"]) || t.id;
            return {
                color: R()(e, ["configurations", "networkTileColors", n, "color"], "#000000")
            }
        }
          , Ba = Object(P.a)(ia.a, ra.a, Object(g.connect)(Ga), aa.a)
          , Fa = Ba(Aa)
          , Ha = (Ba(Ra),
        Object(g.connect)(Ga)(Ra),
        n("m5it"),
        d.a.createElement)
          , Ua = function(e, t) {
            return function(n) {
                var a, i = n.asset, r = n.assetIndex, o = n.collectionId, c = n.collectionIndex, l = n.callback, s = n.isSelected, d = n.toggleVisible, f = n.tealiumTileData, m = n.name, p = Object(N.a)(n, ["asset", "assetIndex", "collectionId", "collectionIndex", "callback", "isSelected", "toggleVisible", "tealiumTileData", "name"]), h = B()("MyStuffTile__toggle", {
                    "MyStuffTile__toggle--dvr": "DVR_TILE" === t
                }), b = B()("MyStuffTile", {
                    "MyStuffTile--active": d
                }), v = R()(i, "id"), g = R()(i, "name"), y = {
                    itemId: v,
                    itemName: g,
                    itemIndex: r,
                    eabId: R()(i, ["bundle", "eab_id"]) || "EAB::".concat(v, "::NULL::NULL"),
                    collectionId: o,
                    collectionIndex: c,
                    recordedEpisodes: R()(i, ["recorded_episodes"]),
                    name: m
                }, O = "DVR_TILE" === t;
                a = Ha(s && O ? Hn.h : s ? Hn.i : O ? Hn.j : Hn.k, null);
                var _ = O ? "manageDvrConfig.tileRemoveLabel" : "myStuffConfig.tileRemoveLabel"
                  , j = Ha("button", {
                    "aria-pressed": s,
                    className: h,
                    onClick: function(e) {
                        return l(y, e)
                    },
                    "data-testid": "remove-button"
                }, Ha("span", {
                    className: "sr-only"
                }, Ha(Ee.view, {
                    from: _,
                    replace: {
                        title: g
                    }
                })), a);
                return Ha("div", {
                    className: b,
                    "data-testid": "my-stuff-tile"
                }, O ? Ha(u.Fragment, null, Ha(e, Object(x.a)({
                    asset: i
                }, p, {
                    tealiumTileData: f
                })), j) : Ha(u.Fragment, null, j, Ha(e, Object(x.a)({
                    asset: i
                }, p, {
                    tealiumTileData: f
                }))))
            }
        }
          , Va = n("rK06")
          , za = d.a.createElement;
        function Wa(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function Ya(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = null != arguments[t] ? arguments[t] : {};
                t % 2 ? Wa(Object(n), !0).forEach((function(t) {
                    Object(C.a)(e, t, n[t])
                }
                )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : Wa(Object(n)).forEach((function(t) {
                    Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                }
                ))
            }
            return e
        }
        var Ka = Ua(Na)
          , qa = Ua(Fa, "NETWORK_TILE")
          , Ja = function(e) {
            var t = e.collection
              , n = e.collectionIndex
              , a = e.stateKey
              , i = e.itemsPendingRemoval
              , r = e.toggleTile
              , o = e.myStuffThemeConfig
              , c = e.pageType
              , l = t.id
              , s = ea()(t.items, (function(e) {
                return e.id
            }
            ))
              , d = R()(t, "name");
            return za(u.Fragment, {
                key: "Mystuff-Collection-".concat(l)
            }, za("h2", {
                className: "MyStuff__collection-title",
                id: t.id
            }, t.name), za("div", {
                className: "MyStuff__tiles"
            }, s.map((function(e, t) {
                return function(e, t, n, a, i, r, o, c, l, s) {
                    var u = R()(e, "id")
                      , d = na()(r, u)
                      , f = !Xt()(r)
                      , m = {
                        asset: Ya({}, e, {
                            collectionId: a
                        }),
                        assetIndex: t,
                        callback: o,
                        collectionId: a,
                        collectionIndex: i,
                        isSelected: d,
                        metricsParams: Object(ee.c)(t, {
                            id: a
                        }, i),
                        stateKey: n,
                        toggleVisible: f,
                        isReadOnly: f,
                        tealiumTileData: Object(Jn.g)(s, c),
                        pageContext: Va.f,
                        name: c
                    }
                      , p = "network" === R()(l, [a, "collection_theme"], "simple")
                      , h = za(p ? qa : Ka, m)
                      , b = B()({
                        MyStuff__tile: !p
                    }, {
                        "MyStuff__network-tile": p
                    });
                    return za("div", {
                        key: e.id,
                        className: b
                    }, h)
                }(e, t, a, l, n, i, r, d, o, c)
            }
            ))))
        }
          , Xa = (n("QPCc"),
        d.a.createElement)
          , Za = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "renderCollection", (function(e, t) {
                    var a = n.props
                      , i = a.stateKey
                      , r = a.itemsPendingRemoval
                      , o = a.toggleTile
                      , c = a.myStuffThemeConfig
                      , l = a.pageType
                      , s = a.metricsInfo
                      , d = R()(e, "items", [])
                      , f = {
                        onEnter: te.a,
                        waitFor: 250,
                        threshold: .1,
                        metricsParams: Object(ee.a)(e.id, t, i, s)
                    };
                    return Xa(u.Fragment, {
                        key: t
                    }, d.length > 0 && Xa("div", {
                        className: "MyStuff__collection"
                    }, Xa(U.a, f, Xa(Ja, {
                        collection: e,
                        stateKey: i,
                        collectionIndex: t,
                        itemsPendingRemoval: r,
                        toggleTile: o,
                        myStuffThemeConfig: c,
                        pageType: l
                    }))))
                }
                )),
                Object(C.a)(Object(D.a)(n), "manageDVR", (function() {
                    (0,
                    n.props.clearSelectedItems)(),
                    Object(p.c)("/manage-dvr"),
                    Object(ke.trackTealium)({
                        event_name: S.n.page_type.dvr_page_type
                    })
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.collections
                      , n = e.itemsPendingRemoval
                      , a = e.handleCancel
                      , i = e.handleRemove
                      , r = e.hasManageDvr
                      , o = Object.keys(n).length
                      , c = t.every((function(e) {
                        return 0 === e.items.length
                    }
                    ));
                    return Xa("div", {
                        className: "MyStuff"
                    }, Xa(zn, {
                        hasManageDvr: r,
                        openManageDVR: this.manageDVR
                    }), Xa(H.a, null, c ? Xa("div", {
                        className: "MyStuff__null-state"
                    }, Xa(Ee.view, {
                        from: "myStuffConfig.nullState.default"
                    })) : Xa("div", {
                        className: "MyStuff__collections"
                    }, t && t.map(this.renderCollection)), Xa(Qn, {
                        totalSelectedItems: o,
                        handleCancel: a,
                        handleRemove: i,
                        footerTextPath: "myStuffConfig.footer"
                    })))
                }
            }]),
            t
        }(d.a.Component)
          , Qa = n("mUg4")
          , $a = n.n(Qa)
          , ei = function(e) {
            $a()(e, (function(e) {
                var t = function(e) {
                    var t = e.itemId.toString();
                    return {
                        userInteractionProps: {
                            actionSpecifier: "mystuff:remove",
                            elementSpecifier: "remove",
                            displayName: e.itemName,
                            entityData: {
                                entityId: t,
                                entityActionId: t,
                                entityActionType: "remove_from_mystuff",
                                recoTags: ""
                            },
                            collectionData: {
                                collectionId: e.collectionId.toString(),
                                collectionIndex: e.collectionIndex,
                                collectionItemIndex: e.itemIndex,
                                collectionSource: Ue.c.COLLECTION_SOURCE.HEIMDALL
                            }
                        }
                    }
                }(e);
                Ue.d.trackUserInteraction(t.userInteractionProps)
            }
            ))
        }
          , ti = d.a.createElement
          , ni = function(e) {
            return function(t) {
                function n() {
                    var e, t;
                    Object(r.a)(this, n);
                    for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                        i[o] = arguments[o];
                    return t = Object(c.a)(this, (e = Object(l.a)(n)).call.apply(e, [this].concat(i))),
                    Object(C.a)(Object(D.a)(t), "state", {
                        itemsPendingRemoval: {},
                        seriesPendingRemoval: {}
                    }),
                    Object(C.a)(Object(D.a)(t), "clearSelectedItems", (function() {
                        t.setState({
                            itemsPendingRemoval: {},
                            seriesPendingRemoval: {}
                        })
                    }
                    )),
                    Object(C.a)(Object(D.a)(t), "handleCancel", (function() {
                        t.clearSelectedItems()
                    }
                    )),
                    Object(C.a)(Object(D.a)(t), "handleRemove", (function() {
                        (0,
                        t.props.removeCallback)(t.state.itemsPendingRemoval),
                        t.clearSelectedItems()
                    }
                    )),
                    Object(C.a)(Object(D.a)(t), "toggleTile", (function(e) {
                        var n = Object.assign({}, t.state.itemsPendingRemoval);
                        n[e.itemId] ? delete n[e.itemId] : n[e.itemId] = e,
                        t.setState({
                            itemsPendingRemoval: n
                        });
                        var a = Object(Jn.b)(t.props.pageType === S.j.MANAGE_DVR, S.n.my_stuff_step.icon);
                        Object(ke.trackTealium)({
                            device_category: S.n.device,
                            content_type: e.name.toLowerCase(),
                            event_name: a
                        })
                    }
                    )),
                    Object(C.a)(Object(D.a)(t), "toggleSeries", (function(e, n) {
                        var a = Object.assign({}, t.state.seriesPendingRemoval);
                        a[e.itemId] ? (delete a[e.itemId],
                        t.removeSeriesItems(e)) : (a[e.itemId] = e,
                        t.addSeriesItems(e)),
                        t.setState({
                            seriesPendingRemoval: a
                        });
                        var i = Object(Jn.b)(t.props.pageType === S.j.MANAGE_DVR, S.n.my_stuff_step.icon);
                        Object(ke.trackTealium)({
                            device_category: S.n.device,
                            content_type: e.name.toLowerCase(),
                            event_name: i
                        }),
                        n.stopPropagation()
                    }
                    )),
                    t
                }
                return Object(s.a)(n, t),
                Object(o.a)(n, [{
                    key: "removeSeriesItems",
                    value: function(e) {
                        var t = Object.assign({}, this.state.itemsPendingRemoval);
                        Object.keys(t).forEach((function(n) {
                            t[n].series_id !== e.itemId && t[n].id !== e.itemId || delete t[n]
                        }
                        )),
                        this.setState({
                            itemsPendingRemoval: t
                        })
                    }
                }, {
                    key: "addSeriesItems",
                    value: function(e) {
                        var t = Object.assign({}, this.state.itemsPendingRemoval);
                        e.recordedEpisodes.forEach((function(e) {
                            t[e.id] = e
                        }
                        )),
                        this.setState({
                            itemsPendingRemoval: t
                        })
                    }
                }, {
                    key: "render",
                    value: function() {
                        var t = {
                            itemsPendingRemoval: this.state.itemsPendingRemoval,
                            seriesPendingRemoval: this.state.seriesPendingRemoval,
                            clearSelectedItems: this.clearSelectedItems,
                            handleCancel: this.handleCancel,
                            handleRemove: this.handleRemove,
                            toggleTile: this.toggleTile,
                            toggleSeries: this.toggleSeries
                        };
                        return ti(e, Object(x.a)({}, this.props, t))
                    }
                }]),
                n
            }(d.a.Component)
        }
          , ai = n("MJIl")
          , ii = n.n(ai)
          , ri = n("WrGL");
        function oi(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        var ci = function() {
            var e = arguments.length > 0 && void 0 !== arguments[0] ? arguments[0] : [];
            return e.filter((function(e) {
                return "collection_theme_storage" !== e.theme
            }
            ))
        }
          , li = Object(J.createSelector)([function(e) {
            return R()(e[v.b], "collections")
        }
        , function(e) {
            return e[v.b]
        }
        , function(e) {
            return R()(e, [ri.c, "profiles"], [])
        }
        , function(e) {
            return R()(e, [ri.c, "currentProfileId"])
        }
        , function(e) {
            return R()(e, ["configurations", "hub_config", "myStuff", "collectionConfig"])
        }
        , function(e) {
            return e[v.b].pageType
        }
        , function(e) {
            return e[v.b].metrics_info
        }
        ], (function(e, t, n, a, i, r, o) {
            var c = Qt()(n, (function(e) {
                return e.id === a
            }
            ))
              , l = R()(c, "name", "")
              , s = ii()(e, (function(e) {
                return "108" === e.id
            }
            ));
            return function(e) {
                for (var t = 1; t < arguments.length; t++) {
                    var n = null != arguments[t] ? arguments[t] : {};
                    t % 2 ? oi(Object(n), !0).forEach((function(t) {
                        Object(C.a)(e, t, n[t])
                    }
                    )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : oi(Object(n)).forEach((function(t) {
                        Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                    }
                    ))
                }
                return e
            }({}, t, {
                title: R()(t, "name"),
                collections: ci(e),
                stateKey: v.b,
                profileName: l,
                hasManageDvr: s,
                myStuffThemeConfig: i,
                pageType: r,
                metricsInfo: o
            })
        }
        ))
          , si = Object(P.a)(Object(de.a)((function(e) {
            var t = e.geo;
            return {
                url: ne.b.default({
                    entity: "watch-later",
                    limit: 100
                }),
                geo: t
            }
        }
        )), Object(g.connect)(li, {
            initContent: b.initContentAndFetchAllItems,
            initTileStates: b.appendTileStates,
            setIsLoadingData: b.setIsLoadingData,
            removeCallback: function(e) {
                return function(t) {
                    t(Object(b.removeMyStuffItems)(e)),
                    ei(e)
                }
            }
        }), ue.a, ni)(Za)
          , ui = n("nYuY")
          , di = n("p3iw")
          , fi = n("KQm4")
          , mi = n("0f81").a
          , pi = (n("xKUQ"),
        d.a.createElement)
          , hi = function(e) {
            var t = e.isInstantOpen
              , n = e.searchBarInputText
              , a = e.selectedItemId
              , i = e.userEmail
              , r = e.userType
              , o = e.changeKeywords
              , c = e.clearSearchResults
              , l = Object(u.useRef)(null);
            Object(u.useEffect)((function() {
                s()
            }
            ), []),
            Object(u.useEffect)((function() {
                n || s()
            }
            ), [n]);
            var s = function() {
                l.current && l.current.focus()
            }
              , d = function() {
                c()
            };
            return pi("div", {
                className: "SearchBarContainer"
            }, pi(H.a, null, pi("div", {
                className: "SearchBar",
                role: "combobox",
                "aria-expanded": t,
                "aria-owns": "instantSearch",
                onKeyDown: function(e) {
                    e.key === S.d.ESCAPE && (d(),
                    e.preventDefault())
                }
            }, pi("input", {
                ref: l,
                type: "text",
                "aria-activedescendant": a,
                "aria-autoComplete": "list",
                placeholder: "Search",
                value: n,
                "aria-label": "Search",
                onChange: function(e) {
                    var t = e.target.value;
                    Xt()(t.trim()) ? d() : o(t, i, r)
                },
                className: "cu-search-input"
            }), pi("button", {
                className: n.length > 0 ? "SearchBar__ClearButton" : "SearchBar__ClearButton--Hidden",
                onClick: d,
                onKeyDown: function(e) {
                    e.key === S.d.ENTER && e.stopPropagation()
                }
            }, pi(Ee.view, {
                from: "search.searchBar.clear"
            })))))
        };
        hi.defaultProps = {
            keywords: ""
        };
        var bi = hi
          , vi = n("kvot")
          , gi = Object(g.connect)((function(e) {
            var t = e[di.d]
              , n = Object(vi.b)(e);
            return {
                searchBarInputText: t.searchBarInputText,
                selectIndex: t.selectIndex,
                userEmail: n.userEmail,
                userType: n.userType
            }
        }
        ), {
            changeKeywords: ui.n,
            clearSearchResults: ui.d
        })(bi)
          , yi = d.a.createElement
          , Oi = function(e) {
            function t(e) {
                var n;
                return Object(r.a)(this, t),
                (n = Object(c.a)(this, Object(l.a)(t).call(this, e))).clickItem = n.clickItem.bind(Object(D.a)(n)),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props.item;
                    return yi("button", {
                        className: "RecentSearch__Item",
                        onClick: this.clickItem
                    }, " ", e.text, " ")
                }
            }, {
                key: "clickItem",
                value: function() {
                    this.props.selectRecentSearchItem(this.props.item, this.props.index)
                }
            }]),
            t
        }(u.Component)
          , _i = (n("OkLi"),
        d.a.createElement)
          , ji = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    this.props.initRecentSearch()
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.recentSearchItems
                      , n = e.userEmail
                      , a = e.userType
                      , i = e.clearRecentSearch
                      , r = e.selectRecentSearchItem;
                    if (0 === t.length)
                        return null;
                    var o = function(e, t) {
                        r(e, t, n, a)
                    };
                    return _i("div", {
                        className: "RecentSearch"
                    }, _i("div", null, _i("h2", {
                        className: "SearchResultContainer__Title"
                    }, _i(Ee.view, {
                        from: "search.recentSearch.title"
                    })), _i("button", {
                        className: "RecentSearch__Clear",
                        onClick: i
                    }, _i(Ee.view, {
                        from: "search.recentSearch.clear"
                    }))), _i("div", {
                        className: "RecentSearch__ItemsContainer"
                    }, t.map((function(e, t) {
                        return _i("div", {
                            key: e.text,
                            className: "RecentSearch__ItemContainer"
                        }, _i(Oi, {
                            item: e,
                            selectRecentSearchItem: o,
                            index: t
                        }))
                    }
                    ))))
                }
            }]),
            t
        }(u.Component)
          , Ii = Object(g.connect)((function(e) {
            var t = e[di.d]
              , n = Object(vi.b)(e);
            return {
                recentSearchItems: t.recentSearchItems,
                userEmail: n.userEmail,
                userType: n.userType
            }
        }
        ), {
            initRecentSearch: ui.k,
            clearRecentSearch: ui.c,
            selectRecentSearchItem: ui.p
        })(ji)
          , wi = (n("dQ7B"),
        Ii)
          , Ti = n("yV38")
          , Si = (n("eZf8"),
        n("SgFH"))
          , Ei = d.a.createElement
          , ki = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    this.props.init()
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props.groups;
                    return 0 === e.length ? null : Ei("div", {
                        className: "ZeroSearch"
                    }, e.map((function(e, t) {
                        return Ei(Ti.a, {
                            collection: e,
                            key: e.type,
                            requireDetailPage: !0,
                            collectionMetricsParams: {
                                collectionName: Si.COLLECTION_NAME.ZERO_SEARCH,
                                collectionIndex: t,
                                pageName: Si.SEARCH_PAGE_NAME.ZERO_SEARCH
                            }
                        })
                    }
                    )))
                }
            }]),
            t
        }(u.Component)
          , Ci = n("Pn51")
          , Ni = (n("BQgp"),
        n("yPh+"),
        d.a.createElement)
          , Pi = function(e) {
            var t = e.name
              , n = e.displayType
              , a = e.collection;
            return Ni("div", {
                className: "RecoView"
            }, Ni("div", {
                className: "RecoView__title"
            }, Ni(Ee.view, {
                from: "search.recoView.title",
                isHTML: !0,
                replace: {
                    NAME: t,
                    YEAR: ""
                }
            })), Ni("div", {
                className: "RecoView__description"
            }, Ni(Ee.view, {
                from: "search.recoView.description",
                replace: {
                    TYPE: n
                }
            })), Ni(Ti.a, {
                collection: a,
                requireDetailPage: !0,
                collectionMetricsParams: {
                    collectionName: Si.COLLECTION_NAME.RECO,
                    collectionIndex: 0,
                    pageName: Si.SEARCH_PAGE_NAME.SEARCH_RECO
                }
            }))
        }
          , Di = n("H9OJ")
          , xi = n("/eAl")
          , Li = d.a.createElement
          , Ri = function(e) {
            var t, n = e.recoResult, a = n.items, i = n.entity;
            if (0 === a.length)
                return null;
            var r = Di.n(i)
              , o = Di.o(i)
              , c = (t = {},
            Object(C.a)(t, xi.a, "movies"),
            Object(C.a)(t, xi.b, "series"),
            t)[o]
              , l = {
                name: Li(Ee.view, {
                    from: "search.recoView.trayTitle",
                    replace: {
                        TYPE: c
                    }
                }),
                items: a,
                hubRelativeUrl: null
            };
            return Li(Pi, {
                displayType: c,
                name: r,
                collection: l
            })
        }
          , Mi = (n("dMi1"),
        d.a.createElement)
          , Ai = function() {
            return Mi("div", {
                className: "ExceptionText"
            }, Mi("div", {
                className: "ExceptionText--Large"
            }, Mi(Ee.view, {
                from: "search.exception.networkError",
                isHTML: !0
            })))
        }
          , Gi = n("eMJF")
          , Bi = d.a.createElement
          , Fi = function(e) {
            var t = e.clickItem
              , n = e.disableMouse
              , a = e.enableMouse
              , i = e.fullTextSearchData
              , r = e.initSearchId
              , o = e.initZeroSearch
              , c = e.instantSearchData
              , l = e.isMouseEnabled
              , s = e.keywords
              , d = e.networkError
              , f = e.offsiteSearchData
              , m = e.ongoingRequest
              , p = e.recoData
              , h = e.restoreSearchState
              , b = e.searchFullText
              , v = e.selectIndex
              , g = v.type
              , y = v.index
              , O = e.setSelectIndex
              , _ = e.zeroSearchData;
            Object(u.useEffect)((function() {
                return r(),
                h
            }
            ), [r, h]);
            var j, I, w = s === R()(c, "keywords") ? R()(c, "result") : [], T = s === R()(f, "keywords") ? R()(f, "result", []) : [], E = s === R()(i, "keywords") ? R()(i, "result") : [], k = !1, C = null != p, N = null != i;
            return 0 === s.length ? j = d ? Bi(Ai, null) : Bi("div", {
                className: "SearchResultContainer__BottomContainer"
            }, Bi(wi, null), Bi(ki, {
                groups: _,
                init: o
            })) : C ? j = Bi(Ri, {
                recoResult: p
            }) : N ? j = d ? Bi(Ai, null) : m || 0 !== E.length || 0 !== T.length ? Bi("div", {
                className: "SearchResultContainer__BottomContainer"
            }, Bi(Ci.b, {
                groups: E,
                offsiteItems: T
            })) : Bi(Gi.a, null) : (k = !0,
            I = Di.g([].concat(Object(fi.a)(w), Object(fi.a)(T))[y]),
            j = d ? Bi(Ai, null) : Bi(mi, {
                list: w,
                offsiteItems: T
            })),
            Bi(H.a, null, l || Bi("div", {
                className: "MouseBlocker",
                onMouseMove: a,
                "data-testid": "mouse-blocker"
            }), Bi("div", {
                className: "SearchContainer",
                onKeyDown: function(e) {
                    switch (e.key) {
                    case S.d.ARROW_UP:
                        g === di.a.ENTITY ? (y >= 0 && O({
                            type: di.a.ENTITY,
                            index: y - 1
                        }),
                        n()) : (y > 0 ? O({
                            type: di.a.OFFSITE,
                            index: y - 1
                        }) : 0 === y && O({
                            type: di.a.ENTITY,
                            index: w.length - 1
                        }),
                        n()),
                        e.preventDefault();
                        break;
                    case S.d.ARROW_DOWN:
                        g === di.a.ENTITY ? y < w.length - 1 ? (O({
                            type: di.a.ENTITY,
                            index: y + 1
                        }),
                        n()) : T.length > 0 && O({
                            type: di.a.OFFSITE,
                            index: 0
                        }) : y < T.length - 1 && (O({
                            type: di.a.OFFSITE,
                            index: y + 1
                        }),
                        n()),
                        e.preventDefault();
                        break;
                    case S.d.ENTER:
                        g === di.a.ENTITY && -1 === y ? b(s) : t({
                            type: g,
                            index: y
                        })
                    }
                },
                "data-testid": "search"
            }, Bi(gi, {
                selectedItemId: I,
                isInstantOpen: k
            }), Bi("div", {
                className: "SearchResultContainer"
            }, j)))
        };
        Fi.defaultProps = {
            keywords: "",
            ongoingRequest: !1,
            networkError: !1
        };
        var Hi = Fi
          , Ui = Object(g.connect)((function(e) {
            return e[di.d]
        }
        ), {
            initSearchId: ui.l,
            initZeroSearch: ui.m,
            clickItem: ui.e,
            enableMouse: ui.i,
            disableMouse: ui.h,
            restoreSearchState: ui.r,
            searchFullText: ui.o,
            setSelectIndex: ui.q
        })(Hi)
          , Vi = (n("C4cC"),
        n("GDm6"))
          , zi = (di.d,
        Vi.f.INIT_SEARCH,
        n("cWlG"))
          , Wi = n("87qS")
          , Yi = (n("fccu"),
        d.a.createElement)
          , Ki = function(e) {
            var t = e.bannerKey
              , n = e.dataReady
              , a = e.pageType
              , i = e.geo
              , r = e.areProfilesLoaded
              , o = e.isKidsProfile
              , c = e.heimdallEntity
              , l = e.heimdallEntityId
              , s = e.pageName
              , d = e.determineAction
              , f = e.isTopLevel
              , m = e.startTime
              , p = e.discoverHref
              , h = a === S.j.HOME
              , b = (a === S.j.HUB || a === S.j.HOME) && !n;
            if (Object(Wi.d)(f, v.b),
            o && h)
                return d({
                    entity: "hub",
                    id: "kids",
                    forcedRedirectToKids: !0
                }),
                null;
            var g = null;
            switch (a) {
            case S.j.SEARCH:
                g = Ui;
                break;
            case S.j.MY_STUFF:
                g = si;
                break;
            case S.j.LIVE_GUIDE:
                g = Ln;
                break;
            default:
                g = be
            }
            var y = {
                tealiumPageType: Object(Jn.c)(a, c),
                pageName: s,
                stateKey: v.b
            };
            return Yi(u.Fragment, null, Yi(E.view, {
                name: "appsFlyerBanner"
            }, t ? Yi(w, {
                bannerKey: t
            }) : null), Yi(E.view, {
                name: "installBanner"
            }, Yi(Ge, null)), Yi(E.view, {
                name: "ratingsPrompt"
            }, Yi(Bn, null)), Yi(T.a, {
                onHomeHub: h
            }), b && Yi(je, {
                isHomeHub: h
            }), Yi("div", {
                className: "LevelOne cu-levelone",
                "aria-busy": b,
                "aria-hidden": !f,
                "data-testid": "level-one-content"
            }, r ? Yi(zi.a, y, Yi("div", {
                id: "main-content"
            }), a && Yi(g, {
                className: "LevelOne__content",
                geo: i,
                pageType: a,
                heimdallEntity: c,
                heimdallEntityId: l,
                startTime: m,
                isTopLevel: f,
                discoverHref: p
            })) : null, Yi(k.a, null)))
        }
          , qi = n("beww")
          , Ji = Object(g.connect)((function(e) {
            return Object(qi.a)(e)
        }
        ), {
            determineAction: b.determineAction
        })(Ki)
          , Xi = n("7tLf")
          , Zi = n("RnOw")
          , Qi = d.a.createElement
          , $i = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "clickActivities", (function() {
                    var e = n.props
                      , t = e.navTitle
                      , a = e.index
                      , i = e.clickFunction
                      , r = e.setActiveFunction;
                    i(a, t),
                    r(n.btn.offsetWidth, n.btn.offsetLeft)
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    var e = this.props
                      , t = e.active
                      , n = e.setActiveFunction;
                    t && n(this.btn.offsetWidth, this.btn.offsetLeft)
                }
            }, {
                key: "render",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.navTitle
                      , a = t.index
                      , i = t.active;
                    return Qi("button", {
                        key: a,
                        className: B()("Subnav__item", {
                            active: i
                        }),
                        onClick: this.clickActivities,
                        role: "tab",
                        "aria-controls": "".concat(n, "-tab"),
                        id: n,
                        ref: function(t) {
                            e.btn = t
                        },
                        tabIndex: i ? 0 : -1,
                        "aria-selected": i
                    }, n)
                }
            }]),
            t
        }(u.Component)
          , er = (n("rbDy"),
        d.a.createElement)
          , tr = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "itemsElement", void 0),
                Object(C.a)(Object(D.a)(n), "state", {
                    activeWidth: 0,
                    activeLeft: 0,
                    isActiveBarVisible: !0,
                    clientWidth: 0,
                    scrollWidth: 0,
                    scrollLeft: 0
                }),
                Object(C.a)(Object(D.a)(n), "setActiveFunction", (function(e, t) {
                    n.setState({
                        activeWidth: e,
                        activeLeft: t,
                        isActiveBarVisible: !0
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "clickFunction", (function(e, t) {
                    var a = n.props
                      , i = a.switchCollection
                      , r = a.pageType;
                    i(e, !1),
                    Object(ke.trackTealium)(Object(Jn.e)(r, t))
                }
                )),
                Object(C.a)(Object(D.a)(n), "recalculate", (function(e) {
                    if (n.itemsElement) {
                        var t = n.itemsElement
                          , a = t.clientWidth
                          , i = t.scrollWidth
                          , r = t.scrollLeft;
                        n.setState({
                            clientWidth: a,
                            scrollWidth: i,
                            scrollLeft: void 0 !== e ? e : r
                        })
                    }
                }
                )),
                Object(C.a)(Object(D.a)(n), "scroll", (function(e) {
                    n.itemsElement.scrollBy({
                        left: e,
                        behavior: "smooth"
                    }),
                    n.recalculate(n.itemsElement.scrollLeft + e)
                }
                )),
                Object(C.a)(Object(D.a)(n), "onResize", (function() {
                    n.recalculate(),
                    n.setState({
                        isActiveBarVisible: !1
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "onKeyDown", (function(e) {
                    He(e),
                    n.recalculate()
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    n("dwco").polyfill(),
                    window.addEventListener("resize", this.onResize),
                    this.recalculate()
                }
            }, {
                key: "componentWillUnmount",
                value: function() {
                    window.removeEventListener("resize", this.onResize)
                }
            }, {
                key: "renderItem",
                value: function(e, t) {
                    var n = this.props.selectedNavIndex;
                    return er($i, {
                        navTitle: e,
                        index: t,
                        active: t === n,
                        clickFunction: this.clickFunction,
                        setActiveFunction: this.setActiveFunction,
                        key: "SubnavButton_".concat(e)
                    })
                }
            }, {
                key: "render",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.navTitles
                      , a = t.paddleLayout
                      , i = t.color
                      , r = t.ariaLabel
                      , o = this.state
                      , c = o.activeWidth
                      , l = o.activeLeft
                      , s = o.isActiveBarVisible
                      , u = o.clientWidth
                      , d = o.scrollWidth
                      , f = o.scrollLeft
                      , m = d > u
                      , p = f > 0
                      , h = f < d - u
                      , b = {
                        transform: "translateX(".concat(l, "px)"),
                        width: c,
                        backgroundColor: i,
                        visibility: s ? "visible" : "hidden"
                    };
                    return n ? er("div", {
                        className: B()("Subnav", {
                            "Subnav--scrollable": m,
                            "Subnav--scrollable-left": p,
                            "Subnav--scrollable-right": h,
                            "Subnav--wrapping": "wrap" === a
                        })
                    }, er("div", {
                        className: "Subnav__wrapper"
                    }, er("div", {
                        className: "Subnav__items",
                        ref: function(t) {
                            return e.itemsElement = t
                        },
                        role: "tablist",
                        onKeyDown: this.onKeyDown,
                        "aria-label": r
                    }, n.map(this.renderItem, this), er("span", {
                        className: "Subnav__accent-bar",
                        style: b
                    }))), er("button", {
                        className: "Subnav__paddle Subnav__paddle--left",
                        disabled: !p,
                        onClick: function() {
                            return e.scroll(-u)
                        }
                    }, Object(Hn.f)()), er("button", {
                        className: "Subnav__paddle Subnav__paddle--right",
                        disabled: !h,
                        onClick: function() {
                            return e.scroll(u)
                        }
                    }, Object(Hn.g)())) : null
                }
            }]),
            t
        }(u.Component)
          , nr = (n("5n5k"),
        d.a.createElement)
          , ar = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    isOpen: !1,
                    focusedIndex: 0
                }),
                Object(C.a)(Object(D.a)(n), "mouseDown", !1),
                Object(C.a)(Object(D.a)(n), "triggerButton", Object(u.createRef)()),
                Object(C.a)(Object(D.a)(n), "menuList", Object(u.createRef)()),
                Object(C.a)(Object(D.a)(n), "onMouseDown", (function(e) {
                    n.mouseDown = !0,
                    e.preventDefault()
                }
                )),
                Object(C.a)(Object(D.a)(n), "onToggleMenu", (function() {
                    n.mouseDown = !1,
                    n.setState((function(e) {
                        return {
                            isOpen: !e.isOpen
                        }
                    }
                    ), (function() {
                        n.state.isOpen && (n.menuList.current.focus(),
                        n.scrollIntoView())
                    }
                    ))
                }
                )),
                Object(C.a)(Object(D.a)(n), "closeMenu", (function() {
                    n.mouseDown || n.setState({
                        isOpen: !1
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "scrollIntoView", (function() {
                    var e = n.menuList.current
                      , t = e.querySelector(".Select__option--is-focused");
                    if (e.scrollHeight > e.clientHeight) {
                        var a = e.clientHeight + e.scrollTop
                          , i = t.offsetTop + t.offsetHeight;
                        i > a ? e.scrollTop = i - e.clientHeight : t.offsetTop < e.scrollTop && (e.scrollTop = t.offsetTop)
                    }
                }
                )),
                Object(C.a)(Object(D.a)(n), "onKeyDown", (function(e) {
                    var t = n.props.options.length - 1;
                    switch (e.key) {
                    case S.d.ARROW_UP:
                        e.preventDefault(),
                        n.setState((function(e) {
                            var t = e.focusedIndex;
                            return {
                                focusedIndex: Math.max(t - 1, 0)
                            }
                        }
                        ), n.scrollIntoView);
                        break;
                    case S.d.ARROW_DOWN:
                        e.preventDefault(),
                        n.setState((function(e) {
                            var n = e.focusedIndex;
                            return {
                                focusedIndex: Math.min(n + 1, t)
                            }
                        }
                        ), n.scrollIntoView);
                        break;
                    case S.d.HOME:
                        e.preventDefault(),
                        n.setState({
                            focusedIndex: 0
                        }, n.scrollIntoView);
                        break;
                    case S.d.END:
                        e.preventDefault(),
                        n.setState({
                            focusedIndex: t
                        }, n.scrollIntoView);
                        break;
                    case S.d.ENTER:
                    case S.d.SPACE:
                        e.preventDefault(),
                        n.onSelect({
                            returnFocus: !0
                        })
                    }
                }
                )),
                Object(C.a)(Object(D.a)(n), "onSelect", (function(e) {
                    var t = e.returnFocus
                      , a = n.props;
                    (0,
                    a.onChange)({
                        value: a.options[n.state.focusedIndex].value
                    }),
                    n.setState({
                        isOpen: !1
                    }, (function() {
                        t && n.triggerButton.current.focus()
                    }
                    ))
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.options
                      , n = e.value;
                    if (1 === t.length)
                        return nr("div", {
                            className: "Select Select--single"
                        }, nr("div", {
                            className: "Select__control"
                        }, nr("div", {
                            className: "Select__single-value"
                        }, t[0].label)));
                    var a = this.state.isOpen
                      , i = B()("Select__control", {
                        "Select__control--menu-is-open": a
                    })
                      , r = t.find((function(e) {
                        return e.value === n
                    }
                    )).label;
                    return nr("div", {
                        className: "Select"
                    }, nr("button", {
                        className: i,
                        onMouseDown: this.onMouseDown,
                        onClick: this.onToggleMenu,
                        "aria-haspopup": "listbox",
                        ref: this.triggerButton
                    }, nr("div", {
                        className: "Select__single-value"
                    }, r), nr("div", {
                        "aria-hidden": "true",
                        className: "Select__dropdown-indicator"
                    })), a && this.renderMenu())
                }
            }, {
                key: "renderMenu",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.options
                      , a = t.value
                      , i = this.state.focusedIndex
                      , r = n[Math.min(i, n.length - 1)].value;
                    return nr("ul", {
                        role: "listbox",
                        tabIndex: "-1",
                        className: "Select__menu-list",
                        ref: this.menuList,
                        onKeyDown: this.onKeyDown,
                        onBlur: this.closeMenu,
                        "aria-activedescendant": r
                    }, n.map((function(t, n) {
                        return nr("li", {
                            role: "option",
                            "aria-selected": t.value === a,
                            id: t.value,
                            key: t.value,
                            onMouseEnter: function() {
                                return e.setState({
                                    focusedIndex: n
                                })
                            },
                            onClick: e.onSelect,
                            className: B()("Select__option", {
                                "Select__option--is-focused": r === t.value
                            })
                        }, t.label)
                    }
                    )))
                }
            }]),
            t
        }(u.Component)
          , ir = (n("wA92"),
        d.a.createElement)
          , rr = function(e) {
            var t = e.items
              , n = e.onChange
              , a = e.currentItem
              , i = e.isStickyInNav
              , r = t.map((function(e) {
                return {
                    value: e.id,
                    label: e.name
                }
            }
            ));
            if (0 === t.length)
                return null;
            var o = B()("DetailsDropdown", {
                "DetailsDropdown--sticky": i
            });
            return ir("div", {
                className: o
            }, ir("div", {
                className: "DetailsDropdown__container",
                "data-testid": "details-dropdown-container"
            }, ir(ar, {
                options: r,
                value: a,
                onChange: n
            })))
        }
          , or = n("Nnso")
          , cr = n("am2Y")
          , lr = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "scrollListener", (function() {
                    var e = n.props
                      , t = e.threshold
                      , a = e.loadMore
                      , i = e.collectionIndex;
                    n.scrollComponent.scrollHeight - n.scrollEl.scrollTop - n.scrollEl.clientHeight < Number(t) && (n.detachScrollListener(),
                    a(i))
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    this.pageLoaded = this.props.pageStart,
                    this.attachScrollListener()
                }
            }, {
                key: "componentDidUpdate",
                value: function() {
                    this.attachScrollListener()
                }
            }, {
                key: "componentWillUnmount",
                value: function() {
                    this.detachScrollListener()
                }
            }, {
                key: "detachScrollListener",
                value: function() {
                    var e = this.props.useCapture
                      , t = this.scrollEl;
                    t && (t.removeEventListener("scroll", this.scrollListener, e),
                    t.removeEventListener("resize", this.scrollListener, e))
                }
            }, {
                key: "attachScrollListener",
                value: function() {
                    var e = this.props
                      , t = e.hasMore
                      , n = e.scrollElement
                      , a = e.useCapture;
                    t && (this.scrollEl = document.querySelector(n),
                    this.scrollEl.addEventListener("scroll", this.scrollListener, a),
                    this.scrollEl.addEventListener("resize", this.scrollListener, a),
                    this.scrollListener())
                }
            }, {
                key: "render",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.children
                      , a = t.element
                      , i = t.hasMore
                      , r = t.loader
                      , o = Object(N.a)(t, ["children", "element", "hasMore", "loader"]);
                    delete o.loadMore,
                    delete o.pageStart,
                    delete o.threshold,
                    delete o.useCapture,
                    delete o.scrollElement,
                    o.ref = function(t) {
                        e.scrollComponent = t
                    }
                    ;
                    var c = [n];
                    return i && r && c.push(r),
                    d.a.createElement.apply(d.a, [a, o].concat(c))
                }
            }]),
            t
        }(u.Component);
        lr.defaultProps = {
            element: "div",
            hasMore: !1,
            pageStart: 0,
            threshold: 250,
            useCapture: !1,
            loader: null
        };
        var sr = lr
          , ur = n("RFCZ")
          , dr = (n("BkQg"),
        n("50VT"))
          , fr = d.a.createElement;
        function mr(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function pr(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = null != arguments[t] ? arguments[t] : {};
                t % 2 ? mr(Object(n), !0).forEach((function(t) {
                    Object(C.a)(e, t, n[t])
                }
                )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : mr(Object(n)).forEach((function(t) {
                    Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                }
                ))
            }
            return e
        }
        var hr = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "removeTilesFromAllStates",
                value: function(e) {
                    var t = this.props
                      , n = t.removeItemFromViewAll
                      , a = t.removeItemFromCollections
                      , i = e.id;
                    n(i),
                    a(i)
                }
            }, {
                key: "mapItemToTile",
                value: function(e, t) {
                    var n = this
                      , a = this.props
                      , i = a.collectionId
                      , r = a.collectionIndex
                      , o = a.stateKey
                      , c = a.pageType
                      , l = a.tilePageContext
                      , s = a.hasSeasonDropDown
                      , d = a.excludedProps
                      , f = a.coverStoryMetricsInfo
                      , m = {
                        asset: pr({}, e, {
                            collectionId: i
                        }),
                        stateKey: o,
                        metricsParams: Object(ee.c)(t, {
                            id: i
                        }, r, null, e.metrics_info),
                        tealiumTileData: Object(Jn.f)(c),
                        removedTileCallback: function() {
                            return n.removeTilesFromAllStates(e)
                        },
                        pageContext: l,
                        hasSeasonDropDown: s,
                        excludedProps: d,
                        index: t,
                        collectionIndex: r
                    };
                    R()(e, "bundle.eab_id") && f && (m.coverStoryMetricsInfo = f);
                    var p = Object(cr.i)(e) ? fr(ur.a, {
                        metricsParams: Object(ee.b)(t, {
                            id: i
                        }, r, o, e, mt.TILE_SIZE.GENERIC_COVER_STORY)
                    }, fr(u.Fragment, null, fr(dr.a, {
                        name: "updatedGenericTile"
                    }, fr(or.a, m)), fr(dr.a, {
                        name: "updatedGenericTile",
                        renderWhenOff: !0
                    }, fr(Et.a, m)))) : fr(u.Fragment, null, fr(dr.a, {
                        name: "updatedGenericTile"
                    }, fr(or.a, m)), fr(dr.a, {
                        name: "updatedGenericTile",
                        renderWhenOff: !0
                    }, fr(Et.a, m)));
                    return fr("div", {
                        key: "".concat(e.id, "-").concat(t),
                        className: "AllUpGrid__generic-tile"
                    }, p)
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.items
                      , n = e.loadMoreItems
                      , a = e.canLoadMore;
                    return fr("div", {
                        className: "AllUpGrid"
                    }, n ? fr(sr, {
                        pageStart: 0,
                        loadMore: n,
                        hasMore: a,
                        loader: fr("div", {
                            className: "loader"
                        }, fr(Ee.view, {
                            from: "allUpGrid.loading"
                        })),
                        scrollElement: ".LevelTwo__scroll-area"
                    }, t.map(this.mapItemToTile, this)) : fr("div", null, t.map(this.mapItemToTile, this)))
                }
            }]),
            t
        }(u.Component);
        function br(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        var vr = Object(g.connect)((function(e, t) {
            var n = R()(e, [v.b, "blacklisted"], []);
            return function(e) {
                for (var t = 1; t < arguments.length; t++) {
                    var n = null != arguments[t] ? arguments[t] : {};
                    t % 2 ? br(Object(n), !0).forEach((function(t) {
                        Object(C.a)(e, t, n[t])
                    }
                    )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : br(Object(n)).forEach((function(t) {
                        Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                    }
                    ))
                }
                return e
            }({}, t, {
                items: R()(t, "items", []).filter((function(e) {
                    var t = R()(e, "actions.stop_suggesting_this.entity_id")
                      , a = R()(e, "actions.remove_from_watch_history.entity_id");
                    return !n.includes(e.id) && !n.includes(t) && !n.includes(a)
                }
                ))
            })
        }
        ), {
            removeItemFromCollections: b.removeItemFromCollections,
            removeItemFromViewAll: Xi.removeItemFromViewAll
        })(hr)
          , gr = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    return this.props.children
                }
            }, {
                key: "componentDidMount",
                value: function() {
                    this.sendCollectionImpression(),
                    this.sendCollectionItemImpression()
                }
            }, {
                key: "componentDidUpdate",
                value: function(e) {
                    this.props.collectionId !== e.collectionId && (this.sendCollectionImpression(),
                    this.sendCollectionItemImpression())
                }
            }, {
                key: "sendCollectionImpression",
                value: function() {
                    var e = this.props
                      , t = e.collectionId
                      , n = e.collectionIndex
                      , a = e.metricsInfo;
                    t && Ue.d.trackCollectionImpression({
                        collectionId: t,
                        collectionIndex: n,
                        metricsInfo: a
                    })
                }
            }, {
                key: "sendCollectionItemImpression",
                value: function() {
                    var e = this.props
                      , t = e.collectionId
                      , n = e.item;
                    t && n && Ue.d.trackCollectionItemImpression(this.getItemMetricsParams())
                }
            }, {
                key: "getItemMetricsParams",
                value: function() {
                    var e = this.props
                      , t = e.collectionId
                      , n = e.collectionIndex
                      , a = e.item
                      , i = {
                        collectionId: t,
                        collectionIndex: n,
                        entityId: R()(a, "id"),
                        recoTags: R()(a, "metrics_info.reco_tags", ""),
                        collectionItemIndex: 0
                    }
                      , r = R()(a, "bundle.eab_id");
                    r && (i.eabId = r,
                    i.entityId = r);
                    var o = R()(a, "metrics_info.airing_type");
                    return o && (i.airingType = o),
                    i
                }
            }]),
            t
        }(d.a.Component)
          , yr = d.a.createElement
          , Or = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    previouslySelectedGroupId: null,
                    selectedGroupId: n.props.initiallyFocusedGroupId,
                    isDropdownVisible: !1,
                    isGridVisible: !1
                }),
                Object(C.a)(Object(D.a)(n), "scrollElement", null),
                Object(C.a)(Object(D.a)(n), "dropdownRef", Object(u.createRef)()),
                Object(C.a)(Object(D.a)(n), "gridRef", Object(u.createRef)()),
                Object(C.a)(Object(D.a)(n), "setItem", (function(e) {
                    var t = n.props
                      , a = t.name
                      , i = t.groupsOfItems
                      , r = t.fetchAllItemsForOneCollection
                      , o = t.isStickyDropdown
                      , c = e.value
                      , l = i.find((function(e) {
                        return e.id === c
                    }
                    ));
                    if (Object(ke.trackTealium)({
                        event_name: "".concat(Object(Jn.c)(S.j.DETAILS), "_").concat(S.n.suffix.season_dropdown),
                        show_select: a,
                        season_number: R()(l, ["series_grouping_metadata", "season_number"])
                    }),
                    0 === l.items.length && r(l),
                    n.setState({
                        selectedGroupId: c,
                        previouslySelectedGroupId: n.state.selectedGroupId
                    }),
                    o) {
                        var s = n.dropdownRef.current.offsetTop - 200;
                        n.scrollElement.scrollTo({
                            top: s,
                            behavior: "smooth"
                        })
                    }
                }
                )),
                Object(C.a)(Object(D.a)(n), "onDropdownVisible", (function() {
                    n.setState({
                        isDropdownVisible: !0
                    }, n.potentiallyChangeStickyState)
                }
                )),
                Object(C.a)(Object(D.a)(n), "onDropdownOffscreen", (function() {
                    n.setState({
                        isDropdownVisible: !1
                    }, n.potentiallyChangeStickyState)
                }
                )),
                Object(C.a)(Object(D.a)(n), "onGridVisible", (function() {
                    n.setState({
                        isGridVisible: !0
                    }, n.potentiallyChangeStickyState)
                }
                )),
                Object(C.a)(Object(D.a)(n), "onGridOffscreen", (function() {
                    n.setState({
                        isGridVisible: !1
                    }, n.potentiallyChangeStickyState)
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    n("dwco").polyfill(),
                    n("Wr5T"),
                    this.scrollElement = document.getElementById("LevelTwo__scroll-area"),
                    this.observer = new V.a,
                    this.observer.add(1, this.dropdownRef.current, this.onDropdownVisible, this.onDropdownOffscreen),
                    this.observer.add(.01, this.gridRef.current, this.onGridVisible, this.onGridOffscreen)
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.state
                      , t = e.selectedGroupId
                      , n = e.previouslySelectedGroupId
                      , a = this.props
                      , i = a.groupsOfItems
                      , r = a.collectionIndex
                      , o = a.collectionId
                      , c = a.stateKey
                      , l = a.pageType
                      , s = a.isStickyDropdown
                      , u = a.isFetchingOneCollection
                      , d = a.coverStoryMetricsInfo
                      , f = t;
                    u && n && (f = n);
                    var m = i && i.find((function(e) {
                        return e.id === f
                    }
                    ))
                      , p = m && m.items || [];
                    return yr(gr, {
                        collectionIndex: r,
                        collectionId: o
                    }, yr("div", {
                        ref: this.dropdownRef
                    }, yr(rr, {
                        items: i,
                        onChange: this.setItem,
                        currentItem: t,
                        isStickyInNav: s
                    })), yr("div", {
                        ref: this.gridRef
                    }, yr(vr, {
                        items: p,
                        collectionId: o,
                        collectionIndex: r,
                        stateKey: c,
                        pageType: l,
                        tilePageContext: Va.c,
                        hasSeasonDropDown: !0,
                        coverStoryMetricsInfo: d
                    })))
                }
            }, {
                key: "potentiallyChangeStickyState",
                value: function() {
                    var e = this.props.setStickyDropdown
                      , t = this.state
                      , n = t.isDropdownVisible
                      , a = t.isGridVisible;
                    e(!n && a)
                }
            }]),
            t
        }(u.Component)
          , _r = Object(g.connect)((function(e) {
            return {
                isFetchingOneCollection: R()(e, [Zi.b, "isFetchingOneCollection"])
            }
        }
        ), {
            fetchAllItemsForOneCollection: Xi.fetchAllItemsForOneCollection
        })(Or)
          , jr = (n("Z4V3"),
        n("wig9"))
          , Ir = n.n(jr)
          , wr = n("7dt2")
          , Tr = n("5tfM")
          , Sr = n("xRt6");
        function Er(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        var kr = function(e) {
            var t = e["detail.horizontal.hero"] || e["team.logo"] || {};
            return {
                hue: R()(t, "accent.hue"),
                url: t.path
            }
        }
          , Cr = function(e) {
            var t = e ? 50 : 12
              , n = e ? 40 : 17;
            return "hsl(".concat(e || 62, ", ").concat(t, "%, ").concat(n, "%)")
        }
          , Nr = function() {
            var e = arguments.length > 0 && void 0 !== arguments[0] ? arguments[0] : void 0
              , t = void 0 !== e ? e : 62
              , n = 100
              , a = 60;
            return "hsl(".concat(t, ", ").concat(n, "%, ").concat(a, "%)")
        }
          , Pr = function(e) {
            var t = e.artwork
              , n = R()(e, "_type");
            return n === Tr.k ? R()(t, ["team.vertical.background"], null) : n === Tr.i ? R()(t, ["detail.vertical.hero"], null) : R()(t, ["program.vertical.tile"], null) || R()(t, ["detail.vertical.hero"], null)
        }
          , Dr = function(e, t, n) {
            var a = R()(t, "premiere_date")
              , i = R()(t, "rating.code")
              , r = Object(Sr.a)(R()(t, "genre_names"))
              , o = R()(t, "league_name")
              , c = R()(n, "name")
              , l = R()(t, "name")
              , s = R()(n, "duration")
              , u = [];
            switch (e) {
            case Tr.f:
                u = Ir()([i, r, "Movie", kt.l(a), kt.g(s)]);
                break;
            case Tr.h:
                u = Ir()([i, r, "TV Series", kt.l(a)]);
                break;
            case Tr.i:
                u = Ir()([o]);
                break;
            case Tr.k:
                u = c && l !== c ? Ir()([o, c]) : Ir()([o])
            }
            return u
        }
          , xr = function(e, t, n) {
            var a = R()(t, "artwork", {})
              , i = R()(e, "artwork", {});
            return n === Tr.k && kr(i).hue ? kr(i) : kr(a)
        }
          , Lr = tt((function(e, t, n, a, i, r, o) {
            var c = wr.b(e, n, !1, t)
              , l = c && c.isPlayable
              , s = R()(e, "id")
              , u = l ? "/watch/".concat(s) : null
              , d = R()(a, "name")
              , f = R()(e, "description") || R()(a, "description")
              , m = Object(Ct.b)({
                availability: c,
                duration: null
            })
              , p = wr.i(e, c)
              , h = R()(a, "_type")
              , b = wr.a(e, c, t)
              , v = R()(e, "bundle.channel_id")
              , g = function(e, t, n, a, i) {
                var r = t && t.isLive
                  , o = t && t.isPlayable
                  , c = t && t.isUpcoming && o
                  , l = R()(n, "watchActionText")
                  , s = R()(n, "relationship")
                  , u = R()(n, "_type")
                  , d = R()(n, "season")
                  , f = R()(n, "number")
                  , m = d > 0 && f > 0 ? "S".concat(d, " E").concat(f) : null
                  , p = R()(n, ["bundle", "network_name"]) || R()(n, ["primary_branding", "name"])
                  , h = l || i;
                return "trailer" === s || u === Tr.b ? R()(a, "watchTrailer") : r || c ? "".concat(R()(a, "liveOn"), " ").concat(p) : e === Tr.h && m ? "".concat(h, ": ").concat(m) : h
            }(h, c, e, i, b)
              , y = R()(e, "bundle.eab_id")
              , O = R()(e, "metrics_info")
              , _ = Object(ft.g)({
                title: d,
                eabId: y,
                playbackMetricsInfo: O,
                watchUrl: u,
                entityId: R()(a, "id"),
                metricsParams: r,
                coverStoryMetricsInfo: o
            });
            return function(e) {
                for (var t = 1; t < arguments.length; t++) {
                    var n = null != arguments[t] ? arguments[t] : {};
                    t % 2 ? Er(Object(n), !0).forEach((function(t) {
                        Object(C.a)(e, t, n[t])
                    }
                    )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : Er(Object(n)).forEach((function(t) {
                        Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                    }
                    ))
                }
                return e
            }({
                menuActions: Object(Nt.b)(d, h, "EAB::".concat(R()(a, "id"), "::NULL::NULL"), R()(e, "bundle.rights.recordable", !1)),
                minLeft: p,
                seriesArtwork: Pr(a),
                metadata: Dr(h, a, e),
                action: b,
                customWatchActionText: g,
                backgroundHero: xr(e, a, h),
                watermark: R()(a, "primary_branding.artwork['brand.watermark.bottom.right'].path", "")
            }, m, {
                availability: c,
                title: d,
                description: f,
                watchUrl: u,
                channelId: v,
                eabId: y,
                watchUrlUserInteractionData: _
            })
        }
        ))
          , Rr = n("f0LI")
          , Mr = (n("AnWQ"),
        d.a.createElement)
          , Ar = function(e) {
            var t, n = e.hue, a = e.action, i = e.subtitle, r = e.networkName, o = e.watchUrl, c = e.statusBadge, l = e.entityId, s = e.channelId, d = e.eabId, f = e.watchUrlUserInteractionData, m = e.metricsParams, p = e.availability, h = !!o, b = "upcoming_items" === R()(m, "collectionId", ""), v = p && p.isLive, g = h && b, y = (t = g ? r ? "".concat(a, " ON ").concat(r.toUpperCase()) : a : r ? Object(Sr.b)(r.toUpperCase(), p) : "") ? "".concat(t, " - ").concat(i) : "".concat(i), O = "hsla(".concat(n, ", 50%, 40%, 1)"), _ = {
                entityId: l,
                collectionId: R()(m, "collectionId", ""),
                channelId: s,
                eabId: d,
                isPlayable: h,
                isLive: v
            }, j = Mr(u.Fragment, null, Mr("div", {
                className: "Banner__circle"
            }, h ? Mr("div", {
                className: "Banner__logo--live"
            }, Mr(Hn.v, null)) : Mr("div", {
                className: "Banner__logo--upcoming"
            })), Mr("div", {
                className: "Banner__title"
            }, y), c && Mr(ma.a, {
                text: c.text,
                modifiers: c.classNames
            }), !g && Mr("div", {
                className: "Banner__time"
            }, a));
            return h ? Mr(oa.a, {
                className: "Banner Banner--hover",
                href: o,
                videoData: _,
                userInteractionData: f
            }, Mr("div", {
                className: "Banner__base",
                style: {
                    backgroundColor: O
                }
            }, j), Mr("div", {
                className: "Banner__overlay"
            })) : Mr("div", {
                className: "Banner"
            }, Mr("div", {
                className: "Banner__base",
                style: {
                    backgroundColor: O
                }
            }, j), Mr("div", {
                className: "Banner__overlay"
            }))
        }
          , Gr = Object(P.a)(ia.a, ra.a, aa.a)(Ar)
          , Br = (n("qXrZ"),
        n("zTeD"))
          , Fr = (n("rQI9"),
        d.a.createElement)
          , Hr = function(e) {
            var t = e.artwork
              , n = t.path
              , a = R()(t, "accent.hue", 0)
              , i = "url(".concat(Object(ba.b)(n, 1280), ")")
              , r = "hsla(".concat(a, ", 50%, 40%, 0)")
              , o = "hsla(".concat(a, ", 50%, 40%, 1)")
              , c = "linear-gradient(80deg, ".concat(o, " 10%, ").concat(r, " 20%), ").concat(i);
            return Fr("div", {
                className: "Background",
                style: {
                    "--hue": a
                }
            }, Fr("div", {
                className: "Background__base"
            }), Fr("div", {
                className: "Background__art",
                style: {
                    backgroundImage: c
                }
            }), Fr("div", {
                className: "Background__gradient"
            }), Fr("div", {
                className: "Background__highlight"
            }), Fr("div", {
                className: "Background__scrim"
            }))
        }
          , Ur = d.a.createElement
          , Vr = function(e) {
            if (e)
                return {
                    borderBottomColor: Nr(e)
                }
        }
          , zr = function(e) {
            return Yr(e, "time")
        }
          , Wr = function(e) {
            return Yr(e, "status")
        }
          , Yr = function(e, t) {
            if (!e)
                return null;
            var n = e.text
              , a = e.classNames
              , i = "Masthead__badge Masthead__badge--".concat(t);
            return Ur("div", {
                className: i
            }, Ur(ma.a, {
                text: n,
                modifiers: a
            }))
        }
          , Kr = function(e) {
            if (e && 0 !== e.length)
                return Ur("ul", {
                    className: "Masthead__metadata"
                }, e.map((function(e) {
                    return Ur("li", {
                        key: e,
                        className: "Masthead__meta"
                    }, e)
                }
                )))
        }
          , qr = function(e, t, n) {
            var a = e && e.progress
              , i = n ? fa.b : fa.d;
            if (a && !(a <= 0))
                return Ur("div", {
                    className: "Masthead__statusContent"
                }, Ur(da.a, {
                    className: "Masthead__statusBar",
                    progressClass: i,
                    progress: a
                }), t && Ur("div", {
                    className: "Masthead__minLeft"
                }, t))
        }
          , Jr = function(e) {
            var t = e.playableOrUpcomingAsset
              , n = e.pageContext
              , a = e.mastheadTileState
              , i = e.detailEntity
              , r = e.configText
              , o = e.metricsParams
              , c = e.coverStoryMetricsInfo
              , l = Lr(t, n, a, i, r, o, c)
              , s = l.timeBadge
              , d = l.metadata
              , f = l.minLeft
              , m = l.statusBadge
              , p = l.description
              , h = l.availability
              , b = l.action
              , v = l.title
              , g = l.seriesArtwork
              , y = l.customWatchActionText
              , O = l.backgroundHero
              , _ = l.watermark
              , j = l.menuActions
              , I = l.watchUrl
              , w = l.channelId
              , T = l.eabId
              , S = l.watchUrlUserInteractionData
              , E = R()(O, "hue")
              , k = {
                accent: {
                    hue: E
                },
                path: R()(O, "url")
            }
              , C = E >= 0 ? "hsla(".concat(E, ", 50%, 40%, 1)") : null
              , N = Ur("img", {
                src: Object(ba.a)({
                    url: _,
                    large: !0
                }),
                alt: "",
                className: "Masthead__watermark"
            })
              , P = R()(g, "path", null)
              , D = h && h.isLive
              , x = h && h.isUpcoming;
            return Ur(u.Fragment, null, Ur(Hr, {
                artwork: k
            }), Ur("div", {
                className: "Masthead__scrim",
                style: Vr(E)
            }), Ur("div", {
                className: "Masthead__container"
            }, Ur("div", {
                className: "Masthead__wrap"
            }, Ur("div", {
                className: "Masthead__content"
            }, g && Ur("div", {
                className: "Masthead__portraitThumbnail"
            }, Ur("div", {
                className: "Masthead__imageContainer"
            }, Ur(va.a, {
                className: "Masthead__artwork",
                src: Object(ba.b)(P, 810),
                role: "presentation"
            }))), Ur("div", {
                className: "Masthead__context"
            }, (D || x) && Ur("div", {
                className: "Masthead__smartstart"
            }, zr(s), Ur("div", {
                className: "Masthead__actions"
            }, b)), Ur("div", {
                className: "Masthead__title"
            }, v), Ur("div", {
                className: "Masthead__metadata-container"
            }, Kr(d), Wr(m)), Ur("div", {
                className: "Masthead__description"
            }, Ur(ca.a, {
                lineHeightEm: 1.5,
                maxLines: 2
            }, p)), qr(h, f, D), Ur("div", {
                className: "Masthead__actionMenu"
            }, Ur(Br.a, {
                watchUrl: I,
                watchUrlUserInteractionData: S,
                metricsParams: o,
                channelId: w,
                eabId: T,
                stateKey: Zi.b,
                menuActions: j,
                availability: h,
                lightSecondaryHue: C,
                light: !0,
                primaryActionEmphasized: !0,
                sendActionImpressions: !0,
                includeWatchAction: !0,
                customWatchActionText: y,
                coverStoryMetricsInfo: c
            })))))), _ && N)
        }
          , Xr = n("A/B6")
          , Zr = (n("p8oL"),
        d.a.createElement)
          , Qr = function(e) {
            var t = e.title
              , n = e.goBack
              , a = e.firstTimeLevelTwo
              , i = e.fxOnHuluBrandComponent
              , r = e.brandUrl
              , o = e.backgroundColor
              , c = e.showBottomDefinition
              , l = e.hideTitle
              , s = e.secondaryActions
              , u = void 0 === s ? [] : s
              , d = Zr("span", {
                className: "sr-only"
            }, Zr(Ee.view, {
                from: "buttons.back"
            }))
              , f = Zr("span", {
                className: "sr-only"
            }, Zr(Ee.view, {
                from: "buttons.close"
            }));
            return Zr("div", {
                className: B()("SimpleModalNav", {
                    "SimpleModalNav--defined": c
                }),
                style: {
                    backgroundColor: o
                }
            }, a ? null : Zr(Xr.b, {
                className: Object(re.a)("leveltwo", "back"),
                icon: Zr(Hn.a, null),
                onClick: function() {
                    return window.history.back()
                },
                label: d,
                "data-testid": "level2-nav-title"
            }), Zr("div", {
                className: B()("SimpleModalNav__title-container", {
                    "SimpleModalNav__title-container--hidden": l
                })
            }, r ? Zr("img", {
                id: "dialog-title",
                className: "SimpleModalNav__brandImage",
                src: r,
                alt: t,
                role: "presentation"
            }) : i || Zr("div", {
                id: "dialog-title",
                className: "SimpleModalNav__title",
                "data-testid": "level2-nav-title"
            }, t)), Zr(Xr.f, null), u.length > 0 ? Zr(Xr.e, {
                actions: u
            }) : null, Zr(Xr.b, {
                className: Object(re.a)("leveltwo", "close"),
                icon: Zr(Hn.n, null),
                onClick: n,
                label: f,
                ariaLabel: "close modal"
            }))
        }
          , $r = Object(J.createSelector)([function(e) {
            return R()(e[Zi.b], "firstLevelTwo")
        }
        ], (function(e) {
            return {
                firstTimeLevelTwo: e
            }
        }
        ))
          , eo = Object(g.connect)((function(e) {
            return $r(e)
        }
        ), {
            goBack: Xi.goBack
        })(Qr)
          , to = n("a8YF")
          , no = d.a.createElement
          , ao = function(e, t) {
            var n;
            return t ? n = "transparent" : e && void 0 !== e.hue && (n = "hsla(".concat(e.hue, ", 50%, 40%, 1)")),
            n
        }
          , io = function(e) {
            var t = e.title
              , n = e.brandUrl
              , a = e.hero
              , i = e.hideTitle
              , r = e.secondaryActions
              , o = e.fxOnHuluBrandComponent;
            return no(to.a, {
                scrollTarget: ".LevelTwo__scroll-area"
            }, (function(e) {
                return no(eo, {
                    title: t,
                    brandUrl: n,
                    fxOnHuluBrandComponent: o,
                    backgroundColor: ao(a, e),
                    showBottomDefinition: e,
                    hideTitle: i,
                    secondaryActions: r
                })
            }
            ))
        }
          , ro = d.a.createElement
          , oo = function(e) {
            var t = e.hero
              , n = e.mastheadAsset
              , a = e.bannerAsset
              , i = e.stateKey
              , r = e.mastheadCollectionId
              , o = e.bannerCollectionId
              , c = e.name
              , l = e.isStickyDropdown
              , s = e.detailEntity
              , d = e.mastheadTileState
              , f = e.configText
              , m = e.modalNavActions
              , p = e.coverStoryMetricsInfo
              , h = ro("div", {
                className: "MastheadAndBanner__head"
            }, ro(gr, {
                collectionId: r,
                collectionIndex: 0,
                item: n
            }, ro(Rr.a, {
                action: mt.ACTION_IMPRESSION_ACTIONS.SMART_START
            }, ro(Jr, {
                playableOrUpcomingAsset: n,
                detailEntity: s,
                mastheadTileState: d,
                pageContext: Va.d,
                configText: f,
                metricsParams: Object(ee.c)(0, {
                    id: r
                }, 0, mt.UI_ELEMENT.MASTHEAD, n.metrics_info),
                coverStoryMetricsInfo: p
            }))))
              , b = ro(gr, {
                collectionId: o,
                collectionIndex: 1,
                item: a
            }, ro(Rr.a, {
                action: mt.ACTION_IMPRESSION_ACTIONS.BANNER
            }, ro("div", {
                className: "MastheadAndBanner__banner"
            }, ro(Gr, {
                asset: a,
                stateKey: i,
                pageContext: Va.d,
                hue: R()(t, "hue", 0),
                metricsParams: Object(ee.c)(0, {
                    id: o
                }, 1, mt.UI_ELEMENT.LIVE_UPCOMING_BANNER, n.metrics_info)
            }))));
            return ro(u.Fragment, null, ro(io, {
                title: c,
                hero: t,
                hideTitle: l,
                secondaryActions: m
            }), ro("div", {
                className: "MastheadAndBanner",
                style: t && t.hue ? {
                    background: Cr(t.hue)
                } : null
            }, h, a && b))
        }
          , co = n("6qam")
          , lo = n.n(co)
          , so = n("qH5C")
          , uo = d.a.createElement;
        function fo(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        var mo = "VOD"
          , po = function(e, t) {
            if (e === mo) {
                var n = R()(t, ["vod_items", "focus", "action_text"], null)
                  , a = R()(t, ["vod_items", "focus", "entity"], null);
                return {
                    asset: a ? Object.assign({}, {
                        watchActionText: n
                    }, a) : null,
                    collectionId: "vod_items"
                }
            }
            if ("LIVE" === e)
                return {
                    asset: R()(t, ["live_items", "items", "0"], null),
                    collectionId: "live_items"
                };
            if ("UPCOMING" === e) {
                var i = R()(t, ["upcoming_items", "items", "0", "airing_information"], null)
                  , r = R()(t, ["upcoming_items", "items", "0", "entity"], null);
                return {
                    asset: r ? Object.assign({}, i, r) : null,
                    collectionId: "upcoming_items"
                }
            }
            return "TRAILER" === e ? {
                asset: R()(t, "trailer", null),
                collectionId: "trailer_items"
            } : {
                asset: null,
                collectionId: null
            }
        }
          , ho = function(e, t, n) {
            var a = po(e, n)
              , i = po(t, n);
            return {
                mastheadAsset: a.asset,
                mastheadCollectionId: a.collectionId,
                bannerAsset: i.asset,
                bannerCollectionId: i.collectionId
            }
        }
          , bo = function(e) {
            var t = [Tr.i, Tr.k]
              , n = R()(e, "_type", "")
              , a = R()(e, "genre_names", []).map((function(e) {
                return e.toLowerCase()
            }
            ));
            return t.includes(n.toLowerCase()) || a.includes("news") || a.includes("sports")
        }
          , vo = Object(J.createSelector)([function(e) {
            return e[Zi.b].details
        }
        , function(e) {
            return e[Zi.b].tileStates
        }
        , function(e) {
            return R()(e, "".concat(so.b, ".config.details_config"))
        }
        ], (function(e, t, n) {
            var a = R()(e, "entity")
              , i = function(e) {
                var t = R()(e, "entity")
                  , n = po(mo, e).asset
                  , a = po("LIVE", e).asset
                  , i = po("UPCOMING", e).asset
                  , r = po("TRAILER", e).asset
                  , o = bo(t)
                  , c = !lo()(R()(i, "bundle", null));
                return n && a ? o ? ho("LIVE", null, e) : ho(mo, "LIVE", e) : n && i && c ? o ? ho("UPCOMING", null, e) : ho(mo, "UPCOMING", e) : n && i && !c ? ho(mo, "UPCOMING", e) : n ? ho(mo, null, e) : a ? ho("LIVE", null, e) : i ? ho("UPCOMING", null, e) : r ? ho("TRAILER", null, e) : {
                    mastheadAsset: {}
                }
            }(e)
              , r = R()(i, ["mastheadAsset", "bundle", "eab_id"]) || R()(i, ["mastheadAsset", "personalization", "eab"])
              , o = R()(t, r, {})
              , c = xr(R()(i, "mastheadAsset"), a, R()(a, "_type"))
              , l = function(e) {
                var t = []
                  , n = R()(e, "_type")
                  , a = R()(e, "teams", []);
                return n === Tr.i && (t = a.map((function(e) {
                    var t = R()(e, "name")
                      , n = R()(e, "id")
                      , a = R()(e, "href")
                      , i = {
                        actionSpecifier: "nav:details",
                        elementSpecifier: "go_to_team",
                        entityData: {
                            entityId: n,
                            entityType: R()(e, "_type"),
                            entityActionId: n,
                            entityActionType: "browse"
                        },
                        displayName: t
                    };
                    return {
                        id: n,
                        href: Object(cr.h)(e),
                        discoverHref: a,
                        label: uo(Ee.view, {
                            from: "details_config.nav.goToTeam",
                            replace: {
                                TEAM: t
                            }
                        }),
                        icon: uo(Hn.b, null),
                        userInteractionData: i
                    }
                }
                ))),
                t
            }(a);
            return function(e) {
                for (var t = 1; t < arguments.length; t++) {
                    var n = null != arguments[t] ? arguments[t] : {};
                    t % 2 ? fo(Object(n), !0).forEach((function(t) {
                        Object(C.a)(e, t, n[t])
                    }
                    )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : fo(Object(n)).forEach((function(t) {
                        Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                    }
                    ))
                }
                return e
            }({
                hero: c
            }, i, {
                stateKey: Zi.b,
                detailEntity: a,
                mastheadTileState: o,
                configText: n,
                modalNavActions: l
            })
        }
        ))
          , go = n("rwwM")
          , yo = n("ZguX")
          , Oo = Object(g.connect)((function(e) {
            return vo(e)
        }
        ), {
            updateNotification: go.a,
            updateAllTileStates: yo.a
        })(oo)
          , _o = n("HyMO")
          , jo = (n("ugAz"),
        d.a.createElement)
          , Io = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "getContent", (function() {
                    return n.content
                }
                )),
                Object(C.a)(Object(D.a)(n), "setRef", (function(e) {
                    n.content = e
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    this.keepFocus = Object(_o.a)(this.getContent)
                }
            }, {
                key: "componentWillUnmount",
                value: function() {
                    this.keepFocus.remove()
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.children
                      , n = e.className
                      , a = Object(N.a)(e, ["children", "className"])
                      , i = B()("L2Content", "addFocus", n);
                    return jo("div", Object(x.a)({
                        className: i,
                        ref: this.setRef,
                        role: "dialog",
                        "aria-labelledby": "dialog-title",
                        tabIndex: "-1"
                    }, a), t)
                }
            }]),
            t
        }(u.Component)
          , wo = (n("l0cG"),
        n("7BTR"),
        d.a.createElement)
          , To = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    return wo("div", {
                        className: "TrailerTile"
                    }, this.getThumbnail(), this.getTitle())
                }
            }, {
                key: "getThumbnail",
                value: function() {
                    var e = this.props.timeBadge;
                    return wo("div", {
                        className: "TrailerTile__thumbnail-container"
                    }, wo(Ia, this.props), e ? this.getBadge(e) : null)
                }
            }, {
                key: "getBadge",
                value: function(e, t) {
                    var n = e.text
                      , a = e.classNames
                      , i = "TrailerTile__badge Trailer__badge--".concat(t);
                    return wo("div", {
                        className: i
                    }, wo(ma.a, {
                        text: n,
                        modifiers: a
                    }))
                }
            }, {
                key: "getTitle",
                value: function() {
                    var e = this.props.title;
                    return wo("div", {
                        className: "TrailerTile__title-container"
                    }, wo("h3", {
                        className: "TrailerTile__title"
                    }, wo(ca.a, {
                        lineHeightEm: 1.3,
                        maxLines: 1
                    }, e)))
                }
            }]),
            t
        }(u.Component)
          , So = Object(P.a)(ia.a, ra.a, aa.a)(To)
          , Eo = d.a.createElement
          , ko = function(e) {
            var t = e.description
              , n = e.genres
              , a = e.premiereDate
              , i = e.rating
              , r = e.sport
              , o = e.sportsLeague
              , c = e.type
              , l = e.name
              , s = e.trailerAsset
              , u = e.stateKey;
            return Eo("div", {
                className: "DetailsTab"
            }, Eo("div", {
                className: "DetailsTab__about"
            }, Eo("p", {
                className: "DetailsTab__head"
            }, Eo(Ee.view, {
                from: "details_config.about.head",
                replace: {
                    TYPE: Object(h.f)(c)
                }
            })), Eo("h4", {
                className: "DetailsTab__name"
            }, l), Eo("p", {
                className: "DetailsTab__description"
            }, o && Eo("p", {
                className: "DetailsTab__league"
            }, o), t), Eo("p", {
                className: "DetailsTab__meta"
            }, Eo("span", {
                className: "DetailsTab__meta-segment"
            }, i), Eo("span", {
                className: "DetailsTab__meta-segment"
            }, r && Eo(Ee.view, {
                from: "details_config.about.sportsMetaData",
                replace: {
                    SPORT: r
                }
            })), Eo("span", {
                className: "DetailsTab__meta-segment"
            }, n.join(", ")), Eo("span", {
                className: "DetailsTab__meta-segment"
            }, Eo(Ee.view, {
                from: "details_config.about.typeDescription.".concat(c)
            }), a && Co(c, a)))), s && Eo("div", {
                className: "DetailsTab__trailer"
            }, Eo("p", {
                className: "DetailsTab__head"
            }, Eo(Ee.view, {
                from: "details_config.trailer"
            })), Eo(Rr.a, {
                action: mt.ACTION_IMPRESSION_ACTIONS.TRAILER
            }, Eo(So, {
                asset: s,
                stateKey: u,
                metricsParams: Object(ee.d)(mt.UI_ELEMENT.TRAILER)
            }))))
        }
          , Co = function(e, t) {
            return "sports_episode" === e ? Eo(Ee.view, {
                from: "details_config.about.airDate",
                replace: {
                    DATE: $t.c.format(t)
                }
            }) : " (".concat(t.getFullYear(), ")")
        };
        ko.defaultProps = {
            genres: []
        };
        var No = ko
          , Po = Object(J.createSelector)([function(e) {
            return R()(e[Zi.b], "details.entity", {})
        }
        , function(e) {
            return R()(e[Zi.b], "details", {})
        }
        ], (function(e, t) {
            var n = R()(t, "trailer", null);
            return {
                description: e.description,
                genres: e.genre_names,
                premiereDate: e.premiere_date && new Date(e.premiere_date),
                rating: R()(e, "rating.code"),
                sport: e.sport_name,
                sportsLeague: e.league_name,
                type: e._type,
                name: e.name,
                trailerAsset: n,
                stateKey: Zi.b
            }
        }
        ))
          , Do = Object(g.connect)((function(e) {
            return Po(e)
        }
        ))(No)
          , xo = (n("RdvZ"),
        d.a.createElement)
          , Lo = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    selectedCollectionIndex: 0,
                    isStickyDropdown: !1
                }),
                Object(C.a)(Object(D.a)(n), "clickSubnav", (function(e) {
                    n.setState({
                        selectedCollectionIndex: e
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "setStickyDropdown", (function(e) {
                    n.setState({
                        isStickyDropdown: e
                    })
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.state
                      , t = e.selectedCollectionIndex
                      , n = e.isStickyDropdown
                      , a = this.props
                      , i = a.name
                      , r = a.hue
                      , o = a.isKidsProfile
                      , c = a.isKidAppropriateContent
                      , l = a.subnavTitles
                      , s = a.pageType
                      , u = a.coverStoryMetricsInfo
                      , d = r ? Nr(r) : null;
                    return o && !c && Object(p.c)("/hub/kids"),
                    xo(Io, {
                        className: "Details"
                    }, xo(Oo, {
                        pageType: s,
                        name: i,
                        isStickyDropdown: n,
                        coverStoryMetricsInfo: u
                    }), xo("div", {
                        className: "Details__subnav"
                    }, xo(tr, {
                        pageType: s,
                        navTitles: l,
                        color: d,
                        selectedNavIndex: t,
                        switchCollection: this.clickSubnav,
                        ariaLabel: "".concat(i, " Navigation")
                    })), xo("div", {
                        className: "Details__collection"
                    }, this.renderSelectedCollection()))
                }
            }, {
                key: "getInitiallyFocusedGroupId",
                value: function(e, t) {
                    var n = R()(e, ["focus_navigation", "collection_id"])
                      , a = R()(t.find((function(e) {
                        return e.id && e.items && e.items.length > 0
                    }
                    )), "id")
                      , i = R()(t[0], "id");
                    return n || a || i
                }
            }, {
                key: "renderSelectedCollection",
                value: function() {
                    var e = this.state
                      , t = e.selectedCollectionIndex
                      , n = e.isStickyDropdown
                      , a = this.props
                      , i = a.collections
                      , r = a.name
                      , o = a.stateKey
                      , c = a.pageType
                      , l = a.coverStoryMetricsInfo;
                    if (t === i.length)
                        return xo(Do, null);
                    var s = R()(i, t, {});
                    if ("collection_theme_groupings" === s.theme) {
                        var u = R()(s, "items", [])
                          , d = this.getInitiallyFocusedGroupId(s, u);
                        return xo(_r, {
                            initiallyFocusedGroupId: d,
                            groupsOfItems: u,
                            collectionIndex: t,
                            collectionId: s.id,
                            name: r,
                            stateKey: o,
                            pageType: c,
                            setStickyDropdown: this.setStickyDropdown,
                            isStickyDropdown: n,
                            coverStoryMetricsInfo: l
                        })
                    }
                    return xo(gr, {
                        collectionIndex: t,
                        collectionId: s.id
                    }, xo(vr, {
                        items: s.items,
                        collectionId: s.id,
                        collectionIndex: t,
                        stateKey: o,
                        pageType: c,
                        tilePageContext: Va.c,
                        coverStoryMetricsInfo: l
                    }))
                }
            }]),
            t
        }(u.Component)
          , Ro = []
          , Mo = function(e) {
            return R()(e, [Zi.b, "collections"], Ro)
        }
          , Ao = Object(J.createSelector)([Mo, function(e) {
            return R()(e, "".concat(so.b, ".config.details_config.about.subnavTitle"), "Details")
        }
        ], (function(e, t) {
            return [].concat(Object(fi.a)(e.map((function(e) {
                return e.name
            }
            ))), [t])
        }
        ))
          , Go = Object(J.createSelector)([Mo, function(e) {
            return R()(e[Zi.b], "details.entity")
        }
        , function(e) {
            return R()(e, [Zi.b, "dataReady"])
        }
        , vi.b, Ao, function(e) {
            return e[Zi.b].coverStoryMetricsInfo
        }
        ], (function(e, t, n, a, i, r) {
            var o = R()(t, "name")
              , c = "kid_appropriate" === R()(t, "restriction_level")
              , l = R()(t, "artwork", {})
              , s = kr(l).hue;
            return {
                dataReady: n,
                name: o,
                isKidsProfile: a.isKidsProfile,
                isKidAppropriateContent: c,
                subnavTitles: i,
                hue: s,
                collections: e,
                stateKey: Zi.b,
                coverStoryMetricsInfo: r
            }
        }
        ))
          , Bo = Object(P.a)(Object(de.a)((function(e) {
            var t = e.heimdallEntity
              , n = e.heimdallEntityId
              , a = e.geo
              , i = e.networkBreadcrumb
              , r = e.pageType
              , o = e.discoverHref;
            return {
                url: o ? ne.b.browseUrlWithDiscoverHref({
                    href: o,
                    limit: 999
                }) : ne.b.default({
                    entity: t,
                    id: n,
                    limit: 999,
                    networkBreadcrumb: i
                }),
                geo: a,
                pageType: r
            }
        }
        )), Object(g.connect)(Go, {
            initContent: Xi.initContent,
            initTileStates: Xi.appendTileStates,
            setIsLoadingData: Xi.setIsLoadingData
        }), ue.a)(Lo)
          , Fo = (n("bBOU"),
        n("HQbG"),
        n("/Aba"),
        d.a.createElement)
          , Ho = function(e) {
            var t = e.artwork
              , n = R()(t, "accent.hue", 0)
              , a = "hsla(".concat(n, ", 50%, 40%, 1)");
            return Fo("div", {
                className: "NetworkBackground"
            }, Fo("div", {
                className: "NetworkBackground__base"
            }), Fo("div", {
                className: "NetworkBackground__art",
                style: {
                    backgroundImage: "linear-gradient(".concat(a, ", ").concat(a, "), ").concat("url(https://assetshuluimcom-a.akamaihd.net/h3o/browse/network_background.jpg)")
                }
            }), Fo("div", {
                className: "NetworkBackground__overlay"
            }))
        }
          , Uo = n("N6Do")
          , Vo = d.a.createElement;
        function zo(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        var Wo = function(e) {
            var t = e.artwork
              , n = e.currentCollection
              , a = e.currentCollectionIndex
              , i = e.setMastheadIndex
              , r = e.stateKey
              , o = e.pageType
              , c = Object(u.useCallback)((function(e) {
                i({
                    mastheadIndex: e
                }),
                Object(ke.trackTealium)({
                    move_to_slide: e,
                    event_name: "".concat(Object(Jn.c)(S.j.NETWORK), "_").concat(S.n.suffix.arrow_button)
                })
            }
            ), [i])
              , l = R()(n, "items", [])
              , s = ea()(l, (function(e) {
                return e.id
            }
            ))
              , d = Object(Jn.f)(o, "masthead")
              , f = s.slice(0, 5).map((function(e, t) {
                return function(e) {
                    for (var t = 1; t < arguments.length; t++) {
                        var n = null != arguments[t] ? arguments[t] : {};
                        t % 2 ? zo(Object(n), !0).forEach((function(t) {
                            Object(C.a)(e, t, n[t])
                        }
                        )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : zo(Object(n)).forEach((function(t) {
                            Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                        }
                        ))
                    }
                    return e
                }({
                    content: Object(Uo.e)({
                        item: e,
                        index: t,
                        collection: n,
                        collectionIndex: a,
                        stateKey: r,
                        pageType: o,
                        skipLoading: 0 !== t,
                        tealiumTileData: d
                    })
                }, Object(Uo.d)(e))
            }
            ));
            return Vo("div", {
                className: "SingleNetworkMasthead__head"
            }, Vo("div", {
                className: "SingleNetworkMasthead__background--network"
            }, Vo(Ho, {
                artwork: t
            })), Vo("div", {
                className: "SingleNetworkMasthead__background--hero"
            }, Vo("div", {
                className: "SingleNetworkMasthead__heros"
            }, Vo(wn.a, {
                key: a,
                className: "SingleNetworkMasthead__slider",
                isCircular: !0,
                shouldShowDots: !0,
                items: f,
                shouldHideOverflow: !0,
                onIndexChanged: c
            }))))
        }
          , Yo = d.a.createElement
          , Ko = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    selectedCollectionIndex: 0,
                    selectedMastheadIndex: 0
                }),
                Object(C.a)(Object(D.a)(n), "setMastheadIndex", (function(e) {
                    var t = e.mastheadIndex;
                    n.setState({
                        selectedMastheadIndex: t
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "switchToBeginningOfCollection", (function(e) {
                    n.setState({
                        selectedCollectionIndex: e,
                        selectedMastheadIndex: 0
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "loadMoreItems", (function() {
                    var e = n.state.selectedCollectionIndex
                      , t = n.props
                      , a = t.collections
                      , i = t.fetchNextPage
                      , r = R()(a[e], "pagination.next");
                    r && i(r)
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.state
                      , t = e.selectedCollectionIndex
                      , n = e.selectedMastheadIndex
                      , a = this.props.pageType
                      , i = this.props
                      , r = i.refresh
                      , o = i.detailEntity
                      , c = i.collections
                      , l = i.stateKey
                      , s = i.name
                      , u = i.heimdallEntityId
                      , d = c[t]
                      , f = R()(d, "id")
                      , m = c.map((function(e) {
                        return e.name
                    }
                    ))
                      , p = function(e) {
                        var t = e.artwork
                          , n = e.series_artwork;
                        return R()(t, '["detail.horizontal.hero"]', null) || R()(n, '["detail.horizontal.hero"]', null) || R()(t, '["video.horizontal.hero"]', null) || {}
                    }(R()(d, "items[".concat(n, "]"), {}))
                      , h = function(e) {
                        var t = R()(e, "path", null);
                        return t ? Object(ba.a)({
                            url: t,
                            trim: !0
                        }) : null
                    }(function(e) {
                        return R()(e, 'primary_branding.artwork["brand.watermark"]', null) && R()(e, 'primary_branding.artwork["brand.watermark.bottom.right"]', null)
                    }(o))
                      , b = R()(p, "accent.hue")
                      , v = void 0 !== b ? Nr(b) : ""
                      , g = "fx_on_hulu" === u ? Yo(Hn.q, null) : null;
                    return Yo(Io, {
                        key: "SingleNetwork-L2Content"
                    }, Yo(io, {
                        title: s,
                        fxOnHuluBrandComponent: g,
                        brandUrl: h,
                        hero: R()(p, "accent")
                    }), Yo(Wo, {
                        detailEntity: o,
                        currentCollection: d,
                        currentCollectionIndex: t,
                        networkName: s,
                        artwork: p,
                        setMastheadIndex: this.setMastheadIndex,
                        stateKey: l,
                        pageType: a
                    }), Yo(H.a, null, Yo("div", {
                        className: "SingleNetwork__subnav"
                    }, Yo(tr, {
                        navTitles: m,
                        selectedNavIndex: t,
                        color: v,
                        switchCollection: this.switchToBeginningOfCollection,
                        alignPaddles: "wrap",
                        pageType: a,
                        ariaLabel: "".concat(s, " Navigation")
                    })), Yo("div", {
                        className: "SingleNetwork__allupgrid"
                    }, Yo(vr, Object(x.a)({}, d, {
                        loadMoreItems: this.loadMoreItems,
                        canLoadMore: !!na()(d, "pagination.next"),
                        collectionId: f,
                        collectionIndex: t,
                        stateKey: l,
                        pageType: a
                    })))), Yo($, {
                        refresh: r
                    }))
                }
            }]),
            t
        }(u.Component);
        Ko.defaultProps = {
            collections: []
        };
        var qo = Ko
          , Jo = []
          , Xo = Object(J.createSelector)([function(e) {
            return R()(e, [Zi.b, "dataReady"])
        }
        , function(e) {
            return R()(e, [Zi.b, "collections"], Jo)
        }
        , function(e) {
            return R()(e[Zi.b], "details.entity")
        }
        , function(e) {
            return R()(e, [Zi.b, "pageType"])
        }
        , function(e) {
            return R()(e, [Zi.b, "discoverHref"])
        }
        , function(e) {
            return e[Zi.b].heimdallEntityId
        }
        , function(e) {
            return e[Zi.b].name
        }
        ], (function(e, t, n, a, i, r, o) {
            var c = "fx_on_hulu" === r ? o : R()(n, "name", null)
              , l = R()(n, ["local_affiliate_branding", "name"], null);
            return {
                dataReady: e,
                detailEntity: n,
                collections: t,
                stateKey: Zi.b,
                name: l || c,
                pageType: a,
                discoverHref: i,
                heimdallEntityId: r
            }
        }
        ))
          , Zo = Object(P.a)(Object(de.a)((function(e) {
            var t = e.heimdallEntity
              , n = e.heimdallEntityId
              , a = e.geo
              , i = e.discoverHref;
            return {
                url: i ? ne.b.browseUrlWithDiscoverHref({
                    href: i
                }) : ne.b.default({
                    entity: t,
                    id: n
                }),
                geo: a
            }
        }
        )), Object(g.connect)(Xo, {
            initContent: Xi.initContent,
            initTileStates: Xi.appendTileStates,
            setIsLoadingData: Xi.setIsLoadingData,
            fetchNextPage: Xi.fetchNextPage,
            refresh: Xi.refresh
        }), ue.a)(qo)
          , Qo = (n("EPnx"),
        d.a.createElement)
          , $o = function(e) {
            var t = e.id
              , n = e.name
              , a = e.items
              , i = e.fetchNextPage
              , r = e.pagination
              , o = e.stateKey
              , c = e.pageType
              , l = {
                onEnter: te.a,
                waitFor: 250,
                threshold: 0,
                metricsParams: Object(ee.a)(t, 0, o)
            }
              , s = {
                items: a,
                loadMoreItems: function() {
                    i(R()(r, "next"))
                },
                canLoadMore: na()(r, "next") && a.length > 0,
                collectionId: t,
                collectionIndex: 0,
                stateKey: o,
                pageType: c
            };
            return Qo(Io, {
                key: "ViewAllCollection-L2Content-".concat(t)
            }, Qo(eo, {
                title: n
            }), Qo(H.a, null, Qo("div", {
                className: "ViewAllCollection__allupgrid"
            }, Qo(U.a, l, Qo(vr, s)))))
        };
        function ec(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        var tc = function(e) {
            return e[Zi.b]
        }
          , nc = Object(J.createSelector)([tc], (function(e) {
            return {
                brandArtwork: Object(F.d)(e),
                name: e.name
            }
        }
        ))
          , ac = Object(J.createSelector)([nc, tc], (function(e, t) {
            return function(e) {
                for (var t = 1; t < arguments.length; t++) {
                    var n = null != arguments[t] ? arguments[t] : {};
                    t % 2 ? ec(Object(n), !0).forEach((function(t) {
                        Object(C.a)(e, t, n[t])
                    }
                    )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : ec(Object(n)).forEach((function(t) {
                        Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                    }
                    ))
                }
                return e
            }({}, t, {
                mastheadData: e,
                stateKey: Zi.b
            })
        }
        ))
          , ic = Object(P.a)(Object(de.a)((function(e) {
            var t = e.heimdallEntity
              , n = e.heimdallEntityId
              , a = e.heimdallCollectionId
              , i = e.geo
              , r = e.discoverHref;
            return {
                url: r ? ne.b.browseUrlWithDiscoverHref({
                    href: r
                }) : ne.b.default({
                    entity: t,
                    id: n,
                    collectionId: a
                }),
                geo: i
            }
        }
        )), Object(g.connect)(ac, {
            initContent: Xi.initContent,
            initTileStates: Xi.appendTileStates,
            setIsLoadingData: Xi.setIsLoadingData,
            fetchNextPage: Xi.fetchNextPage,
            refresh: Xi.refresh
        }), ue.a)($o)
          , rc = (n("uACA"),
        d.a.createElement)
          , oc = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    progressPosition: 0
                }),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    this.performAnimation()
                }
            }, {
                key: "componentDidUpdate",
                value: function() {
                    this.performAnimation()
                }
            }, {
                key: "performAnimation",
                value: function() {
                    var e = this
                      , t = this.props
                      , n = t.progress
                      , a = t.total
                      , i = n >= a ? 100 : n / a * 100;
                    window.requestAnimationFrame((function() {
                        e.state.progressPosition !== i && e.setState({
                            progressPosition: i
                        })
                    }
                    ))
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.minLabel
                      , n = e.maxLabel
                      , a = this.state.progressPosition
                      , i = {
                        width: "".concat(a, "%")
                    }
                      , r = t && n
                      , o = B()("ProgressBar__progress ProgressBar__progress--used", {
                        "ProgressBar__progress--not-complete": a < 100
                    });
                    return rc("div", {
                        className: "ProgressBar"
                    }, rc("div", {
                        className: "ProgressBar__container"
                    }, rc("div", {
                        className: "ProgressBar__progress"
                    }), rc("div", {
                        className: o,
                        style: i
                    })), r && rc("div", {
                        "aria-hidden": !0,
                        className: "ProgressBar__labels"
                    }, rc("span", null, t), rc("span", null, n)))
                }
            }]),
            t
        }(d.a.Component)
          , cc = d.a.createElement
          , lc = function(e) {
            var t = parseFloat((e / 3600).toFixed(1))
              , n = parseFloat((e / 60).toFixed(1))
              , a = cc(Ee.view, {
                from: "manageDvrConfig.availableStorage.timeText.minsPlural",
                replace: {
                    AMOUNT: n
                }
            })
              , i = cc(Ee.view, {
                from: "manageDvrConfig.availableStorage.timeText.minsSingular",
                replace: {
                    AMOUNT: n
                }
            })
              , r = cc(Ee.view, {
                from: "manageDvrConfig.availableStorage.timeText.hoursPlural",
                replace: {
                    AMOUNT: t
                }
            })
              , o = cc(Ee.view, {
                from: "manageDvrConfig.availableStorage.timeText.hoursSingular",
                replace: {
                    AMOUNT: t
                }
            });
            return e < 3600 ? 1 !== n ? a : i : 1 !== t ? r : o
        }
          , sc = function(e) {
            var t = e.availableDvrStorage || {}
              , n = t.availableTime
              , a = t.savedForNow
              , i = t.error
              , r = cc(Ee.view, {
                from: "manageDvrConfig.availableStorage.errorText"
            })
              , o = "";
            return Number.isFinite(n) ? o = function(e, t) {
                var n = e + t
                  , a = cc("div", {
                    className: "ManageDVR__available-amount"
                }, cc(Ee.view, e > 0 ? {
                    from: "manageDvrConfig.availableStorage.availableText",
                    replace: {
                        STORAGE: lc(e)
                    }
                } : {
                    from: "manageDvrConfig.availableStorage.storageFullText"
                }))
                  , i = cc(Ee.view, {
                    from: "manageDvrConfig.availableStorage.progressBarLabel",
                    replace: {
                        AMOUNT: 0
                    }
                })
                  , r = cc(Ee.view, {
                    from: "manageDvrConfig.availableStorage.progressBarLabel",
                    replace: {
                        AMOUNT: (n / 3600).toFixed(0)
                    }
                });
                return cc(u.Fragment, null, a, cc("div", {
                    className: "sr-only"
                }, "Total storage: ", r), cc(oc, {
                    progress: t,
                    total: n,
                    minLabel: i,
                    maxLabel: r
                }))
            }(n, a) : i && (o = r),
            cc("div", {
                className: "ManageDVR__available"
            }, o)
        }
          , uc = n("3mt+")
          , dc = (n("mzXm"),
        d.a.createElement)
          , fc = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props.isSeries
                      , t = B()("DvrTile", {
                        "DvrTile--series": e
                    });
                    return dc("div", {
                        className: t
                    }, this.getThumbnail(), this.getTitleAndDescription(), this.getRecordedInfo())
                }
            }, {
                key: "getThumbnail",
                value: function() {
                    var e = this.props
                      , t = e.timeBadge
                      , n = e.availability
                      , a = R()(n, "isLive");
                    return dc("div", {
                        className: "DvrTile__thumbnail-container"
                    }, dc(Ia, Object(x.a)({}, this.props, {
                        useSmallerIcons: !0
                    })), t || a ? this.getBadge(t, a) : null, this.getStatusBarElement())
                }
            }, {
                key: "getBadge",
                value: function(e, t) {
                    var n = t ? Object(Ct.a)() : e
                      , a = n.text
                      , i = n.classNames
                      , r = i.concat(i.map((function(e) {
                        return "".concat(e, "-dvr")
                    }
                    )));
                    return dc("div", {
                        className: "DvrTile__badge DvrTile__badge--time"
                    }, dc(ma.a, {
                        text: a,
                        modifiers: r
                    }))
                }
            }, {
                key: "getStatusBarElement",
                value: function() {
                    var e = this.props.availability;
                    if (e) {
                        var t = e.isLive
                          , n = e.isPlayable
                          , a = e.progress;
                        if (n)
                            return dc(da.a, {
                                className: "Tile__statusBar",
                                progressClass: t ? fa.c : fa.a,
                                progress: a
                            })
                    }
                }
            }, {
                key: "getTitleAndDescription",
                value: function() {
                    var e = this.props
                      , t = e.title
                      , n = e.description
                      , a = e.onClickContent;
                    return dc("div", {
                        className: "DvrTile__title-container",
                        onClick: a
                    }, dc("h3", {
                        className: "DvrTile__title"
                    }, dc(ca.a, {
                        lineHeightEm: 1.3,
                        maxLines: 1
                    }, t)), dc("p", {
                        className: "DvrTile__description"
                    }, dc(ca.a, {
                        lineHeightEm: 1.3,
                        maxLines: 3
                    }, n)))
                }
            }, {
                key: "getRecordedInfo",
                value: function() {
                    var e = this.props
                      , t = e.recordedDate
                      , n = e.recordedTimeRange
                      , a = e.recordingStatus
                      , i = e.bundleType
                      , r = e.customRecordedInfoLine1
                      , o = e.customRecordedInfoLine2
                      , c = e.onClickContent
                      , l = i === uc.c && "DONE" === a ? t.toUpperCase() : dc(Ee.view, {
                        from: "manageDvrConfig.inProgressRecording"
                    });
                    return dc("div", {
                        className: "DvrTile__recorded-info",
                        onClick: c
                    }, dc("div", {
                        className: "DvrTile__recorded-date"
                    }, r || l), dc("div", {
                        className: "DvrTile__recorded-time"
                    }, o || n))
                }
            }]),
            t
        }(u.Component)
          , mc = Object(P.a)(ia.a, ra.a, aa.a)(fc)
          , pc = d.a.createElement
          , hc = Ua(mc, "DVR_TILE")
          , bc = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    expand: !1
                }),
                Object(C.a)(Object(D.a)(n), "toggleGroupCollapsed", (function() {
                    n.setState({
                        expand: !n.state.expand
                    })
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props.episodeTiles
                      , t = this.state.expand;
                    return pc("div", {
                        className: "ManageDVR__group"
                    }, pc("div", {
                        className: "ManageDVR__series"
                    }, this.renderDvrTile(), this.renderTrigger()), pc("div", {
                        className: "ManageDVR__group-children"
                    }, t && e))
                }
            }, {
                key: "renderDvrTile",
                value: function() {
                    var e = this.props.asset
                      , t = R()(e, "href")
                      , n = R()(e, "recorded_episodes", [])
                      , a = R()(e, "pending_recorded_episodes", [])
                      , i = a.length > 0 ? pc(Ee.view, {
                        from: "manageDvrConfig.seriesEpisodesCountWithPending",
                        replace: {
                            RECORDED: n.length,
                            PENDING: a.length
                        }
                    }) : pc(Ee.view, {
                        from: "manageDvrConfig.seriesEpisodesCount",
                        replace: {
                            RECORDED: n.length
                        }
                    })
                      , r = n.length > 0 ? hc : mc;
                    return pc(r, Object(x.a)({}, this.props, {
                        onClickContent: this.toggleGroupCollapsed,
                        isSeries: !0,
                        customRecordedInfoLine1: pc(Ee.view, {
                            from: "manageDvrConfig.seriesTileText"
                        }),
                        customRecordedInfoLine2: i,
                        discoverHref: t
                    }))
                }
            }, {
                key: "renderTrigger",
                value: function() {
                    var e = this.state.expand
                      , t = B()("ManageDVR__trigger", {
                        "ManageDVR__trigger--expand": e
                    });
                    return pc("button", {
                        className: t,
                        "aria-expanded": e,
                        onClick: this.toggleGroupCollapsed
                    }, pc("span", {
                        className: "sr-only"
                    }, pc(Ee.view, {
                        from: "manageDvrConfig.showEpisodesToggle.".concat(e ? "hide" : "expand")
                    })))
                }
            }]),
            t
        }(u.Component)
          , vc = d.a.createElement;
        function gc(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function yc(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = null != arguments[t] ? arguments[t] : {};
                t % 2 ? gc(Object(n), !0).forEach((function(t) {
                    Object(C.a)(e, t, n[t])
                }
                )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : gc(Object(n)).forEach((function(t) {
                    Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                }
                ))
            }
            return e
        }
        var Oc = Ua(mc, "DVR_TILE")
          , _c = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    selectedTabIndex: 0
                }),
                Object(C.a)(Object(D.a)(n), "clickSubnav", (function(e) {
                    n.setState({
                        selectedTabIndex: e
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "renderDvrTile", (function(e, t, a) {
                    var i = n.props
                      , r = i.stateKey
                      , o = i.collectionId
                      , c = i.collectionIndex
                      , l = i.itemsPendingRemoval
                      , s = i.toggleTile
                      , u = i.pageType
                      , d = Object.assign({}, e, {
                        collectionId: o
                    })
                      , f = R()(d, "id");
                    d.series_id || (d.series_id = f);
                    var m = na()(l, f)
                      , p = !Xt()(l)
                      , h = {
                        asset: d,
                        collectionId: o,
                        collectionIndex: c,
                        metricsParams: Object(ee.c)(t, {
                            id: o
                        }, c),
                        stateKey: r,
                        pageContext: Va.c,
                        isReadOnly: p,
                        tealiumTileData: Object(Jn.g)(u, a),
                        name: a
                    }
                      , b = vc(mc, h);
                    if (n.isRemoveableItem(d)) {
                        var v = yc({}, h, {
                            assetIndex: t,
                            callback: s,
                            isSelected: m,
                            toggleVisible: p
                        });
                        b = vc(Oc, v)
                    }
                    return vc("div", {
                        key: e.id,
                        className: "ManageDVR__item"
                    }, b)
                }
                )),
                Object(C.a)(Object(D.a)(n), "renderSeriesAndItsEpisodes", (function(e, t, a) {
                    var i = n.props
                      , r = i.stateKey
                      , o = i.collectionId
                      , c = i.collectionIndex
                      , l = i.itemsPendingRemoval
                      , s = i.toggleSeries
                      , u = i.seriesPendingRemoval
                      , d = i.pageType
                      , f = n.buildSeriesAssetFromEpisodes(e)
                      , m = R()(f, "id")
                      , p = na()(u, m)
                      , h = !Xt()(l)
                      , b = e.map((function(e, t) {
                        return n.renderDvrTile(e, t, a)
                    }
                    ));
                    return vc(bc, {
                        key: m,
                        asset: f,
                        assetIndex: t,
                        callback: s,
                        collectionId: o,
                        collectionIndex: c,
                        isSelected: p,
                        metricsParams: Object(ee.c)(t, {
                            id: o
                        }, c),
                        stateKey: r,
                        pageContext: Va.b,
                        toggleVisible: h,
                        isReadOnly: h,
                        episodeTiles: b,
                        tealiumTileData: Object(Jn.g)(d, a),
                        name: a
                    })
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.state.selectedTabIndex
                      , t = this.props
                      , n = t.items
                      , a = t.tabs
                      , i = R()(a, [e, "key"])
                      , r = R()(n, i, [])
                      , o = 0 === Object.keys(r).length
                      , c = "episodes" === i ? this.renderSeriesAndItsEpisodes : this.renderDvrTile
                      , l = Object.keys(r).map((function(e, t) {
                        return c(r[e], t, i)
                    }
                    ))
                      , s = a.map((function(e) {
                        return e.name
                    }
                    ));
                    return vc("div", {
                        className: "ManageDVR__collection"
                    }, vc("div", {
                        className: "ManageDVR__subnav"
                    }, vc(tr, {
                        ariaLabel: "Manage DVR Navigation",
                        navTitles: s,
                        selectedNavIndex: e,
                        switchCollection: this.clickSubnav
                    })), o && this.renderNullState(), vc("div", {
                        className: "ManageDVR__items"
                    }, l))
                }
            }, {
                key: "renderNullState",
                value: function() {
                    return vc(H.a, null, vc("div", {
                        className: "ManageDVR__null-state"
                    }, vc(Ee.view, {
                        from: "manageDvrConfig.nullState"
                    })))
                }
            }, {
                key: "buildSeriesAssetFromEpisodes",
                value: function(e) {
                    var t = this
                      , n = e[0]
                      , a = R()(n, ["bundle", "availability", "airing_start_date"])
                      , i = R()(n, "href")
                      , r = e.filter((function(e) {
                        return t.isRemoveableItem(e)
                    }
                    )).map((function(e) {
                        return yc({}, e, {
                            eabId: R()(e, ["bundle", "eab_id"])
                        })
                    }
                    ))
                      , o = e.filter((function(e) {
                        return !t.isRemoveableItem(e)
                    }
                    ));
                    return "episode" === n._type ? {
                        _type: "series",
                        artwork: Object(C.a)({}, "video.horizontal.hero", n.series_artwork["program.tile"]),
                        id: n.series_id,
                        name: n.series_name,
                        recorded_episodes: r,
                        pending_recorded_episodes: o,
                        lastAir: a,
                        href: i
                    } : yc({}, n, {
                        description: "",
                        recorded_episodes: r,
                        pending_recorded_episodes: o,
                        genre_names: [],
                        lastAir: a,
                        href: i
                    })
                }
            }, {
                key: "isRemoveableItem",
                value: function(e) {
                    var t = R()(e, ["bundle", "bundle_type"])
                      , n = R()(e, ["bundle", "recording_info", "recording_status_message"]);
                    return t === uc.c && "DONE" === n
                }
            }]),
            t
        }(u.Component)
          , jc = (n("4PXH"),
        d.a.createElement)
          , Ic = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    (0,
                    this.props.fetchDvrStorage)()
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.collection
                      , n = e.organizedDVRItems
                      , a = e.subnavIndex
                      , i = e.stateKey
                      , r = e.itemsPendingRemoval
                      , o = e.seriesPendingRemoval
                      , c = e.toggleTile
                      , l = e.toggleSeries
                      , s = e.handleCancel
                      , u = e.handleRemove
                      , d = e.availableDvrStorage
                      , f = e.tabs
                      , m = e.pageType
                      , p = e.metricsInfo
                      , h = t.id
                      , b = Object.keys(r).length
                      , v = jc(Ee.view, {
                        from: "manageDvrConfig.modalTitle"
                    })
                      , g = jc(Ee.view, {
                        from: "manageDvrConfig.mastheadTitle"
                    })
                      , y = {
                        onEnter: te.a,
                        waitFor: 250,
                        threshold: 0,
                        metricsParams: Object(ee.a)(h, 0, i, p)
                    };
                    return jc(Io, {
                        className: "ManageDVR"
                    }, jc(io, {
                        title: v,
                        className: "ManageDVR__nav"
                    }), jc("div", {
                        className: "ManageDVR__masthead"
                    }, jc("div", {
                        className: "ManageDVR__title"
                    }, g), jc(sc, {
                        availableDvrStorage: d
                    })), jc(U.a, y, jc(_c, {
                        items: n,
                        stateKey: i,
                        collectionId: h,
                        collectionIndex: 0,
                        subnavIndex: a,
                        itemsPendingRemoval: r,
                        seriesPendingRemoval: o,
                        toggleTile: c,
                        toggleSeries: l,
                        tabs: f,
                        pageType: m
                    })), jc(Qn, {
                        totalSelectedItems: b,
                        handleCancel: s,
                        handleRemove: u,
                        footerTextPath: "manageDvrConfig.footer",
                        manageDVR: !0
                    }))
                }
            }]),
            t
        }(d.a.Component)
          , wc = n("4R/G")
          , Tc = function(e) {
            return R()(e[Zi.b], "collections", [])
        }
          , Sc = Object(J.createSelector)([Tc], (function(e) {
            var t = e[Object(wc.a)(e)];
            return R()(t, "items", []).reduce((function(e, t) {
                var n = Object.assign({}, e);
                return "sports_episode" === t._type.toLowerCase() ? n.sports[t.id] = t : "movie" === t._type.toLowerCase() ? n.movies[t.id] = t : "series" === t._type.toLowerCase() ? n.episodes[t.id] ? n.episodes[t.id].push(t) : n.episodes[t.id] = [t] : n.episodes[t.series_id] ? n.episodes[t.series_id].push(t) : n.episodes[t.series_id] = [t],
                n
            }
            ), {
                episodes: {},
                sports: {},
                movies: {}
            })
        }
        ))
          , Ec = Object(J.createSelector)([Sc, fe.a], (function(e, t) {
            return t.manageDvrConfig.tabs.filter((function(t) {
                return e[t] && Object.keys(e[t]).length > 0
            }
            )).map((function(e) {
                return {
                    key: e,
                    name: e
                }
            }
            ))
        }
        ))
          , kc = Object(J.createSelector)([function(e) {
            return e[Zi.b]
        }
        , Tc, function(e) {
            return R()(e[Zi.b], "dataReady")
        }
        , function(e) {
            return R()(e[Zi.b], "availableDvrStorage")
        }
        , Sc, Ec, function(e) {
            return e[Zi.b].pageType
        }
        , function(e) {
            return e[Zi.b].metrics_info
        }
        ], (function(e, t, n, a, i, r, o, c) {
            var l = Object(wc.a)(t);
            return {
                dataReady: n,
                collection: t[l] || {},
                organizedDVRItems: i,
                collectionIndex: l,
                stateKey: Zi.b,
                availableDvrStorage: a,
                tabs: r,
                pageType: o,
                metricsInfo: c
            }
        }
        ))
          , Cc = Object(P.a)(Object(de.a)((function(e) {
            var t = e.geo;
            return {
                url: ne.b.default({
                    entity: "watch-later",
                    limit: 100
                }),
                geo: t
            }
        }
        )), Object(g.connect)(kc, {
            initContent: Xi.initContentAndFetchAllItems,
            setIsLoadingData: Xi.setIsLoadingData,
            initTileStates: Xi.appendTileStates,
            removeCallback: Xi.removeDvrItems,
            fetchDvrStorage: Xi.fetchDvrStorage
        }), ue.a, ni)(Ic)
          , Nc = (n("d2bg"),
        d.a.createElement);
        var Pc = function(e) {
            var t = e.pageType
              , n = e.geo
              , a = e.goBackToLevelOne
              , i = e.heimdallEntity
              , r = e.heimdallEntityId
              , o = e.heimdallCollectionId
              , c = e.pageName
              , l = e.networkName
              , s = e.networkBreadcrumb
              , d = e.isTopLevel
              , f = e.discoverHref;
            Object(Wi.d)(d, Zi.b),
            Object(u.useEffect)((function() {
                return t ? document.documentElement.classList.add("has-modal") : document.documentElement.classList.remove("has-modal"),
                function() {
                    document.documentElement.classList.remove("has-modal")
                }
            }
            ), [t]);
            var m = null;
            switch (t) {
            case S.j.DETAILS:
                m = Bo;
                break;
            case S.j.NETWORK:
                m = Zo;
                break;
            case S.j.HUB_COLLECTION:
                m = ic;
                break;
            case S.j.MANAGE_DVR:
                m = Cc;
                break;
            default:
                return null
            }
            var p = {
                tealiumPageType: Object(Jn.c)(t, i),
                pageName: c,
                networkName: l,
                heimdallEntityId: r,
                stateKey: Zi.b
            };
            return Nc("div", {
                className: "LevelTwo",
                "data-testid": "level-two",
                onClick: a,
                "aria-hidden": !d
            }, Nc("div", {
                className: "LevelTwo__backdrop"
            }), Nc("div", {
                className: "LevelTwo__scroll-area",
                id: "LevelTwo__scroll-area"
            }, Nc("div", {
                className: "LevelTwo__container",
                "data-testid": "level-two-container",
                onClick: function(e) {
                    return e.stopPropagation()
                },
                onKeyDown: function(e) {
                    "Escape" === e.key && a()
                }
            }, Nc(zi.a, p, Nc(m, {
                className: "LevelTwo__content",
                geo: n,
                networkBreadcrumb: s,
                pageType: t,
                heimdallEntity: i,
                heimdallEntityId: r,
                heimdallCollectionId: o,
                discoverHref: f
            })))))
        }
          , Dc = Object(J.createSelector)([function(e) {
            return e[Zi.b].pageType
        }
        , function(e) {
            return e[Zi.b].heimdallEntity
        }
        , function(e) {
            return e[Zi.b].heimdallEntityId
        }
        , function(e) {
            return e[Zi.b].heimdallCollectionId
        }
        , function(e) {
            return e[Zi.b].name
        }
        , function(e) {
            return R()(e[Zi.b], "details.entity.primary_branding.name")
        }
        , function(e) {
            return e[Zi.b].networkBreadcrumb
        }
        , Z.a, function(e) {
            return e[Zi.b].discoverHref
        }
        ], (function(e, t, n, a, i, r, o, c, l) {
            return {
                pageType: e,
                heimdallEntity: t,
                heimdallEntityId: n,
                heimdallCollectionId: a,
                pageName: i && i.toLowerCase(),
                networkName: r,
                networkBreadcrumb: o,
                isTopLevel: c === Zi.b,
                discoverHref: l
            }
        }
        ))
          , xc = Object(g.connect)((function(e) {
            return Dc(e)
        }
        ), {
            goBackToLevelOne: b.goBackToLevelOne
        })(Pc)
          , Lc = n("ClnW")
          , Rc = n("dAsV")
          , Mc = d.a.createElement
          , Ac = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "state", {
                    stepIndex: 0
                }),
                Object(C.a)(Object(D.a)(n), "next", (function() {
                    var e = n.props
                      , t = e.steps
                      , a = e.setModalClosed;
                    n.state.stepIndex >= t.length - 1 && a(!0),
                    n.setState({
                        stepIndex: n.state.stepIndex + 1
                    })
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "render",
                value: function() {
                    var e = this.props.steps[this.state.stepIndex];
                    return e ? Mc(e, {
                        next: this.next
                    }) : null
                }
            }]),
            t
        }(u.Component)
          , Gc = n("/gXB")
          , Bc = Object(J.createSelector)([function(e) {
            return e[Gc.b].currentProfileId
        }
        ], (function(e) {
            return {
                currentProfileId: e
            }
        }
        ))
          , Fc = Object(g.connect)((function(e) {
            return Bc(e)
        }
        ), {
            setModalClosed: Lc.a
        })(Ac)
          , Hc = n("7bJG")
          , Uc = n("/x/M")
          , Vc = "".concat(Rc.b, ".welcome_modal")
          , zc = Object(P.a)(Object(q.a)(Vc), Object(g.connect)(vi.c))(Uc.a)
          , Wc = n("4Tvr")
          , Yc = n("p075")
          , Kc = (n("BguU"),
        d.a.createElement)
          , qc = function(e) {
            var t = e.configKey
              , n = e.replace
              , a = e.hidden;
            return Kc("div", {
                className: B()("Notification", {
                    "Notification--hidden": a
                })
            }, Kc("div", {
                className: "Notification__text",
                "aria-live": "polite"
            }, Kc(Ee.view, {
                from: t,
                replace: n
            })))
        }
          , Jc = Object(g.connect)((function(e) {
            var t = e[Yc.b];
            return {
                configKey: t.configKey,
                replace: t.replace,
                hidden: t.hidden
            }
        }
        ))(qc)
          , Xc = (n("KfhT"),
        d.a.createElement)
          , Zc = function(e) {
            var t = e.isLoading
              , n = e.lightTheme
              , a = B()("Loader", {
                "Loader--disabled": !t,
                "Loader--light": n
            });
            return Xc("div", {
                className: a
            }, Xc("div", {
                className: "Loader__bar"
            }, Xc("div", {
                className: "Loader__animation-progress"
            })))
        }
          , Qc = function(e) {
            var t = R()(e, [v.b, "pageType"])
              , n = R()(e, [v.b, "dataReady"])
              , a = R()(e, [Zi.b, "dataReady"]);
            return t !== S.j.SEARCH && (t === S.j.HOME || t === S.j.HUB || t === S.j.LIVE_GUIDE ? !!Object(Z.b)(e) && !a : Object(Z.b)(e) ? !a : !n)
        }
          , $c = Object(g.connect)((function(e) {
            return {
                isLoading: Qc(e),
                lightTheme: Object(Z.b)(e)
            }
        }
        ))(Zc)
          , el = n("sKeN")
          , tl = n("oTX3")
          , nl = n("yLiY")
          , al = n.n(nl)
          , il = n("/dNt")
          , rl = n("aPL3")
          , ol = n("qv/e")
          , cl = n("dyG4")
          , ll = (n("ZiUS"),
        function(e) {
            return e
        }
        )
          , sl = function(e) {
            if ("video" === e.type) {
                var t = "/watch/".concat(e.entityId);
                window.location.pathname.indexOf("/watch/") > -1 ? Object(p.g)(t) : Object(p.e)(t)
            } else
                "network" === e.type ? Object(p.c)("/network/".concat(e.networkId)) : "account" === e.type ? Object(p.d)("/account") : Object(p.c)("/".concat(e.entityType, "/").concat(e.entityId))
        }
          , ul = function(e) {
            e !== S.l.PAGE ? document.documentElement.classList.remove("player-modal") : document.documentElement.classList.add("player-modal")
        }
          , dl = d.a.createElement;
        function fl(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function ml(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = null != arguments[t] ? arguments[t] : {};
                t % 2 ? fl(Object(n), !0).forEach((function(t) {
                    Object(C.a)(e, t, n[t])
                }
                )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : fl(Object(n)).forEach((function(t) {
                    Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                }
                ))
            }
            return e
        }
        var pl = "browse"
          , hl = "live_guide"
          , bl = function(e) {
            function t() {
                var e, n;
                Object(r.a)(this, t);
                for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                    i[o] = arguments[o];
                return n = Object(c.a)(this, (e = Object(l.a)(t)).call.apply(e, [this].concat(i))),
                Object(C.a)(Object(D.a)(n), "backToBrowse", (function(e) {
                    var t = n.props
                      , a = t.setPlayerType
                      , i = t.toBrowse;
                    e && "pip" === e.type ? a(S.l.MODAL) : n.clearPlayer(),
                    i()
                }
                )),
                Object(C.a)(Object(D.a)(n), "backToWatch", (function() {
                    (0,
                    n.props.setPlayerType)(S.l.PAGE)
                }
                )),
                Object(C.a)(Object(D.a)(n), "closePlayer", (function() {
                    var e = n.props
                      , t = e.display
                      , a = e.toBrowse;
                      console.log("closePlayer ..");
                    t === S.l.PAGE && a(),
                    Fe.Player.actions.stopVideo(),
                    n.clearPlayer()
                }
                )),
                Object(C.a)(Object(D.a)(n), "refresh", (function() {
                    var e = n.props
                      , t = e.refreshLevelOne;
                    (0,
                    e.refreshLevelTwo)(),
                    t()
                }
                )),
                Object(C.a)(Object(D.a)(n), "showLoginForm", (function() {}
                )),
                Object(C.a)(Object(D.a)(n), "refreshUserLogin", (function() {
                    var e = "/v1/web/chromecast/authenticate";
                    return Object(rl.a)(e, "hoth").then((function(t) {
                        return fetch("".concat(al()().publicRuntimeConfig.endpoints.hoth).concat(e), {
                            method: "POST",
                            credentials: "include",
                            body: "csrf=".concat(t.csrf)
                        }).then((function(e) {
                            return 200 === e.status ? e.json() : Promise.reject({
                                err: e
                            })
                        }
                        ))
                    }
                    ))
                }
                )),
                Object(C.a)(Object(D.a)(n), "clearPlayer", (function() {
                    (0,
                    n.props.setPlayerStatus)({
                        type: S.l.NONE,
                        eabId: null,
                        isLiveTV: !1,
                        collectionId: null,
                        channelId: null
                    })
                }
                )),
                Object(C.a)(Object(D.a)(n), "goToGuide", (function() {
                    (0,
                    n.props.setPlayerType)(S.l.MODAL),
                    Object(p.c)("/live")
                }
                )),
                n
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentWillUnmount",
                value: function() {
                    console.log("componentWillUnmount");
                    Fe.Player.actions.stopVideo()
                }
            }, {
                key: "shouldComponentUpdate",
                value: function(e) {
                    var t = this.props
                      , n = t.eabId
                      , a = t.isLiveTV
                      , i = t.display
                      , r = t.restartTime
                      , o = i === S.l.NONE !== (e.display === S.l.NONE)
                      , c = a !== e.isLiveTV
                      , l = n && n !== e.eabId
                      , s = e.playIntent === S.m.RESTART && r !== e.restartTime;
                    return o || l || c || s
                }
            }, {
                key: "componentDidMount",
                value: function() {
                    this.checkPlay()
                }
            }, {
                key: "componentDidUpdate",
                value: function() {
                    this.checkPlay()
                }
            }, {
                key: "render",
                value: function() {
                    var e = ml({
                        getHitInfo: ll
                    }, this.getFunctionProps(), {}, {
                        computerGuid: Object(il.c)(il.a.GUID)
                    }, {}, function() {
                        var e = Object(el.c)();
                        return e ? {
                            latitude: e.lat,
                            longitude: e.long
                        } : {}
                    }(), {}, this.props, {
                        config: {
                            isNonSub: !1,
                            liveGuideEnabled: !0
                        }
                    });
                    return dl(Fe.Player.view, e)
                }
            }, {
                key: "checkPlay",
                value: function() {
                    console.log("checkPlay ...");
                    var e = this
                      , t = Fe.Player.actions.stopVideo
                      , n = this.props
                      , a = n.eabId
                      , i = n.collectionId
                      , r = n.channelId
                      , o = n.display
                      , c = n.isLiveTV
                      , l = n.setEabId
                      , s = R()(this.props, "entityId", Object(F.g)());
                    if (o === S.l.NONE || c)
                        o !== S.l.NONE && c ? this.getProfileAndPlay(null, {
                            isLiveTV: c,
                            channelId: r
                        }) : t();
                    else if (a)
                        this.getProfileAndPlay(a, {
                            channelId: r,
                            collectionId: i
                        });
                    else if (s) {
                        var u = ne.b.eabId({
                            entityId: s
                        });
                        Object(lt.a)(u, {}, [], Object(el.c)()).then((function(t) {
                            var n = R()(t, "eab_id", null);
                            l(n, S.m.RESUME),
                            e.getProfileAndPlay(n, {})
                        }
                        )).catch((function(t) {
                            console.log("checkPlay catch " + JSON.stringify(t));
                            e.getProfileAndPlay(null),
                            Object(ol.a)(t)
                        }
                        ))
                    }
                }
            }, {
                key: "getProfileAndPlay",
                value: function(e) {
                    console.log("getProfileAndPlay ...");
                    var t = arguments.length > 1 && void 0 !== arguments[1] ? arguments[1] : {}
                      , n = Fe.Player.actions
                      , a = n.startVideo
                      , i = this.props
                      , r = i.currentProfile
                      , o = i.isKidsProfile
                      , c = i.subscription
                      , l = i.router
                      , s = i.playIntent
                      , u = Object(il.c)(il.a.HULU_UID)
                      , d = Object(F.k)(l.asPath) ? hl : pl;
                      console.log("getProfileAndPlay userId: " + u);
                    a(e, {
                        profile: r,
                        subscription: c,
                        userId: u
                    }, ml({}, t, {
                        source: d,
                        play_intent: s
                    })),
                    o || Object(cl.a)(this.getTealiumViewConfig())
                }
            }, {
                key: "getTealiumViewConfig",
                value: function() {
                    var e = this.props
                      , t = e.userEmail
                      , n = e.tealiumProductId;
                    return Object(Jn.a)(S.n.page_type.watch, null, t, n)
                }
            }, {
                key: "getFunctionProps",
                value: function() {
                    return {
                        backToBrowse: this.backToBrowse,
                        backToWatch: this.backToWatch,
                        closePlayer: this.closePlayer,
                        navigateTo: sl,
                        showDetails: (e = this.props.setPlayerType,
                        function(t) {
                            var n, a, r, o, c;
                            return i.a.async((function(l) {
                                for (; ; )
                                    switch (l.prev = l.next) {
                                    case 0:
                                        return e(S.l.MODAL),
                                        l.next = 3,
                                        i.a.awrap(fetch(ne.b.entity(t), {
                                            credentials: "include"
                                        }).then((function(e) {
                                            return e.json()
                                        }
                                        )));
                                    case 3:
                                        n = l.sent,
                                        a = R()(n, "items[0]"),
                                        r = Object(p.a)(a, t),
                                        o = r.detailsUrl,
                                        c = r.discoverHref,
                                        Object(p.c)(o, {}, c);
                                    case 7:
                                    case "end":
                                        return l.stop()
                                    }
                            }
                            ))
                        }
                        ),
                        refresh: this.refresh,
                        showLoginForm: this.showLoginForm,
                        refreshUserLogin: this.refreshUserLogin,
                        goToGuide: this.goToGuide
                    };
                    var e
                }
            }]),
            t
        }(d.a.Component);
        bl.defaultProps = {
            eabId: null,
            display: S.l.NONE,
            isLiveTV: !1
        };
        var vl = Object(f.withRouter)(bl)
          , gl = (n("ZAC8"),
        d.a.createElement)
          , yl = 10
          , Ol = n("ijAN")
          , _l = Object(g.connect)((function(e) {
            return Object(Ol.a)(e)
        }
        ), {
            setPlayerType: qt.setPlayerType,
            setPlayerStatus: qt.setPlayerStatus,
            setEabId: qt.setEabId,
            toBrowse: qt.toBrowse,
            refreshLevelOne: b.refresh,
            refreshLevelTwo: Xi.refresh
        })(function(e) {
            var t = function(t) {
                function n() {
                    var e, t;
                    Object(r.a)(this, n);
                    for (var a = arguments.length, i = new Array(a), o = 0; o < a; o++)
                        i[o] = arguments[o];
                    return t = Object(c.a)(this, (e = Object(l.a)(n)).call.apply(e, [this].concat(i))),
                    Object(C.a)(Object(D.a)(t), "state", {
                        right: yl,
                        bottom: yl
                    }),
                    Object(C.a)(Object(D.a)(t), "cursor2PlayerRB", null),
                    Object(C.a)(Object(D.a)(t), "windowSize", null),
                    Object(C.a)(Object(D.a)(t), "playerSize", null),
                    Object(C.a)(Object(D.a)(t), "onPointerDown", (function(e) {
                        e.preventDefault(),
                        0 === e.button && t.props.display === S.l.MODAL && (t.updateSize(),
                        t.cursor2PlayerRB = {
                            width: t.windowSize.width - e.clientX - t.state.right,
                            height: t.windowSize.height - e.clientY - t.state.bottom
                        },
                        window.addEventListener("pointermove", t.onPointerMove),
                        window.addEventListener("pointerup", t.onPointerUp))
                    }
                    )),
                    Object(C.a)(Object(D.a)(t), "onPointerUp", (function() {
                        window.removeEventListener("pointermove", t.onPointerMove),
                        window.removeEventListener("pointerup", t.onPointerUp)
                    }
                    )),
                    Object(C.a)(Object(D.a)(t), "onPointerMove", (function(e) {
                        e.stopPropagation(),
                        e.preventDefault();
                        var n = t.windowSize.width - e.clientX - t.cursor2PlayerRB.width
                          , a = t.windowSize.height - e.clientY - t.cursor2PlayerRB.height
                          , i = t.getAdjustedPosition(n, a);
                        t.setState(i)
                    }
                    )),
                    Object(C.a)(Object(D.a)(t), "getContent", (function() {
                        return t._player
                    }
                    )),
                    Object(C.a)(Object(D.a)(t), "onResize", (function() {
                        if (t.props.display === S.l.MODAL) {
                            t.updateSize();
                            var e = t.getAdjustedPosition(t.state.right, t.state.bottom);
                            e.right === t.state.right && e.bottom === t.state.bottom || t.setState(e)
                        }
                    }
                    )),
                    t
                }
                return Object(s.a)(n, t),
                Object(o.a)(n, [{
                    key: "updateSize",
                    value: function() {
                        this.windowSize = {
                            width: window.innerWidth,
                            height: window.innerHeight
                        },
                        this.playerSize = {
                            width: this._player ? this._player.offsetWidth : 1e4,
                            height: this._player ? this._player.offsetHeight : 1e4
                        }
                    }
                }, {
                    key: "componentDidUpdate",
                    value: function() {
                        ul(this.props.display),
                        this.props.display === S.l.PAGE ? this.keepFocus || (this.keepFocus = Object(_o.a)(this.getContent)) : this.keepFocus && (this.keepFocus.remove(),
                        delete this.keepFocus)
                    }
                }, {
                    key: "componentDidMount",
                    value: function() {
                        window.addEventListener("resize", this.onResize),
                        ul(this.props.display)
                    }
                }, {
                    key: "componentWillUnmount",
                    value: function() {
                        window.removeEventListener("resize", this.onResize),
                        this.keepFocus && this.keepFocus.remove()
                    }
                }, {
                    key: "render",
                    value: function() {
                        var t = this
                          , n = this.props.display
                          , a = {
                            display: n !== S.l.NONE ? "block" : "none"
                        };
                        n === S.l.MODAL && (a.right = "".concat(this.state.right, "px"),
                        a.bottom = "".concat(this.state.bottom, "px"));
                        var i = B()("Player__container", {
                            "Player__container--modal": n === S.l.MODAL
                        }, {
                            addFocus: n === S.l.PAGE
                        })
                          , r = R()(Fe.Player, "actions.switchMode", null);
                        return r && (n === S.l.MODAL ? r("modal") : r("page")),
                        gl("div", {
                            ref: function(e) {
                                t._player = e
                            },
                            style: a,
                            className: i,
                            onPointerDown: this.onPointerDown,
                            tabIndex: n === S.l.NONE ? -1 : 0,
                            "aria-hidden": n === S.l.NONE,
                            "aria-label": "Video Player",
                            role: "region"
                        }, gl(e, this.props))
                    }
                }, {
                    key: "getAdjustedPosition",
                    value: function(e, t) {
                        var n = {
                            right: e,
                            bottom: t
                        }
                          , a = this.playerSize.width
                          , i = this.playerSize.height
                          , r = this.windowSize.width
                          , o = this.windowSize.height;
                        return e + a + yl > r && (n.right = r - a - yl),
                        n.right = n.right < yl ? yl : n.right,
                        t + i + yl > o && (n.bottom = o - i - yl),
                        n.bottom = n.bottom < yl ? yl : n.bottom,
                        n
                    }
                }]),
                n
            }(u.Component);
            return Object(C.a)(t, "defaultProps", {
                display: S.l.NONE
            }),
            t
        }(vl))
          , jl = d.a.createElement;
        function Il(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function wl(e) {
            return function() {
                return function(e) {
                    for (var t = 1; t < arguments.length; t++) {
                        var n = null != arguments[t] ? arguments[t] : {};
                        t % 2 ? Il(Object(n), !0).forEach((function(t) {
                            Object(C.a)(e, t, n[t])
                        }
                        )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : Il(Object(n)).forEach((function(t) {
                            Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                        }
                        ))
                    }
                    return e
                }({
                    current_url: e,
                    last_url: Tl()
                }, Pn.a(), {
                    trackEvent: Pn.f,
                    trackAdAuditBeacon: Pn.e
                })
            }
        }
        function Tl() {
            var e = window.store;
            if (R()(e.getState()[Zi.b], "heimdallEntity", null)) {
                var t = R()(e.getState(), Zi.b, {})
                  , n = t.heimdallEntity
                  , a = t.heimdallEntityId;
                return Object(F.a)(n, a)
            }
            var i = R()(e.getState(), v.b, {})
              , r = i.heimdallEntity
              , o = i.heimdallEntityId;
            return Object(F.a)(r, o)
        }
        var Sl = function(e) {
            var t = function(t) {
                function n() {
                    return Object(r.a)(this, n),
                    Object(c.a)(this, Object(l.a)(n).apply(this, arguments))
                }
                return Object(s.a)(n, t),
                Object(o.a)(n, [{
                    key: "render",
                    value: function() {
                        return jl(e, this.props)
                    }
                }, {
                    key: "componentDidUpdate",
                    value: function() {
                        this.renderPlayer()
                    }
                }, {
                    key: "componentDidMount",
                    value: function() {
                        this.renderPlayer()
                    }
                }, {
                    key: "componentWillUnmount",
                    value: function() {
                        We.a.unmountComponentAtNode(document.getElementById("__player__"))
                    }
                }, {
                    key: "renderPlayer",
                    value: function() {
                        We.a.render(jl(g.Provider, {
                            store: window.store
                        }, jl(_l, Object(x.a)({}, this.props, {
                            getHitInfo: wl(this.props.router.asPath)
                        }))), document.getElementById("__player__"))
                    }
                }]),
                n
            }(d.a.Component);
            return t.displayName = Object(tl.b)("withPlayer", e),
            t.getInitialProps = Object(tl.a)(e),
            Object(f.withRouter)(t)
        }
          , El = n("PlE4")
          , kl = n("17x9")
          , Cl = function(e) {
            var t = e.children
              , n = Object(u.useState)(!1)
              , a = n[0]
              , i = n[1];
            return Object(u.useEffect)((function() {
                return i(!0)
            }
            ), []),
            a ? t : null
        };
        Cl.propTypes = {
            children: n.n(kl).a.node
        };
        var Nl, Pl = Cl, Dl = n("45R/"), xl = n("WBkB"), Ll = d.a.createElement, Rl = Ji, Ml = xc, Al = Fc, Gl = [zc], Bl = function(e) {
            function t() {
                return Object(r.a)(this, t),
                Object(c.a)(this, Object(l.a)(t).apply(this, arguments))
            }
            return Object(s.a)(t, e),
            Object(o.a)(t, [{
                key: "componentDidMount",
                value: function() {
                    m.a.prefetch("/error"),
                    m.a.prefetch("/offline")
                }
            }, {
                key: "render",
                value: function() {
                    var e = this.props
                      , t = e.bodyClassName
                      , n = e.geoCookie
                      , a = e.userEntitlement
                      , i = Object(el.a)(n)
                      , r = {
                        user_entitlement: a
                    };
                    return Ll(u.Fragment, null, Ll(Kt.c, null, Ll(Ve.b, {
                        customAttributes: r
                    }, Ll(Hc.a, {
                        className: t
                    }), Ll(Ml, {
                        geo: i
                    }), Ll(Rl, {
                        geo: i
                    }), Ll(Pl, null, Ll(Wc.a, null, Ll(Al, {
                        steps: Gl
                    }))), Ll($c, null), Ll(Jc, null))))
                }
            }], [{
                key: "getInitialProps",
                value: function(e) {
                    var t, n, a, r, o, c, l, s, u, d, f, m, v, g, y, O, _, j, I, w, T;
                    return i.a.async((function(S) {
                        for (; ; )
                            switch (S.prev = S.next) {
                            case 0:
                                if (t = e.store,
                                n = e.req,
                                a = e.asPath,
                                r = e.query,
                                o = r.id,
                                c = r.collectionId,
                                l = r.discoverHref,
                                s = R()(e.query, "entity", "home"),
                                u = Object(h.e)(s),
                                f = Object(F.c)(o),
                                m = c ? null : ne.b.deeplink(f, s),
                                !l) {
                                    S.next = 11;
                                    break
                                }
                                d = decodeURIComponent(l),
                                S.next = 17;
                                break;
                            case 11:
                                if (l || !m) {
                                    S.next = 17;
                                    break
                                }
                                return v = n ? {
                                    headers: {
                                        Cookie: n.headers.cookie
                                    }
                                } : {
                                    credentials: "include"
                                },
                                S.next = 15,
                                i.a.awrap(fetch(m, v).then((function(e) {
                                    return e.json()
                                }
                                )).catch((function() {
                                    return null
                                }
                                )));
                            case 15:
                                g = S.sent,
                                d = R()(g, "href");
                            case 17:
                                if (Object(vi.a)(t.getState())) {
                                    S.next = 20;
                                    break
                                }
                                return Object(p.c)("/profiles?next=".concat(a.replace(/^\/app/, "")), e),
                                S.abrupt("return", {});
                            case 20:
                                if (y = Object(vi.b)(t.getState()),
                                !y.isKidsProfile || Object(xl.isKidsSafe)(s, o)) {
                                    S.next = 24;
                                    break
                                }
                                return Object(p.c)("/", e),
                                S.abrupt("return", {});
                            case 24:
                                if (O = Object(fe.c)(t.getState()),
                                _ = O.browse,
                                j = O.player,
                                s === xl.ENTITY_TYPES_TO_URL.LIVE && (s = "live-guide"),
                                I = s === xl.ENTITY_TYPES_TO_URL.LIVE || s === xl.ENTITY_TYPES_TO_URL.WATCH,
                                !0 !== j) {
                                    S.next = 33;
                                    break
                                }
                                if (!0 !== _) {
                                    S.next = 31;
                                    break
                                }
                                return Object(p.c)("/maintenance", e),
                                S.abrupt("return", {});
                            case 31:
                                S.next = 36;
                                break;
                            case 33:
                                if (I || !0 !== _) {
                                    S.next = 36;
                                    break
                                }
                                return Object(p.c)("/maintenance", e),
                                S.abrupt("return", {});
                            case 36:
                                if (w = "true" === R()(e.query, "skip_onboarding"),
                                T = Object(Dl.b)(t.getState()),
                                w || !(T.length > 0)) {
                                    S.next = 41;
                                    break
                                }
                                return Object(p.c)("/onboarding", e),
                                S.abrupt("return", {});
                            case 41:
                                if (t.dispatch(b.determineAction({
                                    entity: s,
                                    id: o,
                                    collectionId: c,
                                    discoverHref: d
                                })),
                                t.dispatch(Xi.determineAction({
                                    entity: s,
                                    id: o,
                                    collectionId: c,
                                    discoverHref: d
                                })),
                                t.dispatch(on.a.determineAction({
                                    entity: s,
                                    id: o
                                })),
                                s !== xl.ENTITY_TYPES_TO_URL.LIVE) {
                                    S.next = 48;
                                    break
                                }
                                return S.abrupt("return", {
                                    isLiveTV: !0,
                                    pageTitle: u,
                                    bodyClassName: "route-Watch"
                                });
                            case 48:
                                if (s !== xl.ENTITY_TYPES_TO_URL.WATCH) {
                                    S.next = 52;
                                    break
                                }
                                return S.abrupt("return", {
                                    entityId: o,
                                    isLiveTV: !1,
                                    eabId: Object(F.f)(o),
                                    collectionId: Object(F.e)(),
                                    pageTitle: u,
                                    bodyClassName: "route-Watch"
                                });
                            case 52:
                                return S.abrupt("return", {
                                    pageTitle: u,
                                    blockBrowse: _,
                                    bodyClassName: "route-Browse"
                                });
                            case 53:
                            case "end":
                                return S.stop()
                            }
                    }
                    ))
                }
            }]),
            t
        }(d.a.Component);
        t.default = (Nl = [Ee, E],
        function(e) {
            var t = arguments.length > 1 && void 0 !== arguments[1] ? arguments[1] : {};
            return Object(P.a)(Object(El.a)(Nl), Sl)(e, t)
        }
        )(Bl)
    },
    aeRQ: function(e, t, n) {
        "use strict";
        Object.defineProperty(t, "__esModule", {
            value: !0
        });
        t.default = function(e, t, n, a, i, r, o, c, l) {
            e.AppsFlyerSdkObject = i,
            e.AF = e.AF || function() {
                (e.AF.q = e.AF.q || []).push([Date.now()].concat(Array.prototype.slice.call(arguments)))
            }
            ,
            e.AF.id = e.AF.id || o,
            e.AF.plugins = {},
            c = t.createElement(n),
            l = t.getElementsByTagName(n)[0],
            c.async = 1,
            c.src = "https://websdk.appsflyer.com?" + (r.length > 0 ? "st=" + r.split(",").sort().join(",") + "&" : "") + (o.length > 0 ? "af_id=" + o : ""),
            l.parentNode.insertBefore(c, l)
        }
    },
    apDF: function(e, t, n) {
        "use strict";
        var a = this && this.__importStar || function(e) {
            if (e && e.__esModule)
                return e;
            var t = {};
            if (null != e)
                for (var n in e)
                    Object.hasOwnProperty.call(e, n) && (t[n] = e[n]);
            return t.default = e,
            t
        }
          , i = this && this.__importDefault || function(e) {
            return e && e.__esModule ? e : {
                default: e
            }
        }
        ;
        Object.defineProperty(t, "__esModule", {
            value: !0
        });
        var r = a(n("q1tI"))
          , o = i(n("7jF1"));
        t.default = function(e) {
            var t = e.bannerKey
              , n = e.creativeId
              , a = e.additionalParams
              , i = e.onError
              , c = o.default({
                bannerKey: t,
                onError: i
            })
              , l = c[0]
              , s = c[1];
            return r.useEffect((function() {
                return l({
                    bannerKey: t,
                    creativeId: n,
                    additionalParams: a
                }),
                function() {
                    s()
                }
            }
            ), [t, n, a]),
            null
        }
    },
    dwco: function(e, t, n) {
        !function() {
            "use strict";
            e.exports = {
                polyfill: function() {
                    var e = window
                      , t = document;
                    if (!("scrollBehavior"in t.documentElement.style && !0 !== e.__forceSmoothScrollPolyfill__)) {
                        var n, a = e.HTMLElement || e.Element, i = 468, r = {
                            scroll: e.scroll || e.scrollTo,
                            scrollBy: e.scrollBy,
                            elementScroll: a.prototype.scroll || l,
                            scrollIntoView: a.prototype.scrollIntoView
                        }, o = e.performance && e.performance.now ? e.performance.now.bind(e.performance) : Date.now, c = (n = e.navigator.userAgent,
                        new RegExp(["MSIE ", "Trident/", "Edge/"].join("|")).test(n) ? 1 : 0);
                        e.scroll = e.scrollTo = function() {
                            void 0 !== arguments[0] && (!0 !== s(arguments[0]) ? h.call(e, t.body, void 0 !== arguments[0].left ? ~~arguments[0].left : e.scrollX || e.pageXOffset, void 0 !== arguments[0].top ? ~~arguments[0].top : e.scrollY || e.pageYOffset) : r.scroll.call(e, void 0 !== arguments[0].left ? arguments[0].left : "object" !== typeof arguments[0] ? arguments[0] : e.scrollX || e.pageXOffset, void 0 !== arguments[0].top ? arguments[0].top : void 0 !== arguments[1] ? arguments[1] : e.scrollY || e.pageYOffset))
                        }
                        ,
                        e.scrollBy = function() {
                            void 0 !== arguments[0] && (s(arguments[0]) ? r.scrollBy.call(e, void 0 !== arguments[0].left ? arguments[0].left : "object" !== typeof arguments[0] ? arguments[0] : 0, void 0 !== arguments[0].top ? arguments[0].top : void 0 !== arguments[1] ? arguments[1] : 0) : h.call(e, t.body, ~~arguments[0].left + (e.scrollX || e.pageXOffset), ~~arguments[0].top + (e.scrollY || e.pageYOffset)))
                        }
                        ,
                        a.prototype.scroll = a.prototype.scrollTo = function() {
                            if (void 0 !== arguments[0])
                                if (!0 !== s(arguments[0])) {
                                    var e = arguments[0].left
                                      , t = arguments[0].top;
                                    h.call(this, this, "undefined" === typeof e ? this.scrollLeft : ~~e, "undefined" === typeof t ? this.scrollTop : ~~t)
                                } else {
                                    if ("number" === typeof arguments[0] && void 0 === arguments[1])
                                        throw new SyntaxError("Value could not be converted");
                                    r.elementScroll.call(this, void 0 !== arguments[0].left ? ~~arguments[0].left : "object" !== typeof arguments[0] ? ~~arguments[0] : this.scrollLeft, void 0 !== arguments[0].top ? ~~arguments[0].top : void 0 !== arguments[1] ? ~~arguments[1] : this.scrollTop)
                                }
                        }
                        ,
                        a.prototype.scrollBy = function() {
                            void 0 !== arguments[0] && (!0 !== s(arguments[0]) ? this.scroll({
                                left: ~~arguments[0].left + this.scrollLeft,
                                top: ~~arguments[0].top + this.scrollTop,
                                behavior: arguments[0].behavior
                            }) : r.elementScroll.call(this, void 0 !== arguments[0].left ? ~~arguments[0].left + this.scrollLeft : ~~arguments[0] + this.scrollLeft, void 0 !== arguments[0].top ? ~~arguments[0].top + this.scrollTop : ~~arguments[1] + this.scrollTop))
                        }
                        ,
                        a.prototype.scrollIntoView = function() {
                            if (!0 !== s(arguments[0])) {
                                var n = m(this)
                                  , a = n.getBoundingClientRect()
                                  , i = this.getBoundingClientRect();
                                n !== t.body ? (h.call(this, n, n.scrollLeft + i.left - a.left, n.scrollTop + i.top - a.top),
                                "fixed" !== e.getComputedStyle(n).position && e.scrollBy({
                                    left: a.left,
                                    top: a.top,
                                    behavior: "smooth"
                                })) : e.scrollBy({
                                    left: i.left,
                                    top: i.top,
                                    behavior: "smooth"
                                })
                            } else
                                r.scrollIntoView.call(this, void 0 === arguments[0] || arguments[0])
                        }
                    }
                    function l(e, t) {
                        this.scrollLeft = e,
                        this.scrollTop = t
                    }
                    function s(e) {
                        if (null === e || "object" !== typeof e || void 0 === e.behavior || "auto" === e.behavior || "instant" === e.behavior)
                            return !0;
                        if ("object" === typeof e && "smooth" === e.behavior)
                            return !1;
                        throw new TypeError("behavior member of ScrollOptions " + e.behavior + " is not a valid value for enumeration ScrollBehavior.")
                    }
                    function u(e, t) {
                        return "Y" === t ? e.clientHeight + c < e.scrollHeight : "X" === t ? e.clientWidth + c < e.scrollWidth : void 0
                    }
                    function d(t, n) {
                        var a = e.getComputedStyle(t, null)["overflow" + n];
                        return "auto" === a || "scroll" === a
                    }
                    function f(e) {
                        var t = u(e, "Y") && d(e, "Y")
                          , n = u(e, "X") && d(e, "X");
                        return t || n
                    }
                    function m(e) {
                        for (; e !== t.body && !1 === f(e); )
                            e = e.parentNode || e.host;
                        return e
                    }
                    function p(t) {
                        var n, a, r, c, l = (o() - t.startTime) / i;
                        c = l = l > 1 ? 1 : l,
                        n = .5 * (1 - Math.cos(Math.PI * c)),
                        a = t.startX + (t.x - t.startX) * n,
                        r = t.startY + (t.y - t.startY) * n,
                        t.method.call(t.scrollable, a, r),
                        a === t.x && r === t.y || e.requestAnimationFrame(p.bind(e, t))
                    }
                    function h(n, a, i) {
                        var c, s, u, d, f = o();
                        n === t.body ? (c = e,
                        s = e.scrollX || e.pageXOffset,
                        u = e.scrollY || e.pageYOffset,
                        d = r.scroll) : (c = n,
                        s = n.scrollLeft,
                        u = n.scrollTop,
                        d = l),
                        p({
                            scrollable: c,
                            method: d,
                            startTime: f,
                            startX: s,
                            startY: u,
                            x: a,
                            y: i
                        })
                    }
                }
            }
        }()
    },
    eMJF: function(e, t, n) {
        "use strict";
        n("dMi1");
        var a = n("q1tI")
          , i = n.n(a)
          , r = n("J+Nd")
          , o = i.a.createElement;
        t.a = function() {
            return o("div", {
                className: "ExceptionText"
            }, o("div", {
                className: "ExceptionText--Large"
            }, o(r.view, {
                from: "search.exception.noResultTitle"
            })), o("div", {
                className: "ExceptionText--Small"
            }, o(r.view, {
                from: "search.exception.noResultDescription"
            })))
        }
    },
    hEOo: function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.MD5 = void 0;
        var a = 0;
        function i(e) {
            return o(r(c(e)))
        }
        function r(e) {
            return s(u(l(e), 8 * e.length))
        }
        function o(e) {
            for (var t, n = a ? "0123456789ABCDEF" : "0123456789abcdef", i = "", r = 0; r < e.length; r++)
                t = e.charCodeAt(r),
                i += n.charAt(t >>> 4 & 15) + n.charAt(15 & t);
            return i
        }
        function c(e) {
            for (var t, n, a = "", i = -1; ++i < e.length; )
                t = e.charCodeAt(i),
                n = i + 1 < e.length ? e.charCodeAt(i + 1) : 0,
                55296 <= t && t <= 56319 && 56320 <= n && n <= 57343 && (t = 65536 + ((1023 & t) << 10) + (1023 & n),
                i++),
                t <= 127 ? a += String.fromCharCode(t) : t <= 2047 ? a += String.fromCharCode(192 | t >>> 6 & 31, 128 | 63 & t) : t <= 65535 ? a += String.fromCharCode(224 | t >>> 12 & 15, 128 | t >>> 6 & 63, 128 | 63 & t) : t <= 2097151 && (a += String.fromCharCode(240 | t >>> 18 & 7, 128 | t >>> 12 & 63, 128 | t >>> 6 & 63, 128 | 63 & t));
            return a
        }
        function l(e) {
            for (var t = Array(e.length >> 2), n = 0; n < t.length; n++)
                t[n] = 0;
            for (n = 0; n < 8 * e.length; n += 8)
                t[n >> 5] |= (255 & e.charCodeAt(n / 8)) << n % 32;
            return t
        }
        function s(e) {
            for (var t = "", n = 0; n < 32 * e.length; n += 8)
                t += String.fromCharCode(e[n >> 5] >>> n % 32 & 255);
            return t
        }
        function u(e, t) {
            e[t >> 5] |= 128 << t % 32,
            e[14 + (t + 64 >>> 9 << 4)] = t;
            for (var n = 1732584193, a = -271733879, i = -1732584194, r = 271733878, o = 0; o < e.length; o += 16) {
                var c = n
                  , l = a
                  , s = i
                  , u = r;
                n = f(n, a, i, r, e[o + 0], 7, -680876936),
                r = f(r, n, a, i, e[o + 1], 12, -389564586),
                i = f(i, r, n, a, e[o + 2], 17, 606105819),
                a = f(a, i, r, n, e[o + 3], 22, -1044525330),
                n = f(n, a, i, r, e[o + 4], 7, -176418897),
                r = f(r, n, a, i, e[o + 5], 12, 1200080426),
                i = f(i, r, n, a, e[o + 6], 17, -1473231341),
                a = f(a, i, r, n, e[o + 7], 22, -45705983),
                n = f(n, a, i, r, e[o + 8], 7, 1770035416),
                r = f(r, n, a, i, e[o + 9], 12, -1958414417),
                i = f(i, r, n, a, e[o + 10], 17, -42063),
                a = f(a, i, r, n, e[o + 11], 22, -1990404162),
                n = f(n, a, i, r, e[o + 12], 7, 1804603682),
                r = f(r, n, a, i, e[o + 13], 12, -40341101),
                i = f(i, r, n, a, e[o + 14], 17, -1502002290),
                n = m(n, a = f(a, i, r, n, e[o + 15], 22, 1236535329), i, r, e[o + 1], 5, -165796510),
                r = m(r, n, a, i, e[o + 6], 9, -1069501632),
                i = m(i, r, n, a, e[o + 11], 14, 643717713),
                a = m(a, i, r, n, e[o + 0], 20, -373897302),
                n = m(n, a, i, r, e[o + 5], 5, -701558691),
                r = m(r, n, a, i, e[o + 10], 9, 38016083),
                i = m(i, r, n, a, e[o + 15], 14, -660478335),
                a = m(a, i, r, n, e[o + 4], 20, -405537848),
                n = m(n, a, i, r, e[o + 9], 5, 568446438),
                r = m(r, n, a, i, e[o + 14], 9, -1019803690),
                i = m(i, r, n, a, e[o + 3], 14, -187363961),
                a = m(a, i, r, n, e[o + 8], 20, 1163531501),
                n = m(n, a, i, r, e[o + 13], 5, -1444681467),
                r = m(r, n, a, i, e[o + 2], 9, -51403784),
                i = m(i, r, n, a, e[o + 7], 14, 1735328473),
                n = p(n, a = m(a, i, r, n, e[o + 12], 20, -1926607734), i, r, e[o + 5], 4, -378558),
                r = p(r, n, a, i, e[o + 8], 11, -2022574463),
                i = p(i, r, n, a, e[o + 11], 16, 1839030562),
                a = p(a, i, r, n, e[o + 14], 23, -35309556),
                n = p(n, a, i, r, e[o + 1], 4, -1530992060),
                r = p(r, n, a, i, e[o + 4], 11, 1272893353),
                i = p(i, r, n, a, e[o + 7], 16, -155497632),
                a = p(a, i, r, n, e[o + 10], 23, -1094730640),
                n = p(n, a, i, r, e[o + 13], 4, 681279174),
                r = p(r, n, a, i, e[o + 0], 11, -358537222),
                i = p(i, r, n, a, e[o + 3], 16, -722521979),
                a = p(a, i, r, n, e[o + 6], 23, 76029189),
                n = p(n, a, i, r, e[o + 9], 4, -640364487),
                r = p(r, n, a, i, e[o + 12], 11, -421815835),
                i = p(i, r, n, a, e[o + 15], 16, 530742520),
                n = h(n, a = p(a, i, r, n, e[o + 2], 23, -995338651), i, r, e[o + 0], 6, -198630844),
                r = h(r, n, a, i, e[o + 7], 10, 1126891415),
                i = h(i, r, n, a, e[o + 14], 15, -1416354905),
                a = h(a, i, r, n, e[o + 5], 21, -57434055),
                n = h(n, a, i, r, e[o + 12], 6, 1700485571),
                r = h(r, n, a, i, e[o + 3], 10, -1894986606),
                i = h(i, r, n, a, e[o + 10], 15, -1051523),
                a = h(a, i, r, n, e[o + 1], 21, -2054922799),
                n = h(n, a, i, r, e[o + 8], 6, 1873313359),
                r = h(r, n, a, i, e[o + 15], 10, -30611744),
                i = h(i, r, n, a, e[o + 6], 15, -1560198380),
                a = h(a, i, r, n, e[o + 13], 21, 1309151649),
                n = h(n, a, i, r, e[o + 4], 6, -145523070),
                r = h(r, n, a, i, e[o + 11], 10, -1120210379),
                i = h(i, r, n, a, e[o + 2], 15, 718787259),
                a = h(a, i, r, n, e[o + 9], 21, -343485551),
                n = b(n, c),
                a = b(a, l),
                i = b(i, s),
                r = b(r, u)
            }
            return [n, a, i, r]
        }
        function d(e, t, n, a, i, r) {
            return b((o = b(b(t, e), b(a, r))) << (c = i) | o >>> 32 - c, n);
            var o, c
        }
        function f(e, t, n, a, i, r, o) {
            return d(t & n | ~t & a, e, t, i, r, o)
        }
        function m(e, t, n, a, i, r, o) {
            return d(t & a | n & ~a, e, t, i, r, o)
        }
        function p(e, t, n, a, i, r, o) {
            return d(t ^ n ^ a, e, t, i, r, o)
        }
        function h(e, t, n, a, i, r, o) {
            return d(n ^ (t | ~a), e, t, i, r, o)
        }
        function b(e, t) {
            var n = (65535 & e) + (65535 & t);
            return (e >> 16) + (t >> 16) + (n >> 16) << 16 | 65535 & n
        }
        const v = {
            hexMD5: i
        };
        t.MD5 = v
    },
    iR1w: function(e, t, n) {
        "use strict";
        n.d(t, "b", (function() {
            return _
        }
        )),
        n.d(t, "a", (function() {
            return j
        }
        )),
        n.d(t, "c", (function() {
            return w
        }
        ));
        var a = n("wx14");
        function i(e, t) {
            e.prototype = Object.create(t.prototype),
            e.prototype.constructor = e,
            e.__proto__ = t
        }
        var r = n("JX7q")
          , o = function(e, t) {
            return e === t
        }
          , c = function(e) {
            var t = arguments.length > 1 && void 0 !== arguments[1] ? arguments[1] : o
              , n = void 0
              , a = []
              , i = void 0
              , r = !1
              , c = function(e, n) {
                return t(e, a[n])
            }
              , l = function() {
                for (var t = arguments.length, o = Array(t), l = 0; l < t; l++)
                    o[l] = arguments[l];
                return r && n === this && o.length === a.length && o.every(c) ? i : (r = !0,
                n = this,
                a = o,
                i = e.apply(this, o))
            };
            return l
        }
          , l = n("q1tI")
          , s = n("zLVn")
          , u = "object" === typeof performance && "function" === typeof performance.now ? function() {
            return performance.now()
        }
        : function() {
            return Date.now()
        }
        ;
        function d(e) {
            cancelAnimationFrame(e.id)
        }
        function f(e, t) {
            var n = u();
            var a = {
                id: requestAnimationFrame((function i() {
                    u() - n >= t ? e.call(null) : a.id = requestAnimationFrame(i)
                }
                ))
            };
            return a
        }
        var m = 150
          , p = function(e, t) {
            return e
        };
        function h(e) {
            var t, n, o = e.getItemOffset, s = e.getEstimatedTotalSize, u = e.getItemSize, h = e.getOffsetForIndexAndAlignment, v = e.getStartIndexForOffset, g = e.getStopIndexForStartIndex, y = e.initInstanceProps, O = e.shouldResetStyleCacheOnItemSizeChange, _ = e.validateProps;
            return n = t = function(e) {
                function t(t) {
                    var n;
                    return (n = e.call(this, t) || this)._instanceProps = y(n.props, Object(r.a)(Object(r.a)(n))),
                    n._outerRef = void 0,
                    n._resetIsScrollingTimeoutId = null,
                    n.state = {
                        isScrolling: !1,
                        scrollDirection: "forward",
                        scrollOffset: "number" === typeof n.props.initialScrollOffset ? n.props.initialScrollOffset : 0,
                        scrollUpdateWasRequested: !1
                    },
                    n._callOnItemsRendered = void 0,
                    n._callOnItemsRendered = c((function(e, t, a, i) {
                        return n.props.onItemsRendered({
                            overscanStartIndex: e,
                            overscanStopIndex: t,
                            visibleStartIndex: a,
                            visibleStopIndex: i
                        })
                    }
                    )),
                    n._callOnScroll = void 0,
                    n._callOnScroll = c((function(e, t, a) {
                        return n.props.onScroll({
                            scrollDirection: e,
                            scrollOffset: t,
                            scrollUpdateWasRequested: a
                        })
                    }
                    )),
                    n._getItemStyle = void 0,
                    n._getItemStyle = function(e) {
                        var t, a = n.props, i = a.direction, r = a.itemSize, c = n._getItemStyleCache(O && r, O && i);
                        if (c.hasOwnProperty(e))
                            t = c[e];
                        else {
                            var l = o(n.props, e, n._instanceProps)
                              , s = u(n.props, e, n._instanceProps);
                            c[e] = t = {
                                position: "absolute",
                                left: "horizontal" === i ? l : 0,
                                top: "vertical" === i ? l : 0,
                                height: "vertical" === i ? s : "100%",
                                width: "horizontal" === i ? s : "100%"
                            }
                        }
                        return t
                    }
                    ,
                    n._getItemStyleCache = void 0,
                    n._getItemStyleCache = c((function(e, t) {
                        return {}
                    }
                    )),
                    n._onScrollHorizontal = function(e) {
                        var t = e.currentTarget.scrollLeft;
                        n.setState((function(e) {
                            return e.scrollOffset === t ? null : {
                                isScrolling: !0,
                                scrollDirection: e.scrollOffset < t ? "forward" : "backward",
                                scrollOffset: t,
                                scrollUpdateWasRequested: !1
                            }
                        }
                        ), n._resetIsScrollingDebounced)
                    }
                    ,
                    n._onScrollVertical = function(e) {
                        var t = e.currentTarget.scrollTop;
                        n.setState((function(e) {
                            return e.scrollOffset === t ? null : {
                                isScrolling: !0,
                                scrollDirection: e.scrollOffset < t ? "forward" : "backward",
                                scrollOffset: t,
                                scrollUpdateWasRequested: !1
                            }
                        }
                        ), n._resetIsScrollingDebounced)
                    }
                    ,
                    n._outerRefSetter = function(e) {
                        var t = n.props.outerRef;
                        n._outerRef = e,
                        "function" === typeof t ? t(e) : null != t && "object" === typeof t && t.hasOwnProperty("current") && (t.current = e)
                    }
                    ,
                    n._resetIsScrollingDebounced = function() {
                        null !== n._resetIsScrollingTimeoutId && d(n._resetIsScrollingTimeoutId),
                        n._resetIsScrollingTimeoutId = f(n._resetIsScrolling, m)
                    }
                    ,
                    n._resetIsScrolling = function() {
                        n._resetIsScrollingTimeoutId = null,
                        n.setState({
                            isScrolling: !1
                        }, (function() {
                            n._getItemStyleCache(-1, null)
                        }
                        ))
                    }
                    ,
                    n
                }
                i(t, e),
                t.getDerivedStateFromProps = function(e, t) {
                    return b(e),
                    _(e),
                    null
                }
                ;
                var n = t.prototype;
                return n.scrollTo = function(e) {
                    this.setState((function(t) {
                        return {
                            scrollDirection: t.scrollOffset < e ? "forward" : "backward",
                            scrollOffset: e,
                            scrollUpdateWasRequested: !0
                        }
                    }
                    ), this._resetIsScrollingDebounced)
                }
                ,
                n.scrollToItem = function(e, t) {
                    void 0 === t && (t = "auto");
                    var n = this.state.scrollOffset;
                    this.scrollTo(h(this.props, e, t, n, this._instanceProps))
                }
                ,
                n.componentDidMount = function() {
                    console.log("componentDidMount ...");
                    var e = this.props
                      , t = e.initialScrollOffset
                      , n = e.direction;
                    "number" === typeof t && null !== this._outerRef && ("horizontal" === n ? this._outerRef.scrollLeft = t : this._outerRef.scrollTop = t),
                    this._callPropsCallbacks()
                }
                ,
                n.componentDidUpdate = function() {
                    var e = this.props.direction
                      , t = this.state
                      , n = t.scrollOffset;
                    t.scrollUpdateWasRequested && null !== this._outerRef && ("horizontal" === e ? this._outerRef.scrollLeft = n : this._outerRef.scrollTop = n),
                    this._callPropsCallbacks()
                }
                ,
                n.componentWillUnmount = function() {
                    null !== this._resetIsScrollingTimeoutId && d(this._resetIsScrollingTimeoutId)
                }
                ,
                n.render = function() {
                    var e = this.props
                      , t = e.children
                      , n = e.className
                      , i = e.direction
                      , r = e.height
                      , o = e.innerRef
                      , c = e.innerElementType
                      , u = e.innerTagName
                      , d = e.itemCount
                      , f = e.itemData
                      , m = e.itemKey
                      , h = void 0 === m ? p : m
                      , b = e.outerElementType
                      , v = e.outerTagName
                      , g = e.style
                      , y = e.useIsScrolling
                      , O = e.width
                      , _ = this.state.isScrolling
                      , j = "vertical" === i ? this._onScrollVertical : this._onScrollHorizontal
                      , I = this._getRangeToRender()
                      , w = I[0]
                      , T = I[1]
                      , S = [];
                    if (d > 0)
                        for (var E = w; E <= T; E++)
                            S.push(Object(l.createElement)(t, {
                                data: f,
                                key: h(E, f),
                                index: E,
                                isScrolling: y ? _ : void 0,
                                style: this._getItemStyle(E)
                            }));
                    var k = s(this.props, this._instanceProps);
                    return Object(l.createElement)(b || v || "div", {
                        className: n,
                        onScroll: j,
                        ref: this._outerRefSetter,
                        style: Object(a.a)({
                            position: "relative",
                            height: r,
                            width: O,
                            overflow: "auto",
                            WebkitOverflowScrolling: "touch",
                            willChange: "transform"
                        }, g)
                    }, Object(l.createElement)(c || u || "div", {
                        children: S,
                        ref: o,
                        style: {
                            height: "horizontal" === i ? "100%" : k,
                            pointerEvents: _ ? "none" : "",
                            width: "horizontal" === i ? k : "100%"
                        }
                    }))
                }
                ,
                n._callPropsCallbacks = function() {
                    if ("function" === typeof this.props.onItemsRendered && this.props.itemCount > 0) {
                        var e = this._getRangeToRender()
                          , t = e[0]
                          , n = e[1]
                          , a = e[2]
                          , i = e[3];
                        this._callOnItemsRendered(t, n, a, i)
                    }
                    if ("function" === typeof this.props.onScroll) {
                        var r = this.state
                          , o = r.scrollDirection
                          , c = r.scrollOffset
                          , l = r.scrollUpdateWasRequested;
                        this._callOnScroll(o, c, l)
                    }
                }
                ,
                n._getRangeToRender = function() {
                    var e = this.props
                      , t = e.itemCount
                      , n = e.overscanCount
                      , a = this.state
                      , i = a.isScrolling
                      , r = a.scrollDirection
                      , o = a.scrollOffset;
                    if (0 === t)
                        return [0, 0, 0, 0];
                    var c = v(this.props, o, this._instanceProps)
                      , l = g(this.props, c, o, this._instanceProps)
                      , s = i && "backward" !== r ? 1 : Math.max(1, n)
                      , u = i && "forward" !== r ? 1 : Math.max(1, n);
                    return [Math.max(0, c - s), Math.max(0, Math.min(t - 1, l + u)), c, l]
                }
                ,
                t
            }(l.PureComponent),
            t.defaultProps = {
                direction: "vertical",
                itemData: void 0,
                overscanCount: 2,
                useIsScrolling: !1
            },
            n
        }
        var b = function(e) {
            e.children,
            e.direction,
            e.height,
            e.innerTagName,
            e.outerTagName,
            e.width
        }
          , v = function(e, t, n) {
            var a = e.itemSize
              , i = n.itemMetadataMap
              , r = n.lastMeasuredIndex;
            if (t > r) {
                var o = 0;
                if (r >= 0) {
                    var c = i[r];
                    o = c.offset + c.size
                }
                for (var l = r + 1; l <= t; l++) {
                    var s = a(l);
                    i[l] = {
                        offset: o,
                        size: s
                    },
                    o += s
                }
                n.lastMeasuredIndex = t
            }
            return i[t]
        }
          , g = function(e, t, n, a, i) {
            for (; a <= n; ) {
                var r = a + Math.floor((n - a) / 2)
                  , o = v(e, r, t).offset;
                if (o === i)
                    return r;
                o < i ? a = r + 1 : o > i && (n = r - 1)
            }
            return a > 0 ? a - 1 : 0
        }
          , y = function(e, t, n, a) {
            for (var i = e.itemCount, r = 1; n < i && v(e, n, t).offset < a; )
                n += r,
                r *= 2;
            return g(e, t, Math.min(n, i - 1), Math.floor(n / 2), a)
        }
          , O = function(e, t) {
            var n = e.itemCount
              , a = t.itemMetadataMap
              , i = t.estimatedItemSize
              , r = t.lastMeasuredIndex
              , o = 0;
            if (r >= n && (r = n - 1),
            r >= 0) {
                var c = a[r];
                o = c.offset + c.size
            }
            return o + (n - r - 1) * i
        }
          , _ = h({
            getItemOffset: function(e, t, n) {
                return v(e, t, n).offset
            },
            getItemSize: function(e, t, n) {
                return n.itemMetadataMap[t].size
            },
            getEstimatedTotalSize: O,
            getOffsetForIndexAndAlignment: function(e, t, n, a, i) {
                var r = e.direction
                  , o = e.height
                  , c = e.width
                  , l = "horizontal" === r ? c : o
                  , s = v(e, t, i)
                  , u = O(e, i)
                  , d = Math.max(0, Math.min(u - l, s.offset))
                  , f = Math.max(0, s.offset - l + s.size);
                switch (n) {
                case "start":
                    return d;
                case "end":
                    return f;
                case "center":
                    return Math.round(f + (d - f) / 2);
                case "auto":
                default:
                    return a >= f && a <= d ? a : a - f < d - a ? f : d
                }
            },
            getStartIndexForOffset: function(e, t, n) {
                return function(e, t, n) {
                    var a = t.itemMetadataMap
                      , i = t.lastMeasuredIndex;
                    return (i > 0 ? a[i].offset : 0) >= n ? g(e, t, i, 0, n) : y(e, t, Math.max(0, i), n)
                }(e, n, t)
            },
            getStopIndexForStartIndex: function(e, t, n, a) {
                for (var i = e.direction, r = e.height, o = e.itemCount, c = e.width, l = "horizontal" === i ? c : r, s = v(e, t, a), u = n + l, d = s.offset + s.size, f = t; f < o - 1 && d < u; )
                    f++,
                    d += v(e, f, a).size;
                return f
            },
            initInstanceProps: function(e, t) {
                var n = {
                    itemMetadataMap: {},
                    estimatedItemSize: e.estimatedItemSize || 50,
                    lastMeasuredIndex: -1
                };
                return t.resetAfterIndex = function(e, a) {
                    void 0 === a && (a = !0),
                    n.lastMeasuredIndex = Math.min(n.lastMeasuredIndex, e - 1),
                    t._getItemStyleCache(-1),
                    a && t.forceUpdate()
                }
                ,
                n
            },
            shouldResetStyleCacheOnItemSizeChange: !1,
            validateProps: function(e) {
                e.itemSize
            }
        })
          , j = h({
            getItemOffset: function(e, t) {
                var n = e.itemSize;
                e.size;
                return t * n
            },
            getItemSize: function(e, t) {
                var n = e.itemSize;
                e.size;
                return n
            },
            getEstimatedTotalSize: function(e) {
                var t = e.itemCount;
                return e.itemSize * t
            },
            getOffsetForIndexAndAlignment: function(e, t, n, a) {
                var i = e.direction
                  , r = e.height
                  , o = e.itemCount
                  , c = e.itemSize
                  , l = e.width
                  , s = "horizontal" === i ? l : r
                  , u = Math.max(0, Math.min(o * c - s, t * c))
                  , d = Math.max(0, t * c - s + c);
                switch (n) {
                case "start":
                    return u;
                case "end":
                    return d;
                case "center":
                    return Math.round(d + (u - d) / 2);
                case "auto":
                default:
                    return a >= d && a <= u ? a : a - d < u - a ? d : u
                }
            },
            getStartIndexForOffset: function(e, t) {
                var n = e.itemCount
                  , a = e.itemSize;
                return Math.max(0, Math.min(n - 1, Math.floor(t / a)))
            },
            getStopIndexForStartIndex: function(e, t, n) {
                var a = e.direction
                  , i = e.height
                  , r = e.itemCount
                  , o = e.itemSize
                  , c = e.width
                  , l = t * o
                  , s = "horizontal" === a ? c : i;
                return Math.max(0, Math.min(r - 1, t + Math.floor((s + (n - l)) / o)))
            },
            initInstanceProps: function(e) {},
            shouldResetStyleCacheOnItemSizeChange: !0,
            validateProps: function(e) {
                e.itemSize
            }
        });
        function I(e, t) {
            for (var n in e)
                if (!(n in t))
                    return !0;
            for (var a in t)
                if (e[a] !== t[a])
                    return !0;
            return !1
        }
        function w(e, t) {
            return !function(e, t) {
                var n = e.style
                  , a = Object(s.a)(e, ["style"])
                  , i = t.style
                  , r = Object(s.a)(t, ["style"]);
                return !I(n, i) && !I(a, r)
            }(this.props, e) || I(this.state, t)
        }
    },
    m4ty: function(e, t, n) {
        "use strict";
        var a = n("8B9+");
        t.a = a.a
    },
    p075: function(e, t, n) {
        "use strict";
        n.d(t, "b", (function() {
            return s
        }
        ));
        var a, i = n("rePB"), r = n("hpN2"), o = n("ep5y");
        function c(e, t) {
            var n = Object.keys(e);
            if (Object.getOwnPropertySymbols) {
                var a = Object.getOwnPropertySymbols(e);
                t && (a = a.filter((function(t) {
                    return Object.getOwnPropertyDescriptor(e, t).enumerable
                }
                ))),
                n.push.apply(n, a)
            }
            return n
        }
        function l(e) {
            for (var t = 1; t < arguments.length; t++) {
                var n = null != arguments[t] ? arguments[t] : {};
                t % 2 ? c(Object(n), !0).forEach((function(t) {
                    Object(i.a)(e, t, n[t])
                }
                )) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(n)) : c(Object(n)).forEach((function(t) {
                    Object.defineProperty(e, t, Object.getOwnPropertyDescriptor(n, t))
                }
                ))
            }
            return e
        }
        var s = "Notification"
          , u = (a = {},
        Object(i.a)(a, o.b, (function(e, t) {
            return l({}, e, {
                configKey: t.configKey,
                replace: t.replace,
                hidden: !1
            })
        }
        )),
        Object(i.a)(a, o.a, (function(e) {
            return l({}, e, {
                hidden: !0
            })
        }
        )),
        a);
        t.a = Object(r.a)({
            hidden: !0,
            configKey: "",
            replace: {}
        }, u)
    },
    qPyV: function(e, t, n) {
        var a = n("ut/Y")
          , i = n("LGYb");
        e.exports = function(e, t) {
            return e && e.length ? i(e, a(t, 2)) : []
        }
    },
    tK7F: function(e, t, n) {
        "use strict";
        n.d(t, "a", (function() {
            return l
        }
        )),
        n.d(t, "b", (function() {
            return r
        }
        )),
        n.d(t, "c", (function() {
            return c
        }
        ));
        var a = n("q1tI")
          , i = n.n(a).a.createElement
          , r = Object(a.createContext)()
          , o = r.Provider
          , c = (r.Consumer,
        function(e) {
            var t = e.children
              , n = Object(a.useState)(null)
              , r = n[0]
              , c = n[1];
            return i(o, {
                value: {
                    pageTitleOverride: r,
                    setPageTitleOverride: c
                }
            }, t)
        }
        )
          , l = function(e) {
            return function(e, t) {
                var n = Object(a.useContext)(r).setPageTitleOverride;
                Object(a.useEffect)((function() {
                    return n(t ? e : null),
                    function() {
                        return n(null)
                    }
                }
                ), [e, t, n])
            }(e.title, e.useOverrideTitle),
            null
        }
    },
    uyJD: function(e, t, n) {
        "use strict";
        var a = n("q1tI")
          , i = n.n(a)
          , r = n("mwIZ")
          , o = n.n(r)
          , c = n("rK06")
          , l = n("8h9a")
          , s = n("Nnso")
          , u = n("p3iw")
          , d = n("nYuY")
          , f = n("Vg22")
          , m = n("YYy2")
          , p = n("H9OJ")
          , h = n("8Wld")
          , b = n("SgFH")
          , v = n("50VT")
          , g = i.a.createElement;
        t.a = Object(f.connect)(null, (function(e) {
            return {
                clickOnTile: function(t, n, a, i, r, o, c, l) {
                    e(Object(d.g)(t, n, a, i, r, o, c, l))
                },
                clickOnActionMenuItem: function(t, n, a, i, r) {
                    e(Object(d.f)(t, n, a, i, r))
                }
            }
        }
        ))((function(e) {
            var t = e.asset
              , n = e.index
              , i = e.requireDetailPage
              , r = e.highlight
              , d = e.queryFromRecent
              , f = e.actionFrom
              , y = e.actionTo
              , O = e.collectionMetricsParams
              , _ = e.userInteractionMetricsParams
              , j = e.upsellPackageId
              , I = function() {
                e.clickOnTile(p.j(t), n, O, d, f, y, j, t.selectionTrackingId)
            }
              , w = function(a) {
                var i;
                switch (a) {
                case h.a.DETAIL:
                    i = b.SEARCH_ACTION_PAGE_TYPE.DETAILS;
                    break;
                case h.a.MY_STUFF:
                    i = b.SEARCH_ACTION_PAGE_TYPE.SEARCH;
                    break;
                case h.a.RECORD:
                    i = b.SEARCH_ACTION_PAGE_TYPE.OVERLAY;
                    break;
                default:
                    return
                }
                e.clickOnActionMenuItem(p.j(t), n, O, i, j)
            };
            return g(a.Fragment, null, g(v.a, {
                name: "updatedGenericTile"
            }, g(s.a, {
                asset: t,
                highlight: r,
                stateKey: u.d,
                onClick: I,
                pageContext: c.g,
                requireDetailsPage: i,
                metricsParams: _,
                tealiumTileData: Object(m.h)(p.o(t)),
                onActionMenuItemClick: w,
                index: n,
                collectionIndex: o()(O, "collectionIndex")
            })), g(v.a, {
                name: "updatedGenericTile",
                renderWhenOff: !0
            }, g(l.a, {
                asset: t,
                highlight: r,
                stateKey: u.d,
                onClick: I,
                pageContext: c.g,
                requireDetailsPage: i,
                metricsParams: _,
                tealiumTileData: Object(m.h)(p.o(t)),
                onActionMenuItemClick: w,
                index: n,
                collectionIndex: o()(O, "collectionIndex")
            })))
        }
        ))
    },
    vlRD: function(e, t, n) {
        (window.__NEXT_P = window.__NEXT_P || []).push(["/", function() {
            return n("RNiq")
        }
        ])
    },
    y81G: function(e, t, n) {
        "use strict";
        t.__esModule = !0,
        t.CBC7 = void 0;
        t.CBC7 = {
            encrypt: function(e, t, n) {
                for (var a = 4 * e._blocksize, i = a - t.length % a, r = 0; r < i; r++)
                    t.push(i);
                for (var o = 0; o < t.length; o += a) {
                    if (0 == o)
                        for (r = 0; r < a; r++)
                            t[r] ^= n[r];
                    else
                        for (r = 0; r < a; r++)
                            t[o + r] ^= t[o + r - a];
                    e._encryptblock(t, o)
                }
            },
            decrypt: function(e, t, n, a) {
                for (var i = 4 * e._blocksize, r = 0; r < t.length; r += i) {
                    var o = t.slice(r, r + i);
                    if (e._decryptblock(t, r),
                    0 == r)
                        for (var c = 0; c < i; c++)
                            t[c] ^= n[c];
                    else
                        for (c = 0; c < i; c++)
                            t[r + c] ^= l[c];
                    var l = o
                }
                if (a) {
                    var s = t[t.length - 1];
                    for (c = 0; c < s; c++)
                        t.pop()
                }
            }
        }
    },
    yV38: function(e, t, n) {
        "use strict";
        (function(e) {
            n.d(t, "a", (function() {
                return g
            }
            ));
            var a = n("q1tI")
              , i = n.n(a)
              , r = n("uyJD")
              , o = n("8yiw")
              , c = n("BN2M")
              , l = n("hpcX")
              , s = n("GDm6")
              , u = n("ZoaF")
              , d = n("azIb")
              , f = n("h0o4")
              , m = n("H9OJ")
              , p = n("VGA2")
              , h = n("4a2M")
              , b = (n("yvAb"),
            i.a.createElement)
              , v = 500;
            function g(t) {
                var n = t.collection
                  , a = t.requireDetailPage
                  , i = t.collectionMetricsParams
                  , g = t.metricsInfo
                  , y = n.items
                  , O = n.name
                  , _ = function(t) {
                    var n = t.item
                      , a = t.collectionMetricsParams
                      , i = {
                        collectionMetricsParams: a
                    }
                      , r = Object(s.d)([n]);
                    m.t(n) && (i.upsellImpression = Object(s.e)([n])),
                    a.pageName === s.a.SEARCH_PAGE_NAME.SEARCH_RECO ? i.recoImpression = r : i.impression = r,
                    i.selectionTrackingId = n.selectionTrackingId,
                    s.g.trackImpression(e.store.getState, i)
                }
                  , j = function(e, t) {
                    var n, o, l = {
                        item: e,
                        collectionMetricsParams: i
                    }, d = {
                        collectionId: s.a.USERINTERACTION_COLLECTION_ID,
                        collectionIndex: i.collectionIndex,
                        position: t,
                        collectionSource: c.c.COLLECTION_SOURCE.SEARCH
                    };
                    if (n = i.pageName === s.a.SEARCH_PAGE_NAME.SEARCH_RECO ? s.a.SEARCH_ACTION_PAGE_TYPE.SEARCH_RECO : s.a.SEARCH_ACTION_PAGE_TYPE.SEARCH,
                    m.t(e))
                        o = s.a.SEARCH_ACTION_PAGE_TYPE.UPSELL_LEDGER;
                    else if (a)
                        o = s.a.SEARCH_ACTION_PAGE_TYPE.DETAILS;
                    else {
                        o = m.a(e) ? s.a.SEARCH_ACTION_PAGE_TYPE.PLAYBACK : s.a.SEARCH_ACTION_PAGE_TYPE.DETAILS
                    }
                    var f = b("div", {
                        className: "SearchResultTray__item"
                    }, b(u.a, {
                        metricsParams: l,
                        waitFor: v,
                        threshold: .75,
                        onEnter: _
                    }, b(r.a, {
                        asset: e,
                        index: t,
                        requireDetailPage: a,
                        collectionMetricsParams: i,
                        userInteractionMetricsParams: d,
                        actionFrom: n,
                        actionTo: o,
                        upsellPackageId: m.r(e)
                    })));
                    return {
                        id: m.g(e),
                        content: f
                    }
                };
                return b("div", {
                    className: "SearchResultTray"
                }, b(l.a, {
                    collection: n,
                    pageType: d.j.SEARCH,
                    metricsInfo: g
                }), b("div", {
                    className: "SearchResultTray__slider"
                }, b(h.a, null, (function(e) {
                    return b(o.a, {
                        items: y.map(j),
                        itemsPerPage: Object(p.a)(e, f.a),
                        hasGenericTiles: !0,
                        collectionTitle: O,
                        isCircular: !0
                    })
                }
                ))))
            }
        }
        ).call(this, n("3r9c"))
    }
}, [["vlRD", 1, 2, 8, 0, 3, 4, 5, 6, 7, 9]]]);
