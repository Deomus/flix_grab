! function e(t, i, n) {
    function r(s, a) {
        if (!i[s]) {
            if (!t[s]) {
                var l = "function" == typeof require && require;
                if (!a && l) return l(s, !0);
                if (o) return o(s, !0);
                var c = new Error("Cannot find module '" + s + "'");
                throw (c.code = "MODULE_NOT_FOUND", c)
            }
            var u = i[s] = {
                exports: {}
            };
            t[s][0].call(u.exports, function(e) {
                var i = t[s][1][e];
                return r(i ? i : e)
            }, u, u.exports, e, t, i, n)
        }
        return i[s].exports
    }
    for (var o = "function" == typeof require && require, s = 0; s < n.length; s++) r(n[s]);
    return r
}({
    1: [function(e, t, i) {
        t.exports = {
            genericError: {
                title: "Video Error",
                description: 'We\'re experiencing a problem playing this video. Please try again. For further assistance, <a href="//{{contactUsDigitalUrl}}" target="_blank" rel="noopener noreferrer">contact Amazon Customer Service</a>.'
            },
            genericUnsupportedBrowser: {
                title: "Unsupported Browser",
                description: "This web browser isn't compatible with Prime Video. Please use one of the following web browsers:<ul><li>Chrome (latest version)</li><li>Firefox (latest version)</li><li>Microsoft Edge (version 15 or newer)</li><li>Internet Explorer (version 10 or newer)</li><li>Apple Safari (latest version)</li><li>Opera (latest version)</li></ul>",
                footer: 'For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            linuxUnsupportedBrowser: {
                title: "Unsupported Browser",
                description: "This web browser isn't compatible with Prime Video. Please use one of the following web browsers:<ul><li>Chrome (latest version)</li></ul>",
                footer: 'For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            macUnsupportedBrowser: {
                title: "Unsupported Browser",
                description: "This web browser isn't compatible with Prime Video. Please use one of the following web browsers:<ul><li>Chrome (latest version)</li><li>Firefox (latest version)</li><li>Safari (latest version)</li><li>Opera (latest version)</li></ul>",
                footer: 'For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            macLessThan10Point9UnsupportedBrowser: {
                title: "Unsupported Browser",
                description: "This web browser isn't compatible with Prime Video. Please use one of the following web browsers:<ul><li>Safari</li></ul>",
                footer: 'For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            chromeIos10NotSupported: {
                title: "Unsupported Browser",
                description: "Chrome on iOS 10 is currently not supported. Please try again using Safari."
            },
            windowsUnsupportedBrowser: {
                title: "Unsupported Browser",
                description: "This web browser isn't compatible with Prime Video. Please use one of the following web browsers:<ul><li>Chrome (latest version)</li><li>Firefox (latest version)</li><li>Microsoft Edge (version 15 or newer)</li><li>Internet Explorer (version 10 or newer)</li><li>Opera (latest version)</li></ul>",
                footer: 'For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            genericUnsupportedOs: {
                title: "Unsupported Operating System",
                description: 'Prime Video titles can\'t be watched on this device because the operating system isn\'t supported. For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            firefoxOutOfDate: {
                title: "Mozilla Firefox Update Required",
                description: "A later version of Mozilla Firefox is required for Prime Video. To update, click the menu button (three horizontal stripes) on the web browser toolbar, click <b>About Firefox</b>, and then click <b>Restart to update Firefox</b>.",
                footer: 'For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            chromeOutOfDate: {
                title: "Google Chrome Update Required",
                description: "A later version of Google Chrome is required for Prime Video. To update, click the Chrome menu (three horizontal stripes) on the browser toolbar, click <b>About Google Chrome</b>, and then click <b>Relaunch</b>.",
                footer: 'For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            operaOutOfDate: {
                title: "Opera Update Required",
                description: "A later version of Opera is required for Prime Video. To check for updates, click the <b>Opera</b> menu, click <b>About Opera</b>, and then click <b>Relaunch Now</b>.",
                footer: 'For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            firefoxMissingCdm: {
                title: "Digital Rights Error",
                description: "Your web browser is missing a digital rights component.<br/><br/>In your web browser's address bar, type <b>about:addons</b>, press Enter, and then on the left, click <b>Plugins</b>. For the <b>Widevine Content Decryption Module</b>, select <b>Always Activate</b>, and then refresh the video's page and try watching the video.<br/><br/>If the video doesn't play, go back to <b>Plugins</b>, click the gear icon at the top of the page, and then click <b>Check for Updates</b>. When the plugins have updated, refresh the video's page, and then try watching the video.<br/><br/>",
                footer: 'For further assistance, please contact Amazon Customer Service at <a href="//{{videoHelpUrl}}" target="_blank" rel="noopener noreferrer">{{videoHelpUrl}}</a> and refer to error 7235.'
            },
            chromeMissingCdm: {
                title: "Digital Rights Error",
                description: "Your web browser is missing a digital rights component. Go to <b>chrome://components</b> and under <b>WidevineCdm</b>, click <b>Check for update</b>.",
                footer: 'For further assistance, please contact Amazon Customer Service at <a href="//{{videoHelpUrl}}" target="_blank" rel="noopener noreferrer">{{videoHelpUrl}}</a> and refer to error 7235.'
            },
            androidMissingCdm: {
                title: "Protected Content Error",
                description: "To watch protected content, in the top right of Chrome, click the three dots icon, and then tap <b>Settings</b> > <b>Site settings</b> > <b>Protected content</b>. Under <b>Blocked</b>, tap <b>Amazon.com</b> > <b>Clear and Reset</b>. Refresh or reload the video's detail page, and then tap <b>Watch Now</b>.",
                footer: 'For further assistance, please contact Amazon Customer Service at <a href="//{{videoHelpUrl}}" target="_blank" rel="noopener noreferrer">{{videoHelpUrl}}</a> and refer to error 7218.'
            },
            operaMissingCdm: {
                title: "Digital Rights Error",
                description: "A later version of Opera is required for Prime Video. To check for updates, click the <b>Opera</b> menu, click <b>About Opera</b>, and then click <b>Relaunch Now</b>.",
                footer: 'For more information, please see <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Prime Video system requirements</a>.'
            },
            pinLookupError: {
                title: "PIN Lookup Problem",
                description: 'We\'re having a problem checking your account\'s PIN restrictions which is preventing your video from playing. Please try again later, or you can <a href="//{{avSettingsUrl}}" target="_blank" rel="noopener noreferrer">turn off PIN restrictions</a> to watch your video immediately.'
            },
            pinLookupErrorFsk18: {
                title: "PIN Lookup Problem",
                description: "We're having a problem checking your account's PIN restrictions which is preventing your video from playing. Please try again later. If this problem continues, contact Amazon Customer Service and refer to error 5016."
            },
            pinValidationError: {
                title: "PIN Validation Problem",
                description: 'We\'re having a problem validating your PIN. Please try again. If this problem continues, please <a href="//{{contactUsOrdersUrl}}" target="_blank" rel="noopener noreferrer">contact Amazon Customer Service</a>and refer to error 5014.'
            },
            pinModal: {
                cancelButton: "Cancel",
                submitButton: "Continue",
                title: "Viewing Restrictions Enabled",
                description: "Please enter your Prime Video PIN:",
                notes: '<a href="//{{resetPinUrl}}" target="_blank" rel="noopener noreferrer">Reset your PIN</a> or <a href="//{{avSettingsUrl}}" target="_blank" rel="noopener noreferrer">change viewing restrictions</a>',
                incorrectPin: 'We don\'t recognize the PIN you entered. Please try again or <a href="//{{resetPinUrl}}" target="_blank" rel="noopener noreferrer">reset your PIN.</a>'
            },
            pinSetupRequired: {
                title: "Viewing Restrictions Enabled",
                description: 'To view this video, a PIN is required. To update your PIN, go to the Prime Video <a href="//{{setupPinUrl}}" target="_blank" rel="noopener noreferrer">Settings page</a>.'
            },
            pinAgeVerificationRequired: {
                title: "Viewing Restrictions Enabled",
                description: "To view this video, age verification is required."
            },
            silverlightInstall: {
                title: "Microsoft Silverlight is not installed",
                description: "Install the free Silverlight plug-in; it will just take a minute. Please refresh this page when the installation completes.<p>If you have issues after installing, restart your browser and try again. If the issue persists, try using our new HTML5 video player on Chrome, Internet Explorer 11 on Windows 8.1 or later, Microsoft Edge or Opera.</p>",
                button: '<a href="http://go2.microsoft.com/fwlink/?LinkID=149156" target="_blank" rel="noopener noreferrer">Install Silverlight</a>',
                footer: 'Already installed Silverlight? <a href="//{{videoIssuesUrl}}" target="_blank" rel="noopener noreferrer">Get Silverlight plug-in help</a>.'
            },
            silverlightInstallSafari: {
                title: "Microsoft Silverlight not installed or enabled",
                description: '<b>To install:</b> Install for free by tapping <b><a href="http://go2.microsoft.com/fwlink/?LinkID=149156" target="_blank" rel="noopener noreferrer">Install Silverlight</a></b>. Refresh this page when installation completes.<br/><br/><b>To enable:</b> In Safari preferences, <b>Security</b> > <b>Plugins</b> > <b>Silverlight</b>, select <b>Enable</b> and then refresh this page.<br/><br/>If it still doesn\'t work, we recommend using the HTML5 video player for better performance and more features. The HTML5 video player is available in Chrome, Internet Explorer 11 on Windows 8.1 and later, Edge or Opera.',
                footer: 'Already installed Silverlight? <a href="//{{videoIssuesUrl}}" target="_blank" rel="noopener noreferrer">Get Silverlight plug-in help</a>.'
            },
            silverlightUpgrade: {
                title: "Your Microsoft Silverlight is out of date",
                description: "Upgrade your Silverlight plug-in&mdash;it will just take a minute.",
                button: '<a href="http://go2.microsoft.com/fwlink/?LinkID=149156" target="_blank" rel="noopener noreferrer">Install Silverlight</a>',
                footer: 'If you\'re experiencing problems with Silverlight, please see our <a href="//{{videoIssuesUrl}}" target="_blank" rel="noopener noreferrer">troubleshooting tips</a>.'
            },
            silverlightActiveX: {
                title: "ActiveX Not Enabled",
                description: "To watch your video in Internet Explorer, ActiveX controls must be enabled.",
                steps: ["In Internet Explorer, press the <b>Alt</b> key, and then on <b>Tools</b> menu, click <b>Internet options</b>.", "On the <b>Security</b> tab, click <b>Custom level</b>.", "Scroll down to the <b>ActiveX controls and plug-ins</b> section.", "Click <b>Enable</b> for <b>Run ActiveX controls and plug-ins</b> and <b>Script ActiveX controls marked safe for scripting</b>.", "Click <b>OK</b>, and then restart Internet Explorer."],
                footer: 'For further assistance, <a href="//{{contactUsDigitalUrl}}" target="_blank" rel="noopener noreferrer">contact Amazon Customer Service</a>.'
            },
            silverlightCorruptionWindows: {
                title: "Silverlight Problem",
                description: "There's a problem with the installation of Microsoft Silverlight on your computer. Microsoft provides a tool that can often fix this problem.",
                fixItToolSteps: ['<a href="https://download.microsoft.com/download/6/C/9/6C970550-32AB-4235-9CDD-7FC9DD848BBB/silverlight.diagcab" target="_blank" rel="noopener noreferrer">Run the Fix It tool</a>', "After the tool has completed, refresh this page."],
                manualRepairSteps: {
                    preLinkText: "If the Microsoft Fix It tool fails to fix the problem, try the ",
                    linkText: "Manual Repair Steps",
                    postLinkText: ".",
                    steps: ["Close all web browser windows, including this one.", "Click the Start button, type <b>Uninstall a program</b>, and then press <b>Enter</b>.<br/>Windows 8: Swipe in from the right edge of the screen, then tap <b>Settings</b>. You can also press Windows key+I. Select <b>Control Panel</b>, and then select <b>Uninstall a program</b>.", "Click <b>Microsoft Silverlight</b>, and then click <b>Uninstall</b>.", "Return to this page to install Silverlight."]
                },
                footer: 'For further assistance, <a href="//{{contactUsDigitalUrl}}" target="_blank" rel="noopener noreferrer">contact Amazon Customer Service</a>.'
            },
            silverlightCorruptionMac: {
                title: "Silverlight Problem",
                description: "There's a problem with the installation of Microsoft Silverlight on your computer. Often, this can be fixed by uninstalling, and then reinstalling Silverlight.",
                steps: ["Close all web browser windows, including this one.", "Click on the Desktop, or open Finder.", 'Click <span class="highlight">Go > Computer</span>.', 'Double-click <span class="highlight">Macintosh HD > Library > Internet Plugins</span>.', 'Drag <span class="highlight">Silverlight.plugin</span> to the Trash.', "Return to this page to install Silverlight."],
                footer: 'For further assistance, <a href="//{{contactUsDigitalUrl}}" target="_blank" rel="noopener noreferrer">contact Amazon Customer Service</a>.'
            },
            notification: {
                showMore: "Show More",
                showLess: "Show Less",
                dismiss: "Dismiss",
                dontShowAgain: "Don't show again"
            },
            silverlightBlocked: {
                message: "The Silverlight plugin isn't loading and might be blocked by your browser.",
                chromeUnblock: 'To enable Silverlight, click this icon <span class="chromePluginBlockerIcon"></span> inside the right edge of the address bar, select <b>"Always allow plug-ins on www.amazon.com"</b>, and then click <b>"Done"</b>.',
                troubleshoot: 'Please see the <a href="//{{videoIssuesUrl}}" target="_blank" rel="noopener noreferrer">Amazon website</a> for additional troubleshooting help. Error: 7303.'
            },
            silverlightBlockedSafari10: 'Silverlight is disabled by default in this version of Safari. To re-enable and watch Prime Video, click <b>Use Silverlight</b>.<br/><br/>\n                            If Silverlight isn\'t installed on your Mac, click <b>Missing Plugin</b>, and then click <b>More Info</b> to go to the Silverlight installation page.<br/><br/>\n                            For additional troubleshooting help, go to the <a href="//{{videoIssuesUrl}}" target="_blank" rel="noopener noreferrer">Amazon website</a>. Error: 7304`',
            silverlightForcedNarratives: "The Silverlight player doesn't support forced narratives. Subtitles will be automatically enabled. Forced narratives translate dialogue or text that doesn't match the primary language of the video. For example, a scene with sailors speaking Russian, but the video is for English audiences.",
            updateChrome: {
                title: "Chrome Update Available",
                message: "An updated version of Chrome is available which enables an improved HTML5 video web player.",
                instructions: "To update, click the Chrome menu (three horizontal stripes) on the browser toolbar, click <b>About Google Chrome</b>, and then click <b>Relaunch</b>."
            },
            safariBetterWatchExperience: "For the best streaming experience, we recommend using Chrome or Edge.",
            switchToHtml5: "For more features and less buffering, we recommend using the HTML5 video player on Google Chrome, Internet Explorer 11 on Windows 8.1 or later, Microsoft Edge, Firefox 60 or later, or Opera.",
            updateFirefox: 'Update to <a href="https://support.mozilla.org/kb/update-firefox-latest-version" target="_blank" rel="noopener noreferrer">Firefox 60</a> or later to access our HTML5 video player with better performance and features like X-Ray, Auto-play, Live streaming, and more.',
            SilverlightUnsupportedFeatureError: {
                title: "Video Unavailable",
                description: 'This video requires an HTML5 video player which provides a better viewing experience and more features. <a href="//{{systemRequirementsUrl}}" target="_blank" rel="noopener noreferrer">Learn more</a> about web browsers that support the HTML5 player and the benefits.'
            }
        }
    }, {}],
    2: [function(e, t, i) {
        "use strict";

        function n(e, t, i) {
            i = a.isNullOrUndefined(i) ? "" : "?ref=" + i, l.chain(e).keys().forEach(function(t) {
                e[t] += i
            });
            for (var n = 0, r = c; n < r.length; n++) {
                var o = r[n];
                e[o] = u[t] + i
            }
            return e
        }
        var r = e(80),
            o = r.AmazonFQDN,
            s = r.MarketplaceID,
            a = e(93),
            l = e(294),
            c = [s.ROE_EU, s.ROW_EU, s.ROW_FE, s.ROW_NA, o.PRIME_VIDEO],
            u = {
                contactUs: "/help/contact-us",
                devicesAndFeatures: "/help?nodeId=202064920",
                pin: "/settings/parental-controls",
                systemRequirements: "/help?nodeId=202064950",
                videoHelp: "/help",
                videoIssues: "/help?nodeId=202065190"
            },
            p = {
                CONTACT_US_DIGITAL: n({
                    "*": "/gp/help/contact-us/digital-video.html"
                }, "contactUs", "aiv_ply_sl_gen_err_help"),
                CONTACT_US_ORDERS: n({
                    "*": "/gp/help/customer/contact-us"
                }, "contactUs"),
                SYSTEM_REQUIREMENTS: n({
                    "*": "/gp/help/customer/display.html?nodeId=201422810"
                }, "systemRequirements"),
                VIDEO_ISSUES: n({
                    "*": "/gp/help/customer/display.html?nodeId=201460940"
                }, "videoIssues"),
                AV_SETTINGS: n({
                    "*": "/pin"
                }, "pin", "dp_pct_pin_chg"),
                RESET_PIN: n({
                    "*": "/pin"
                }, "pin", "dp_pct_pin_forg"),
                VIDEO_HELP: n({
                    "*": "/videohelp"
                }, "videoHelp"),
                SETUP_PIN: n({
                    "*": "/pin"
                }, "pin"),
                DEVICES_AND_FEATURES: n({
                    "*": "/gp/help/customer/display.html?nodeId=201460880"
                }, "devicesAndFeatures")
            };
        t.exports = p
    }, {
        294: 294,
        80: 80,
        93: 93
    }],
    3: [function(e, t, i) {
        "use strict";
        var n = this && this.__assign || Object.assign || function(e) {
                for (var t, i = 1, n = arguments.length; i < n; i++) {
                    t = arguments[i];
                    for (var r in t) Object.prototype.hasOwnProperty.call(t, r) && (e[r] = t[r])
                }
                return e
            },
            r = e(187),
            o = e(80),
            s = o.PartnerServiceEndpoint,
            a = o.StorageKeys,
            l = o.Weblabs,
            c = o.WeblabTreatment,
            u = e(116),
            p = u.BasicStore,
            h = e(231),
            f = h.AbortedEvent,
            d = h.AbortEvent,
            y = h.AsinChangedEvent,
            g = h.CachePriority,
            m = h.CloseEvent,
            v = h.DiagnosticsState,
            E = h.DiagnosticsProtocol,
            _ = h.EndOfContentEvent,
            S = h.ErrorSeverity,
            T = h.Features,
            b = h.InitializedEvent,
            I = h.NotificationCancelEvent,
            w = h.NotificationEvent,
            R = h.PINAcceptedEvent,
            C = h.PINErrorEvent,
            P = h.PINRequiredEvent,
            A = h.PlayerCapabilitiesEvent,
            O = h.PlayingEvent,
            N = h.PreparedEvent,
            x = h.StoppingEvent,
            M = h.VideoType,
            L = e(232),
            k = e(243),
            D = k.Reporter,
            U = e(272),
            F = U.BrowserType,
            H = e(186),
            W = H.Locales,
            V = H.LocalizationStore,
            B = e(82),
            q = e(83),
            j = e(113),
            z = j.AmazonUrlCreator,
            G = j.CookieStorage,
            Y = j.EnumUtils,
            K = j.LocalStorage,
            X = j.SessionStorage,
            Q = j.ResourceAcquirer,
            J = e(126),
            Z = e(93),
            $ = e(284),
            ee = e(281),
            te = e(294),
            ie = e(24),
            ne = e(286),
            re = e(288),
            oe = e(30),
            se = e(34),
            ae = e(36),
            le = e(33),
            ce = e(29),
            ue = e(41),
            pe = e(18),
            he = e(19),
            fe = e(20),
            de = e(21),
            ye = e(22),
            ge = e(32),
            me = e(17),
            ve = e(6),
            Ee = e(25),
            _e = e(2),
            Se = e(16),
            Te = e(1),
            be = ee.of("DelegatingWebPlayer"),
            Ie = $.get("DelegatingWebPlayer"),
            we = "playerBeta",
            Re = function() {
                function e(t, i, n, r, o, s, a, l, c, u, h, f, d, y, g, m, v, E, _, T, I) {
                    void 0 === n && (n = new D), void 0 === r && (r = new oe), void 0 === o && (o = new Q), void 0 === s && (s = new le), void 0 === a && (a = new ae), void 0 === l && (l = new ue), void 0 === c && (c = new Se), void 0 === u && (u = new X), void 0 === h && (h = new G), void 0 === f && (f = te.now), void 0 === d && (d = window.navigator), void 0 === y && (y = window.location), void 0 === g && (g = new ve), void 0 === m && (m = new se), void 0 === v && (v = new K), void 0 === E && (E = W), void 0 === _ && (_ = V), void 0 === T && (T = p), void 0 === I && (I = J.getInstance());
                    var w = this;
                    this.featureConfig = null, this.playerCacheEnabled = !0, this.onPlayerCreated = function(t, i) {
                        try {
                            w.webPlayerFactory = t, w.webPlayer = i, w.attachEventListeners(w.webPlayer), w.tryNotifyPlaybackInitiated(), w.pinDisplayManager.initialize(w.webPlayer, w.eventEmitter, w.uiElement, w.localizationStore, w.amazonUrlCreator, function() {
                                w.onClosePlayerClicked()
                            });
                            var n = ie(!0, w.playerConfig, {
                                domElement: w.playerElement
                            });
                            w.webPlayer.once(b, function() {
                                Z.exists(w.spinnerPanel) && w.spinnerPanel.uninitialize(), w.spinnerPanel = null, w.fsm.transitionTo(me.INITIALIZED), w.eventEmitter.emit(b)
                            }), w.initializeTimer.end(), w.webPlayer.initialize(n)
                        } catch (t) {
                            Ie.error(t), w.abort(e.UNHANDLED_INITIALIZE_ERROR, t.message, S.Fatal)
                        }
                    }, this.abort = function(e, t, i, n) {
                        Z.isNullOrUndefined(w.playerCapabilities) && (w.playerCapabilities = {
                            supportsDrm: !1,
                            supportsNativePlayback: !1,
                            supportsWebPlayback: !1,
                            supportsAVOD: !1,
                            supportsMidStreamMTA: !1,
                            supportsHDMovies: !1,
                            supportsHDTVShows: !1,
                            supportsLive: !1
                        }, w.eventEmitter.emit(A, new A(w.getPlayerCapabilities()))), w.reportFatalError(e, t, i), w.showErrorMessage(n, e), Ie.error("The player has terminated due to a Fatal error: " + e + ": " + t), w.reporter.flush(), w.qosReporter.flush(), w.transitionToAborted()
                    }, this.onNotificationEvent = function(e) {
                        w.notificationManager.addNotification(e, !1)
                    }, this.onNotificationCancelEvent = function(e) {
                        w.notificationManager.cancelNotification(e.getNotification())
                    }, this.fsm = this.createFiniteStateMachine(), this.loaderConfig = t, this.flagManager = c, this.sessionStorage = u, this.cookieStorage = h, this.localStorage = v, this.localesRef = E, this.hasPlaybackInitiated = !1, this.notifyPlaybackInitiatedOnInitialized = !1, this.sortedPlayerFactories = i, this.reporter = n, this.reporter.preInitializeLogging(), this.errorDisplayManager = r, this.resourceAcquirer = o, this.notificationManager = s, this.qosReporter = a, this.spinnerPanel = l, this.now = f, this.navigator = d, this.locationRef = y, this.initializePipeline = g, this.deviceInformation = I, this.eventEmitter = new B.BasicEventEmitter, this.pinDisplayManager = m, this.webPlayer = null, this.webPlayerFactory = null, this.localizationStoreType = _, this.configurationStore = new T
                }
                return (e.prototype.getState = function() {
                    return this.fsm.getCurrentState()
                }, e.prototype.initialize = function(t) {
                    var i = this;
                    if (be(t, "playerConfig").is.an.object(), be(te.isElement(t.domElement), "playerConfig.domElement has to be a dom element").is["true"](), be(t.marketplaceId, "playerConfig.marketplaceId").is.a.string(), be(t.marketplaceId, "playerConfig.marketplaceId").is.not.empty(), !this.fsm.transitionTo(me.INITIALIZING)) throw new Error("Unable to initialize player while in the " + me[this.fsm.getCurrentState()] + " state");
                    this.initializeTimer = r.startTimer(e.INITIALIZE_TIME), this.webPlayerFactory = null;
                    var n = this.sanitizePlayerConfig(t);
                    Z.isNullOrUndefined(n.deviceId) && (n.deviceId = this.createDeviceId(n.customerId));
                    var o = this.flagManager.isActiveFlag(Se.HTML5_DIAGNOSTICS_STORAGE_KEY),
                        a = o ? v.Enabled : v.Disabled,
                        u = L.getParameter(e.PLAYER_DEBUG_QUERY_PARAM);
                    Z.exists(u) && (a = Z.withDefault(v[u], a)), this.diagnosticsSessionConfiguration = this.createDiagnosticsConfig();
                    var p = this.flagManager.isActiveFlag(Se.HTML5_HEVC_TEST_STORAGE_KEY),
                        h = L.getParameter(e.PLAYER_CACHE_QUERY_PARAM);
                    null !== h && (this.playerCacheEnabled = h);
                    var f = this.getStoredOverrides(),
                        d = Z.nestedValue(f, "userWatchSessionId", re.v4());
                    this.configurationStore.setValue({
                        userWatchSessionId: d,
                        primitiveSessionId: d + this.now(),
                        diagnosticsSessionConfiguration: this.diagnosticsSessionConfiguration
                    }).commit(), this.playerConfig = ie(!0, {
                        loaderConfig: this.loaderConfig,
                        partnerServiceEndpoint: s.fromMarketplaceId(n.marketplaceId),
                        locale: W.EN_US,
                        diagnostics: a,
                        enableAvod: !0,
                        enableHEVC: p,
                        sessionId: this.cookieStorage.get(e.SESSION_ID_KEY),
                        tokenClient: null,
                        clientId: null,
                        configurationStore: this.configurationStore
                    }, n);
                    var y = [W.EN_US, this.localesRef.getLocaleFromMarketplaceId(this.playerConfig.marketplaceId), this.localesRef.getClosestLocale(this.playerConfig.locale, W.AvailableLocales), this.localesRef.getStandardizedLocale(this.playerConfig.locale)];
                    this.playerConfig.locale = te.reduce(y, function(e, t) {
                        return Z.withDefault(t, e)
                    });
                    var g = {},
                        m = L.getParameter(we),
                        E = n.playerBeta;
                    if (null != m && (te.isBoolean(m) ? E = m : te.isNumber(m) && (E = 1 === m)), Z.exists(f.weblabs)) {
                        var _ = te.map(f.weblabs.split("&"), function(e) {
                            return e.split(":")
                        });
                        te.each(_, function(e) {
                            var t = e[1];
                            te.isString(t) && (g[e[0]] = "C" === t ? null : c[t])
                        })
                    }
                    this.featureConfig = ie(!0, e.DEFAULT_FEATURE_CONFIG, this.playerConfig.features), this.qosReporter.reportInformation(e.CLIENT_ID, Z.exists(this.playerConfig.clientId) ? this.playerConfig.clientId : "NONE"), Ie.info("Using web player version " + this.loaderConfig.dependencies.ATVWebPlayer.version), Ie.info("Player config parameter:", t), Ie.info("Final player config:", this.playerConfig), Ie.info("User Agent: " + this.navigator.userAgent), Ie.info("Detected operating system: " + this.deviceInformation.operatingSystem), Ie.info("Detected browser: " + this.deviceInformation.browser), Ie.info("Page URL: " + (te.isUndefined(this.locationRef) ? "" : this.locationRef.href)), this.createHTMLElements();
                    var T = this.loaderConfig.assetsRoot + "/" + this.loaderConfig.dependencies.ATVWebPlayer.name + "/" + this.loaderConfig.dependencies.ATVWebPlayer.version + "/" + e.CSS_ASSET_PATH;
                    document.querySelector("link[href='" + T + "']") || this.resourceAcquirer.acquire(T), this.localizationStore = this.createLocalizationStore(), this.amazonUrlCreator = new z(this.playerConfig.marketplaceId, _e), this.localizationStore.setLanguage(W.EN_US, function() {}, function() {}), this.notificationManager.initialize(this.playerElementWrapper, this.playerElement, this.localizationStore, this.amazonUrlCreator), this.errorDisplayManager.initialize(this.uiElement, this.localizationStore, this.amazonUrlCreator, function() {
                        i.onClosePlayerClicked()
                    });
                    try {
                        this.initializePipeline.on(Ee, this.onNotificationEvent), this.initializePipeline.initialize({
                            onPlayerCapabilities: function(e) {
                                i.playerCapabilities = e, i.eventEmitter.emit(A, new A(e))
                            },
                            authToken: n.authToken,
                            loaderConfig: this.loaderConfig,
                            playerFactories: this.sortedPlayerFactories,
                            playerElement: this.playerElement,
                            weblabNames: l.getActiveWeblabs(),
                            weblabOverrides: g,
                            playerBeta: E,
                            playerCacheEnabled: this.playerCacheEnabled,
                            playerConfig: this.playerConfig,
                            playerOverride: Z.nestedValue(f, "playerOverride"),
                            qosReporter: this.qosReporter,
                            reporter: this.reporter,
                            spinnerPanel: this.spinnerPanel,
                            localizationStore: this.localizationStore
                        }, this.onPlayerCreated, this.abort)
                    } catch (t) {
                        Ie.error(t), this.abort(e.UNHANDLED_INITIALIZE_ERROR, t.message, S.Fatal)
                    }
                }, e.prototype.prepareCache = function(e) {
                    be(e, "contentConfig").is.an.object(), be(e.asin, "contentConfig.asin").is.a.string(), be(e.asin, "contentConfig.asin").is.not.empty(), be(e.cachePriority, "contentConfig.cachePriority").is.a.number(), be(g[e.cachePriority]).exists(), Ie.info("Prepare cache with " + e.asin + " at " + e.cachePriority + " priority")
                }, e.prototype.prepare = function(t) {
                    var i = this;
                    if (be(t, "contentConfig").is.an.object(), be(t.asin, "contentConfig.asin").is.a.string(), be(t.asin, "contentConfig.asin").is.not.empty(), !this.fsm.canTransitionTo(me.PREPARING)) throw new Error("Unable to prepare player while in the " + me[this.fsm.getCurrentState()] + " state");
                    var n = this.sanitizeContentConfig(t);
                    try {
                        var r = ie(!0, {
                            cachePriority: g.High,
                            isAVOD: !1,
                            trailer: !1,
                            immediateConsumption: !0
                        }, n);
                        if (this.ignorePrepare(r)) return;
                        Z.exists(t.features) && (this.featureConfig = ie(!0, e.DEFAULT_FEATURE_CONFIG, t.features)), this.fsm.transitionTo(me.PREPARING);
                        var o = this.getStoredOverrides();
                        if (te.each(e.ALLOWED_CONTENT_OVERRIDES, function(e) {
                                r[e] = Z.nestedValue(o, e, r[e])
                            }), r.asin === e.EVENT_HACK_LCID && (r.videoType = "External"), Ie.info("Content config parameter:", t), Ie.info("Final content config:", r), this.qosReporter.setAsin(r.asin), te.isEmpty(r.clientMetadata) || this.qosReporter.reportInformation(e.CLIENT_METADATA, r.clientMetadata), this.preparedListener = function() {
                                i.webPlayer.once(x, i.stoppingListener), i.webPlayer.off(b, i.initializedListener), i.fsm.transitionTo(me.PREPARED), i.eventEmitter.emit(N)
                            }, this.stoppingListener = function() {
                                i.webPlayer.once(b, i.initializedListener), i.fsm.transitionTo(me.STOPPING), i.eventEmitter.emit(x)
                            }, this.initializedListener = function() {
                                i.webPlayer.off(N, i.preparedListener), i.fsm.transitionTo(me.INITIALIZED), i.eventEmitter.emit(b)
                            }, this.webPlayer.once(N, this.preparedListener), this.webPlayer.once(b, this.initializedListener), M[r.videoType] === M.Live && this.deviceInformation.browser.getType() === F.Safari && "true" !== this.localStorage.get(ce.DONT_SHOW_AGAIN_KEY)) {
                            var s = new ce("safariBetterWatchExperience");
                            this.notificationManager.addNotification(s.createNotification(), !1)
                        }
                        this.webPlayer.prepare(r)
                    } catch (t) {
                        Ie.error(t), this.abort(e.UNHANDLED_PREPARE_ERROR, t.message, S.Fatal)
                    }
                }, e.prototype.play = function(t) {
                    if (be(t, "playbackConfig").is.an.object(), !this.fsm.transitionTo(me.PLAYING)) throw new Error("Unable to prepare player while in the " + me[this.fsm.getCurrentState()] + " state");
                    try {
                        var i = this.sanitizePlaybackConfig(t),
                            n = ie(!0, {}, i);
                        Ie.info("Playback config parameter:", t), Ie.info("Final playback config:", n), this.webPlayer.play(n), this.notificationManager.showQueuedNotifications()
                    } catch (t) {
                        Ie.error(t), this.abort(e.UNHANDLED_PLAY_ERROR, t.message, S.Fatal)
                    }
                }, e.prototype.playbackInitiated = function(e) {
                    this.hasPlaybackInitiated = !0, e || te.contains([me.INITIALIZED, me.INITIALIZING], this.fsm.getCurrentState()) ? (this.tryReportPlaybackInitiated(), this.tryNotifyPlaybackInitiated()) : this.notifyPlaybackInitiatedOnInitialized = !0
                }, e.prototype.stop = function() {
                    var t = this;
                    if (this.fsm.transitionTo(me.STOPPING)) try {
                        this.hasPlaybackInitiated = !1, this.webPlayer.off(b, this.initializedListener), this.webPlayer.off(N, this.preparedListener), this.webPlayer.off(x, this.stoppingListener), this.initializePipeline.off(Ee, this.onNotificationEvent), this.initializedListener = function() {
                            t.fsm.transitionTo(me.INITIALIZED), t.reporter.flush(), t.qosReporter.flush();
                            var e = re.v4(),
                                i = e + t.now();
                            t.configurationStore.setValue({
                                userWatchSessionId: e,
                                primitiveSessionId: i,
                                diagnosticsSessionConfiguration: t.diagnosticsSessionConfiguration
                            }).commit(), t.qosReporter.updateSessionIds(e, i), t.reporter.updateLoggingSessionId(e), t.reporter.updateMetricsSessionId(e), t.notifyPlaybackInitiatedOnInitialized && (t.notifyPlaybackInitiatedOnInitialized = !1, t.playbackInitiated(!0)), t.eventEmitter.emit(b)
                        }, this.webPlayer.once(b, this.initializedListener), this.pinDisplayManager.hideModal(), this.webPlayer.stop()
                    } catch (t) {
                        Ie.error(t), this.abort(e.UNHANDLED_STOP_ERROR, t.message, S.Fatal)
                    }
                }, e.prototype.seek = function(e) {
                    be(e, "position").is.a.number(), be(e >= 0, "position >= 0").is["true"]();
                    var t = this.fsm.getCurrentState();
                    return t !== me.PREPARED && t !== me.PLAYING ? void Ie.warn("The player has not started playback; ignoring call to seek") : (Ie.info("Seeking to position: " + e), void this.webPlayer.seek(e))
                }, e.prototype.on = function(e, t) {
                    this.eventEmitter.on(e, t)
                }, e.prototype.once = function(e, t) {
                    this.eventEmitter.once(e, t)
                }, e.prototype.off = function(e, t) {
                    this.eventEmitter.off(e, t)
                }, e.prototype.getFeature = function(e, t, i) {
                    var n = T[T[e]];
                    return Z.exists(n) ? Z.exists(this.webPlayer) ? void this.webPlayer.getFeature(n, t, i) : void i(new Error("Cannot acquire the requested feature because the player is not initialized yet.")) : void i(new Error("The requested feature does not exist."))
                }, e.prototype.getPosition = function() {
                    var e = this.fsm.getCurrentState();
                    return e !== me.PREPARED && e !== me.PLAYING ? (Ie.warn("The player has not been prepared; ignoring call to getPosition"), 0) : this.webPlayer.getPosition()
                }, e.prototype.getDuration = function() {
                    var e = this.fsm.getCurrentState();
                    return e !== me.PREPARED && e !== me.PLAYING ? (Ie.warn("The player has not been prepared; ignoring call to getDuration"), 0) : this.webPlayer.getDuration()
                }, e.prototype.getPlayerCapabilities = function() {
                    return (be(this.fsm.getCurrentState() !== me.UNINITIALIZED, "currentState !== UNINITIALIZED").is["true"](), te.clone(this.playerCapabilities))
                }, e.prototype.sanitizePlayerConfig = function(t) {
                    var i = {
                        authToken: t.authToken,
                        domElement: t.domElement,
                        marketplaceId: t.marketplaceId,
                        requiredCapabilities: t.requiredCapabilities,
                        loaderConfig: this.loaderConfig,
                        clientId: t.clientId,
                        deviceId: t.deviceId
                    };
                    return (this.isConfigString(t.customerId, "PlayerConfig", "customerId") && (i.customerId = t.customerId), this.isConfigString(t.locale, "PlayerConfig", "locale") && (i.locale = t.locale), this.isConfigString(t.sessionId, "PlayerConfig", "sessionId") && (i.sessionId = t.sessionId), te.isObject(t.features) && (i.features = t.features), te.isObject(t.featuresSettings) && (i.featuresSettings = t.featuresSettings), this.isConfigString(t.siteSectionId, "PlayerConfig", "siteSectionId") && (i.siteSectionId = t.siteSectionId), this.isConfigBoolean(t.playerBeta, "PlayerConfig", "playerBeta") && (i.playerBeta = t.playerBeta), te.each(te.difference(te.keys(t), e.PLAYER_CONFIG_PROPERTIES), function(e) {
                        Ie.warn("PlayerConfig has unsupported property: " + e)
                    }), i)
                }, e.prototype.sanitizeContentConfig = function(t) {
                    var i = {
                        asin: t.asin
                    };
                    this.isConfigBoolean(t.isAVOD, "ContentConfig", "isAVOD") && (i.isAVOD = t.isAVOD),
                        this.isConfigNumber(t.position, "ContentConfig", "position") && (i.position = t.position), te.isObject(t.features) && (i.features = t.features);
                    var n = this.getEnumValue(t.videoType, "ContentConfig", "videoType", M);
                    return (Z.exists(n) ? i.videoType = M[n] : (n = M.Feature, this.isConfigBoolean(t.trailer, "ContentConfig", "trailer") && t.trailer && (n = M.Trailer), this.isConfigBoolean(t.live, "ContentConfig", "live") && t.live && (be(n === M.Feature, "videoType is consistent").is["true"](), n = M.Live), i.videoType = M[n]), this.isConfigBoolean(t.immediateConsumption, "ContentConfig", "immediateConsumption") && (i.immediateConsumption = t.immediateConsumption), this.isConfigString(t.clientMetadata, "ContentConfig", "clientMetadata") && (i.clientMetadata = t.clientMetadata), te.each(te.difference(te.keys(t), e.CONTENT_CONFIG_PROPERTIES), function(e) {
                        Ie.warn("ContentConfig has unsupported property: " + e)
                    }), i)
                }, e.prototype.sanitizePlaybackConfig = function(t) {
                    var i = {
                        playbackInitiatedTime: t.playbackInitiatedTime
                    };
                    return (this.isConfigNumber(t.cacheType, "PlaybackConfig", "cacheType") && (i.cacheType = t.cacheType), this.isConfigString(t.audioTrackId, "PlaybackConfig", "audioTrackId") && (i.audioTrackId = t.audioTrackId), this.isConfigString(t.clickstreamRefMarker, "PlaybackConfig", "clickstreamRefMarker") && (i.clickstreamRefMarker = t.clickstreamRefMarker), te.each(te.difference(te.keys(t), e.PLAYBACK_CONFIG_PROPERTIES), function(e) {
                        Ie.warn("PlaybackConfig has unsupported property: " + e)
                    }), i)
                }, e.prototype.isConfigBoolean = function(e, t, i) {
                    return !(Z.isNullOrUndefined(e) || !te.isBoolean(e) && (Ie.warn(t + " property '" + i + "' must be a boolean but was " + e), 1))
                }, e.prototype.isConfigNumber = function(e, t, i) {
                    return !(Z.isNullOrUndefined(e) || (te.isNumber(e) ? e < 0 && (Ie.warn(t + " property '" + i + "' cannot be a negative but was " + e), 1) : (Ie.warn(t + " property '" + i + "' must be a number but was " + e), 1)))
                }, e.prototype.isConfigString = function(e, t, i) {
                    return !(Z.isNullOrUndefined(e) || (te.isString(e) ? te.isEmpty(e) && (Ie.warn(t + " property '" + i + "' cannot be an empty string"), 1) : (Ie.warn(t + " property '" + i + "' must be a string but was " + e), 1)))
                }, e.prototype.getEnumValue = function(e, t, i, n) {
                    if (Z.isNullOrUndefined(e)) return null;
                    if (!te.isString(e)) return (Ie.warn(t + " property '" + i + "' must be a string but was " + e), null);
                    if (te.isEmpty(e)) return (Ie.warn(t + " property '" + i + "' cannot be an empty string"), null);
                    var r = Y.getEnumValue(e, n);
                    return Z.isNullOrUndefined(r) ? (Ie.warn(t + " property '" + i + "' did not receive a valid enum value for the given type. Value received " + e + ". "), null) : r
                }, e.prototype.ignorePrepare = function(e) {
                    return !e.immediateConsumption && (Z.exists(this.navigator.connection) && "cellular" === this.navigator.connection.type ? (Ie.warn("Pre-cache disabled on Cellular connections, ignoring calls to prepare."), !0) : e.videoType === M[M.Live] && (Ie.warn("Pre-cache disabled for Live streams, ignoring calls to prepare."), !0))
                }, e.prototype.createHTMLElements = function() {
                    this.playerElementWrapper = document.createElement("div"), this.playerElementWrapper.style.height = "100%", this.playerElementWrapper.style.width = "100%", this.playerElement = document.createElement("div"), this.playerElement.className = "webPlayerElement", this.playerElementWrapper.appendChild(this.playerElement), this.uiElement = document.createElement("div"), this.uiElement.className = "uiElement", this.parentElement = document.createElement("div"), this.parentElement.className = "webPlayerContainer", this.parentElement.appendChild(this.playerElementWrapper), this.parentElement.appendChild(this.uiElement), this.playerConfig.domElement.innerHTML = "", this.playerConfig.domElement.appendChild(this.parentElement)
                }, e.prototype.createDiagnosticsConfig = function() {
                    var t, i = L.getParameter(e.PLAYER_DIAGNOSTICS_TYPE_PARAM),
                        n = Y.getEnumValue(i, E);
                    if (n === E.Http) t = this.localStorage.get(a.DiagnosticsEndpointKey);
                    else {
                        var r = this.loaderConfig.dependencies.ATVWebDiagnosticsRenderer;
                        n = E.PostMessage, t = this.loaderConfig.assetsRoot + "/" + r.name + "/" + r.version + "/html/HTML5Diagnostics.html"
                    }
                    return {
                        protocol: n,
                        endpointURI: t
                    }
                }, e.prototype.transitionToAborted = function() {
                    this.fsm.transitionTo(me.ABORTED), this.eventEmitter.emit(f)
                }, e.prototype.reportFatalError = function(t, i, n) {
                    this.qosReporter.reportError(t, n, i), r.putMetric(t), n !== S.Crash && n !== S.Fatal || r.putMetric(e.FATAL_ERROR_METRIC)
                }, e.prototype.showErrorMessage = function(e, t) {
                    var i = this.localizationStore.getLocalization();
                    this.playerElementWrapper.innerHTML = "", this.playerElement = null, this.playerElementWrapper = null, Z.exists(e) ? this.errorDisplayManager.showError(e, this.featureConfig.playerCloseable) : Z.exists(t) && Z.exists(i[t]) ? this.errorDisplayManager.showError(new ge(t), this.featureConfig.playerCloseable) : this.errorDisplayManager.showError(new pe, this.featureConfig.playerCloseable)
                }, e.prototype.hidePlayer = function() {
                    this.playerElementWrapper.style.height = "1px", this.playerElementWrapper.style.width = "1px"
                }, e.prototype.showPlayer = function() {
                    Z.exists(this.playerElementWrapper) && (this.playerElementWrapper.style.height = "100%", this.playerElementWrapper.style.width = "100%")
                }, e.prototype.proxyEvent = function(e, t) {
                    var i = this;
                    e.on(t, function(e) {
                        i.eventEmitter.emit(t, e)
                    })
                }, e.prototype.attachEventListeners = function(t) {
                    var i = this;
                    t.on(d, function(e) {
                        null !== e.getError() && Ie.error(e.getError()), i.abort(e.getErrorName(), e.getErrorMessage(), e.getErrorSeverity(), e.getTemplate())
                    }), t.on(f, function() {
                        i.transitionToAborted()
                    }), t.on(I, this.onNotificationCancelEvent), t.on(w, this.onNotificationEvent), t.on(P, function(e) {
                        i.onPINRequired(e)
                    }), t.on(R, function() {
                        i.showPlayer()
                    }), t.on(C, function() {
                        i.abort(e.PIN_VALIDATION_ERROR, "An error occurred while attempting to validate the customer's PIN", S.Fatal, new ye)
                    }), this.proxyEvent(t, y), this.proxyEvent(t, m), this.proxyEvent(t, _), this.proxyEvent(t, O)
                }, e.prototype.onPINRequired = function(t) {
                    t.getRequiresPINSetup() ? this.abort(e.PIN_SETUP_REQUIRED_ERROR, "PIN on playback is required but the customer does not have a PIN set up", S.NonFatal, new de) : t.getRequiresAgeVerification() ? this.abort(e.PIN_AGE_VERIFICATION_ERROR, "Age verification on playback is required but the customer does not have age verified", S.NonFatal, new he) : this.showPINModal(t.getPINLength(), t.getHidePlayer())
                }, e.prototype.onClosePlayerClicked = function() {
                    this.showPlayer(), this.eventEmitter.emit(m)
                }, e.prototype.createLocalizationStore = function() {
                    var e = this.loaderConfig.dependencies.ATVWebPlayer,
                        t = this.loaderConfig.assetsRoot + "/" + e.name + "/" + e.version,
                        i = W.AvailableLocales.reduce(function(e, i) {
                            return n({}, e, (r = {}, r[i] = i === W.EN_US ? Te : t + "/languages/" + i + ".json", r));
                            var r
                        }, {});
                    return new this.localizationStoreType(e.version, e.name, i, W.EN_US, this.playerCacheEnabled)
                }, e.prototype.createDeviceId = function(t) {
                    var i = this.localStorage.get(e.DEVICE_ID_KEY);
                    if (Z.isNullOrUndefined(i)) {
                        var n = new ne(t + re.v4(), "TEXT");
                        i = n.getHash("SHA-224", "HEX"), this.localStorage.set(e.DEVICE_ID_KEY, i)
                    }
                    return i
                }, e.prototype.showPINModal = function(t, i) {
                    i && this.hidePlayer();
                    try {
                        this.pinDisplayManager.requestPIN(t)
                    } catch (t) {
                        this.abort(e.PIN_LOOKUP_ERROR, "An error occurred while attempting to validate the customer's PIN", S.Fatal, new fe)
                    }
                }, e.prototype.tryNotifyPlaybackInitiated = function() {
                    null !== this.webPlayer && this.hasPlaybackInitiated && this.webPlayer.playbackInitiated()
                }, e.prototype.tryReportPlaybackInitiated = function() {
                    this.hasPlaybackInitiated && this.qosReporter.reportInformation(e.PLAYBACK_INITIATED)
                }, e.prototype.getStoredOverrides = function() {
                    try {
                        var t = this.sessionStorage.get(e.OVERRIDES_STORAGE_KEY),
                            i = JSON.parse(t);
                        return te.isObject(i) ? i : {}
                    } catch (e) {
                        return {}
                    }
                }, e.prototype.createFiniteStateMachine = function() {
                    var e = new q(me.UNINITIALIZED);
                    return (e.addTransition(me.UNINITIALIZED, me.INITIALIZING), e.addTransition(me.INITIALIZING, me.INITIALIZED), e.addTransition(me.INITIALIZING, me.ABORTED), e.addTransition(me.INITIALIZED, me.PREPARING), e.addTransition(me.PREPARING, me.PREPARED), e.addTransition(me.PREPARING, me.INITIALIZED), e.addTransition(me.PREPARING, me.STOPPING), e.addTransition(me.PREPARING, me.ABORTED), e.addTransition(me.PREPARED, me.STOPPING), e.addTransition(me.PREPARED, me.ABORTED), e.addTransition(me.PREPARED, me.PLAYING), e.addTransition(me.PLAYING, me.STOPPING), e.addTransition(me.PLAYING, me.ABORTED), e.addTransition(me.STOPPING, me.INITIALIZED), e.addTransition(me.STOPPING, me.ABORTED), e.onTransition(function(e, t) {
                        Ie.info("Transitioned from " + me[e] + " to " + me[t])
                    }), e.onIllegalTransition(function(e, t) {
                        Ie.warn("Attempted illegal state transition from " + me[e] + " to " + me[t])
                    }), e)
                }, e.EVENT_HACK_LCID = "amzn1.dv.lcid.Ext73509-2ae9-4b43-a957-236304d15be1", e.CSS_ASSET_PATH = "css/ATVWebPlayer.css", e.PLAYER_CACHE_QUERY_PARAM = "playerCache", e.PLAYER_DEBUG_QUERY_PARAM = "playerDebug", e.PLAYER_DIAGNOSTICS_TYPE_PARAM = "debugType", e.OVERRIDES_STORAGE_KEY = "playerOverrides", e.DEVICE_ID_KEY = "atvwebplayer_deviceid", e.SESSION_ID_KEY = "session-id", e.PLAYER_CONFIG_PROPERTIES = ["sessionId", "authToken", "clientId", "customerId", "domElement", "features", "featuresSettings", "marketplaceId", "locale", "requiredCapabilities", "siteSectionId"], e.CONTENT_CONFIG_PROPERTIES = ["asin", "cachePriority", "clientMetadata", "features", "immediateConsumption", "isAVOD", "live", "position", "trailer", "videoType"], e.ALLOWED_CONTENT_OVERRIDES = ["position"], e.PLAYBACK_CONFIG_PROPERTIES = ["audioTrackId", "clickstreamRefMarker", "playbackInitiatedTime", "cacheType"], e.INITIALIZE_TIME = "DelegatorInitializeTime", e.FATAL_ERROR_METRIC = "FatalError", e.CLIENT_METADATA = "ClientMetadata", e.CLIENT_ID = "ClientID", e.PIN_AGE_VERIFICATION_ERROR = "PINAgeVerificationError", e.PIN_LOOKUP_ERROR = "PINLookupError", e.PIN_SETUP_REQUIRED_ERROR = "PINSetupRequiredError", e.PIN_VALIDATION_ERROR = "PINValidationError", e.UNHANDLED_INITIALIZE_ERROR = "UnhandledInitializeError", e.UNHANDLED_PREPARE_ERROR = "UnhandledPrepareError", e.UNHANDLED_PLAY_ERROR = "UnhandledPlayError", e.UNHANDLED_STOP_ERROR = "UnhandledStopError", e.PLAYBACK_INITIATED = "PlaybackInitiated", e.DEFAULT_FEATURE_CONFIG = {
                    enableHdIndicator: !0,
                    enableLiveContentWarning: !0,
                    enableMta: !0,
                    enableNextUp: !0,
                    enablePause: !0,
                    enableSeek: !0,
                    enableUpdateStream: !0,
                    enableXray: !0,
                    enableOptions: !0,
                    heroImageUrl: null,
                    playerCloseable: !0
                }, e)
            }();
        t.exports = Re
    }, {
        1: 1,
        113: 113,
        116: 116,
        126: 126,
        16: 16,
        17: 17,
        18: 18,
        186: 186,
        187: 187,
        19: 19,
        2: 2,
        20: 20,
        21: 21,
        22: 22,
        231: 231,
        232: 232,
        24: 24,
        243: 243,
        25: 25,
        272: 272,
        281: 281,
        284: 284,
        286: 286,
        288: 288,
        29: 29,
        294: 294,
        30: 30,
        32: 32,
        33: 33,
        34: 34,
        36: 36,
        41: 41,
        6: 6,
        80: 80,
        82: 82,
        83: 83,
        93: 93
    }],
    4: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.PLAYER_OVERRIDE = "PlayerOverride", e.WEBLAB = "Weblab", e.MISSING_WIDEVINE_CDM = "MissingWidevineCDM", e.MISSING_PLAYREADY_CDM = "MissingPlayReadyCDM", e.MISSING_FAIRPLAY_CDM = "MissingFairplayCDM", e.MISSING_CODEC_IN_PLAYREADY_CDM = "MissingCodecInPlayReadyCDM", e.UNKNOWN = "UNKNOWN", e.UNSUPPORTED_BROWSER = "UnsupportedBrowser", e.UNSUPPORTED_OS = "UnsupportedOperatingSystem", e.MISSING_MEDIA_SOURCE = "MissingMediaSource", e)
        }();
        t.exports = n
    }, {}],
    5: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e, t, i, n) {
                this.name = e, this.message = t, this.severity = i, this.template = n
            }
            return (e.ERROR_NAME = "InitializeAbortedError", e)
        }();
        t.exports = n
    }, {}],
    6: [function(e, t, i) {
        "use strict";
        var n = e(289),
            r = e(281),
            o = e(93),
            s = e(284),
            a = e(82),
            l = a.BasicEventEmitter,
            c = e(231),
            u = c.ErrorSeverity,
            p = e(7),
            h = e(5),
            f = r.of("InitializePipeline"),
            d = s.get("InitializePipeline"),
            y = function() {
                function e(e, t) {
                    void 0 === e && (e = p), void 0 === t && (t = new l), this.eventEmitter = t, this.stages = e
                }
                return (e.prototype.initialize = function(t, i, r) {
                    f(t, "config").exists(), f(i, "onInitialized").is.a["function"](), f(r, "onError").is.a["function"]();
                    var s = {
                        config: t,
                        eventEmitter: this.eventEmitter,
                        factory: null,
                        playerCapabilities: null,
                        webPlayer: null
                    };
                    return n(s).then(this.stages.preinitializeReporting).then(this.stages.downloadLocalization).then(this.stages.getWeblabs).then(this.stages.createTokenClient).then(this.stages.cacheAuthToken).then(this.stages.selectFactory).then(this.stages.initializeReporting).then(this.stages.createWebPlayer).done(function(e) {
                        i(e.factory, e.webPlayer)
                    }, function(t) {
                        t.name !== h.ERROR_NAME || o.isNullOrUndefined(s.abortReason) ? (d.error("Unhandled initialize error", t), r(e.UNHANDLED_INITIALIZE_ERROR, t.message, u.Fatal)) : (d.error("Initialization aborted", s.abortReason), r(s.abortReason.name, s.abortReason.message, s.abortReason.severity, s.abortReason.template))
                    })
                }, e.prototype.on = function(e, t) {
                    this.eventEmitter.on(e, t)
                }, e.prototype.once = function(e, t) {
                    this.eventEmitter.once(e, t)
                }, e.prototype.off = function(e, t) {
                    this.eventEmitter.off(e, t)
                }, e.UNHANDLED_INITIALIZE_ERROR = "UnhandledInitializeError", e)
            }();
        t.exports = y
    }, {
        231: 231,
        281: 281,
        284: 284,
        289: 289,
        5: 5,
        7: 7,
        82: 82,
        93: 93
    }],
    7: [function(e, t, i) {
        "use strict";
        var n = e(9),
            r = e(10),
            o = e(8),
            s = e(11),
            a = e(12),
            l = e(13),
            c = e(14),
            u = e(15),
            p = {
                cacheAuthToken: o,
                createTokenClient: n,
                createWebPlayer: r,
                downloadLocalization: s,
                getWeblabs: a,
                initializeReporting: l,
                preinitializeReporting: c,
                selectFactory: u
            };
        t.exports = p
    }, {
        10: 10,
        11: 11,
        12: 12,
        13: 13,
        14: 14,
        15: 15,
        8: 8,
        9: 9
    }],
    8: [function(e, t, i) {
        "use strict";

        function n(e) {
            var t = e.config.playerConfig.tokenClient;
            return (o.exists(t) && t.getToken(function(e) {
                s.info("Auth token cache succeeded.")
            }, function(e) {
                s.warn("Auth token cache failed.")
            }), e)
        }
        var r = e(284),
            o = e(93),
            s = r.get("CacheAuthToken");
        t.exports = n
    }, {
        284: 284,
        93: 93
    }],
    9: [function(e, t, i) {
        "use strict";

        function n(e, t) {
            var i = {
                appVersion: e.config.loaderConfig.dependencies.ATVWebPlayer.version,
                appName: d,
                marketplaceId: e.config.playerConfig.marketplaceId,
                token: {
                    access: t.access,
                    expireTime: t.expireTime,
                    refresh: t.refresh
                }
            };
            return new a(i, function(t) {
                e.config.qosReporter.reportInformation(y, null, t), p.putMetric(y, t, f)
            }, function(e) {
                p.putMetric(e.name)
            })
        }

        function r(e) {
            var t = e.config.authToken;
            return h.exists(t) && e.config.playerConfig.weblabs[c.ATV_WEB_PLAYER_OAUTH] === u.T1 ? n(e, t) : null
        }

        function o(e) {
            return h.isNullOrUndefined(e.config.playerConfig.customerId) ? e : (e.config.playerConfig.tokenClient = r(e), e.config.qosReporter.setTokenClient(e.config.playerConfig.tokenClient), e)
        }
        var s = e(58),
            a = s.OAuthTokenClient,
            l = e(80),
            c = l.Weblabs,
            u = l.WeblabTreatment,
            p = e(187),
            h = e(93),
            f = "ms",
            d = "ATVWebPlayer",
            y = "OAuthTokenTime";
        t.exports = o
    }, {
        187: 187,
        58: 58,
        80: 80,
        93: 93
    }],
    10: [function(e, t, i) {
        "use strict";

        function n(e) {
            var t = r.defer();
            return (e.factory.createWebPlayer(function(i) {
                e.webPlayer = i, t.resolve(e)
            }, function(i, n, r, a) {
                e.abortReason = new o(i, n, r, a), t.reject(s.newError(o.ERROR_NAME, e.abortReason.message))
            }, e.config.playerConfig.loaderConfig.assetsRoot), t.promise)
        }
        var r = e(289),
            o = e(5),
            s = e(159);
        t.exports = n
    }, {
        159: 159,
        289: 289,
        5: 5
    }],
    11: [function(e, t, i) {
        "use strict";

        function n(e, t) {
            void 0 === t && (t = r);
            var i = "{{locale}}",
                n = "LanguageDownloadCacheHit",
                o = "LanguageDownloadCacheMiss",
                l = "LanguageDownload.Error." + i,
                c = "LanguageDownload.Time." + i,
                u = s.defer(),
                p = e.config.playerConfig.locale;
            return (e.config.localizationStore.setLanguage(p, function(s, l, h) {
                t.putMetric(c.replace(i, p), l, r.MILLISECONDS), t.putMetric(h ? n : o), a.info("Localization download successful."), u.resolve(e)
            }, function(n) {
                t.putMetric(l.replace(i, p)), a.warn("Localization download failed. Falling back to en-US."), u.resolve(e)
            }), u.promise)
        }
        var r = e(187),
            o = e(284),
            s = e(289),
            a = o.get("DownloadLocalization");
        t.exports = n
    }, {
        187: 187,
        284: 284,
        289: 289
    }],
    12: [function(e, t, i) {
        "use strict";

        function n(e, t) {
            void 0 === t && (t = new o);
            var i = p.defer(),
                n = null;
            return (u.exists(e.config.playerConfig.customerId) && (n = {
                customerId: e.config.playerConfig.customerId,
                tokenClient: e.config.playerConfig.tokenClient
            }), t.initialize(e.config.playerConfig.partnerServiceEndpoint, e.config.playerConfig.marketplaceId, a.HTML5, e.config.playerConfig.deviceId, e.config.playerConfig.sessionId, n), t.getWeblabs(e.config.weblabNames, function(t, n) {
                u.exists(n) && (c.putMetric(y, n, c.MILLISECONDS), e.config.qosReporter.reportInformation(y, null, n)), f.info("Acquired weblabs: ", t), e.config.playerConfig.weblabs = h.defaults(e.config.weblabOverrides, t), f.info("Final weblabs: ", e.config.playerConfig.weblabs);
                try {
                    e.config.qosReporter.reportInformation("Weblabs", JSON.stringify(e.config.playerConfig.weblabs))
                } catch (e) {
                    f.error(e)
                }
                i.resolve(e)
            }, function(t, n) {
                f.error("Unable to receive weblabs.", t), c.putMetric(d), u.exists(n) && c.putMetric(y, n, c.MILLISECONDS), e.config.playerConfig.weblabs = e.config.weblabOverrides, i.resolve(e)
            }), i.promise)
        }
        var r = e(278),
            o = r.WeblabClient,
            s = e(80),
            a = s.DeviceTypeId,
            l = e(284),
            c = e(187),
            u = e(93),
            p = e(289),
            h = e(294),
            f = l.get("getWeblabs"),
            d = "GetWeblabTreatmentError",
            y = "GetWeblabTreatmentTime";
        t.exports = n
    }, {
        187: 187,
        278: 278,
        284: 284,
        289: 289,
        294: 294,
        80: 80,
        93: 93
    }],
    13: [function(e, t, i) {
        "use strict";

        function n(e) {
            var t = e.config.playerConfig;
            e.config.reporter.initializeLogging(t.marketplaceId, a.withDefault(t.customerId, l), t.configurationStore.getValue().userWatchSessionId, t.configurationStore, t.logLevel, c, t.diagnostics), e.config.reporter.initializeMetrics(e.factory.getMetricsFeature()), e.config.qosReporter.initialize(e.factory.getPlayerVersion(), e.factory.getDeviceTypeId());
            var i = r.getInstance();
            return (t.diagnostics === s.Auto && i.open(), e)
        }
        var r = e(158),
            o = e(231),
            s = o.DiagnosticsState,
            a = e(93),
            l = "UNAUTHENTICATED",
            c = 10;
        t.exports = n
    }, {
        158: 158,
        231: 231,
        93: 93
    }],
    14: [function(e, t, i) {
        "use strict";

        function n(e, t, i, n) {
            void 0 === t && (t = s.getOperatingSystem()), void 0 === i && (i = s.getBrowser()), void 0 === n && (n = navigator);
            var l = i.toString();
            o.exists(n.buildID) && (l = l + " build:" + n.buildID);
            var c = e.config.playerConfig.configurationStore.getValue();
            return (e.config.reporter.preInitializeMetrics(e.config.playerConfig.marketplaceId, c.userWatchSessionId, e.config.playerConfig.clientId, a, i), e.config.qosReporter.preInitialize(e.config.playerConfig.partnerServiceEndpoint, e.config.loaderConfig.dependencies.ATVWebPlayer.version, e.config.playerConfig.marketplaceId, r.DeviceTypeId.HTML5, e.config.playerConfig.deviceId, c.primitiveSessionId, t.toString(), l), e.config.qosReporter.updateSessionIds(c.userWatchSessionId, c.primitiveSessionId), e)
        }
        var r = e(80),
            o = e(93),
            s = e(272),
            a = "HTML5Player";
        t.exports = n
    }, {
        272: 272,
        80: 80,
        93: 93
    }],
    15: [function(e, t, i) {
        "use strict";

        function n(e, t) {
            void 0 === t && (t = ""), m.isNullOrUndefined(e.config.playerOverride) || (t = a.PLAYER_OVERRIDE), e.config.qosReporter.reportInformation(u.HTML5_NOT_USED_ERROR_NAME, t), E.warn("HTML5 player is not used because of " + t)
        }

        function r(e, t, i, r, a) {
            void 0 === t && (t = new c), void 0 === i && (i = d.getOperatingSystem()), void 0 === r && (r = d.getBrowser()), void 0 === a && (a = l);
            var y;
            y = m.isNullOrUndefined(e.config.playerFactories) ? [new u(e.config.loaderConfig, e.config.qosReporter, e.config.playerBeta), new p(i, r, e.config.loaderConfig, e.config.qosReporter)] : e.config.playerFactories, t.initialize(y, e.config.playerConfig.marketplaceId, e.config.playerConfig.weblabs, e.config.playerCacheEnabled, e.config.localizationStore.getCurrentLanguage(), e.config.playerConfig.clientId);
            var v = o.defer(),
                E = function(t, o, l, c) {
                    if (null === t) return (n(e, l), e.abortReason = new s(_, "No player is supported on this platform.", g.NonFatal, new a(i, r, c)), void v.reject(f.newError(s.ERROR_NAME, e.abortReason.message)));
                    t.getPlayerName() !== S && n(e, l), e.factory = t, e.factory.on(h, function(t) {
                        e.eventEmitter.emit(h, t)
                    }), e.config.onPlayerCapabilities(o);
                    var u = e.config.loaderConfig.dependencies.ATVWebPlayer,
                        p = e.config.loaderConfig.assetsRoot + "/" + u.name + "/" + u.version;
                    e.config.spinnerPanel.initialize(e.config.playerElement, p, t.getPlayerName() !== S), v.resolve(e)
                };
            return (m.isNullOrUndefined(e.config.playerOverride) ? t.selectFactory(E, e.config.playerConfig.requiredCapabilities, S) : t.selectFactoryWithOverride(E, e.config.playerOverride), v.promise)
        }
        var o = e(289),
            s = e(5),
            a = e(4),
            l = e(23),
            c = e(50),
            u = e(27),
            p = e(37),
            h = e(25),
            f = e(159),
            d = e(272),
            y = e(231),
            g = y.ErrorSeverity,
            m = e(93),
            v = e(284),
            E = v.get("selectFactory"),
            _ = "UnsupportedPlatformError",
            S = "html5";
        t.exports = r
    }, {
        159: 159,
        23: 23,
        231: 231,
        25: 25,
        27: 27,
        272: 272,
        284: 284,
        289: 289,
        37: 37,
        4: 4,
        5: 5,
        50: 50,
        93: 93
    }],
    16: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = e(232),
            o = e(113),
            s = o.LocalStorage,
            a = function() {
                function e(t, i) {
                    void 0 === t && (t = new s), void 0 === i && (i = r);
                    var o = this;
                    this.storage = t, this.queryString = i, n.chain(e.flagStorageKeys).keys().each(function(t) {
                        o.handleFlagStorage(t, e.flagStorageKeys[t])
                    })
                }
                return (e.prototype.isActiveFlag = function(e) {
                    return "true" === this.storage.get(e)
                }, e.prototype.handleFlagStorage = function(e, t) {
                    var i = this.queryString.getParameter(e);
                    i === !0 ? this.storage.set(t, "true") : i === !1 && this.storage.remove(t)
                }, e.HTML5_DIAGNOSTICS_STORAGE_KEY = "html5_web_player_diagnostics", e.HTML5_HEVC_TEST_STORAGE_KEY = "html5_web_player_hevc_test", e.HTML5_XRAY_BETA_STORAGE_KEY = "html5_web_player_xray_beta", e.flagStorageKeys = {
                    playerDebug: e.HTML5_DIAGNOSTICS_STORAGE_KEY,
                    hevcTest: e.HTML5_HEVC_TEST_STORAGE_KEY,
                    xrayBeta: e.HTML5_XRAY_BETA_STORAGE_KEY
                }, e)
            }();
        t.exports = a
    }, {
        113: 113,
        232: 232,
        294: 294
    }],
    17: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.UNINITIALIZED = 0] = "UNINITIALIZED", e[e.INITIALIZING = 1] = "INITIALIZING", e[e.INITIALIZED = 2] = "INITIALIZED", e[e.PREPARING = 3] = "PREPARING", e[e.PREPARED = 4] = "PREPARED", e[e.PLAYING = 5] = "PLAYING", e[e.STOPPING = 6] = "STOPPING", e[e.ABORTED = 7] = "ABORTED"
        }(n || (n = {})), t.exports = n
    }, {}],
    18: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(32),
            o = function(e) {
                function t() {
                    return e.call(this, t.KEY) || this
                }
                return (n(t, e), t.KEY = "genericError", t)
            }(r);
        t.exports = o
    }, {
        32: 32
    }],
    19: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(32),
            o = function(e) {
                function t() {
                    return e.call(this, t.KEY) || this
                }
                return (n(t, e), t.KEY = "pinAgeVerificationRequired", t)
            }(r);
        t.exports = o
    }, {
        32: 32
    }],
    20: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(32),
            o = function(e) {
                function t() {
                    return e.call(this, t.KEY) || this
                }
                return (n(t, e), t.KEY = "pinLookupErrorFsk18", t)
            }(r);
        t.exports = o
    }, {
        32: 32
    }],
    21: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(32),
            o = function(e) {
                function t() {
                    return e.call(this, t.KEY) || this
                }
                return (n(t, e), t.KEY = "pinSetupRequired", t)
            }(r);
        t.exports = o
    }, {
        32: 32
    }],
    22: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(32),
            o = function(e) {
                function t() {
                    return e.call(this, t.KEY) || this
                }
                return (n(t, e), t.KEY = "pinValidationError", t)
            }(r);
        t.exports = o
    }, {
        32: 32
    }],
    23: [function(e, t, i) {
        "use strict";
        var n, r = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            o = e(272),
            s = o.BrowserType,
            a = o.OperatingSystemFamily,
            l = e(113),
            c = l.Version,
            u = e(93),
            p = e(49),
            h = e(32),
            f = function(e) {
                function t(i, n, r) {
                    var o = this,
                        l = u.exists(t.KEYS[r]) ? t.KEYS[r][s[n.getType()]] : null;
                    return (t.isChromeAndroid(n, i) ? l = t.MISSING_CDM_KEYS.Android : i.getFamily() === a.Mac && i.getVersion().compareTo(c.fromString("10.9")) < 0 && (l = t.MAC_LESS_THAN_10_9_UNSUPPORTED_BROWSER), u.isNullOrUndefined(l) && (l = r === p.UNSUPPORTED_OS ? t.GENERIC_UNSUPPORTED_OS : t.UNSUPPORTED_BROWSER_OS_KEYS[a[i.getFamily()]]), l = u.withDefault(l, t.GENERIC_UNSUPPORTED_BROWSER), o = e.call(this, l) || this)
                }
                return (r(t, e), t.isChromeAndroid = function(e, t) {
                    return e.getType() === s.Chrome && (t.getFamily() === a.Linux || t.getFamily() === a.Android) && u.exists(navigator.connection)
                }, t.GENERIC_UNSUPPORTED_BROWSER = "genericUnsupportedBrowser", t.GENERIC_UNSUPPORTED_OS = "genericUnsupportedOs", t.MAC_LESS_THAN_10_9_UNSUPPORTED_BROWSER = "macLessThan10Point9UnsupportedBrowser", t.UPDATE_BROWSER_KEYS = {
                    Chrome: "chromeOutOfDate",
                    Firefox: "firefoxOutOfDate",
                    Firefox64: "firefoxOutOfDate",
                    Opera: "operaOutOfDate"
                }, t.MISSING_CDM_KEYS = {
                    Android: "androidMissingCdm",
                    Chrome: "chromeMissingCdm",
                    Firefox: "firefoxMissingCdm",
                    Firefox64: "firefoxMissingCdm",
                    Opera: "operaMissingCdm"
                }, t.UNSUPPORTED_BROWSER_KEYS = {}, t.UNSUPPORTED_BROWSER_OS_KEYS = {
                    Linux: "linuxUnsupportedBrowser",
                    Mac: "macUnsupportedBrowser",
                    Windows: "windowsUnsupportedBrowser",
                    Android: t.GENERIC_UNSUPPORTED_OS
                }, t.UNSUPPORTED_OS_KEYS = {}, t.KEYS = (n = {}, n[p.BROWSER_OUT_OF_DATE] = t.UPDATE_BROWSER_KEYS, n[p.MISSING_CDM] = t.MISSING_CDM_KEYS, n[p.UNSUPPORTED_BROWSER] = t.UNSUPPORTED_BROWSER_KEYS, n[p.UNSUPPORTED_OS] = t.UNSUPPORTED_OS_KEYS, n), t)
            }(h);
        t.exports = f
    }, {
        113: 113,
        272: 272,
        32: 32,
        49: 49,
        93: 93
    }],
    24: [function(e, t, i) {
        "use strict";

        function n(e) {
            var t = e.length,
                i = l.type(e);
            return !l.isWindow(e) && (!(1 !== e.nodeType || !t) || "array" === i || "function" !== i && (0 === t || "number" == typeof t && t > 0 && t - 1 in e))
        }

        function r(e) {
            for (var t = [], i = 1; i < arguments.length; i++) t[i - 1] = arguments[i];
            var n, o, s, a, c, u, p = arguments[0] || {},
                h = 1,
                f = arguments.length,
                d = !1;
            for ("boolean" == typeof p && (d = p, p = arguments[1] || {}, h = 2), "object" == typeof p || l.isFunction(p) || (p = {}), f === h && (p = this, --h); h < f; h++)
                if (null !== (c = arguments[h]))
                    for (a in c)
                        if (c.hasOwnProperty(a)) {
                            if (n = p[a], s = c[a], p === s) continue;
                            d && s && (l.isPlainObject(s) || (o = l.isArray(s))) ? (o ? (o = !1, u = n && l.isArray(n) ? n : []) : u = n && l.isPlainObject(n) ? n : {}, p[a] = r(d, u, s)) : void 0 !== s && (p[a] = s)
                        } return p
        }
        var o = {},
            s = o.toString,
            a = o.hasOwnProperty,
            l = {
                isFunction: function(e) {
                    return "function" === l.type(e)
                },
                isArray: Array.isArray || function(e) {
                    return "array" === l.type(e)
                },
                isWindow: function(e) {
                    return null !== e && e === e.window
                },
                isNumeric: function(e) {
                    return !isNaN(parseFloat(e)) && isFinite(e)
                },
                type: function(e) {
                    return null === e ? String(e) : "object" == typeof e || "function" == typeof e ? o[s.call(e)] || "object" : typeof e
                },
                isPlainObject: function(e) {
                    if (!e || "object" !== l.type(e) || e.nodeType || l.isWindow(e)) return !1;
                    try {
                        if (e.constructor && !a.call(e, "constructor") && !a.call(e.constructor.prototype, "isPrototypeOf")) return !1
                    } catch (e) {
                        return !1
                    }
                    var t;
                    for (t in e) e.hasOwnProperty(t);
                    return void 0 === t || a.call(e, t)
                },
                each: function(e, t) {
                    var i, r, o = e.length,
                        s = n(e);
                    if (s)
                        for (r = 0; r < o && (i = t.call(e[r], r, e[r]), i !== !1); r++);
                    else
                        for (r in e)
                            if (e.hasOwnProperty(r) && (i = t.call(e[r], r, e[r]), i === !1)) break;
                    return e
                }
            };
        l.each("Boolean Number String Function Array Date RegExp Object Error".split(" "), function(e, t) {
            o["[object " + t + "]"] = t.toLowerCase()
        }), t.exports = r
    }, {}],
    25: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.toString = function() {
                return "FACTORY_NOTIFICATION_EVENT"
            }, e)
        }();
        t.exports = n
    }, {}],
    26: [function(e, t, i) {
        "use strict";
        var n, r, o, s, a, l, c, u, p, h, f, d = e(272),
            y = d.BrowserType,
            g = d.OperatingSystemFamily,
            m = d.WindowsVersion,
            v = e(93),
            E = e(113),
            _ = E.Version,
            S = e(4),
            T = e(49),
            b = e(284),
            I = e(294),
            w = (b.get("HTML5DeviceInformationBlacklist"), "2c3c3654-ac62-46a9-a36e-76e1ff64a679"),
            R = (n = {}, n[y.Chrome] = (r = {}, r["DRM:YES"] = [{
                browserRange: {
                    min: _.fromNumber(69)
                },
                osFamily: g.Windows,
                osVersion: {
                    min: m.WINDOWS_7
                }
            }, {
                browserRange: {
                    min: _.fromNumber(69)
                },
                osFamily: g.Android,
                osVersion: {
                    min: _.fromString("4.4.3")
                }
            }], r[w + "|DRM:NO"] = [{
                browserRange: {
                    min: _.fromNumber(42)
                }
            }], r), n[y.Chromium] = (o = {}, o["DRM:YES"] = [{
                browserRange: {
                    min: _.fromNumber(69)
                },
                osFamily: g.Windows,
                osVersion: {
                    min: m.WINDOWS_7
                }
            }, {
                browserRange: {
                    min: _.fromNumber(69)
                },
                osFamily: g.Android,
                osVersion: {
                    min: _.fromString("4.4.3")
                }
            }], o[w + "|DRM:NO"] = [{
                browserRange: {
                    min: _.fromNumber(42)
                }
            }], o), n[y.Firefox] = (s = {}, s["DRM:YES"] = [{
                browserRange: {
                    min: _.fromNumber(60)
                },
                osFamily: g.Windows,
                osVersion: {
                    min: m.WINDOWS_7
                }
            }, {
                browserRange: {
                    min: _.fromNumber(60)
                },
                osFamily: g.Android,
                osVersion: {
                    min: _.fromString("4.4.3")
                }
            }], s[w + "|DRM:NO"] = [{
                browserRange: {
                    min: _.fromNumber(53)
                },
                osFamily: g.Android,
                osVersion: {
                    min: _.fromString("4.4.3")
                }
            }, {
                browserRange: {
                    min: _.fromNumber(42)
                }
            }], s), n[y.Firefox64] = (a = {}, a["DRM:YES"] = [{
                browserRange: {
                    min: _.fromNumber(60)
                },
                osFamily: g.Windows,
                osVersion: {
                    min: m.WINDOWS_7
                }
            }, {
                browserRange: {
                    min: _.fromNumber(60)
                },
                osFamily: g.Android,
                osVersion: {
                    min: _.fromString("4.4.3")
                }
            }], a[w + "|DRM:NO"] = [{
                browserRange: {
                    min: _.fromNumber(53)
                },
                osFamily: g.Android,
                osVersion: {
                    min: _.fromString("4.4.3")
                }
            }, {
                browserRange: {
                    min: _.fromNumber(42)
                }
            }], a), n[y.Opera] = (l = {}, l["DRM:YES"] = [{
                browserRange: {
                    min: _.fromNumber(37)
                },
                osFamily: g.Windows,
                osVersion: {
                    min: m.WINDOWS_7
                }
            }, {
                browserRange: {
                    min: _.fromNumber(37)
                },
                osFamily: g.Android,
                osVersion: {
                    min: _.fromString("4.4.3")
                }
            }], l[w + "|DRM:NO"] = [{
                browserRange: {
                    min: _.fromNumber(32)
                }
            }], l), n[y.IE] = (c = {}, c["DRM:YES"] = [{
                browserRange: {
                    min: _.fromNumber(11)
                }
            }], c), n[y.Safari] = (u = {}, u["DRM:YES"] = [{
                browserRange: {
                    min: _.fromNumber(10)
                },
                osFamily: g.Mac,
                osVersion: {
                    min: _.fromString("10.12.1")
                }
            }], u["DRM:NO"] = [{
                browserRange: {
                    min: _.fromNumber(8)
                }
            }], u), n[y.Silk] = (p = {}, p["DRM:YES"] = [{
                browserRange: !1
            }], p), n[y.AmazonWebView] = {}, n[y.Edge] = (h = {}, h["DRM:YES"] = [{
                browserRange: {
                    min: _.fromNumber(15)
                }
            }], h["DRM:NO"] = [{
                browserRange: {
                    min: _.fromNumber(15)
                }
            }], h), n[y.EdgeNext] = (f = {}, f["DRM:YES"] = [{
                browserRange: {
                    min: _.fromNumber(64)
                }
            }], f["DRM:NO"] = [{
                browserRange: {
                    min: _.fromNumber(64)
                }
            }], f), n),
            C = function() {
                function e(e) {
                    this.deviceInformation = e
                }
                return (e.prototype.getSupport = function(e, t, i, n) {
                    var r = this.deviceInformation.browser,
                        o = this.deviceInformation.operatingSystem,
                        s = r.getType(),
                        a = {
                            supportsWebPlayback: !0,
                            supportsLive: !0,
                            supportsDrm: !0
                        };
                    if (!v.exists(R[s])) return (a.supportsWebPlayback = !1, a.note = S.UNSUPPORTED_BROWSER, a.notSupportedReason = T.UNSUPPORTED_BROWSER, a);
                    var l = this.getBlacklistEntry(s, o, t, n);
                    if (v.exists(l)) {
                        if (this.isBlacklistedOs(l, o)) return (a.supportsWebPlayback = !1, a.notSupportedReason = T.UNSUPPORTED_OS, a.note = S.UNSUPPORTED_OS, a);
                        if (this.isBlacklistedBrowser(l, r)) return (a.supportsWebPlayback = !1, a.notSupportedReason = T.BROWSER_OUT_OF_DATE, a.note = S.UNSUPPORTED_BROWSER, a)
                    }
                    return a
                }, e.prototype.isInRange = function(e, t) {
                    return !!(v.exists(t.min) && t.min.compareTo(e) > 0) || !!(v.exists(t.max) && t.max.compareTo(e) < 0)
                }, e.prototype.isBlacklistedOs = function(e, t) {
                    return !!v.exists(e.osFamily) && (e.osVersion === !1 || this.isInRange(t.getVersion(), e.osVersion))
                }, e.prototype.isBlacklistedBrowser = function(e, t) {
                    return e.browserRange === !1 || this.isInRange(t.getVersion(), e.browserRange)
                }, e.prototype.keyString = function(e) {
                    return (v.exists(e.clientId) ? e.clientId + "|" : "") + ("DRM:" + (e.requiresDrm ? "YES" : "NO"))
                }, e.prototype.getFirst = function(e) {
                    for (var t = [], i = 1; i < arguments.length; i++) t[i - 1] = arguments[i];
                    for (var n = 0, r = t; n < r.length; n++) {
                        var o = r[n];
                        if (v.exists(e[o])) return e[o]
                    }
                    return null
                }, e.prototype.getBlacklistEntry = function(e, t, i, n) {
                    if (!v.exists(R[e])) return null;
                    var r = R[e],
                        o = this.getFirst(r, this.keyString({
                            clientId: n,
                            requiresDrm: i
                        }), this.keyString({
                            clientId: n,
                            requiresDrm: !0
                        }), this.keyString({
                            requiresDrm: i
                        }), this.keyString({
                            requiresDrm: !0
                        })),
                        s = I.max(o, function(e) {
                            return e.osFamily === t.getFamily() ? 2 : v.exists(e.osFamily) ? -(1 / 0) : 1
                        });
                    return I.isObject(s) ? s : null
                }, e)
            }();
        t.exports = C
    }, {
        113: 113,
        272: 272,
        284: 284,
        294: 294,
        4: 4,
        49: 49,
        93: 93
    }],
    27: [function(e, t, i) {
        "use strict";
        var n = e(126),
            r = e(232),
            o = e(272),
            s = o.BrowserType,
            a = o.OperatingSystemFamily,
            l = o.WindowsVersion,
            c = e(80),
            u = c.Weblabs,
            p = c.WeblabTreatment,
            h = e(82),
            f = h.BasicEventEmitter,
            d = e(93),
            y = e(281),
            g = e(284),
            m = e(294),
            v = e(113),
            E = v.LocalStorage,
            _ = v.Version,
            S = e(4),
            T = e(28),
            b = e(49),
            I = e(54),
            w = e(48),
            R = e(26),
            C = y.of("HTML5WebPlayerFactory"),
            P = g.get("HTML5WebPlayerFactory"),
            A = "xpBeta",
            O = ["f22dbddb-ef2c-48c5-8876-bed0d47594fd", "898eb3b8-96c0-4c6b-baa3-2d253860be8c", "98d383c3-632a-4872-8ef8-7cc10d033066", "5ba3a8dc-e147-4755-8617-0af2d89944ef", "9ee9ba9b-f89c-41bb-8aa0-4db6e7f24cad"],
            N = ["367349ae-26ce-4e87-8324-99c337ebac9b", "511cc9af-3ecb-48fe-9825-886ebfbb6503"],
            x = function() {
                function e(e, t, i, o, s, a, l, c, u, p, h) {
                    void 0 === i && (i = !1), void 0 === o && (o = new E),
                        void 0 === s && (s = navigator), void 0 === a && (a = n.getInstance()), void 0 === l && (l = T), void 0 === c && (c = I), void 0 === u && (u = R), void 0 === p && (p = new f), void 0 === h && (h = r), this.loaderConfig = C(e, "loaderConfig").exists(), this.qosReporter = C(t, "qosReporter").exists(), this.playerBeta = i, this.storage = o, this.navigator = s, this.deviceInformation = a, this.html5LoaderConfig = e.dependencies.ATVWebCascadesPlayer, this.html5WebPlayerLoader = new l(this.loaderConfig, this.qosReporter), this.wrappedXpPlayerLoader = new c(this.qosReporter), this.deviceInformationBlacklist = new u(a), this.eventEmitter = p, this.queryString = h
                }
                return (e.prototype.initialize = function(e, t, i, n, r) {
                    this.cacheOverride = i, this.clientId = r, this.languageCode = n, this.marketplaceId = e, this.weblabs = t
                }, e.prototype.getPlayerName = function() {
                    return e.PLAYER_NAME
                }, e.prototype.getPlayerVersion = function() {
                    return this.loaderConfig.dependencies.ATVWebCascadesPlayer.version
                }, e.prototype.getMetricsFeature = function() {
                    return e.METRICS_FEATURE
                }, e.prototype.getDeviceTypeId = function() {
                    return c.DeviceTypeId.HTML5
                }, e.prototype.getPlayerCapabilities = function(t, i) {
                    void 0 === i && (i = {});
                    var n = this.deviceInformation.browser,
                        r = this.deviceInformation.operatingSystem,
                        o = r.getVersion(),
                        c = n.getType(),
                        u = n.getVersion(),
                        p = {
                            supportsDrm: !1,
                            supportsNativePlayback: !1,
                            supportsWebPlayback: !1,
                            supportsAVOD: !1,
                            supportsMidStreamMTA: !1,
                            supportsHDMovies: m.contains(e.HD_MOVIE_SUPPORTED_OSES, r.getFamily()),
                            supportsHDTVShows: !0,
                            supportsLive: !0
                        },
                        h = d.withDefault(i.supportsDrm, !0),
                        f = this.deviceInformationBlacklist.getSupport(this.weblabs, h, d.withDefault(i.supportsLive, !1), this.clientId);
                    if (p.supportsDrm = f.supportsDrm, p.supportsLive = f.supportsLive, !f.supportsWebPlayback) return void t(p, f.note, f.notSupportedReason);
                    var y = S.UNSUPPORTED_BROWSER,
                        g = b.UNSUPPORTED_BROWSER,
                        v = this.clientId === e.TNF_GATEWAY_CLIENT_ID,
                        E = !1;
                    if (v && (c === s.Safari && (E = !0), this.isFirefoxBrowser(c) && (r.getFamily() === a.Mac && o.compareTo(_.fromString("10.12")) >= 0 && o.compareTo(_.fromString("10.14")) < 0 && u.compareTo(_.fromNumber(60)) >= 0 && u.compareTo(_.fromNumber(62)) < 0 && (E = !0), r.getFamily() === a.Windows && (r.getVersion().equals(l.WINDOWS_7) || r.getVersion().equals(l.WINDOWS_10)) && c === s.Firefox64 && (E = !0)), r.getVersion().equals(l.WINDOWS_8_1) && c === s.IE && u.compareTo(_.fromNumber(11)) >= 0 && u.compareTo(_.fromNumber(12)) < 0 && (E = !0), r.getVersion().equals(l.WINDOWS_10) && c === s.Edge && u.compareTo(_.fromNumber(14)) >= 0 && u.compareTo(_.fromNumber(16)) < 0 && (E = !0)), E) return (p.supportsWebPlayback = !1, void t(p, y, g));
                    if ((v || i.supportsLive) && this.isFirefoxBrowser(c) && u.equals(_.fromNumber(55)) && r.getFamily() === a.Mac && o.equals(_.fromString("10.13"))) return (p.supportsWebPlayback = !1, void t(p, y, g));
                    if (v && (!p.supportsLive || !p.supportsDrm)) return (p.supportsWebPlayback = !1, y = S.UNSUPPORTED_BROWSER, g = b.UNSUPPORTED_BROWSER, void t(p, y, g));
                    if (d.isNullOrUndefined(window.MediaSource) && r.getFamily() === a.iOS && !h && this.isHLSCapable()) P.info("HLS non-DRM support detected."), p.supportsWebPlayback = !0, g = null, y = "";
                    else if (d.isNullOrUndefined(window.MediaSource)) p.supportsWebPlayback = !1, g = b.UNSUPPORTED_BROWSER, y = S.MISSING_MEDIA_SOURCE;
                    else if (h) {
                        if (this.isSupportedWebkitBrowser(c)) {
                            if (d.exists(this.navigator.requestMediaKeySystemAccess)) return (P.info("navigator.requestMediaKeySystemAccess support detected."), void this.supportsUnprefixedEme(function(e, i) {
                                y = e ? "" : c === s.EdgeNext ? S.MISSING_PLAYREADY_CDM : S.MISSING_WIDEVINE_CDM, d.exists(i) && (y += ": " + i), p.supportsDrm = e, p.supportsWebPlayback = e, g = e ? null : b.MISSING_CDM, t(p, y, g)
                            }))
                        } else if (c === s.IE || c === s.Edge)
                            if (d.exists(window.MSMediaKeys)) try {
                                window.MSMediaKeys.isTypeSupported(e.PLAY_READY_KEY_SYSTEM, e.VIDEO_CONTENT_TYPE) && window.MSMediaKeys.isTypeSupported(e.PLAY_READY_KEY_SYSTEM, e.AUDIO_CONTENT_TYPE) ? (P.info("PlayReady support detected."), p.supportsWebPlayback = !0, p.supportsDrm = !0, g = null, y = "") : (y = S.MISSING_CODEC_IN_PLAYREADY_CDM, g = b.MISSING_CODEC, P.info("Audio/Video types not supported by MSMediaKeys"))
                            } catch (e) {
                                y = S.MISSING_PLAYREADY_CDM, g = b.MISSING_CDM, P.error("An error occurred when checking MSMediaKeys", e)
                            } else y = S.MISSING_PLAYREADY_CDM, g = b.MISSING_CDM, P.info("No MSMediaKeys object detected. PlayReady not supported");
                            else if (c === s.Safari) this.isFairplaySupported() ? (p.supportsWebPlayback = !0, p.supportsDrm = !0, p.supportsLive = !0, y = "", g = null, P.info("Safari MP2T+MSE support detected.")) : (p.supportsWebPlayback = !1, y = S.MISSING_FAIRPLAY_CDM, g = b.MISSING_CDM, P.info("Detected Safari version " + u.toString() + " on MacOS version " + o.toString() + ", but WebKitMediaKeys.canPlayType returned false"));
                        else if (this.isFirefoxBrowser(c)) {
                            if (d.exists(this.navigator.requestMediaKeySystemAccess)) return (P.info("navigator.requestMediaKeySystemAccess support detected."), void this.supportsUnprefixedEme(function(e, i) {
                                y = e ? "" : S.MISSING_WIDEVINE_CDM, d.exists(i) && (y += ": " + i), p.supportsDrm = e, p.supportsWebPlayback = e, g = e ? null : b.MISSING_CDM, t(p, y, g)
                            }));
                            p.supportsWebPlayback = !1, y = S.UNSUPPORTED_BROWSER, g = b.UNSUPPORTED_BROWSER, P.info("Firefox not supported because DRM is required.")
                        }
                    } else {
                        P.info("No DRM required, not checking DRM support.");
                        var T = MediaSource.isTypeSupported(e.VIDEO_CONTENT_TYPE),
                            I = MediaSource.isTypeSupported(e.AUDIO_CONTENT_TYPE);
                        P.info('"' + e.VIDEO_CONTENT_TYPE + '" supported: ' + T), P.info('"' + e.AUDIO_CONTENT_TYPE + '" supported: ' + I);
                        var w = I && T;
                        p.supportsWebPlayback = w, g = w ? null : b.MISSING_CODEC, y = ""
                    }
                    t(p, y, g)
                }, e.prototype.createWebPlayer = function(e, t) {
                    this.supportsWrappedXpPlayer() ? this.wrappedXpPlayerLoader.createWebPlayer(this.cacheOverride, this.playerBeta, e, t) : this.html5WebPlayerLoader.createWebPlayer(this.cacheOverride, e, t)
                }, e.prototype.on = function(e, t) {
                    this.eventEmitter.on(e, t)
                }, e.prototype.once = function(e, t) {
                    this.eventEmitter.once(e, t)
                }, e.prototype.off = function(e, t) {
                    this.eventEmitter.off(e, t)
                }, e.prototype.supportsWrappedXpPlayer = function() {
                    var e = !1;
                    e = this.weblabs[u.HASTUR_ALL_CLIENT_IDS] === p.T1 ? !m.contains(N, this.clientId) : m.contains(O, this.clientId);
                    var t = this.queryString.getParameter(A);
                    return t === !0 || this.browserSupportsWrappedXpPlayer() && this.osSupportsWrappedXpPlayer() && e
                }, e.prototype.browserSupportsWrappedXpPlayer = function() {
                    var e = this.deviceInformation.browser.getType();
                    switch (e) {
                        case s.AmazonWebView:
                        case s.Chrome:
                        case s.EdgeNext:
                        case s.Firefox:
                        case s.Firefox64:
                        case s.Opera:
                            return !0;
                        case s.Chromium:
                            return this.weblabs[u.HASTUR_CHROMIUM] === p.T1;
                        case s.Edge:
                            return this.weblabs[u.HASTUR_EDGE] === p.T1;
                        case s.IE:
                            return this.weblabs[u.HASTUR_IE] === p.T1;
                        case s.Safari:
                            return this.weblabs[u.HASTUR_SAFARI] === p.T1;
                        default:
                            return !1
                    }
                }, e.prototype.osSupportsWrappedXpPlayer = function() {
                    var e = this.deviceInformation.operatingSystem.getFamily();
                    switch (e) {
                        case a.ChromeOS:
                            return this.weblabs[u.HASTUR_CHROMEOS] === p.T1;
                        case a.Linux:
                            return this.weblabs[u.HASTUR_LINUX] === p.T1;
                        default:
                            return !0
                    }
                }, e.prototype.isFirefoxBrowser = function(e) {
                    return e === s.Firefox || e === s.Firefox64
                }, e.prototype.isFairplaySupported = function() {
                    return "undefined" != typeof window.WebKitMediaKeys && window.WebKitMediaKeys.isTypeSupported(e.SAFARI_KEY_SYSTEM, e.MP2TS_CONTENT_TYPE)
                }, e.prototype.isSupportedWebkitBrowser = function(e) {
                    var t = this.weblabs[u.HTML5_ENABLE_CHROMIUM] === p.T1;
                    return e === s.EdgeNext || e === s.Chrome || e === s.Silk || e === s.Opera || e === s.AmazonWebView || e === s.Chromium && t
                }, e.prototype.isHLSCapable = function() {
                    var t = document.createElement("video");
                    return d.exists(t.canPlayType) && "" !== t.canPlayType(e.HLS_CONTENT_TYPE)
                }, e.prototype.supportsUnprefixedEme = function(t) {
                    var i = this,
                        n = new w,
                        r = n.getDrmSystems(),
                        o = function(n, r) {
                            return i.navigator.requestMediaKeySystemAccess(n.keySystem, r).then(function() {
                                P.info(n.keySystem + ", " + e.INIT_DATA_TYPE + ', "' + e.VIDEO_CONTENT_TYPE + '", and "' + e.AUDIO_CONTENT_TYPE + '" support detected.'), t(!0)
                            })
                        };
                    r.reduce(function(e, t) {
                        return t.configurations.reduce(function(e, i) {
                            return d.exists(e) ? e["catch"](function(e) {
                                return o(t, i)
                            }) : o(t, i)
                        }, e)
                    }, null)["catch"](function(i) {
                        P.info("No supported keySystem, " + e.INIT_DATA_TYPE + ', "' + e.VIDEO_CONTENT_TYPE + '", and/or "' + e.AUDIO_CONTENT_TYPE + '" support detected.'), t(!1, i)
                    })
                }, e.HTML5_NOT_USED_ERROR_NAME = "HTML5PlayerNotUsedWarning", e.PLAYER_NAME = "html5", e.TNF_GATEWAY_CLIENT_ID = "511cc9af-3ecb-48fe-9825-886ebfbb6503", e.VIDEO_CONTENT_TYPE = 'video/mp4; codecs="avc1.640028"', e.AUDIO_CONTENT_TYPE = 'audio/mp4; codecs="mp4a.40.2"', e.HLS_CONTENT_TYPE = "application/vnd.apple.mpegURL", e.MP2TS_CONTENT_TYPE = "video/mp2t", e.INIT_DATA_TYPE = "cenc", e.PLAY_READY_KEY_SYSTEM = "com.microsoft.playready", e.SAFARI_KEY_SYSTEM = "com.apple.fps.2_0", e.METRICS_FEATURE = "HTML5Player", e.HD_MOVIE_SUPPORTED_OSES = [a.Windows, a.WindowsPhone, a.ChromeOS, a.Mac], e)
            }();
        t.exports = x
    }, {
        113: 113,
        126: 126,
        232: 232,
        26: 26,
        272: 272,
        28: 28,
        281: 281,
        284: 284,
        294: 294,
        4: 4,
        48: 48,
        49: 49,
        54: 54,
        80: 80,
        82: 82,
        93: 93
    }],
    28: [function(e, t, i) {
        "use strict";
        var n = e(231),
            r = n.ErrorSeverity,
            o = e(187),
            s = e(281),
            a = e(284),
            l = e(294),
            c = e(250),
            u = c.JavaScriptLoader,
            p = c.LazyCachingScriptLoader,
            h = c.TransparentCachingScriptLoader,
            f = c.ScriptType,
            d = s.of("HTML5WebPlayerLoader"),
            y = a.get("HTML5WebPlayerLoader"),
            g = "HTML5JavaScriptDownloadTime",
            m = "HTML5JavaScriptDownloadError",
            v = "HTML5JavaScriptCacheHit",
            E = "HTML5JavaScriptCacheMiss",
            _ = "HTML5JavaScriptDownloadInfo",
            S = function() {
                function e(e, t, i, n, r) {
                    void 0 === i && (i = new u), void 0 === n && (n = new p), void 0 === r && (r = new h), this.loaderConfig = d(e, "loaderConfig").exists(), this.qosReporter = d(t, "qosReporter").exists(), this.javaScriptLoader = i, this.lazyScriptLoader = n, this.transparentScriptLoader = r, this.html5LoaderConfig = e.dependencies.ATVWebCascadesPlayer
                }
                return (e.prototype.createWebPlayer = function(e, t, i) {
                    var n, s = this,
                        a = function(e) {
                            var t = e && l.isFunction(e.toString) ? e.toString() : "Unknown Error";
                            i(m, t, r.Fatal)
                        },
                        c = this.loaderConfig.assetsRoot + "/" + this.html5LoaderConfig.name + "/" + this.html5LoaderConfig.version + "/js/" + this.html5LoaderConfig.name + ".js";
                    n = l.isBoolean(e) && !e ? this.javaScriptLoader : this.html5LoaderConfig.forceUpdate ? this.transparentScriptLoader : this.lazyScriptLoader;
                    var u = c;
                    if (n) {
                        var p = {
                            crc32: this.html5LoaderConfig.crc32,
                            url: c,
                            name: this.html5LoaderConfig.name,
                            type: f.JAVASCRIPT,
                            version: this.html5LoaderConfig.version
                        };
                        n.get(p, function(e) {
                            u = JSON.stringify(e), o.putMetric(g, e.totalMs, "ms"), o.putMetric(e.cached ? v : E, 1), s.qosReporter.reportInformation(_, u, e.totalMs);
                            var i;
                            try {
                                i = new window.ATVWebCascadesPlayer
                            } catch (e) {
                                return void a(e)
                            }
                            t(i)
                        }, function(e, t) {
                            var i = e && l.isFunction(e.toString) ? e.toString() : "Unknown Error",
                                n = JSON.stringify(t);
                            y.error(i), s.qosReporter.reportError(m, r.NonFatal, i, n)
                        }, a)
                    }
                }, e)
            }();
        t.exports = S
    }, {
        187: 187,
        231: 231,
        250: 250,
        281: 281,
        284: 284,
        294: 294
    }],
    29: [function(e, t, i) {
        "use strict";
        var n = e(231),
            r = n.NotificationEvent,
            o = e(113),
            s = o.LocalStorage,
            a = function() {
                function e(e, t) {
                    void 0 === t && (t = new s);
                    var i = this;
                    this.getTemplate = function() {
                        var e = i.browserUpdateKey;
                        return {
                            renderString: function(t) {
                                return t[e]
                            }
                        }
                    }, this.browserUpdateKey = e, this.storage = t
                }
                return (e.prototype.createNotification = function() {
                    var e = this;
                    return new r(this.getTemplate(), null, !0, null, function() {
                        return e.setDontShowAgain()
                    })
                }, e.prototype.setDontShowAgain = function() {
                    this.storage.set(e.DONT_SHOW_AGAIN_KEY, "true")
                }, e.DONT_SHOW_AGAIN_KEY = "dont_show_update_browser", e)
            }();
        t.exports = a
    }, {
        113: 113,
        231: 231
    }],
    30: [function(e, t, i) {
        "use strict";
        var n = e(113),
            r = n.HTMLElementUtils,
            o = e(281),
            s = o.of("ErrorDisplayManager"),
            a = function() {
                function e(e) {
                    void 0 === e && (e = document), this.parent = null, this.localizationStore = null, this.amazonUrlCreator = null, this.onClosePlayerClicked = null, this.documentRef = e
                }
                return (e.prototype.initialize = function(e, t, i, n) {
                    this.parent = s(e, "parent").exists(), this.localizationStore = s(t, "localizationStore").exists(), this.amazonUrlCreator = s(i, "amazonUrlCreator").exists(), this.onClosePlayerClicked = s(n, "onClosePlayerClicked").is.a["function"]()
                }, e.prototype.showError = function(t, i) {
                    void 0 === i && (i = !0), s(this.parent, "this.parent").exists(), s(t, "template").exists();
                    var n = this.documentRef.createElement("div");
                    n.style.height = "100%", n.style.width = "100%", n.innerHTML = t.renderString(this.localizationStore.getLocalization(), this.amazonUrlCreator), this.parent.appendChild(n);
                    for (var o = r.getElementsByClassName(n, e.CLOSE_PLAYER_BUTTON_CLASS), a = 0; a < o.length; a++) i ? o[a].onclick = this.onClosePlayerClicked : o[a].style.display = "none"
                }, e.CLOSE_PLAYER_BUTTON_CLASS = "closePlayerButton", e)
            }();
        t.exports = a
    }, {
        113: 113,
        281: 281
    }],
    31: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(93),
            o = e(287),
            s = n.of("ModalTemplate"),
            a = function() {
                function e(e) {
                    this.mustacheTemplate = '<div class="table"><div class="tableCell"><div class="modal">{{>contents}}</div></div></div>', this.key = s(e, "key").is.a.string()
                }
                return (e.prototype.renderString = function(e, t, i) {
                    s(e, "localization").exists(), r.isNullOrUndefined(i) && (i = {
                        contents: "{{{description}}}"
                    });
                    var n = {
                            contactUsDigitalUrl: t.createCSUrl("CONTACT_US_DIGITAL"),
                            contactUsOrdersUrl: t.createCSUrl("CONTACT_US_ORDERS"),
                            devicesAndFeatures: t.createCSUrl("DEVICES_AND_FEATURES"),
                            systemRequirementsUrl: t.createCSUrl("SYSTEM_REQUIREMENTS"),
                            avSettingsUrl: t.createCSUrl("AV_SETTINGS"),
                            resetPinUrl: t.createCSUrl("RESET_PIN"),
                            setupPinUrl: t.createCSUrl("SETUP_PIN"),
                            videoHelpUrl: t.createCSUrl("VIDEO_HELP"),
                            videoIssuesUrl: t.createCSUrl("VIDEO_ISSUES")
                        },
                        a = e[this.key];
                    return (a.description = o.render(a.description, n), a.notes = o.render(a.notes, n), a.footer = o.render(a.footer, n), a.incorrectPin = o.render(a.incorrectPin, n), a.troubleshoot = o.render(a.troubleshoot, n), o.render(this.mustacheTemplate, a, i))
                }, e)
            }();
        t.exports = a
    }, {
        281: 281,
        287: 287,
        93: 93
    }],
    32: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(93),
            o = e(294),
            s = e(31),
            a = function(e) {
                function t(t) {
                    return e.call(this, t) || this
                }
                return (n(t, e), t.prototype.renderString = function(i, n, s) {
                    var a = {
                        closeButton: '<div class="closePlayerButton closePlayer html5"></div>',
                        contents: t.TEMPLATE,
                        extraText: ""
                    };
                    return (r.isNullOrUndefined(s) || o.extend(a, s), e.prototype.renderString.call(this, i, n, a))
                }, t.TEMPLATE = "<div class='title'>{{{title}}}{{>closeButton}}</div><div class='description'>{{{description}}}{{>extraText}}</div>{{#footer}}<div class='footer'>{{{footer}}}</div>{{/footer}}", t)
            }(s);
        t.exports = a
    }, {
        294: 294,
        31: 31,
        93: 93
    }],
    33: [function(e, t, i) {
        "use strict";
        var n = e(190),
            r = n.NotificationManager,
            o = e(113),
            s = o.HTMLElementUtils,
            a = e(284),
            l = e(287),
            c = e(93),
            u = e(281),
            p = u.of("NotificationDisplayManager"),
            h = a.get("NotificationDisplayManager"),
            f = function() {
                function e(e, t, i) {
                    void 0 === e && (e = new r), void 0 === t && (t = window), void 0 === i && (i = document);
                    var n = this;
                    this.localizationStore = null, this.amazonUrlCreator = null, this.notificationContainerElement = null, this.currentNotification = null, this.hidingNotification = !1, this.notificationHiddenCallback = null, this.onWindowResize = function() {
                        n.notificationContainerElement.offsetHeight > 0 && n.setNotificationAndPlayerHeight()
                    }, this.notificationManager = e, this.windowRef = t, this.documentRef = i
                }
                return (e.prototype.initialize = function(e, t, i, n) {
                    var r = this;
                    this.parent = p(e, "parent").exists(), this.playerElement = p(t, "player").exists(), this.localizationStore = p(i, "localizationStore").exists(), this.amazonUrlCreator = p(n, "amazonUrlCreator").exists(), this.notificationManager.initialize(function(e) {
                        return r.hidingNotification ? null !== r.notificationHiddenCallback ? void h.error("Attempted to show a notification while another notification is waiting to be shown") : void(r.notificationHiddenCallback = function() {
                            r.onShowNotification(e)
                        }) : void r.onShowNotification(e)
                    }, function(e) {
                        r.onCloseNotification(e)
                    })
                }, e.prototype.addNotification = function(e, t) {
                    void 0 === t && (t = !0), p(e, "event").exists(), this.tryInitializeContainer(), this.notificationManager.addNotification(e, t)
                }, e.prototype.cancelNotification = function(e) {
                    p(e, "event").exists(), this.notificationManager.cancelNotification(e)
                }, e.prototype.showQueuedNotifications = function() {
                    this.notificationManager.hasQueuedNotifications() && (this.setNotificationAndPlayerHeight(), this.notificationManager.showQueuedNotifications())
                }, e.prototype.onShowNotification = function(t) {
                    this.tryInitializeContainer(), this.currentNotification = t;
                    var i = this.localizationStore.getLocalization(),
                        n = {
                            contactUsDigitalUrl: this.amazonUrlCreator.createCSUrl("CONTACT_US_DIGITAL"),
                            contactUsOrdersUrl: this.amazonUrlCreator.createCSUrl("CONTACT_US_ORDERS"),
                            devicesAndFeatures: this.amazonUrlCreator.createCSUrl("DEVICES_AND_FEATURES"),
                            systemRequirementsUrl: this.amazonUrlCreator.createCSUrl("SYSTEM_REQUIREMENTS"),
                            avSettingsUrl: this.amazonUrlCreator.createCSUrl("AV_SETTINGS"),
                            resetPinUrl: this.amazonUrlCreator.createCSUrl("RESET_PIN"),
                            setupPinUrl: this.amazonUrlCreator.createCSUrl("SETUP_PIN"),
                            videoHelpUrl: this.amazonUrlCreator.createCSUrl("VIDEO_HELP"),
                            videoIssuesUrl: this.amazonUrlCreator.createCSUrl("VIDEO_ISSUES")
                        };
                    this.messageTextElement.innerHTML = this.currentNotification.getMessageTemplate().renderString(i, n) + " ", this.expandedMessageElement.innerHTML = "";
                    var r = this.currentNotification.getClosable(),
                        o = c.exists(this.currentNotification.getOnDontShowAgain()),
                        a = !1;
                    c.exists(this.currentNotification.getExpandedMessageTemplate()) && (this.expandedMessageElement.innerHTML = this.currentNotification.getExpandedMessageTemplate().renderString(i, n), a = !0), this.addWindowListeners(), s.addClass(this.playerElement, e.PLAYER_HEIGHT_TRANSITION_CSS_CLASS), this.messageIsExpanded = !1, this.showMoreButton.innerHTML = i.notification.showMore, this.showMoreButton.style.display = a ? "" : "none", this.showMoreDivider.style.display = a && (r || o) ? "" : "none", this.dismissButton.style.display = r ? "" : "none", this.dismissDivider.style.display = r && o ? "" : "none", this.dontShowAgainButton.style.display = o ? "" : "none", this.setNotificationAndPlayerHeight()
                }, e.prototype.onCloseNotification = function(e) {
                    e === this.currentNotification && this.hideNotification()
                }, e.prototype.tryInitializeContainer = function() {
                    var t = this;
                    if (!this.notificationInitialized) {
                        this.messageIsExpanded = !1, this.notificationContainerElement = this.documentRef.createElement("div"), this.notificationContainerElement.className = e.CONTAINER_CSS_CLASS;
                        var i = this.localizationStore.getLocalization(),
                            n = {
                                dismiss: i.notification.dismiss,
                                dismiss_button_css_class: e.DISMISS_BUTTON_CSS_CLASS,
                                dismiss_divider_css_class: e.DISMISS_DIVIDER_CSS_CLASS,
                                dont_show_again: i.notification.dontShowAgain,
                                dont_show_again_css_class: e.DONT_SHOW_AGAIN_BUTTON_CSS_CLASS,
                                expanded_message_css_class: e.EXPANDED_MESSAGE_CSS_CLASS,
                                message_css_class: e.MESSAGE_CSS_CLASS,
                                message_text_css_class: e.MESSAGE_TEXT_CSS_CLASS,
                                show_more: i.notification.showMore,
                                show_more_button_css_class: e.SHOW_MORE_BUTTON_CSS_CLASS,
                                show_more_divider_css_class: e.SHOW_MORE_DIVIDER_CSS_CLASS
                            };
                        this.notificationContainerElement.innerHTML = l.render(e.MUSTACHE_TEMPLATE, n), this.parent.insertBefore(this.notificationContainerElement, this.parent.firstChild), this.messageElement = s.getElementsByClassName(this.notificationContainerElement, e.MESSAGE_CSS_CLASS)[0], this.messageTextElement = s.getElementsByClassName(this.notificationContainerElement, e.MESSAGE_TEXT_CSS_CLASS)[0], this.expandedMessageElement = s.getElementsByClassName(this.notificationContainerElement, e.EXPANDED_MESSAGE_CSS_CLASS)[0], this.showMoreButton = s.getElementsByClassName(this.notificationContainerElement, e.SHOW_MORE_BUTTON_CSS_CLASS)[0], this.showMoreButton.onclick = function() {
                            t.toggleExpandedMessage()
                        }, this.dismissButton = s.getElementsByClassName(this.notificationContainerElement, e.DISMISS_BUTTON_CSS_CLASS)[0], this.dismissButton.onclick = function() {
                            t.cancelNotification(t.currentNotification)
                        }, this.dontShowAgainButton = s.getElementsByClassName(this.notificationContainerElement, e.DONT_SHOW_AGAIN_BUTTON_CSS_CLASS)[0], this.dontShowAgainButton.onclick = function() {
                            if (c.exists(t.currentNotification)) {
                                var e = t.currentNotification.getOnDontShowAgain();
                                c.exists(e) && e(), t.cancelNotification(t.currentNotification)
                            } else t.hideNotification()
                        }, this.dismissDivider = s.getElementsByClassName(this.notificationContainerElement, e.DISMISS_DIVIDER_CSS_CLASS)[0], this.showMoreDivider = s.getElementsByClassName(this.notificationContainerElement, e.SHOW_MORE_DIVIDER_CSS_CLASS)[0], this.hideNotification(), this.notificationInitialized = !0
                    }
                }, e.prototype.hideNotification = function() {
                    var t = this;
                    return (this.removeWindowListeners(), this.messageIsExpanded = !1, this.notificationContainerElement.style.height = "0px", this.playerElement.style.height = "100%", null === this.currentNotification ? void this.onNotificationHidden() : (this.hidingNotification = !0, void setTimeout(function() {
                        t.onNotificationHidden()
                    }, e.NOTIFICATION_CLOSE_TIME)))
                }, e.prototype.onNotificationHidden = function() {
                    this.hidingNotification = !1, s.removeClass(this.playerElement, e.PLAYER_HEIGHT_TRANSITION_CSS_CLASS), this.expandedMessageElement.innerHTML = "", this.messageTextElement.innerHTML = "", this.showMoreButton.innerHTML = "", this.dismissButton.style.display = "none", this.dismissDivider.style.display = "none", this.dontShowAgainButton.style.display = "none", this.showMoreButton.style.display = "none", this.showMoreDivider.style.display = "none", this.currentNotification = null, null !== this.notificationHiddenCallback && (this.notificationHiddenCallback(), this.notificationHiddenCallback = null)
                }, e.prototype.toggleExpandedMessage = function() {
                    var e = this.localizationStore.getLocalization();
                    this.messageIsExpanded = !this.messageIsExpanded, this.showMoreButton.innerHTML = this.messageIsExpanded ? e.notification.showLess : e.notification.showMore, this.setNotificationAndPlayerHeight()
                }, e.prototype.addWindowListeners = function() {
                    this.windowRef.addEventListener ? this.windowRef.addEventListener("resize", this.onWindowResize, !0) : this.windowRef.attachEvent && this.windowRef.attachEvent("onresize", this.onWindowResize)
                }, e.prototype.removeWindowListeners = function() {
                    this.windowRef.removeEventListener ? this.windowRef.removeEventListener("resize", this.onWindowResize) : this.windowRef.detachEvent && this.windowRef.detachEvent("onresize", this.onWindowResize)
                }, e.prototype.setNotificationAndPlayerHeight = function() {
                    this.messageIsExpanded ? (this.notificationContainerElement.style.height = this.messageElement.offsetHeight + this.expandedMessageElement.offsetHeight + "px", this.playerElement.style.height = this.parent.offsetHeight - this.messageElement.offsetHeight - this.expandedMessageElement.offsetHeight + "px") : (this.notificationContainerElement.style.height = this.messageElement.offsetHeight + "px", this.playerElement.style.height = this.parent.offsetHeight - this.messageElement.offsetHeight + "px")
                }, e.CONTAINER_CSS_CLASS = "notificationContainer", e.DISMISS_BUTTON_CSS_CLASS = "dismissButton", e.DISMISS_DIVIDER_CSS_CLASS = "dismissDivider", e.DONT_SHOW_AGAIN_BUTTON_CSS_CLASS = "dontShowAgainButton", e.EXPANDED_MESSAGE_CSS_CLASS = "notificationExpandedMessage", e.MESSAGE_CSS_CLASS = "notificationMessage", e.MESSAGE_TEXT_CSS_CLASS = "notificationMessageText", e.PLAYER_HEIGHT_TRANSITION_CSS_CLASS = "heightTransition", e.SHOW_MORE_BUTTON_CSS_CLASS = "showMoreButton", e.SHOW_MORE_DIVIDER_CSS_CLASS = "showMoreDivider", e.MUSTACHE_TEMPLATE = "<div class='{{{message_css_class}}}'>\n            <span class='{{{message_text_css_class}}}'></span>\n            <a style='display:none;' class='{{{show_more_button_css_class}}}'>{{{show_more}}}</a>\n            <span style='display:none;' class='{{{show_more_divider_css_class}}}'> | </span>\n            <a style='display:none;' class='{{{dismiss_button_css_class}}}'>{{{dismiss}}}</a>\n            <span style='display:none;' class='{{{dismiss_divider_css_class}}}'> | </span>\n            <a style='display:none;' class='{{{dont_show_again_css_class}}}'>{{{dont_show_again}}}</a>\n        </div>\n        <div class='{{{expanded_message_css_class}}}'></div>", e.NOTIFICATION_CLOSE_TIME = 200, e)
            }();
        t.exports = f
    }, {
        113: 113,
        190: 190,
        281: 281,
        284: 284,
        287: 287,
        93: 93
    }],
    34: [function(e, t, i) {
        "use strict";
        var n = e(113),
            r = n.Version,
            o = e(272),
            s = o.BrowserType,
            a = e(231),
            l = a.PINAcceptedEvent,
            c = a.PINDeclinedEvent,
            u = a.PINErrorEvent,
            p = a.PINRequiredEvent,
            h = e(113),
            f = h.HTMLElementUtils,
            d = e(284),
            y = e(93),
            g = e(281),
            m = e(294),
            v = e(35),
            E = g.of("PINDisplayManager"),
            _ = d.get("PINDisplayManager"),
            S = function() {
                function e(e) {
                    if (y.isNullOrUndefined(e)) {
                        var t = o.getBrowser();
                        this.isIE9 = t.getType() === s.IE && t.getVersion().compareTo(r.fromNumber(10)) < 0
                    } else this.isIE9 = e
                }
                return (e.prototype.initialize = function(e, t, i, n, r, o) {
                    this.webPlayer = E(e, "webPlayer").exists(), this.eventEmitter = E(t, "eventEmitter").exists(), this.parent = E(i, "parent").exists(), this.localizationStore = E(n, "localizationStore").exists(), this.amazonUrlCreator = E(r, "amazonUrlCreator").exists(), this.onClosePlayerClicked = E(o, "onClosePlayerClicked").is.a["function"](), this.attachEventListeners(), this.createDOMElements(), this.validatingPIN = !1
                }, e.prototype.requestPIN = function(t) {
                    var i = this;
                    y.isNullOrUndefined(t) && (t = this.pinLength), E(this.parent, "this.parent").exists(), E(0 !== t, "pinLength !== 0").is["true"]();
                    var n = f.getElementsByClassName(this.pinWrapper, e.PIN_FIELDS_CLASS)[0];
                    n.innerHTML = "", this.pinFields = [], this.pinValues = [];
                    for (var r = 0; r < t; r++) {
                        var o = document.createElement("input");
                        o.type = "text", o.maxLength = 1, o.className = e.PIN_FIELD_CLASS, this.isIE9 && (o.className += " ie9"), n.appendChild(o), this.pinFields.push(o), o.onkeydown = function(e) {
                            i.onKeyDown(e)
                        }, o.onkeyup = function() {
                            i.onKeyUp()
                        }, this.pinValues.push("")
                    }
                    0 === f.getElementsByClassName(this.parent, e.PIN_WRAPPER_CLASS).length && this.parent.appendChild(this.pinWrapper), this.pinWrapper.style.display = "block", this.pinFields[0].focus()
                }, e.prototype.hideModal = function() {
                    this.toggleVisibility(e.INCORRECT_PIN_TEXT_CLASS, !1), this.pinWrapper.style.display = ""
                }, e.prototype.onPINRequired = function(e) {
                    this.pinLength = e.getPINLength()
                }, e.prototype.onPINAccepted = function() {
                    _.info("PIN on playback was accepted."), this.validatingPIN = !1, this.toggleVisibility(e.INCORRECT_PIN_TEXT_CLASS, !1), this.hideModal()
                }, e.prototype.onPINDeclined = function() {
                    _.info("PIN on playback was declined."), this.validatingPIN = !1, this.toggleVisibility(e.INCORRECT_PIN_TEXT_CLASS, !0);
                    for (var t = 0; t < this.pinFields.length; t++) this.pinFields[t].value = "", this.pinValues[t] = "";
                    this.pinFields[0].focus()
                }, e.prototype.onPINError = function() {
                    _.error("Error returned by ValidatePin API."), this.validatingPIN = !1, this.toggleVisibility(e.INCORRECT_PIN_TEXT_CLASS, !1), this.hideModal()
                }, e.prototype.toggleVisibility = function(e, t) {
                    var i = f.getElementsByClassName(this.pinWrapper, e)[0];
                    i.style.display = t ? "block" : "none"
                }, e.prototype.keyCodeIsNumber = function(t) {
                    return t >= e.START_OF_NUMBER_KEYCODES && t <= e.END_OF_NUMBER_KEYCODES || t >= e.START_OF_NUMPAD_KEYCODES && t <= e.END_OF_NUMPAD_KEYCODES
                }, e.prototype.pinIsEntered = function() {
                    for (var e = 0; e < this.pinValues.length; e++)
                        if ("" === this.pinValues[e]) return !1;
                    return !0
                }, e.prototype.onKeyDown = function(t) {
                    t = t || event;
                    var i = t.target || t.srcElement,
                        n = m.indexOf(this.pinFields, i),
                        r = window.event ? t.keyCode : t.which;
                    if (r === e.DELETE_KEYCODE || r === e.BACKSPACE_KEYCODE) {
                        this.preventDefault(t);
                        var o = n;
                        n > 0 && "" === this.pinFields[n].value && (o = n - 1), this.pinFields[o].focus(), this.pinFields[o].value = "", this.pinValues[o] = ""
                    } else this.keyCodeIsNumber(r) ? (this.preventDefault(t), r > e.END_OF_NUMBER_KEYCODES && (r -= e.START_OF_NUMBER_KEYCODES), this.pinFields[n].value = "\u2022", this.pinValues[n] = String.fromCharCode(r), n !== this.pinFields.length - 1 && this.pinFields[n + 1].focus()) : r === e.ESCAPE_KEYCODE ? this.onCancelPIN() : r !== e.TAB_KEYCODE && r !== e.SHIFT_KEYCODE && this.preventDefault(t)
                }, e.prototype.preventDefault = function(e) {
                    y.isNullOrUndefined(e.preventDefault) ? e.returnValue = !1 : e.preventDefault()
                }, e.prototype.onKeyUp = function() {
                    this.onSubmitPIN()
                }, e.prototype.getPINString = function() {
                    for (var e = "", t = 0; t < this.pinValues.length; t++) e += this.pinValues[t];
                    return e
                }, e.prototype.onCancelPIN = function() {
                    _.info("PIN on playback was cancelled."), this.hideModal(), this.onClosePlayerClicked()
                }, e.prototype.onSubmitPIN = function() {
                    this.pinIsEntered() && !this.validatingPIN && (_.info("PIN on playback was submitted."), this.validatingPIN = !0, this.webPlayer.validatePIN(this.getPINString()))
                }, e.prototype.attachEventListeners = function() {
                    var e = this;
                    this.webPlayer.on(l, function() {
                        e.onPINAccepted()
                    }), this.webPlayer.on(c, function() {
                        e.onPINDeclined()
                    }), this.webPlayer.on(u, function() {
                        e.onPINError()
                    }), this.webPlayer.on(p, function(t) {
                        e.onPINRequired(t)
                    })
                }, e.prototype.createDOMElements = function() {
                    var t = this;
                    this.pinModalTemplate = new v(e.LOCALIZATION_KEY), this.pinWrapper = document.createElement("div"), this.pinWrapper.className = e.PIN_WRAPPER_CLASS, this.pinWrapper.style.display = "none", this.pinWrapper.innerHTML = this.pinModalTemplate.renderString(this.localizationStore.getLocalization(), this.amazonUrlCreator);
                    for (var i = f.getElementsByClassName(this.pinWrapper, e.CLOSE_PLAYER_BUTTON_CLASS), n = 0; n < i.length; n++) i[n].onclick = function() {
                        t.onCancelPIN()
                    }
                }, e.LOCALIZATION_KEY = "pinModal", e.CLOSE_PLAYER_BUTTON_CLASS = "closePlayerButton", e.INCORRECT_PIN_TEXT_CLASS = "incorrectPINText", e.PIN_FIELD_CLASS = "pinField", e.PIN_FIELDS_CLASS = "pinFields", e.PIN_WRAPPER_CLASS = "pinWrapper", e.START_OF_NUMBER_KEYCODES = 48, e.END_OF_NUMBER_KEYCODES = 57, e.START_OF_NUMPAD_KEYCODES = 96, e.END_OF_NUMPAD_KEYCODES = 105, e.BACKSPACE_KEYCODE = 8, e.DELETE_KEYCODE = 46, e.ESCAPE_KEYCODE = 27, e.SHIFT_KEYCODE = 16, e.TAB_KEYCODE = 9, e)
            }();
        t.exports = S
    }, {
        113: 113,
        231: 231,
        272: 272,
        281: 281,
        284: 284,
        294: 294,
        35: 35,
        93: 93
    }],
    35: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(32),
            o = function(e) {
                function t(t) {
                    return e.call(this, t) || this
                }
                return (n(t, e), t.prototype.renderString = function(i, n) {
                    return e.prototype.renderString.call(this, i, n, {
                        extraText: t.PIN_TEMPLATE
                    })
                }, t.PIN_TEMPLATE = "<div class='pinFields'></div><div class='incorrectPINText'>{{{incorrectPin}}}</div><div class='notes'>{{{notes}}}</div>", t)
            }(r);
        t.exports = o
    }, {
        32: 32
    }],
    36: [function(e, t, i) {
        "use strict";
        var n = e(284),
            r = e(93),
            o = e(281),
            s = e(294),
            a = e(80),
            l = a.MarketplaceID,
            c = e(58),
            u = c.TokenType,
            p = e(178),
            h = p.HttpRequest,
            f = e(231),
            d = f.ErrorSeverity,
            y = o.of("QosReporter"),
            g = n.get("QosReporter"),
            m = function() {
                function e(e) {
                    void 0 === e && (e = h), this.initialized = !1, this.eventQueue = [], this.httpRequest = e
                }
                return (e.prototype.preInitialize = function(e, t, i, n, r, o, s, a, l) {
                    this.partnerServiceUrl = y(e, "partnerServiceUrl").is.a.string(), this.clientVersion = y(t, "version").is.a.string(), this.marketplaceId = y(i, "marketplaceId").is.a.string(), this.deviceTypeId = y(n, "deviceTypeId").is.a.string(), this.deviceId = y(r, "deviceId").is.a.string(), this.primitiveSessionId = y(o, "primitiveSessionId").is.a.string(), this.operatingSystem = y(s, "operatingSystem").is.a.string(), this.browser = y(a, "browser").is.a.string(), this.tokenClient = l
                }, e.prototype.initialize = function(e, t) {
                    this.clientVersion = y(e, "version").is.a.string(), this.deviceTypeId = y(t, "deviceTypeId").is.a.string(), this.initialized = !0, this.tryFlush()
                }, e.prototype.updateSessionIds = function(e, t) {
                    this.userWatchSessionId = e, this.primitiveSessionId = t
                }, e.prototype.setAsin = function(e) {
                    this.asin = e
                }, e.prototype.setTokenClient = function(e) {
                    this.tokenClient = e
                }, e.prototype.reportError = function(t, i, n, r) {
                    var o = {
                        eventType: "Error",
                        eventSubtype: t,
                        errorSeverity: Math.min(5, Math.max(1, i)).toString(),
                        httpCode: n,
                        note: r
                    };
                    this.enqueueEvent(o), i <= e.DO_NOT_QUEUE_SEVERITY && this.flush()
                }, e.prototype.reportInformation = function(e, t, i) {
                    var n = {
                        eventType: "Information",
                        eventSubtype: e,
                        note: t,
                        duration: i
                    };
                    this.enqueueEvent(n)
                }, e.prototype.flush = function() {
                    var e = this;
                    r.isNullOrUndefined(this.partnerServiceUrl) || 0 !== this.eventQueue.length && (r.exists(this.tokenClient) ? this.tokenClient.getToken(function(t) {
                        e.sendEventBatch(t)
                    }, function() {
                        g.warn("Could not get auth token, reporting without token."), e.sendEventBatch(null)
                    }) : this.sendEventBatch(null), clearTimeout(this.flushTimeout), this.flushTimeout = null)
                }, e.prototype.enqueueEvent = function(e) {
                    var t = s.extend({
                        primitiveSessionId: this.primitiveSessionId,
                        streamingSessionId: this.primitiveSessionId,
                        userWatchSessionId: this.userWatchSessionId,
                        asin: this.asin
                    }, e);
                    t = this.removeMissingParameters(t), t.enqueueTime = (new Date).getTime(), this.eventQueue.push(t), this.tryFlush()
                }, e.prototype.tryFlush = function() {
                    var t = this;
                    this.initialized && (this.eventQueue.length >= e.MAX_QUEUE_SIZE ? this.flush() : r.isNullOrUndefined(this.flushTimeout) && (this.flushTimeout = setTimeout(function() {
                        return t.flush()
                    }, e.FLUSH_TIMEOUT_MS)))
                }, e.prototype.sendEventBatch = function(t) {
                    var i = this,
                        n = this.partnerServiceUrl.replace(/\/$/, "") + e.ENDPOINT,
                        o = this.httpRequest.post(n).withCredentials(),
                        a = {
                            gascEnabled: !l.isClassic(this.marketplaceId),
                            firmware: e.FIRMWARE,
                            version: e.VERSION,
                            client_version: this.clientVersion,
                            marketplaceId: this.marketplaceId,
                            deviceTypeID: this.deviceTypeId,
                            sourceSystem: this.deviceTypeId,
                            deviceID: this.deviceId,
                            operatingSystem: this.operatingSystem,
                            browser: this.browser
                        };
                    r.exists(t) && t.type === u.OAUTH && o.bearerToken(t.token), a = this.removeMissingParameters(a);
                    var c = s.map(this.eventQueue, function(e) {
                        return i.prepareEvent(e)
                    });
                    try {
                        o.field("metrics", JSON.stringify(c)).query(a).send()
                    } catch (e) {
                        g.error(e)
                    }
                    this.eventQueue.length = 0
                }, e.prototype.removeMissingParameters = function(e) {
                    var t = s.chain(e).keys().filter(function(t) {
                        return r.isNullOrUndefined(e[t])
                    }).value();
                    return s.omit(e, t)
                }, e.prototype.prepareEvent = function(e) {
                    var t = s.extend({
                        primitiveSessionId: this.primitiveSessionId,
                        streamingSessionId: this.primitiveSessionId,
                        userWatchSessionId: this.userWatchSessionId,
                        asin: this.asin
                    }, e);
                    return (t = this.removeMissingParameters(t), r.isNullOrUndefined(t.enqueueTime) || (t.eventAge = (new Date).getTime() - t.enqueueTime, t = s.omit(t, "enqueueTime")), t)
                }, e.ENDPOINT = "/cdp/usage/ReportEvent", e.FIRMWARE = "1", e.VERSION = "1", e.MAX_QUEUE_SIZE = 5, e.DO_NOT_QUEUE_SEVERITY = d.Fatal, e.FLUSH_TIMEOUT_MS = 1e4, e)
            }();
        t.exports = m
    }, {
        178: 178,
        231: 231,
        281: 281,
        284: 284,
        294: 294,
        58: 58,
        80: 80,
        93: 93
    }],
    37: [function(e, t, i) {
        "use strict";
        var n = e(80),
            r = n.Weblabs,
            o = n.WeblabTreatment,
            s = e(281),
            a = e(272),
            l = a.OperatingSystemFamily,
            c = e(40),
            u = e(38),
            p = e(41),
            h = e(266),
            f = h.SilverlightWebPlayer,
            d = s.of("SilverlightWebPlayerFactory"),
            y = function() {
                function e(e, t, i, n, r) {
                    void 0 === r && (r = new u(e, t, c.createModalTemplate)), this.silverlightDisabled = !1, this.os = d(e, "os").exists(), this.browser = d(t, "browser").exists(), this.loaderConfig = d(i, "loaderConfig").exists(), this.qosReporter = d(n, "qosReporter").exists(), this.playerOption = r
                }
                return (e.prototype.initialize = function(e, t, i, n, s) {
                    this.silverlightDisabled = t[r.DISABLE_SILVERLIGHT] === o.T1, this.clientId = s
                }, e.getPlayerName = function() {
                    return e.PLAYER_NAME
                }, e.prototype.getPlayerName = function() {
                    return e.PLAYER_NAME
                }, e.prototype.getPlayerVersion = function() {
                    return this.loaderConfig.dependencies.DVWebGlobalPlayer.version
                }, e.prototype.getMetricsFeature = function() {
                    return e.METRICS_FEATURE
                }, e.prototype.getDeviceTypeId = function() {
                    return this.os.getFamily() === l.Mac ? n.DeviceTypeId.SILVERLIGHT_MAC : n.DeviceTypeId.SILVERLIGHT_PC
                }, e.prototype.getPlayerCapabilities = function(t, i, n) {
                    void 0 === i && (i = {});
                    var r = this.clientId === e.TNF_GATEWAY_CLIENT_ID,
                        o = this.silverlightDisabled || i.supportsLive === !0 || r;
                    t({
                        supportsDrm: !0,
                        supportsNativePlayback: !1,
                        supportsWebPlayback: !o && this.playerOption.isCompatible(),
                        supportsAVOD: !0,
                        supportsMidStreamMTA: !0,
                        supportsHDMovies: this.os.getFamily() === l.Windows,
                        supportsHDTVShows: !0,
                        supportsLive: !1
                    })
                }, e.prototype.createWebPlayer = function(e, t) {
                    var i = this;
                    this.playerOption.checkAvailability(function() {
                        e(new f(i.loaderConfig, i.qosReporter, new p, c.createModalTemplate))
                    }, t)
                }, e.prototype.on = function(e, t) {}, e.prototype.once = function(e, t) {}, e.prototype.off = function(e, t) {}, e.PLAYER_NAME = "silverlight", e.METRICS_FEATURE = "SilverlightPlayer", e.TNF_GATEWAY_CLIENT_ID = "511cc9af-3ecb-48fe-9825-886ebfbb6503", e)
            }();
        t.exports = y
    }, {
        266: 266,
        272: 272,
        281: 281,
        38: 38,
        40: 40,
        41: 41,
        80: 80
    }],
    38: [function(e, t, i) {
        "use strict";
        var n = e(113),
            r = n.Version,
            o = e(231),
            s = o.ErrorSeverity,
            a = e(272),
            l = a.BrowserType,
            c = a.OperatingSystemFamily,
            u = a.WindowsVersion,
            p = e(93),
            h = e(281),
            f = e(284),
            d = e(39),
            y = e(294),
            g = h.of("SilverlightWebPlayerOption"),
            m = f.get("SilverlightWebPlayerOption"),
            v = function() {
                function e(e, t, i, n) {
                    void 0 === n && (n = navigator), this.os = g(e, "os").exists(), this.browser = g(t, "browser").exists(), this.createModalTemplate = g(i, "createModalTemplate").exists(), this.navigatorRef = n
                }
                return (e.prototype.isCompatible = function() {
                    if (this.os.getFamily() !== c.Windows && this.os.getFamily() !== c.Mac) return (m.info("Silverlight is not supported on non-Win/Mac operating systems."), !1);
                    if (y.contains(e.UNSUPPORTED_BROWSERS, this.browser.getType()) || this.isUnsupportedFirefoxVersion(this.browser)) return (m.info("Silverlight is not supported on " + l[this.browser.getType()] + "."), !1);
                    if (this.browser.getType() === l.IE) {
                        if (u.WINDOWS_8.compareTo(this.os.getVersion()) <= 0 && !this.isActiveXEnabled()) return (m.info("Silverlight is not supported on Internet Explorer Metro."), !1);
                        if (r.fromNumber(e.MIN_IE_VERSION).compareTo(this.browser.getVersion()) > 0) return (m.info("Silverlight is only supported on Internet Explorer version " + this.browser.getVersion() + "."), !1)
                    }
                    return (m.info("Silverlight is supported on " + this.browser + " on " + this.os), !0)
                }, e.prototype.checkAvailability = function(t, i) {
                    return this.os.getFamily() === c.Windows && u.WINDOWS_8.compareTo(this.os.getVersion()) > 0 && this.browser.getType() === l.IE && !this.isActiveXEnabled() ? void i(e.ACTIVEX_DISABLED_ERROR, "ActiveX is not enabled", s.NonFatal, this.createModalTemplate(d.ACTIVEX_KEY, d.ACTIVEX_EXTRA_TEXT)) : void(this.isAvailable() ? this.isUpToDate() ? t() : i(e.UPGRADE_REQUIRED_ERROR, "Silverlight needs to be upgraded", s.NonFatal, this.createModalTemplate(d.UPGRADE_KEY, d.BUTTON_TEMPLATE)) : i(e.NOT_INSTALLED_ERROR, "Silverlight is not installed", s.NonFatal, this.createModalTemplate(this.getBrowserInstallKey(), d.BUTTON_TEMPLATE)))
                }, e.prototype.isUnsupportedFirefoxVersion = function(t) {
                    return this.isFirefoxBrowser(t.getType()) && this.browser.getVersion().compareTo(r.fromNumber(e.FIREFOX_NPAPI_REMOVED_VERSION)) >= 0
                }, e.prototype.isFirefoxBrowser = function(e) {
                    return e === l.Firefox || e === l.Firefox64
                }, e.prototype.isAvailable = function() {
                    if (this.isSafari10OrHigher()) return !0;
                    var t = this.navigatorRef.plugins[e.SILVERLIGHT_NSAPI_ID];
                    if (!p.isNullOrUndefined(t)) return !0;
                    try {
                        return (new ActiveXObject(e.SILVERLIGHT_ACTIVEX_ID), !0)
                    } catch (e) {
                        return !1
                    }
                }, e.prototype.isUpToDate = function() {
                    var t = this.navigatorRef.plugins[e.SILVERLIGHT_NSAPI_ID];
                    if (!p.isNullOrUndefined(t)) {
                        var i = t.description,
                            n = parseFloat(i);
                        return n >= e.MIN_SILVERLIGHT_VERSION
                    }
                    if (this.isSafari10OrHigher()) return !0;
                    try {
                        return (t = new ActiveXObject(e.SILVERLIGHT_ACTIVEX_ID), t.IsVersionSupported("" + e.MIN_SILVERLIGHT_VERSION))
                    } catch (e) {
                        m.info("Silverlight ActiveX plugin not found.")
                    }
                    return !1
                }, e.prototype.getBrowserInstallKey = function() {
                    return this.browser.getType() === l.Safari ? d.INSTALL_SAFARI_KEY : d.INSTALL_KEY
                }, e.prototype.isActiveXEnabled = function() {
                    try {
                        return !!new ActiveXObject("htmlfile")
                    } catch (e) {
                        return !1
                    }
                }, e.prototype.isSafari10OrHigher = function() {
                    return this.browser.getType() === l.Safari && this.browser.getVersion().compareTo(r.fromNumber(e.SAFARI_10_VERSION)) >= 0
                }, e.MIN_SILVERLIGHT_VERSION = 5.1, e.FIREFOX_NPAPI_REMOVED_VERSION = 52, e.MIN_IE_VERSION = 10, e.SAFARI_10_VERSION = 10, e.UNSUPPORTED_BROWSERS = [l.Chrome, l.Edge, l.Opera, l.Unknown], e.SILVERLIGHT_NSAPI_ID = "Silverlight Plug-In", e.SILVERLIGHT_ACTIVEX_ID = "AgControl.AgControl", e.ACTIVEX_DISABLED_ERROR = "SilverlightActiveXDisabledError", e.NOT_INSTALLED_ERROR = "SilverlightNotInstalledError", e.UPGRADE_REQUIRED_ERROR = "SilverlightUpgradeRequiredError", e)
            }();
        t.exports = v
    }, {
        113: 113,
        231: 231,
        272: 272,
        281: 281,
        284: 284,
        294: 294,
        39: 39,
        93: 93
    }],
    39: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(93),
            o = e(32),
            s = function(e) {
                function t(t, i) {
                    var n = e.call(this, t) || this;
                    return (n.extraText = i, n)
                }
                return (n(t, e), t.prototype.renderString = function(t, i, n) {
                    var o = {
                        extraText: this.extraText
                    };
                    return (r.isNullOrUndefined(n) || _.extend(o, n), e.prototype.renderString.call(this, t, i, o))
                }, t.ACTIVEX_KEY = "silverlightActiveX", t.INSTALL_KEY = "silverlightInstall", t.INSTALL_SAFARI_KEY = "silverlightInstallSafari", t.UPGRADE_KEY = "silverlightUpgrade", t.BUTTON_TEMPLATE = "<div class='buttonWrapper'>{{{button}}}</div>", t.ACTIVEX_EXTRA_TEXT = "<ol>{{#steps}}<li>{{{.}}}</li>{{/steps}}</ol>", t)
            }(o);
        t.exports = s
    }, {
        32: 32,
        93: 93
    }],
    40: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(39),
            o = n.of("SilverlightModalTemplate"),
            s = function() {
                function e() {}
                return (e.createModalTemplate = function(e, t) {
                    return (void 0 === t && (t = ""), o(e, "key").exists(), new r(e, t))
                }, e)
            }();
        t.exports = s
    }, {
        281: 281,
        39: 39
    }],
    41: [function(e, t, i) {
        "use strict";
        var n = e(284),
            r = e(93),
            o = e(281),
            s = n.get("SpinnerPanel"),
            a = o.of("SpinnerPanel"),
            l = function() {
                function e() {}
                return (e.prototype.initialize = function(e, t, i, n) {
                    this.parent = a(e, "parent").exists(), this.assetsRoot = a(t, "assetsRoot").is.a.string(), this.contents = n, this.createHTMLElements(i)
                }, e.prototype.uninitialize = function() {
                    if (r.exists(this.parent) && r.exists(this.spinner) && this.spinner.parentNode === this.parent) try {
                        this.parent.removeChild(this.spinner)
                    } catch (e) {
                        s.error("Failed to remove spinner from parent", e)
                    }
                }, e.prototype.createHTMLElements = function(t) {
                    this.spinner = document.createElement("div"), this.spinner.className = "playerLoadingOverlay", this.spinner.style.width = "100%", this.spinner.style.height = "100%", t ? this.spinner.innerHTML = '<div style="width:100%; height:44%"></div><div style="width:100%; height:11.36%">' + ('<img class="spinning orangeSpinner" src="' + this.assetsRoot + "/" + e.SPINNER_PNG_PATH + '"/>') + '</div><div style="width:100%; height:4.5%"></div>' : this.spinner.innerHTML = '<div class="spinning whiteSpinner"></div>', this.parent.appendChild(this.spinner), this.show()
                }, e.prototype.show = function() {
                    this.spinner.style.display = "", r.isNullOrUndefined(this.contents) || (this.contents.style.width = "1px", this.contents.style.height = "1px")
                }, e.prototype.hide = function() {
                    this.spinner.style.display = "none", r.isNullOrUndefined(this.contents) || (this.contents.style.width = "", this.contents.style.height = "")
                }, e.SPINNER_PNG_PATH = "img/orange_spinner.png", e)
            }();
        t.exports = l
    }, {
        281: 281,
        284: 284,
        93: 93
    }],
    42: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.toString = function() {
                return "aborted"
            }, e)
        }();
        t.exports = n
    }, {}],
    43: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.toString = function() {
                return "asinChanged"
            }, e)
        }();
        t.exports = n
    }, {}],
    44: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e) {
                this.position = e
            }
            return (e.toString = function() {
                return "closed"
            }, e)
        }();
        t.exports = n
    }, {}],
    45: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.toString = function() {
                return "playerCapabilities"
            }, e)
        }();
        t.exports = n
    }, {}],
    46: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.toString = function() {
                return "playing"
            }, e)
        }();
        t.exports = n
    }, {}],
    47: [function(e, t, i) {
        "use strict";
        var n = e(42),
            r = e(43),
            o = e(44),
            s = e(45),
            a = e(46),
            l = e(231),
            c = l.AbortedEvent,
            u = l.CloseEvent,
            p = l.Features,
            h = l.InitializedEvent,
            f = l.PreparedEvent,
            d = l.PlayingEvent,
            y = l.StoppingEvent,
            g = l.EndOfContentEvent,
            m = l.AsinChangedEvent,
            v = l.PlayerCapabilitiesEvent,
            E = l.CachePriority,
            _ = l.CacheType,
            S = e(281),
            T = e(93),
            b = e(82),
            I = e(294),
            w = e(284),
            R = e(3),
            C = e(24),
            P = e(17),
            A = S.of("StatelessWebPlayer"),
            O = w.get("StatelessWebPlayer"),
            N = function() {
                function e(e, t, i, n) {
                    void 0 === t && (t = R), void 0 === i && (i = I.now), void 0 === n && (n = !0);
                    var o = this;
                    this.Features = p, this.webPlayer = null, this.playerConfig = null, this.contentConfig = null, this.playbackConfig = null, this.newPlayerConfig = null, this.newContentConfig = null, this.newPlaybackConfig = null, this.playerCapabilities = null, this.isWaiting = !1, this.canPrepareOnInitialize = !0, this.shouldEmitClosed = !1, this.lastPosition = null, this.cacheType = _.CacheMiss, this.playInvokedTime = null, this.eventEmitter = new b.BasicEventEmitter, this.featureListeners = [], this.onStateChanged = function() {
                        o.isWaiting = !1;
                        var e = o.getPlayerState();
                        switch (O.info("Changed state to: " + P[e]), e) {
                            case P.UNINITIALIZED:
                                o.onUninitialized();
                                break;
                            case P.INITIALIZED:
                                o.onInitialized();
                                break;
                            case P.PREPARING:
                                o.onPreparing();
                                break;
                            case P.PREPARED:
                                o.onPrepared();
                                break;
                            case P.PLAYING:
                                o.onPlaying();
                                break;
                            case P.STOPPING:
                                o.onStopping();
                                break;
                            case P.ABORTED:
                                o.onAborted();
                                break;
                            default:
                                throw new Error("Unable to transition at invalid state: " + e)
                        }
                    }, this.onPlayerCapabilities = function(e) {
                        var t = e.capabilities;
                        I.isEqual(o.playerCapabilities, t) || (o.playerCapabilities = C(!0, {}, t), o.emitEvent(s, {
                            capabilities: o.playerCapabilities
                        }))
                    }, this.onPlayerClose = function() {
                        o.shouldEmitClosed = !0;
                        var e = o.getPlayerState();
                        e !== P.PREPARED && e !== P.PLAYING || (o.lastPosition = o.webPlayer.getPosition()), o.newContentConfig = null, o.newPlaybackConfig = null, o.onStateChanged()
                    }, this.reportPlaying = function(e) {
                        o.emitEvent(a, e)
                    }, this.reportAsinChanged = function(e) {
                        o.emitEvent(r, e)
                    }, this.loaderConfig = e, this.webPlayerClass = t, this.now = i, this.preInitEnabled = n
                }
                return (e.prototype.initialize = function(e) {
                    if (!this.preInitEnabled) return void O.info("Pre-init disabled, ignoring call to initialize.");
                    this.setConfigs(e);
                    try {
                        O.info("Initialize called."), this.updateStateFromConfig()
                    } catch (e) {
                        throw (O.error("Unhandled exception in initialize.", e), e)
                    }
                }, e.prototype.prepare = function(e, t) {
                    if (!this.preInitEnabled) return void O.warn("Pre-init disabled, ignoring call to prepare.");
                    if (!T.isNullOrUndefined(this.newPlaybackConfig)) return void O.warn("Pre-init disabled while already playing content, ignoring calls to prepare.");
                    if (this.setConfigs(e, t), !T.isNullOrUndefined(this.webPlayer) && this.newContentConfig.cachePriority !== E.High) return void this.webPlayer.prepareCache(this.newContentConfig);
                    try {
                        if (O.info("Prepare called."), this.isEqualWithoutPosition(this.newContentConfig, this.contentConfig) && null !== this.newPlaybackConfig) return void O.info("Content config matches previous call to play, ignoring call to prepare.");
                        this.updateStateFromConfig()
                    } catch (e) {
                        throw (O.error("Unhandled exception in prepare.", e), e)
                    }
                }, e.prototype.play = function(e, t, i) {
                    try {
                        if (this.setConfigs(e, t, i), O.info("Play called."), this.newContentConfig.cachePriority = E.High, !(null !== this.playInvokedTime || I.isEqual(this.contentConfig, this.newContentConfig) && I.isEqual(this.playbackConfig, this.newPlaybackConfig))) {
                            switch (this.playInvokedTime = this.now(), this.getPlayerState()) {
                                case P.PREPARED:
                                    this.cacheType = _.CacheHit;
                                    break;
                                case P.PREPARING:
                                    this.cacheType = _.CachePartialHit;
                                    break;
                                default:
                                    this.cacheType = _.CacheMiss
                            }
                            null !== this.webPlayer && this.webPlayer.playbackInitiated(this.isEqualWithoutPosition(this.contentConfig, this.newContentConfig))
                        }
                        I.isEqual(this.contentConfig, this.newContentConfig) || (this.cacheType = _.CacheMiss), this.updateStateFromConfig()
                    } catch (e) {
                        throw (O.error("Unhandled exception in play.", e), e)
                    }
                }, e.prototype.on = function(e, t) {
                    this.eventEmitter.on(e, t)
                }, e.prototype.once = function(e, t) {
                    this.eventEmitter.once(e, t)
                }, e.prototype.off = function(e, t) {
                    this.eventEmitter.off(e, t)
                }, e.prototype.getFeature = function(e, t, i) {
                    return T.isNullOrUndefined(this.webPlayer) ? void this.featureListeners.push({
                        feature: e,
                        onAvailable: t,
                        onError: i
                    }) : void this.webPlayer.getFeature(e, t, i)
                }, e.prototype.updateStateFromConfig = function() {
                    null !== this.newContentConfig && (this.canPrepareOnInitialize = !0), this.shouldEmitClosed && (this.shouldEmitClosed = !1), this.isWaiting || this.onStateChanged()
                }, e.prototype.setConfigs = function(e, t, i) {
                    void 0 === t && (t = null), void 0 === i && (i = null), A(null === this.playerConfig || I.isEqual(this.playerConfig, e), "The playerConfig cannot be changed after initialization.").is["true"](), this.newPlayerConfig = C(!0, {}, e), null !== t && (t.cachePriority = T.withDefault(t.cachePriority, E.High)), this.newContentConfig = null !== t ? C(!0, {}, t) : null, this.newPlaybackConfig = null !== i ? C(!0, {}, i) : null
                    ;if(null !== this.newContentConfig) console.log("newContentConfig: " + JSON.stringify(this.newContentConfig, null, 4));
                }, e.prototype.onUninitialized = function() {
                    this.playerConfig = this.newPlayerConfig, null !== this.webPlayer && (this.webPlayer.off(h, this.onStateChanged), this.webPlayer.off(f, this.onStateChanged), this.webPlayer.off(d, this.onStateChanged), this.webPlayer.off(d, this.reportPlaying), this.webPlayer.off(m, this.reportAsinChanged), this.webPlayer.off(u, this.onPlayerClose), this.webPlayer.off(g, this.onPlayerClose), this.webPlayer.off(v, this.onPlayerCapabilities), this.webPlayer.off(c, this.onStateChanged)), this.webPlayer = new this.webPlayerClass(this.loaderConfig), this.webPlayer.on(h, this.onStateChanged), this.webPlayer.on(f, this.onStateChanged), this.webPlayer.on(d, this.onStateChanged), this.webPlayer.on(d, this.reportPlaying), this.webPlayer.on(y, this.onStateChanged), this.webPlayer.on(m, this.reportAsinChanged), this.webPlayer.on(u, this.onPlayerClose), this.webPlayer.on(g, this.onPlayerClose), this.webPlayer.once(v, this.onPlayerCapabilities), this.webPlayer.once(c, this.onStateChanged), this.isWaiting = !0, this.webPlayer.initialize(this.playerConfig), null !== this.newPlaybackConfig && this.webPlayer.playbackInitiated(!0)
                }, e.prototype.onInitialized = function() {
                    if (this.reportClosedEvent(), !this.canPrepareOnInitialize) return (this.canPrepareOnInitialize = !0, this.contentConfig = null, void(this.playbackConfig = null));
                    if (!I.isEqual(this.playerConfig, this.newPlayerConfig)) return void this.onUninitialized();
                    for (var e = 0, t = this.featureListeners; e < t.length; e++) {
                        var i = t[e];
                        this.webPlayer.getFeature(i.feature, i.onAvailable, i.onError)
                    }
                    if (this.featureListeners = [], !I.isEqual(this.contentConfig, this.newContentConfig) || !I.isEqual(this.playbackConfig, this.newPlaybackConfig))
                        if (this.contentConfig = this.newContentConfig, null !== this.contentConfig) {
                            var n = !T.isNullOrUndefined(this.newPlaybackConfig);
                            this.canPrepareOnInitialize = !1, this.contentConfig.cachePriority === E.High ? this.webPlayer.prepare(I.extend({
                                immediateConsumption: n
                            }, this.contentConfig)) : this.webPlayer.prepareCache(this.contentConfig)
                        } else this.playbackConfig = null
                }, e.prototype.onPreparing = function() {
                    var e = this.isEqualWithoutPosition(this.contentConfig, this.newContentConfig);
                    I.isEqual(this.playerConfig, this.newPlayerConfig) && e || (this.isWaiting = !0, this.webPlayer.stop())
                }, e.prototype.onPrepared = function() {
                    this.canPrepareOnInitialize = !0;
                    var e = this.isEqualWithoutPosition(this.contentConfig, this.newContentConfig);
                    return I.isEqual(this.playerConfig, this.newPlayerConfig) && e ? (I.isNumber(this.newContentConfig.position) && this.contentConfig.position !== this.newContentConfig.position && (this.contentConfig.position = this.newContentConfig.position, this.webPlayer.seek(this.contentConfig.position)), this.playbackConfig = this.newPlaybackConfig, void(null !== this.playbackConfig && (this.webPlayer.play(C({}, this.playbackConfig, {
                        playbackInitiatedTime: this.playInvokedTime,
                        cacheType: this.cacheType
                    })), this.playInvokedTime = null))) : (this.isWaiting = !0, void this.webPlayer.stop())
                }, e.prototype.onPlaying = function() {
                    var e = this.isEqualWithoutPosition(this.contentConfig, this.newContentConfig);
                    if (!I.isEqual(this.playerConfig, this.newPlayerConfig) || !e || !I.isEqual(this.playbackConfig, this.newPlaybackConfig)) return (this.isWaiting = !0, void this.webPlayer.stop());
                    if (null !== this.contentConfig && I.isNumber(this.newContentConfig.position) && this.contentConfig.position !== this.newContentConfig.position) {
                        var t = this.newContentConfig.position;
                        this.webPlayer.seek(t)
                    }
                    this.newContentConfig.position = null, this.contentConfig = this.newContentConfig
                }, e.prototype.onStopping = function() {
                    this.newContentConfig = null, this.newPlaybackConfig = null
                }, e.prototype.onAborted = function() {
                    this.reportClosedEvent(), this.emitEvent(n, new n)
                }, e.prototype.reportClosedEvent = function() {
                    this.shouldEmitClosed && (this.emitEvent(o, new o(this.lastPosition)), this.lastPosition = null, this.shouldEmitClosed = !1)
                }, e.prototype.emitEvent = function(e, t) {
                    try {
                        this.eventEmitter.emit(e, t)
                    } catch (t) {
                        O.error("An exception was thrown in a StatelessWebPlayer callback for event: " + e, t)
                    }
                }, e.prototype.getPlayerState = function() {
                    return null === this.webPlayer ? P.UNINITIALIZED : this.webPlayer.getState()
                }, e.prototype.isEqualWithoutPosition = function(e, t) {
                    return I.isEqual(I.omit(e, "position"), I.omit(t, "position"))
                }, e)
            }();
        t.exports = N
    }, {
        17: 17,
        231: 231,
        24: 24,
        281: 281,
        284: 284,
        294: 294,
        3: 3,
        42: 42,
        43: 43,
        44: 44,
        45: 45,
        46: 46,
        82: 82,
        93: 93
    }],
    48: [function(e, t, i) {
        "use strict";
        var n = "com.widevine.alpha",
            r = "com.microsoft.playready",
            o = "cenc",
            s = function() {
                function e() {}
                return (e.prototype.getDrmSystems = function() {
                    return [{
                        keySystem: r,
                        configurations: [
                            [{
                                initDataTypes: [o],
                                audioCapabilities: [{
                                    contentType: 'audio/mp4; codecs="mp4a.40.2"'
                                }],
                                videoCapabilities: [{
                                    contentType: 'video/mp4; codecs="avc1.4D401E"',
                                    robustness: "HW_SECURE_DECODE"
                                }, {
                                    contentType: 'video/mp4; codecs="avc1.4D401E"',
                                    robustness: "SW_SECURE_DECODE"
                                }]
                            }]
                        ]
                    }, {
                        keySystem: n,
                        configurations: [
                            [{
                                initDataTypes: [o],
                                videoCapabilities: [{
                                    contentType: 'video/mp4; codecs="avc1.4D401E"',
                                    robustness: "HW_SECURE_DECODE"
                                }, {
                                    contentType: 'video/mp4; codecs="avc1.4D401E"',
                                    robustness: "SW_SECURE_DECODE"
                                }]
                            }],
                            [{
                                initDataTypes: [o],
                                videoCapabilities: [{
                                    contentType: 'video/mp4; codecs="avc1.640028"'
                                }]
                            }],
                            [{
                                initDataTypes: [o]
                            }]
                        ]
                    }]
                }, e)
            }();
        t.exports = s
    }, {}],
    49: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.BROWSER_OUT_OF_DATE = 0] = "BROWSER_OUT_OF_DATE", e[e.MISSING_CODEC = 1] = "MISSING_CODEC", e[e.MISSING_CDM = 2] = "MISSING_CDM", e[e.UNSUPPORTED_BROWSER = 3] = "UNSUPPORTED_BROWSER", e[e.UNSUPPORTED_OS = 4] = "UNSUPPORTED_OS"
        }(n || (n = {})), t.exports = n
    }, {}],
    50: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(281),
            o = e(284),
            s = e(294),
            a = r.of("WebPlayerFactorySelector"),
            l = o.get("WebPlayerFactorySelector"),
            c = function() {
                function e() {
                    this.sortedPlayerFactories = null
                }
                return (e.prototype.initialize = function(e, t, i, n, r, o) {
                    this.sortedPlayerFactories = a(e, "factoryList").exists(), a(i, "weblabs").exists(), s.forEach(this.sortedPlayerFactories, function(e) {
                        e.initialize(t, i, n, r, o)
                    })
                }, e.prototype.reset = function() {
                    this.sortedPlayerFactories = null
                }, e.prototype.selectFactory = function(e, t, i) {
                    a(this.sortedPlayerFactories, "this.sortedPlayerFactories").exists();
                    var r = s.clone(this.sortedPlayerFactories);
                    if (!n.isNullOrUndefined(i)) {
                        var o = i.toLowerCase(),
                            c = s.findIndex(r, function(e) {
                                return e.getPlayerName() === o
                            });
                        if (c === -1) l.warn("Cannot find preferred player factory.");
                        else {
                            var u = r.splice(c, 1);
                            r.splice(0, 0, u[0])
                        }
                    }
                    this.findPlayerFactory(r, 0, e, t)
                }, e.prototype.findPlayerFactory = function(e, t, i, n, r, o) {
                    var s = this;
                    void 0 === r && (r = ""), void 0 === o && (o = null), this.isFactorySupported(e[t], function(a, l, c, u) {
                        void 0 === c && (c = "");
                        var p = r + c;
                        null === o && (o = u), a ? i(e[t], l, p) : t !== e.length - 1 ? s.findPlayerFactory(e, t + 1, i, n, p, o) : i(null, null, p, o)
                    }, n)
                }, e.prototype.selectFactoryWithOverride = function(e, t) {
                    a(t, "override").is.a.string(), a(this.sortedPlayerFactories, "this.sortedPlayerFactories").exists();
                    var i = t.toLowerCase(),
                        n = s.find(this.sortedPlayerFactories, function(e) {
                            return e.getPlayerName() === i
                        });
                    return s.isUndefined(n) ? (l.error("Overridden player '" + t + "' does not exist."), void e(null)) : void this.isFactorySupported(n, function(i, r) {
                        i || l.warn("Overridden player '" + t + "' is not supported on this platform. Playback may not function."), e(n, r)
                    })
                }, e.prototype.isFactorySupported = function(e, t, i) {
                    e.getPlayerCapabilities(function(e, i, n) {
                        t(e.supportsWebPlayback || e.supportsNativePlayback, e, i, n)
                    }, i)
                }, e)
            }();
        t.exports = c
    }, {
        281: 281,
        284: 284,
        294: 294,
        93: 93
    }],
    51: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e.Unloaded = "Unloaded", e.Waiting = "Waiting", e.Ready = "Ready", e.Error = "Error"
        }(n || (n = {})), t.exports = n
    }, {}],
    52: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e.Paused = "Paused", e.Playing = "Playing"
        }(n || (n = {})), t.exports = n
    }, {}],
    53: [function(e, t, i) {
        "use strict";
        var n = this && this.__assign || Object.assign || function(e) {
                for (var t, i = 1, n = arguments.length; i < n; i++) {
                    t = arguments[i];
                    for (var r in t) Object.prototype.hasOwnProperty.call(t, r) && (e[r] = t[r])
                }
                return e
            },
            r = e(281),
            o = e(284),
            s = e(93),
            a = e(80),
            l = a.DeviceTypeId,
            c = e(82),
            u = c.BasicEventEmitter,
            p = e(231),
            h = p.DiagnosticsState,
            f = p.Features,
            d = p.AbortedEvent,
            y = p.AsinChangedEvent,
            g = p.CloseEvent,
            m = p.EndOfContentEvent,
            v = p.InitializedEvent,
            E = p.PausedEvent,
            _ = p.PlayingEvent,
            S = p.PreparedEvent,
            T = p.StoppingEvent,
            b = e(51),
            I = e(52),
            w = r.of("WrappedXpPlayer"),
            R = o.get("WrappedXpPlayer"),
            C = function() {
                function e(e, t) {
                    void 0 === t && (t = new u);
                    var i = this;
                    this.Features = f, this.pendingFeatureRequests = [], this.currentTitleId = null, this.notifyPlaybackInitiatedOnInitialized = !1, this.onContentStateChange = function(e) {
                        e === b.Unloaded ? i.eventEmitter.emit(v) : e === b.Ready && i.eventEmitter.emit(S)
                    }, this.onPlaybackStateChange = function(e) {
                        e === I.Paused ? i.eventEmitter.emit(E) : e === I.Playing && i.eventEmitter.emit(_)
                    }, this.onTimeDataChange = function(e) {
                        var t = i.player.timeline.contentInfos[i.player.timeline.items[e.currentTimelineItemIndex].contentId];
                        null !== i.currentTitleId && i.currentTitleId !== t.titleId && "Live" !== i.sdkContentConfig.videoMaterialType && i.eventEmitter.emit(y, new y(t.titleId)), i.currentTitleId = t.titleId
                    }, this.onTimelineEnded = function() {
                        i.eventEmitter.emit(m)
                    }, this.onError = function(e) {
                        i.emitError(e)
                    }, this.onContentError = function(e) {
                        i.emitError(e)
                    }, this.onPlayerClose = function() {
                        i.eventEmitter.emit(g)
                    }, this.webPlayerSdkType = e, this.eventEmitter = t
                }
                return (e.prototype.initialize = function(e) {
                    var t = this;
                    if (s.isNullOrUndefined(this.webPlayerSdk)) {
                        var i = {
                            marketplaceId: e.marketplaceId,
                            clientId: e.clientId,
                            deviceId: e.deviceId,
                            debug: e.diagnostics === h.Enabled || e.diagnostics === h.Auto,
                            weblabs: e.weblabs,
                            sessionId: e.sessionId
                        };
                        this.webPlayerSdk = new this.webPlayerSdkType(i)
                    }
                    var n = {
                        container: e.domElement,
                        locale: e.locale,
                        titleDecorationScheme: "primary-content"
                    };
                    this.webPlayerSdk.getFeature(this.webPlayerSdk.Features.AmazonVideoPlayer, n).then(function(e) {
                        t.player = e, t.notifyPlaybackInitiatedOnInitialized && t.player.playbackInitiated(), t.attachEventListeners(t.player), t.attachPendingFeatureRequests(), t.eventEmitter.emit(v)
                    })["catch"](function(e) {
                        R.error("Couldn't load player.", e), t.emitError(e)
                    })
                }, e.prototype.prepare = function(e) {
                    w(this.player, "player").exists(), this.sdkContentConfig = {
                        titleId: e.asin,
                        videoMaterialType: e.videoType
                    }, s.exists(e.position) && (this.sdkContentConfig.position = this.convertToMilliseconds(e.position)), this.player.load(this.sdkContentConfig)
                }, e.prototype.prepareCache = function(e) {}, e.prototype.play = function(e) {
                    w(this.player, "player").exists(), this.sdkContentConfig = n({}, this.sdkContentConfig, {
                        clickstreamRefMarker: e.clickstreamRefMarker
                    }), this.player.start(this.sdkContentConfig)
                }, e.prototype.playbackInitiated = function() {
                    s.exists(this.player) ? this.player.playbackInitiated() : this.notifyPlaybackInitiatedOnInitialized = !0
                }, e.prototype.stop = function() {
                    w(this.player, "player").exists(), this.player.unload(), this.eventEmitter.emit(T), this.notifyPlaybackInitiatedOnInitialized = !1
                }, e.prototype.seek = function(e) {
                    w(this.player, "player").exists(), this.player.seek(this.convertToMilliseconds(e))
                }, e.prototype.once = function(e, t) {
                    this.eventEmitter.once(e, t)
                }, e.prototype.on = function(e, t) {
                    this.eventEmitter.on(e, t)
                }, e.prototype.off = function(e, t) {
                    this.eventEmitter.off(e, t)
                }, e.prototype.getPosition = function() {
                    return (w(this.player, "player").exists(), this.convertToSeconds(this.player.currentTime.currentPosition))
                }, e.prototype.getDuration = function() {
                    w(this.player, "player").exists();
                    var e = this.player.timeline.items[this.player.currentTime.currentTimelineItemIndex];
                    return null === e.startTime || null === e.endTime ? 0 : this.convertToSeconds(e.endTime - e.startTime)
                }, e.prototype.getDeviceTypeId = function() {
                    return l.HTML5
                }, e.prototype.validatePIN = function(e) {
                    throw new Error("WrappedXpPlayer doesn't implement validatePIN.")
                }, e.prototype.getFeature = function(e, t, i) {
                    return s.exists(this.player) ? e === f.volume ? void this.player.getFeature(this.player.Features.Volume).then(t)["catch"](i) : void i(new Error("WrappedXpPlayer doesn't support the requested feature.")) : void this.pendingFeatureRequests.push({
                        feature: e,
                        onAvailable: t,
                        onError: i
                    })
                }, e.prototype.convertToMilliseconds = function(e) {
                    return 1e3 * e
                }, e.prototype.convertToSeconds = function(e) {
                    return e / 1e3
                }, e.prototype.emitError = function(e) {
                    this.eventEmitter.emit(d, new d)
                }, e.prototype.attachEventListeners = function(e) {
                    e.on(e.Events.ContentError, this.onContentError), e.on(e.Events.ContentStateChange, this.onContentStateChange), e.on(e.Events.Error, this.onError), e.on(e.Events.PlaybackStateChange, this.onPlaybackStateChange), e.on(e.Events.TimeDataChange, this.onTimeDataChange), e.on(e.Events.TimelineEnded, this.onTimelineEnded), e.on(e.Events.Close, this.onPlayerClose)
                }, e.prototype.attachPendingFeatureRequests = function() {
                    for (; this.pendingFeatureRequests.length > 0;) {
                        var e = this.pendingFeatureRequests.shift();
                        this.getFeature(e.feature, e.onAvailable, e.onError)
                    }
                }, e)
            }();
        t.exports = C
    }, {
        231: 231,
        281: 281,
        284: 284,
        51: 51,
        52: 52,
        80: 80,
        82: 82,
        93: 93
    }],
    54: [function(e, t, i) {
        "use strict";
        var n = e(231),
            r = n.ErrorSeverity,
            o = e(187),
            s = e(281),
            a = e(284),
            l = e(294),
            c = e(250),
            u = c.JavaScriptLoader,
            p = c.ScriptType,
            h = c.TransparentCachingScriptLoader,
            f = e(53),
            d = s.of("WrappedXpPlayerLoader"),
            y = a.get("WrappedXpPlayerLoader"),
            g = {
                cloudfront: "cloudfront",
                limelight: "limelight",
                lvlt: "level3"
            },
            m = "XpLoaderJavaScriptDownloadTime",
            v = "XpLoaderJavaScriptDownloadError",
            E = "XpLoaderJavaScriptCacheHit",
            _ = "XpLoaderJavaScriptCacheMiss",
            S = "XpLoaderJavaScriptDownloadInfo",
            T = ["https://cloudfront.xp-assets.aiv-cdn.net/packages/ATVWebPlayerSDK-1.0/gamma/WebPlayerSDK.js", "https://lvlt.xp-assets.aiv-cdn.net/packages/ATVWebPlayerSDK-1.0/gamma/WebPlayerSDK.js", "https://limelight.xp-assets.aiv-cdn.net/packages/ATVWebPlayerSDK-1.0/gamma/WebPlayerSDK.js"],
            b = ["https://cloudfront.xp-assets.aiv-cdn.net/packages/ATVWebPlayerSDK-1.0/prod/WebPlayerSDK.js", "https://lvlt.xp-assets.aiv-cdn.net/packages/ATVWebPlayerSDK-1.0/prod/WebPlayerSDK.js", "https://limelight.xp-assets.aiv-cdn.net/packages/ATVWebPlayerSDK-1.0/prod/WebPlayerSDK.js"],
            I = function() {
                function e(e, t, i, n) {
                    void 0 === t && (t = new h), void 0 === i && (i = new u), void 0 === n && (n = f), this.qosReporter = d(e, "qosReporter").exists(), this.blobScriptLoader = t, this.scriptTagScriptLoader = i, this.wrappedXpPlayerType = n
                }
                return (e.prototype.createWebPlayer = function(e, t, i, n) {
                    this.acquirePlayer(e, t, i, n)
                }, e.prototype.acquirePlayer = function(e, t, i, n, s) {
                    var a = this;
                    void 0 === s && (s = 0);
                    var l = t ? T[s] : b[s],
                        c = {
                            crc32: null,
                            url: l,
                            name: "WebPlayerSDK",
                            type: p.JAVASCRIPT,
                            version: "0.0.0.0"
                        },
                        u = function(c, u) {
                            a.tryReportWithCDN(v, 1, l);
                            var p = a.getErrorString(c);
                            p += a.getAdditionalErrorInfo(u), y.error(v + ": " + p);
                            var h = t ? T : b;
                            return s >= h.length - 1 ? n(v, p, r.Fatal) : (o.putMetric(v, 1), void a.acquirePlayer(e, t, i, n, s + 1))
                        },
                        h = e === !1 ? this.scriptTagScriptLoader : this.blobScriptLoader;
                    h.get(c, function(e) {
                        var t = JSON.stringify(e);
                        o.putMetric(m, e.totalMs, "ms"), a.tryReportWithCDN(m, e.totalMs, l, "ms"), o.putMetric(e.cached ? E : _, 1), a.qosReporter.reportInformation(S, t, e.totalMs);
                        var n;
                        try {
                            n = new a.wrappedXpPlayerType(window.WebPlayerSDK)
                        } catch (e) {
                            return void u(e)
                        }
                        i(n)
                    }, function(e, t) {
                        var i = a.getErrorString(e),
                            n = JSON.stringify(t);
                        y.error(i + ", note: " + n), a.qosReporter.reportError(v, r.NonFatal, i, n)
                    }, u, !1)
                }, e.prototype.getAdditionalErrorInfo = function(e) {
                    var t = "";
                    if (null != e)
                        for (var i = 0, n = ["attempt", "contentHash", "contentLength", "contentLengthHeader", "serverIP", "cloudfrontRequestID"]; i < n.length; i++) {
                            var r = n[i];
                            null != e[r] && (t += ", " + r + ": " + e[r])
                        }
                    return t
                }, e.prototype.getErrorString = function(e) {
                    return e && l.isFunction(e.toString) ? e.toString() : "Unknown Error"
                }, e.prototype.tryReportWithCDN = function(e, t, i, n) {
                    var r = this.getCDN(i);
                    null != r && o.putMetric(e + "." + r, t, n)
                }, e.prototype.getCDN = function(e) {
                    var t = this.getSubdomain(e);
                    return null != t && this.isValidCDN(t) ? g[t] : null
                }, e.prototype.getSubdomain = function(e) {
                    var t = e.match("https?://([^.]+)?\\.");
                    return null == t || null == t[1] ? null : t[1]
                }, e.prototype.isValidCDN = function(e) {
                    return Object.keys(g).indexOf(e) !== -1
                }, e)
            }();
        t.exports = I
    }, {
        187: 187,
        231: 231,
        250: 250,
        281: 281,
        284: 284,
        294: 294,
        53: 53
    }],
    55: [function(e, t, i) {
        "use strict";

        function n(e) {
            var t = new r(e);
            this.Features = t.Features, this.initialize = function(e) {
                t.initialize(e)
            }, this.prepare = function(e, i) {
                t.prepare(e, i)
            }, this.play = function(e, i, n) {
                t.play(e, i, n)
            }, this.on = function(e, i) {
                t.on(e, i)
            }, this.once = function(e, i) {
                t.once(e, i)
            }, this.off = function(e, i) {
                t.off(e, i)
            }, this.getFeature = function(e, i, n) {
                t.getFeature(e, i, n)
            }
        }
        var r = e(47);
        "undefined" != typeof window && null !== window && (window.ATVWebPlayer = n), t.exports = n
    }, {
        47: 47
    }],
    56: [function(e, t, i) {
        "use strict";
        var n = e(113),
            r = n.LocalStorage,
            o = e(178),
            s = e(252),
            a = e(281),
            l = e(80),
            c = e(159),
            u = s.ExponentialBackoffRetry,
            p = o.HttpRequest,
            h = e(284),
            f = l.MarketplaceID,
            d = e(93),
            y = e(57),
            g = e(294),
            m = a.of("OAuthTokenClient"),
            v = h.get("OAuthTokenClient"),
            E = function() {
                function e(e, t, i, n, o, s) {
                    void 0 === n && (n = new r), void 0 === o && (o = p), void 0 === s && (s = g.now), this.callbacks = [], this.requestInProgress = !1, this.config = e, this.onSuccessMetric = t, this.onErrorMetric = i, this.storage = n, this.httpRequest = o, this.now = s, d.isNullOrUndefined(this.config.token) || (this.token = this.config.token)
                }
                return (e.prototype.getToken = function(t, i) {
                    var n = this;
                    if (m(t, "onSuccess").exists(), m(i, "onError").exists(), this.callbacks.push({
                            onSuccess: t,
                            onError: i
                        }), !this.requestInProgress) {
                        this.requestInProgress = !0;
                        var r = this.now();
                        if (d.isNullOrUndefined(this.token)) {
                            var o = JSON.parse(this.storage.get(e.STORAGE_KEY));
                            this.validateStoredToken(o) && (this.token = o)
                        }
                        d.isNullOrUndefined(this.token) || d.isNullOrUndefined(this.token.refresh) ? this.getNewToken(function(e, t) {
                            return (void 0 === t && (t = !1), n.onGetTokenSuccess(e, n.now() - r, t))
                        }, function(e) {
                            return n.onGetTokenError(e)
                        }) : this.refreshAndGetToken(this.token, function(e, t) {
                            return (void 0 === t && (t = !1), n.onGetTokenSuccess(e, n.now() - r, t))
                        }, function(e) {
                            return n.onGetTokenError(e)
                        })
                    }
                }, e.prototype.getType = function() {
                    return y.OAUTH
                }, e.prototype.onGetTokenSuccess = function(t, i, n) {
                    void 0 === n && (n = !1), this.token = t, this.storage.set(e.STORAGE_KEY, JSON.stringify(this.token));
                    for (var r = 0, o = this.callbacks; r < o.length; r++) {
                        var s = o[r];
                        s.onSuccess({
                            token: this.token.access,
                            type: y.OAUTH
                        })
                    }
                    this.callbacks = [], this.requestInProgress = !1, n || d.isNullOrUndefined(this.onSuccessMetric) || this.onSuccessMetric(i)
                }, e.prototype.onGetTokenError = function(e) {
                    for (var t = 0, i = this.callbacks; t < i.length; t++) {
                        var n = i[t];
                        n.onError(e)
                    }
                    this.callbacks = [], this.requestInProgress = !1, d.isNullOrUndefined(this.onErrorMetric) || this.onErrorMetric(e)
                }, e.prototype.refreshAndGetToken = function(t, i, n) {
                    var r = this;
                    if (!this.tokenHasExpired(t)) return void i(t, !0);
                    var o = {
                            app_name: this.config.appName,
                            app_version: this.config.appVersion,
                            requested_token_type: e.ACCESS_TOKEN_TYPE,
                            source_token: this.token.refresh,
                            source_token_type: e.REFRESH_TOKEN_TYPE
                        },
                        s = function(e) {
                            var t = r.validateTokenResponse(e);
                            return d.isNullOrUndefined(t) ? void i({
                                access: e.access_token,
                                expireTime: r.now() + 1e3 * parseInt(e.expires_in),
                                refresh: r.token.refresh
                            }) : void n(t)
                        };
                    this.sendRequest(e.REFRESH_API, o, s, n)
                }, e.prototype.getNewToken = function(t, i) {
                    var n = this;
                    if (!d.isNullOrUndefined(this.token) && !this.tokenHasExpired(this.token)) return void t(this.token, !0);
                    var r = {
                            app_name: this.config.appName,
                            app_version: this.config.appVersion,
                            scope: [e.TOKEN_SCOPE]
                        },
                        o = function(e) {
                            var r = n.validateTokenResponse(e);
                            return d.isNullOrUndefined(r) ? void t({
                                access: e.access_token,
                                expireTime: n.now() + 1e3 * parseInt(e.expires_in)
                            }) : void i(r)
                        };
                    this.sendRequest(e.COOKIES_API, r, o, i)
                }, e.prototype.tokenHasExpired = function(e) {
                    return e.expireTime - this.now() < 0
                }, e.prototype.validateTokenResponse = function(t) {
                    return d.isNullOrUndefined(t.access_token) ? c.newError(e.OAUTH_ERROR, "access_token was not in response.") : d.isNullOrUndefined(t.expires_in) ? c.newError(e.OAUTH_ERROR, "expires_in was not in response.") : d.isNullOrUndefined(t.token_type) || t.token_type !== e.BEARER_TOKEN_TYPE ? c.newError(e.OAUTH_ERROR, "token_type was not in response or the wrong type.") : null
                }, e.prototype.validateStoredToken = function(e) {
                    return !d.isNullOrUndefined(e) && !d.isNullOrUndefined(e.access) && !d.isNullOrUndefined(e.expireTime)
                }, e.prototype.getEndpointForMarketplace = function(t) {
                    for (var i in e.ENDPOINTS)
                        if (e.ENDPOINTS.hasOwnProperty(i) && g.contains(e.ENDPOINTS[i], t)) return i;
                    return null
                }, e.prototype.sendRequest = function(t, i, n, r) {
                    var o = this,
                        s = this.getEndpointForMarketplace(this.config.marketplaceId);
                    if (d.isNullOrUndefined(s)) return void r(c.newError(e.OAUTH_ERROR, "Invalid marketplace ID: " + this.config.marketplaceId));
                    var a = s + t;
                    (new u).withMaxAttempts(e.MAX_ATTEMPTS).withInitialIntervalMs(500).withExponentialFactor(2).withRandomizationFactor(200).onSuccess(n).onError(r).onAttempt(function(t, n, r, s) {
                        o.httpRequest.post(a).accept("application/json").json(i).onError(function(i) {
                            v.error("Attempt " + (t + 1) + " of " + e.MAX_ATTEMPTS + ": " + i.message), n(i)
                        }).onLoad(function(t) {
                            var i, n = t.getText();
                            if (!d.isNullOrUndefined(n)) try {
                                i = JSON.parse(n)
                            } catch (t) {
                                return void s(c.newError(e.OAUTH_ERROR, "An error occurred while trying to parse JSON response: " + t))
                            }
                            var o = d.nestedValue(i, "response.error");
                            if (!d.isNullOrUndefined(o)) {
                                var a = d.isNullOrUndefined(o.detail) ? o.message : o.message + " (" + o.detail + ")";
                                return void s(c.newError(e.OAUTH_ERROR, "An error was returned by the service. " + o.code + ": " + a))
                            }
                            return d.isNullOrUndefined(i.error) && d.isNullOrUndefined(i.error_description) ? void r(i) : void s(c.newError(e.OAUTH_ERROR, "An error was returned by the service. " + i.error + ": " + i.error_description))
                        }).withCredentials().send()
                    }).execute()
                }, e.TOKEN_SCOPE = "aiv:full", e.MAX_ATTEMPTS = 4, e.OAUTH_ERROR = "OAuthError", e.STORAGE_KEY = "atvwebplayer_token", e.ACCESS_TOKEN_TYPE = "access_token", e.BEARER_TOKEN_TYPE = "bearer", e.REFRESH_TOKEN_TYPE = "refresh_token", e.ENDPOINTS = {
                    "https://api.amazon.de/auth/": [f.DE],
                    "https://api.amazon.co.jp/auth/": [f.JP, f.ROW_FE],
                    "https://api.amazon.co.uk/auth/": [f.UK, f.ROE_EU, f.ROW_EU],
                    "https://api.amazon.com/auth/": [f.ROW_NA, f.US]
                }, e.REFRESH_API = "token", e.COOKIES_API = "token/cookies", e)
            }();
        t.exports = E
    }, {
        113: 113,
        159: 159,
        178: 178,
        252: 252,
        281: 281,
        284: 284,
        294: 294,
        57: 57,
        80: 80,
        93: 93
    }],
    57: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.OAUTH = 0] = "OAUTH"
        }(n || (n = {})), t.exports = n
    }, {}],
    58: [function(e, t, i) {
        "use strict";
        var n = e(56),
            r = e(57),
            o = {
                OAuthTokenClient: n,
                TokenType: r
            };
        t.exports = o
    }, {
        56: 56,
        57: 57
    }],
    59: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(60),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.PlayStream = new t("PlayStream"), t.PlayTrailer = new t("PlayTrailer"), t)
            }(r);
        t.exports = o
    }, {
        60: 60
    }],
    60: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e) {
                this.name = e
            }
            return (e.prototype.toString = function() {
                return this.name
            }, e)
        }();
        t.exports = n
    }, {}],
    61: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(60),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.PageHit = new t("PageHit"), t.PageTouch = new t("PageTouch"), t)
            }(r);
        t.exports = o
    }, {
        60: 60
    }],
    62: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(60),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.Player = new t("Player"), t)
            }(r);
        t.exports = o
    }, {
        60: 60
    }],
    63: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(60),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.ASIN = new t("ASIN"), t)
            }(r);
        t.exports = o
    }, {
        60: 60
    }],
    64: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(60),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.Pin = new t("Pin"), t.PinError = new t("PinError"), t.Player = new t("Player"), t)
            }(r);
        t.exports = o
    }, {
        60: 60
    }],
    65: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.plr_hd_on = 0] = "plr_hd_on", e[e.plr_hd_off = 1] = "plr_hd_off", e[e.plr_nxt_ep = 2] = "plr_nxt_ep", e[e.plr_pin_ok = 3] = "plr_pin_ok", e[e.plr_b_fscrn_on = 4] = "plr_b_fscrn_on", e[e.plr_b_fscrn_off = 5] = "plr_b_fscrn_off"
        }(n || (n = {})), t.exports = n
    }, {}],
    66: [function(e, t, i) {
        "use strict";
        var n = e(59),
            r = e(60),
            o = e(61),
            s = e(62),
            a = e(63),
            l = e(65),
            c = e(64),
            u = {
                Action: n,
                ClickstreamParameter: r,
                HitType: o,
                PageType: s,
                PageTypeIdSource: a,
                RefMarker: l,
                SubPageType: c
            };
        t.exports = u
    }, {
        59: 59,
        60: 60,
        61: 61,
        62: 62,
        63: 63,
        64: 64,
        65: 65
    }],
    67: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(80),
            o = r.MarketplaceID,
            s = e(93),
            a = e(58),
            l = a.TokenType,
            c = e(178),
            u = c.HttpRequest,
            p = e(159),
            h = n.of("ClickstreamClient"),
            f = function() {
                function e(t, i, n, r, o, s, a, l) {
                    void 0 === l && (l = u), h(t, "partnerServiceEndpoint").exists(), this.clickstreamSessionId = h(i, "clickstreamSessionId").exists(), this.marketplaceId = h(n, "marketplaceId").exists(), this.customerId = h(r, "customerId").exists(), this.deviceId = h(o, "deviceId").exists(), this.deviceTypeId = h(s, "deviceTypeId").exists(), this.tokenClient = a, this.httpRequest = l, this.url = t.replace(/\/$/, "") + e.URL_PATH
                }
                return (e.isSupported = function(e) {
                    return (void 0 === e && (e = u), e.isSupported())
                }, e.prototype.report = function(t, i, n) {
                    h(t, "event").exists();
                    var r = {
                            gascEnabled: !o.isClassic(this.marketplaceId),
                            deviceID: this.deviceId,
                            deviceTypeID: this.deviceTypeId,
                            firmware: "1"
                        },
                        a = {
                            EntryList: [t]
                        },
                        c = this.httpRequest.post(this.url).withCredentials().header("x-atv-session-id", this.clickstreamSessionId).header("x-retry-count", "0").header("x-request-priority", "NOTIFICATION").accept("application/json").timeout(e.HTTP_REQUEST_TIMEOUT_MS).json(a);
                    s.isNullOrUndefined(i) || c.onLoad(function(e) {
                        i(e.getLatencyMs())
                    }), s.isNullOrUndefined(n) || c.onError(function(e, t) {
                        s.isNullOrUndefined(t) ? n(e) : n(e, t.getLatencyMs())
                    }), s.exists(this.tokenClient) ? this.tokenClient.getToken(function(e) {
                        e.type === l.OAUTH && c.bearerToken(e.token), c.query(r).send()
                    }, function(t) {
                        n(p.newError(e.CLICKSTREAM_ERROR, "An auth token error occured while trying to report clickstream: " + t))
                    }) : c.query(r).send()
                }, e.CLICKSTREAM_ERROR = "ClickstreamError", e.HTTP_REQUEST_TIMEOUT_MS = 4e3, e.URL_PATH = "/cdp/usage/Clickstream", e)
            }();
        t.exports = f
    }, {
        159: 159,
        178: 178,
        281: 281,
        58: 58,
        80: 80,
        93: 93
    }],
    68: [function(e, t, i) {
        "use strict";
        var n = e(80),
            r = n.WeblabTreatment,
            o = e(294),
            s = function() {
                function e() {
                    this.event = {}
                }
                return (e.prototype.withAction = function(e) {
                    return (this.event.action = e.toString(), this)
                }, e.prototype.withHitType = function(e) {
                    return (this.event.hitType = e.toString(), this)
                }, e.prototype.withPageType = function(e) {
                    return (this.event.pageType = e.toString(), this)
                }, e.prototype.withPageTypeId = function(e) {
                    return (this.event.pageTypeId = e, this)
                }, e.prototype.withPageTypeIdSource = function(e) {
                    return (this.event.pageTypeIdSource = e.toString(), this)
                }, e.prototype.withRefMarker = function(e) {
                    return (this.event.refMarker = e, this)
                }, e.prototype.withSubPageType = function(e) {
                    return (this.event.subPageType = e.toString(), this)
                }, e.prototype.withWeblabs = function(e) {
                    return (this.event.Weblabs = o.chain(e).keys().map(function(t) {
                        return {
                            weblabName: t,
                            treatmentName: r[e[t]]
                        }
                    }).value(), o.isEmpty(this.event.Weblabs) && delete this.event.Weblabs, this)
                }, e.prototype.withAnalyticsMap = function(e) {
                    return (this.event = e, this)
                }, e.prototype.build = function() {
                    return this.event.refMarker && this.event.pageType ? this.event : null
                }, e)
            }();
        t.exports = s
    }, {
        294: 294,
        80: 80
    }],
    69: [function(e, t, i) {
        "use strict";
        var n = e(67),
            r = e(68),
            o = {
                ClickstreamClient: n,
                ClickstreamEventBuilder: r
            };
        t.exports = o
    }, {
        67: 67,
        68: 68
    }],
    70: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = n.of("Arrays"),
            o = function() {
                function e() {}
                return (e.binarySearchIndexOf = function(e, t) {
                    return this.binarySearch(e, t, !1)
                }, e.binarySearchIndexOfClosestBelow = function(e, t) {
                    return this.binarySearch(e, t, !0)
                }, e.binarySearch = function(e, t, i) {
                    r(e, "array").exists(), r(t, "compare").exists();
                    for (var n, o, s = 0, a = e.length - 1; s <= a;) {
                        if (n = Math.floor((a + s) / 2), o = t(e[n]), 0 === o) return n;
                        o < 0 ? a = n - 1 : s = n + 1
                    }
                    return i ? Math.max(a, 0) : -1
                }, e.indexOfMinimum = function(e, t) {
                    r(e, "array").exists(), r(e.length > 0, "length > 0").is["true"](), r(t, "compare").exists();
                    for (var i = 0, n = e[0], o = 1; o < e.length; ++o) t(e[o], n) < 0 && (n = e[o], i = o);
                    return i
                }, e.indexOfMaximum = function(t, i) {
                    return (r(t, "array").exists(), r(t.length > 0, "length > 0").is["true"](), r(i, "compare").exists(), e.indexOfMinimum(t, function(e, t) {
                        return -1 * i(e, t)
                    }))
                }, e)
            }();
        t.exports = o
    }, {
        281: 281
    }],
    71: [function(e, t, i) {
        "use strict";
        var n = e(74),
            r = function() {
                function e() {}
                return (e.isAmazon = function(t, i) {
                    return (void 0 === i && (i = window), t = null === t || "undefined" == typeof t ? i.location.hostname : t, e.AMAZON_REGEX.test(t))
                }, e.fromMarketplaceId = function(t) {
                    switch (t) {
                        case n.DE:
                            return e.DE;
                        case n.JP:
                            return e.JP;
                        case n.UK:
                            return e.UK;
                        case n.ROE_EU:
                        case n.ROW_EU:
                        case n.ROW_FE:
                        case n.ROW_NA:
                            return e.PRIME_VIDEO;
                        case n.US:
                            return e.US;
                        default:
                            throw new Error("Can't get AmazonFQDN for unknown MarketplaceId! Received: " + t)
                    }
                }, e.AMAZON_REGEX = /((.*\.|^)amazon\..*|(.*\.|^)primevideo\..*)/, e.DE = "www.amazon.de", e.JP = "www.amazon.co.jp", e.UK = "www.amazon.co.uk", e.US = "www.amazon.com", e.PRIME_VIDEO = "www.primevideo.com", e)
            }();
        t.exports = r
    }, {
        74: 74
    }],
    72: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.SILVERLIGHT_PC = "A63V4FRV3YUP9", e.SILVERLIGHT_MAC = "A2G17C9GWLWFKO", e.HTML5 = "AOAGZA014O5RE", e)
        }();
        t.exports = n
    }, {}],
    73: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.BACKSPACE = 8, e.DELETE = 46, e.ESC = 27, e.SHIFT = 16, e.SPACE = 32, e.TAB = 9, e.LEFT_ARROW = 37, e.UP_ARROW = 38, e.RIGHT_ARROW = 39, e.DOWN_ARROW = 40, e.START_OF_NUMBER = 48, e.END_OF_NUMBER = 57, e.START_OF_NUMPAD = 96, e.END_OF_NUMPAD = 105, e.A = 65, e.C = 67, e.F = 70, e.M = 77, e)
        }();
        t.exports = n
    }, {}],
    74: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.isClassic = function(t) {
                switch (t) {
                    case e.DE:
                    case e.JP:
                    case e.UK:
                    case e.US:
                        return !0
                }
                return !1
            }, e.ROE_EU = "A3K6Y4MI8GDYMT", e.ROW_EU = "A2MFUE2XK8ZSSY", e.ROW_FE = "A15PK738MTQHSO", e.ROW_NA = "ART4WZ8MWBX2Y", e.DE = "A1PA6795UKMFR9", e.JP = "A1VC38T7YXB528", e.UK = "A1F83G8C2ARO7P", e.US = "ATVPDKIKX0DER", e)
        }();
        t.exports = n
    }, {}],
    75: [function(e, t, i) {
        "use strict";
        var n = e(74),
            r = function() {
                function e() {}
                return (e.fromMarketplaceId = function(t, i, n) {
                    void 0 === i && (i = !0), void 0 === n && (n = window);
                    var r = e.getDomain(t, i, n);
                    return e.PROTOCOL + e.getSubdomain(t, r) + r
                }, e.getDomain = function(t, i, r) {
                    if (i) {
                        var o = r.location.host.match(e.DOMAIN_REGEX);
                        if (null !== o) return o[1];
                        if (t === n.US && (o = r.location.host.match(e.IMDB_REGEX), null !== o)) return o[1]
                    }
                    return e.getDomainFromMarketplaceId(t)
                }, e.getSubdomain = function(t, i) {
                    if (i === e.IMDB_DOMAIN) return e.IMDB_SUBDOMAIN;
                    switch (t) {
                        case n.ROW_NA:
                        case n.US:
                            return e.US_SUBDOMAIN;
                        case n.ROE_EU:
                        case n.ROW_EU:
                        case n.UK:
                        case n.DE:
                            return e.EU_SUBDOMAIN;
                        case n.ROW_FE:
                        case n.JP:
                            return e.JP_SUBDOMAIN;
                        default:
                            throw new Error("Can't get PartnerServiceEndpoint for unknown MarketplaceId! Received: " + t)
                    }
                }, e.getDomainFromMarketplaceId = function(e) {
                    switch (e) {
                        case n.ROW_NA:
                        case n.ROE_EU:
                        case n.ROW_EU:
                        case n.ROW_FE:
                            return ".primevideo.com";
                        case n.DE:
                            return ".amazon.de";
                        case n.JP:
                            return ".amazon.co.jp";
                        case n.UK:
                            return ".amazon.co.uk";
                        default:
                            return ".amazon.com"
                    }
                }, e.EU_SUBDOMAIN = "atv-ps-eu", e.JP_SUBDOMAIN = "atv-ps-fe", e.US_SUBDOMAIN = "atv-ps", e.IMDB_DOMAIN = ".imdb.com", e.IMDB_SUBDOMAIN = "primevideo-dp-na", e.DOMAIN_REGEX = /.*(\.(amazon|primevideo)\..*)/, e.IMDB_REGEX = /.*(\.imdb\.com)/, e.PROTOCOL = "https://", e)
            }();
        t.exports = r
    }, {
        74: 74
    }],
    76: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.DiagnosticsEndpointKey = "diagnostics_url", e)
        }();
        t.exports = n
    }, {}],
    77: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.EXTRA_SMALL_SCREEN_MAX_WIDTH = 479, e.SMALL_SCREEN_MIN_WIDTH = 480, e.MEDIUM_SCREEN_MIN_WIDTH = 640, e.LARGE_SCREEN_MIN_WIDTH = 1200, e.EXTRA_LARGE_SCREEN_MIN_WIDTH = 1921, e.SCALE_FACTOR_EXTRA_SMALL = 1, e.SCALE_FACTOR_SMALL = 100 / e.SMALL_SCREEN_MIN_WIDTH, e.SCALE_FACTOR_MEDIUM = 100 / e.MEDIUM_SCREEN_MIN_WIDTH, e.SCALE_FACTOR_LARGE = 100 / e.LARGE_SCREEN_MIN_WIDTH, e.SCALE_FACTOR_EXTRA_LARGE = 100 / e.EXTRA_LARGE_SCREEN_MIN_WIDTH, e.SCREEN_EXTRA_SMALL_MEDIA_QUERY = "(max-width: " + e.EXTRA_SMALL_SCREEN_MAX_WIDTH + "px)", e.SCREEN_SMALL_MEDIA_QUERY = "(min-width: " + e.SMALL_SCREEN_MIN_WIDTH + "px)", e.SCREEN_MEDIUM_MEDIA_QUERY = "(min-width: " + e.MEDIUM_SCREEN_MIN_WIDTH + "px)", e.SCREEN_LARGE_MEDIA_QUERY = "(min-width: " + e.LARGE_SCREEN_MIN_WIDTH + "px)", e.SCREEN_EXTRA_LARGE_MEDIA_QUERY = "(min-width: " + e.EXTRA_LARGE_SCREEN_MIN_WIDTH + "px)", e)
        }();
        t.exports = n
    }, {}],
    78: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.C = 0] = "C", e[e.T1 = 1] = "T1", e[e.T2 = 2] = "T2", e[e.T3 = 3] = "T3", e[e.T4 = 4] = "T4", e[e.T5 = 5] = "T5", e[e.T6 = 6] = "T6", e[e.T7 = 7] = "T7", e[e.T8 = 8] = "T8", e[e.T9 = 9] = "T9", e[e.T10 = 10] = "T10", e[e.T11 = 11] = "T11", e[e.T12 = 12] = "T12", e[e.T13 = 13] = "T13", e[e.T14 = 14] = "T14", e[e.T15 = 15] = "T15", e[e.T16 = 16] = "T16", e[e.T17 = 17] = "T17", e[e.T18 = 18] = "T18", e[e.T19 = 19] = "T19", e[e.T20 = 20] = "T20"
        }(n || (n = {})), t.exports = n
    }, {}],
    79: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.getActiveWeblabs = function() {
                return [e.ATV_WEB_PLAYER_OAUTH, e.DISABLE_SILVERLIGHT, e.HTML5_CAROUSEL, e.HTML5_ENABLE_CHROMIUM, e.HTML5_ENABLE_SAFARI_STARTOVER, e.HTML5_HEVC, e.HTML5_NETWORK_SEED, e.HTML5_NETWORK_STATUS_TOAST, e.HTML5_PICTUREINPICTURE, e.HTML5_REMOVE_BUFFER_SIZE_RATIO, e.HTML5_SAFARI_CVBR, e.HTML5_XRAY_ENCODER_TIMESTAMP_WEBLAB, e.HASTUR_ALL_CLIENT_IDS, e.HASTUR_CHROMIUM, e.HASTUR_EDGE, e.HASTUR_IE, e.HASTUR_SAFARI, e.LIVE_LOW_LATENCY_CHROME, e.LIVE_LOW_LATENCY_CHROMIUM, e.LIVE_LOW_LATENCY_EDGENEXT, e.LIVE_LOW_LATENCY_FIREFOX, e.LIVE_LOW_LATENCY_FIREFOX64, e.LIVE_LOW_LATENCY_OPERA]
            }, e.ATV_WEB_PLAYER_OAUTH = "ATV_WEB_PLAYER_OAUTH_74228", e.DISABLE_SILVERLIGHT = "AIV_WEB_PLAYER_DISABLE_SILVERLIGHT_91138", e.HTML5_CAROUSEL = "AIV_HTML5_MIRO_CAROUSEL_196734", e.HTML5_ENABLE_CHROMIUM = "AIV_HTML5_ENABLE_CHROMIUM_251485", e.HTML5_ENABLE_SAFARI_STARTOVER = "HTML5_ENABLE_SAFARI_STARTOVER_228094", e.HTML5_HEVC = "AIV_HTML5_HEVC_79711", e.HTML5_NETWORK_SEED = "AIV_HTML5_NETWORK_SEED_95840", e.HTML5_NETWORK_STATUS_TOAST = "AIV_HTML5_NETWORK_STATUS_TOAST_98636", e.HTML5_PICTUREINPICTURE = "AIV_HTML5_PICTUREINPICTURE_LAUNCH_309146", e.HTML5_REMOVE_BUFFER_SIZE_RATIO = "AIV_HTML5_REMOVE_BUFFER_SIZE_RATIO_99314", e.HTML5_SAFARI_CVBR = "AIV_HTML5_SAFARI_CVBR_93874", e.HTML5_XRAY_ENCODER_TIMESTAMP_WEBLAB = "AIV_XRAY_LIVE_ENCODER_TIMESTAMP_241326", e.HASTUR_CHROMIUM = "AIV_HTML5_HASTUR_LAUNCH_CHROMIUM_227119", e.HASTUR_EDGE = "AIV_HTML5_HASTUR_LAUNCH_EDGE_227134", e.HASTUR_IE = "AIV_HTML5_HASTUR_LAUNCH_IE_227137", e.HASTUR_SAFARI = "AIV_HTML5_HASTUR_LAUNCH_SAFARI_227138", e.HASTUR_CHROMEOS = "AIV_HTML5_HASTUR_LAUNCH_CHROMEOS_257480", e.HASTUR_LINUX = "AIV_HTML5_HASTUR_LAUNCH_LINUX_257479", e.HASTUR_ALL_CLIENT_IDS = "AIV_HTML5_HASTUR_LAUNCH_REMAINING_CLIENT_IDS_300512", e.LIVE_LOW_LATENCY_CHROME = "AIV_HTML5_LIVE_LOW_LATENCY_CHROME_242583", e.LIVE_LOW_LATENCY_CHROMIUM = "AIV_HTML5_LIVE_LOW_LATENCY_CHROMIUM_242584", e.LIVE_LOW_LATENCY_EDGENEXT = "AIV_HTML5_LIVE_LOW_LATENCY_EDGENEXT_306349", e.LIVE_LOW_LATENCY_FIREFOX = "AIV_HTML5_LIVE_LOW_LATENCY_FIREFOX_242581", e.LIVE_LOW_LATENCY_FIREFOX64 = "AIV_HTML5_LIVE_LOW_LATENCY_FIREFOX64_242582", e.LIVE_LOW_LATENCY_OPERA = "AIV_HTML5_LIVE_LOW_LATENCY_OPERA_242580", e)
        }();
        t.exports = n
    }, {}],
    80: [function(e, t, i) {
        "use strict";
        var n = e(71),
            r = e(72),
            o = e(73),
            s = e(74),
            a = e(75),
            l = e(76),
            c = e(77),
            u = e(79),
            p = e(78),
            h = {
                AmazonFQDN: n,
                DeviceTypeId: r,
                KeyCode: o,
                MarketplaceID: s,
                PartnerServiceEndpoint: a,
                StorageKeys: l,
                Styling: c,
                Weblabs: u,
                WeblabTreatment: p
            };
        t.exports = h
    }, {
        71: 71,
        72: 72,
        73: 73,
        74: 74,
        75: 75,
        76: 76,
        77: 77,
        78: 78,
        79: 79
    }],
    81: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(283),
            o = n.of("BasicEventEmitter"),
            s = function() {
                function e() {
                    this.eventEmitter = new r.EventEmitter2
                }
                return (e.prototype.on = function(e, t) {
                    var i = this.getKey(e);
                    this.eventEmitter.on(i, t)
                }, e.prototype.onAny = function(e) {
                    this.eventEmitter.onAny(e)
                }, e.prototype.once = function(e, t) {
                    var i = this.getKey(e);
                    this.eventEmitter.once(i, t)
                }, e.prototype.many = function(e, t, i) {
                    var n = this.getKey(e);
                    this.eventEmitter.many(n, t, i)
                }, e.prototype.off = function(e, t) {
                    var i = this.getKey(e);
                    this.eventEmitter.off(i, t)
                }, e.prototype.offAny = function(e) {
                    this.eventEmitter.offAny(e)
                }, e.prototype.emit = function(e, t) {
                    var i = this.getKey(e),
                        n = "undefined" != typeof t ? t : e;
                    this.eventEmitter.emit(i, n)
                }, e.prototype.getKey = function(e) {
                    return (o(e, "eventType").exists(), o(e.toString, "toString").is.a["function"](), e.toString())
                }, e)
            }();
        t.exports = s
    }, {
        281: 281,
        283: 283
    }],
    82: [function(e, t, i) {
        "use strict";
        var n = e(81),
            r = {
                BasicEventEmitter: n
            };
        t.exports = r
    }, {
        81: 81
    }],
    83: [function(e, t, i) {
        "use strict";
        var n = e(284),
            r = e(294),
            o = n.get("FiniteStateMachine"),
            s = function() {
                function e(e) {
                    this.currentState = e, this.states = {}, this.transitionCallbacks = [], this.transitionsInProgress = 0, this.illegalTransitionCallbacks = [], this.stateCallbacks = {}
                }
                return (e.prototype.addTransition = function(e, t) {
                    this.states[t] = r.union(this.states[t], [e]), r.isUndefined(this.stateCallbacks[t]) && (this.stateCallbacks[t] = [])
                }, e.prototype.onTransition = function(e) {
                    this.transitionCallbacks = r.union(this.transitionCallbacks, [e])
                }, e.prototype.onIllegalTransition = function(e) {
                    this.illegalTransitionCallbacks = r.union(this.illegalTransitionCallbacks, [e])
                }, e.prototype.onTransitionTo = function(e, t) {
                    this.stateCallbacks[e] = r.union(this.stateCallbacks[e], [t])
                }, e.prototype.transitionTo = function(e, t, i, n) {
                    if (r.isUndefined(this.states[e]) || !r.contains(this.states[e], this.currentState)) return (this.callCallbacks(this.illegalTransitionCallbacks, this.currentState, e), !1);
                    if (this.transitionsInProgress > 0 && o.warn("Requesting state transition to nextState: " + e + " while " + this.transitionsInProgress + " transitions are already in progress."), this.transitionsInProgress++, !r.isUndefined(i) && !r.isNull(i) && !i()) return (this.transitionsInProgress--, !1);
                    var s = this.currentState;
                    return (this.currentState = e, r.isUndefined(n) || r.isNull(n) || n() ? (this.callCallbacks(this.transitionCallbacks, s, e, t), this.callCallbacks(this.stateCallbacks[e], s, e, t), this.transitionsInProgress--, !0) : (this.transitionsInProgress--, this.currentState = s, !1))
                }, e.prototype.canTransitionTo = function(e) {
                    return !r.isUndefined(this.states[e]) && r.contains(this.states[e], this.currentState)
                }, e.prototype.getCurrentState = function() {
                    return this.currentState
                }, e.prototype.callCallbacks = function(e) {
                    for (var t = [], i = 1; i < arguments.length; i++) t[i - 1] = arguments[i];
                    r.each(e, function(e) {
                        e.apply(null, t)
                    })
                }, e)
            }();
        t.exports = s
    }, {
        284: 284,
        294: 294
    }],
    84: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = n.of("CappedGeometricSequence"),
            o = function() {
                function e(e) {
                    void 0 === e && (e = Math.random), this.initialValue = 0, this.exponentialFactor = 1, this.randomizationFactor = 0, this.maxValue = -1, this.random = e
                }
                return (e.prototype.withInitialValue = function(e) {
                    return (r(e).exists(), r(e >= 0, "initialValue >= 0").is["true"](), this.initialValue = e, this)
                }, e.prototype.withExponentialFactor = function(e) {
                    return (r(e > 0, "exponentialFactor > 0").is["true"](), this.exponentialFactor = e, this)
                }, e.prototype.withRandomizationFactor = function(e) {
                    return (r(e).exists(), r(e >= 0, "randomizationFactor >= 0").is["true"](), this.randomizationFactor = e, this)
                }, e.prototype.withMaxValue = function(e) {
                    return (r(e > 0, "maxValue > 0").is["true"](), this.maxValue = e, this)
                }, e.prototype.getValue = function(e) {
                    r(e > 0, "n > 0").is["true"]();
                    var t = this.initialValue * Math.pow(this.exponentialFactor, e - 1) + Math.floor(this.random() * this.randomizationFactor * e);
                    return (this.maxValue !== -1 && t > this.maxValue && (t = this.maxValue), t)
                }, e)
            }();
        t.exports = o
    }, {
        281: 281
    }],
    85: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e) {
                this.itemHeap = [], this.comparator = e
            }
            return (e.prototype.push = function(e) {
                this.itemHeap.push(e), this["float"](this.itemHeap.length - 1)
            }, e.prototype.remove = function(e) {
                var t = this.itemHeap.indexOf(e);
                return t !== -1 && (t === this.itemHeap.length - 1 ? (this.itemHeap.pop(), !0) : (this.itemHeap[t] = this.itemHeap.pop(), this.comparator(this.itemHeap[t], e) ? this["float"](t) : this.sink(t), !0))
            }, e.prototype.pop = function() {
                if (0 === this.itemHeap.length) return null;
                if (1 === this.itemHeap.length) return this.itemHeap.pop();
                var e = this.itemHeap[0];
                return (this.itemHeap[0] = this.itemHeap.pop(), this.sink(0), e)
            }, e.prototype.size = function() {
                return this.itemHeap.length
            }, e.prototype["float"] = function(t) {
                if (0 !== t) {
                    var i = e.parent(t),
                        n = this.itemHeap[i];
                    this.comparator(this.itemHeap[t], n) && (this.itemHeap[i] = this.itemHeap[t], this.itemHeap[t] = n, this["float"](i))
                }
            }, e.prototype.sink = function(t) {
                if (t !== this.itemHeap.length - 1) {
                    var i = e.leftChild(t);
                    if (!(i >= this.itemHeap.length)) {
                        var n = this.itemHeap[i];
                        this.comparator(n, this.itemHeap[t]) && (this.itemHeap[i] = this.itemHeap[t], this.itemHeap[t] = n, this.sink(i))
                    }
                }
            }, e.leftChild = function(e) {
                return 2 * e + 1
            }, e.parent = function(e) {
                return e % 2 === 1 ? (e - 1) / 2 : e / 2
            }, e)
        }();
        t.exports = n
    }, {}],
    86: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(90),
            o = n.of("LinearRegressionCalculator"),
            s = function() {
                function e(e) {
                    this.APPRX_ZERO = 1e-6, o(e > 0, "maxSamples > 0").is["true"](), this.xSampleAggregator = new r(e), this.ySampleAggregator = new r(e)
                }
                return (e.prototype.addPoint = function(e, t) {
                    o(e, "x").exists(), o(t, "y").exists(), this.xSampleAggregator.addSample(e), this.ySampleAggregator.addSample(t)
                }, e.prototype.reset = function() {
                    this.xSampleAggregator.reset(), this.ySampleAggregator.reset()
                }, e.prototype.linearRegression = function(e) {
                    o(e >= 2, "windowSize >= 2.").is["true"]();
                    var t = this.xSampleAggregator.getSamplesInWindow(e),
                        i = this.ySampleAggregator.getSamplesInWindow(e);
                    o(t.length === i.length, "sample length equal").is["true"]();
                    var n = t.length;
                    if (n < 2) return {
                        slope: 0,
                        yIntercept: 0,
                        determinationCoefficient: 0
                    };
                    for (var r = this.xSampleAggregator.getAverage(n), s = this.ySampleAggregator.getAverage(n), a = this.xSampleAggregator.getStdDev(n), l = this.ySampleAggregator.getStdDev(n), c = 0, u = 0, p = 0; p < n; ++p) {
                        var h = t[p] - r,
                            f = i[p] - s;
                        c += h * f, u += h * h
                    }
                    if (0 === u) return {
                        slope: 0,
                        yIntercept: 0,
                        determinationCoefficient: 0
                    };
                    var d, y = c / u,
                        g = s - y * r;
                    return (d = Math.abs(l) < this.APPRX_ZERO ? 0 : y / (l / a), {
                        slope: y,
                        yIntercept: g,
                        determinationCoefficient: d * d
                    })
                }, e)
            }();
        t.exports = s
    }, {
        281: 281,
        90: 90
    }],
    87: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(70),
            o = e(93),
            s = n.of("NumberLine"),
            a = function() {
                function e(e) {
                    void 0 === e && (e = Number.MIN_VALUE), this.intervals = [], this.epsilon = e
                }
                return (e.prototype.add = function(e) {
                    var t = this;
                    s(e, "interval").exists(), s(e.start < e.end, "interval.start(" + e.start + ") < interval.end(" + e.end + ")").is["true"]();
                    var i = r.binarySearchIndexOfClosestBelow(this.intervals, function(i) {
                        return t.compare(e.start, i.start)
                    });
                    for (i < this.intervals.length && this.compare(e.start, this.intervals[i].start) > 0 && i++, this.intervals.splice(i, 0, {
                            start: e.start,
                            end: e.end
                        }), i > 0 && this.compare(this.intervals[i - 1].end, this.intervals[i].start) >= 0 && i--; i + 1 < this.intervals.length && this.compare(this.intervals[i + 1].start, this.intervals[i].end) <= 0;) this.intervals[i].end = Math.max(this.intervals[i].end, this.intervals[i + 1].end), this.intervals.splice(i + 1, 1)
                }, e.prototype.remove = function(e) {
                    var t = this;
                    s(e, "interval").exists(), s(e.start < e.end, "interval.start > interval.end").is["true"]();
                    for (var i = r.binarySearchIndexOfClosestBelow(this.intervals, function(i) {
                            return t.compare(e.start, i.start)
                        }); i < this.intervals.length && this.compare(this.intervals[i].start, e.end) < 0;) {
                        var n = this.intervalDifference(this.intervals[i], e);
                        (o = this.intervals).splice.apply(o, [i, 1].concat(n)), i += n.length
                    }
                    var o
                }, e.prototype.find = function(e) {
                    var t = this;
                    s(e, "point").exists();
                    var i = r.binarySearchIndexOf(this.intervals, function(i) {
                        return t.compare(e, i.start) < 0 ? -1 : t.compare(e, i.end) > 0 ? 1 : 0
                    });
                    return i === -1 ? null : this.intervals[i]
                }, e.prototype.union = function(t) {
                    s(t, "other").exists();
                    for (var i = new e(this.epsilon), n = 0, r = 0, o = -1; n < t.intervals.length || r < this.intervals.length;) {
                        var a = null;
                        a = r >= this.intervals.length || n < t.intervals.length && this.compare(t.intervals[n].start, this.intervals[r].start) < 0 ? t.intervals[n++] : this.intervals[r++], o === -1 || this.compare(i.intervals[o].end, a.start) < 0 ? (i.intervals.push({
                            start: a.start,
                            end: a.end
                        }), o++) : i.intervals[o].end = Math.max(i.intervals[o].end, a.end)
                    }
                    return i
                }, e.prototype.intersection = function(t) {
                    s(t, "other").exists();
                    for (var i = new e(this.epsilon), n = 0, r = 0; n < this.intervals.length && r < t.intervals.length;) {
                        var a = this.intervalIntersection(this.intervals[n], t.intervals[r]);
                        o.exists(a) && i.intervals.push(a), this.compare(this.intervals[n].end, t.intervals[r].end) > 0 ? r++ : n++
                    }
                    return i
                }, e.prototype.shift = function(t) {
                    s(t, "value").exists();
                    for (var i = new e(this.epsilon), n = 0, r = this.intervals; n < r.length; n++) {
                        var o = r[n];
                        i.add({
                            start: o.start + t,
                            end: o.end + t
                        })
                    }
                    return i
                }, e.prototype.getIntervals = function() {
                    return this.intervals
                }, e.prototype.intervalIntersection = function(e, t) {
                    return this.compare(e.end, t.start) <= 0 ? null : this.compare(e.start, t.end) >= 0 ? null : {
                        start: Math.max(e.start, t.start),
                        end: Math.min(e.end, t.end)
                    }
                }, e.prototype.intervalDifference = function(e, t) {
                    return this.compare(e.start, t.end) >= 0 || this.compare(e.end, t.start) < 0 ? [e] : this.compare(e.start, t.start) >= 0 && this.compare(e.end, t.end) <= 0 ? [] : this.compare(e.start, t.start) < 0 && this.compare(e.end, t.end) > 0 ? [{
                        start: e.start,
                        end: t.start
                    }, {
                        start: t.end,
                        end: e.end
                    }] : this.compare(e.start, t.start) < 0 ? [{
                        start: e.start,
                        end: t.start
                    }] : this.compare(e.end, t.end) > 0 ? [{
                        start: t.end,
                        end: e.end
                    }] : void 0
                }, e.prototype.compare = function(e, t) {
                    var i = e - t;
                    return i < -this.epsilon ? -1 : i > this.epsilon ? 1 : 0
                }, e)
            }();
        t.exports = a
    }, {
        281: 281,
        70: 70,
        93: 93
    }],
    88: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = n.of("SampleAggregator"),
            o = function() {
                function e() {}
                return (e.average = function(e) {
                    r(e, "values").exists(), r(e.length > 0, "length > 0").is["true"]();
                    var t = e.reduce(function(e, t) {
                        return e + t
                    }, 0);
                    return t / e.length
                }, e.stdDev = function(t) {
                    r(t, "values").exists(), r(t.length > 0, "length > 0").is["true"]();
                    var i = e.average(t),
                        n = t.reduce(function(e, t) {
                            return e + (t - i) * (t - i)
                        }, 0);
                    return Math.sqrt(n / t.length)
                }, e.lerp = function(e, t, i) {
                    return (r(e, "start").exists(), r(t, "end").exists(), r(i, "amount").exists(), e + (t - e) * Math.min(Math.max(i, 0), 1))
                }, e.exponentialInterp = function(t, i, n, o) {
                    return (r(t, "exponentialFactor").exists(), r(0 !== t, "exponentialFactor !== 0").is["true"](), r(i, "start").exists(), r(n, "end").exists(), r(o, "amount").exists(), e.lerp(i, n, (Math.exp(t * Math.min(Math.max(o, 0), 1)) - 1) / (Math.exp(t) - 1)))
                }, e.sinInterp = function(e, t, i) {
                    return (r(e, "start").exists(), r(t, "end").exists(), r(i, "amount").exists(), e + (t - e) * Math.sin(Math.PI / 2 * i))
                }, e.isApprxEqual = function(e, t, i) {
                    return (r(e, "value").exists(), r(t, "expected").exists(), r(i, "delta").exists(), r(i >= 0, "delta >= 0").is["true"](), Math.abs(t - e) <= i)
                }, e)
            }();
        t.exports = o
    }, {
        281: 281
    }],
    89: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(294),
            o = e(88),
            s = n.of("PiecewiseLinearFunction"),
            a = function() {
                function e(e) {
                    s(e, "coordinates").exists(), s(e.length > 0, "length > 0").is["true"]();
                    var t = r.unzip(e);
                    this.xCoords = t[0], this.yCoords = t[1];
                    for (var i = 0; i < this.xCoords.length - 1; ++i) s(this.xCoords[i] < this.xCoords[i + 1], "x coordinates must be ordered and unique.").is["true"]()
                }
                return (e.prototype.evaluate = function(e) {
                    if (s(e, "xValue").exists(), e >= this.xCoords[this.xCoords.length - 1]) return this.yCoords[this.xCoords.length - 1];
                    for (var t = this.xCoords.length - 2; t >= 0; --t)
                        if (e >= this.xCoords[t]) return o.lerp(this.yCoords[t], this.yCoords[t + 1], (e - this.xCoords[t]) / (this.xCoords[t + 1] - this.xCoords[t]));
                    return this.yCoords[0]
                }, e)
            }();
        t.exports = a
    }, {
        281: 281,
        294: 294,
        88: 88
    }],
    90: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(88),
            o = e(294),
            s = n.of("SampleAggregator"),
            a = function() {
                function e(e) {
                    s(e > 0, "maxSamples > 0.").is["true"](), this.maxSamples = e, this.reset()
                }
                return (e.prototype.addSample = function(e) {
                    s(e, "sample").exists(), this.samples.length >= this.maxSamples && this.samples.shift(), this.samples.push(e)
                }, e.prototype.reset = function() {
                    this.samples = []
                }, e.prototype.getAverage = function(e) {
                    s(e > 0, "windowSize > 0").is["true"]();
                    var t = this.getSamplesInWindow(e);
                    return 0 === t.length ? 0 : r.average(t)
                }, e.prototype.getStdDev = function(e) {
                    s(e > 0, "windowSize > 0").is["true"]();
                    var t = this.getSamplesInWindow(e);
                    return 0 === t.length ? 0 : r.stdDev(t)
                }, e.prototype.getSum = function(e) {
                    s(e > 0, "windowSize > 0").is["true"]();
                    var t = this.getSamplesInWindow(e);
                    return 0 === t.length ? 0 : t.reduce(function(e, t) {
                        return e + t
                    }, 0)
                }, e.prototype.getMinimum = function(e) {
                    s(e > 0, "windowSize > 0").is["true"]();
                    var t = this.getSamplesInWindow(e);
                    return 0 === t.length ? 0 : o.min(t)
                }, e.prototype.getSamplesInWindow = function(e) {
                    return o.last(this.samples, Math.min(this.maxSamples, e))
                }, e)
            }();
        t.exports = a
    }, {
        281: 281,
        294: 294,
        88: 88
    }],
    91: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(294),
            o = n.of("SlidingWindowEventTracker"),
            s = function() {
                function e(e, t) {
                    void 0 === t && (t = r.now), this.eventTimestamps = [], o(e > 0, "windowLengthMs > 0").is["true"](), this.windowLengthMs = e, this.now = t
                }
                return (e.prototype.recordEvent = function(e) {
                    (r.isNull(e) || r.isUndefined(e)) && (e = this.now()), o(e > 0, "eventTimestamp > 0").is["true"](), this.eventTimestamps.push(e)
                }, e.prototype.getEventCountInWindow = function() {
                    return (this.purgeRecordsOutsideWindow(), this.eventTimestamps.length)
                }, e.prototype.purgeRecordsOutsideWindow = function() {
                    var e = this,
                        t = this.now();
                    this.eventTimestamps = r.filter(this.eventTimestamps, function(i) {
                        return t - i <= e.windowLengthMs
                    })
                }, e)
            }();
        t.exports = s
    }, {
        281: 281,
        294: 294
    }],
    92: [function(e, t, i) {
        "use strict";
        var n = e(88),
            r = e(84),
            o = e(86),
            s = e(87),
            a = e(89),
            l = e(85),
            c = e(90),
            u = e(91),
            p = {
                CappedGeometricSequence: r,
                LinearRegressionCalculator: o,
                NumberLine: s,
                PiecewiseLinearFunction: a,
                PriorityQueue: l,
                SampleAggregator: c,
                SlidingWindowEventTracker: u,
                stdDev: n.stdDev,
                average: n.average,
                lerp: n.lerp,
                exponentialInterp: n.exponentialInterp,
                isApprxEqual: n.isApprxEqual,
                sinInterp: n.sinInterp
            };
        t.exports = p
    }, {
        84: 84,
        85: 85,
        86: 86,
        87: 87,
        88: 88,
        89: 89,
        90: 90,
        91: 91
    }],
    93: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(294),
            o = n.of("Objects"),
            s = function() {
                function e() {}
                return (e.withDefault = function(e, t) {
                    return this.isNullOrUndefined(e) ? t : e
                }, e.exists = function(e) {
                    return null !== e && "undefined" != typeof e
                }, e.isNullOrUndefined = function(e) {
                    return null === e || "undefined" == typeof e
                }, e.isUndefined = function(e) {
                    return "undefined" == typeof e
                }, e.isPropertyNullOrUndefined = function(e, t) {
                    return (o(t, "propertyName").exists(), this.isNullOrUndefined(e) || this.isNullOrUndefined(e[t]))
                }, e.nestedValue = function(t, i, n) {
                    void 0 === n && (n = null), o(i, "keys").exists();
                    var s;
                    if (s = "string" == typeof i ? i.split(".") : i, 0 === s.length) return n;
                    var a = r.reduce(s, function(t, i) {
                        return e.isNullOrUndefined(t) ? null : t[i]
                    }, t);
                    return e.isNullOrUndefined(a) ? n : a
                }, e.deepTraverse = function(t, i) {
                    for (var n in t) t.hasOwnProperty(n) && (i(t, n, t[n]), e.exists(t[n]) && "object" == typeof t[n] && e.deepTraverse(t[n], i))
                }, e.deepExtend = function(t, i) {
                    if (e.isNullOrUndefined(i)) return e.withDefault(r.clone(t), null);
                    if (e.isNullOrUndefined(t)) return r.clone(i);
                    for (var n = {}, o = r.union(r.keys(t), r.keys(i)), s = 0, a = o; s < a.length; s++) {
                        var l = a[s];
                        r.isUndefined(t[l]) ? n[l] = r.clone(i[l]) : r.isObject(t[l]) && r.isObject(i[l]) && !r.isArray(t[l]) && !r.isArray(i[l]) ? n[l] = this.deepExtend(t[l], i[l]) : n[l] = i.hasOwnProperty(l) ? i[l] : t[l]
                    }
                    return n
                }, e)
            }();
        t.exports = s
    }, {
        281: 281,
        294: 294
    }],
    94: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e, t) {
                void 0 === e && (e = Date), this.dateConstructor = e, this.performanceRef = "undefined" == typeof t && "undefined" != typeof performance ? performance : t
            }
            return (e.prototype.getHighResRelativeTimeMs = function() {
                return "undefined" == typeof this.performanceRef ? (new this.dateConstructor).getTime() : this.performanceRef.now()
            }, e)
        }();
        t.exports = n
    }, {}],
    95: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(94),
            o = n.of("Time"),
            s = {
                TimeProvider: r,
                millisecondsToHoursMinutesSeconds: function(e) {
                    if (e <= 0) return {
                        hours: 0,
                        minutes: 0,
                        seconds: 0
                    };
                    if (e <= 1e3) return {
                        hours: 0,
                        minutes: 0,
                        seconds: 1
                    };
                    var t = Math.floor(e / 36e5),
                        i = Math.floor(e % 36e5 / 6e4),
                        n = Math.round(e % 6e4 / 1e3);
                    return (60 === n && (n = 0, i++, 60 === i && (i = 0, t++)), {
                        hours: t,
                        minutes: i,
                        seconds: n
                    })
                },
                unixTimestampToHoursMinutesSeconds: function(e) {
                    if (e <= 0) return {
                        hours: 0,
                        minutes: 0,
                        seconds: 0
                    };
                    var t = new Date(e);
                    return {
                        hours: t.getHours(),
                        minutes: t.getMinutes(),
                        seconds: t.getSeconds()
                    }
                },
                isoToMillis: function(e) {
                    o(e, "isoString").exists(), o(e).is.a.string(), o(e.length > 3, "length > 3").is["true"]();
                    var t = new RegExp("PT([0-9]*H)*([0-9]*M)*((?!0\\d)\\d*(\\.\\d+)?S)*");
                    if (!t.test(e)) throw new Error("Invalid ISO Time String passed into isoToMillis");
                    e = e.substring(2);
                    var i = e.indexOf("H"),
                        n = e.indexOf("M"),
                        r = e.indexOf("S"),
                        s = i > 0 ? Number(e.substring(0, i)) : 0,
                        a = n > 0 ? Number(e.substring(i + 1, n)) : 0,
                        l = r > 0 ? Number(e.substring(Math.max(i, n) + 1, r)) : 0;
                    return Math.round(1e3 * (60 * s * 60 + 60 * a + l))
                },
                formatTime: function(e, t) {
                    void 0 === t && (t = !1), o(e, "hms").exists();
                    var i = e.minutes < 10 ? "0" + e.minutes : e.minutes.toString(),
                        n = e.seconds < 10 ? "0" + e.seconds : e.seconds.toString();
                    return e.hours > 0 || t === !0 ? e.hours + ":" + i + ":" + n : i + ":" + n
                },
                dateTimeToString: function(e) {
                    var t = function(e, t) {
                        return (t + e).slice(-t.length)
                    };
                    return t(e.getFullYear(), "0000") + "-" + t(e.getMonth() + 1, "00") + "-" + t(e.getDate(), "00") + " " + t(e.getHours(), "00") + ":" + t(e.getMinutes(), "00") + ":" + t(e.getSeconds(), "00") + "." + t(e.getMilliseconds(), "000")
                }
            };
        t.exports = s
    }, {
        281: 281,
        94: 94
    }],
    96: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(294),
            o = e(71),
            s = n.of("AmazonUrlCreator"),
            a = function() {
                function e(e, t, i) {
                    void 0 === i && (i = window), this.marketplaceId = null, this.csUrlPathMap = null, this.marketplaceId = s(e, "marketplaceId").exists(), this.csUrlPathMap = s(t, "csUrlPathMap").exists(), this.windowRef = s(i, "windowRef").exists()
                }
                return (e.prototype.createCSUrl = function(t) {
                    s(this.csUrlPathMap[t], "csUrlPathMap[csUrlPathKey]").exists();
                    var i = this.windowRef.location.hostname,
                        n = i,
                        a = this.csUrlPathMap[t][n];
                    (r.isNull(a) || r.isUndefined(a)) && (i = o.fromMarketplaceId(this.marketplaceId), n = this.marketplaceId);
                    var l = this.csUrlPathMap[t][n];
                    return ((r.isNull(l) || r.isUndefined(l)) && (n = e.DEFAULT_ID), i + this.csUrlPathMap[t][n])
                }, e.prototype.createDetailPageUrl = function(t) {
                    var i = this.windowRef.location.hostname;
                    return (o.isAmazon(i, this.windowRef) || (i = o.fromMarketplaceId(this.marketplaceId)), i + e.DETAIL_PAGE_BASE + t)
                }, e.DEFAULT_ID = "*", e.DETAIL_PAGE_BASE = "/dp/", e)
            }();
        t.exports = a
    }, {
        281: 281,
        294: 294,
        71: 71
    }],
    97: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = n.of("DOMUtils"),
            o = function() {
                function e() {}
                return (e.getAttributeOrThrow = function(e, t) {
                    r(e, "element").exists(), r(t, "attribute").exists();
                    var i = e.getAttribute(t);
                    if (null === i) throw new Error("Unable to get attribute '" + t + "'");
                    return i
                }, e.querySelectOrThrow = function(e, t) {
                    r(e, "element").exists(), r(t, "query").exists();
                    var i = e.querySelector(t);
                    if (null === i) throw new Error("Unable to querySelect '" + t + "'");
                    return i
                }, e.querySelectAllOrThrow = function(e, t) {
                    r(e, "element").exists(), r(t, "query").exists();
                    var i = e.querySelectorAll(t);
                    if (null === i || 0 === i.length) throw new Error("Unable to querySelectAll '" + t + "'");
                    return i
                }, e.getAttributesInNamespace = function(e, t) {
                    var i = [];
                    if (t += ":", e.hasAttributes())
                        for (var n = 0; n < e.attributes.length; n++) e.attributes[n].name.substr(0, t.length) === t && i.push(e.attributes[n].name);
                    return i
                }, e.getAttributeWithoutNamespace = function(e) {
                    var t = /.*:(.*)/,
                        i = e.match(t);
                    return null === i || i.length < 2 ? e : i[1]
                }, e)
            }();
        t.exports = o
    }, {
        281: 281
    }],
    98: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.getPlayerSize = function(e, t, i) {
                return (void 0 === i && (i = 1), {
                    width: e * i,
                    height: t * i
                })
            }, e)
        }();
        t.exports = n
    }, {}],
    99: [function(e, t, i) {
        "use strict";
        var n = e(284),
            r = e(294),
            o = n.get("EnumUtils"),
            s = function() {
                function e() {}
                return (e.getEnumValue = function(e, t) {
                    if (null === e || void 0 === e) return null;
                    if (!r.isString(e)) return (o.warn("Value must be a string but was " + e), null);
                    if (r.isEmpty(e)) return (o.warn("Value cannot be an empty string"), null);
                    var i = [];
                    for (var n in t) r.isNumber(t[n]) && i.push(n);
                    var s = r.find(i, function(t) {
                        return t.toLowerCase() === e.toLowerCase()
                    });
                    return null === s || void 0 === s ? (o.warn("Value must be one of [" + i.join(",") + "], but was " + e), null) : t[s]
                }, e)
            }();
        t.exports = s
    }, {
        284: 284,
        294: 294
    }],
    100: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = e(284),
            o = function() {
                function e(e) {
                    this.ephemeralIds = [], this.logger = r.get(e)
                }
                return (e.prototype.setLevel = function(e) {
                    this.logger.setLevel(e)
                }, e.prototype.enabledFor = function(e) {
                    return this.logger.enabledFor(e)
                }, e.prototype.log = function() {
                    for (var e = [], t = 0; t < arguments.length; t++) e[t] = arguments[t];
                    (i = this.logger).log.apply(i, [this.getId()].concat(e));
                    var i
                }, e.prototype.debug = function() {
                    for (var e = [], t = 0; t < arguments.length; t++) e[t] = arguments[t];
                    (i = this.logger).debug.apply(i, [this.getId()].concat(e));
                    var i
                }, e.prototype.info = function() {
                    for (var e = [], t = 0; t < arguments.length; t++) e[t] = arguments[t];
                    (i = this.logger).info.apply(i, [this.getId()].concat(e));
                    var i
                }, e.prototype.warn = function() {
                    for (var e = [], t = 0; t < arguments.length; t++) e[t] = arguments[t];
                    (i = this.logger).warn.apply(i, [this.getId()].concat(e));
                    var i
                }, e.prototype.error = function() {
                    for (var e = [], t = 0; t < arguments.length; t++) e[t] = arguments[t];
                    (i = this.logger).error.apply(i, [this.getId()].concat(e));
                    var i
                }, e.prototype.setId = function() {
                    for (var e = [], t = 0; t < arguments.length; t++) e[t] = arguments[t];
                    this.ephemeralIds = e
                }, e.prototype.getId = function() {
                    return n.map(this.ephemeralIds, function(e) {
                        return "[" + e + "]"
                    }).join(" ")
                }, e)
            }();
        t.exports = o
    }, {
        284: 284,
        294: 294
    }],
    101: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = e(281),
            o = r.of("HTMLElementUtils"),
            s = function() {
                function e() {}
                return (e.addClass = function(e, t) {
                    if ("undefined" != typeof e.className) {
                        var i = e.className.split(" "),
                            r = n.find(i, function(e) {
                                return e === t
                            });
                        "undefined" == typeof r && (e.className += " " + t)
                    }
                }, e.removeClass = function(e, t) {
                    if ("undefined" != typeof e.className) {
                        var i = e.className.split(" "),
                            r = n.find(i, function(e) {
                                return e === t
                            });
                        "undefined" != typeof r && (i = n.filter(i, function(e) {
                            return e !== t
                        }), e.className = i.join(" "))
                    }
                }, e.getElementsByClassName = function(e, t) {
                    var i;
                    return i = n.isUndefined(e.getElementsByClassName) ? e.querySelectorAll("." + t) : e.getElementsByClassName(t)
                }, e.clickedInElement = function(e, t) {
                    o(e, "element").exists(), o(t, "event").exists();
                    var i = e.getBoundingClientRect();
                    return i.left < t.clientX && i.left + e.offsetWidth > t.clientX && i.top < t.clientY && i.top + e.offsetHeight > t.clientY
                }, e.isScrollable = function(t) {
                    return t.scrollHeight > t.offsetHeight + e.SCROLLABLE_MARGIN_OF_ERROR_PX
                }, e.SCROLLABLE_MARGIN_OF_ERROR_PX = 1, e)
            }();
        t.exports = s
    }, {
        281: 281,
        294: 294
    }],
    102: [function(e, t, i) {
        "use strict";
        var n = "(\\d|[1-9]\\d|1\\d\\d|2([0-4]\\d|5[0-5]))",
            r = new RegExp("^" + n + "\\." + n + "\\." + n + "\\." + n + "$"),
            o = function() {
                function e() {}
                return (e.isValidIPv4Address = function(e) {
                    return r.test(e)
                }, e)
            }();
        t.exports = o
    }, {}],
    103: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e) {
                this.EXTENSION_REGEX = /(?:\.([^.]+))?$/, this.timeoutMillis = 12e4, e > 0 && (this.timeoutMillis = e)
            }
            return (e.prototype.acquire = function(e, t) {
                var i = this.getFileExtension(e);
                if ("js" === i) this.acquireJavaScript(e, t);
                else {
                    if ("css" !== i) throw new Error("Unable to acquire non-JavaScript or CSS resource: " + e);
                    this.acquireCSS(e, t)
                }
            }, e.prototype.getFileExtension = function(e) {
                return this.EXTENSION_REGEX.exec(e)[1]
            }, e.prototype.acquireJavaScript = function(e, t) {
                var i = this,
                    n = document.createElement("script"),
                    r = null,
                    o = function(e) {
                        clearTimeout(r), n && (n.onload = n.onerror = null), "function" == typeof t && t(e)
                    };
                r = setTimeout(function() {
                    o(new Error("Timed out acquiring resource after " + i.timeoutMillis + "ms: " + e))
                }, this.timeoutMillis), n.src = e, n.onload = function() {
                    o()
                }, n.onerror = function(t) {
                    o(new Error("Error acquiring resource (probably doesn't exist): " + e))
                }, document.getElementsByTagName("head")[0].appendChild(n)
            }, e.prototype.acquireCSS = function(e, t) {
                var i = document.createElement("link");
                i.setAttribute("rel", "stylesheet"), i.setAttribute("type", "text/css"), i.setAttribute("href", e), document.getElementsByTagName("head")[0].appendChild(i), "function" == typeof t && t()
            }, e)
        }();
        t.exports = n
    }, {}],
    104: [function(e, t, i) {
        "use strict";
        var n = e(284),
            r = e(294),
            o = n.get("CookieStorage"),
            s = function() {
                function e(e) {
                    void 0 === e && (e = document), this.document = e
                }
                return (e.prototype.get = function(e) {
                    try {
                        if (r.isUndefined(this.document.cookie) || r.isNull(this.document.cookie)) return (o.warn("Cookie is unavailable."), null);
                        var t = r.map(this.document.cookie.split(";"), function(e) {
                                return e.split("=")
                            }),
                            i = r.find(t, function(t) {
                                return t[0].trim() === e
                            });
                        return r.isUndefined(i) || r.isNull(i) ? (o.warn("Cannot find key " + e + " in storage"), null) : (i.shift(), decodeURIComponent(i.join("=")))
                    } catch (t) {
                        return (o.warn("Failed to retrieve key " + e + " from storage", t), null)
                    }
                }, e)
            }();
        t.exports = s
    }, {
        284: 284,
        294: 294
    }],
    105: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(107),
            o = "atvwebplayersdk_",
            s = function(e) {
                function t(t) {
                    return (void 0 === t && (t = window), e.call(this, "undefined" == typeof t.localStorage ? null : t.localStorage, o) || this)
                }
                return (n(t, e), t)
            }(r);
        t.exports = s
    }, {
        107: 107
    }],
    106: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(107),
            o = function(e) {
                function t(t) {
                    void 0 === t && (t = window);
                    var i = this,
                        n = t.sessionStorage;
                    return ("undefined" == typeof n && (n = null), i = e.call(this, n) || this)
                }
                return (n(t, e), t)
            }(r);
        t.exports = o
    }, {
        107: 107
    }],
    107: [function(e, t, i) {
        "use strict";
        var n = e(284),
            r = n.get("WebStorage"),
            o = function() {
                function e(e, t) {
                    void 0 === e && (e = null), void 0 === t && (t = null), this.storage = e, this.namespace = t
                }
                return (e.prototype.set = function(e, t) {
                    if (null === this.storage) return void r.warn("Storage is unavailable.");
                    var i = this.getNamespacedKey(e);
                    try {
                        this.storage.setItem(i, t)
                    } catch (e) {
                        r.warn("Failed to save key " + i + " to storage", e)
                    }
                }, e.prototype.get = function(e) {
                    if (null === this.storage) return (r.warn("Storage is unavailable."), null);
                    var t = this.getNamespacedKey(e);
                    try {
                        var i = this.storage.getItem(t);
                        return null != i ? i : (null != this.namespace && (i = this.storage.getItem(e), null != i && this.set(e, i)), i)
                    } catch (e) {
                        return (r.warn("Failed to retrieve key " + t + " from storage", e), null)
                    }
                }, e.prototype.remove = function(e) {
                    if (null === this.storage) return void r.warn("Storage is unavailable.");
                    var t = this.getNamespacedKey(e);
                    try {
                        this.storage.removeItem(t)
                    } catch (e) {
                        r.warn("Failed to remove key " + t + " from storage", e)
                    }
                }, e.prototype.removeAll = function() {
                    if (null === this.storage) return void r.warn("Storage is unavailable.");
                    try {
                        this.storage.clear()
                    } catch (e) {
                        r.warn("Failed to clear storage", e)
                    }
                }, e.prototype.getNamespacedKey = function(e) {
                    return null != this.namespace ? "" + this.namespace + e : e
                }, e)
            }();
        t.exports = o
    }, {
        284: 284
    }],
    108: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = n.of("ThrottledEventListener"),
            o = function() {
                function e() {
                    var e = this;
                    this.callback = null, this.listenerType = null, this.eventTarget = null, this.pollingInterval = null, this.executeThrottledEvent = function() {
                        e.eventTarget.removeEventListener(e.listenerType, e.executeThrottledEvent), setTimeout(function() {
                            e.callback(), e.eventTarget.addEventListener(e.listenerType, e.executeThrottledEvent)
                        }, e.pollingInterval)
                    }
                }
                return (e.prototype.attachThrottledListener = function(e, t, i, n) {
                    r(e, "eventTarget").exists(), r(t, "listenerType").exists(), this.eventTarget = e, this.listenerType = t, this.pollingInterval = i, this.callback = n, e.addEventListener(t, this.executeThrottledEvent)
                }, e.prototype.removeThrottledListener = function() {
                    null !== this.eventTarget && (this.eventTarget.removeEventListener(this.listenerType, this.executeThrottledEvent), this.eventTarget = null, this.listenerType = null, this.pollingInterval = null, this.callback = null)
                }, e)
            }();
        t.exports = o
    }, {
        281: 281
    }],
    109: [function(e, t, i) {
        "use strict";
        var n = !1;
        try {
            var r = new URL("b", "http://a");
            r.pathname = "c%20d", n = "http://a/c%20d" === r.href
        } catch (e) {}
        var o;
        if (n) o = URL;
        else {
            var s = Object.create(null);
            s.ftp = 21, s.file = 0, s.gopher = 70, s.http = 80, s.https = 443, s.ws = 80, s.wss = 443;
            var a = Object.create(null);
            a["%2e"] = ".", a[".%2e"] = "..", a["%2e."] = "..", a["%2e%2e"] = "..";
            var l = function(e) {
                    return void 0 !== s[e]
                },
                c = function() {
                    m.call(this), this._isInvalid = !0
                },
                u = function(e) {
                    return ("" == e && c.call(this), e.toLowerCase())
                },
                p = function(e) {
                    var t = e.charCodeAt(0);
                    return t > 32 && t < 127 && [34, 35, 60, 62, 63, 96].indexOf(t) == -1 ? e : encodeURIComponent(e)
                },
                h = function(e) {
                    var t = e.charCodeAt(0);
                    return t > 32 && t < 127 && [34, 35, 60, 62, 96].indexOf(t) == -1 ? e : encodeURIComponent(e)
                },
                f = void 0,
                d = /[a-zA-Z]/,
                y = /[a-zA-Z0-9\+\-\.]/,
                g = function(e, t, i) {
                    function n(e) {
                        E.push(e)
                    }
                    var r = t || "scheme start",
                        o = 0,
                        g = "",
                        m = !1,
                        v = !1,
                        E = [];
                    e: for (;
                        (e[o - 1] != f || 0 == o) && !this._isInvalid;) {
                        var _ = e[o];
                        switch (r) {
                            case "scheme start":
                                if (!_ || !d.test(_)) {
                                    if (t) {
                                        n("Invalid scheme.");
                                        break e
                                    }
                                    g = "", r = "no scheme";
                                    continue
                                }
                                g += _.toLowerCase(), r = "scheme";
                                break;
                            case "scheme":
                                if (_ && y.test(_)) g += _.toLowerCase();
                                else {
                                    if (":" != _) {
                                        if (t) {
                                            if (f == _) break e;
                                            n("Code point not allowed in scheme: " + _);
                                            break e
                                        }
                                        g = "", o = 0, r = "no scheme";
                                        continue
                                    }
                                    if (this._scheme = g, g = "", t) break e;
                                    l(this._scheme) && (this._isRelative = !0), r = "file" == this._scheme ? "relative" : this._isRelative && i && i._scheme == this._scheme ? "relative or authority" : this._isRelative ? "authority first slash" : "scheme data"
                                }
                                break;
                            case "scheme data":
                                "?" == _ ? (this._query = "?", r = "query") : "#" == _ ? (this._fragment = "#", r = "fragment") : f != _ && "\t" != _ && "\n" != _ && "\r" != _ && (this._schemeData += p(_));
                                break;
                            case "no scheme":
                                if (i && l(i._scheme)) {
                                    r = "relative";
                                    continue
                                }
                                n("Missing scheme."), c.call(this);
                                break;
                            case "relative or authority":
                                if ("/" != _ || "/" != e[o + 1]) {
                                    n("Expected /, got: " + _), r = "relative";
                                    continue
                                }
                                r = "authority ignore slashes";
                                break;
                            case "relative":
                                if (this._isRelative = !0, "file" != this._scheme && (this._scheme = i._scheme), f == _) {
                                    this._host = i._host, this._port = i._port, this._path = i._path.slice(), this._query = i._query, this._username = i._username, this._password = i._password;
                                    break e
                                }
                                if ("/" == _ || "\\" == _) "\\" == _ && n("\\ is an invalid code point."), r = "relative slash";
                                else if ("?" == _) this._host = i._host, this._port = i._port, this._path = i._path.slice(), this._query = "?", this._username = i._username, this._password = i._password, r = "query";
                                else {
                                    if ("#" != _) {
                                        var S = e[o + 1],
                                            T = e[o + 2];
                                        ("file" != this._scheme || !d.test(_) || ":" != S && "|" != S || f != T && "/" != T && "\\" != T && "?" != T && "#" != T) && (this._host = i._host, this._port = i._port, this._username = i._username, this._password = i._password, this._path = i._path.slice(), this._path.pop()), r = "relative path";
                                        continue
                                    }
                                    this._host = i._host, this._port = i._port, this._path = i._path.slice(), this._query = i._query, this._fragment = "#", this._username = i._username, this._password = i._password, r = "fragment"
                                }
                                break;
                            case "relative slash":
                                if ("/" != _ && "\\" != _) {
                                    "file" != this._scheme && (this._host = i._host, this._port = i._port, this._username = i._username, this._password = i._password), r = "relative path";
                                    continue
                                }
                                "\\" == _ && n("\\ is an invalid code point."), r = "file" == this._scheme ? "file host" : "authority ignore slashes";
                                break;
                            case "authority first slash":
                                if ("/" != _) {
                                    n("Expected '/', got: " + _), r = "authority ignore slashes";
                                    continue
                                }
                                r = "authority second slash";
                                break;
                            case "authority second slash":
                                if (r = "authority ignore slashes", "/" != _) {
                                    n("Expected '/', got: " + _);
                                    continue
                                }
                                break;
                            case "authority ignore slashes":
                                if ("/" != _ && "\\" != _) {
                                    r = "authority";
                                    continue
                                }
                                n("Expected authority, got: " + _);
                                break;
                            case "authority":
                                if ("@" == _) {
                                    m && (n("@ already seen."), g += "%40"), m = !0;
                                    for (var b = 0; b < g.length; b++) {
                                        var I = g[b];
                                        if ("\t" != I && "\n" != I && "\r" != I)
                                            if (":" != I || null !== this._password) {
                                                var w = p(I);
                                                null !== this._password ? this._password += w : this._username += w
                                            } else this._password = "";
                                        else n("Invalid whitespace in authority.")
                                    }
                                    g = ""
                                } else {
                                    if (f == _ || "/" == _ || "\\" == _ || "?" == _ || "#" == _) {
                                        o -= g.length, g = "", r = "host";
                                        continue
                                    }
                                    g += _
                                }
                                break;
                            case "file host":
                                if (f == _ || "/" == _ || "\\" == _ || "?" == _ || "#" == _) {
                                    2 != g.length || !d.test(g[0]) || ":" != g[1] && "|" != g[1] ? 0 == g.length ? r = "relative path start" : (this._host = u.call(this, g), g = "", r = "relative path start") : r = "relative path";
                                    continue
                                }
                                "\t" == _ || "\n" == _ || "\r" == _ ? n("Invalid whitespace in file host.") : g += _;
                                break;
                            case "host":
                            case "hostname":
                                if (":" != _ || v) {
                                    if (f == _ || "/" == _ || "\\" == _ || "?" == _ || "#" == _) {
                                        if (this._host = u.call(this, g), g = "", r = "relative path start", t) break e;
                                        continue
                                    }
                                    "\t" != _ && "\n" != _ && "\r" != _ ? ("[" == _ ? v = !0 : "]" == _ && (v = !1), g += _) : n("Invalid code point in host/hostname: " + _)
                                } else if (this._host = u.call(this, g), g = "", r = "port", "hostname" == t) break e;
                                break;
                            case "port":
                                if (/[0-9]/.test(_)) g += _;
                                else {
                                    if (f == _ || "/" == _ || "\\" == _ || "?" == _ || "#" == _ || t) {
                                        if ("" != g) {
                                            var R = parseInt(g, 10);
                                            R != s[this._scheme] && (this._port = R + ""), g = ""
                                        }
                                        if (t) break e;
                                        r = "relative path start";
                                        continue
                                    }
                                    "\t" == _ || "\n" == _ || "\r" == _ ? n("Invalid code point in port: " + _) : c.call(this)
                                }
                                break;
                            case "relative path start":
                                if ("\\" == _ && n("'\\' not allowed in path."), r = "relative path", "/" != _ && "\\" != _) continue;
                                break;
                            case "relative path":
                                if (f != _ && "/" != _ && "\\" != _ && (t || "?" != _ && "#" != _)) "\t" != _ && "\n" != _ && "\r" != _ && (g += p(_));
                                else {
                                    "\\" == _ && n("\\ not allowed in relative path.");
                                    var C;
                                    (C = a[g.toLowerCase()]) && (g = C), ".." == g ? (this._path.pop(), "/" != _ && "\\" != _ && this._path.push("")) : "." == g && "/" != _ && "\\" != _ ? this._path.push("") : "." != g && ("file" == this._scheme && 0 == this._path.length && 2 == g.length && d.test(g[0]) && "|" == g[1] && (g = g[0] + ":"), this._path.push(g)), g = "", "?" == _ ? (this._query = "?", r = "query") : "#" == _ && (this._fragment = "#", r = "fragment")
                                }
                                break;
                            case "query":
                                t || "#" != _ ? f != _ && "\t" != _ && "\n" != _ && "\r" != _ && (this._query += h(_)) : (this._fragment = "#", r = "fragment");
                                break;
                            case "fragment":
                                f != _ && "\t" != _ && "\n" != _ && "\r" != _ && (this._fragment += _)
                        }
                        o++
                    }
                },
                m = function() {
                    this._scheme = "", this._schemeData = "", this._username = "", this._password = null, this._host = "", this._port = "", this._path = [], this._query = "", this._fragment = "", this._isInvalid = !1, this._isRelative = !1
                };
            if (o = function(e, t) {
                    void 0 === t || t instanceof o || (t = new o(String(t))), this._url = e, m.call(this);
                    var i = e.replace(/^[ \t\r\n\f]+|[ \t\r\n\f]+$/g, "");
                    g.call(this, i, null, t)
                }, o.prototype = {
                    toString: function() {
                        return this.href
                    },
                    get href() {
                        if (this._isInvalid) return this._url;
                        var e = "";
                        return ("" == this._username && null == this._password || (e = this._username + (null != this._password ? ":" + this._password : "") + "@"), this.protocol + (this._isRelative ? "//" + e + this.host : "") + this.pathname + this._query + this._fragment)
                    },
                    set href(e) {
                        m.call(this), g.call(this, e)
                    },
                    get protocol() {
                        return this._scheme + ":"
                    },
                    set protocol(e) {
                        this._isInvalid || g.call(this, e + ":", "scheme start")
                    },
                    get host() {
                        return this._isInvalid ? "" : this._port ? this._host + ":" + this._port : this._host
                    },
                    set host(e) {
                        !this._isInvalid && this._isRelative && g.call(this, e, "host")
                    },
                    get hostname() {
                        return this._host
                    },
                    set hostname(e) {
                        !this._isInvalid && this._isRelative && g.call(this, e, "hostname")
                    },
                    get port() {
                        return this._port
                    },
                    set port(e) {
                        !this._isInvalid && this._isRelative && g.call(this, e, "port")
                    },
                    get pathname() {
                        return this._isInvalid ? "" : this._isRelative ? "/" + this._path.join("/") : this._schemeData
                    },
                    set pathname(e) {
                        !this._isInvalid && this._isRelative && (this._path = [], g.call(this, e, "relative path start"))
                    },
                    get search() {
                        return this._isInvalid || !this._query || "?" == this._query ? "" : this._query
                    },
                    set search(e) {
                        !this._isInvalid && this._isRelative && (this._query = "?", "?" == e[0] && (e = e.slice(1)), g.call(this, e, "query"))
                    },
                    get hash() {
                        return this._isInvalid || !this._fragment || "#" == this._fragment ? "" : this._fragment
                    },
                    set hash(e) {
                        this._isInvalid || (this._fragment = "#", "#" == e[0] && (e = e.slice(1)), g.call(this, e, "fragment"))
                    },
                    get origin() {
                        var e;
                        if (this._isInvalid || !this._scheme) return "";
                        switch (this._scheme) {
                            case "data":
                            case "file":
                            case "javascript":
                            case "mailto":
                                return "null"
                        }
                        return (e = this.host, e ? this._scheme + "://" + e : "")
                    }
                }, "undefined" != typeof URL) {
                var v = URL;
                v && (o.createObjectURL = function(e) {
                    return v.createObjectURL.apply(v, arguments)
                }, o.revokeObjectURL = function(e) {
                    v.revokeObjectURL(e)
                })
            }
        }
        t.exports = o
    }, {}],
    110: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.prototype.decode = function(e) {
                for (var t = new Uint8Array(e.buffer, e.byteOffset, e.byteLength), i = [], n = 0; n < t.length;) {
                    var r = 0,
                        o = 0,
                        s = t[n++];
                    if (240 === (248 & s)) r = 3, o = 7 & s;
                    else if (224 === (240 & s)) r = 2, o = 15 & s;
                    else if (192 === (224 & s)) r = 1, o = 31 & s;
                    else {
                        if (0 !== (128 & s)) throw new TypeError("Failed to decode value: " + s.toString(16));
                        r = 0, o = 127 & s
                    }
                    for (; r > 0 && n < t.length;) o <<= 6, o |= 63 & t[n++], r--;
                    if (r > 0) throw new TypeError("Failed to decode value");
                    if (o >= 65536) {
                        o -= 65536;
                        var a = 56320 | 1023 & o,
                            l = 55296 | o >> 10 & 1023;
                        i.push(String.fromCharCode(l, a))
                    } else i.push(String.fromCharCode(o))
                }
                return i.join("")
            }, e)
        }();
        t.exports = n
    }, {}],
    111: [function(e, t, i) {
        "use strict";
        var n, r = e(110);
        n = "undefined" != typeof TextDecoder ? function() {
            return new TextDecoder("utf-8")
        } : r, t.exports = n
    }, {
        110: 110
    }],
    112: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = e(281),
            o = r.of("Version"),
            s = "unknown",
            a = function() {
                function e(e, t) {
                    o(e, "segments").exists(), o(e.length > 0, "length > 0").is["true"](), o(t, "str").exists(), this.segments = e, this.str = t
                }
                return (e.fromNumber = function(t) {
                    return (o(t, "version").exists(), new e([t], t.toString()))
                }, e.fromString = function(t) {
                    return (o(t, "version").exists(), new e(e.split(t), t))
                }, e.createUnknownVersion = function() {
                    return e.fromString(s)
                }, e.prototype.isUnknownVersion = function() {
                    return this.str === s
                }, e.prototype.toString = function() {
                    return this.str
                }, e.prototype.equals = function(e) {
                    return 0 === this.compareTo(e)
                }, e.prototype.compareTo = function(e) {
                    if (this === e) return 0;
                    if (n.isUndefined(e) || n.isNull(e)) return 1;
                    if (this.str === s && e.str === s) return 0;
                    for (var t = Math.min(this.segments.length, e.segments.length), i = 0; i < t; i++) {
                        if (this.segments[i] < e.segments[i]) return -1;
                        if (this.segments[i] > e.segments[i]) return 1
                    }
                    return this.isGreaterThanZero(e.segments.slice(this.segments.length)) ? -1 : this.isGreaterThanZero(this.segments.slice(e.segments.length)) ? 1 : 0
                }, e.prototype.isGreaterThanZero = function(e) {
                    for (var t = 0; t < e.length; t++)
                        if (n.isString(e[t]) && e[t].length > 0 || n.isNumber(e[t]) && e[t] > 0) return !0
                }, e.split = function(e) {
                    return n.map(e.toLowerCase().match(/\d+|[a-z]+/g), function(e) {
                        return /^\d+$/.test(e) ? parseInt(e) : e
                    })
                }, e)
            }();
        t.exports = a
    }, {
        281: 281,
        294: 294
    }],
    113: [function(e, t, i) {
        "use strict";
        var n = e(96),
            r = e(104),
            o = e(98),
            s = e(97),
            a = e(99),
            l = e(100),
            c = e(101),
            u = e(102),
            p = e(105),
            h = e(103),
            f = e(106),
            d = e(108),
            y = e(109),
            g = e(111),
            m = e(112),
            v = {
                AmazonUrlCreator: n,
                CookieStorage: r,
                Display: o,
                DOMUtils: s,
                EnumUtils: a,
                EphemeralJSLogger: l,
                HTMLElementUtils: c,
                IPAddress: u,
                LocalStorage: p,
                ResourceAcquirer: h,
                SessionStorage: f,
                ThrottledEventListener: d,
                URL: y,
                Utf8TextDecoder: g,
                Version: m
            };
        t.exports = v
    }, {
        100: 100,
        101: 101,
        102: 102,
        103: 103,
        104: 104,
        105: 105,
        106: 106,
        108: 108,
        109: 109,
        111: 111,
        112: 112,
        96: 96,
        97: 97,
        98: 98,
        99: 99
    }],
    114: [function(e, t, i) {
        "use strict";
        var n = e(163),
            r = n.EventEmitter,
            o = "change",
            s = function() {
                function e(e) {
                    void 0 === e && (e = new r), this.value = null, this.eventEmitter = e
                }
                return (e.prototype.getValue = function() {
                    return this.value
                }, e.prototype.setValue = function(e) {
                    return (this.value = e, this)
                }, e.prototype.commit = function() {
                    this.eventEmitter.emitAsync(o)
                }, e.prototype.onChange = function(e) {
                    this.eventEmitter.on(o, e)
                }, e.prototype.offChange = function(e) {
                    this.eventEmitter.off(o, e)
                }, e.prototype.onceChange = function(e) {
                    this.eventEmitter.once(o, e)
                }, e)
            }();
        t.exports = s
    }, {
        163: 163
    }],
    115: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e, t, i) {
                var n = this;
                this.defaultStore = e, this.backingStore = t, this.mergeFunction = i, this.defaultStore.onChange(function() {
                    n.commit()
                })
            }
            return (e.prototype.getValue = function() {
                return this.mergeFunction(this.defaultStore.getValue(), this.backingStore.getValue())
            }, e.prototype.setValue = function(e) {
                return (this.backingStore.setValue(e), this)
            }, e.prototype.commit = function() {
                this.backingStore.commit()
            }, e.prototype.onChange = function(e) {
                this.backingStore.onChange(e)
            }, e.prototype.offChange = function(e) {
                this.backingStore.offChange(e)
            }, e.prototype.onceChange = function(e) {
                this.backingStore.onceChange(e)
            }, e)
        }();
        t.exports = n
    }, {}],
    116: [function(e, t, i) {
        "use strict";
        var n = e(114),
            r = e(115),
            o = {
                BasicStore: n,
                MergingStore: r
            };
        t.exports = o
    }, {
        114: 114,
        115: 115
    }],
    117: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e) {
                this.applicationName = e.an, this.applicationVersion = e.av, this.operatingSystem = e.os, this.operatingSystemVersion = e.ov, this.productLine = e.di.pr, this.model = e.di.md, this.modelVersion = e.di.v, this.manufacturer = e.di.mf, this.deviceSerialNumber = e.di.dsn, this.deviceTypeId = e.di.dti, this.serviceProvider = e.di.ca, this.connectionType = e.di.ct, this.displayHeight = e.dm.h, this.displayWidth = e.dm.w
            }
            return (e.prototype.getSimulatedUserAgent = function() {
                return this.applicationName + "/" + this.applicationVersion + " (" + this.operatingSystem + "/" + this.operatingSystemVersion + "/" + this.model + ")"
            }, e)
        }();
        t.exports = n
    }, {}],
    118: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e, t, i, n) {
                this.browser = e, this.operatingSystem = t, this.device = i, this.displayInformation = n
            }
            return (e.prototype.equals = function(e) {
                return this.browser.equals(e.browser) && this.operatingSystem.equals(e.operatingSystem) && this.device.equals(e.device) && this.displayInformation.equals(e.displayInformation)
            }, e)
        }();
        t.exports = n
    }, {}],
    119: [function(e, t, i) {
        "use strict";
        var n = e(124),
            r = e(125),
            o = e(93),
            s = e(113),
            a = s.CookieStorage,
            l = function() {
                function e() {}
                return (e.getDeviceInformation = function(e, t) {
                    void 0 === e && (e = window.navigator), void 0 === t && (t = new a);
                    var i = t.get("amzn-app-ctxt");
                    return o.isNullOrUndefined(i) ? r.fromBrowser(e) : n.fromCookie(i)
                }, e)
            }();
        t.exports = l
    }, {
        113: 113,
        124: 124,
        125: 125,
        93: 93
    }],
    120: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = e(122),
            o = function() {
                function e() {}
                return (e.fromMashCookie = function(t) {
                    var i = new e;
                    return (i.applicationName = t.applicationName, i.applicationVersion = t.applicationVersion, i.productLine = t.productLine, i.model = r.fromMashCookie(t), i.manufacturer = t.manufacturer, i.serialNumber = t.deviceSerialNumber, i.deviceTypeId = t.deviceTypeId, i.serviceProvider = t.serviceProvider, i.connectionType = t.connectionType, i)
                }, e.fromUserAgent = function(t) {
                    var i = new e;
                    return (i.model = r.fromUserAgent(t), i)
                }, e.prototype.equals = function(e) {
                    return n.isEqual(this, e)
                }, e.UNKNOWN = new e, e)
            }();
        t.exports = o
    }, {
        122: 122,
        294: 294
    }],
    121: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = function() {
                function e(e, t) {
                    this.height = e, this.width = t
                }
                return (e.fromMashCookie = function(t) {
                    return new e(t.displayHeight, t.displayWidth)
                }, e.fromBrowser = function(t) {
                    return (void 0 === t && (t = window.screen), new e(t.height, t.width))
                }, e.prototype.equals = function(e) {
                    return n.isEqual(this, e)
                }, e.UNKNOWN = new e(null, null), e)
            }();
        t.exports = r
    }, {
        294: 294
    }],
    122: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(281),
            o = e(113),
            s = o.Version,
            a = e(123),
            l = r.of("Model"),
            c = function() {
                function e(e, t, i) {
                    this.type = l(e, "type").exists(), this.version = n.isNullOrUndefined(t) ? null : t, this.name = n.isNullOrUndefined(i) ? a[e] : i
                }
                return (e.fromMashCookie = function(t) {
                    var i = n.isNullOrUndefined(t.modelVersion) ? null : s.fromString(t.modelVersion);
                    return n.isNullOrUndefined(a[t.model]) ? new e(a.Unknown, i, t.model) : new e(a[t.model], i)
                }, e.fromUserAgent = function(t) {
                    for (var i = 0; i < e.REGEXES.length; i++) {
                        var n = e.REGEXES[i].regex.exec(t);
                        if (n) {
                            var r = n[1] ? s.fromString(n[1]) : null;
                            return new e(e.REGEXES[i].type, r)
                        }
                    }
                    return new e(a.Unknown)
                }, e.prototype.getType = function() {
                    return this.type
                }, e.prototype.getVersion = function() {
                    return this.version
                }, e.prototype.getModelName = function() {
                    return this.name
                }, e.prototype.toString = function() {
                    var e = this.name;
                    return (null !== this.version && (e += " " + this.version), e)
                }, e.prototype.equals = function(e) {
                    return (this.type === e.type || this.name === e.name) && (null === this.version ? null === e.version : this.version.equals(e.version))
                }, e.UNKNOWN = new e(a.Unknown), e.REGEXES = [{
                    regex: /iPad/,
                    type: a.iPad
                }, {
                    regex: /iPhone/,
                    type: a.iPhone
                }, {
                    regex: /iPod/,
                    type: a.iPod
                }], e)
            }();
        t.exports = c
    }, {
        113: 113,
        123: 123,
        281: 281,
        93: 93
    }],
    123: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Unknown = 0] = "Unknown", e[e.iPad = 1] = "iPad", e[e.iPhone = 2] = "iPhone", e[e.iPod = 3] = "iPod"
        }(n || (n = {})), t.exports = n
    }, {}],
    124: [function(e, t, i) {
        "use strict";
        var n = e(272),
            r = n.Browser,
            o = n.OperatingSystem,
            s = e(120),
            a = e(118),
            l = e(121),
            c = e(117),
            u = e(284),
            p = u.get("MashDeviceInformationFactory"),
            h = function() {
                function e() {}
                return (e.fromCookie = function(e, t, i, n, u) {
                    void 0 === t && (t = r), void 0 === i && (i = o), void 0 === n && (n = s), void 0 === u && (u = l);
                    try {
                        var h = new c(JSON.parse(e.slice(e.indexOf("{")))),
                            f = t.fromUserAgent(h.getSimulatedUserAgent()),
                            d = i.fromUserAgent(h.getSimulatedUserAgent()),
                            y = n.fromMashCookie(h),
                            g = u.fromMashCookie(h);
                        return new a(f, d, y, g)
                    } catch (t) {
                        return (p.warn("Error parsing the MASH cookie. Returning unknown browser and operating system. Cookie format: " + e, t), new a(r.UNKNOWN, o.UNKNOWN, s.UNKNOWN, l.UNKNOWN))
                    }
                }, e)
            }();
        t.exports = h
    }, {
        117: 117,
        118: 118,
        120: 120,
        121: 121,
        272: 272,
        284: 284
    }],
    125: [function(e, t, i) {
        "use strict";
        var n = e(272),
            r = n.Browser,
            o = n.OperatingSystem,
            s = e(120),
            a = e(118),
            l = e(121),
            c = function() {
                function e() {}
                return (e.fromBrowser = function(e, t, i, n, c, u) {
                    void 0 === e && (e = window.navigator), void 0 === t && (t = window.screen), void 0 === i && (i = r), void 0 === n && (n = o), void 0 === c && (c = s), void 0 === u && (u = l);
                    var p = i.fromUserAgent(e.userAgent),
                        h = n.fromUserAgent(e.userAgent),
                        f = c.fromUserAgent(e.userAgent),
                        d = u.fromBrowser(t);
                    return new a(p, h, f, d)
                }, e)
            }();
        t.exports = c
    }, {
        118: 118,
        120: 120,
        121: 121,
        272: 272
    }],
    126: [function(e, t, i) {
        "use strict";
        var n = e(119),
            r = e(93),
            o = function() {
                function e() {}
                return (e.getInstance = function() {
                    return (r.isNullOrUndefined(this.deviceInformation) && (this.deviceInformation = n.getDeviceInformation()), this.deviceInformation)
                }, e)
            }();
        t.exports = o
    }, {
        119: 119,
        93: 93
    }],
    127: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e) {
                this.timestampMs = Date.now(), this.payload = e
            }
            return e
        }();
        t.exports = n
    }, {}],
    128: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i]);
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.BufferLengthChangedEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    129: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.BufferingStartedEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    130: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.CDNSwitchEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    131: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.ClientVersionReportedEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    132: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.DecodeMetricsEmittedEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    133: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.DiagnosticsSessionInformationEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    134: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t, i) {
                    var n = e.call(this, i) || this;
                    return (n.type = o.FragmentDownloadFailedEvent, n.priority = s.RealTime, n.version = "1.0", n.fragmentType = t, n.isSuccessful = !1, n)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    135: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t, i) {
                    var n = e.call(this, i) || this;
                    return (n.type = o.FragmentDownloadedEvent, n.priority = s.RealTime, n.version = "1.0", n.fragmentType = t, n.bandwidthBps = i.sizeBytes / i.downloadTransferTimeMs * 1e3 * 8, n.isSuccessful = !0, n)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    136: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = e(149),
            l = function(e) {
                function t(t, i) {
                    void 0 === i && (i = a.INFO);
                    var n = e.call(this, t) || this;
                    return (n.type = o.LogEvent, n.version = "1.0", n.priority = s.RealTime, n.logLevel = i, n)
                }
                return (n(t, e), t)
            }(r);
        t.exports = l
    }, {
        127: 127,
        146: 146,
        147: 147,
        149: 149
    }],
    137: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.MaxBufferSelectedEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    138: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.OriginSwitchEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    139: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.PositionChangedEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    140: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.PrepareAsinEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    141: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t, i, n, r) {
                    var a = e.call(this, r) || this;
                    return (a.type = o.QualityLevelChangedEvent, a.priority = s.RealTime, a.version = "1.0", a.fragmentType = t, a.bandwidths = i, a.streamId = n, a)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    142: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.StateChangedEvent, i.priority = s.RealTime, i.version = "1.0", i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    143: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(127),
            o = e(147),
            s = e(146),
            a = function(e) {
                function t(t) {
                    var i = e.call(this, t) || this;
                    return (i.type = o.VolumeChangedEvent, i.version = "1.0", i.priority = s.RealTime, i)
                }
                return (n(t, e), t)
            }(r);
        t.exports = a
    }, {
        127: 127,
        146: 146,
        147: 147
    }],
    144: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Expected = 0] = "Expected", e[e.Unexpected = 1] = "Unexpected"
        }(n || (n = {})), t.exports = n
    }, {}],
    145: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return e
        }();
        t.exports = n
    }, {}],
    146: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.RealTime = 0] = "RealTime", e[e.Storage = 1] = "Storage"
        }(n || (n = {})), t.exports = n
    }, {}],
    147: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.BufferLengthChangedEvent = 0] = "BufferLengthChangedEvent", e[e.BufferingStartedEvent = 1] = "BufferingStartedEvent", e[e.ClientVersionReportedEvent = 2] = "ClientVersionReportedEvent", e[e.CDNSwitchEvent = 3] = "CDNSwitchEvent", e[e.DecodeMetricsEmittedEvent = 4] = "DecodeMetricsEmittedEvent", e[e.DiagnosticsSessionInformationEvent = 5] = "DiagnosticsSessionInformationEvent", e[e.FragmentDownloadedEvent = 6] = "FragmentDownloadedEvent", e[e.FragmentDownloadFailedEvent = 7] = "FragmentDownloadFailedEvent", e[e.LogEvent = 8] = "LogEvent", e[e.MaxBufferSelectedEvent = 9] = "MaxBufferSelectedEvent", e[e.OriginSwitchEvent = 10] = "OriginSwitchEvent", e[e.PositionChangedEvent = 11] = "PositionChangedEvent", e[e.PrepareAsinEvent = 12] = "PrepareAsinEvent", e[e.QualityLevelChangedEvent = 13] = "QualityLevelChangedEvent", e[e.StateChangedEvent = 14] = "StateChangedEvent", e[e.VolumeChangedEvent = 15] = "VolumeChangedEvent"
        }(n || (n = {})), t.exports = n
    }, {}],
    148: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Audio = 0] = "Audio", e[e.Video = 1] = "Video"
        }(n || (n = {})), t.exports = n
    }, {}],
    149: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.DEBUG = 0] = "DEBUG", e[e.INFO = 1] = "INFO", e[e.WARN = 2] = "WARN", e[e.ERROR = 3] = "ERROR"
        }(n || (n = {})), t.exports = n
    }, {}],
    150: [function(e, t, i) {
        "use strict";
        var n = e(136),
            r = e(149),
            o = e(146),
            s = e(147),
            a = e(144),
            l = e(148),
            c = e(145),
            u = e(129),
            p = e(128),
            h = e(130),
            f = e(131),
            d = e(132),
            y = e(133),
            g = e(135),
            m = e(134),
            v = e(137),
            E = e(138),
            _ = e(139),
            S = e(140),
            T = e(141),
            b = e(142),
            I = e(143),
            w = {
                BufferingStartedEvent: u,
                BufferingType: a,
                BufferLengthChangedEvent: p,
                CDNSwitchEvent: h,
                ClientVersionReportedEvent: f,
                DecodeMetricsEmittedEvent: d,
                DiagnosticsSessionData: c,
                DiagnosticsSessionInformationEvent: y,
                EventPriority: o,
                EventType: s,
                FragmentDownloadedEvent: g,
                FragmentDownloadFailedEvent: m,
                FragmentType: l,
                LogEvent: n,
                LogLevels: r,
                MaxBufferSelectedEvent: v,
                OriginSwitchEvent: E,
                PositionChangedEvent: _,
                PrepareAsinEvent: S,
                QualityLevelChangedEvent: T,
                StateChangedEvent: b,
                VolumeChangedEvent: I
            };
        t.exports = w
    }, {
        128: 128,
        129: 129,
        130: 130,
        131: 131,
        132: 132,
        133: 133,
        134: 134,
        135: 135,
        136: 136,
        137: 137,
        138: 138,
        139: 139,
        140: 140,
        141: 141,
        142: 142,
        143: 143,
        144: 144,
        145: 145,
        146: 146,
        147: 147,
        148: 148,
        149: 149
    }],
    151: [function(e, t, i) {
        "use strict";
        var n = e(231),
            r = n.DiagnosticsProtocol,
            o = e(157),
            s = e(156),
            a = function() {
                function e(e, t) {
                    void 0 === e && (e = o), void 0 === t && (t = s), this.postMessageHandlerType = e, this.httpMessageHandlerType = t
                }
                return (e.prototype.getHandler = function(e) {
                    var t = e.getValue().diagnosticsSessionConfiguration;
                    switch (t.protocol) {
                        case r.PostMessage:
                            return new this.postMessageHandlerType(e);
                        case r.Http:
                            return new this.httpMessageHandlerType(e);
                        default:
                            throw new Error("The selected diagnostics protocol is not implemented")
                    }
                }, e)
            }();
        t.exports = a
    }, {
        156: 156,
        157: 157,
        231: 231
    }],
    152: [function(e, t, i) {
        "use strict";
        var n = e(288),
            r = e(93),
            o = e(150),
            s = o.DiagnosticsSessionInformationEvent,
            a = e(153),
            l = e(151),
            c = e(155),
            u = function() {
                function e(e, t, i, r) {
                    void 0 === e && (e = new l), void 0 === t && (t = []), void 0 === i && (i = n.v4()), void 0 === r && (r = new c), this.handlerFactory = e, this.telemetrySessionId = i, this.reportQueue = t, this.isHandlerConnected = !1, this.sessionMap = r
                }
                return (e.prototype.initialize = function(e) {
                    var t = this;
                    this.reportHandler = this.handlerFactory.getHandler(e), this.reportHandler.on(a.Connect, function() {
                        t.isHandlerConnected = !0, t.fireQueuedEvents()
                    }), this.reportHandler.on(a.Disconnect, function() {
                        t.isHandlerConnected = !1
                    })
                }, e.prototype.open = function() {
                    r.exists(this.reportHandler) && this.reportHandler.open()
                }, e.prototype.report = function(e) {
                    e.telemetrySessionId = this.telemetrySessionId, this.sessionMap.setMap(e), this.isHandlerConnected ? this.reportHandler.report(e) : this.queueEvent(e)
                }, e.prototype.queueEvent = function(e) {
                    this.reportQueue.length > 500 && this.reportQueue.shift(), this.reportQueue.push(e)
                }, e.prototype.fireQueuedEvents = function() {
                    var e = this.sessionMap.getSessionData(this.telemetrySessionId),
                        t = new s(e);
                    for (this.report(t); 0 !== this.reportQueue.length;) this.report(this.reportQueue.shift())
                }, e)
            }();
        t.exports = u
    }, {
        150: 150,
        151: 151,
        153: 153,
        155: 155,
        288: 288,
        93: 93
    }],
    153: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Connect = 0] = "Connect", e[e.Disconnect = 1] = "Disconnect"
        }(n || (n = {})), t.exports = n
    }, {}],
    154: [function(e, t, i) {
        "use strict";
        var n = e(163),
            r = n.EventEmitter,
            o = function() {
                function e(e) {
                    void 0 === e && (e = new r), this.eventEmitter = e, this.errorCount = 0
                }
                return (e.prototype.on = function(e, t) {
                    this.eventEmitter.on(e, t)
                }, e.prototype.emit = function(e, t) {
                    this.eventEmitter.emit(e, t)
                }, e)
            }();
        t.exports = o
    }, {
        163: 163
    }],
    155: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(150),
            o = r.DiagnosticsSessionData,
            s = r.EventType,
            a = r.FragmentType,
            l = function() {
                function e() {
                    this.sessionDataMap = {}
                }
                return (e.prototype.setMap = function(e) {
                    n.isNullOrUndefined(this.sessionDataMap[e.telemetrySessionId]) && (this.sessionDataMap[e.telemetrySessionId] = new o);
                    var t = this.sessionDataMap[e.telemetrySessionId];
                    switch (t.lastUpdatedTimestamp = Date.now(), e.type) {
                        case s.PrepareAsinEvent:
                            var i = e.payload;
                            t.asin = i;
                            break;
                        case s.CDNSwitchEvent:
                            var r = e.payload;
                            t.cdn = r;
                            break;
                        case s.OriginSwitchEvent:
                            var l = e.payload;
                            t.origin = l;
                            break;
                        case s.ClientVersionReportedEvent:
                            var c = e.payload;
                            t.clientVersion = c;
                            break;
                        case s.StateChangedEvent:
                            var u = e.payload.nextState;
                            t.state = u;
                            break;
                        case s.QualityLevelChangedEvent:
                            e.fragmentType === a.Audio ? t.audioQualityLevel = e : e.fragmentType === a.Video && (t.videoQualityLevel = e)
                    }
                }, e.prototype.getSessionData = function(e) {
                    return this.sessionDataMap[e]
                }, e)
            }();
        t.exports = l
    }, {
        150: 150,
        93: 93
    }],
    156: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(178),
            o = r.HttpRequestBuilder,
            s = e(284),
            a = e(154),
            l = e(153),
            c = s.get("HttpMessageHandler"),
            u = function(e) {
                function t(t, i, n) {
                    void 0 === n && (n = o);
                    var r = e.call(this, i) || this;
                    r.configurationStore = t;
                    var s = r.configurationStore.getValue(),
                        a = s.diagnosticsSessionConfiguration.endpointURI;
                    return (r.requestBuilder = n.post(a), r)
                }
                return (n(t, e), t.prototype.open = function() {
                    this.emit(l.Connect)
                }, t.prototype.report = function(e) {
                    try {
                        if (this.errorCount <= t.MAX_RETRY_COUNT) {
                            var i = this.requestBuilder.body(JSON.stringify(e)).build();
                            i.send()
                        } else this.emit(l.Disconnect);
                        this.errorCount = 0
                    } catch (e) {
                        this.errorCount++, c.error("Error Sending Message " + e)
                    }
                }, t.MAX_RETRY_COUNT = 500, t)
            }(a);
        t.exports = u
    }, {
        153: 153,
        154: 154,
        178: 178,
        284: 284
    }],
    157: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(126),
            o = e(272),
            s = o.BrowserType,
            a = e(93),
            l = e(284),
            c = e(154),
            u = e(153),
            p = l.get("PostMessageHandler"),
            h = function(e) {
                function t(t, i, n, o) {
                    void 0 === i && (i = window), void 0 === o && (o = r.getInstance());
                    var s = e.call(this, n) || this;
                    s.configurationStore = t, s.windowRef = i, s.deviceInformationRef = o;
                    var l = s.configurationStore.getValue();
                    return (s.diagnosticsWindow = l.diagnosticsSessionConfiguration.window, a.exists(s.diagnosticsWindow) && !s.diagnosticsWindow.closed && s.emit(u.Connect), s)
                }
                return (n(t, e), t.prototype.open = function() {
                    var e = this;
                    if (a.isNullOrUndefined(this.diagnosticsWindow) || this.diagnosticsWindow.closed) {
                        this.errorCount = 0;
                        var t = this.configurationStore.getValue(),
                            i = t.diagnosticsSessionConfiguration,
                            n = this.deviceInformationRef.browser,
                            r = a.exists(n) && (n.getType() === s.Edge || n.getType() === s.IE);
                        this.diagnosticsWindow = r ? this.windowRef.open("/") : this.windowRef.open(i.endpointURI), t.diagnosticsSessionConfiguration.window = this.diagnosticsWindow, this.configurationStore.setValue(t), this.configurationStore.commit(), a.exists(this.diagnosticsWindow) ? (r && this.diagnosticsWindow.location.assign(i.endpointURI), setTimeout(function() {
                            e.emit(u.Connect)
                        }, 1e3)) : p.error("Diagnostics Error - The diagnostics window did not open correctly.")
                    } else this.emit(u.Connect)
                }, t.prototype.report = function(e) {
                    try {
                        var i = a.isNullOrUndefined(this.diagnosticsWindow) || this.diagnosticsWindow.closed;
                        !i && this.errorCount <= t.MAX_RETRY_COUNT ? this.diagnosticsWindow.postMessage(e, "*") : (this.emit(u.Disconnect), this.diagnosticsWindow = null), this.errorCount = 0
                    } catch (e) {
                        this.errorCount++, p.error("Error Posting Message " + e)
                    }
                }, t.MAX_RETRY_COUNT = 500, t)
            }(c);
        t.exports = h
    }, {
        126: 126,
        153: 153,
        154: 154,
        272: 272,
        284: 284,
        93: 93
    }],
    158: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(152),
            o = function() {
                function e() {}
                return (e.getInstance = function() {
                    return (n.isNullOrUndefined(this.diagnosticsReporter) && (this.diagnosticsReporter = new r), this.diagnosticsReporter)
                }, e)
            }();
        t.exports = o
    }, {
        152: 152,
        93: 93
    }],
    159: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.newError = function(e, t, i) {
                var n = new Error(t);
                return (n.name = e, "undefined" != typeof i && null !== i && (n.cause = i), n)
            }, e.newAuthenticationError = function(t) {
                return e.newError(e.AUTHENTICATION_ERROR_NAME, t)
            }, e.newIllegalArgumentError = function(t) {
                return e.newError(e.ILLEGAL_ARGUMENT_ERROR_NAME, t)
            }, e.newIllegalStateError = function(t) {
                return e.newError(e.ILLEGAL_STATE_ERROR_NAME, t)
            }, e.getAllErrorNames = function(e) {
                for (var t = [e.name]; e.cause instanceof Error;) e = e.cause, t.push(e.name);
                return t
            }, e.AUTHENTICATION_ERROR_NAME = "AuthenticationError", e.ILLEGAL_ARGUMENT_ERROR_NAME = "IllegalArgumentError", e.ILLEGAL_STATE_ERROR_NAME = "IllegalStateError", e)
        }();
        t.exports = n
    }, {}],
    160: [function(e, t, i) {
        "use strict";
        var n = e(187),
            r = e(254),
            o = e(283),
            s = o.EventEmitter2,
            a = e(284),
            l = a.get("EventEmitter"),
            c = function() {
                function e(e, t, i) {
                    void 0 === t && (t = null), void 0 === i && (i = r.getInstance()), this.eventEmitter = null === t ? new s(e) : t, this.immediateExecutor = i
                }
                return (e.prototype.emit = function(t, i) {
                    try {
                        this.eventEmitter.emit(this.getEventString(t), i)
                    } catch (i) {
                        l.error("An error occurred in a listener for the '" + t + "' event: " + i.name + ": " + i.message, i), n.putMetric(e.UNHANDLED_EVENT_LISTENER_ERROR_METRIC)
                    }
                }, e.prototype.emitAsync = function(e, t) {
                    var i = this;
                    this.immediateExecutor.setImmediate(function() {
                        return i.emit(e, t)
                    })
                }, e.prototype.many = function(e, t, i) {
                    this.eventEmitter.many(this.getEventString(e), t, i)
                }, e.prototype.off = function(e, t) {
                    this.eventEmitter.off(this.getEventString(e), t)
                }, e.prototype.offAny = function(e) {
                    this.eventEmitter.offAny(e)
                }, e.prototype.on = function(e, t) {
                    this.eventEmitter.on(this.getEventString(e), t)
                }, e.prototype.onAny = function(e) {
                    this.eventEmitter.onAny(e)
                }, e.prototype.once = function(e, t) {
                    this.eventEmitter.once(this.getEventString(e), t)
                }, e.prototype.removeAllListeners = function(e) {
                    void 0 === e && (e = null), null === e ? this.eventEmitter.removeAllListeners() : this.eventEmitter.removeAllListeners(this.getEventString(e))
                }, e.prototype.setMaxListeners = function(e) {
                    this.eventEmitter.setMaxListeners(e)
                }, e.prototype.listeners = function(e) {
                    return this.eventEmitter.listeners(this.getEventString(e))
                }, e.prototype.listenersAny = function() {
                    return this.eventEmitter.listenersAny()
                }, e.prototype.getEventString = function(e) {
                    return "" + e
                }, e.UNHANDLED_EVENT_LISTENER_ERROR_METRIC = "UnhandledEventListenerError", e)
            }();
        t.exports = c
    }, {
        187: 187,
        254: 254,
        283: 283,
        284: 284
    }],
    161: [function(e, t, i) {
        "use strict";
        var n = e(187),
            r = e(281),
            o = e(284),
            s = e(294),
            a = r.of("MulticastCallback"),
            l = o.get("MulticastCallback"),
            c = function() {
                function e() {
                    this.callbacks = []
                }
                return (e.prototype.add = function(e) {
                    a(e, "callback").exists(), this.callbacks = s.union(this.callbacks, [e])
                }, e.prototype.remove = function(e) {
                    a(e, "callback").exists(), this.callbacks = s.without(this.callbacks, e)
                }, e.prototype.removeAll = function() {
                    this.callbacks = []
                }, e.prototype.invoke = function(t) {
                    for (var i = 0, r = this.callbacks; i < r.length; i++) {
                        var o = r[i];
                        try {
                            o(t)
                        } catch (t) {
                            l.error("An error occurred in a callback", t), n.putMetric(e.UNHANDLED_EVENT_LISTENER_ERROR_METRIC)
                        }
                    }
                }, e.UNHANDLED_EVENT_LISTENER_ERROR_METRIC = "UnhandledEventListenerError", e)
            }();
        t.exports = c
    }, {
        187: 187,
        281: 281,
        284: 284,
        294: 294
    }],
    162: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(281),
            o = e(284),
            s = e(294),
            a = r.of("MultiplexedEventSource"),
            l = o.get("MultiplexedEventSource"),
            c = function() {
                function e(e) {
                    void 0 === e && (e = !1), this.sources = [], this.allListeners = [], this.bufferedCallbacks = [], this.bufferInactiveEvents = e
                }
                return (e.prototype.selectSource = function(e) {
                    a(e, "id").exists();
                    var t = s.find(this.sources, function(t) {
                        return t.id === e
                    });
                    a(t, "entry with id " + e).exists(), this.active = t.source, this.reEmitEventsForSourceId(e)
                }, e.prototype.addSource = function(e, t) {
                    var i = this;
                    a(e, "id").exists(), a(t, "source").exists();
                    var n = s.find(this.sources, function(t) {
                        return t.id === e
                    });
                    a(n, "source with id " + e).not.exists(), this.sources.push({
                        id: e,
                        source: t
                    }), s.each(this.allListeners, function(n) {
                        i.listenDownstream(e, t, n)
                    })
                }, e.prototype.removeSource = function(e) {
                    var t = this;
                    a(e, "id").exists();
                    var i = s.partition(this.sources, function(t) {
                        return t.id === e
                    });
                    a(i[0], "source with id " + e).is.not.empty(), this.sources = i[1];
                    var n = i[0][0];
                    s.each(this.allListeners, function(i) {
                        t.unlistenDownstream(e, n.source, i)
                    }), this.active === n.source && (this.active = null)
                }, e.prototype.on = function(e, t) {
                    a(e, "event").exists(), a(t, "callback").is.a["function"]();
                    var i = {
                        event: e,
                        callback: t,
                        downstreamListeners: []
                    };
                    this.allListeners.push(i), this.listenAllDownstreams(i)
                }, e.prototype.off = function(e, t) {
                    a(e, "event").exists(), a(t, "callback").is.a["function"]();
                    var i = s.findIndex(this.allListeners, function(i) {
                        return i.event === e && i.callback === t
                    });
                    if (i !== -1) {
                        var n = this.allListeners[i];
                        this.allListeners.splice(i, 1), s.each(n.downstreamListeners, function(e) {
                            e.source.off(n.event, e.callback)
                        })
                    }
                }, e.prototype.listenAllDownstreams = function(e) {
                    var t = this;
                    s.each(this.sources, function(i) {
                        t.listenDownstream(i.id, i.source, e)
                    })
                }, e.prototype.listenDownstream = function(e, t, i) {
                    var n = this,
                        r = function() {
                            for (var r = [], o = 0; o < arguments.length; o++) r[o] = arguments[o];
                            var s = function() {
                                return i.callback.apply(n, r)
                            };
                            n.active === t ? s() : n.bufferInactiveEvents && (n.bufferCallbackForId(e, s), l.debug("Received event " + i.event + " from inactive source " + e + ". Buffering for when source is active."))
                        };
                    i.downstreamListeners.push({
                        source: t,
                        callback: r
                    }), t.on(i.event, r)
                }, e.prototype.unlistenDownstream = function(e, t, i) {
                    i.downstreamListeners = s.reject(i.downstreamListeners, function(e) {
                        return e.source === t && (t.off(i.event, e.callback), !0)
                    });
                    var r = this.getBufferedCallbacksForId(e);
                    n.exists(r) && (r.callbacks = [])
                }, e.prototype.bufferCallbackForId = function(e, t) {
                    var i = this.getBufferedCallbacksForId(e);
                    n.isNullOrUndefined(i) && (i = {
                        id: e,
                        callbacks: []
                    }, this.bufferedCallbacks.push(i)), i.callbacks.push(t)
                }, e.prototype.reEmitEventsForSourceId = function(e) {
                    if (this.bufferInactiveEvents) {
                        var t = this.getBufferedCallbacksForId(e);
                        if (n.exists(t) && !s.isEmpty(t.callbacks)) {
                            for (var i = 0, r = t.callbacks; i < r.length; i++) {
                                var o = r[i];
                                o()
                            }
                            t.callbacks = []
                        }
                    }
                }, e.prototype.getBufferedCallbacksForId = function(e) {
                    return s.find(this.bufferedCallbacks, function(t) {
                        return t.id === e
                    })
                }, e)
            }();
        t.exports = c
    }, {
        281: 281,
        284: 284,
        294: 294,
        93: 93
    }],
    163: [function(e, t, i) {
        "use strict";
        var n = e(160),
            r = e(161),
            o = e(162),
            s = {
                EventEmitter: n,
                MulticastCallback: r,
                MultiplexedEventSource: o
            };
        t.exports = s
    }, {
        160: 160,
        161: 161,
        162: 162
    }],
    164: [function(e, t, i) {
        "use strict";
        var n = e(80),
            r = n.MarketplaceID,
            o = e(93),
            s = e(294),
            a = function() {
                function e() {}
                return (e.hasEndpoint = function(t) {
                    return !o.isNullOrUndefined(e.getEndpointForMarketplace(t))
                }, e.getEmptyResponseBaseUrl = function(t, i) {
                    return e.getBaseUrl(t, i) + "OE"
                }, e.getImageResponseBaseUrl = function(t, i) {
                    return e.getBaseUrl(t, i) + "OP"
                }, e.getBaseUrl = function(t, i) {
                    return "https://" + e.getEndpointForMarketplace(t) + "/1/" + i + "/1/"
                }, e.getEndpointForMarketplace = function(t) {
                    for (var i in e.ENDPOINTS)
                        if (s.contains(e.ENDPOINTS[i], t)) return i;
                    return null
                }, e.ENDPOINTS = {
                    "fls-eu.amazon.com": [r.DE, r.ROE_EU, r.ROW_EU, r.UK],
                    "fls-fe.amazon.com": [r.JP, r.ROW_FE],
                    "fls-na.amazon.com": [r.ROW_NA, r.US]
                }, e)
            }();
        t.exports = a
    }, {
        294: 294,
        80: 80,
        93: 93
    }],
    165: [function(e, t, i) {
        "use strict";
        var n, r = e(178),
            o = r.HttpRequest,
            s = e(93),
            a = e(281),
            l = e(285),
            c = e(164);
        ! function(e) {
            e[e.DEBUG = 0] = "DEBUG", e[e.INFO = 1] = "INFO", e[e.WARN = 2] = "WARN", e[e.ERROR = 3] = "ERROR"
        }(n || (n = {}));
        var u = a.of("LogPublisher"),
            p = function() {
                function e(t, i, n, r, s) {
                    void 0 === r && (r = 0), void 0 === s && (s = o), u(t, "marketplaceId").exists(), u(c.hasEndpoint(t), "hasEndpoint(marketplaceId)").is["true"](), this.customerId = u(i, "customerId").exists(), this.sessionId = u(n, "sessionId").exists(), this.url = c.getEmptyResponseBaseUrl(t, e.CHANNEL), this.batchSize = r, this.lines = [], this.httpRequest = s
                }
                return (e.prototype.updateSessionId = function(e) {
                    this.flush(), this.sessionId = u(e, "sessionId").exists()
                }, e.isSupported = function() {
                    return o.isSupported() && "undefined" != typeof Array.isArray && "undefined" != typeof Array.prototype.map && "undefined" != typeof Date.prototype.toISOString && "undefined" != typeof JSON
                }, e.prototype.log = function(e, t) {
                    var i = this;
                    u(e, "level").exists();
                    var r = n[e.toUpperCase()];
                    u(r, "logLevel").exists(), u(t, "message").is.not.empty();
                    var o = t.map(function(e) {
                            return i.stringify(e)
                        }),
                        s = this.escapeNewlines(o.join(" ")),
                        a = "[" + this.customerId + "] [" + this.sessionId + "] [" + this.getTime() + "] [" + n[r] + "] " + s + "\n";
                    this.lines.push(a), (this.lines.length >= this.batchSize || r >= n.ERROR) && this.flush()
                }, e.prototype.flush = function() {
                    if (!(this.lines.length < 1)) {
                        var e = this.lines.join("");
                        this.httpRequest.post(this.url).body(e).send(), this.lines = []
                    }
                }, e.prototype.getTime = function() {
                    return (new Date).toISOString()
                }, e.prototype.stringify = function(e) {
                    var t = this;
                    if ("function" == typeof e) return "[function]";
                    if ("object" != typeof e) return "" + e;
                    if (e instanceof Error) {
                        for (var i = e, n = []; i instanceof Error;) {
                            var r = i.toString();
                            s.isNullOrUndefined(i.stack) || (r += " [" + i.stack + "]"), n.push(r), i = i.cause
                        }
                        return n.join(" caused by: ")
                    }
                    return this.hasOverriddenToString(e) ? e.toString() : l(e, function(e, i) {
                        return t.hasOverriddenToString(i) ? i.toString() : i
                    })
                }, e.prototype.hasOverriddenToString = function(t) {
                    return "object" == typeof t && null !== t && !Array.isArray(t) && t.toString() !== e.DEFAULT_OBJECT_STRING
                }, e.prototype.escapeNewlines = function(e) {
                    return e.replace(/\r?\n/g, "\\n")
                }, e.CHANNEL = "aiv-web-player", e.DEFAULT_OBJECT_STRING = (new Object).toString(), e)
            }();
        t.exports = p
    }, {
        164: 164,
        178: 178,
        281: 281,
        285: 285,
        93: 93
    }],
    166: [function(e, t, i) {
        "use strict";
        var n = e(80),
            r = n.MarketplaceID,
            o = e(178),
            s = o.HttpRequest,
            a = e(93),
            l = e(281),
            c = e(294),
            u = e(164),
            p = l.of("MetricsPublisher"),
            h = function() {
                function e(t, i, n, o, a, l, c, h) {
                    void 0 === a && (a = 0), void 0 === l && (l = e.FLUSH_TIMEOUT_MS), void 0 === c && (c = s), void 0 === h && (h = Image), this.batchedMetrics = [], this.marketplaceId = p(t, "marketplaceID").exists(), p(u.hasEndpoint(t), "hasEndpoint(marketplaceID)").is["true"](), this.sessionId = p(i, "sessionId").exists(), this.client = p(o, "client").exists(), this.clientId = p(n, "clientId").exists(), e.MARKETPLACES[r.DE] = "de", e.MARKETPLACES[r.JP] = "jp", e.MARKETPLACES[r.ROE_EU] = "roe_eu", e.MARKETPLACES[r.ROW_EU] = "row_eu", e.MARKETPLACES[r.ROW_FE] = "row_fe", e.MARKETPLACES[r.ROW_NA] = "row_na", e.MARKETPLACES[r.UK] = "uk", e.MARKETPLACES[r.US] = "us", this.batchSize = a, this.flushTimeoutMs = l, c.isSupported() ? (this.baseUrl = u.getEmptyResponseBaseUrl(t, e.CHANNEL) + "/aiv-web-player/action/", this.handler = this.putMetricXhr) : (this.baseUrl = u.getImageResponseBaseUrl(t, e.CHANNEL) + "/aiv-web-player/action/", this.handler = this.putMetricImg), this.httpRequest = c, this.imageType = h
                }
                return (e.prototype.updateSessionId = function(e) {
                    this.sessionId = p(e, "sessionId").exists()
                }, e.prototype.putMetric = function(e, t, i, n) {
                    var r = this;
                    void 0 === i && (i = 1), p(e, "feature").exists(), p(t, "resource").exists();
                    var o = e + ":" + t + "@v=" + i;
                    a.isNullOrUndefined(n) || (o = o + ":u=" + n), this.batchedMetrics.push(o), this.batchedMetrics.length >= this.batchSize ? this.flush() : (clearTimeout(this.flushTimeoutId), this.flushTimeoutId = setTimeout(function() {
                        r.flush()
                    }, this.flushTimeoutMs))
                }, e.prototype.flush = function() {
                    if (!(this.batchedMetrics.length < 1)) {
                        clearTimeout(this.flushTimeoutId);
                        var e = this.baseUrl + this.batchedMetrics.join("/");
                        this.handler(e, !0), this.handler(e, !1), this.batchedMetrics = []
                    }
                }, e.prototype.putMetricImg = function(e, t) {
                    var i = this.createMetricParameters(t),
                        n = c.map(i, function(e, t) {
                            return t + "=" + e
                        }),
                        r = new this.imageType;
                    r.src = e + "?" + n.join("&")
                }, e.prototype.putMetricXhr = function(e, t) {
                    this.httpRequest.get(e).query(this.createMetricParameters(t)).send()
                }, e.prototype.createMetricParameters = function(t) {
                    var i = {
                        instance: this.clientId,
                        marketplace: e.MARKETPLACES[this.marketplaceId],
                        marketplaceId: this.marketplaceId,
                        requestId: 1,
                        service: e.SERVICE_NAME,
                        session: this.sessionId
                    };
                    return (t && (i.client = this.client), i)
                }, e.CHANNEL = "action-impressions", e.SERVICE_NAME = "AIVWebPlayer", e.FLUSH_TIMEOUT_MS = 3e4, e.MARKETPLACES = {}, e)
            }();
        t.exports = h
    }, {
        164: 164,
        178: 178,
        281: 281,
        294: 294,
        80: 80,
        93: 93
    }],
    167: [function(e, t, i) {
        "use strict";
        var n = e(165),
            r = e(166),
            o = {
                LogPublisher: n,
                MetricsPublisher: r
            };
        t.exports = o
    }, {
        165: 165,
        166: 166
    }],
    168: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(294),
            o = e(80),
            s = o.MarketplaceID,
            a = e(93),
            l = e(58),
            c = l.TokenType,
            u = e(178),
            p = e(252),
            h = p.ExponentialBackoffRetry,
            f = u.HttpRequest,
            d = e(159),
            y = e(169),
            g = n.of("GetSectionsClient"),
            m = function() {
                function e(t, i, n, r, o, s, a) {
                    void 0 === s && (s = f), void 0 === a && (a = h), this.authContext = g(o, "authContext").exists(), this.deviceId = g(t, "deviceId").exists(), this.deviceTypeId = g(i, "deviceTypeId").exists(), this.marketplaceId = g(n, "marketplaceId").exists(), this.url = g(r, "partnerServiceUrl").exists().replace(/\/$/, "") + e.SERVICE_ENDPOINT, this.requestType = s, this.retryType = a
                }
                return (e.prototype.getSections = function(t, i, n, r) {
                    var o = this;
                    g(t, "asin").exists(), g(i, "decorationScheme").exists(), g(n, "onSuccess").exists(), g(r, "onError").exists();
                    var s, l = (new this.retryType).withMaxAttempts(e.MAX_ATTEMPTS).withInitialIntervalMs(e.BASE_RETRY).withExponentialFactor(2).withRandomizationFactor(e.JITTER).onError(function(e) {
                        return o.onFail(r, e)
                    }).onAttempt(function(e, a) {
                        s = o.sendRequest(t, i, e, a, n, r)
                    }).execute();
                    return function() {
                        l.abort(), a.exists(s) && s.abort()
                    }
                }, e.prototype.sendRequest = function(t, i, n, o, l, u) {
                    var p = this,
                        h = this.requestType.get(this.url),
                        f = {
                            deviceID: this.deviceId,
                            deviceTypeID: this.deviceTypeId,
                            marketplaceId: this.marketplaceId,
                            pageId: t,
                            firmware: "1",
                            format: "json",
                            gascEnabled: !s.isClassic(this.marketplaceId),
                            version: "default",
                            sectionTypes: "bottom",
                            pageType: "player",
                            decorationScheme: i
                        };
                    return (h.accept("application/json").withCredentials().header(e.RETRY_COUNT_HEADER, "" + n).header(e.REQUEST_PRIORITY_HEADER, e.PRIORITY_CRITICAL).onLoad(function(e) {
                        p.onLoad(e, o, l, u)
                    }).onError(function(t, i) {
                        var n = "An error occurred while sending GetSections request: " + t,
                            s = d.newError(e.ERROR, n, t);
                        a.isNullOrUndefined(i) || !r.contains(e.UNRETRIABLE_HTTP_ERRORS, i.getStatus()) ? o(s, i) : p.onFail(u, s, i);
                    }), a.exists(this.authContext.tokenClient) ? this.authContext.tokenClient.getToken(function(e) {
                        e.type === c.OAUTH && h.bearerToken(e.token), h.query(f).send()
                    }, u) : h.query(f).send(), h)
                }, e.prototype.onLoad = function(t, i, n, o) {
                    var s = t.getText(),
                        l = t.getLatencyMs();
                    if (null === s) return void i(d.newError(e.ERROR, "GetSections response is null."), t);
                    var c;
                    try {
                        c = JSON.parse(s)
                    } catch (n) {
                        var u = "An error occurred while parsing the GetSections JSON response: " + n;
                        return void i(d.newError(e.ERROR, u, n), t)
                    }
                    var p = a.nestedValue(c, "sections"),
                        h = null;
                    r.isEmpty(p) || (h = new y(c)), n(h, l)
                }, e.prototype.onFail = function(e, t, i) {
                    var n;
                    a.exists(i) && (n = i.getLatencyMs()), e(t, n)
                }, e.MAX_ATTEMPTS = 4, e.JITTER = 200, e.BASE_RETRY = 500, e.UNRETRIABLE_HTTP_ERRORS = [400, 401, 403], e.RETRY_COUNT_HEADER = "x-retry-count", e.REQUEST_PRIORITY_HEADER = "x-request-priority", e.PRIORITY_CRITICAL = "CRITICAL", e.ERROR = "GetSectionsError", e.SERVICE_ENDPOINT = "/cdp/discovery/GetSections", e)
            }();
        t.exports = m
    }, {
        159: 159,
        169: 169,
        178: 178,
        252: 252,
        281: 281,
        294: 294,
        58: 58,
        80: 80,
        93: 93
    }],
    169: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(281),
            o = e(294),
            s = r.of("GetSectionsResponse"),
            a = function() {
                function e(e) {
                    var t = this;
                    this.asin = null, this.episodeImageURL = null, this.episodeNumber = null, this._hasCaptions = !1, this._isPlayable = !1, this.regulatoryRating = null, this.seasonNumber = null, this.seriesTitle = null, this.title = null, this.autoplayEnabled = !1, this.numTitlesToAutoplay = null, this.showAutoplayCard = !1, this.preferredImageName = null, this.inSameSeries = !0, this.recommendationStrategy = null, s(e, "getSectionsJson").exists();
                    var i = n.nestedValue(e, "sections.bottom.collections.collectionList.0.items.itemList.0"),
                        r = n.nestedValue(i, "decoratedTitle"),
                        a = n.nestedValue(e, "sections.bottom.collections.collectionList.0.autoplayConfig"),
                        l = n.nestedValue(i, "metadata");
                    if (this.asin = n.nestedValue(i, "titleId"), this.autoplayEnabled = n.nestedValue(a, "autoplayEnabled") === !0, this.numTitlesToAutoplay = n.nestedValue(a, "numTitlesToAutoplay"), this.showAutoplayCard = n.nestedValue(a, "showAutoplayCard") === !0, this.preferredImageName = n.nestedValue(a, "autoplayCardPreferredImage"), n.exists(l) && (this.recommendationStrategy = n.withDefault(o.find(Object.keys(l), function(e) {
                            return 0 === e.indexOf("source.")
                        }), null), this.inSameSeries = "source.OtherEpisodes" === this.recommendationStrategy), null !== r) {
                        this.episodeImageURL = n.nestedValue(r, "images.imageUrls.next_up"), this.episodeNumber = n.nestedValue(r, "catalog.episodeNumber"), this._hasCaptions = n.nestedValue(r, "computed.simple.CLOSED_CAPTION_BADGE") === !0, this._isPlayable = n.nestedValue(r, "playback.summary.isPlayable") === !0, this.regulatoryRating = n.nestedValue(r, "catalog.regulatoryRating"), this.title = n.nestedValue(r, "catalog.title");
                        var c = n.nestedValue(r, "family.tvAncestors");
                        n.isNullOrUndefined(c) || c.forEach(function(e) {
                            var i = n.nestedValue(e, "catalog.type");
                            "SEASON" === i ? t.seasonNumber = n.nestedValue(e, "catalog.seasonNumber") : "SHOW" === i && (t.seriesTitle = n.nestedValue(e, "catalog.title"))
                        })
                    }
                }
                return (e.prototype.getAsin = function() {
                    return this.asin
                }, e.prototype.getEpisodeImageURL = function() {
                    return this.episodeImageURL
                }, e.prototype.getEpisodeNumber = function() {
                    return this.episodeNumber
                }, e.prototype.getRegulatoryRating = function() {
                    return this.regulatoryRating
                }, e.prototype.getSeasonNumber = function() {
                    return this.seasonNumber
                }, e.prototype.getSeriesTitle = function() {
                    return this.seriesTitle
                }, e.prototype.getTitle = function() {
                    return this.title
                }, e.prototype.hasCaptions = function() {
                    return this._hasCaptions
                }, e.prototype.isPlayable = function() {
                    return this._isPlayable
                }, e.prototype.isAutoplayEnabled = function() {
                    return this.autoplayEnabled
                }, e.prototype.getNumTitlesToAutoplay = function() {
                    return this.numTitlesToAutoplay
                }, e.prototype.getShowAutoplayCard = function() {
                    return this.showAutoplayCard
                }, e.prototype.isInSameSeries = function() {
                    return this.inSameSeries
                }, e.prototype.getPreferredImageName = function() {
                    return this.preferredImageName
                }, e.prototype.getRecommendationStrategy = function() {
                    return this.recommendationStrategy
                }, e)
            }();
        t.exports = a
    }, {
        281: 281,
        294: 294,
        93: 93
    }],
    170: [function(e, t, i) {
        "use strict";
        var n = e(168),
            r = e(169),
            o = {
                GetSectionsClient: n,
                GetSectionsResponse: r
            };
        t.exports = o
    }, {
        168: 168,
        169: 169
    }],
    171: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.GET = 0] = "GET", e[e.POST = 1] = "POST", e[e.PUT = 2] = "PUT", e[e.DELETE = 3] = "DELETE", e[e.HEAD = 4] = "HEAD", e[e.OPTIONS = 5] = "OPTIONS"
        }(n || (n = {})), t.exports = n
    }, {}],
    172: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = e(93),
            o = e(281),
            s = e(275),
            a = e(171),
            l = e(174),
            c = e(175),
            u = e(176),
            p = e(177),
            h = o.of("HttpRequest"),
            f = function() {
                function e(e, t, i, r) {
                    void 0 === i && (i = window.performance), this.queryString = null, this.xhrData = null, this.xhrFields = [], this.xhrRequestHeaders = {}, this.xhrResponseType = null, this.xhrTimeout = 0, this.xhrWithCredentials = !1, this.latencyTimeoutId = null, this.latencyTimeoutMs = 0, this.startTime = null, this.delayMs = 0, this.delayTimeoutId = null, this.sent = !1, this.isAborted = !1, this.abortXhr = null, this.loadCallbacks = [], this.errorCallbacks = [], this.timeoutCallbacks = [], this.abortCallbacks = [], this.progressCallbacks = [], this.eventListeners = [], this.method = h(e, "method").exists(), this.url = h(t, "url").exists(), this.performance = i, this.xhrType = n.isUndefined(r) && "undefined" != typeof XMLHttpRequest ? XMLHttpRequest : r
                }
                return (e.isSupported = function(e) {
                    if (n.isUndefined(e) && "undefined" != typeof XMLHttpRequest && (e = XMLHttpRequest), "undefined" == typeof JSON || "undefined" == typeof e || n.isUndefined(Array.prototype.forEach)) return !1;
                    var t = new e;
                    return !(n.isUndefined(t.abort) || n.isUndefined(t.addEventListener) || n.isUndefined(t.getAllResponseHeaders) || n.isUndefined(t.open) || n.isUndefined(t.readyState) || n.isUndefined(t.removeEventListener) || n.isUndefined(t.response) || n.isUndefined(t.responseText) || n.isUndefined(t.responseType) || n.isUndefined(t.send) || n.isUndefined(t.setRequestHeader) || n.isUndefined(t.status) || n.isUndefined(t.timeout) || n.isUndefined(t.withCredentials))
                }, e.get = function(t) {
                    return (h(t, "url").exists(), new e(a.GET, t))
                }, e.post = function(t) {
                    return (h(t, "url").exists(), new e(a.POST, t))
                }, e.prototype.header = function(e, t) {
                    return (h(e, "header").exists(), h(t, "value").exists(), h(this.sent, "this.sent").is["false"](), this.xhrRequestHeaders[e] = t, this)
                }, e.prototype.headers = function(e) {
                    return (h(e, "headers").exists(), h(this.sent, "this.sent").is["false"](), n.extend(this.xhrRequestHeaders, e), this)
                }, e.prototype.contentType = function(e) {
                    return (h(e, "mimeType").exists(), h(this.sent, "this.sent").is["false"](), this.header("Content-Type", e), this)
                }, e.prototype.accept = function(e) {
                    return (h(e, "mimeType").exists(), h(this.sent, "this.sent").is["false"](), this.header("Accept", e), this)
                }, e.prototype.responseType = function(e) {
                    return (h(e, "type").exists(), h(this.sent, "this.sent").is["false"](), this.xhrResponseType = e, this)
                }, e.prototype.delay = function(e) {
                    return (h(e, "delayMs").exists(), h(this.sent, "this.sent").is["false"](), this.delayMs = e, this)
                }, e.prototype.timeout = function(e, t) {
                    return (h(e, "timeoutMs").exists(), h(this.sent, "this.sent").is["false"](), t === u.LATENCY ? this.latencyTimeoutMs = e : this.xhrTimeout = e, this)
                }, e.prototype.withCredentials = function() {
                    return (h(this.sent, "this.sent").is["false"](), this.xhrWithCredentials = !0, this)
                }, e.prototype.query = function(e) {
                    return (h(e, "parameters").exists(), h(this.sent, "this.sent").is["false"](), this.queryString = p.toUriEncodedString(n.map(e, function(e, t) {
                        return new p(t, e)
                    })), this)
                }, e.prototype.body = function(e) {
                    return (h(e, "data").exists(), h(this.sent, "this.sent").is["false"](), h(this.xhrFields, "this.xhrFields").is.empty(), this.xhrData = e, this)
                }, e.prototype.json = function(e) {
                    return (h(e, "data").exists(), h(this.sent, "this.sent").is["false"](), h(this.xhrFields, "this.xhrFields").is.empty(), this.xhrData = JSON.stringify(e), this.contentType("application/json"), this)
                }, e.prototype.field = function(e, t) {
                    return (h(e, "name").exists(), h(t, "value").exists(), h(this.sent, "this.sent").is["false"](), h(this.xhrData, "this.xhrData").is["null"](), this.xhrFields.push(new p(e, t)), this.contentType("application/x-www-form-urlencoded"), this)
                }, e.prototype.fields = function(e) {
                    var t = this;
                    return (h(e, "fields").exists(), h(this.sent, "this.sent").is["false"](), h(this.xhrData, "this.xhrData").is["null"](), n.each(e, function(e, i) {
                        t.xhrFields.push(new p(i, e))
                    }), this.contentType("application/x-www-form-urlencoded"), this)
                }, e.prototype.bearerToken = function(e) {
                    return (h(e, "token").exists(), h(this.sent, "this.sent").is["false"](), this.header("Authorization", "Bearer " + e), this)
                }, e.prototype.onLoad = function(e) {
                    return (h(e, "callback").exists(), h(this.sent, "this.sent").is["false"](), this.loadCallbacks = n.union(this.loadCallbacks, [e]), this)
                }, e.prototype.onProgress = function(e) {
                    return (h(e, "callback").exists(), h(this.sent, "this.sent").is["false"](), this.progressCallbacks = n.union(this.progressCallbacks, [e]), this)
                }, e.prototype.onError = function(e) {
                    return (h(e, "callback").exists(), h(this.sent, "this.sent").is["false"](), this.errorCallbacks = n.union(this.errorCallbacks, [e]), this)
                }, e.prototype.onTimeout = function(e) {
                    return (h(e, "callback").exists(), h(this.sent, "this.sent").is["false"](), this.timeoutCallbacks = n.union(this.timeoutCallbacks, [e]), this)
                }, e.prototype.onAbort = function(e) {
                    return (h(e, "callback").exists(), h(this.sent, "this.sent").is["false"](), this.abortCallbacks = n.union(this.abortCallbacks, [e]), this)
                }, e.prototype.abort = function() {
                    this.isAborted = !0, null !== this.delayTimeoutId && (s.clearTimeout(this.delayTimeoutId), this.delayTimeoutId = null, this.handleAbort({
                        requestStarted: !1,
                        responseReceived: !1,
                        loadedBytes: 0,
                        totalBytes: 0,
                        latencyMs: 0,
                        downloadMs: 0,
                        totalMs: 0
                    })), null !== this.abortXhr && (this.abortXhr(), this.abortXhr = null)
                }, e.prototype.send = function() {
                    var e = this;
                    return (this.delayMs > 0 ? this.delayTimeoutId = s.setTimeout(function() {
                        e.delayTimeoutId = null, e.sendRequest()
                    }, this.delayMs) : this.sendRequest(), this)
                }, e.prototype.sendRequest = function() {
                    var e = this;
                    this.sent = !0;
                    var t = a[this.method],
                        i = n.isEmpty(this.queryString) ? this.url : this.url + "?" + this.queryString,
                        r = new this.xhrType;
                    (this.loadCallbacks.length > 0 || this.progressCallbacks.length > 0 || this.errorCallbacks.length > 0 || this.timeoutCallbacks.length > 0 || this.abortCallbacks.length > 0) && this.addEventListeners(r), r.open(t, i, !0), n.each(this.xhrRequestHeaders, function(e, t) {
                        r.setRequestHeader(t, e)
                    }), null !== this.xhrResponseType && (r.responseType = c[this.xhrResponseType].toLowerCase()), this.xhrTimeout > 0 && (r.timeout = this.xhrTimeout), this.latencyTimeoutMs > 0 && (this.latencyTimeoutMs < this.xhrTimeout || 0 === this.xhrTimeout) && (this.latencyTimeoutId = s.setTimeout(function() {
                        e.removeAllListeners(r), null !== e.abortXhr && e.abortXhr(), e.abortXhr = null, e.handleTimeout(e.createTimeoutEvent(e.latencyTimeoutMs, e.startTime, null, u.LATENCY))
                    }, this.latencyTimeoutMs)), this.xhrWithCredentials && (r.withCredentials = !0), this.abortXhr = function() {
                        r.abort()
                    }, null !== this.xhrData ? (r.send(this.xhrData), this.xhrData = null) : n.isEmpty(this.xhrFields) ? r.send() : (r.send(p.toWwwFormUrlEncodedString(this.xhrFields)), this.xhrFields.length = 0)
                }, e.prototype.addEventListeners = function(e) {
                    var t = this,
                        i = null;
                    this.addEventListener(e, "loadstart", function(e) {
                        t.startTime = t.correctXHREventTimeStamp(e.timeStamp)
                    });
                    var n = function(r) {
                        e.readyState === t.xhrType.HEADERS_RECEIVED && (i = t.correctXHREventTimeStamp(r.timeStamp), e.removeEventListener("readystatechange", n), s.clearTimeout(t.latencyTimeoutId))
                    };
                    this.addEventListener(e, "readystatechange", n), (this.loadCallbacks.length > 0 || this.errorCallbacks.length > 0) && this.addEventListener(e, "load", function(n) {
                        if (t.abortXhr = null, t.removeAllListeners(e), t.isAborted) t.handleAbort(t.createAbortEvent(t.startTime, i, n));
                        else {
                            var r = t.createLoadEvent(t.startTime, i, n);
                            t.handleLoad(l.createFromXMLHttpRequest(e, r))
                        }
                    }), this.progressCallbacks.length > 0 && this.addEventListener(e, "progress", function(e) {
                        t.isAborted || t.handleProgress(t.createProgressEvent(t.startTime, i, e))
                    }), this.errorCallbacks.length > 0 && this.addEventListener(e, "error", function(n) {
                        t.abortXhr = null, t.removeAllListeners(e), t.isAborted ? t.handleAbort(t.createAbortEvent(t.startTime, i, n)) : t.handleError()
                    }), (this.timeoutCallbacks.length > 0 || this.errorCallbacks.length > 0) && this.addEventListener(e, "timeout", function(n) {
                        t.abortXhr = null, t.removeAllListeners(e), t.isAborted ? t.handleAbort(t.createAbortEvent(t.startTime, i, n)) : t.handleTimeout(t.createTimeoutEvent(e.timeout, t.startTime, i, u.DEFAULT, n))
                    }), this.abortCallbacks.length > 0 && this.addEventListener(e, "abort", function(n) {
                        t.abortXhr = null, t.removeAllListeners(e), t.handleAbort(t.createAbortEvent(t.startTime, i, n))
                    })
                }, e.prototype.addEventListener = function(e, t, i) {
                    this.eventListeners.push({
                        type: t,
                        listener: i
                    }), e.addEventListener(t, i)
                }, e.prototype.removeAllListeners = function(e) {
                    this.eventListeners.forEach(function(t) {
                        e.removeEventListener(t.type, t.listener)
                    }), this.eventListeners.length = 0
                }, e.prototype.handleLoad = function(e) {
                    if (e.getStatus() >= 200 && e.getStatus() < 400) this.loadCallbacks.forEach(function(t) {
                        t(e)
                    });
                    else {
                        var t = new Error("Request to " + this.url + " resulted in HTTP status code " + e.getStatus());
                        this.errorCallbacks.forEach(function(i) {
                            i(t, e)
                        })
                    }
                    this.clearCallbacks()
                }, e.prototype.handleProgress = function(e) {
                    this.progressCallbacks.forEach(function(t) {
                        t(e)
                    })
                }, e.prototype.handleError = function() {
                    var e = new Error("An unknown network error occurred while attempting a request to " + this.url + " with query string " + this.queryString);
                    this.errorCallbacks.forEach(function(t) {
                        t(e)
                    }), s.clearTimeout(this.latencyTimeoutId), this.clearCallbacks()
                }, e.prototype.handleTimeout = function(e) {
                    if (this.timeoutCallbacks.length > 0) this.timeoutCallbacks.forEach(function(t) {
                        t(e)
                    });
                    else {
                        var t = new Error("Request to " + this.url + " timed out after " + e.timeoutMs + " ms");
                        this.errorCallbacks.forEach(function(e) {
                            e(t)
                        })
                    }
                    this.clearCallbacks()
                }, e.prototype.handleAbort = function(e) {
                    this.abortCallbacks.forEach(function(t) {
                        t(e)
                    }), s.clearTimeout(this.latencyTimeoutId), this.clearCallbacks()
                }, e.prototype.createProgressEvent = function(e, t, i) {
                    return n.extend({
                        lengthComputable: i.lengthComputable
                    }, this.createHttpEvent(e, t, i))
                }, e.prototype.createAbortEvent = function(e, t, i) {
                    return n.extend({
                        requestStarted: !0
                    }, this.createHttpEvent(e, t, i))
                }, e.prototype.createTimeoutEvent = function(e, t, i, r, o) {
                    return n.extend({
                        timeoutMs: e,
                        timeoutType: r
                    }, this.createHttpEvent(t, i, o))
                }, e.prototype.createLoadEvent = function(e, t, i) {
                    return n.extend({}, this.createHttpEvent(e, t, i), {
                        lengthComputable: !0,
                        loadedBytes: i.loaded,
                        totalBytes: i.loaded
                    })
                }, e.prototype.createHttpEvent = function(e, t, i) {
                    var n = r.exists(i) && i.lengthComputable;
                    return {
                        responseReceived: null !== t,
                        loadedBytes: n ? i.loaded : 0,
                        totalBytes: n ? i.total : 0,
                        latencyMs: null !== t ? t - e : 0,
                        downloadMs: r.exists(i) && null !== t ? this.correctXHREventTimeStamp(i.timeStamp) - t : 0,
                        totalMs: r.exists(i) && null !== e ? this.correctXHREventTimeStamp(i.timeStamp) - e : 0
                    }
                }, e.prototype.clearCallbacks = function() {
                    this.loadCallbacks.length = 0, this.errorCallbacks.length = 0, this.timeoutCallbacks.length = 0, this.abortCallbacks.length = 0, this.progressCallbacks.length = 0
                }, e.prototype.correctXHREventTimeStamp = function(e) {
                    return r.isNullOrUndefined(this.performance) ? e : e - this.performance.now() <= 1e3 ? e : this.performance.now()
                }, e)
            }();
        t.exports = f
    }, {
        171: 171,
        174: 174,
        175: 175,
        176: 176,
        177: 177,
        275: 275,
        281: 281,
        294: 294,
        93: 93
    }],
    173: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(294),
            o = e(171),
            s = e(172),
            a = n.of("HttpRequestBuilder"),
            l = function() {
                function e(e, t, i) {
                    void 0 === i && (i = s), this.parameters = {}, this.xhrFields = {}, this.xhrRequestHeaders = {}, this.xhrResponseType = null, this.xhrWithCredentials = !1, this.bodyData = null, this.jsonData = null, this.timeoutMs = 0, this.timeoutType = null, this.method = a(e, "method").exists(), this.url = a(t, "url").exists(), this.httpRequestType = i
                }
                return (e.prototype.build = function() {
                    var e = new this.httpRequestType(this.method, this.url);
                    return (r.isEmpty(this.parameters) || e.query(this.parameters), r.isEmpty(this.xhrFields) || e.fields(this.xhrFields), r.isEmpty(this.xhrRequestHeaders) || e.headers(this.xhrRequestHeaders), null !== this.xhrResponseType && e.responseType(this.xhrResponseType), this.xhrWithCredentials && e.withCredentials(), null !== this.bodyData && e.body(this.bodyData), null !== this.jsonData && e.json(this.jsonData), 0 !== this.timeoutMs && e.timeout(this.timeoutMs, this.timeoutType), e)
                }, e.get = function(t, i) {
                    return (void 0 === i && (i = s), a(t, "url").exists(), new e(o.GET, t, i))
                }, e.post = function(t, i) {
                    return (void 0 === i && (i = s), a(t, "url").exists(), new e(o.POST, t, i))
                }, e.prototype.header = function(e, t) {
                    return (a(e, "header").exists(), a(t, "value").exists(), this.xhrRequestHeaders[e] = t, this)
                }, e.prototype.headers = function(e) {
                    return (a(e, "headers").exists(), r.extend(this.xhrRequestHeaders, e), this)
                }, e.prototype.contentType = function(e) {
                    return (a(e, "mimeType").exists(), this.header("Content-Type", e), this)
                }, e.prototype.accept = function(e) {
                    return (a(e, "mimeType").exists(), this.header("Accept", e), this)
                }, e.prototype.responseType = function(e) {
                    return (this.xhrResponseType = a(e, "type").exists(), this)
                }, e.prototype.timeout = function(e, t) {
                    return (this.timeoutMs = a(e, "timeoutMs").exists(), this.timeoutType = t, this)
                }, e.prototype.withCredentials = function() {
                    return (this.xhrWithCredentials = !0, this)
                }, e.prototype.query = function(e) {
                    return (this.parameters = a(e, "parameters").exists(), this)
                }, e.prototype.body = function(e) {
                    return (this.bodyData = a(e, "data").exists(), this)
                }, e.prototype.json = function(e) {
                    return (this.jsonData = a(e, "data").exists(), this)
                }, e.prototype.field = function(e, t) {
                    return (a(e, "name").exists(), a(t, "value").exists(), this.xhrFields[e] = t, this)
                }, e.prototype.fields = function(e) {
                    return (a(e, "fields").exists(), r.extend(this.xhrFields, e), this)
                }, e.prototype.bearerToken = function(e) {
                    return (a(e, "token").exists(), this.header("Authorization", "Bearer " + e), this)
                }, e)
            }();
        t.exports = l
    }, {
        171: 171,
        172: 172,
        281: 281,
        294: 294
    }],
    174: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = e(93),
            o = function() {
                function e(e, t, i, o, s, a, l, c) {
                    var u = this;
                    this.sizeBytes = 0, this.status = e, this.headers = {}, Object.keys(t).forEach(function(e) {
                        u.headers[e.toLowerCase()] = t[e]
                    }), this.arrayBuffer = i, this.blob = o, this.text = s, this.document = a, this.latencyMs = l.latencyMs, this.downloadMs = l.downloadMs, this.totalMs = l.totalMs, this.responseUrl = c;
                    var p = parseInt(this.getHeader("Content-Length"));
                    r.isNullOrUndefined(this.arrayBuffer) ? n.isNaN(p) ? this.sizeBytes = l.loadedBytes : this.sizeBytes = p : this.sizeBytes = this.arrayBuffer.byteLength
                }
                return (e.createFromXMLHttpRequest = function(t, i) {
                    var o = e.parseHeaders(t.getAllResponseHeaders()),
                        s = null;
                    n.isUndefined(t.responseURL) || (s = t.responseURL);
                    var a = r.withDefault(t.responseType, "");
                    switch (a.toLowerCase()) {
                        case "arraybuffer":
                            return new e(t.status, o, t.response, null, null, null, i, s);
                        case "blob":
                            return new e(t.status, o, null, t.response, null, null, i, s);
                        case "document":
                            return new e(t.status, o, null, null, null, t.response, i, s);
                        default:
                            return new e(t.status, o, null, null, t.responseText, null, i, s)
                    }
                }, e.prototype.getStatus = function() {
                    return this.status
                }, e.prototype.getHeader = function(e) {
                    var t = this.headers[e.toLowerCase()];
                    return n.isUndefined(t) ? null : t
                }, e.prototype.getHeaders = function() {
                    return n.clone(this.headers)
                }, e.prototype.getArrayBuffer = function() {
                    return this.arrayBuffer
                }, e.prototype.getBlob = function() {
                    return this.blob
                }, e.prototype.getText = function() {
                    return this.text
                }, e.prototype.getDocument = function() {
                    return this.document
                }, e.prototype.getLatencyMs = function() {
                    return this.latencyMs
                }, e.prototype.getDownloadMs = function() {
                    return this.downloadMs
                }, e.prototype.getTotalMs = function() {
                    return this.totalMs
                }, e.prototype.getResponseUrl = function() {
                    return this.responseUrl
                }, e.prototype.getMetrics = function() {
                    return {
                        requestStarted: !0,
                        responseReceived: !0,
                        loadedBytes: this.sizeBytes,
                        totalBytes: this.sizeBytes,
                        latencyMs: this.latencyMs,
                        downloadMs: this.downloadMs,
                        totalMs: this.totalMs
                    }
                }, e.parseHeaders = function(t) {
                    var i = {};
                    return (t.split(/\r?\n/).forEach(function(t) {
                        var n = e.HEADER_REGEX.exec(t);
                        n && (i[n[1]] = n[2])
                    }), i)
                }, e.HEADER_REGEX = /^\s*(.+?)\s*:\s*(.+?)\s*$/, e)
            }();
        t.exports = o
    }, {
        294: 294,
        93: 93
    }],
    175: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.ArrayBuffer = 0] = "ArrayBuffer", e[e.Blob = 1] = "Blob", e[e.Text = 2] = "Text", e[e.Document = 3] = "Document"
        }(n || (n = {})), t.exports = n
    }, {}],
    176: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.DEFAULT = 1] = "DEFAULT", e[e.LATENCY = 2] = "LATENCY"
        }(n || (n = {})), t.exports = n
    }, {}],
    177: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(294),
            o = n.of("KeyValuePair"),
            s = function() {
                function e(e, t) {
                    this.key = o(e, "key").exists(), this.value = o(t, "value").exists()
                }
                return (e.toUriEncodedString = function(e) {
                    return r.map(e, function(e) {
                        return e.toUriEncodedString()
                    }).join("&")
                }, e.toWwwFormUrlEncodedString = function(e) {
                    return r.map(e, function(e) {
                        return e.toWwwFormUrlEncodedString()
                    }).join("&")
                }, e.prototype.toUriEncodedString = function() {
                    return encodeURIComponent(this.key) + "=" + encodeURIComponent(this.value)
                }, e.prototype.toWwwFormUrlEncodedString = function() {
                    return this.encodeWwwFormUrlComponent(this.key) + "=" + this.encodeWwwFormUrlComponent(this.value)
                }, e.prototype.encodeWwwFormUrlComponent = function(e) {
                    return encodeURIComponent(e).replace(/%20/g, "+")
                }, e)
            }();
        t.exports = s
    }, {
        281: 281,
        294: 294
    }],
    178: [function(e, t, i) {
        "use strict";
        var n = e(171),
            r = e(172),
            o = e(173),
            s = e(174),
            a = e(175),
            l = e(176),
            c = {
                HttpMethod: n,
                HttpRequest: r,
                HttpRequestBuilder: o,
                HttpResponse: s,
                HttpResponseType: a,
                HttpTimeoutType: l
            };
        t.exports = c
    }, {
        171: 171,
        172: 172,
        173: 173,
        174: 174,
        175: 175,
        176: 176
    }],
    179: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(t, i) {
                void 0 === i && (i = window);
                var n = this;
                this.VERSION = 1, this.pendingDbOperations = [], this.tryInitialize = function(t) {
                    n.database || !e.isSupported(n.windowRef) ? t() : (n.pendingDbOperations.push(t), n.dbOpenRequest || n.initialize(function() {
                        for (var e = 0; e < n.pendingDbOperations.length; e++) {
                            var t = n.pendingDbOperations[e];
                            "function" == typeof t && n.pendingDbOperations[e]()
                        }
                        n.dbOpenRequest = null, n.pendingDbOperations = []
                    }))
                }, this.initialize = function(e) {
                    try {
                        var t = n.windowRef.indexedDB.open(n.dbName, n.VERSION);
                        t.onsuccess = function(t) {
                            try {
                                n.database = t.target.result
                            } catch (e) {}
                            e()
                        }, t.onupgradeneeded = function(e) {
                            e.oldVersion < 1 && (n.database = e.target.result, n.database.createObjectStore(n.dbName))
                        }, t.onerror = function(t) {
                            t.preventDefault(), e()
                        }
                    } catch (t) {
                        e()
                    }
                }, this.dbName = t, this.windowRef = i
            }
            return (e.prototype.get = function(e, t) {
                var i = this;
                this.tryInitialize(function() {
                    try {
                        if (!i.database) return t(null);
                        var n = (new Date).getTime(),
                            r = i.database.transaction([i.dbName], "readonly").objectStore(i.dbName).get(e);
                        r.onsuccess = function(e) {
                            try {
                                var i = e.target.result;
                                t(i, (new Date).getTime() - n)
                            } catch (e) {
                                t(null)
                            }
                        }, r.onerror = function() {
                            t(null)
                        }
                    } catch (e) {
                        t(null)
                    }
                })
            }, e.prototype.set = function(e, t) {
                var i = this;
                this.tryInitialize(function() {
                    try {
                        i.database && i.database.transaction([i.dbName], "readwrite").objectStore(i.dbName).put(t, e)
                    } catch (e) {}
                })
            }, e.prototype.clear = function() {
                var e = this;
                this.tryInitialize(function() {
                    try {
                        e.database && e.database.transaction([e.dbName], "readwrite").objectStore(e.dbName).clear()
                    } catch (e) {}
                })
            }, e.isSupported = function(e) {
                return (void 0 === e && (e = window), "undefined" != typeof e.indexedDB)
            }, e)
        }();
        t.exports = n
    }, {}],
    180: [function(e, t, i) {
        "use strict";
        var n = e(179),
            r = {
                IndexedDBStorage: n
            };
        t.exports = r
    }, {
        179: 179
    }],
    181: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e) {
                this.languageCode = e
            }
            return (e.prototype.getLanguageCode = function() {
                return this.languageCode
            }, e.toString = function() {
                return "LANGUAGE_CHANGED_EVENT"
            }, e)
        }();
        t.exports = n
    }, {}],
    182: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = n.of("LanguageCodes"),
            o = function() {
                function e() {}
                return (e.getIso639_1LanguageCode = function(t) {
                    for (var i = 0; i < e.LANGUAGE_MAP.length; ++i) {
                        var n = e.LANGUAGE_MAP[i];
                        if (n["iso639-2t"] === t) return n["iso639-1"]
                    }
                    return null
                }, e.getIso639_2tLanguageCode = function(t) {
                    for (var i = 0; i < e.LANGUAGE_MAP.length; ++i) {
                        var n = e.LANGUAGE_MAP[i];
                        if (n["iso639-1"] === t) return n["iso639-2t"]
                    }
                    return null
                }, e.getLanguageCodeFromLocale = function(e) {
                    return (r(e, "locale").exists(), e = e.replace("_", "-"), e.split("-")[0])
                }, e.LANGUAGE_MAP = [{
                    "iso639-1": "ab",
                    "iso639-2t": "abk"
                }, {
                    "iso639-1": "aa",
                    "iso639-2t": "aar"
                }, {
                    "iso639-1": "af",
                    "iso639-2t": "afr"
                }, {
                    "iso639-1": "ak",
                    "iso639-2t": "aka"
                }, {
                    "iso639-1": "sq",
                    "iso639-2t": "sqi"
                }, {
                    "iso639-1": "am",
                    "iso639-2t": "amh"
                }, {
                    "iso639-1": "ar",
                    "iso639-2t": "ara"
                }, {
                    "iso639-1": "an",
                    "iso639-2t": "arg"
                }, {
                    "iso639-1": "hy",
                    "iso639-2t": "hye"
                }, {
                    "iso639-1": "as",
                    "iso639-2t": "asm"
                }, {
                    "iso639-1": "av",
                    "iso639-2t": "ava"
                }, {
                    "iso639-1": "ae",
                    "iso639-2t": "ave"
                }, {
                    "iso639-1": "ay",
                    "iso639-2t": "aym"
                }, {
                    "iso639-1": "az",
                    "iso639-2t": "aze"
                }, {
                    "iso639-1": "bm",
                    "iso639-2t": "bam"
                }, {
                    "iso639-1": "ba",
                    "iso639-2t": "bak"
                }, {
                    "iso639-1": "eu",
                    "iso639-2t": "eus"
                }, {
                    "iso639-1": "be",
                    "iso639-2t": "bel"
                }, {
                    "iso639-1": "bn",
                    "iso639-2t": "ben"
                }, {
                    "iso639-1": "bh",
                    "iso639-2t": "bih"
                }, {
                    "iso639-1": "bi",
                    "iso639-2t": "bis"
                }, {
                    "iso639-1": "bs",
                    "iso639-2t": "bos"
                }, {
                    "iso639-1": "br",
                    "iso639-2t": "bre"
                }, {
                    "iso639-1": "bg",
                    "iso639-2t": "bul"
                }, {
                    "iso639-1": "my",
                    "iso639-2t": "mya"
                }, {
                    "iso639-1": "ca",
                    "iso639-2t": "cat"
                }, {
                    "iso639-1": "ch",
                    "iso639-2t": "cha"
                }, {
                    "iso639-1": "ce",
                    "iso639-2t": "che"
                }, {
                    "iso639-1": "ny",
                    "iso639-2t": "nya"
                }, {
                    "iso639-1": "zh",
                    "iso639-2t": "zho"
                }, {
                    "iso639-1": "cv",
                    "iso639-2t": "chv"
                }, {
                    "iso639-1": "kw",
                    "iso639-2t": "cor"
                }, {
                    "iso639-1": "co",
                    "iso639-2t": "cos"
                }, {
                    "iso639-1": "cr",
                    "iso639-2t": "cre"
                }, {
                    "iso639-1": "hr",
                    "iso639-2t": "hrv"
                }, {
                    "iso639-1": "cs",
                    "iso639-2t": "ces"
                }, {
                    "iso639-1": "da",
                    "iso639-2t": "dan"
                }, {
                    "iso639-1": "dv",
                    "iso639-2t": "div"
                }, {
                    "iso639-1": "nl",
                    "iso639-2t": "nld"
                }, {
                    "iso639-1": "dz",
                    "iso639-2t": "dzo"
                }, {
                    "iso639-1": "en",
                    "iso639-2t": "eng"
                }, {
                    "iso639-1": "eo",
                    "iso639-2t": "epo"
                }, {
                    "iso639-1": "et",
                    "iso639-2t": "est"
                }, {
                    "iso639-1": "ee",
                    "iso639-2t": "ewe"
                }, {
                    "iso639-1": "fo",
                    "iso639-2t": "fao"
                }, {
                    "iso639-1": "fj",
                    "iso639-2t": "fij"
                }, {
                    "iso639-1": "fi",
                    "iso639-2t": "fin"
                }, {
                    "iso639-1": "fr",
                    "iso639-2t": "fra"
                }, {
                    "iso639-1": "ff",
                    "iso639-2t": "ful"
                }, {
                    "iso639-1": "gl",
                    "iso639-2t": "glg"
                }, {
                    "iso639-1": "ka",
                    "iso639-2t": "kat"
                }, {
                    "iso639-1": "de",
                    "iso639-2t": "deu"
                }, {
                    "iso639-1": "el",
                    "iso639-2t": "ell"
                }, {
                    "iso639-1": "gn",
                    "iso639-2t": "grn"
                }, {
                    "iso639-1": "gu",
                    "iso639-2t": "guj"
                }, {
                    "iso639-1": "ht",
                    "iso639-2t": "hat"
                }, {
                    "iso639-1": "ha",
                    "iso639-2t": "hau"
                }, {
                    "iso639-1": "he",
                    "iso639-2t": "heb"
                }, {
                    "iso639-1": "hz",
                    "iso639-2t": "her"
                }, {
                    "iso639-1": "hi",
                    "iso639-2t": "hin"
                }, {
                    "iso639-1": "ho",
                    "iso639-2t": "hmo"
                }, {
                    "iso639-1": "hu",
                    "iso639-2t": "hun"
                }, {
                    "iso639-1": "ia",
                    "iso639-2t": "ina"
                }, {
                    "iso639-1": "id",
                    "iso639-2t": "ind"
                }, {
                    "iso639-1": "ie",
                    "iso639-2t": "ile"
                }, {
                    "iso639-1": "ga",
                    "iso639-2t": "gle"
                }, {
                    "iso639-1": "ig",
                    "iso639-2t": "ibo"
                }, {
                    "iso639-1": "ik",
                    "iso639-2t": "ipk"
                }, {
                    "iso639-1": "io",
                    "iso639-2t": "ido"
                }, {
                    "iso639-1": "is",
                    "iso639-2t": "isl"
                }, {
                    "iso639-1": "it",
                    "iso639-2t": "ita"
                }, {
                    "iso639-1": "iu",
                    "iso639-2t": "iku"
                }, {
                    "iso639-1": "ja",
                    "iso639-2t": "jpn"
                }, {
                    "iso639-1": "jv",
                    "iso639-2t": "jav"
                }, {
                    "iso639-1": "kl",
                    "iso639-2t": "kal"
                }, {
                    "iso639-1": "kn",
                    "iso639-2t": "kan"
                }, {
                    "iso639-1": "kr",
                    "iso639-2t": "kau"
                }, {
                    "iso639-1": "ks",
                    "iso639-2t": "kas"
                }, {
                    "iso639-1": "kk",
                    "iso639-2t": "kaz"
                }, {
                    "iso639-1": "km",
                    "iso639-2t": "khm"
                }, {
                    "iso639-1": "ki",
                    "iso639-2t": "kik"
                }, {
                    "iso639-1": "rw",
                    "iso639-2t": "kin"
                }, {
                    "iso639-1": "ky",
                    "iso639-2t": "kir"
                }, {
                    "iso639-1": "kv",
                    "iso639-2t": "kom"
                }, {
                    "iso639-1": "kg",
                    "iso639-2t": "kon"
                }, {
                    "iso639-1": "ko",
                    "iso639-2t": "kor"
                }, {
                    "iso639-1": "ku",
                    "iso639-2t": "kur"
                }, {
                    "iso639-1": "kj",
                    "iso639-2t": "kua"
                }, {
                    "iso639-1": "la",
                    "iso639-2t": "lat"
                }, {
                    "iso639-1": "lb",
                    "iso639-2t": "ltz"
                }, {
                    "iso639-1": "lg",
                    "iso639-2t": "lug"
                }, {
                    "iso639-1": "li",
                    "iso639-2t": "lim"
                }, {
                    "iso639-1": "ln",
                    "iso639-2t": "lin"
                }, {
                    "iso639-1": "lo",
                    "iso639-2t": "lao"
                }, {
                    "iso639-1": "lt",
                    "iso639-2t": "lit"
                }, {
                    "iso639-1": "lu",
                    "iso639-2t": "lub"
                }, {
                    "iso639-1": "lv",
                    "iso639-2t": "lav"
                }, {
                    "iso639-1": "gv",
                    "iso639-2t": "glv"
                }, {
                    "iso639-1": "mk",
                    "iso639-2t": "mkd"
                }, {
                    "iso639-1": "mg",
                    "iso639-2t": "mlg"
                }, {
                    "iso639-1": "ms",
                    "iso639-2t": "msa"
                }, {
                    "iso639-1": "ml",
                    "iso639-2t": "mal"
                }, {
                    "iso639-1": "mt",
                    "iso639-2t": "mlt"
                }, {
                    "iso639-1": "mi",
                    "iso639-2t": "mri"
                }, {
                    "iso639-1": "mr",
                    "iso639-2t": "mar"
                }, {
                    "iso639-1": "mh",
                    "iso639-2t": "mah"
                }, {
                    "iso639-1": "mn",
                    "iso639-2t": "mon"
                }, {
                    "iso639-1": "na",
                    "iso639-2t": "nau"
                }, {
                    "iso639-1": "nv",
                    "iso639-2t": "nav"
                }, {
                    "iso639-1": "nd",
                    "iso639-2t": "nde"
                }, {
                    "iso639-1": "ne",
                    "iso639-2t": "nep"
                }, {
                    "iso639-1": "ng",
                    "iso639-2t": "ndo"
                }, {
                    "iso639-1": "nb",
                    "iso639-2t": "nob"
                }, {
                    "iso639-1": "nn",
                    "iso639-2t": "nno"
                }, {
                    "iso639-1": "no",
                    "iso639-2t": "nor"
                }, {
                    "iso639-1": "ii",
                    "iso639-2t": "iii"
                }, {
                    "iso639-1": "nr",
                    "iso639-2t": "nbl"
                }, {
                    "iso639-1": "oc",
                    "iso639-2t": "oci"
                }, {
                    "iso639-1": "oj",
                    "iso639-2t": "oji"
                }, {
                    "iso639-1": "cu",
                    "iso639-2t": "chu"
                }, {
                    "iso639-1": "om",
                    "iso639-2t": "orm"
                }, {
                    "iso639-1": "or",
                    "iso639-2t": "ori"
                }, {
                    "iso639-1": "os",
                    "iso639-2t": "oss"
                }, {
                    "iso639-1": "pa",
                    "iso639-2t": "pan"
                }, {
                    "iso639-1": "pi",
                    "iso639-2t": "pli"
                }, {
                    "iso639-1": "fa",
                    "iso639-2t": "fas"
                }, {
                    "iso639-1": "pl",
                    "iso639-2t": "pol"
                }, {
                    "iso639-1": "ps",
                    "iso639-2t": "pus"
                }, {
                    "iso639-1": "pt",
                    "iso639-2t": "por"
                }, {
                    "iso639-1": "qu",
                    "iso639-2t": "que"
                }, {
                    "iso639-1": "rm",
                    "iso639-2t": "roh"
                }, {
                    "iso639-1": "rn",
                    "iso639-2t": "run"
                }, {
                    "iso639-1": "ro",
                    "iso639-2t": "ron"
                }, {
                    "iso639-1": "ru",
                    "iso639-2t": "rus"
                }, {
                    "iso639-1": "sa",
                    "iso639-2t": "san"
                }, {
                    "iso639-1": "sc",
                    "iso639-2t": "srd"
                }, {
                    "iso639-1": "sd",
                    "iso639-2t": "snd"
                }, {
                    "iso639-1": "se",
                    "iso639-2t": "sme"
                }, {
                    "iso639-1": "sm",
                    "iso639-2t": "smo"
                }, {
                    "iso639-1": "sg",
                    "iso639-2t": "sag"
                }, {
                    "iso639-1": "sr",
                    "iso639-2t": "srp"
                }, {
                    "iso639-1": "gd",
                    "iso639-2t": "gla"
                }, {
                    "iso639-1": "sn",
                    "iso639-2t": "sna"
                }, {
                    "iso639-1": "si",
                    "iso639-2t": "sin"
                }, {
                    "iso639-1": "sk",
                    "iso639-2t": "slk"
                }, {
                    "iso639-1": "sl",
                    "iso639-2t": "slv"
                }, {
                    "iso639-1": "so",
                    "iso639-2t": "som"
                }, {
                    "iso639-1": "st",
                    "iso639-2t": "sot"
                }, {
                    "iso639-1": "es",
                    "iso639-2t": "spa"
                }, {
                    "iso639-1": "su",
                    "iso639-2t": "sun"
                }, {
                    "iso639-1": "sw",
                    "iso639-2t": "swa"
                }, {
                    "iso639-1": "ss",
                    "iso639-2t": "ssw"
                }, {
                    "iso639-1": "sv",
                    "iso639-2t": "swe"
                }, {
                    "iso639-1": "ta",
                    "iso639-2t": "tam"
                }, {
                    "iso639-1": "te",
                    "iso639-2t": "tel"
                }, {
                    "iso639-1": "tg",
                    "iso639-2t": "tgk"
                }, {
                    "iso639-1": "th",
                    "iso639-2t": "tha"
                }, {
                    "iso639-1": "ti",
                    "iso639-2t": "tir"
                }, {
                    "iso639-1": "bo",
                    "iso639-2t": "bod"
                }, {
                    "iso639-1": "tk",
                    "iso639-2t": "tuk"
                }, {
                    "iso639-1": "tl",
                    "iso639-2t": "tgl"
                }, {
                    "iso639-1": "tn",
                    "iso639-2t": "tsn"
                }, {
                    "iso639-1": "to",
                    "iso639-2t": "ton"
                }, {
                    "iso639-1": "tr",
                    "iso639-2t": "tur"
                }, {
                    "iso639-1": "ts",
                    "iso639-2t": "tso"
                }, {
                    "iso639-1": "tt",
                    "iso639-2t": "tat"
                }, {
                    "iso639-1": "tw",
                    "iso639-2t": "twi"
                }, {
                    "iso639-1": "ty",
                    "iso639-2t": "tah"
                }, {
                    "iso639-1": "ug",
                    "iso639-2t": "uig"
                }, {
                    "iso639-1": "uk",
                    "iso639-2t": "ukr"
                }, {
                    "iso639-1": "ur",
                    "iso639-2t": "urd"
                }, {
                    "iso639-1": "uz",
                    "iso639-2t": "uzb"
                }, {
                    "iso639-1": "ve",
                    "iso639-2t": "ven"
                }, {
                    "iso639-1": "vi",
                    "iso639-2t": "vie"
                }, {
                    "iso639-1": "vo",
                    "iso639-2t": "vol"
                }, {
                    "iso639-1": "wa",
                    "iso639-2t": "wln"
                }, {
                    "iso639-1": "cy",
                    "iso639-2t": "cym"
                }, {
                    "iso639-1": "wo",
                    "iso639-2t": "wol"
                }, {
                    "iso639-1": "fy",
                    "iso639-2t": "fry"
                }, {
                    "iso639-1": "xh",
                    "iso639-2t": "xho"
                }, {
                    "iso639-1": "yi",
                    "iso639-2t": "yid"
                }, {
                    "iso639-1": "yo",
                    "iso639-2t": "yor"
                }, {
                    "iso639-1": "za",
                    "iso639-2t": "zha"
                }, {
                    "iso639-1": "zu",
                    "iso639-2t": "zul"
                }], e)
            }();
        t.exports = o
    }, {
        281: 281
    }],
    183: [function(e, t, i) {
        "use strict";
        var n, r = e(80),
            o = r.MarketplaceID,
            s = e(93),
            a = e(281),
            l = e(182),
            c = a.of("Locales"),
            u = function() {
                function e() {}
                return (e.getStandardizedLocale = function(t) {
                    if (s.isNullOrUndefined(t)) return null;
                    var i = e[t.replace("-", "_").toUpperCase()];
                    return s.withDefault(i, null)
                }, e.getLocaleFromMarketplaceId = function(t) {
                    c(t, "marketplaceId").exists();
                    var i = e.MARKETPLACE_LOCALE_MAP[t];
                    return s.withDefault(i, null)
                }, e.getClosestLocale = function(e, t) {
                    c(e, "requestedLocale").exists(), c(t, "availableLocales").exists(), c(t, "availableLocales").is.not.empty();
                    for (var i = l.getLanguageCodeFromLocale(e), n = null, r = 0; r < t.length; ++r) {
                        var o = t[r],
                            s = l.getLanguageCodeFromLocale(o);
                        if (e === o) return o;
                        null === n && (n = i === s ? o : null)
                    }
                    return n
                }, e.AR_AE = "ar-AE", e.CS_CZ = "cs-CZ", e.DA_DK = "da-DK", e.DE_DE = "de-DE", e.EN_GB = "en-GB", e.EN_US = "en-US", e.ES_ES = "es-ES", e.ES_MX = "es-MX", e.ES_US = "es-US", e.FI_FI = "fi-FI", e.FIL_PH = "fil-PH", e.FR_FR = "fr-FR", e.HE_IL = "he-IL", e.HI_IN = "hi-IN", e.ID_ID = "id-ID", e.IT_IT = "it-IT", e.JA_JP = "ja-JP", e.KO_KR = "ko-KR", e.MS_MY = "ms-MY", e.NB_NO = "nb-NO", e.NL_NL = "nl-NL", e.PL_PL = "pl-PL", e.PT_BR = "pt-BR", e.PT_PT = "pt-PT", e.RU_RU = "ru-RU", e.SV_SE = "sv-SE", e.TA_IN = "ta-IN", e.TE_IN = "te-IN", e.TH_TH = "th-TH", e.TR_TR = "tr-TR", e.ZH_CN = "zh-CN", e.ZH_TW = "zh-TW", e.AvailableLocales = [e.AR_AE, e.CS_CZ, e.DA_DK, e.DE_DE, e.EN_GB, e.EN_US, e.ES_ES, e.ES_US, e.FI_FI, e.FIL_PH, e.FR_FR, e.HE_IL, e.HI_IN, e.ID_ID, e.IT_IT, e.JA_JP, e.KO_KR, e.MS_MY, e.NB_NO, e.NL_NL, e.PL_PL, e.PT_BR, e.PT_PT, e.RU_RU, e.SV_SE, e.TA_IN, e.TE_IN, e.TH_TH, e.TR_TR, e.ZH_CN, e.ZH_TW], e.MARKETPLACE_LOCALE_MAP = (n = {}, n[o.US] = e.EN_US, n[o.UK] = e.EN_GB, n[o.DE] = e.DE_DE, n[o.JP] = e.JA_JP, n), e)
            }();
        t.exports = u
    }, {
        182: 182,
        281: 281,
        80: 80,
        93: 93
    }],
    184: [function(e, t, i) {
        "use strict";
        var n = e(250),
            r = e(284),
            o = e(93),
            s = n.ScriptType,
            a = n.TransparentCachingScriptLoader,
            l = e(294),
            c = r.get("LocalizationAcquirer"),
            u = function() {
                function e(e) {
                    void 0 === e && (e = new a), this.loader = e
                }
                return (e.prototype.initialize = function(e, t, i, n) {
                    void 0 === n && (n = !0), this.version = e, this.name = t, this.sources = i, this.cacheEnabled = n, this.cacheEnabled || c.info("Localization caching disabled for " + this.name + ".");
                }, e.prototype.acquire = function(t, i, n) {
                    if (l.isEmpty(this.sources)) return void n(new Error(e.NO_SOURCES_ERROR));
                    if (o.isNullOrUndefined(this.version)) return void n(new Error(e.NO_VERSION_ERROR));
                    var r = this.sources[t];
                    if (o.isNullOrUndefined(r)) return void n(new Error("URL not found for " + t));
                    var a = {
                        url: r,
                        name: this.name + "-" + t,
                        type: s.JSON,
                        version: this.version
                    };
                    this.loader.get(a, function(t) {
                        var r = t.payload;
                        return l.isEmpty(r) ? void n(new Error(e.EMPTY_LOCALIZATION_ERROR)) : void i(r, t.totalMs, t.cached)
                    }, function(e) {
                        c.warn("A non-fatal error occurred while trying to download " + t + " at " + r, e)
                    }, n, this.cacheEnabled)
                }, e.EMPTY_LOCALIZATION_ERROR = "Acquired localization object is empty.", e.NO_SOURCES_ERROR = "No sources are defined. LocalizationAcquirer may not have been initialized.", e.NO_VERSION_ERROR = "No version is defined. LocalizationAcquirer may not have been initialized.", e)
            }();
        t.exports = u
    }, {
        250: 250,
        284: 284,
        294: 294,
        93: 93
    }],
    185: [function(e, t, i) {
        "use strict";
        var n = e(82),
            r = n.BasicEventEmitter,
            o = e(181),
            s = e(184),
            a = e(284),
            l = e(93),
            c = e(294),
            u = a.get("LocalizationStore"),
            p = function() {
                function e(e, t, i, n, o, a, l) {
                    void 0 === o && (o = !0), void 0 === a && (a = new s), void 0 === l && (l = new r);
                    var p = this;
                    this.loadedLanguages = {}, this.localizationAcquirer = a, this.eventEmitter = l;
                    var h = {};
                    c.chain(i).keys().forEach(function(e) {
                        c.isString(i[e]) ? h[e] = i[e] : p.loadedLanguages[e] = i[e]
                    }), this.localizationAcquirer.initialize(e, t, h, o), u.info("Pre-loaded languages: " + c.keys(this.loadedLanguages).join(", ")), u.info("Not loaded languages: " + c.keys(h).join(", ")), c.isEmpty(n) || (c.contains(c.keys(this.loadedLanguages), n) ? (this.defaultLanguageCode = n, u.info("Default language code set to " + this.defaultLanguageCode + ".")) : u.warn("Default language code set to " + n + " but will be ignored because its localization object was not provided."))
                }
                return (e.prototype.setLanguage = function(e, t, i) {
                    var n = this;
                    return l.exists(this.loadedLanguages[e]) ? void this.onAcquireSuccess(e, this.loadedLanguages[e], 0, !0, t) : void this.localizationAcquirer.acquire(e, function(i, r, o) {
                        n.onAcquireSuccess(e, i, r, o, t)
                    }, function(r) {
                        n.onAcquireError(e, r, t, i)
                    })
                }, e.prototype.getLocalization = function() {
                    return this.loadedLanguages[this.currentLanguageCode]
                }, e.prototype.getCurrentLanguage = function() {
                    return this.currentLanguageCode
                }, e.prototype.onLanguageChanged = function(e) {
                    this.eventEmitter.on(o, e)
                }, e.prototype.offLanguageChanged = function(e) {
                    this.eventEmitter.off(o, e)
                }, e.prototype.onAcquireSuccess = function(e, t, i, n, r) {
                    u.info("Acquired localization for " + e + " in " + i + "ms" + (n ? " from cache" : "") + "."), this.currentLanguageCode = e, this.loadedLanguages[this.currentLanguageCode] = t, this.eventEmitter.emit(o, new o(this.currentLanguageCode)), l.exists(r) && r(t, i, n)
                }, e.prototype.onAcquireError = function(e, t, i, n) {
                    u.error("An error occurred while trying to acquire " + e + ".", t), l.exists(n) && n(t), l.isNullOrUndefined(this.currentLanguageCode) && !c.isEmpty(this.defaultLanguageCode) && (u.info("Falling back to " + this.defaultLanguageCode + "."), this.setLanguage(this.defaultLanguageCode, i, n))
                }, e)
            }();
        t.exports = p
    }, {
        181: 181,
        184: 184,
        284: 284,
        294: 294,
        82: 82,
        93: 93
    }],
    186: [function(e, t, i) {
        "use strict";
        var n = e(181),
            r = e(182),
            o = e(183),
            s = e(185),
            a = {
                LanguageChangedEvent: n,
                LanguageCodes: r,
                Locales: o,
                LocalizationStore: s
            };
        t.exports = a
    }, {
        181: 181,
        182: 182,
        183: 183,
        185: 185
    }],
    187: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(284),
            o = e(188),
            s = n.of("Metrics"),
            a = r.get("Metrics"),
            l = function() {
                function e() {}
                return (e.setHandler = function(t) {
                    e.handler = s(t, "handler").exists()
                }, e.setFlusher = function(t) {
                    e.flusher = s(t, "flusher").exists()
                }, e.putMetric = function(t, i, n) {
                    s(t, "name").exists();
                    try {
                        e.handler(t, i, n)
                    } catch (e) {
                        a.error("An error occurred while publishing metric " + t + ": " + e.name + ": " + e.message, e)
                    }
                }, e.flush = function() {
                    try {
                        e.flusher()
                    } catch (e) {
                        a.error("An error occurred while flushing metrics", e)
                    }
                }, e.startTimer = function(t) {
                    return new o(e, t)
                }, e.MILLISECONDS = "ms", e.handler = function() {}, e.flusher = function() {}, e)
            }();
        t.exports = l
    }, {
        188: 188,
        281: 281,
        284: 284
    }],
    188: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(284),
            o = n.of("Timer"),
            s = r.get("Timer"),
            a = function() {
                function e(t, i) {
                    this.metrics = o(t, "metrics").exists(), this.metricName = o(i, "metricName").exists(), this.startTime = e.now(), this.elapsedTime = null
                }
                return (e.prototype.end = function() {
                    if (null === this.elapsedTime) {
                        this.elapsedTime = e.now() - this.startTime;
                        try {
                            this.metrics.putMetric(this.metricName, this.elapsedTime, e.UNIT)
                        } catch (e) {
                            s.error("An error occurred while stopping the timer for metric " + this.metricName + ": " + e.name + ": " + e.message, e)
                        }
                        s.info(this.metricName + " was " + this.elapsedTime + " " + e.UNIT)
                    }
                    return this.elapsedTime
                }, e.UNIT = "ms", e.now = "undefined" != typeof Date.now ? Date.now : function() {
                    return (new Date).getTime()
                }, e)
            }();
        t.exports = a
    }, {
        281: 281,
        284: 284
    }],
    189: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(281),
            o = e(294),
            s = r.of("NotificationManager"),
            a = function() {
                function e() {
                    this.initialized = !1
                }
                return (e.prototype.initialize = function(e, t, i) {
                    void 0 === i && (i = 1), s(!this.initialized, "!this.initialized").is["true"](), this.onShowNotification = s(e, "onShowNotification").exists(), this.onCloseNotification = s(t, "onCloseNotification").exists(), this.currentNotifications = [], this.maxNotificationsAtOnce = i, this.queuedNotifications = [], this.initialized = !0
                }, e.prototype.addNotification = function(e, t) {
                    void 0 === t && (t = !0), s(this.initialized, "this.initialized").is["true"](), s(e, "notification").exists(), this.queuedNotifications.push(e), t && this.runNotification()
                }, e.prototype.hasQueuedNotifications = function() {
                    return 0 !== this.queuedNotifications.length
                }, e.prototype.showQueuedNotifications = function() {
                    this.runNotification()
                }, e.prototype.cancelNotification = function(e) {
                    var t = this;
                    s(this.initialized, "this.initialized").is["true"](), s(e, "notification").exists();
                    var i = o.some(this.currentNotifications, function(i) {
                        if (i.notification === e) return (t.closeNotification(e), !0)
                    });
                    i || o.some(this.queuedNotifications, function(i, n) {
                        if (i === e) return (t.queuedNotifications.splice(n, 1), !0)
                    })
                }, e.prototype.closeNotification = function(e) {
                    o.some(this.currentNotifications, function(t) {
                        if (t.notification === e && !n.isNullOrUndefined(t.timeoutID)) return (clearTimeout(t.timeoutID), !0)
                    }), this.runNextNotification(e)
                }, e.prototype.runNotification = function() {
                    var e = this;
                    if (this.currentNotifications.length !== this.maxNotificationsAtOnce) {
                        var t = {
                            notification: this.queuedNotifications.shift()
                        };
                        n.isNullOrUndefined(t.notification) || (this.currentNotifications.push(t), this.onShowNotification(t.notification), n.exists(t.notification.getDisplayTimeSeconds()) && (t.timeoutID = setTimeout(function() {
                            e.runNextNotification(t.notification)
                        }, 1e3 * t.notification.getDisplayTimeSeconds())))
                    }
                }, e.prototype.runNextNotification = function(e) {
                    var t = this;
                    this.onCloseNotification(e), o.some(this.currentNotifications, function(i, n) {
                        if (i.notification === e) return (t.currentNotifications.splice(n, 1), !0)
                    }), this.queuedNotifications.length > 0 && this.runNotification()
                }, e)
            }();
        t.exports = a
    }, {
        281: 281,
        294: 294,
        93: 93
    }],
    190: [function(e, t, i) {
        "use strict";
        var n = e(189),
            r = {
                NotificationManager: n
            };
        t.exports = r
    }, {
        189: 189
    }],
    191: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(294),
            o = function() {
                function e() {}
                return (e.prototype.convert = function(e) {
                    for (var t = e.avCdnUrlSets, i = {}, o = r.isEmpty(t) ? null : t[0].urlSetId, s = r.isEmpty(t) ? null : t[0].avUrlInfoList[0].cacheKey, a = 0; a < t.length; a++) {
                        var l = t[a],
                            c = l.urlSetId,
                            u = n.withDefault(t[a + 1], t[0]).urlSetId;
                        i[c] = {
                            urlSetId: c,
                            urls: {
                                manifest: {
                                    url: l.avUrlInfoList[0].url,
                                    audioFormat: null,
                                    videoQuality: l.avUrlInfoList[0].videoQuality,
                                    audioTrackId: l.audioTrackId,
                                    cdn: l.cdn,
                                    origin: l.avUrlInfoList[0].origin,
                                    drm: l.drm,
                                    dynamicRange: l.hdrFormat,
                                    compressionStandard: l.encode,
                                    streamingTechnology: l.streamingTechnology
                                },
                                adsTrackingEndpoint: l.avUrlInfoList[0].adsTrackingEndpoint
                            },
                            failover: {
                                cdn: {
                                    urlSetId: u,
                                    mode: "seamless"
                                },
                                "default": {
                                    urlSetId: u,
                                    mode: "seamless"
                                }
                            }
                        }
                    }
                    return {
                        audioTracks: e.audioTrackMetadata,
                        cacheKey: s,
                        defaultAudioTrackId: e.defaultAudioTrackId,
                        defaultUrlSetId: o,
                        urlSets: i
                    }
                }, e)
            }();
        t.exports = o
    }, {
        294: 294,
        93: 93
    }],
    192: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.AD_SUPPORTED_ENTITLEMENT_TYPE_ERROR = "AdSupportedEntitlementTypeError", e.MISSING_PLAYBACK_RESOURCE_ERROR = "MissingPlaybackResourceError", e.NO_URL_SET_ERROR = "NoUrlSetError", e.PAYMENT_FAILURE_ERROR = "PaymentFailureError", e.UNHANDLED_PLAYBACK_RESOURCE_ERROR = "UnhandledPlaybackResourceError", e)
        }();
        t.exports = n
    }, {}],
    193: [function(e, t, i) {
        "use strict";
        Object.defineProperty(i, "__esModule", {
            value: !0
        });
        var n;
        ! function(e) {
            e[e.asin = 0] = "asin", e[e.audioTrackId = 1] = "audioTrackId", e[e.clientId = 2] = "clientId", e[e.consumptionType = 3] = "consumptionType", e[e.customerID = 4] = "customerID", e[e.desiredAudioQuality = 5] = "desiredAudioQuality", e[e.desiredResources = 6] = "desiredResources", e[e.deviceAudioQualityOverride = 7] = "deviceAudioQualityOverride", e[e.deviceBitrateAdaptationsOverride = 8] = "deviceBitrateAdaptationsOverride", e[e.deviceDrmOverride = 9] = "deviceDrmOverride", e[e.deviceID = 10] = "deviceID", e[e.deviceProtocolOverride = 11] = "deviceProtocolOverride", e[e.deviceStreamingTechnologyOverride = 12] = "deviceStreamingTechnologyOverride", e[e.deviceTypeID = 13] = "deviceTypeID", e[e.deviceVideoCodecOverride = 14] = "deviceVideoCodecOverride", e[e.deviceVideoQualityOverride = 15] = "deviceVideoQualityOverride", e[e.firmware = 16] = "firmware", e[e.gascEnabled = 17] = "gascEnabled", e[e.languageFeature = 18] = "languageFeature", e[e.linearRestrictionEnd = 19] = "linearRestrictionEnd", e[e.linearRestrictionStart = 20] = "linearRestrictionStart", e[e.liveManifestType = 21] = "liveManifestType", e[e.liveScheduleConfigToken = 22] = "liveScheduleConfigToken", e[e.marketplaceID = 23] = "marketplaceID", e[e.operatingSystemName = 24] = "operatingSystemName", e[e.operatingSystemVersion = 25] = "operatingSystemVersion", e[e.playbackSettingsFormatVersion = 26] = "playbackSettingsFormatVersion", e[e.playerAttributes = 27] = "playerAttributes", e[e.playerType = 28] = "playerType", e[e.resourceUsage = 29] = "resourceUsage", e[e.siteSectionId = 30] = "siteSectionId", e[e.subtitleFormat = 31] = "subtitleFormat", e[e.supportedDRMKeyScheme = 32] = "supportedDRMKeyScheme", e[e.titleDecorationScheme = 33] = "titleDecorationScheme", e[e.token = 34] = "token", e[e.userWatchSessionId = 35] = "userWatchSessionId", e[e.uxLocale = 36] = "uxLocale", e[e.videoMaterialType = 37] = "videoMaterialType", e[e.videoMinBitrate = 38] = "videoMinBitrate", e[e.xrayDeviceClass = 39] = "xrayDeviceClass", e[e.xrayPlaybackMode = 40] = "xrayPlaybackMode", e[e.xrayToken = 41] = "xrayToken"
        }(n = i.QueryStringParameter || (i.QueryStringParameter = {}));
        var r;
        ! function(e) {
            e[e.playReadyChallenge = 0] = "playReadyChallenge", e[e.playReadyKeyId = 1] = "playReadyKeyId", e[e.fairPlayChallenge = 2] = "fairPlayChallenge", e[e.fairPlayKeyId = 3] = "fairPlayKeyId", e[e.widevine2Challenge = 4] = "widevine2Challenge"
        }(r = i.PostBodyParameter || (i.PostBodyParameter = {}));
        var o;
        ! function(e) {
            e[e.AudioVideoUrls = 0] = "AudioVideoUrls", e[e.CatalogMetadata = 1] = "CatalogMetadata", e[e.CuepointPlaylist = 2] = "CuepointPlaylist", e[e.DevicePlaybackConfiguration = 3] = "DevicePlaybackConfiguration", e[e.FairPlayApplicationCertificate = 4] = "FairPlayApplicationCertificate", e[e.FairPlayLicense = 5] = "FairPlayLicense", e[e.ForcedNarratives = 6] = "ForcedNarratives", e[e.LinearRestrictions = 7] = "LinearRestrictions", e[e.LiveSchedule = 8] = "LiveSchedule", e[e.PlaybackSettings = 9] = "PlaybackSettings", e[e.PlaybackUrls = 10] = "PlaybackUrls", e[e.PlayReadyLicense = 11] = "PlayReadyLicense", e[e.SubtitlePresets = 12] = "SubtitlePresets", e[e.SubtitleUrls = 13] = "SubtitleUrls", e[e.TransitionTimecodes = 14] = "TransitionTimecodes", e[e.TrickplayUrls = 15] = "TrickplayUrls", e[e.Widevine2License = 16] = "Widevine2License", e[e.WhisperSyncBookmark = 17] = "WhisperSyncBookmark", e[e.XRayMetadata = 18] = "XRayMetadata"
        }(o = i.DesiredResources || (i.DesiredResources = {}));
        var s;
        ! function(e) {
            e[e.ImmediateConsumption = 0] = "ImmediateConsumption", e[e.CacheResources = 1] = "CacheResources", e[e.OptimisticFulfillment = 2] = "OptimisticFulfillment"
        }(s = i.ResourceUsage || (i.ResourceUsage = {}));
        var a;
        ! function(e) {
            e[e.Feature = 0] = "Feature", e[e.Trailer = 1] = "Trailer", e[e.LiveStreaming = 2] = "LiveStreaming", e[e.External = 3] = "External", e[e.VideoAds = 4] = "VideoAds"
        }(a = i.VideoMaterialType || (i.VideoMaterialType = {}));
        var l;
        ! function(e) {
            e[e.TTML = 0] = "TTML", e[e.TTMLv2 = 1] = "TTMLv2"
        }(l = i.SubtitleFormat || (i.SubtitleFormat = {}));
        var c;
        ! function(e) {
            e[e.MLFv2 = 0] = "MLFv2"
        }(c = i.LanguageFeature || (i.LanguageFeature = {}))
    }, {}],
    194: [function(e, t, i) {
        "use strict";
        var n = e(281),
            r = e(93),
            o = e(196),
            s = n.of("PlaybackResourceClient"),
            a = function() {
                function e(t, i, n, a, l, c, u) {
                    void 0 === u && (u = o), this.deviceId = s(t, "deviceId").exists(), this.deviceTypeId = s(i, "deviceTypeId").exists(), this.marketplaceId = s(n, "marketplaceId").exists(), s(a, "partnerServiceUrl").exists(), r.exists(l) && (s(l.customerId).exists(), this.authContext = l), r.exists(c) && (this.clientId = c), this.url = a.replace(/\/$/, "") + e.SERVICE_ENDPOINT, this.playbackResourceRequestType = u
                }
                return (e.prototype.createRequest = function(e, t, i, n, r, o) {
                    s(e, "asin").exists(), s(t, "consumptionType").exists(), s(i, "desiredResources").exists(), s(i.length > 0, "desiredResources.length > 0").is["true"](), s(n, "resourceUsage").exists(), s(r, "videoMaterialType").exists(), s(o, "userWatchSessionId").exists();
                    var a = {
                        asin: e,
                        consumptionType: t,
                        desiredResources: i,
                        deviceId: this.deviceId,
                        deviceTypeId: this.deviceTypeId,
                        marketplaceId: this.marketplaceId,
                        resourceUsage: n,
                        userWatchSessionId: o,
                        videoMaterialType: r
                    };
                    return new this.playbackResourceRequestType(a, this.url, this.authContext, this.clientId)
                }, e.SERVICE_ENDPOINT = "/cdp/catalog/GetPlaybackResources", e)
            }();
        t.exports = a
    }, {
        196: 196,
        281: 281,
        93: 93
    }],
    195: [function(e, t, i) {
        "use strict";
        var n = e(159),
            r = e(93),
            o = e(200),
            s = e(294),
            a = function() {
                function e() {}
                return (e.createError = function(t) {
                    var i = o.PRSError,
                        s = e.ERROR_MAPPING[t.type];
                    r.isNullOrUndefined(s) || (i = s);
                    var a = e.ERROR_CODE_MAPPING[t.errorCode];
                    r.isNullOrUndefined(a) || (i = a);
                    var l = n.newError(o[i], t.message);
                    return (l.errorCode = t.errorCode, r.exists(t.downstreamReason) && (l.downstreamReason = t.downstreamReason), l)
                }, e.createPRSError = function(e, t) {
                    return n.newError(o[o.PRSError], e, t)
                }, e.convertResourceExceptions = function(t) {
                    return r.isNullOrUndefined(t) ? t : s.object(s.map(t, function(t, i) {
                        return [i, e.createError(t)]
                    }))
                }, e.convertRightsException = function(t) {
                    if (r.isNullOrUndefined(t)) return t;
                    var i = r.nestedValue(t, "selectedEntitlement.rightsException");
                    return (null !== i && (t.selectedEntitlement.rightsException = e.createError(i)), t)
                }, e.ERROR_MAPPING = {
                    PRSConcurrencyException: o.PRSConcurrencyError,
                    PRSGeoIpException: o.PRSGeoIpError,
                    PRSOwnershipException: o.PRSOwnershipError,
                    PRSException: o.PRSError,
                    PRSDependencyException: o.PRSError,
                    PRSInvalidRequestException: o.PRSError,
                    PRSRightsAndPoliciesException: o.PRSError,
                    PRSWidevine2LicenseDeniedException: o.PRSError
                }, e.ERROR_CODE_MAPPING = {
                    "PRS.NoRights.AnonymizerIP": o.PRSHttpProxyError
                }, e)
            }();
        t.exports = a
    }, {
        159: 159,
        200: 200,
        294: 294,
        93: 93
    }],
    196: [function(e, t, i) {
        "use strict";
        var n = e(58),
            r = n.TokenType,
            o = e(178),
            s = o.HttpRequest,
            a = e(252),
            l = a.ExponentialBackoffRetry,
            c = e(159),
            u = e(203),
            p = u.AudioQuality,
            h = u.BitrateAdaptation,
            f = u.ConsumptionType,
            d = u.ContentProtection,
            y = u.DrmKeyScheme,
            g = u.Protocol,
            m = u.StreamingTechnology,
            v = u.VideoCodec,
            E = u.VideoQuality,
            _ = e(272),
            S = e(80),
            T = S.MarketplaceID,
            b = e(93),
            I = e(281),
            w = e(195),
            R = e(193),
            C = R.DesiredResources,
            P = R.LanguageFeature,
            A = R.PostBodyParameter,
            O = R.QueryStringParameter,
            N = R.ResourceUsage,
            x = R.SubtitleFormat,
            M = R.VideoMaterialType,
            L = e(294),
            k = I.of("PlaybackResourceRequest"),
            D = "html5",
            U = function() {
                function e(e, t, i, n, r, o, a) {
                    void 0 === r && (r = s), void 0 === o && (o = l), void 0 === a && (a = _.getOperatingSystem()), this.playerAttributes = {}, this.retry = null, this.request = null, k(e, "options").exists(), k(t, "url").exists(), b.exists(i) && (k(i.customerId, "customerId").exists(), this.authContext = i), this.httpRequest = r, this.retryType = o, this.url = t, this.formFields = null, this.queryParams = {}, this.addQueryParam(O.asin, e.asin), this.addQueryParam(O.consumptionType, f[e.consumptionType]), this.addQueryParam(O.desiredResources, e.desiredResources.map(function(e) {
                        return C[e]
                    }).join()), this.addQueryParam(O.deviceID, e.deviceId), this.addQueryParam(O.deviceTypeID, e.deviceTypeId), this.addQueryParam(O.firmware, "1"), this.addQueryParam(O.gascEnabled, !T.isClassic(e.marketplaceId)), this.addQueryParam(O.marketplaceID, e.marketplaceId), this.addQueryParam(O.resourceUsage, N[e.resourceUsage]), this.addQueryParam(O.userWatchSessionId, e.userWatchSessionId), this.addQueryParam(O.videoMaterialType, M[e.videoMaterialType]), this.addQueryParam(O.playerType, D), b.exists(n) && this.addQueryParam(O.clientId, n);
                    var c = a.getFamilyName();
                    null !== c && this.addQueryParam(O.operatingSystemName, c);
                    var u = a.getKernelVersion();
                    null !== u && this.addQueryParam(O.operatingSystemVersion, u), this.resourceUsage = e.resourceUsage
                }
                return (e.prototype.withAudioTrackId = function(e) {
                    return (k(e, "trackId").exists(), this.addQueryParam(O.audioTrackId, e), this)
                }, e.prototype.withDesiredAudioQuality = function(e) {
                    return (k(e, "audioQuality").exists(), this.addQueryParam(O.desiredAudioQuality, p[e]), this)
                }, e.prototype.withDeviceAudioQualityOverride = function(e) {
                    return (k(e, "deviceAudioQuality").exists(), this.addQueryParam(O.deviceAudioQualityOverride, p[e]), this)
                }, e.prototype.withDeviceDrmOverride = function(e) {
                    return (k(e, "deviceDrm").exists(), this.addQueryParam(O.deviceDrmOverride, d[e]), this)
                }, e.prototype.withDeviceProtocolOverride = function(e) {
                    return (k(e, "deviceProtocol").exists(), this.addQueryParam(O.deviceProtocolOverride, g[e]), this)
                }, e.prototype.withDeviceStreamingTechnologyOverride = function(e) {
                    return (k(e, "streamingTechnology").exists(), this.addQueryParam(O.deviceStreamingTechnologyOverride, m[e]), this)
                }, e.prototype.withDeviceVideoQualityOverride = function(e) {
                    return (k(e, "deviceVideoQuality").exists(), this.addQueryParam(O.deviceVideoQualityOverride, E[e]), this)
                }, e.prototype.withLiveScheduleConfigToken = function(e) {
                    return (k(e, "token").exists(), this.addQueryParam(O.liveScheduleConfigToken, e), this)
                }, e.prototype.withPlayReadyChallenge = function(e, t) {
                    return (k(e, "challenge").exists(), this.addPostBodyParam(A.playReadyChallenge, e), b.isNullOrUndefined(t) || this.addPostBodyParam(A.playReadyKeyId, t), this)
                }, e.prototype.withFairPlayChallenge = function(e, t) {
                    return (k(e, "challenge").exists(), k(t, "keyId").exists(), this.addPostBodyParam(A.fairPlayChallenge, e), this.addPostBodyParam(A.fairPlayKeyId, t), this)
                }, e.prototype.withWidevine2Challenge = function(e, t) {
                    return (k(e, "challenge").exists(), this.addPostBodyParam(A.widevine2Challenge, e), this.addPostBodyParam(A.widevine2Challenge, e), this)
                }, e.prototype.withTitleDecorationScheme = function(e) {
                    return (k(e, "titleDecorationScheme").exists(), this.addQueryParam(O.titleDecorationScheme, e), this)
                }, e.prototype.withUserWatchSessionId = function(e) {
                    return (k(e, "userWatchSessionId").exists(), this.addQueryParam(O.userWatchSessionId, e), this)
                }, e.prototype.withVideoMinBitrate = function(e) {
                    return (k(e, "bitrate").exists(), this.addQueryParam(O.videoMinBitrate, e), this)
                }, e.prototype.withSubtitleFormat = function(e) {
                    return (k(e, "format").exists(), e !== x.TTML && this.addQueryParam(O.subtitleFormat, x[e]), this)
                }, e.prototype.withLanguageFeature = function(e) {
                    return (k(e, "languageFeature").exists(), this.addQueryParam(O.languageFeature, P[e]), this)
                }, e.prototype.withSiteSectionId = function(e) {
                    return (k(e, "siteSectionId").exists(), this.addQueryParam(O.siteSectionId, e), this)
                }, e.prototype.withPlaybackSettingsFormatVersion = function(e) {
                    return (k(e, "formatVersion").exists(), this.addQueryParam(O.playbackSettingsFormatVersion, e), this)
                }, e.prototype.withSupportedDrmKeyScheme = function(e) {
                    return (k(e, "drmKeyScheme").exists(), this.addQueryParam(O.supportedDRMKeyScheme, y[e]), this)
                }, e.prototype.withUxLocale = function(e) {
                    return (k(e, "uxLocale").exists(), this.addQueryParam(O.uxLocale, e.replace("-", "_")), this)
                }, e.prototype.withXrayParameters = function(e, t, i) {
                    return (k(e, "xrayDeviceClass").exists(), k(t, "xrayPlaybackMode").exists(), k(i, "xrayToken").exists(), this.addQueryParam(O.xrayDeviceClass, e), this.addQueryParam(O.xrayPlaybackMode, t), this.addQueryParam(O.xrayToken, i), this)
                }, e.prototype.withLinearRestrictionsWindow = function(e, t) {
                    return (k(this.hasDesiredResource(C.LinearRestrictions), "request has LinearRestrictions").is["true"](), k(e, "startTimeMins").exists(), k(t, "endTimeMins").exists(), k(e < t, "startTimeMins < endTimeMins").is["true"](), this.addQueryParam(O.linearRestrictionStart, e), this.addQueryParam(O.linearRestrictionEnd, t), this)
                }, e.prototype.withLiveManifestTypes = function(e) {
                    return (k(e, "liveManifestTypes").is.not.empty(), this.addQueryParam(O.liveManifestType, e.join(",")), this)
                }, e.prototype.withPlayerAttributes = function(e, t) {
                    return (k(e, "name").exists(), k(t, "playerAttributesValue").exists(), L.isArray(t) && (t = t.join()), this.playerAttributes[e] = t, this)
                }, e.prototype.send = function(t, i, n) {
                    var r = this;
                    k(t, "onSuccess").exists(), k(i, "onError").exists(), k(this.request, "request").is["null"](), this.retry = (new this.retryType).withMaxAttempts(e.MAX_ATTEMPTS).withInitialIntervalMs(e.BASE_RETRY).withExponentialFactor(2).withRandomizationFactor(e.JITTER).onSuccess(t).onError(i).onAttempt(function(e, t, i, o) {
                        r.sendRequest(e, t, i, o, n)
                    }).execute()
                }, e.prototype.abort = function() {
                    null !== this.retry && (this.retry.abort(), this.retry = null), null !== this.request && (this.request.abort(), this.request = null)
                }, e.prototype.sendRequest = function(t, i, n, o, s) {
                    var a = this,
                        l = this.resourceUsage === N.ImmediateConsumption ? e.PRIORITY_CRITICAL : e.PRIORITY_BACKGROUND;
                    L.isEmpty(this.playerAttributes) || this.addQueryParam(O.playerAttributes, JSON.stringify(this.playerAttributes)), this.request = this.httpRequest.post(this.url).accept("application/json").timeout(e.HTTP_REQUEST_TIMEOUT_MS).onError(function(t, n) {
                        a.request = null;
                        var r = "Error executing playback resource request.",
                            l = w.createPRSError(r, t);
                        b.isNullOrUndefined(n) || !L.contains(e.UNRETRIABLE_HTTP_ERRORS, n.getStatus()) ? (a.provideMetrics(n, s), i(l)) : (b.exists(n) && 403 === n.getStatus() && (l = c.newAuthenticationError(r)), o(l))
                    }).onLoad(function(t) {
                        a.request = null, a.provideMetrics(t, s);
                        var r = t.getText();
                        if (b.isNullOrUndefined(r)) return void i(w.createPRSError("PlaybackResourceResponse is null or undefined."));
                        var l;
                        try {
                            if (l = JSON.parse(r), b.exists(l.error)) {
                                var c = w.createError(l.error);
                                return void(L.contains(e.RETRIABLE_PRS_ERRORS, l.error.type) ? i(c) : o(c))
                            }
                            l.errorsByResource = w.convertResourceExceptions(l.errorsByResource), l.returnedTitleRendition = w.convertRightsException(l.returnedTitleRendition)
                        } catch (e) {
                            return void i(w.createPRSError("Error parsing json response", e))
                        }
                        n(l)
                    }).onAbort(function(e) {
                        a.request = null
                    }).withCredentials(), 0 !== t && this.request.header(e.RETRY_COUNT_HEADER, "" + t).header(e.REQUEST_PRIORITY_HEADER, l), null !== this.formFields && this.request.fields(this.formFields), b.exists(this.authContext) && b.exists(this.authContext.tokenClient) ? this.authContext.tokenClient.getToken(function(e) {
                        e.type === r.OAUTH && a.request.bearerToken(e.token), a.request.query(a.queryParams).send()
                    }, o) : this.request.query(this.queryParams).send()
                }, e.prototype.provideMetrics = function(e, t) {
                    b.isNullOrUndefined(e) || b.isNullOrUndefined(t) || t({
                        downloadMs: e.getDownloadMs(),
                        latencyMs: e.getLatencyMs(),
                        totalMs: e.getTotalMs()
                    })
                }, e.prototype.addQueryParam = function(e, t) {
                    this.queryParams[O[e]] = t
                }, e.prototype.addPostBodyParam = function(e, t) {
                    null === this.formFields && (this.formFields = {}), this.formFields[A[e]] = t
                }, e.prototype.hasDesiredResource = function(e) {
                    var t = b.withDefault(this.queryParams[O[O.desiredResources]], "");
                    return b.exists(t.match("(^|,)" + C[e] + "($|,)"))
                }, e.MAX_ATTEMPTS = 4, e.JITTER = 200, e.BASE_RETRY = 500, e.UNRETRIABLE_HTTP_ERRORS = [400, 401, 403], e.RETRIABLE_PRS_ERRORS = ["PRSDependencyException"], e.RETRY_COUNT_HEADER = "x-retry-count", e.REQUEST_PRIORITY_HEADER = "x-request-priority", e.PRIORITY_CRITICAL = "CRITICAL", e.PRIORITY_BACKGROUND = "BACKGROUND", e.HTTP_REQUEST_TIMEOUT_MS = 6e3, e)
            }();
        t.exports = U
    }, {
        159: 159,
        178: 178,
        193: 193,
        195: 195,
        203: 203,
        252: 252,
        272: 272,
        281: 281,
        294: 294,
        58: 58,
        80: 80,
        93: 93
    }],
    197: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.AD_SUPPORTED = "AD_SUPPORTED", e.PRIME_SUBSCRIPTION = "PRIME_SUBSCRIPTION", e.THIRD_PARTY_SUBSCRIPTION = "THIRD_PARTY_SUBSCRIPTION", e)
        }();
        t.exports = n
    }, {}],
    198: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.NONE = "HDCP_NONE", e.V1 = "HDCP_NONE", e.V2 = "HDCP_NONE", e.V2_1 = "HDCP_NONE", e.V2_2 = "HDCP_NONE", e = "HDCP_NONE")
        }();
        t.exports = n
    }, {}],
    199: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.CONTENT = "CONTENT")
        }();
        t.exports = n
    }, {}],
    200: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.PRSError = 0] = "PRSError", e[e.PRSConcurrencyError = 1] = "PRSConcurrencyError", e[e.PRSGeoIpError = 2] = "PRSGeoIpError", e[e.PRSHttpProxyError = 3] = "PRSHttpProxyError", e[e.PRSOwnershipError = 4] = "PRSOwnershipError"
        }(n || (n = {})), t.exports = n
    }, {}],
    201: [function(e, t, i) {
        "use strict";
        var n = e(159),
            r = e(93),
            o = e(284),
            s = e(192),
            a = e(294),
            l = o.get("PRSResponseValidator"),
            c = function() {
                function e() {}
                return (e.prototype.validateResponse = function(e) {
                    var t = this.validateTitleRendition(e);
                    if (null !== t) return t;
                    var i = this.validateAudioVideoUrls(e);
                    return null !== i ? i : (this.logNonFatalErrorsByResource(e.errorsByResource), null)
                }, e.prototype.validateTitleRendition = function(t) {
                    var i = r.nestedValue(t, "returnedTitleRendition.selectedEntitlement");
                    if (null === i) return n.newError(s.MISSING_PLAYBACK_RESOURCE_ERROR, "selectedEntitlement does not exist.");
                    var o = i.rightsException;
                    return r.isNullOrUndefined(o) ? null : o.downstreamReason === e.PAYMENT_FAILURE_DOWNSTREAM_REASON ? n.newError(s.PAYMENT_FAILURE_ERROR, "Payment failed.", o) : o
                }, e.prototype.validateAudioVideoUrls = function(t) {
                    var i = r.nestedValue(t, "errorsByResource.AudioVideoUrls");
                    return (null === i && (i = r.nestedValue(t, "errorsByResource.PlaybackUrls")), null !== i ? i.message.indexOf("UrlVendingService has no urls") > -1 ? n.newError(s.NO_URL_SET_ERROR, "No url sets returned from PRS.", i) : i.downstreamReason === e.PAYMENT_FAILURE_DOWNSTREAM_REASON ? n.newError(s.PAYMENT_FAILURE_ERROR, "Payment failed.", i) : i : null === r.nestedValue(t, "audioVideoUrls.avCdnUrlSets") && null === r.nestedValue(t, "playbackUrls.urlSets") ? n.newError(s.MISSING_PLAYBACK_RESOURCE_ERROR, "neither avCdnUrlSets nor urlSets exist.") : null)
                }, e.prototype.logNonFatalErrorsByResource = function(e) {
                    r.isNullOrUndefined(e) || a.each(e, function(e, t) {
                        l.error(t + " resource error occurred with name: " + e.name + ", errorCode: " + e.errorCode + ", message: " + e.message + ", downstreamReason: " + e.downstreamReason)
                    })
                }, e.PAYMENT_FAILURE_DOWNSTREAM_REASON = "PAYMENT_FAILURE", e)
            }();
        t.exports = c
    }, {
        159: 159,
        192: 192,
        284: 284,
        294: 294,
        93: 93
    }],
    202: [function(e, t, i) {
        "use strict";
        var n = e(191),
            r = e(197),
            o = e(198),
            s = e(199),
            a = e(193),
            l = e(194),
            c = e(196),
            u = e(192),
            p = e(201),
            h = {
                AudioVideoUrlsConverter: n,
                DesiredResources: a.DesiredResources,
                EntitlementTypes: r,
                HdcpVersions: "HDCP_NONE",
                KeyTypes: "CONTENT",
                LanguageFeature: a.LanguageFeature,
                PlaybackResourceClient: l,
                PlaybackResourceRequest: c,
                PRSErrors: u,
                PRSResponseValidator: p,
                ResourceUsage: a.ResourceUsage,
                SubtitleFormat: a.SubtitleFormat,
                VideoMaterialType: a.VideoMaterialType
            };
        t.exports = h
    }, {
        191: 191,
        192: 192,
        193: 193,
        194: 194,
        196: 196,
        197: 197,
        198: 198,
        199: 199,
        201: 201
    }],
    203: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Streaming = 0] = "Streaming", e[e.Download = 1] = "Download"
        }(n || (n = {}));
        var r;
        ! function(e) {
            e[e.Stereo = 0] = "Stereo", e[e.AC3 = 1] = "AC3"
        }(r || (r = {}));
        var o;
        ! function(e) {
            e[e.CBR = 0] = "CBR", e[e.CVBR = 1] = "CVBR"
        }(o || (o = {}));
        var s;
        ! function(e) {
            e[e.SINGLE_KEY = 0] = "SINGLE_KEY", e[e.DUAL_KEY = 1] = "DUAL_KEY"
        }(s || (s = {}));
        var a;
        ! function(e) {
            e[e.PlayReady = 0] = "PlayReady", e[e.Widevine2 = 1] = "Widevine2", e[e.NONE = 2] = "NONE", e[e.WMRM = 3] = "WMRM", e[e.Marlin = 4] = "Marlin", e[e.FairPlay = 5] = "FairPlay", e[e.CENC = 6] = "CENC"
        }(a || (a = {}));
        var l;
        ! function(e) {
            e[e.Http = 0] = "Http", e[e.Https = 1] = "Https", e[e.Rtmpe = 2] = "Rtmpe"
        }(l || (l = {}));
        var c;
        ! function(e) {
            e[e.H264 = 0] = "H264", e[e.H265 = 1] = "H265"
        }(c || (c = {}));
        var u;
        ! function(e) {
            e[e.UHD = 0] = "UHD", e[e.HD = 1] = "HD", e[e.SD = 2] = "SD"
        }(u || (u = {}));
        var p;
        ! function(e) {
            e[e.DASH = 0] = "DASH", e[e.SmoothStreaming = 1] = "SmoothStreaming", e[e.HLS = 2] = "HLS", e[e.MP4 = 3] = "MP4", e[e.WMV = 4] = "WMV", e[e.MPEGPS = 5] = "MPEGPS", e[e.MPEGTS = 6] = "MPEGTS"
        }(p || (p = {}));
        var h;
        ! function(e) {
            e.Accumulating = "accumulating", e.Live = "live"
        }(h || (h = {}));
        var f = {
            AudioQuality: r,
            BitrateAdaptation: o,
            ConsumptionType: n,
            ContentProtection: a,
            DrmKeyScheme: s,
            LiveManifestType: h,
            Protocol: l,
            StreamingTechnology: p,
            VideoCodec: c,
            VideoQuality: u
        };
        t.exports = f
    }, {}],
    204: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.High = 0] = "High", e[e.Medium = 1] = "Medium", e[e.Low = 2] = "Low"
        }(n || (n = {})), t.exports = n
    }, {}],
    205: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.CacheMiss = 0] = "CacheMiss", e[e.CachePartialHit = 1] = "CachePartialHit", e[e.CacheHit = 2] = "CacheHit"
        }(n || (n = {})), t.exports = n
    }, {}],
    206: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.PostMessage = 0] = "PostMessage", e[e.Http = 1] = "Http"
        }(n || (n = {})), t.exports = n
    }, {}],
    207: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Enabled = 0] = "Enabled", e[e.Auto = 1] = "Auto", e[e.Disabled = 2] = "Disabled"
        }(n || (n = {})), t.exports = n
    }, {}],
    208: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Crash = 1] = "Crash", e[e.Fatal = 2] = "Fatal", e[e.NonFatal = 3] = "NonFatal", e[e.Retriable = 4] = "Retriable", e[e.NoImpact = 5] = "NoImpact"
        }(n || (n = {})), t.exports = n
    }, {}],
    209: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(93),
            o = e(281),
            s = e(228),
            a = e(208),
            l = o.of("AbortEvent"),
            c = function(e) {
                function t(t, i, n, o) {
                    void 0 === n && (n = a.Fatal);
                    var s = e.call(this) || this;
                    return (s.errorName = l(t, "errorName").exists(), l(i, "errorOrErrorMessage").exists(), s.errorName = t, s.errorSeverity = n, s.template = r.isUndefined(o) ? null : o, i instanceof Error ? (s.errorMessage = i.message, s.error = i) : (s.errorMessage = i, s.error = null), s)
                }
                return (n(t, e), t.prototype.getErrorName = function() {
                    return this.errorName
                }, t.prototype.getErrorMessage = function() {
                    return this.errorMessage
                }, t.prototype.getErrorSeverity = function() {
                    return this.errorSeverity
                }, t.prototype.getTemplate = function() {
                    return this.template
                }, t.prototype.getError = function() {
                    return this.error
                }, t.toString = function() {
                    return "abort"
                }, t)
            }(s);
        t.exports = c
    }, {
        208: 208,
        228: 228,
        281: 281,
        93: 93
    }],
    210: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.toString = function() {
                return "aborted"
            }, e)
        }();
        t.exports = n
    }, {}],
    211: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(281),
            o = e(228),
            s = r.of("AsinChangedEvent"),
            a = function(e) {
                function t(t) {
                    var i = e.call(this) || this;
                    return (i.asin = s(t, "asin").exists(), i)
                }
                return (n(t, e), t.toString = function() {
                    return "asinChanged"
                }, t)
            }(o);
        t.exports = a
    }, {
        228: 228,
        281: 281
    }],
    212: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(228),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.toString = function() {
                    return "close"
                }, t)
            }(r);
        t.exports = o
    }, {
        228: 228
    }],
    213: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(228),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this;
                }
                return (n(t, e), t.toString = function() {
                    return "contentEnd"
                }, t)
            }(r);
        t.exports = o
    }, {
        228: 228
    }],
    214: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(228),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.toString = function() {
                    return "firstFrame"
                }, t)
            }(r);
        t.exports = o
    }, {
        228: 228
    }],
    215: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(228),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.toString = function() {
                    return "initialized"
                }, t)
            }(r);
        t.exports = o
    }, {
        228: 228
    }],
    216: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(281),
            o = e(228),
            s = r.of("NotificationCancelEvent"),
            a = function(e) {
                function t(t) {
                    var i = e.call(this) || this;
                    return (i.notification = s(t, "notification").exists(), i)
                }
                return (n(t, e), t.prototype.getNotification = function() {
                    return this.notification
                }, t.toString = function() {
                    return "notificationCancel"
                }, t)
            }(o);
        t.exports = a
    }, {
        228: 228,
        281: 281
    }],
    217: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(281),
            o = e(228),
            s = r.of("NotificationEvent"),
            a = function(e) {
                function t(t, i, n, r, o) {
                    void 0 === n && (n = !1), void 0 === r && (r = 5);
                    var a = e.call(this) || this;
                    return (a.messageTemplate = s(t, "messageTemplate").exists(), a.expandedMessageTemplate = i, a.closable = n, a.displayTimeSeconds = r, a.onDontShowAgain = o, a)
                }
                return (n(t, e), t.prototype.getMessageTemplate = function() {
                    return this.messageTemplate
                }, t.prototype.getExpandedMessageTemplate = function() {
                    return this.expandedMessageTemplate
                }, t.prototype.getClosable = function() {
                    return this.closable
                }, t.prototype.getDisplayTimeSeconds = function() {
                    return this.displayTimeSeconds
                }, t.prototype.getOnDontShowAgain = function() {
                    return this.onDontShowAgain
                }, t.toString = function() {
                    return "notification"
                }, t)
            }(o);
        t.exports = a
    }, {
        228: 228,
        281: 281
    }],
    218: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(221),
            o = function(e) {
                function t(t) {
                    var i = e.call(this) || this;
                    return (i.pinValidationStartTimeMs = t, i)
                }
                return (n(t, e), t.prototype.getPinValidationStartTimeMs = function() {
                    return this.pinValidationStartTimeMs
                }, t.toString = function() {
                    return "pinAccepted"
                }, t)
            }(r);
        t.exports = o
    }, {
        221: 221
    }],
    219: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(221),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.toString = function() {
                    return "pinDeclined"
                }, t)
            }(r);
        t.exports = o
    }, {
        221: 221
    }],
    220: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(221),
            o = function(e) {
                function t(t) {
                    var i = e.call(this) || this;
                    return (i.errorName = t, i)
                }
                return (n(t, e), t.prototype.getErrorName = function() {
                    return this.errorName
                }, t.toString = function() {
                    return "pinError"
                }, t)
            }(r);
        t.exports = o
    }, {
        221: 221
    }],
    221: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return e
        }();
        t.exports = n
    }, {}],
    222: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(221),
            o = function(e) {
                function t(t, i, n, r) {
                    void 0 === i && (i = !1), void 0 === n && (n = !1), void 0 === r && (r = !1);
                    var o = e.call(this) || this;
                    return (o.pinLength = t, o.requiresPINSetup = i, o.hidePlayer = r, o.requiresAgeVerification = n, o)
                }
                return (n(t, e), t.prototype.getHidePlayer = function() {
                    return this.hidePlayer
                }, t.prototype.getPINLength = function() {
                    return this.pinLength
                }, t.prototype.getRequiresAgeVerification = function() {
                    return this.requiresAgeVerification
                }, t.prototype.getRequiresPINSetup = function() {
                    return this.requiresPINSetup
                }, t.toString = function() {
                    return "pinRequired"
                }, t)
            }(r);
        t.exports = o
    }, {
        221: 221
    }],
    223: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(228),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.toString = function() {
                    return "paused"
                }, t)
            }(r);
        t.exports = o
    }, {
        228: 228
    }],
    224: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(281),
            o = e(228),
            s = r.of("PlayerCapabilitiesEvent"),
            a = function(e) {
                function t(t) {
                    var i = e.call(this) || this;
                    return (i.capabilities = s(t, "playerCapabilities").exists(), i)
                }
                return (n(t, e), t.toString = function() {
                    return "playerCapabilities"
                }, t)
            }(o);
        t.exports = a
    }, {
        228: 228,
        281: 281
    }],
    225: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(228),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.toString = function() {
                    return "playing"
                }, t)
            }(r);
        t.exports = o
    }, {
        228: 228
    }],
    226: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(228),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.toString = function() {
                    return "prepared"
                }, t)
            }(r);
        t.exports = o
    }, {
        228: 228
    }],
    227: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(228),
            o = function(e) {
                function t() {
                    return null !== e && e.apply(this, arguments) || this
                }
                return (n(t, e), t.toString = function() {
                    return "stopping"
                }, t)
            }(r);
        t.exports = o
    }, {
        228: 228
    }],
    228: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return e
        }();
        t.exports = n
    }, {}],
    229: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.volume = 0] = "volume"
        }(n || (n = {})), t.exports = n
    }, {}],
    230: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Feature = 0] = "Feature", e[e.Trailer = 1] = "Trailer", e[e.Live = 2] = "Live", e[e.External = 3] = "External", e[e.VideoAds = 4] = "VideoAds"
        }(n || (n = {})), t.exports = n
    }, {}],
    231: [function(e, t, i) {
        "use strict";
        var n = e(209),
            r = e(210),
            o = e(211),
            s = e(205),
            a = e(204),
            l = e(212),
            c = e(207),
            u = e(206),
            p = e(213),
            h = e(208),
            f = e(229),
            d = e(214),
            y = e(215),
            g = e(216),
            m = e(217),
            v = e(218),
            E = e(219),
            _ = e(221),
            S = e(220),
            T = e(222),
            b = e(223),
            I = e(224),
            w = e(225),
            R = e(226),
            C = e(227),
            P = e(230),
            A = e(228),
            O = {
                AbortEvent: n,
                AbortedEvent: r,
                AsinChangedEvent: o,
                CacheType: s,
                CachePriority: a,
                CloseEvent: l,
                DiagnosticsState: c,
                DiagnosticsProtocol: u,
                EndOfContentEvent: p,
                ErrorSeverity: h,
                Features: f,
                FirstFrameEvent: d,
                InitializedEvent: y,
                NotificationCancelEvent: g,
                NotificationEvent: m,
                PINAcceptedEvent: v,
                PINDeclinedEvent: E,
                PINEvent: _,
                PINErrorEvent: S,
                PINRequiredEvent: T,
                PausedEvent: b,
                PlayerCapabilitiesEvent: I,
                PlayingEvent: w,
                PreparedEvent: R,
                StoppingEvent: C,
                VideoType: P,
                WebPlayerEvent: A
            };
        t.exports = O
    }, {
        204: 204,
        205: 205,
        206: 206,
        207: 207,
        208: 208,
        209: 209,
        210: 210,
        211: 211,
        212: 212,
        213: 213,
        214: 214,
        215: 215,
        216: 216,
        217: 217,
        218: 218,
        219: 219,
        220: 220,
        221: 221,
        222: 222,
        223: 223,
        224: 224,
        225: 225,
        226: 226,
        227: 227,
        228: 228,
        229: 229,
        230: 230
    }],
    232: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(281),
            o = e(290),
            s = e(294),
            a = r.of("QueryString"),
            l = function() {
                function e() {}
                return (e.getParameter = function(t, i) {
                    a(t, "name").exists();
                    var n = e.getQueryString(i);
                    return n.hasOwnProperty(t) ? n[t] : null
                }, e.getQueryString = function(t) {
                    if (n.isNullOrUndefined(t)) {
                        if ("undefined" == typeof location || "undefined" == typeof location.search) return {};
                        t = location.search
                    }
                    try {
                        var i = o.parse(t.replace(/^\?/, ""));
                        return e.parseStrings(i)
                    } catch (e) {
                        return {}
                    }
                }, e.parseStrings = function(t) {
                    if (s.isString(t)) return !!/^true$/i.test(t) || !/^false$/i.test(t) && (/^null$/i.test(t) ? null : /^undefined$/i.test(t) ? e.UNDEFINED : /^\d+$/.test(t) ? parseInt(t, 10) : t);
                    if (s.isArray(t) || s.isObject(t)) {
                        var i = s.isArray(t) ? [] : {};
                        for (var n in t) t.hasOwnProperty(n) && (i[n] = this.parseStrings(t[n]));
                        return i
                    }
                    return t
                }, e.UNDEFINED = void 0, e)
            }();
        t.exports = l
    }, {
        281: 281,
        290: 290,
        294: 294,
        93: 93
    }],
    233: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.prototype.fatalErrorOccurred = function(e) {}, e.prototype.nonFatalErrorOccurred = function(e) {}, e.prototype.appLoadStarted = function() {}, e.prototype.appLoadCompleted = function() {
                return 0
            }, e.prototype.playerInitializationStarted = function() {}, e.prototype.playerInitializationCompleted = function() {}, e.prototype.contentPreparationStarted = function(e, t) {}, e.prototype.contentPreparationSucceeded = function() {}, e.prototype.contentPreparationFailed = function() {}, e.prototype.playbackInitiated = function(e) {}, e.prototype.playbackStarted = function(e) {
                return 0
            }, e.prototype.pinDialogDisplayed = function() {}, e.prototype.pinSubmitted = function() {}, e.prototype.pinAccepted = function() {}, e.prototype.pinDeclined = function() {}, e.prototype.seekInitiated = function() {}, e.prototype.seekCompleted = function() {}, e.prototype.nextTitlePlaybackInitiated = function() {}, e.prototype.hdToggled = function(e) {}, e.prototype.manifestDownloadStarted = function() {}, e.prototype.manifestDownloaded = function() {}, e.prototype.bufferingStarted = function(e, t) {}, e.prototype.bufferingComplete = function(e) {}, e.prototype.reportInitialBufferCompleted = function(e) {}, e.prototype.prepareClickstream = function(e, t) {}, e.prototype.clickstreamNextTitle = function() {}, e.prototype.clickstreamPlaybackStarted = function(e) {}, e.prototype.reportClickstream = function(e) {}, e.prototype.fullScreenToggled = function(e, t, i) {}, e.prototype.prePlaybackModalStarted = function(e) {}, e.prototype.prePlaybackModalClosed = function(e) {}, e)
        }();
        t.exports = n
    }, {}],
    234: [function(e, t, i) {
        "use strict";
        var n = e(242),
            r = e(95),
            o = 2e5,
            s = function() {
                function e(e, t, i) {
                    void 0 === e && (e = null), void 0 === t && (t = null), void 0 === i && (i = n), this.bufferMaxBytes = o, this.viewIndex = 0, this.autoFlush = !1, this.isEnabled = !0, this.isFullyWritten = !1;
                    try {
                        null === e || null === t ? (this.logBuffer = new ArrayBuffer(o), this.bufferView = new Uint8Array(this.logBuffer)) : (this.bufferMaxBytes = t.buffer.byteLength, this.logBuffer = e, this.bufferView = t), this.typedArrayDownloader = i
                    } catch (e) {
                        this.isEnabled = !1
                    }
                }
                return (e.prototype.setAutoFlush = function(e) {
                    this.isEnabled && (this.autoFlush = e)
                }, e.prototype.log = function(e) {
                    if (this.isEnabled)
                        for (var t = 0; t < e.length; t++) this.bufferView.set([e.charCodeAt(t)], this.viewIndex), ++this.viewIndex >= this.bufferMaxBytes && (this.viewIndex %= this.bufferMaxBytes, this.isFullyWritten = !0, this.autoFlush && this.flushLogs(!1))
                }, e.prototype.manuallyDownloadLogs = function() {
                    this.isEnabled && this.flushLogs(!0)
                }, e.prototype.flushLogs = function(e) {
                    var t, i;
                    e ? (t = this.formatManualView(), i = "manualDownload.txt") : (t = this.bufferView, i = r.dateTimeToString(new Date) + ".txt"), this.typedArrayDownloader.downloadUint8(t, i)
                }, e.prototype.formatManualView = function() {
                    var e = new Uint8Array(this.logBuffer, 0, this.viewIndex);
                    if (this.isFullyWritten) {
                        var t = new Uint8Array(this.logBuffer, this.viewIndex, this.logBuffer.byteLength - this.viewIndex),
                            i = new Uint8Array(this.logBuffer.byteLength);
                        return (i.set(t), i.set(e, t.byteLength), i)
                    }
                    return e
                }, e)
            }();
        t.exports = s
    }, {
        242: 242,
        95: 95
    }],
    235: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Ad = 0] = "Ad", e[e.Main = 1] = "Main"
        }(n || (n = {})), t.exports = n
    }, {}],
    236: [function(e, t, i) {
        "use strict";
        var n = e(66),
            r = n.Action,
            o = n.PageType,
            s = n.RefMarker,
            a = n.SubPageType,
            l = n.HitType,
            c = e(69),
            u = c.ClickstreamEventBuilder,
            p = e(187),
            h = e(93),
            f = e(281),
            d = e(284),
            y = e(235),
            g = f.of("DefaultMetricsContext"),
            m = d.get("MetricsContext"),
            v = function() {
                function e(e, t, i) {
                    void 0 === i && (i = p), this.createClickstreamClient = g(e, "createClickstreamClient").exists(), this.now = h.exists(t) ? t : Date.now ? Date.now : function() {
                        return (new Date).getTime()
                    }, this.metricReporter = i, this.clickstreamClient = null, this.asin = null, this.refMarker = null, this.appLoadStartTime = null, this.contentPreparationStartTime = null, this.nextTitlePlaybackInitiatedTime = null, this.manifestDownloadStartTime = null, this.pinDialogDisplayedTime = null, this.pinEntryTime = 0, this.playbackInitiatedTime = null, this.playerInitializationStartTime = null, this.prePlaybackModalStartedTime = null, this.prePlaybackModalClosedTime = null, this.seekInitiatedTime = null, this.bufferReasons = {}, this.bufferReasons[y.Ad] = null, this.bufferReasons[y.Main] = null, this.bufferTimers = {}, this.bufferTimers[y.Ad] = null, this.bufferTimers[y.Main] = null
                }
                return (e.prototype.fatalErrorOccurred = function(t) {
                    this.nonFatalErrorOccurred(t), this.putMetric(e.FATAL_ERROR_METRIC)
                }, e.prototype.nonFatalErrorOccurred = function(e) {
                    g(e, "error").exists(), e instanceof Error ? this.putMetric(e.name) : this.putMetric(e)
                }, e.prototype.appLoadStarted = function() {
                    return null !== this.appLoadStartTime ? void m.error("appLoadStarted called when app load is already in progress") : void(this.appLoadStartTime = this.now())
                }, e.prototype.appLoadCompleted = function() {
                    if (null === this.appLoadStartTime) return void m.error("appLoadCompleted called when app load is not in progress");
                    var t = this.now() - this.appLoadStartTime;
                    return (this.putTime(e.APP_LOAD_TIME_METRIC, t), this.appLoadStartTime = null, t)
                }, e.prototype.playerInitializationStarted = function() {
                    return null !== this.playerInitializationStartTime ? void m.error("playerInitializationStarted called when player initialization is already in progress") : (this.reportRateBasedMetrics(), void(this.playerInitializationStartTime = this.now()))
                }, e.prototype.playerInitializationCompleted = function() {
                    if (null === this.playerInitializationStartTime) return void m.error("playerInitializationCompleted called when player initialization is not in progress");
                    var t = this.now() - this.playerInitializationStartTime;
                    this.putTime(e.INITIALIZE_TIME_METRIC, t), this.playerInitializationStartTime = null
                }, e.prototype.contentPreparationStarted = function(e, t) {
                    return (this.asin = g(e, "asin").exists(), this.isTrailer = g(t, "isTrailer").exists(), null !== this.contentPreparationStartTime ? void m.error("contentPreparationStarted called when content preparation is already in progress") : (this.contentPreparationStartTime = this.now(), void(this.pinDialogDisplayedTime = null)))
                }, e.prototype.contentPreparationSucceeded = function() {
                    if (null === this.contentPreparationStartTime) return void m.error("contentPreparationSucceeded called when content preparation is not in progress");
                    var t = this.now() - this.contentPreparationStartTime;
                    this.putTime(e.PREPARE_TIME_METRIC, t), this.contentPreparationStartTime = null
                }, e.prototype.contentPreparationFailed = function() {
                    return null === this.contentPreparationStartTime ? void m.error("contentPreparationFailed called when content preparation is not in progress") : void(this.contentPreparationStartTime = null)
                }, e.prototype.playbackInitiated = function(t) {
                    return (g(t, "playbackInitiatedTime").exists(), null === this.asin || null === this.isTrailer ? void m.error("playbackStarted called without first preparing content") : null !== this.playbackInitiatedTime ? void m.error("playbackInitiated called when playback has already been initiated") : (this.putMetric(e.PLAY_METRIC), void(this.playbackInitiatedTime = t)))
                }, e.prototype.playbackStarted = function(t) {
                    if (void 0 === t && (t = !1), null === this.playbackInitiatedTime && null === this.nextTitlePlaybackInitiatedTime) return void m.error("playbackStarted called when neither initial playback or next title playback has been initiated");
                    if (null !== this.playbackInitiatedTime && null !== this.nextTitlePlaybackInitiatedTime) return void m.error("playbackStarted called when both playback and next title playback have been initiated");
                    var i = h.exists(this.playbackInitiatedTime) ? e.TTFF_METRIC : e.NEXT_TITLE_TTFF_METRIC,
                        n = this.calculateTimeToFirstFrame(h.withDefault(this.playbackInitiatedTime, this.nextTitlePlaybackInitiatedTime), t);
                    return (this.putTime(i, n), n)
                }, e.prototype.pinDialogDisplayed = function() {
                    return null !== this.pinDialogDisplayedTime ? void m.error("pinDialogDisplayed called when PIN dialog is already displayed") : (this.pinDialogDisplayedTime = this.now(), void this.reportClickstreamPageType(o.Player, a.Pin))
                }, e.prototype.pinSubmitted = function() {
                    return null === this.pinDialogDisplayedTime ? void m.error("pinSubmitted called when PIN dialog is not displayed") : void(this.refMarker = s[s.plr_pin_ok])
                }, e.prototype.pinAccepted = function() {
                    return null === this.pinDialogDisplayedTime ? void m.error("pinAccepted called when PIN dialog is not displayed") : (this.pinEntryTime = this.now() - this.pinDialogDisplayedTime, this.putTime(e.PIN_ENTRY_TIME_METRIC, this.pinEntryTime), void(this.pinDialogDisplayedTime = null))
                }, e.prototype.pinDeclined = function() {
                    return null === this.pinDialogDisplayedTime ? void m.error("pinDeclined called when PIN dialog is not displayed") : void this.reportClickstreamPageType(o.Player, a.PinError)
                }, e.prototype.seekInitiated = function() {
                    this.seekInitiatedTime = this.now()
                }, e.prototype.seekCompleted = function() {
                    if (null === this.seekInitiatedTime) return void m.error("seekCompleted called when seek is not in progress");
                    var t = this.now() - this.seekInitiatedTime;
                    this.putTime(e.SEEK_TIME_METRIC, t), this.seekInitiatedTime = null
                }, e.prototype.nextTitlePlaybackInitiated = function(e) {
                    return (g(e, "playbackInitiatedTime").exists(), null !== this.nextTitlePlaybackInitiatedTime ? void m.error("nextTitlePlaybackInitiated called when next title playback has already been initiated") : void(this.nextTitlePlaybackInitiatedTime = e))
                }, e.prototype.hdToggled = function(e) {
                    var t = (new u).withPageTypeId(this.asin).withRefMarker(e ? s[s.plr_hd_on] : s[s.plr_hd_off]).withPageType(o.Player).withSubPageType(a.Player).build();
                    this.reportClickstream(t)
                }, e.prototype.fullScreenToggled = function(e, t, i) {
                    var n = (new u).withPageTypeId(this.asin).withRefMarker(e ? s[s.plr_b_fscrn_on] : s[s.plr_b_fscrn_off]).withPageType(t).withSubPageType(i).withHitType(l.PageTouch).build();
                    this.reportClickstream(n)
                }, e.prototype.manifestDownloadStarted = function() {
                    this.manifestDownloadStartTime = this.now()
                }, e.prototype.manifestDownloaded = function() {
                    if (null === this.manifestDownloadStartTime) return void m.error("manifestDownloaded called when manifest download has not been started");
                    var t = this.now() - this.manifestDownloadStartTime;
                    this.putTime(e.MANIFEST_DOWNLOAD_TIME_METRIC, t), this.manifestDownloadStartTime = null
                }, e.prototype.bufferingStarted = function(e, t) {
                    return h.isNullOrUndefined(e) ? void m.error("bufferingStarted called but no contentType was provided") : h.isNullOrUndefined(t) ? void m.error("bufferingStarted called but no bufferReason was provided") : (this.bufferReasons[e] = t, void(this.bufferTimers[e] = this.now()))
                }, e.prototype.bufferingComplete = function(t) {
                    if (h.isNullOrUndefined(t)) return void m.error("bufferingComplete called but no contentType was provided");
                    var i = this.bufferReasons[t],
                        n = this.bufferTimers[t];
                    if (null !== n) {
                        var r = this.now() - n;
                        this.putTime("" + y[t] + e.BUFFER_TIME_METRIC + "." + i, r), this.bufferTimers[t] = null, this.bufferReasons[t] = null
                    }
                }, e.prototype.reportInitialBufferCompleted = function(t) {
                    this.putMetric(e.INITIAL_BUFFER_COMPLETED, t)
                }, e.prototype.prepareClickstream = function(t, i) {
                    h.isNullOrUndefined(t) ? (m.error("No clickstreamSessionId was provided; Clickstream events will not be reported"), this.putMetric(e.MISSING_CLICKSTREAM_SESSION_ID_ERROR_METRIC)) : h.isNullOrUndefined(i) ? (m.error("No clickstreamRefMarker was provided; Clickstream events will not be reported"), this.putMetric(e.MISSING_CLICKSTREAM_REF_MARKER_ERROR_METRIC)) : (this.clickstreamClient = this.createClickstreamClient(t), this.refMarker = i)
                }, e.prototype.clickstreamNextTitle = function() {
                    this.refMarker = s[s.plr_nxt_ep]
                }, e.prototype.clickstreamPlaybackStarted = function(e) {
                    var t = this.isTrailer ? r.PlayTrailer : r.PlayStream;
                    this.reportClickstreamAction(t, o.Player, e)
                }, e.prototype.reportClickstream = function(t) {
                    var i = this;
                    if (null === this.clickstreamClient) return void m.error("Attempted to report a Clickstream page with a null Clickstream client reference");
                    if (null === t) return void m.error("Attempted to report a Clickstream page type with a null event");
                    if (null === t.refMarker) return void m.error("Attempted to report a Clickstream page type without a ref marker");
                    if (null === t.pageTypeId) return void m.error("Attempted to report Clickstream with no pageTypeId");
                    m.debug("Reporting Clickstream event for Action '" + t.action + "'\n            with page type '" + t.pageType + "' SubPageType '" + t.subPageType + "' and ref marker\n            '" + t.refMarker + "' for pageTypeId '" + t.pageTypeId + "'");
                    try {
                        this.clickstreamClient.report(t, function(t) {
                            i.putTime(e.CLICKSTREAM_TIME_METRIC, t)
                        }, function(t, n) {
                            m.error("Unable to send Clickstream event: " + t, t), i.putMetric(e.CLICKSTREAM_ERROR_METRIC), h.isNullOrUndefined(n) || i.putTime(e.CLICKSTREAM_TIME_METRIC, n)
                        })
                    } catch (e) {
                        m.error("Failed to send ClickStream event: " + e, e)
                    }
                }, e.prototype.prePlaybackModalStarted = function(e) {
                    g(e, "startTimeMs").exists(), this.prePlaybackModalStartedTime = h.withDefault(this.prePlaybackModalStartedTime, e)
                }, e.prototype.prePlaybackModalClosed = function(e) {
                    g(e, "closeTimeMs").exists(), h.isNullOrUndefined(this.prePlaybackModalClosedTime) && h.exists(this.prePlaybackModalStartedTime) && (this.prePlaybackModalClosedTime = e)
                }, e.prototype.reportRateBasedMetrics = function() {
                    this.putMetric(e.PLAYBACK_STUCK_METRIC, 0), this.putMetric(e.FATAL_ERROR_METRIC, 0)
                }, e.prototype.putMetric = function(e, t) {
                    try {
                        this.metricReporter.putMetric(e, t)
                    } catch (i) {
                        m.error("An error occurred while publishing metric " + e + ": " + i.name + ": " + (h.exists(t) ? t + ":" : "") + " " + i.message, i)
                    }
                }, e.prototype.putTime = function(t, i) {
                    m.debug(t + " was " + i + " ms");
                    try {
                        this.metricReporter.putMetric(t, i, e.MILLISECONDS)
                    } catch (e) {
                        m.error("An error occurred while publishing metric " + t + ": " + e.name + ": " + e.message, e)
                    }
                }, e.prototype.reportClickstreamAction = function(e, t, i) {
                    var n = (new u).withAction(e).withPageType(t).withRefMarker(this.refMarker).withSubPageType(a.Player).withPageTypeId(this.asin).withWeblabs(i).build();
                    this.refMarker = null, this.reportClickstream(n)
                }, e.prototype.reportClickstreamPageType = function(e, t) {
                    if (null !== this.clickstreamClient) {
                        var i = (new u).withPageType(e).withSubPageType(t).withRefMarker(this.refMarker).withPageTypeId(this.asin).build();
                        this.refMarker = null, this.reportClickstream(i)
                    }
                }, e.prototype.calculateTimeToFirstFrame = function(e, t) {
                    var i = this.now() - (t ? this.pinEntryTime : 0);
                    return (h.isNullOrUndefined(this.prePlaybackModalClosedTime) ? i -= e : this.playbackInitiatedTime < this.prePlaybackModalStartedTime ? i += this.prePlaybackModalStartedTime - e - this.prePlaybackModalClosedTime : i -= this.prePlaybackModalClosedTime, this.nextTitlePlaybackInitiatedTime = null, this.pinEntryTime = 0, this.playbackInitiatedTime = null, this.prePlaybackModalStartedTime = null, this.prePlaybackModalClosedTime = null, i)
                }, e.MILLISECONDS = "ms", e.APP_LOAD_TIME_METRIC = "AppLoadTime", e.BUFFER_TIME_METRIC = "BufferTime", e.CLICKSTREAM_ERROR_METRIC = "ClickstreamError", e.CLICKSTREAM_TIME_METRIC = "ClickstreamTime", e.FATAL_ERROR_METRIC = "FatalError", e.INITIAL_BUFFER_COMPLETED = "InitialBufferCompleted", e.INITIALIZE_TIME_METRIC = "InitializeTime", e.MANIFEST_DOWNLOAD_TIME_METRIC = "ManifestDownloadTime", e.MISSING_CLICKSTREAM_REF_MARKER_ERROR_METRIC = "MissingClickstreamRefMarkerError", e.MISSING_CLICKSTREAM_SESSION_ID_ERROR_METRIC = "MissingClickstreamSessionIdError", e.NEXT_TITLE_TTFF_METRIC = "NextTitleTTFF", e.PIN_ENTRY_TIME_METRIC = "PinEntryTime", e.PLAY_METRIC = "Play", e.PLAYBACK_STUCK_METRIC = "PlaybackStuck", e.PREPARE_TIME_METRIC = "PrepareTime", e.SEEK_TIME_METRIC = "SeekTime", e.TTFF_METRIC = "TTFF", e)
            }();
        t.exports = v
    }, {
        187: 187,
        235: 235,
        281: 281,
        284: 284,
        66: 66,
        69: 69,
        93: 93
    }],
    237: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = function() {
                function e() {
                    this.capturedLogs = []
                }
                return (e.prototype.getHandler = function() {
                    var e = this;
                    return function(t, i) {
                        e.capturedLogs.push({
                            messages: t,
                            context: i
                        })
                    }
                }, e.prototype.flushTo = function(e, t) {
                    for (var i = 0; i < this.capturedLogs.length; i++)(n.isNullOrUndefined(t) || this.capturedLogs[i].context.level.value >= t.value) && e(this.capturedLogs[i].messages, this.capturedLogs[i].context);
                    this.capturedLogs.length = 0
                }, e)
            }();
        t.exports = r
    }, {
        93: 93
    }],
    238: [function(e, t, i) {
        "use strict";
        var n = e(158),
            r = e(231),
            o = r.DiagnosticsState,
            s = e(150),
            a = s.LogEvent,
            l = s.LogLevels,
            c = e(284),
            u = e(93),
            p = e(281),
            h = e(95),
            f = e(234),
            d = p.of("Logging"),
            y = function() {
                function e() {}
                return (e.parseLevel = function(t) {
                    d(t, "level").exists();
                    var i = e.LEVELS[t.toLowerCase()];
                    return u.isUndefined(i) ? e.DEFAULT_LOG_LEVEL : i
                }, e.setBufferFilter = function(t) {
                    e.bufferedLogFilterLevel = e.parseLevel(t)
                }, e.createHandler = function(t, i, r, s) {
                    var c = this;
                    void 0 === i && (i = o.Disabled), void 0 === s && (s = n.getInstance()), u.isNullOrUndefined(r) && "undefined" != typeof console && (r = console);
                    var p = [];
                    return (i !== o.Disabled && "undefined" != typeof r && p.push(function(t, i) {
                        try {
                            var n = [h.dateTimeToString(new Date)].concat(i),
                                o = e.getConsoleMethodForLevel(t, r);
                            Function.prototype.apply.call(o, r, n)
                        } catch (e) {}
                    }), u.isNullOrUndefined(t) || p.push(function(e, i) {
                        try {
                            t.log(e.name, i)
                        } catch (e) {}
                    }), i !== o.Disabled && (this.bufferedLogger = new f, p.push(function(t, i) {
                        try {
                            var n = [h.dateTimeToString(new Date), t.name].concat(i),
                                r = n.join(" ") + "\n";
                            t.value >= e.bufferedLogFilterLevel.value && c.bufferedLogger.log(r)
                        } catch (e) {}
                    }), this.realTimeLogger = s, p.push(function(e, t) {
                        try {
                            var i = [h.dateTimeToString(new Date), e.name].concat(t),
                                n = i.join(" ") + "\n";
                            c.realTimeLogger.report(new a(n, l[e.name.toUpperCase()]))
                        } catch (e) {}
                    })), 0 === p.length ? function(e, t) {} : function(e, t) {
                        if (0 !== e.length) {
                            var i = Array.prototype.slice.call(e);
                            u.isNullOrUndefined(t.name) || i.unshift("[" + t.name + "]");
                            for (var n = 0; n < p.length; n++) p[n](t.level, i)
                        }
                    })
                }, e.getConsoleMethodForLevel = function(e, t) {
                    var i = t[e.name.toLowerCase()];
                    return u.isUndefined(i) ? t.log : i
                }, e.DEFAULT_LOG_LEVEL = c.INFO, e.bufferedLogger = null, e.bufferedLogFilterLevel = c.INFO, e.LEVELS = {
                    debug: c.DEBUG,
                    info: c.INFO,
                    warn: c.WARN,
                    error: c.ERROR
                }, e)
            }();
        t.exports = y
    }, {
        150: 150,
        158: 158,
        231: 231,
        234: 234,
        281: 281,
        284: 284,
        93: 93,
        95: 95
    }],
    239: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {
                this.capturedMetrics = []
            }
            return (e.prototype.getHandler = function() {
                var e = this;
                return function(t, i, n) {
                    e.capturedMetrics.push({
                        name: t,
                        value: i,
                        unit: n
                    })
                }
            }, e.prototype.flushTo = function(e) {
                for (var t = 0; t < this.capturedMetrics.length; t++) e(this.capturedMetrics[t].name, this.capturedMetrics[t].value, this.capturedMetrics[t].unit);
                this.capturedMetrics.length = 0
            }, e)
        }();
        t.exports = n
    }, {}],
    240: [function(e, t, i) {
        "use strict";
        var n = e(69),
            r = n.ClickstreamClient,
            o = e(93),
            s = e(281),
            a = e(236),
            l = s.of("MetricsContextFactory"),
            c = function() {
                function e() {}
                return (e.prototype.createMetricsContext = function(e, t, i, n, s, c) {
                    l(e, "partnerServiceEndpoint").exists(), l(t, "deviceId").exists(), l(i, "deviceTypeId").exists(), l(n, "marketplaceId").exists();
                    var u = function(a) {
                        return r.isSupported() && o.exists(s) ? new r(e, a, n, s, t, i, c) : null
                    };
                    return new a(u)
                }, e)
            }();
        t.exports = c
    }, {
        236: 236,
        281: 281,
        69: 69,
        93: 93
    }],
    241: [function(e, t, i) {
        "use strict";
        var n = e(167),
            r = n.LogPublisher,
            o = n.MetricsPublisher,
            s = e(272),
            a = s.BrowserType,
            l = e(231),
            c = l.DiagnosticsState,
            u = e(187),
            p = e(158),
            h = e(284),
            f = e(93),
            d = e(281),
            y = e(237),
            g = e(238),
            m = e(239),
            v = d.of("Reporter"),
            E = h.get("Reporter"),
            _ = function() {
                function e(e) {
                    void 0 === e && (e = window), this.isLoggingInitialized = !1, this.areMetricsPreInitialized = !1, this.areMetricsInitialized = !1, this.logCaptor = null, this.metricsCaptor = null, this.logPublisher = null, this.metricsFeature = null, this.metricsPublisher = null, this.flushLogs = null, this.flushMetrics = null, this._window = e
                }
                return (e.prototype.preInitializeLogging = function() {
                    this.isLoggingInitialized || (this.logCaptor = new y, h.setLevel(h.DEBUG), h.setHandler(this.logCaptor.getHandler()))
                }, e.prototype.initializeLogging = function(e, t, i, n, o, s, a) {
                    var l = this;
                    void 0 === s && (s = 0), void 0 === a && (a = c.Disabled), v(e, "marketplaceId").exists(), v(t, "customerId").exists(), v(i, "sessionId").exists();
                    var u = g.DEFAULT_LOG_LEVEL;
                    if (a !== c.Disabled) {
                        u = h.INFO;
                        var d = p.getInstance();
                        d.initialize(n)
                    }
                    if (f.isNullOrUndefined(o) || (u = g.parseLevel(o)), h.setLevel(u), !r.isSupported()) return void h.setHandler(g.createHandler(null, a));
                    this.logPublisher = new r(e, t, i, s);
                    var y = g.createHandler(this.logPublisher, a);
                    h.setHandler(y), this.flushLogs = function() {
                        try {
                            l.logPublisher.flush()
                        } catch (e) {}
                    }, this.addBeforeUnloadListener(this.flushLogs), null !== this.logCaptor && (this.logCaptor.flushTo(y, u), this.logCaptor = null), this.isLoggingInitialized = !0
                }, e.prototype.updateLoggingSessionId = function(e) {
                    v(e, "sessionId").exists(), null !== this.logPublisher && this.logPublisher.updateSessionId(e)
                }, e.prototype.preInitializeMetrics = function(t, i, n, r, s, a) {
                    void 0 === a && (a = 0), v(!this.areMetricsPreInitialized, "!this.areMetricsPreInitialized").is["true"](), v(t, "marketplaceId").exists(), v(i, "sessionId").exists(), v(r, "metricsFeature").exists(), v(s, "browser").exists();
                    var l = this.getClient(s);
                    n = f.withDefault(n, e.DEFAULT_CLIENT_ID), this.metricsFeature = r, this.metricsPublisher = new o(t, i, n, l, a), this.metricsCaptor = new m, u.setHandler(this.metricsCaptor.getHandler()), this.areMetricsPreInitialized = !0
                }, e.prototype.initializeMetrics = function(e) {
                    v(this.areMetricsPreInitialized, "this.areMetricsPreInitialized").is["true"](), v(e, "metricsFeature").exists(), this.metricsFeature = e, this.areMetricsInitialized = !0, this.setMetricsHandlerAndFlusher()
                }, e.prototype.updateMetricsSessionId = function(e) {
                    v(e, "sessionId").exists(), null !== this.metricsPublisher && this.metricsPublisher.updateSessionId(e)
                }, e.prototype.flush = function() {
                    this.areMetricsPreInitialized && !this.areMetricsInitialized && this.setMetricsHandlerAndFlusher(), null !== this.flushLogs && this.flushLogs(), null !== this.flushMetrics && this.flushMetrics()
                }, e.prototype.setMetricsHandlerAndFlusher = function() {
                    var e = this,
                        t = function(t, i, n) {
                            e.metricsPublisher.putMetric(e.metricsFeature, "." + t, i, n)
                        };
                    u.setHandler(t), this.flushMetrics = function() {
                        try {
                            e.metricsPublisher.flush()
                        } catch (e) {
                            E.warn(e)
                        }
                    }, u.setFlusher(this.flushMetrics), this.addBeforeUnloadListener(this.flushMetrics), null !== this.metricsCaptor && (this.metricsCaptor.flushTo(t),
                        this.metricsCaptor = null)
                }, e.prototype.addBeforeUnloadListener = function(e) {
                    "undefined" != typeof this._window.addEventListener ? this._window.addEventListener("beforeunload", e) : "undefined" != typeof this._window.attachEvent && this._window.attachEvent("onbeforeunload", e)
                }, e.prototype.removeBeforeUnloadListener = function(e) {
                    "undefined" != typeof this._window.addEventListener ? this._window.removeEventListener("beforeunload", e) : "undefined" != typeof this._window.attachEvent && this._window.detachEvent("onbeforeunload", e)
                }, e.prototype.getClient = function(e) {
                    return a[e.getType()]
                }, e.DEFAULT_CLIENT_ID = "NONE", e)
            }();
        t.exports = _
    }, {
        158: 158,
        167: 167,
        187: 187,
        231: 231,
        237: 237,
        238: 238,
        239: 239,
        272: 272,
        281: 281,
        284: 284,
        93: 93
    }],
    242: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.downloadUint8 = function(e, t) {
                try {
                    var i = new Blob([e], {
                            type: "text/plain"
                        }),
                        n = window.URL.createObjectURL(i),
                        r = document.createElement("a");
                    r.setAttribute("download", t), r.setAttribute("href", n), r.setAttribute("target", "_blank"), document.body.appendChild(r), r.click(), document.body.removeChild(r)
                } catch (e) {}
            }, e)
        }();
        t.exports = n
    }, {}],
    243: [function(e, t, i) {
        "use strict";
        var n = e(233),
            r = e(235),
            o = e(240),
            s = e(238),
            a = e(241),
            l = e(234),
            c = {
                BlackHoleMetricsContext: n,
                ContentType: r,
                MetricsContextFactory: o,
                Logging: s,
                Reporter: a,
                BufferedLogger: l
            };
        t.exports = c
    }, {
        233: 233,
        234: 234,
        235: 235,
        238: 238,
        240: 240,
        241: 241
    }],
    244: [function(e, t, i) {
        "use strict";
        var n = e(180),
            r = n.IndexedDBStorage,
            o = e(245),
            s = e(247),
            a = e(248),
            l = function() {
                function e(e, t, i, n) {
                    void 0 === e && (e = new o), void 0 === t && (t = new a), void 0 === i && (i = new r("ScriptStore"));
                    var l = this;
                    this.SCRIPT_TYPES_XHR_REQUIRED = [s.JSON], this.XHR_TIMEOUT_MS = 12e4, this.XHR_RETRIES = 2, this.loadJsonFromString = function(e, t, i) {
                        try {
                            var n = (new Date).getTime();
                            i.payload = JSON.parse(t), i.parseMs = (new Date).getTime() - n, i.totalMs = l.calculateTotalLoadMs(i), e.onLoad(i)
                        } catch (t) {
                            l.onError(e, t)
                        }
                    }, this.loadJavaScriptFromString = function(e, t, i) {
                        try {
                            var n = new Blob([t], {
                                    type: "application/javascript"
                                }),
                                r = {
                                    url: l.createObjectURL(n),
                                    name: e.scriptInfo.name,
                                    type: e.scriptInfo.type,
                                    version: e.scriptInfo.version
                                };
                            l.javaScriptLoader.get(r, function(t) {
                                i.parseMs = t.downloadMs, i.totalMs = l.calculateTotalLoadMs(i), e.onLoad(i)
                            }, e.onNonFatal, function(t) {
                                return l.onError(e, t)
                            })
                        } catch (t) {
                            l.onError(e, t)
                        }
                    }, this.loadJavaScriptFromOrigin = function(e) {
                        l.javaScriptLoader.get(e.scriptInfo, e.onLoad, e.onNonFatal, e.onFatal, !0)
                    }, this.loadJsonFromOrigin = function(e) {
                        l.urlAcquirer.get(e.scriptInfo.url, e.scriptInfo.crc32, l.XHR_TIMEOUT_MS, l.XHR_RETRIES, function(t, i) {
                            l.loadFromString(e, t, i)
                        }, function(t) {
                            e.onNonFatal(t)
                        }, function(t) {
                            e.onFatal(t)
                        })
                    }, this.javaScriptLoader = e, this.urlAcquirer = t, this.offlineStorage = i, this.loadFromStringFunctions = {}, this.loadFromStringFunctions[s.JAVASCRIPT] = this.loadJavaScriptFromString, this.loadFromStringFunctions[s.JSON] = this.loadJsonFromString, this.loadFromOriginFunctions = {}, this.loadFromOriginFunctions[s.JAVASCRIPT] = this.loadJavaScriptFromOrigin, this.loadFromOriginFunctions[s.JSON] = this.loadJsonFromOrigin, !this.isDefined(n) && this.isDefined(window.URL) ? this.createObjectURL = URL.createObjectURL : this.createObjectURL = n
                }
                return (e.prototype.getContent = function(e, t) {
                    var i = this.requiresUrlAcquirer(e.scriptInfo.type);
                    i && !this.urlAcquirer.isSupported() ? e.onFatal(new Error("URLAcquirer not supported but " + s[e.scriptInfo.type] + " requires it.")) : !i && this.onlySupportsOrigin() ? this.loadFromOrigin(e) : this.offlineStorage.get(e.scriptInfo.name, t)
                }, e.prototype.loadFromString = function(e, t, i) {
                    var n = this.loadFromStringFunctions[e.scriptInfo.type];
                    return this.isDefined(n) ? void n(e, t, i) : void e.onFatal(new Error("No loadFromString function found for " + s[e.scriptInfo.type]))
                }, e.prototype.loadFromOrigin = function(e) {
                    var t = this.loadFromOriginFunctions[e.scriptInfo.type];
                    return this.isDefined(t) ? void t(e) : void e.onFatal(new Error("No loadFromOrigin function found for " + s[e.scriptInfo.type]))
                }, e.prototype.acquireNewAsset = function(e, t, i) {
                    var n = this;
                    void 0 === i && (i = !0), this.urlAcquirer.get(e.scriptInfo.url, e.scriptInfo.crc32, this.XHR_TIMEOUT_MS, this.XHR_RETRIES, function(r, o) {
                        i && n.offlineStorage.set(e.scriptInfo.name, {
                            version: e.scriptInfo.version,
                            data: r
                        }), t && n.loadFromString(e, r, o)
                    }, function(t, i) {
                        e.onNonFatal(t, i)
                    }, function(i, r) {
                        t && n.onError(e, i, r)
                    })
                }, e.prototype.onError = function(e, t, i) {
                    e.onNonFatal(t, i), this.loadFromOrigin(e)
                }, e.prototype.requiresUrlAcquirer = function(e) {
                    return this.SCRIPT_TYPES_XHR_REQUIRED.indexOf(e) !== -1
                }, e.prototype.onlySupportsOrigin = function() {
                    return !(this.isDefined(window.Blob) && this.urlAcquirer.isSupported())
                }, e.prototype.isDefined = function(e) {
                    return "undefined" != typeof e
                }, e.prototype.nonNumberToZero = function(e) {
                    return "number" == typeof e ? e : 0
                }, e.prototype.calculateTotalLoadMs = function(e) {
                    return this.nonNumberToZero(e.cacheLoadMs) + this.nonNumberToZero(e.latencyMs) + this.nonNumberToZero(e.downloadMs) + this.nonNumberToZero(e.verifyMs) + this.nonNumberToZero(e.parseMs)
                }, e)
            }();
        t.exports = l
    }, {
        180: 180,
        245: 245,
        247: 247,
        248: 248
    }],
    245: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(e, t, i) {
                void 0 === e && (e = 12e4), void 0 === t && (t = 2), void 0 === i && (i = document);
                var n = this;
                this.scriptErrorCount = 0, this.scriptLoaded = !1, this.onScriptError = function(e, t, i) {
                    e && (e.onload = e.onerror = e.onreadystatechange = null), clearTimeout(n.scriptTimerId), n.objectExists(n.scriptInfo.name) || (++n.scriptErrorCount < n.maxAttempts ? (n.onNonFatal(t), n.setScript(i)) : n.onFatal(t))
                }, this.scriptTimeoutMs = e, this.maxAttempts = t, this.documentInstance = i
            }
            return (e.prototype.get = function(e, t, i, n, r) {
                void 0 === r && (r = !1), this.scriptInfo = e, this.onLoad = t, this.onNonFatal = i, this.onFatal = n, this.scriptErrorCount = 0, this.scriptLoaded = !1, this.bustCache = r, clearTimeout(this.scriptTimerId), this.setScript(this.scriptInfo.url)
            }, e.prototype.setScript = function(e) {
                var t = this,
                    i = this.documentInstance.createElement("script");
                this.scriptTimerId = setTimeout(function() {
                    t.onScriptError(i, new Error("Script load timed out. " + t.scriptInfo.name + " for " + e), e)
                }, this.scriptTimeoutMs);
                var n = (new Date).getTime();
                i.setAttribute("crossorigin", "anonymous"), i.onload = i.onreadystatechange = function() {
                    if (!t.scriptLoaded && ("undefined" == typeof i.readyState || "loaded" === i.readyState || "complete" === i.readyState))
                        if (t.objectExists(t.scriptInfo.name)) {
                            clearTimeout(t.scriptTimerId), t.scriptLoaded = !0;
                            var r = (new Date).getTime() - n;
                            i.onload = i.onerror = i.onreadystatechange = null, t.onLoad({
                                cached: !1,
                                downloadMs: r,
                                totalMs: r
                            })
                        } else t.onScriptError(i, new Error("Script loaded but missing result object. Expected " + t.scriptInfo.name + " for " + e), e)
                }, i.onerror = function() {
                    t.onScriptError(i, new Error("Script onerror fired: " + t.scriptInfo.name + " for " + e), e)
                };
                var r = this.bustCache ? "?t=" + (new Date).getTime() : "";
                i.src = e + r, this.documentInstance.getElementsByTagName("head")[0].appendChild(i)
            }, e.prototype.objectExists = function(e) {
                return "undefined" != typeof window[e]
            }, e)
        }();
        t.exports = n
    }, {}],
    246: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(244),
            o = function(e) {
                function t(t, i, n, r) {
                    return e.call(this, t, i, n, r) || this
                }
                return (n(t, e), t.prototype.get = function(e, t, i, n) {
                    var r = this,
                        o = {
                            scriptInfo: e,
                            onLoad: t,
                            onNonFatal: i,
                            onFatal: n
                        };
                    this.getContent(o, function(t, i) {
                        t ? (r.loadFromString(o, t.data, {
                            cached: !0,
                            cacheLoadMs: i
                        }), t.version !== e.version && r.acquireNewAsset(o, !1)) : r.acquireNewAsset(o, !0)
                    })
                }, t)
            }(r);
        t.exports = o
    }, {
        244: 244
    }],
    247: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.JAVASCRIPT = 0] = "JAVASCRIPT", e[e.JSON = 1] = "JSON"
        }(n || (n = {})), t.exports = n
    }, {}],
    248: [function(e, t, i) {
        "use strict";
        var n = e(282),
            r = "X-Server-IP",
            o = function() {
                function e(e, t) {
                    void 0 === e && (e = XMLHttpRequest), void 0 === t && (t = n.str);
                    var i = this;
                    this.makeXHRRequest = function(e, t, n, o, s, a, l, c) {
                        void 0 === c && (c = 0);
                        var u = "",
                            p = new i.XHR,
                            h = function(h) {
                                h || (h = {
                                    type: "unknown error"
                                });
                                var f = new Error("XHR " + h.type + " for " + e + ". Status: " + p.status + ", readyState: " + p.readyState),
                                    d = {
                                        attempt: c,
                                        cloudfrontRequestID: i.getHeaderIfAvailable("x-amz-cf-id", u, p),
                                        contentLength: h.contentLength,
                                        contentLengthHeader: i.getHeaderIfAvailable("Content-Length", u, p),
                                        contentHash: h.contentHash,
                                        serverIP: i.getHeaderIfAvailable(r, u, p)
                                    };
                                o > 0 ? (a(f, d), i.makeXHRRequest(e, t, n, --o, s, a, l, c + 1)) : l(f, d)
                            },
                            f = i.now(),
                            d = f;
                        p.addEventListener("readystatechange", function() {
                            2 === p.readyState && (d = i.now())
                        }), p.addEventListener("load", function() {
                            if (u = p.getAllResponseHeaders(), 200 === p.status) {
                                var e = p.responseText,
                                    n = i.now(),
                                    o = !0,
                                    a = null,
                                    l = "(not checked)";
                                if (t) {
                                    var c = i.now();
                                    l = "" + i.toCRC32(e), o = l === t, a = i.now() - c
                                }
                                if (o) s(e, {
                                    cached: !1,
                                    latencyMs: d - f,
                                    downloadMs: n - d,
                                    verifyMs: a,
                                    serverIP: i.getHeaderIfAvailable(r, u, p)
                                });
                                else {
                                    var y = e ? e.length : 0,
                                        g = y > 0 ? "[" + e.substr(0, 15) + "]" : "(empty)";
                                    h({
                                        type: "verify failed " + g,
                                        contentLength: y,
                                        contentHash: l
                                    })
                                }
                            } else h({
                                type: "bad response"
                            })
                        }), p.addEventListener("error", h), p.addEventListener("timeout", h);
                        var y = 0 === c ? "" : "?t=" + i.now();
                        p.open("GET", e + y), p.timeout = n, p.send()
                    }, this.XHR = e, this.toCRC32 = t
                }
                return (e.prototype.isSupported = function() {
                    return "withCredentials" in new this.XHR
                }, e.prototype.get = function(e, t, i, n, r, o, s) {
                    this.makeXHRRequest(e, t, i, n, r, o, s)
                }, e.prototype.now = function() {
                    return (new Date).getTime()
                }, e.prototype.getHeaderIfAvailable = function(e, t, i) {
                    return t.indexOf(e) !== -1 ? i.getResponseHeader(e) : null
                }, e)
            }();
        t.exports = o
    }, {
        282: 282
    }],
    249: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(244),
            o = function(e) {
                function t(t, i, n, r) {
                    return e.call(this, t, i, n, r) || this
                }
                return (n(t, e), t.prototype.get = function(e, t, i, n, r) {
                    var o = this;
                    void 0 === r && (r = !0);
                    var s = {
                        scriptInfo: e,
                        onLoad: t,
                        onNonFatal: i,
                        onFatal: n
                    };
                    r ? this.getContent(s, function(t, i) {
                        t && t.version === e.version ? o.loadFromString(s, t.data, {
                            cached: !0,
                            cacheLoadMs: i
                        }) : o.acquireNewAsset(s, !0)
                    }) : this.onlySupportsOrigin() ? this.loadFromOrigin(s) : this.acquireNewAsset(s, !0, !1)
                }, t)
            }(r);
        t.exports = o
    }, {
        244: 244
    }],
    250: [function(e, t, i) {
        "use strict";
        var n = e(246),
            r = e(249),
            o = e(245),
            s = e(247),
            a = {
                LazyCachingScriptLoader: n,
                TransparentCachingScriptLoader: r,
                JavaScriptLoader: o,
                ScriptType: s
            };
        t.exports = a
    }, {
        245: 245,
        246: 246,
        247: 247,
        249: 249
    }],
    251: [function(e, t, i) {
        "use strict";
        var n = e(92),
            r = n.CappedGeometricSequence,
            o = e(284),
            s = e(93),
            a = e(281),
            l = e(294),
            c = a.of("ExponentialBackoffRetry"),
            u = o.get("ExponentialBackoffRetry"),
            p = function() {
                function e(e) {
                    void 0 === e && (e = new r);
                    var t = this;
                    this.isAborted = !1, this.handleRetry = function(e, i) {
                        if (t.isAborted) return void u.error("Attempted to retry after aborted");
                        if (t.currentAttempt++, t.canStillRetry()) {
                            var n = t.delaySequence.getValue(t.currentAttempt);
                            0 !== n ? t.retryTimeoutId = setTimeout(function() {
                                return t.executeCode()
                            }, n) : t.executeCode()
                        } else s.isNullOrUndefined(e) && (e = new Error("Reached retries limit, and callable did not specify a more specific error")), t.handleFail(e, i)
                    }, this.handleSuccess = function(e) {
                        return t.isAborted ? void u.error("Attempted to succeed after aborted") : void l.forEach(t.successCallbacks, function(t) {
                            t(e)
                        })
                    }, this.handleFail = function(e, i) {
                        return t.isAborted ? void u.error("Attempted to fail after aborted") : void l.forEach(t.errorCallbacks, function(t) {
                            t(e, i)
                        })
                    }, this.maxAttempts = -1, this.maxElapsedTimeMs = -1, this.delaySequence = e, this.started = !1, this.currentAttempt = 0, this.successCallbacks = [], this.errorCallbacks = []
                }
                return (e.prototype.withMaxAttempts = function(e) {
                    return (c(!this.started, "!this.started").is["true"](), c(e, "maxAttempts").exists(), c(e >= 1, "maxAttempts >= 1").is["true"](), this.maxAttempts = e, this)
                }, e.prototype.withMaxElapsedTimeMs = function(e) {
                    return (c(!this.started, "!this.started").is["true"](), c(e, "maxElapsedTimeMs").exists(), c(e > 0, "maxElapsedTimeMs > 0").is["true"](), this.maxElapsedTimeMs = e, this)
                }, e.prototype.withInitialIntervalMs = function(e) {
                    return (c(!this.started, "!this.started").is["true"](), c(e, "initialIntervalMs").exists(), c(e >= 0, "initialIntervalMs >= 0").is["true"](), this.delaySequence.withInitialValue(e), this)
                }, e.prototype.withExponentialFactor = function(e) {
                    return (c(!this.started, "!this.started").is["true"](), c(e, "exponentialFactor").exists(), c(e > 0, "exponentialFactor > 0").is["true"](), this.delaySequence.withExponentialFactor(e), this)
                }, e.prototype.withRandomizationFactor = function(e) {
                    return (c(!this.started, "!this.started").is["true"](), c(e, "randomizationFactor").exists(), c(e >= 0, "randomizationFactor >= 0").is["true"](), this.delaySequence.withRandomizationFactor(e), this)
                }, e.prototype.withMaxDelayMs = function(e) {
                    return (c(!this.started, "!this.started").is["true"](), c(e, "maxDelayMs").exists(), c(e > 0, "maxDelayMs > 0").is["true"](), this.delaySequence.withMaxValue(e), this)
                }, e.prototype.onSuccess = function(e) {
                    return (c(!this.started, "!this.started").is["true"](), c(e, "callback").exists(), this.successCallbacks = l.union(this.successCallbacks, [e]), this)
                }, e.prototype.onError = function(e) {
                    return (c(!this.started, "!this.started").is["true"](), c(e, "callback").exists(), this.errorCallbacks = l.union(this.errorCallbacks, [e]), this)
                }, e.prototype.onAttempt = function(e) {
                    return (c(!this.started, "!this.started").is["true"](), c(e, "callable").exists(), this.callable = e, this)
                }, e.prototype.execute = function() {
                    return (c(!this.started, "!this.started").is["true"](), c(this.callable, "this.callable").exists(), this.started = !0, this.startTime = (new Date).getTime(), this.executeCode(), this)
                }, e.prototype.abort = function() {
                    this.isAborted = !0, clearTimeout(this.retryTimeoutId)
                }, e.prototype.executeCode = function() {
                    try {
                        this.callable(this.currentAttempt, this.handleRetry, this.handleSuccess, this.handleFail)
                    } catch (e) {
                        this.handleFail(e)
                    }
                }, e.prototype.canStillRetry = function() {
                    return !(this.maxAttempts !== -1 && this.currentAttempt >= this.maxAttempts || this.maxElapsedTimeMs !== -1 && (new Date).getTime() - this.startTime >= this.maxElapsedTimeMs)
                }, e)
            }();
        t.exports = p
    }, {
        281: 281,
        284: 284,
        294: 294,
        92: 92,
        93: 93
    }],
    252: [function(e, t, i) {
        "use strict";
        var n = e(251),
            r = {
                ExponentialBackoffRetry: n
            };
        t.exports = r
    }, {
        251: 251
    }],
    253: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = function() {
                function e(t, i, r, o, s, a, l, c, u, p) {
                    void 0 === t && (t = window.setImmediate), void 0 === i && (i = window.clearImmediate), void 0 === r && (r = window.msSetImmediate), void 0 === o && (o = window.msClearImmediate), void 0 === s && (s = window.postMessage), void 0 === a && (a = window.addEventListener), void 0 === l && (l = window.setTimeout), void 0 === c && (c = window.clearTimeout), void 0 === u && (u = "setImmediate-" + Math.random()), void 0 === p && (p = window.location);
                    var h = this;
                    if (this.pendingHandles = {}, this.nextId = 0, this.postMessageSetImmediate = function(e) {
                            for (var t = [], i = 1; i < arguments.length; i++) t[i - 1] = arguments[i];
                            if (n.isFunction(e)) {
                                var r = h.nextId++;
                                return (h.pendingHandles[r] = h.wrapHandler(e, t), h.postMessage.call(window, h.messageIdentifier + r, h.origin), r)
                            }
                        }, this.postMessageClearImmediate = function(e) {
                            delete h.pendingHandles[e]
                        }, this.onPostMessage = function(e) {
                            if (n.isString(e.data) && 0 === e.data.indexOf(h.messageIdentifier)) {
                                var t = e.data.substr(h.messageIdentifier.length),
                                    i = h.pendingHandles[t];
                                i && i(), h.postMessageClearImmediate(+t)
                            }
                        }, this.timeoutSetImmediate = function(e) {
                            for (var t = [], i = 1; i < arguments.length; i++) t[i - 1] = arguments[i];
                            return h.setTimeout(h.wrapHandler(e, t), 0)
                        }, this.timeoutClearImmediate = function(e) {
                            return h.clearTimeout(e)
                        }, t && i) this.setImmediate = t.bind(window), this.clearImmediate = i.bind(window);
                    else if (r && o) this.setImmediate = r.bind(window), this.clearImmediate = o.bind(window);
                    else if (s && a) {
                        if (this.messageIdentifier = u, this.postMessage = s, p.origin) this.origin = p.origin;
                        else {
                            var f = p.port ? ":" + p.port : "";
                            this.origin = p.protocol + "//" + p.hostname + f
                        }
                        a(e.MESSAGE_EVENT, this.onPostMessage), this.setImmediate = this.postMessageSetImmediate, this.clearImmediate = this.postMessageClearImmediate
                    } else this.setTimeout = l, this.clearTimeout = c, this.setImmediate = this.timeoutSetImmediate, this.clearImmediate = this.timeoutClearImmediate
                }
                return (e.prototype.wrapHandler = function(e, t) {
                    return function() {
                        return e.apply(void 0, t)
                    }
                }, e.MESSAGE_EVENT = "message", e)
            }();
        t.exports = r
    }, {
        294: 294
    }],
    254: [function(e, t, i) {
        "use strict";
        var n = e(253),
            r = function() {
                function e() {}
                return (e.getInstance = function() {
                    return (this.immediateExecutor || (this.immediateExecutor = new n), this.immediateExecutor)
                }, e)
            }();
        t.exports = r
    }, {
        253: 253
    }],
    255: [function(e, t, i) {
        "use strict";
        var n = e(231),
            r = n.NotificationEvent,
            o = e(113),
            s = o.LocalStorage,
            a = function() {
                function e(t) {
                    void 0 === t && (t = new s);
                    var i = this;
                    this.setDontShowAgain = function() {
                        i.storage.set(e.DONT_SHOW_AGAIN_KEY, "true")
                    }, this.storage = t
                }
                return (e.prototype.createNotification = function() {
                    return new r(this.getTemplate(), null, !0, null, this.setDontShowAgain)
                }, e.prototype.shouldShowNotification = function() {
                    return "true" !== this.storage.get(e.DONT_SHOW_AGAIN_KEY)
                }, e.prototype.getTemplate = function() {
                    return {
                        renderString: function(t, i) {
                            return t[e.MESSAGE_KEY]
                        }
                    }
                }, e.MESSAGE_KEY = "silverlightForcedNarratives", e.DONT_SHOW_AGAIN_KEY = "dont_show_forced_narratives", e)
            }();
        t.exports = a
    }, {
        113: 113,
        231: 231
    }],
    256: [function(e, t, i) {
        "use strict";
        var n = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            r = e(113),
            o = r.Version,
            s = e(272),
            a = s.BrowserType,
            l = e(287),
            c = e(231),
            u = c.NotificationEvent,
            p = function(e) {
                function t(i) {
                    void 0 === i && (i = s.getBrowser());
                    var n = this,
                        r = null,
                        o = !1;
                    return (t.isSafari10OrHigher(i) || (r = t.getDetailsTemplate(i), o = !0), n = e.call(this, t.getSubjectTemplate(i), r, o, null) || this)
                }
                return (n(t, e), t.getSubjectTemplate = function(e) {
                    return {
                        renderString: function(i, n) {
                            if (t.isSafari10OrHigher(e)) return l.render(i[t.SAFARI_10_KEY], n);
                            var r = "{{{message}}}";
                            return l.render(r, i[t.MESSAGE_KEY])
                        }
                    }
                }, t.getDetailsTemplate = function(e) {
                    var i = t.isChrome(e.getType());
                    return {
                        renderString: function(e, n) {
                            var r = e[t.MESSAGE_KEY],
                                o = r[t.CHROME_UNBLOCK_KEY],
                                s = l.render(r[t.TROUBLESHOOT_KEY], n),
                                a = "{{#isChrome}}" + ("{{{" + t.CHROME_UNBLOCK_KEY + "}}}<br />") + "{{/isChrome}}" + ("{{{" + t.TROUBLESHOOT_KEY + "}}}");
                            return l.render(a, (c = {
                                isChrome: i
                            }, c[t.CHROME_UNBLOCK_KEY] = o, c[t.TROUBLESHOOT_KEY] = s, c));
                            var c
                        }
                    }
                }, t.isChrome = function(e) {
                    return e === a.Chrome || e === a.Chromium
                }, t.isSafari10OrHigher = function(e) {
                    return e.getType() === a.Safari && e.getVersion().compareTo(o.fromNumber(t.SAFARI_10_VERSION)) >= 0
                }, t.CHROME_UNBLOCK_KEY = "chromeUnblock", t.MESSAGE_KEY = "silverlightBlocked", t.SAFARI_10_KEY = "silverlightBlockedSafari10", t.TROUBLESHOOT_KEY = "troubleshoot", t.SAFARI_10_VERSION = 10, t)
            }(u);
        t.exports = p
    }, {
        113: 113,
        231: 231,
        272: 272,
        287: 287
    }],
    257: [function(e, t, i) {
        "use strict";
        var n = e(231),
            r = n.NotificationEvent,
            o = e(126),
            s = e(272),
            a = s.BrowserType,
            l = s.OperatingSystem,
            c = s.OperatingSystemFamily,
            u = function() {
                function e(e) {
                    void 0 === e && (e = o.getInstance()), this.deviceInformation = e, this.isFirefoxAvailable = this.isFirefoxHTML5Available(this.deviceInformation)
                }
                return (e.prototype.createNotification = function() {
                    return new r(this.getTemplate(this.isFirefoxAvailable), null, !0, null, null)
                }, e.prototype.shouldShow = function(e) {
                    var t = this.deviceInformation.operatingSystem;
                    return !e && !t.equals(l.WINDOWS_XP) && !t.equals(l.WINDOWS_VISTA)
                }, e.prototype.getTemplate = function(t) {
                    return {
                        renderString: function(i, n) {
                            return i[t ? e.FIREFOX_MESSAGE_KEY : e.MESSAGE_KEY]
                        }
                    }
                }, e.prototype.isFirefoxHTML5Available = function(e) {
                    var t = e.browser,
                        i = e.operatingSystem;
                    return !(i.getFamily() !== c.Windows && i.getFamily() !== c.Mac || t.getType() !== a.Firefox && t.getType() !== a.Firefox64)
                }, e.MESSAGE_KEY = "switchToHtml5", e.FIREFOX_MESSAGE_KEY = "updateFirefox", e)
            }();
        t.exports = u
    }, {
        126: 126,
        231: 231,
        272: 272
    }],
    258: [function(e, t, i) {
        "use strict";
        var n = {};
        n._silverlightCount = 0, n.createObject = function(e, t, i, r, o, s, a) {
            var l = new Object,
                c = r,
                u = o;
            l.version = c.version, c.source = e, l.alt = c.alt, s && (c.initParams = s), c.isWindowless && !c.windowless && (c.windowless = c.isWindowless), c.framerate && !c.maxFramerate && (c.maxFramerate = c.framerate), i && !c.id && (c.id = i), delete c.ignoreBrowserVer, delete c.inplaceInstallPrompt, delete c.version, delete c.isWindowless, delete c.framerate, delete c.data, delete c.src, delete c.alt;
            for (var p in u)
                if (u[p]) {
                    if ("onLoad" === p && "function" == typeof u[p] && 1 !== u[p].length) {
                        var h = u[p];
                        u[p] = function(e) {
                            return h(document.getElementById(i), a, e)
                        }
                    }
                    var f = n.__getHandlerName(u[p]);
                    if (null === f) throw "typeof events." + p + " must be 'function' or 'string'";
                    c[p] = f, u[p] = null
                } var d = n.buildHTML(c);
            return t ? void(t.innerHTML = d) : d
        }, n.buildHTML = function(e) {
            var t = [];
            t.push('<object type="application/x-silverlight" data="data:application/x-silverlight,"'), null !== e.id && t.push(' id="' + n.HtmlAttributeEncode(e.id) + '"'), null !== e.width && t.push(' width="' + e.width + '"'), null !== e.height && t.push(' height="' + e.height + '"'), t.push(" >"), delete e.id, delete e.width, delete e.height;
            for (var i in e) e[i] && t.push('<param name="' + n.HtmlAttributeEncode(i) + '" value="' + n.HtmlAttributeEncode(e[i]) + '" />');
            return (t.push("</object>"), t.join(""))
        }, n.createObjectEx = function(e) {
            var t = e,
                i = n.createObject(t.source, t.parentElement, t.id, t.properties, t.events, t.initParams, t.context);
            if (null === t.parentElement) return i
        }, n.HtmlAttributeEncode = function(e) {
            var t, i = "";
            if (null === e) return null;
            for (var n = 0; n < e.length; n++) t = e.charCodeAt(n), t > 96 && t < 123 || t > 64 && t < 91 || t > 43 && t < 58 && 47 !== t || 95 === t ? i += String.fromCharCode(t) : i = i + "&#" + t + ";";
            return i
        }, n.__cleanup = function() {
            for (var e = n._silverlightCount - 1; e >= 0; e--) window["__slEvent" + e] = null;
            n._silverlightCount = 0, window.removeEventListener ? window.removeEventListener("unload", n.__cleanup, !1) : window.detachEvent("onunload", n.__cleanup)
        }, n.__getHandlerName = function(e) {
            var t = "";
            if ("string" == typeof e) t = e;
            else if ("function" == typeof e) {
                0 === n._silverlightCount && (window.addEventListener ? window.addEventListener("unload", n.__cleanup, !1) : window.attachEvent("onunload", n.__cleanup));
                var i = n._silverlightCount++;
                t = "__slEvent" + i, window[t] = e
            } else t = null;
            return t
        }, t.exports = n
    }, {}],
    259: [function(e, t, i) {
        "use strict";
        var n = e(82),
            r = e(284),
            o = e(281),
            s = e(258),
            a = e(262),
            l = e(261),
            c = e(260),
            u = o.of("SilverlightInitializer"),
            p = r.get("SilverlightInitializer"),
            h = function() {
                function e() {
                    var t = this;
                    this.monitoringTimer = null, this.pluginId = null, this.complete = !1, this.onMonitorCheck = function() {
                        t.isSilverlightPluginLoaded() && (t.clearMonitor(), t.eventEmitter.emit(a, new a), t.monitoringTimer = setTimeout(t.onXapLoadTimeout, e.PLAYER_LOAD_TIMEOUT))
                    }, this.onPlayerLoad = function() {
                        t.clearMonitor(), t.complete = !0, p.info("The XAP has successfully loaded"), t.eventEmitter.emit(l, new l)
                    }, this.onPlayerError = function(e, i) {
                        p.info("A fatal error has occurred during XAP initialization", i), t.complete || (t.clearMonitor(), t.complete = !0, t.eventEmitter.emit(c, new c(i.errorCode, i.errorType, i.errorMessage)))
                    }, this.onXapLoadTimeout = function() {
                        t.onPlayerError(null, {
                            errorCode: c.LOAD_TIMEOUT_ERROR_CODE
                        })
                    }, u(s, "Silverlight").exists(), this.eventEmitter = new n.BasicEventEmitter
                }
                return (e.prototype.initialize = function(t, i, n, r, o, a) {
                    u(t, "parentElement").exists(), u(i, "id").exists(), u(n, "xapUrl").exists(), u(r, "minVersion").exists(), u(o, "windowless").exists(), this.pluginId = i, this.complete = !1, this.clearMonitor();
                    var l = this.generateInitParamString(a);
                    this.monitoringTimer = setInterval(this.onMonitorCheck, e.SILVERLIGHT_PLUGIN_LOAD_POLL_INTERVAL), s.createObjectEx({
                        source: n,
                        parentElement: t,
                        id: i,
                        properties: {
                            windowless: "" + o,
                            width: "100%",
                            height: "100%",
                            minRuntimeVersion: r,
                            enableGPUAcceleration: "true",
                            background: "#000000",
                            enableHtmlAccess: "true"
                        },
                        initParams: l,
                        events: {
                            onLoad: this.onPlayerLoad,
                            onError: this.onPlayerError
                        }
                    })
                }, e.prototype.on = function(e, t) {
                    this.eventEmitter.on(e, t)
                }, e.prototype.once = function(e, t) {
                    this.eventEmitter.once(e, t)
                }, e.prototype.off = function(e, t) {
                    this.eventEmitter.off(e, t)
                }, e.prototype.stop = function() {
                    this.clearMonitor()
                }, e.prototype.clearMonitor = function() {
                    clearInterval(this.monitoringTimer), clearTimeout(this.monitoringTimer), this.monitoringTimer = null
                }, e.prototype.isSilverlightPluginLoaded = function() {
                    var e = document.getElementById(this.pluginId);
                    return null !== e && "undefined" != typeof e.isVersionSupported
                }, e.prototype.generateInitParamString = function(e) {
                    var t = new Array;
                    for (var i in e) e.hasOwnProperty(i) && "string" == typeof e[i] && null !== e[i] && t.push(i + "=" + e[i]);
                    return t.join(",")
                }, e.SILVERLIGHT_PLUGIN_LOAD_POLL_INTERVAL = 500, e.PLAYER_LOAD_TIMEOUT = 6e4, e)
            }();
        t.exports = h
    }, {
        258: 258,
        260: 260,
        261: 261,
        262: 262,
        281: 281,
        284: 284,
        82: 82
    }],
    260: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = function() {
                function e(e, t, i) {
                    this.errorCode = e, this.errorType = t, this.errorMessage = i
                }
                return (e.toString = function() {
                    return "silverlightPlayerLoadError"
                }, e.prototype.getErrorCode = function() {
                    return this.errorCode
                }, e.prototype.getErrorType = function() {
                    return this.errorType
                }, e.prototype.getErrorMessage = function() {
                    return this.errorMessage
                }, e.prototype.isCorruptionError = function() {
                    return n.contains([2103, 2104, 2105, 2150, 2152, 2153, e.LOAD_TIMEOUT_ERROR_CODE], this.errorCode)
                }, e.LOAD_TIMEOUT_ERROR_CODE = "sl_timeout", e)
            }();
        t.exports = r
    }, {
        294: 294
    }],
    261: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.toString = function() {
                return "silverlightPlayerLoaded"
            }, e)
        }();
        t.exports = n
    }, {}],
    262: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e() {}
            return (e.toString = function() {
                return "silverlightPluginLoaded"
            }, e)
        }();
        t.exports = n
    }, {}],
    263: [function(e, t, i) {
        "use strict";
        var n = e(203),
            r = n.ConsumptionType,
            o = n.Protocol,
            s = e(202),
            a = s.DesiredResources,
            l = s.LanguageFeature,
            c = s.PRSResponseValidator,
            u = s.ResourceUsage,
            p = s.VideoMaterialType,
            h = e(281),
            f = e(159),
            d = e(284),
            y = e(93),
            g = e(294),
            m = h.of("SilverlightResourceAcquirer"),
            v = d.get("SilverlightResourceAcquirer"),
            E = function() {
                function e(e, t, i, n) {
                    void 0 === i && (i = function(e) {
                        return btoa(e)
                    }), void 0 === n && (n = new c), this.encodeB64 = null, this.playbackResourceClient = m(e, "playbackResourceClient").exists(), this.configurationStore = m(t, "configurationStore").exists(), this.encodeB64 = i, this.prsResponseValidator = n
                }
                return (e.prototype.acquireLicense = function(e, t, i, n) {
                    var o = this;
                    m(e, "licenseChallenge").exists(), m(t, "asin").exists(), m(i, "onSuccess").exists(), m(n, "onError").exists();
                    var s = this.encodeB64(e);
                    this.request = this.playbackResourceClient.createRequest(t, r.Streaming, [a.PlayReadyLicense], u.ImmediateConsumption, p.Feature, this.configurationStore.getValue().userWatchSessionId).withPlayReadyChallenge(s), v.info("Requesting PlayReady license from PRS."), this.request.send(function(e) {
                        v.info("Got license from PRS."), o.request = null;
                        var t = o.validateLicenseResponse(e);
                        return null !== t ? (v.error("Bad license response.", t), void n(JSON.stringify(t))) : void i(e.playReadyLicense.encodedLicenseResponse)
                    }, function(e) {
                        v.error("Failed to retrieve license.", e), o.request = null, n(JSON.stringify(e))
                    })
                }, e.prototype.acquireResources = function(t, i, n, s) {
                    var a = this;
                    m(t, "asin").exists(), m(i, "videoMaterialType").exists(), m(n, "onSuccess").exists(), m(s, "onError").exists(), this.request = this.playbackResourceClient.createRequest(t, r.Streaming, e.DESIRED_RESOURCES, u.ImmediateConsumption, i, this.configurationStore.getValue().userWatchSessionId).withAudioTrackId(e.AUDIO_TRACK_ALL).withDeviceProtocolOverride(o.Https).withLanguageFeature(l.MLFv2).withTitleDecorationScheme(e.PLAYER_TITLE_DECORATION_SCHEME), v.info("Requesting playback resources from PRS."), this.request.send(function(e) {
                        v.info("Got desired playback resources from PRS."), a.request = null;
                        var t = a.prsResponseValidator.validateResponse(e);
                        return null !== t ? void s(JSON.stringify(t)) : void n(JSON.stringify(e), !g.isEmpty(e.forcedNarratives))
                    }, function(e) {
                        v.error("Failed to retrieve playback resources from PRS.", e), a.request = null, s(JSON.stringify(e))
                    })
                }, e.prototype.acquireNextEpisodeResources = function(t, i, n, s) {
                    var l = this;
                    m(t, "asin").exists(), m(i, "videoMaterialType").exists(), m(n, "onSuccess").exists(), m(s, "onError").exists(), this.request = this.playbackResourceClient.createRequest(t, r.Streaming, [a.CatalogMetadata], u.ImmediateConsumption, i, this.configurationStore.getValue().userWatchSessionId).withDeviceProtocolOverride(o.Https).withTitleDecorationScheme(e.PLAYER_TITLE_DECORATION_SCHEME), v.info("Requesting playback resources for next episode from PRS."), this.request.send(function(e) {
                        v.info("Got desired playback resources for next episode from PRS."), l.request = null, n(JSON.stringify(e))
                    }, function(e) {
                        v.error("Failed to retrieve playback resources for next episode from PRS.", e), l.request = null, s(JSON.stringify(e))
                    })
                }, e.prototype.abort = function() {
                    y.isNullOrUndefined(this.request) || this.request.abort()
                }, e.prototype.validateLicenseResponse = function(t) {
                    var i = y.nestedValue(t, "errorsByResource.PlayReadyLicense");
                    if (null !== i) return f.newError(e.DRM_ERROR, JSON.stringify(i));
                    if (null === y.nestedValue(t, "playReadyLicense.encodedLicenseResponse") || "" === t.playReadyLicense.encodedLicenseResponse) {
                        var n = y.nestedValue(t, "playReadyLicense.asinResponse.message");
                        return null === n ? f.newError(e.DRM_ERROR, "PlayReady license is missing from PRS response") : f.newError(e.DRM_ERROR, n)
                    }
                    return null
                }, e.AUDIO_TRACK_ALL = "all", e.DESIRED_RESOURCES = [a.AudioVideoUrls, a.CatalogMetadata, a.CuepointPlaylist, a.ForcedNarratives, a.SubtitlePresets, a.SubtitleUrls], e.DRM_ERROR = "DrmError", e.PLAYER_TITLE_DECORATION_SCHEME = "primary-content", e)
            }();
        t.exports = E
    }, {
        159: 159,
        202: 202,
        203: 203,
        281: 281,
        284: 284,
        294: 294,
        93: 93
    }],
    264: [function(e, t, i) {
        "use strict";
        var n = e(58),
            r = n.TokenType,
            o = e(231),
            s = o.AbortEvent,
            a = o.AsinChangedEvent,
            l = o.CloseEvent,
            c = o.DiagnosticsState,
            u = o.ErrorSeverity,
            p = o.Features,
            h = o.FirstFrameEvent,
            f = o.InitializedEvent,
            d = o.PINAcceptedEvent,
            y = o.PINDeclinedEvent,
            g = o.PINErrorEvent,
            m = o.PINRequiredEvent,
            v = o.PlayingEvent,
            E = o.PreparedEvent,
            _ = o.NotificationCancelEvent,
            S = o.NotificationEvent,
            T = o.VideoType,
            b = e(243),
            I = b.MetricsContextFactory,
            w = e(187),
            R = e(272),
            C = R.OperatingSystemFamily,
            P = e(265),
            A = e(80),
            O = A.DeviceTypeId,
            N = e(82),
            x = e(93),
            M = e(281),
            L = e(284),
            k = e(255),
            D = e(256),
            U = e(259),
            F = e(261),
            H = e(260),
            W = e(262),
            V = e(263),
            B = e(257),
            q = e(113),
            j = q.LocalStorage,
            z = e(202),
            G = z.PlaybackResourceClient,
            Y = z.VideoMaterialType,
            K = e(170),
            X = K.GetSectionsClient,
            Q = e(288),
            J = e(294),
            Z = M.of("SilverlightWebPlayer"),
            $ = L.get("SilverlightWebPlayer"),
            ee = "xap/ATVSilverlightWebPlayer.xap",
            te = 2e3,
            ie = "SilverlightAuthError",
            ne = "SilverlightCorruptionError",
            re = "PluginLoadBlocked",
            oe = "silverlightCorruptionMac",
            se = "silverlightCorruptionWindows",
            ae = "SilverlightStartupError",
            le = "SilverlightUnsupportedFeatureError",
            ce = "<ol>{{#steps}}<li>{{{.}}}</li>{{/steps}}</ol>",
            ue = "GetPlaybackResourcesError",
            pe = "GetPlaybackResourcesTime",
            he = "GetSectionsError",
            fe = "GetSectionsTime",
            de = "none",
            ye = "amazon",
            ge = "5.1",
            me = !1,
            ve = "THIRD_PARTY_SUBSCRIPTION",
            Ee = function() {
                function e(e, t, i, n, r, o, s, a, l) {
                    void 0 === r && (r = new U), void 0 === o && (o = new I), void 0 === s && (s = X), void 0 === a && (a = new j), void 0 === l && (l = document), this.Features = p, this.playerConfig = null, this.customerId = null, this.partnerServiceUrl = null, this.domElement = null, this.xapUrl = null, this.diagnostics = null, this.asin = null, this.startPosition = null, this.playTrailer = null, this.pinRequired = null, this.pinVerified = null, this.pluginBridge = null, this.currentPosition = 0, this.contentDuration = 1, this.spinnerPanel = null, this.getSectionsClient = null, this.playbackResourceClient = null, this.resourceAcquirer = null, this.tokenClient = null, this.loaderConfig = Z(e, "loaderConfig").exists(), this.qosReporter = Z(t, "qosReporter").exists(), this.spinnerPanel = i, this.createModalTemplate = n, this.initializer = r, this.metricsContextFactory = o, this.getSectionsClientType = s, this.eventEmitter = new N.BasicEventEmitter, this.storage = a, this.documentRef = l, this.os = R.getOperatingSystem(), this.browser = R.getBrowser(), this.isInitialized = !1, this.isPrepared = !1, this.isPluginLoading = !1, this.pluginBlockedNotification = null, this.silverlightWaitTimer = null, this.pluginLoadedCallback = null, this.playerLoadedCallback = null, this.playerLoadErrorCallback = null
                }
                return (e.prototype.initialize = function(e) {
                    this.playerConfig = Z(e, "playerConfig").exists(), Z(e.marketplaceId, "playerConfig.marketplaceId").exists(), Z(e.partnerServiceEndpoint, "playerConfig.partnerServiceEndpoint").exists(), Z(e.domElement, "playerConfig.domElement").exists(), Z(e.domElement.id, "playerConfig.domElement.id").exists(), Z(e.locale, "playerConfig.locale").exists(), Z(e.deviceId, "playerConfig.deviceId").exists(), Z(e.weblabs, "playerConfig.weblabs").exists(),
                        this.customerId = x.withDefault(e.customerId, null), this.tokenClient = x.withDefault(e.tokenClient, null), this.partnerServiceUrl = e.partnerServiceEndpoint.replace(/\/$/, "") + "/cdp/", this.domElement = this.documentRef.createElement("div"), this.domElement.className = "webPlayerElement", this.diagnostics = e.diagnostics !== c.Disabled;
                    var t = this.loaderConfig.dependencies.ATVWebPlayer,
                        i = this.loaderConfig.assetsRoot + "/" + t.name + "/" + t.version,
                        n = this.loaderConfig.dependencies.DVWebGlobalPlayer;
                    this.xapUrl = this.loaderConfig.assetsRoot + "/" + n.name + "/" + n.version + "/" + ee, 0 === location.protocol.indexOf("http:") && (this.xapUrl = this.xapUrl.replace("https:", "http:")), e.domElement.innerHTML = "", e.domElement.appendChild(this.domElement), this.spinnerPanel.initialize(e.domElement, i, !0, this.domElement), this.metricsContext = this.metricsContextFactory.createMetricsContext(e.partnerServiceEndpoint, e.deviceId, this.getDeviceTypeId(), e.marketplaceId, e.customerId, this.tokenClient), this.getSectionsClient = this.createGetSectionsClient(e), this.playbackResourceClient = new G(e.deviceId, this.getDeviceTypeId(), e.marketplaceId, e.partnerServiceEndpoint, this.createAuthenticationContext(e)), this.resourceAcquirer = new V(this.playbackResourceClient, e.configurationStore), this.isInitialized = !0, this.pinRequired = !1, this.pinVerified = !1, this.eventEmitter.emit(f)
                }, e.prototype.prepareCache = function(e) {}, e.prototype.prepare = function(e) {
                    Z(this.isInitialized, "this.isInitialized").is["true"](), Z(e, "contentConfig").exists(), Z(e.asin, "contentConfig.asin").exists(), this.asin = e.asin, this.startPosition = "number" == typeof e.position ? e.position : 0, this.playTrailer = e.videoType === T[T.Trailer], this.isPrepared = !0, this.qosReporter.setAsin(e.asin), this.metricsContext.contentPreparationStarted(this.asin, this.playTrailer), this.metricsContext.contentPreparationSucceeded(), this.eventEmitter.emit(E, new E)
                }, e.prototype.play = function(e) {
                    return (Z(e, "playbackConfig").exists(), Z(e.playbackInitiatedTime, "playbackConfig.playbackInitiatedTime").exists(), this.isPrepared ? null !== this.pluginBridge || this.isPluginLoading ? void $.warn("Ignoring call to play: play has already been called") : (this.metricsContext.prepareClickstream(this.playerConfig.sessionId, e.clickstreamRefMarker), this.metricsContext.playbackInitiated(e.playbackInitiatedTime), e = x.exists(e) ? e : {}, void this.loadPlugin(e.audioTrackId)) : void $.warn("Ignoring call to play: SilverlightWebPlayer has not been prepared"))
                }, e.prototype.playbackInitiated = function() {}, e.prototype.stop = function() {
                    this.isPluginLoading && (this.stopWaitingForSilverlight(), this.initializer.stop(), this.isPluginLoading = !1), null !== this.pluginBridge && ("function" == typeof this.pluginBridge.exitVideoPlayer && this.pluginBridge.exitVideoPlayer(), this.pluginBridge = null), null !== this.pluginLoadedCallback && (this.initializer.off(W, this.pluginLoadedCallback), this.pluginLoadedCallback = null), null !== this.playerLoadedCallback && (this.initializer.off(F, this.playerLoadedCallback), this.playerLoadedCallback = null), null !== this.playerLoadErrorCallback && (this.initializer.off(H, this.playerLoadErrorCallback), this.playerLoadErrorCallback = null), null !== this.domElement && (this.domElement.innerHTML = ""), this.asin = null, this.startPosition = null, this.playTrailer = null, this.isPrepared = !1, this.pinRequired = !1, this.pinVerified = !1, this.eventEmitter.emit(f)
                }, e.prototype.seek = function(e) {
                    Z(e, "position").exists(), Z(e >= 0, "position >= 0").is["true"](), null === this.pluginBridge ? this.isPrepared ? this.startPosition = e : $.warn("Player is not initialized, ignoring call to Seek") : this.pluginBridge.PositionSyncRequest(e + "")
                }, e.prototype.on = function(e, t) {
                    this.eventEmitter.on(e, t)
                }, e.prototype.once = function(e, t) {
                    this.eventEmitter.once(e, t)
                }, e.prototype.off = function(e, t) {
                    this.eventEmitter.off(e, t)
                }, e.prototype.getPosition = function() {
                    return this.currentPosition
                }, e.prototype.getDuration = function() {
                    return this.contentDuration
                }, e.prototype.getDeviceTypeId = function() {
                    return this.os.getFamily() === C.Mac ? O.SILVERLIGHT_MAC : O.SILVERLIGHT_PC
                }, e.prototype.validatePIN = function(e) {
                    "undefined" != typeof this.pluginBridge ? (this.metricsContext.pinSubmitted(), this.pluginBridge.ValidatePIN(e)) : $.warn("Player is not initialized, ignoring call to ValidatePin")
                }, e.prototype.getFeature = function(e, t, i) {
                    i(new Error("The Silverlight player does not support any features."))
                }, e.prototype.generateInitParameters = function(e) {
                    var t = {};
                    return (t.host = ye, t.customer = this.customerId, t.marketplace = this.playerConfig.marketplaceId, t.culture = this.playerConfig.locale, t.deviceId = this.playerConfig.deviceId, t.partnerService = this.partnerServiceUrl, t.diag = "" + this.diagnostics, t.auto_play = "false", t.userAgent = this.browser.toString(), t.asin = this.asin, t.v2api = "true", t.playTrailer = this.playTrailer ? "1" : "0", t.timeCode = "" + this.startPosition, t.clientVersion = this.loaderConfig.dependencies.DVWebGlobalPlayer.version, t.weblabs = P.stringify(this.playerConfig.weblabs), t.userWatchSessionId = this.playerConfig.configurationStore.getValue().userWatchSessionId, t.primitiveSessionId = this.playerConfig.configurationStore.getValue().primitiveSessionId, t.amazonSessionId = this.playerConfig.sessionId, "string" == typeof e && (t.audiotrackid = e), t)
                }, e.prototype.createPluginEventListeners = function() {
                    var e = this;
                    this.pluginBridge.OnAcquireLicense = function(t, i, n) {
                        e.onAcquireLicense(t, i, n)
                    }, this.pluginBridge.OnChangeASIN = function(t) {
                        e.onASINChange(t)
                    }, this.pluginBridge.OnClosePlayer = function() {
                        e.onPlayerClose()
                    }, this.pluginBridge.OnContentPrepared = function(t, i) {
                        e.onContentPrepared(t, i)
                    }, this.pluginBridge.OnFirstFrame = function() {
                        return e.onFirstFrame()
                    }, this.pluginBridge.OnGetNextEpisodePlaybackResources = function(t, i, n) {
                        e.onGetNextPlaybackResources(t, i, n)
                    }, this.pluginBridge.OnGetPlaybackResources = function(t, i, n) {
                        e.onGetPlaybackResources(t, i, n)
                    }, this.pluginBridge.OnGetSections = function(t, i, n) {
                        e.onGetSections(t, i, n)
                    }, this.pluginBridge.OnMediaStateChanged = function(t) {
                        e.onMediaStateChanged(t)
                    }, this.pluginBridge.OnPINRequired = function(t, i, n) {
                        e.onPINRequired(t, i, n)
                    }, this.pluginBridge.OnPINAccepted = function() {
                        e.onPINAccepted()
                    }, this.pluginBridge.OnPINDeclined = function() {
                        e.onPINDeclined()
                    }, this.pluginBridge.OnPINError = function() {
                        e.onPINError()
                    }, this.pluginBridge.OnPlayerEvent = function(t, i) {
                        e.onPlayerEvent(t, i)
                    }, this.pluginBridge.OnTokenRequired = function(t) {
                        e.onTokenRequired(t)
                    }, this.pluginBridge.OnReloadPlayer = function() {
                        document.location.reload(!0)
                    }
                }, e.prototype.onTokenRequired = function(e) {
                    var t = this;
                    return ($.info("The Silverlight player emitted a TokenRequired event"), x.isNullOrUndefined(this.tokenClient) ? void e(null, null) : void this.tokenClient.getToken(function(t) {
                        e(t.token, t.type === r.OAUTH ? "oauth" : "website")
                    }, function(e) {
                        t.abort(ie, "An error occurred while trying to acquire an auth token: " + e, u.Fatal)
                    }))
                }, e.prototype.onPINError = function() {
                    $.info("Emitted a PINError event"), this.eventEmitter.emit(g)
                }, e.prototype.onPINRequired = function(e, t, i) {
                    $.info("Emitted a PINRequired event"), this.pinVerified ? $.info("Ignoring PINRequired event as the PIN has already been verified") : (this.pluginBridge.HideLoadingScreen(), this.pinRequired = !0, this.metricsContext.pinDialogDisplayed(), this.eventEmitter.emit(m, new m(e, t, i, !0)))
                }, e.prototype.onPINAccepted = function() {
                    $.info("Emitted a PINAccepted event"), this.metricsContext.pinAccepted(), this.pinRequired = !1, this.pinVerified = !0, this.pluginBridge.Play(), this.eventEmitter.emit(d), this.pluginBridge.ShowLoadingScreen()
                }, e.prototype.onPINDeclined = function() {
                    $.info("Emitted a PINDeclined event"), this.metricsContext.pinDeclined(), this.eventEmitter.emit(y)
                }, e.prototype.onPlayerEvent = function(e, t) {
                    "position" === e && "number" == typeof t ? this.currentPosition = t : "loaded" === e && "undefined" != typeof t && "number" == typeof t.duration && (this.contentDuration = t.duration)
                }, e.prototype.onMediaStateChanged = function(e) {
                    switch ($.info("Changed state to " + e), e) {
                        case "Playing":
                            this.eventEmitter.emit(v);
                            break;
                        case "Buffering":
                            break;
                        case "Paused":
                            break;
                        case "Ended":
                            this.eventEmitter.emit(l)
                    }
                }, e.prototype.onASINChange = function(e) {
                    $.info("Changed ASIN to " + e), "string" == typeof e && e.length > 0 && null !== this.pluginBridge ? (this.metricsContext.nextTitlePlaybackInitiated(J.now()), this.metricsContext.clickstreamNextTitle(), this.metricsContext.contentPreparationStarted(e, !1), this.metricsContext.contentPreparationSucceeded(), this.asin = e, this.eventEmitter.emit(a, new a(e))) : $.error("Unable to change to asin: " + e)
                }, e.prototype.onPlayerClose = function() {
                    $.info("Emitted a Close event"), this.eventEmitter.emit(l)
                }, e.prototype.loadPlugin = function(e) {
                    var t = this;
                    this.isPluginLoading = !0;
                    var i = this.generateInitParameters(e);
                    $.info("Initialization parameters:", i);
                    var n = "silverlight_player_" + Math.floor(1e4 * Math.random());
                    this.silverlightWaitTimer = setTimeout(function() {
                        return t.notifySilverlightNotLoaded()
                    }, te), this.pluginLoadedCallback = function() {
                        t.onSilverlightLoaded()
                    }, this.playerLoadedCallback = function() {
                        t.onPluginLoaded(n)
                    }, this.playerLoadErrorCallback = function(e) {
                        t.onPluginError(e)
                    }, this.initializer.on(W, this.pluginLoadedCallback), this.initializer.on(F, this.playerLoadedCallback), this.initializer.on(H, this.playerLoadErrorCallback), this.hideSpinner(), this.metricsContext.appLoadStarted(), this.initializer.initialize(this.domElement, n, this.xapUrl, ge, me, i)
                }, e.prototype.showSpinner = function() {
                    this.spinnerPanel.show()
                }, e.prototype.hideSpinner = function() {
                    this.spinnerPanel.hide()
                }, e.prototype.notifySilverlightNotLoaded = function() {
                    this.isPluginLoading && (this.pluginBlockedNotification = new D, this.eventEmitter.emit(S, this.pluginBlockedNotification), this.qosReporter.reportError(re, u.NonFatal, "Silverlight plugin blocked or possibly corrupted"))
                }, e.prototype.stopWaitingForSilverlight = function() {
                    null !== this.pluginBlockedNotification && (this.eventEmitter.emit(_, new _(this.pluginBlockedNotification)), this.pluginBlockedNotification = null), null !== this.silverlightWaitTimer && (clearInterval(this.silverlightWaitTimer), this.silverlightWaitTimer = null)
                }, e.prototype.onSilverlightLoaded = function() {
                    this.isPluginLoading && (this.stopWaitingForSilverlight(), this.showSpinner())
                }, e.prototype.onPluginLoaded = function(e) {
                    this.isPluginLoading = !1;
                    var t = this.metricsContext.appLoadCompleted();
                    this.stopWaitingForSilverlight(), this.hideSpinner();
                    var i = document.getElementById(e);
                    if (x.isNullOrUndefined(i)) return void this.abort(ae, "Unable to access the Silverlight plugin through the JavaScript bridge", u.Fatal);
                    var n = i.content;
                    if (x.isNullOrUndefined(n) || x.isNullOrUndefined(n.Bridge)) return void this.abort(ae, "Unable to access the Silverlight plugin through the JavaScript bridge", u.Fatal);
                    var r = new B;
                    r.shouldShow(this.playTrailer) && this.eventEmitter.emit(S, r.createNotification()), $.info("Successfully initialized"), this.pluginBridge = n.Bridge, this.createPluginEventListeners(), x.exists(t) ? this.pluginBridge.Prepare('{"playerLoadTime": ' + t + " }", !0) : this.pluginBridge.Prepare()
                }, e.prototype.onContentPrepared = function(e, t) {
                    $.info("Prepared ASIN " + e + " and is ready to play it."), this.pinRequired && !this.pinVerified || this.pluginBridge.Play()
                }, e.prototype.onPluginError = function(e) {
                    this.stopWaitingForSilverlight();
                    var t;
                    if (e.isCorruptionError()) {
                        t = "Unable to load Silverlight plugin: code: " + e.getErrorCode() + ", type: " + e.getErrorType() + ", message: " + e.getErrorMessage();
                        var i = void 0,
                            n = void 0;
                        this.os.getFamily() === C.Mac ? (i = oe, n = ce) : (i = se, n = this.createWindowsExtraText()), this.abort(ne, t, u.Fatal, this.createModalTemplate(i, n))
                    } else t = "Unable to load Silverlight plugin: code: " + e.getErrorCode() + ", type: " + e.getErrorType() + ", message: " + e.getErrorMessage(), this.abort(ae, t, u.Fatal)
                }, e.prototype.createWindowsExtraText = function() {
                    var e = "manual_repair_steps_" + Q.v4().replace(/-/g, ""),
                        t = "(function () {const e = document.getElementById('" + e + "');e.style.display = e.style.display === 'none' ? '' : 'none';})();";
                    return '<ol>{{#fixItToolSteps}}<li>{{{.}}}</li>{{/fixItToolSteps}}</ol>{{#manualRepairSteps}}{{{preLinkText}}}<a href="javascript:' + t + '">{{{linkText}}}</a>{{{postLinkText}}}<ol id="' + e + '" style="display:none;">{{#steps}}<li>{{{.}}}</li>{{/steps}}</ol>{{/manualRepairSteps}}'
                }, e.prototype.onFirstFrame = function() {
                    $.info("Emitted a FirstFrame event"), this.metricsContext.clickstreamPlaybackStarted();
                    var e = this.metricsContext.playbackStarted(!0);
                    return (this.eventEmitter.emit(h), {
                        ttff: e
                    })
                }, e.prototype.onAcquireLicense = function(e, t, i) {
                    $.info("Acquiring DRM license"), this.resourceAcquirer.acquireLicense(e, this.asin, function(e) {
                        t(e)
                    }, function(e) {
                        i(e)
                    })
                }, e.prototype.onGetNextPlaybackResources = function(e, t, i) {
                    var n = this;
                    $.info("Acquiring playback resources for the next episode"), this.acquireResourcesTimer = w.startTimer(pe), this.resourceAcquirer.acquireNextEpisodeResources(e, Y.Feature, function(e) {
                        n.qosReporter.reportInformation(pe, null, n.acquireResourcesTimer.end()), t(e)
                    }, function(e) {
                        w.putMetric(ue), i(e)
                    })
                }, e.prototype.hasUnsupportedFeature = function(e) {
                    var t = {};
                    try {
                        t = JSON.parse(e)
                    } catch (e) {}
                    var i = x.nestedValue(t, "returnedTitleRendition.timeShift"),
                        n = x.nestedValue(t, "cuepointPlaylist.encodedManifest"),
                        r = x.nestedValue(t, "returnedTitleRendition.selectedEntitlement"),
                        o = r.entitlementType === ve;
                    return o && x.exists(n) || J.some(i, function(e) {
                        return e.enabled === !1
                    })
                }, e.prototype.onGetPlaybackResources = function(e, t, i) {
                    var n = this;
                    $.info("Acquiring playback resources"), this.acquireResourcesTimer = w.startTimer(pe), this.resourceAcquirer.acquireResources(e, this.playTrailer ? Y.Trailer : Y.Feature, function(e, i) {
                        if (n.qosReporter.reportInformation(pe, null, n.acquireResourcesTimer.end()), n.hasUnsupportedFeature(e)) return void n.abort(le, "Title requires a feature which Silverlight doesn't support.", u.NonFatal);
                        if (i) {
                            n.pluginBridge.ToggleCaptions(!0);
                            var r = new k;
                            r.shouldShowNotification() && n.eventEmitter.emit(S, r.createNotification())
                        }
                        t(e)
                    }, function(e) {
                        w.putMetric(ue), i(e)
                    })
                }, e.prototype.onGetSections = function(e, t, i) {
                    $.info("Calling GetSections"), this.getSectionsClient.getSections(e, de, function(e, i) {
                        w.putMetric(fe, i, w.MILLISECONDS), t(JSON.stringify(e))
                    }, function(e, t) {
                        w.putMetric(he), x.exists(t) && w.putMetric(fe, t, w.MILLISECONDS), i(JSON.stringify(e))
                    })
                }, e.prototype.abort = function(e, t, i, n) {
                    this.eventEmitter.emit(s, new s(e, t, i, n))
                }, e.prototype.createAuthenticationContext = function(e) {
                    if (x.isNullOrUndefined(e.customerId)) return null;
                    var t = {
                        customerId: e.customerId,
                        tokenClient: e.tokenClient
                    };
                    return t
                }, e.prototype.createGetSectionsClient = function(e) {
                    return x.isNullOrUndefined(e.customerId) ? null : new this.getSectionsClientType(e.deviceId, this.getDeviceTypeId(), e.marketplaceId, e.partnerServiceEndpoint, {
                        customerId: e.customerId,
                        tokenClient: e.tokenClient
                    })
                }, e)
            }();
        t.exports = Ee
    }, {
        113: 113,
        170: 170,
        187: 187,
        202: 202,
        231: 231,
        243: 243,
        255: 255,
        256: 256,
        257: 257,
        259: 259,
        260: 260,
        261: 261,
        262: 262,
        263: 263,
        265: 265,
        272: 272,
        281: 281,
        284: 284,
        288: 288,
        294: 294,
        58: 58,
        80: 80,
        82: 82,
        93: 93
    }],
    265: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(281),
            o = e(80),
            s = o.WeblabTreatment,
            a = r.of("WeblabStringifier"),
            l = function() {
                function e() {}
                return (e.stringify = function(e) {
                    a(e, "weblabs").exists();
                    var t = [];
                    for (var i in e)
                        if (e.hasOwnProperty(i)) {
                            var r = e[i],
                                o = n.isNullOrUndefined(r) ? "C" : s[r];
                            t.push(i + ":" + o)
                        } return t.join(" ")
                }, e)
            }();
        t.exports = l
    }, {
        281: 281,
        80: 80,
        93: 93
    }],
    266: [function(e, t, i) {
        "use strict";
        var n = e(264),
            r = {
                SilverlightWebPlayer: n
            };
        t.exports = r
    }, {
        264: 264
    }],
    267: [function(e, t, i) {
        "use strict";
        var n = e(93),
            r = e(281),
            o = e(113),
            s = o.Version,
            a = e(268),
            l = r.of("Browser"),
            c = function() {
                function e(e, t) {
                    this.type = l(e, "type").exists(), this.version = n.isNullOrUndefined(t) ? null : t
                }
                return (e.fromUserAgent = function(t) {
                    for (var i = 0; i < e.REGEXES.length; i++) {
                        var n = e.REGEXES[i].regex.exec(t);
                        if (n) {
                            var r = n[1] ? s.fromString(n[1]) : null;
                            return new e(e.REGEXES[i].type, r)
                        }
                    }
                    return new e(a.Unknown)
                }, e.prototype.getType = function() {
                    return this.type
                }, e.prototype.getVersion = function() {
                    return this.version
                }, e.prototype.toString = function() {
                    var e = a[this.type];
                    return (null !== this.version && (e += " " + this.version), e)
                }, e.prototype.equals = function(e) {
                    return this.type === e.type && (null === this.version ? null === e.version : this.version.equals(e.version))
                }, e.UNKNOWN = new e(a.Unknown), e.REGEXES = [{
                    regex: /(?:Camino|Chimera|hpwOS|webOS|Play[Ss]tation)\//,
                    type: a.Unknown
                }, {
                    regex: /Silk\/(\d+[.0-9\-]+)/,
                    type: a.Silk
                }, {
                    regex: /(?:BlackBerry|BB10).*Version\/(\d+[.0-9]*)/,
                    type: a.BlackBerry
                }, {
                    regex: /(?:OPR\/|Opera |Opera.*Version\/)(\d+[.0-9]*)/,
                    type: a.Opera
                }, {
                    regex: /(?:Edg)\/(\d+[.0-9]*)/,
                    type: a.EdgeNext
                }, {
                    regex: /(?:Edge)\/(\d+[.0-9]*)/,
                    type: a.Edge
                }, {
                    regex: /(?:MSIE |Trident.*rv:)(\d+[.0-9]*)/,
                    type: a.IE
                }, {
                    regex: /Chromium\/(\d+[.0-9]*)/,
                    type: a.Chromium
                }, {
                    regex: /(?:Chrome|CriOS)\/(\d+[.0-9]*)/,
                    type: a.Chrome
                }, {
                    regex: /SeaMonkey\/(\d+[.0-9]*)/,
                    type: a.SeaMonkey
                }, {
                    regex: /x64;.*Firefox\/(\d+[.0-9]*)/,
                    type: a.Firefox64
                }, {
                    regex: /Firefox\/(\d+[.0-9]*)/,
                    type: a.Firefox
                }, {
                    regex: /(?:Amazon.com|Windowshop|Amazon)\/(\d+[.0-9]*)/,
                    type: a.AmazonWebView
                }, {
                    regex: /Android[ \-\/](\d+[.0-9a-z\-]*)/,
                    type: a.Android
                }, {
                    regex: /Version\/(\d+[.0-9]*).*Safari/,
                    type: a.Safari
                }, {
                    regex: /AppleWebKit\/(\d+[.0-9]*)/,
                    type: a.AppleWebKit
                }], e)
            }();
        t.exports = c
    }, {
        113: 113,
        268: 268,
        281: 281,
        93: 93
    }],
    268: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Unknown = 0] = "Unknown", e[e.AmazonWebView = 1] = "AmazonWebView", e[e.Android = 2] = "Android", e[e.AppleWebKit = 3] = "AppleWebKit", e[e.BlackBerry = 4] = "BlackBerry", e[e.Chrome = 5] = "Chrome", e[e.Chromium = 6] = "Chromium", e[e.Edge = 7] = "Edge", e[e.EdgeNext = 8] = "EdgeNext", e[e.Firefox = 9] = "Firefox", e[e.Firefox64 = 10] = "Firefox64", e[e.IE = 11] = "IE", e[e.Opera = 12] = "Opera", e[e.Safari = 13] = "Safari", e[e.SeaMonkey = 14] = "SeaMonkey", e[e.Silk = 15] = "Silk"
        }(n || (n = {})), t.exports = n
    }, {}],
    269: [function(e, t, i) {
        "use strict";
        var n, r = e(93),
            o = e(281),
            s = e(113),
            a = s.Version,
            l = e(270),
            c = e(271),
            u = (n = {}, n[l.Unknown] = "Unknown", n[l.Android] = "Android", n[l.BlackBerry] = "BlackBerry", n[l.ChromeOS] = "Chrome OS", n[l.FirefoxOS] = "Firefox OS", n[l.GoogleTV] = "Google TV", n[l.iOS] = "iOS", n[l.Linux] = "Linux", n[l.Mac] = "Mac OS X", n[l.MacPPC] = "Mac OS PPC", n[l.Windows] = "Windows", n[l.WindowsPhone] = "Windows Phone", n[l.WindowsRT] = "Windows RT", n[l.XboxOne] = "Xbox One", n),
            p = o.of("OperatingSystem"),
            h = function() {
                function e(e, t, i) {
                    this.family = p(e, "family").exists(), this.version = r.isNullOrUndefined(t) ? a.createUnknownVersion() : t, this.kernelVersion = r.isNullOrUndefined(i) ? this.version : i
                }
                return (e.fromUserAgent = function(t) {
                    for (var i = 0; i < e.REGEXES.length; i++) {
                        var n = e.REGEXES[i].regex.exec(t);
                        if (n) return e.REGEXES[i].handler(n)
                    }
                    return new e(l.Unknown)
                }, e.prototype.getFamily = function() {
                    return this.family
                }, e.prototype.getFamilyName = function() {
                    return u[this.family] || null
                }, e.prototype.getVersion = function() {
                    return this.version
                }, e.prototype.getKernelVersion = function() {
                    return this.kernelVersion
                }, e.prototype.toString = function() {
                    var e = u[this.family];
                    return (null === this.version || this.version.isUnknownVersion() || (e += " " + this.version), e)
                }, e.prototype.equals = function(e) {
                    return this.family === e.family && (null === this.version ? null === e.version : this.version.equals(e.version))
                }, e.UNKNOWN = new e(l.Unknown), e.MAC_OS_X_YOSEMITE = new e(l.Mac, a.fromString("10.10")), e.WINDOWS_2000 = new e(l.Windows, c.WINDOWS_2000), e.WINDOWS_XP = new e(l.Windows, c.WINDOWS_XP), e.WINDOWS_VISTA = new e(l.Windows, c.WINDOWS_VISTA), e.WINDOWS_7 = new e(l.Windows, c.WINDOWS_7), e.WINDOWS_8 = new e(l.Windows, c.WINDOWS_8), e.WINDOWS_8_1 = new e(l.Windows, c.WINDOWS_8_1), e.WINDOWS_10 = new e(l.Windows, c.WINDOWS_10), e.REGEXES = [{
                    regex: /Windows ?Mobile|Xbox(?!.*One)|Play[Ss]tation/,
                    handler: function() {
                        return new e(l.Unknown)
                    }
                }, {
                    regex: /Windows Phone(?: (?:OS[ \/])?(\d+[.0-9]*))?/,
                    handler: function(t) {
                        var i = t[1] ? a.fromString(t[1]) : null;
                        return new e(l.WindowsPhone, i)
                    }
                }, {
                    regex: /Xbox One(?: (?:OS[ \/])?(\d+[.0-9]*))?/,
                    handler: function(t) {
                        var i = t[1] ? a.fromString(t[1]) : null;
                        return new e(l.XboxOne, i)
                    }
                }, {
                    regex: /(?:BB10|BlackBerry)(?:.*Version\/(\d+[.0-9]*))?/,
                    handler: function(t) {
                        var i = t[1] ? a.fromString(t[1]) : null;
                        return new e(l.BlackBerry, i)
                    }
                }, {
                    regex: /GoogleTV(?:[ \/]([0-9a-b]+[.0-9]*))?/,
                    handler: function(t) {
                        var i = t[1] ? a.fromString(t[1]) : null;
                        return new e(l.GoogleTV, i)
                    }
                }, {
                    regex: /Android(?:[ \-\/](\d+[.0-9a-z\-]*))?|\bSilk\b/,
                    handler: function(t) {
                        var i = t[1] ? a.fromString(t[1]) : null;
                        return new e(l.Android, i)
                    }
                }, {
                    regex: /CrOS(?: i686 (\d+[.0-9]*))?/,
                    handler: function(t) {
                        var i = t[1] ? a.fromString(t[1]) : null;
                        return new e(l.ChromeOS, i)
                    }
                }, {
                    regex: /(?:iPad|iPhone|iPod)(?:.*OS (\d+[_.0-9]*))?/,
                    handler: function(t) {
                        var i = t[1] ? a.fromString(t[1].replace(/_/g, ".")) : null;
                        return new e(l.iOS, i)
                    }
                }, {
                    regex: /Intel Mac OS X(?: (\d+[_.0-9]*))?/,
                    handler: function(t) {
                        var i = t[1] ? a.fromString(t[1].replace(/_/g, ".")) : null;
                        return new e(l.Mac, i)
                    }
                }, {
                    regex: /PPC Mac OS(?: X)?(?: (\d+[_.0-9]*))?/,
                    handler: function(t) {
                        var i = t[1] ? a.fromString(t[1].replace(/_/g, ".")) : null;
                        return new e(l.MacPPC, i)
                    }
                }, {
                    regex: /Windows NT(?: (5\.[012]|6\.[01234]))?.*ARM/,
                    handler: function(t) {
                        var i = t[1],
                            n = i ? c.fromKernelVersion(i) : null,
                            r = i ? a.fromString(i) : null;
                        return new e(l.WindowsRT, n, r)
                    }
                }, {
                    regex: /Windows NT(?: (5\.[012]|6\.[01234]|10\.[012]))?/,
                    handler: function(t) {
                        var i = t[1],
                            n = i ? c.fromKernelVersion(i) : null,
                            r = i ? a.fromString(i) : null;
                        return new e(l.Windows, n, r)
                    }
                }, {
                    regex: /Linux/,
                    handler: function() {
                        return new e(l.Linux)
                    }
                }, {
                    regex: /Mozilla\/5.0 \((?:Mobile|Tablet); (?:[0-9A-Za-z .\-]+; )?rv:\d+[.0-9]*\) Gecko\/\d+[.0-9]* Firefox\/\d+[.0-9]*/,
                    handler: function() {
                        return new e(l.FirefoxOS)
                    }
                }], e)
            }();
        t.exports = h
    }, {
        113: 113,
        270: 270,
        271: 271,
        281: 281,
        93: 93
    }],
    270: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.Unknown = 0] = "Unknown", e[e.Android = 1] = "Android", e[e.BlackBerry = 2] = "BlackBerry", e[e.ChromeOS = 3] = "ChromeOS", e[e.FirefoxOS = 4] = "FirefoxOS", e[e.GoogleTV = 5] = "GoogleTV", e[e.iOS = 6] = "iOS", e[e.Linux = 7] = "Linux", e[e.Mac = 8] = "Mac", e[e.MacPPC = 9] = "MacPPC", e[e.Windows = 10] = "Windows", e[e.WindowsPhone = 11] = "WindowsPhone", e[e.WindowsRT = 12] = "WindowsRT", e[e.XboxOne = 13] = "XboxOne"
        }(n || (n = {})), t.exports = n
    }, {}],
    271: [function(e, t, i) {
        "use strict";
        var n, r = this && this.__extends || function() {
                var e = Object.setPrototypeOf || {
                    __proto__: []
                }
                instanceof Array && function(e, t) {
                    e.__proto__ = t
                } || function(e, t) {
                    for (var i in t) t.hasOwnProperty(i) && (e[i] = t[i])
                };
                return function(t, i) {
                    function n() {
                        this.constructor = t
                    }
                    e(t, i), t.prototype = null === i ? Object.create(i) : (n.prototype = i.prototype, new n)
                }
            }(),
            o = e(281),
            s = e(113),
            a = s.Version;
        ! function(e) {
            e[e.Windows2000 = 0] = "Windows2000", e[e.WindowsXP = 1] = "WindowsXP", e[e.WindowsVista = 2] = "WindowsVista", e[e.Windows7 = 3] = "Windows7", e[e.Windows8 = 4] = "Windows8", e[e.Windows8_1 = 5] = "Windows8_1", e[e.Windows10 = 6] = "Windows10"
        }(n || (n = {}));
        var l, c = {
                "5.0": n.Windows2000,
                5.1: n.WindowsXP,
                5.2: n.WindowsXP,
                "6.0": n.WindowsVista,
                6.1: n.Windows7,
                6.2: n.Windows8,
                6.3: n.Windows8_1,
                6.4: n.Windows10,
                "10.0": n.Windows10,
                10.1: n.Windows10,
                10.2: n.Windows10
            },
            u = (l = {}, l[n.Windows2000] = "2000", l[n.WindowsXP] = "XP", l[n.WindowsVista] = "Vista", l[n.Windows7] = "7", l[n.Windows8] = "8", l[n.Windows8_1] = "8.1", l[n.Windows10] = "10", l),
            p = o.of("WindowsVersion"),
            h = function(e) {
                function t(t) {
                    return e.call(this, [t], u[t]) || this
                }
                return (r(t, e), t.fromKernelVersion = function(e) {
                    return (p(e, "kernelVersion").exists(), p(c[e], "WINDOWS_NUMERICAL_VERSIONS[" + e + "]").exists(), new t(c[e]))
                }, t.WINDOWS_2000 = new t(n.Windows2000), t.WINDOWS_XP = new t(n.WindowsXP), t.WINDOWS_VISTA = new t(n.WindowsVista), t.WINDOWS_7 = new t(n.Windows7), t.WINDOWS_8 = new t(n.Windows8), t.WINDOWS_8_1 = new t(n.Windows8_1), t.WINDOWS_10 = new t(n.Windows10), t)
            }(a);
        t.exports = h
    }, {
        113: 113,
        281: 281
    }],
    272: [function(e, t, i) {
        "use strict";
        var n = e(267),
            r = e(268),
            o = e(269),
            s = e(270),
            a = e(271),
            l = {
                Browser: n,
                BrowserType: r,
                OperatingSystem: o,
                OperatingSystemFamily: s,
                WindowsVersion: a,
                getBrowser: function(e) {
                    return (void 0 === e && (e = navigator.userAgent), n.fromUserAgent(e))
                },
                getOperatingSystem: function(e) {
                    return (void 0 === e && (e = navigator.userAgent), o.fromUserAgent(e))
                }
            };
        t.exports = l
    }, {
        267: 267,
        268: 268,
        269: 269,
        270: 270,
        271: 271
    }],
    273: [function(e, t, i) {
        "use strict";
        var n;
        ! function(e) {
            e[e.TIMEOUT = 0] = "TIMEOUT", e[e.INTERVAL = 1] = "INTERVAL", e[e.CLEAR = 2] = "CLEAR"
        }(n || (n = {})), t.exports = n
    }, {}],
    274: [function(e, t, i) {
        "use strict";
        var n = e(284),
            r = n.get("WebWorkerBasedTimerClient"),
            o = e(294),
            s = e(273),
            a = '!function e(t,r,i){function n(o,a){if(!r[o]){if(!t[o]){var u="function"==typeof require&&require;if(!a&&u)return u(o,!0);if(s)return s(o,!0);var c=new Error("Cannot find module \'"+o+"\'");throw c.code="MODULE_NOT_FOUND",c}var f=r[o]={exports:{}};t[o][0].call(f.exports,function(e){var r=t[o][1][e];return n(r?r:e)},f,f.exports,e,t,r,i)}return r[o].exports}for(var s="function"==typeof require&&require,o=0;o<i.length;o++)n(i[o]);return n}({1:[function(e,t,r){"use strict";var i;!function(e){e[e.TIMEOUT=0]="TIMEOUT",e[e.INTERVAL=1]="INTERVAL",e[e.CLEAR=2]="CLEAR"}(i||(i={})),t.exports=i},{}],2:[function(e,t,r){"use strict";var i=e(1),n=function(){function e(e){var t=this;this.timerMap={},this.onMessage=function(e){var r=e.data;r.type===i.CLEAR?t.clearTimer(r.id):r.type===i.INTERVAL?t.setWorkerInterval(r):r.type===i.TIMEOUT?t.setWorkerTimeout(r):console.warn("Received unknown message "+r)},this.clearTimer=function(e){var t=this.timerMap[e];clearTimeout(t),delete this.timerMap[e]},this.sendMessage=function(e){t.postMessageOverride?t.postMessageOverride(e):postMessage(e)},this.setWorkerTimeout=function(e){var t=this,r=setTimeout(function(){t.sendMessage(e),delete t.timerMap[e.id]},e.timeout);this.timerMap[e.id]=r},this.setWorkerInterval=function(e){var t=this,r=setInterval(function(){t.sendMessage(e)},e.timeout);this.timerMap[e.id]=r},this.postMessageOverride=e}return e}();t.exports=n},{1:1}],3:[function(e,t,r){"use strict";Object.defineProperty(r,"__esModule",{value:!0});var i=e(2),n=new i;onmessage=n.onMessage},{2:2}]},{},[3]);',
            l = function() {
                function e() {
                    var e = this;
                    this.callbackMap = {}, this.lastId = 0, this.worker = null, this.onMessage = function(t) {
                        var i = t.data,
                            n = e.callbackMap[i.id];
                        o.isFunction(n) && n(), i.type === s.TIMEOUT && delete e.callbackMap[i.id]
                    }
                }
                return (e.prototype.initialize = function(e) {
                    if (o.isUndefined(e)) try {
                        var t = new Blob([a], {
                            type: "application/javascript"
                        });
                        this.worker = new Worker(URL.createObjectURL(t))
                    } catch (e) {
                        r.warn("Unable to create a WebWorkerTimer, failing over to window timers.")
                    } else this.worker = e;
                    null !== this.worker && this.worker.addEventListener("message", this.onMessage)
                }, e.prototype.setTimeout = function(e, t) {
                    return this.setTimer(e, t, s.TIMEOUT)
                }, e.prototype.setInterval = function(e, t) {
                    return this.setTimer(e, t, s.INTERVAL)
                }, e.prototype.clearTimeout = function(e) {
                    this.clearTimer(e)
                }, e.prototype.clearInterval = function(e) {
                    this.clearTimer(e)
                }, e.prototype.clearTimer = function(e) {
                    this.worker ? (delete this.callbackMap[e], this.worker.postMessage({
                        type: s.CLEAR,
                        id: e
                    })) : clearTimeout(e)
                }, e.prototype.setTimer = function(e, t, i) {
                    if (this.worker) {
                        var n = {
                            type: i,
                            id: ++this.lastId,
                            timeout: t
                        };
                        return (this.callbackMap[n.id] = e, this.worker.postMessage(n), n.id)
                    }
                    return i === s.INTERVAL ? setInterval(e, t) : i === s.TIMEOUT ? setTimeout(e, t) : void 0
                }, e)
            }();
        t.exports = l
    }, {
        273: 273,
        284: 284,
        294: 294
    }],
    275: [function(e, t, i) {
        "use strict";
        var n = e(274),
            r = new n;
        r.initialize(), t.exports = r
    }, {
        274: 274
    }],
    276: [function(e, t, i) {
        "use strict";
        var n = e(294),
            r = e(93),
            o = e(113),
            s = o.LocalStorage,
            a = "html5_web_player_weblab_cache",
            l = 6e5,
            c = function() {
                function e(e, t) {
                    void 0 === e && (e = new s), void 0 === t && (t = n.now), this.localStorage = e, this.now = t
                }
                return (e.prototype.cacheWeblabs = function(e) {
                    var t = {
                        weblabs: e,
                        cacheTimeMs: this.now()
                    };
                    this.localStorage.set(a, JSON.stringify(t))
                }, e.prototype.getCachedWeblabs = function() {
                    var e = this.localStorage.get(a),
                        t = JSON.parse(e);
                    return this.isValidCache(t) ? t.weblabs : (this.localStorage.remove(a), null)
                }, e.prototype.isValidCache = function(e) {
                    return r.exists(e) && e.cacheTimeMs + l > this.now()
                }, e)
            }();
        t.exports = c
    }, {
        113: 113,
        294: 294,
        93: 93
    }],
    277: [function(e, t, i) {
        "use strict";
        var n = e(58),
            r = n.TokenType,
            o = e(284),
            s = e(93),
            a = e(281),
            l = e(294),
            c = e(80),
            u = c.MarketplaceID,
            p = c.WeblabTreatment,
            h = e(178),
            f = h.HttpRequest,
            d = e(276),
            y = a.of("WeblabClient"),
            g = o.get("WeblabClient"),
            m = function() {
                function e(e, t) {
                    void 0 === e && (e = f), void 0 === t && (t = new d), this.httpRequest = e, this.weblabCache = t
                }
                return (e.prototype.initialize = function(t, i, n, r, o, s) {
                    this.deviceId = y(r, "deviceId").exists(), this.deviceTypeId = y(n, "deviceTypeId").exists(), this.marketplaceId = y(i, "marketplaceId").exists(), this.authContext = s, this.sessionId = o, this.url = y(t, "partnerServiceUrl").exists().replace(/\/$/, "") + e.SERVICE_ENDPOINT, this.initialized = !0
                }, e.prototype.getWeblabs = function(t, i, n) {
                    var o = this;
                    y(this.initialized, "initialized").is["true"](), y(t, "weblabs").exists(), y(i, "onSuccess").exists(), y(n, "onError").exists();
                    var a = this.weblabCache.getCachedWeblabs();
                    if (s.exists(a)) return (g.info("Using cached weblabs: ", a), void i(a));
                    if (0 !== t.length && this.httpRequest.isSupported()) {
                        var c = this.url,
                            p = {},
                            h = {
                                gascEnabled: u.isClassic(this.marketplaceId) ? "false" : "true",
                                firmware: "1",
                                deviceId: this.deviceId,
                                deviceTypeId: this.deviceTypeId,
                                marketplaceId: this.marketplaceId
                            };
                        s.exists(this.sessionId) && (p[e.SESSION_ID_HEADER] = this.sessionId);
                        var f = this.httpRequest.post(c).accept("application/json").timeout(e.HTTP_REQUEST_TIMEOUT_MS).headers(p).body(this.createBody(t)).onError(function(e, t) {
                            s.exists(t) ? n(e, t.getLatencyMs()) : n(e)
                        }).onLoad(function(e) {
                            var t = {},
                                r = e.getText();
                            if (s.exists(r)) {
                                var a = void 0;
                                try {
                                    a = JSON.parse(r)
                                } catch (t) {
                                    return void n(t, e.getLatencyMs())
                                }
                                if (s.exists(a) && s.exists(a.TreatmentList)) {
                                    var c = a.TreatmentList;
                                    g.info("Received Weblabs.", c), l.forEach(c, function(e) {
                                        var i = o.getTreatmentFromName(e.treatmentName);
                                        s.exists(i) && (t[e.weblabName] = i)
                                    })
                                }
                            }
                            o.weblabCache.cacheWeblabs(t), i(t, e.getLatencyMs())
                        }).withCredentials();
                        s.exists(this.authContext) && s.exists(this.authContext.tokenClient) ? this.authContext.tokenClient.getToken(function(e) {
                            e.type === r.OAUTH && f.bearerToken(e.token), f.query(h).send()
                        }, function(e) {
                            n(e)
                        }) : f.query(h).send()
                    } else i({})
                }, e.prototype.createBody = function(e) {
                    var t = "<WeblabTreatmentRequest><WeblabList>";
                    return (l.forEach(e, function(e) {
                        t += "<member>" + e + "</member>"
                    }), t + "</WeblabList></WeblabTreatmentRequest>")
                }, e.prototype.getTreatmentFromName = function(e) {
                    return s.withDefault(p[e], null)
                }, e.SESSION_ID_HEADER = "x-atv-session-id", e.SERVICE_ENDPOINT = "/cdp/usage/GetWeblabTreatment", e.HTTP_REQUEST_TIMEOUT_MS = 4e3, e)
            }();
        t.exports = m
    }, {
        178: 178,
        276: 276,
        281: 281,
        284: 284,
        294: 294,
        58: 58,
        80: 80,
        93: 93
    }],
    278: [function(e, t, i) {
        "use strict";
        var n = e(277),
            r = {
                WeblabClient: n
            };
        t.exports = r
    }, {
        277: 277
    }],
    279: [function(e, t, i) {
        "use strict";
        var n = function() {
            function e(t, i, n, r) {
                this.is = this, this.a = this, this.an = this, this.baseName = t, this.target = i, this.targetName = n, this.negated = r, r || (this.not = new e(t, i, n, !0))
            }
            return (e.prototype["function"] = function() {
                return this.checkType("Function")
            }, e.prototype.object = function() {
                return this.checkType("Object")
            }, e.prototype.number = function() {
                return this.checkType("Number")
            }, e.prototype.string = function() {
                return this.checkType("String")
            }, e.prototype.array = function() {
                return this.checkType("Array")
            }, e.prototype["null"] = function() {
                return this.verify(null === this.target, "null")
            }, e.prototype.undefined = function() {
                return this.verify("undefined" == typeof this.target, "undefined")
            }, e.prototype.empty = function() {
                return this.verify(null === this.target || "undefined" == typeof this.target || "" === this.target || this.isType("Array") && 0 === this.target.length || this.isType("Object") && !this.hasProperties(), "empty")
            }, e.prototype.exists = function() {
                return this.verify("undefined" != typeof this.target && null !== this.target, "defined and non-null")
            }, e.prototype["true"] = function() {
                return this.verify(this.target === !0, "true")
            }, e.prototype["false"] = function() {
                return this.verify(this.target === !1, "false")
            }, e.prototype.checkType = function(e) {
                return this.verify(this.isType(e), "a " + e)
            }, e.prototype.isType = function(e) {
                return Object.prototype.toString.call(this.target) === "[object " + e + "]"
            }, e.prototype.hasProperties = function() {
                for (var e in this.target)
                    if (this.target.hasOwnProperty(e)) return !0;
                return !1
            }, e.prototype.verify = function(e, t) {
                if (this.negated ? e : !e) {
                    var i = this.baseName ? "[" + this.baseName + "] " : "",
                        n = this.targetName ? this.targetName + " (" + this.target + ")" : "" + this.target;
                    throw new Error(i + "Check failed: " + n + " was " + (this.negated ? "" : "not ") + t)
                }
                return this.target
            }, e)
        }();
        t.exports = n
    }, {}],
    280: [function(e, t, i) {
        "use strict";
        var n = e(279),
            r = function(e) {
                return (void 0 === e && (e = n), {
                    of: function(t) {
                        return function(i, n) {
                            return new e(t, i, n)
                        }
                    },
                    check: function(t, i) {
                        return new e(null, t, i)
                    }
                })
            };
        t.exports = r
    }, {
        279: 279
    }],
    281: [function(e, t, i) {
        "use strict";
        var n = e(280);
        t.exports = n()
    }, {
        280: 280
    }],
    282: [function(e, t, i) {
        var n;
        ! function(e) {
            "undefined" == typeof DO_NOT_EXPORT_CRC ? "object" == typeof i ? e(i) : "function" == typeof define && define.amd ? define(function() {
                var t = {};
                return (e(t), t)
            }) : e(n = {}) : e(n = {})
        }(function(e) {
            function t() {
                for (var e = 0, t = new Array(256), i = 0; 256 != i; ++i) e = i, e = 1 & e ? -306674912 ^ e >>> 1 : e >>> 1, e = 1 & e ? -306674912 ^ e >>> 1 : e >>> 1, e = 1 & e ? -306674912 ^ e >>> 1 : e >>> 1, e = 1 & e ? -306674912 ^ e >>> 1 : e >>> 1, e = 1 & e ? -306674912 ^ e >>> 1 : e >>> 1, e = 1 & e ? -306674912 ^ e >>> 1 : e >>> 1, e = 1 & e ? -306674912 ^ e >>> 1 : e >>> 1, e = 1 & e ? -306674912 ^ e >>> 1 : e >>> 1, t[i] = e;
                return "undefined" != typeof Int32Array ? new Int32Array(t) : t
            }

            function i(e) {
                if (e.length > 32768 && a) return r(new Buffer(e));
                for (var t = -1, i = e.length - 1, n = 0; n < i;) t = s[255 & (t ^ e.charCodeAt(n++))] ^ t >>> 8, t = s[255 & (t ^ e.charCodeAt(n++))] ^ t >>> 8;
                return (n === i && (t = t >>> 8 ^ s[255 & (t ^ e.charCodeAt(n))]), t ^ -1)
            }

            function n(e) {
                if (e.length > 1e4) return r(e);
                for (var t = -1, i = 0, n = e.length - 3; i < n;) t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])];
                for (; i < n + 3;) t = t >>> 8 ^ s[255 & (t ^ e[i++])];
                return t ^ -1
            }

            function r(e) {
                for (var t = -1, i = 0, n = e.length - 7; i < n;) t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])], t = t >>> 8 ^ s[255 & (t ^ e[i++])];
                for (; i < n + 7;) t = t >>> 8 ^ s[255 & (t ^ e[i++])];
                return t ^ -1
            }

            function o(e) {
                for (var t, i, n = -1, r = 0, o = e.length; r < o;) t = e.charCodeAt(r++), t < 128 ? n = n >>> 8 ^ s[255 & (n ^ t)] : t < 2048 ? (n = n >>> 8 ^ s[255 & (n ^ (192 | t >> 6 & 31))], n = n >>> 8 ^ s[255 & (n ^ (128 | 63 & t))]) : t >= 55296 && t < 57344 ? (t = (1023 & t) + 64, i = 1023 & e.charCodeAt(r++), n = n >>> 8 ^ s[255 & (n ^ (240 | t >> 8 & 7))], n = n >>> 8 ^ s[255 & (n ^ (128 | t >> 2 & 63))], n = n >>> 8 ^ s[255 & (n ^ (128 | i >> 6 & 15 | (3 & t) << 4))], n = n >>> 8 ^ s[255 & (n ^ (128 | 63 & i))]) : (n = n >>> 8 ^ s[255 & (n ^ (224 | t >> 12 & 15))], n = n >>> 8 ^ s[255 & (n ^ (128 | t >> 6 & 63))], n = n >>> 8 ^ s[255 & (n ^ (128 | 63 & t))]);
                return n ^ -1
            }
            e.version = "0.4.0";
            var s = t(),
                a = "undefined" != typeof Buffer;
            e.table = s, e.bstr = i, e.buf = n, e.str = o
        })
    }, {}],
    283: [function(e, t, i) {
        ! function(e) {
            function t() {
                this._events = {}, this._conf && n.call(this, this._conf)
            }

            function n(e) {
                e && (this._conf = e, e.delimiter && (this.delimiter = e.delimiter), e.maxListeners && (this._events.maxListeners = e.maxListeners), e.wildcard && (this.wildcard = e.wildcard), e.newListener && (this.newListener = e.newListener), this.wildcard && (this.listenerTree = {}))
            }

            function r(e) {
                this._events = {}, this.newListener = !1, n.call(this, e)
            }

            function o(e, t, i, n) {
                if (!i) return [];
                var r, s, a, l, c, u, p, h = [],
                    f = t.length,
                    d = t[n],
                    y = t[n + 1];
                if (n === f && i._listeners) {
                    if ("function" == typeof i._listeners) return (e && e.push(i._listeners), [i]);
                    for (r = 0, s = i._listeners.length; r < s; r++) e && e.push(i._listeners[r]);
                    return [i]
                }
                if ("*" === d || "**" === d || i[d]) {
                    if ("*" === d) {
                        for (a in i) "_listeners" !== a && i.hasOwnProperty(a) && (h = h.concat(o(e, t, i[a], n + 1)));
                        return h
                    }
                    if ("**" === d) {
                        p = n + 1 === f || n + 2 === f && "*" === y, p && i._listeners && (h = h.concat(o(e, t, i, f)));
                        for (a in i) "_listeners" !== a && i.hasOwnProperty(a) && ("*" === a || "**" === a ? (i[a]._listeners && !p && (h = h.concat(o(e, t, i[a], f))), h = h.concat(o(e, t, i[a], n))) : h = a === y ? h.concat(o(e, t, i[a], n + 2)) : h.concat(o(e, t, i[a], n)));
                        return h
                    }
                    h = h.concat(o(e, t, i[d], n + 1))
                }
                if (l = i["*"], l && o(e, t, l, n + 1), c = i["**"])
                    if (n < f) {
                        c._listeners && o(e, t, c, f);
                        for (a in c) "_listeners" !== a && c.hasOwnProperty(a) && (a === y ? o(e, t, c[a], n + 2) : a === d ? o(e, t, c[a], n + 1) : (u = {}, u[a] = c[a], o(e, t, {
                            "**": u
                        }, n + 1)))
                    } else c._listeners ? o(e, t, c, f) : c["*"] && c["*"]._listeners && o(e, t, c["*"], f);
                return h
            }

            function s(e, t) {
                e = "string" == typeof e ? e.split(this.delimiter) : e.slice();
                for (var i = 0, n = e.length; i + 1 < n; i++)
                    if ("**" === e[i] && "**" === e[i + 1]) return;
                for (var r = this.listenerTree, o = e.shift(); o;) {
                    if (r[o] || (r[o] = {}), r = r[o], 0 === e.length) {
                        if (r._listeners) {
                            if ("function" == typeof r._listeners) r._listeners = [r._listeners, t];
                            else if (a(r._listeners) && (r._listeners.push(t), !r._listeners.warned)) {
                                var s = l;
                                "undefined" != typeof this._events.maxListeners && (s = this._events.maxListeners), s > 0 && r._listeners.length > s && (r._listeners.warned = !0, console.error("(node) warning: possible EventEmitter memory leak detected. %d listeners added. Use emitter.setMaxListeners() to increase limit.", r._listeners.length), console.trace())
                            }
                        } else r._listeners = t;
                        return !0
                    }
                    o = e.shift()
                }
                return !0
            }
            var a = Array.isArray ? Array.isArray : function(e) {
                    return "[object Array]" === Object.prototype.toString.call(e)
                },
                l = 10;
            r.prototype.delimiter = ".", r.prototype.setMaxListeners = function(e) {
                this._events || t.call(this), this._events.maxListeners = e, this._conf || (this._conf = {}), this._conf.maxListeners = e
            }, r.prototype.event = "", r.prototype.once = function(e, t) {
                return (this.many(e, 1, t), this)
            }, r.prototype.many = function(e, t, i) {
                function n() {
                    0 === --t && r.off(e, n), i.apply(this, arguments)
                }
                var r = this;
                if ("function" != typeof i) throw new Error("many only accepts instances of Function");
                return (n._origin = i, this.on(e, n), r)
            }, r.prototype.emit = function() {
                this._events || t.call(this);
                var e = arguments[0];
                if ("newListener" === e && !this.newListener && !this._events.newListener) return !1;
                if (this._all) {
                    for (var i = arguments.length, n = new Array(i - 1), r = 1; r < i; r++) n[r - 1] = arguments[r];
                    for (r = 0, i = this._all.length; r < i; r++) this.event = e, this._all[r].apply(this, n)
                }
                if ("error" === e && !(this._all || this._events.error || this.wildcard && this.listenerTree.error)) throw arguments[1] instanceof Error ? arguments[1] : new Error("Uncaught, unspecified 'error' event.");
                var s;
                if (this.wildcard) {
                    s = [];
                    var a = "string" == typeof e ? e.split(this.delimiter) : e.slice();
                    o.call(this, s, a, this.listenerTree, 0)
                } else s = this._events[e];
                if ("function" == typeof s) {
                    if (this.event = e, 1 === arguments.length) s.call(this);
                    else if (arguments.length > 1) switch (arguments.length) {
                        case 2:
                            s.call(this, arguments[1]);
                            break;
                        case 3:
                            s.call(this, arguments[1], arguments[2]);
                            break;
                        default:
                            for (var i = arguments.length, n = new Array(i - 1), r = 1; r < i; r++) n[r - 1] = arguments[r];
                            s.apply(this, n)
                    }
                    return !0
                }
                if (s) {
                    for (var i = arguments.length, n = new Array(i - 1), r = 1; r < i; r++) n[r - 1] = arguments[r];
                    for (var l = s.slice(), r = 0, i = l.length; r < i; r++) this.event = e, l[r].apply(this, n);
                    return l.length > 0 || !!this._all
                }
                return !!this._all
            }, r.prototype.on = function(e, i) {
                if ("function" == typeof e) return (this.onAny(e), this);
                if ("function" != typeof i) throw new Error("on only accepts instances of Function");
                if (this._events || t.call(this), this.emit("newListener", e, i), this.wildcard) return (s.call(this, e, i), this);
                if (this._events[e]) {
                    if ("function" == typeof this._events[e]) this._events[e] = [this._events[e], i];
                    else if (a(this._events[e]) && (this._events[e].push(i), !this._events[e].warned)) {
                        var n = l;
                        "undefined" != typeof this._events.maxListeners && (n = this._events.maxListeners), n > 0 && this._events[e].length > n && (this._events[e].warned = !0, console.error("(node) warning: possible EventEmitter memory leak detected. %d listeners added. Use emitter.setMaxListeners() to increase limit.", this._events[e].length), console.trace())
                    }
                } else this._events[e] = i;
                return this
            }, r.prototype.onAny = function(e) {
                if ("function" != typeof e) throw new Error("onAny only accepts instances of Function");
                return (this._all || (this._all = []), this._all.push(e), this)
            }, r.prototype.addListener = r.prototype.on, r.prototype.off = function(e, t) {
                if ("function" != typeof t) throw new Error("removeListener only takes instances of Function");
                var i, n = [];
                if (this.wildcard) {
                    var r = "string" == typeof e ? e.split(this.delimiter) : e.slice();
                    n = o.call(this, null, r, this.listenerTree, 0)
                } else {
                    if (!this._events[e]) return this;
                    i = this._events[e], n.push({
                        _listeners: i
                    })
                }
                for (var s = 0; s < n.length; s++) {
                    var l = n[s];
                    if (i = l._listeners, a(i)) {
                        for (var c = -1, u = 0, p = i.length; u < p; u++)
                            if (i[u] === t || i[u].listener && i[u].listener === t || i[u]._origin && i[u]._origin === t) {
                                c = u;
                                break
                            } if (c < 0) continue;
                        return (this.wildcard ? l._listeners.splice(c, 1) : this._events[e].splice(c, 1), 0 === i.length && (this.wildcard ? delete l._listeners : delete this._events[e]), this)
                    }(i === t || i.listener && i.listener === t || i._origin && i._origin === t) && (this.wildcard ? delete l._listeners : delete this._events[e])
                }
                return this
            }, r.prototype.offAny = function(e) {
                var t, i = 0,
                    n = 0;
                if (e && this._all && this._all.length > 0) {
                    for (t = this._all, i = 0, n = t.length; i < n; i++)
                        if (e === t[i]) return (t.splice(i, 1), this)
                } else this._all = [];
                return this
            }, r.prototype.removeListener = r.prototype.off, r.prototype.removeAllListeners = function(e) {
                if (0 === arguments.length) return (!this._events || t.call(this), this);
                if (this.wildcard)
                    for (var i = "string" == typeof e ? e.split(this.delimiter) : e.slice(), n = o.call(this, null, i, this.listenerTree, 0), r = 0; r < n.length; r++) {
                        var s = n[r];
                        s._listeners = null
                    } else {
                        if (!this._events[e]) return this;
                        this._events[e] = null
                    }
                return this
            }, r.prototype.listeners = function(e) {
                if (this.wildcard) {
                    var i = [],
                        n = "string" == typeof e ? e.split(this.delimiter) : e.slice();
                    return (o.call(this, i, n, this.listenerTree, 0), i)
                }
                return (this._events || t.call(this), this._events[e] || (this._events[e] = []), a(this._events[e]) || (this._events[e] = [this._events[e]]), this._events[e])
            }, r.prototype.listenersAny = function() {
                return this._all ? this._all : []
            }, "function" == typeof define && define.amd ? define(function() {
                return r
            }) : "object" == typeof i ? i.EventEmitter2 = r : window.EventEmitter2 = r
        }()
    }, {}],
    284: [function(e, t, i) {
        ! function(e) {
            "use strict";
            var i = {};
            i.VERSION = "0.9.14";
            var n, r = {},
                o = function(e, t) {
                    return function() {
                        return t.apply(e, arguments)
                    }
                },
                s = function() {
                    var e, t, i = arguments,
                        n = i[0];
                    for (t = 1; t < i.length; t++)
                        for (e in i[t]) e in n || !i[t].hasOwnProperty(e) || (n[e] = i[t][e]);
                    return n
                },
                a = function(e, t) {
                    return {
                        value: e,
                        name: t
                    }
                };
            i.DEBUG = a(1, "DEBUG"), i.INFO = a(2, "INFO"), i.WARN = a(4, "WARN"), i.ERROR = a(8, "ERROR"), i.OFF = a(99, "OFF");
            var l = function(e) {
                this.context = e, this.setLevel(e.filterLevel), this.log = this.info
            };
            l.prototype = {
                setLevel: function(e) {
                    e && "value" in e && (this.context.filterLevel = e)
                },
                enabledFor: function(e) {
                    var t = this.context.filterLevel;
                    return e.value >= t.value
                },
                debug: function() {
                    console.log("DEBUG 284: " + JSON.stringify(arguments[0], null, 4) + (arguments.length > 1? JSON.stringify(arguments[1]):""));
                    this.invoke(i.DEBUG, arguments)
                },
                info: function() {
                    console.log("INFO 284: " + JSON.stringify(arguments[0], null, 4) + (arguments.length > 1? JSON.stringify(arguments[1]):""));
                    this.invoke(i.INFO, arguments)
                },
                warn: function() {
                    console.log("WARN 284: " + JSON.stringify(arguments[0], null, 4) + (arguments.length > 1? JSON.stringify(arguments[1]):""));
                    this.invoke(i.WARN, arguments)
                },
                error: function() {
                    console.log("ERROR 284: " + JSON.stringify(arguments[0], null, 4) + (arguments.length > 1? JSON.stringify(arguments[1]):""));
                    this.invoke(i.ERROR, arguments)
                },
                invoke: function(e, t) {
                    n && this.enabledFor(e) && n(t, s({
                        level: e
                    }, this.context))
                }
            };
            var c = new l({
                filterLevel: i.OFF
            });
            ! function() {
                var e = i;
                e.enabledFor = o(c, c.enabledFor), e.debug = o(c, c.debug), e.info = o(c, c.info), e.warn = o(c, c.warn), e.error = o(c, c.error), e.log = e.info
            }(), i.setHandler = function(e) {
                n = e
            }, i.setLevel = function(e) {
                c.setLevel(e);
                for (var t in r) r.hasOwnProperty(t) && r[t].setLevel(e)
            }, i.get = function(e) {
                return r[e] || (r[e] = new l(s({
                    name: e
                }, c.context)))
            }, i.useDefaults = function(e) {
                "undefined" != typeof console && (i.setLevel(e || i.DEBUG), i.setHandler(function(e, t) {
                    var n = console.log;
                    t.name && (e[0] = "[" + t.name + "] " + e[0]), t.level === i.WARN && console.warn ? n = console.warn : t.level === i.ERROR && console.error ? n = console.error : t.level === i.INFO && console.info && (n = console.info), Function.prototype.apply.call(n, console, e)
                }))
            }, "function" == typeof define && define.amd ? define(i) : "undefined" != typeof t && t.exports ? t.exports = i : (i._prevLogger = e.Logger, i.noConflict = function() {
                return (e.Logger = i._prevLogger, i)
            }, e.Logger = i)
        }(this)
    }, {}],
    285: [function(e, t, i) {
        function n(e, t, i, n) {
            return JSON.stringify(e, r(t, n), i)
        }

        function r(e, t) {
            var i = [],
                n = [];
            return (null == t && (t = function(e, t) {
                return i[0] === t ? "[Circular ~]" : "[Circular ~." + n.slice(0, i.indexOf(t)).join(".") + "]"
            }), function(r, o) {
                if (i.length > 0) {
                    var s = i.indexOf(this);
                    ~s ? i.splice(s + 1) : i.push(this), ~s ? n.splice(s, 1 / 0, r) : n.push(r), ~i.indexOf(o) && (o = t.call(this, r, o))
                } else i.push(o);
                return null == e ? o : e.call(this, r, o)
            })
        }
        i = t.exports = n, i.getSerialize = r
    }, {}],
    286: [function(e, t, i) {
        ! function(e) {
            function n(e, t, i) {
                var n = 0,
                    r = [0],
                    p = "",
                    h = null,
                    p = i || "UTF8";
                if ("UTF8" !== p && "UTF16" !== p) throw "encoding must be UTF8 or UTF16";
                if ("HEX" === t) {
                    if (0 !== e.length % 2) throw "srcString of HEX type must be in byte increments";
                    h = s(e), n = h.binLen, r = h.value
                } else if ("ASCII" === t || "TEXT" === t) h = o(e, p), n = h.binLen, r = h.value;
                else {
                    if ("B64" !== t) throw "inputFormat must be HEX, TEXT, ASCII, or B64";
                    h = a(e), n = h.binLen, r = h.value
                }
                this.getHash = function(e, t, i, o) {
                    var s, a = null,
                        p = r.slice(),
                        h = n;
                    if (3 === arguments.length ? "number" != typeof i && (o = i, i = 1) : 2 === arguments.length && (i = 1), i !== parseInt(i, 10) || 1 > i) throw "numRounds must a integer >= 1";
                    switch (t) {
                        case "HEX":
                            a = l;
                            break;
                        case "B64":
                            a = c;
                            break;
                        default:
                            throw "format must be HEX or B64"
                    }
                    if ("SHA-1" === e)
                        for (s = 0; s < i; s++) p = L(p, h), h = 160;
                    else if ("SHA-224" === e)
                        for (s = 0; s < i; s++) p = k(p, h, e), h = 224;
                    else if ("SHA-256" === e)
                        for (s = 0; s < i; s++) p = k(p, h, e), h = 256;
                    else if ("SHA-384" === e)
                        for (s = 0; s < i; s++) p = k(p, h, e), h = 384;
                    else {
                        if ("SHA-512" !== e) throw "Chosen SHA variant is not supported";
                        for (s = 0; s < i; s++) p = k(p, h, e), h = 512
                    }
                    return a(p, u(o))
                }, this.getHMAC = function(e, t, i, h, f) {
                    var d, y, g, m, v = [],
                        E = [];
                    switch (d = null, h) {
                        case "HEX":
                            h = l;
                            break;
                        case "B64":
                            h = c;
                            break;
                        default:
                            throw "outputFormat must be HEX or B64"
                    }
                    if ("SHA-1" === i) y = 64, m = 160;
                    else if ("SHA-224" === i) y = 64, m = 224;
                    else if ("SHA-256" === i) y = 64, m = 256;
                    else if ("SHA-384" === i) y = 128, m = 384;
                    else {
                        if ("SHA-512" !== i) throw "Chosen SHA variant is not supported";
                        y = 128, m = 512
                    }
                    if ("HEX" === t) d = s(e), g = d.binLen, d = d.value;
                    else if ("ASCII" === t || "TEXT" === t) d = o(e, p), g = d.binLen, d = d.value;
                    else {
                        if ("B64" !== t) throw "inputFormat must be HEX, TEXT, ASCII, or B64";
                        d = a(e), g = d.binLen, d = d.value
                    }
                    for (e = 8 * y, t = y / 4 - 1, y < g / 8 ? (d = "SHA-1" === i ? L(d, g) : k(d, g, i), d[t] &= 4294967040) : y > g / 8 && (d[t] &= 4294967040), y = 0; y <= t; y += 1) v[y] = 909522486 ^ d[y], E[y] = 1549556828 ^ d[y];
                    return (i = "SHA-1" === i ? L(E.concat(L(v.concat(r), e + n)), e + m) : k(E.concat(k(v.concat(r), e + n, i)), e + m, i), h(i, u(f)))
                }
            }

            function r(e, t) {
                this.a = e, this.b = t
            }

            function o(e, t) {
                var i, n, r = [],
                    o = [],
                    s = 0;
                if ("UTF8" === t)
                    for (n = 0; n < e.length; n += 1)
                        for (i = e.charCodeAt(n), o = [], 2048 < i ? (o[0] = 224 | (61440 & i) >>> 12, o[1] = 128 | (4032 & i) >>> 6, o[2] = 128 | 63 & i) : 128 < i ? (o[0] = 192 | (1984 & i) >>> 6, o[1] = 128 | 63 & i) : o[0] = i, i = 0; i < o.length; i += 1) r[s >>> 2] |= o[i] << 24 - s % 4 * 8, s += 1;
                else if ("UTF16" === t)
                    for (n = 0; n < e.length; n += 1) r[s >>> 2] |= e.charCodeAt(n) << 16 - s % 4 * 8, s += 2;
                return {
                    value: r,
                    binLen: 8 * s
                }
            }

            function s(e) {
                var t, i, n = [],
                    r = e.length;
                if (0 !== r % 2) throw "String of HEX type must be in byte increments";
                for (t = 0; t < r; t += 2) {
                    if (i = parseInt(e.substr(t, 2), 16), isNaN(i)) throw "String of HEX type contains invalid characters";
                    n[t >>> 3] |= i << 24 - t % 8 * 4
                }
                return {
                    value: n,
                    binLen: 4 * r
                }
            }

            function a(e) {
                var t, i, n, r, o, s = [],
                    a = 0;
                if (-1 === e.search(/^[a-zA-Z0-9=+\/]+$/)) throw "Invalid character in base-64 string";
                if (t = e.indexOf("="), e = e.replace(/\=/g, ""), -1 !== t && t < e.length) throw "Invalid '=' found in base-64 string";
                for (i = 0; i < e.length; i += 4) {
                    for (o = e.substr(i, 4), n = r = 0; n < o.length; n += 1) t = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".indexOf(o[n]), r |= t << 18 - 6 * n;
                    for (n = 0; n < o.length - 1; n += 1) s[a >> 2] |= (r >>> 16 - 8 * n & 255) << 24 - a % 4 * 8, a += 1
                }
                return {
                    value: s,
                    binLen: 8 * a
                }
            }

            function l(e, t) {
                var i, n, r = "",
                    o = 4 * e.length;
                for (i = 0; i < o; i += 1) n = e[i >>> 2] >>> 8 * (3 - i % 4), r += "0123456789abcdef".charAt(n >>> 4 & 15) + "0123456789abcdef".charAt(15 & n);
                return t.outputUpper ? r.toUpperCase() : r
            }

            function c(e, t) {
                var i, n, r, o = "",
                    s = 4 * e.length;
                for (i = 0; i < s; i += 3)
                    for (r = (e[i >>> 2] >>> 8 * (3 - i % 4) & 255) << 16 | (e[i + 1 >>> 2] >>> 8 * (3 - (i + 1) % 4) & 255) << 8 | e[i + 2 >>> 2] >>> 8 * (3 - (i + 2) % 4) & 255, n = 0; 4 > n; n += 1) o = 8 * i + 6 * n <= 32 * e.length ? o + "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".charAt(r >>> 6 * (3 - n) & 63) : o + t.b64Pad;
                return o
            }

            function u(e) {
                var t = {
                    outputUpper: !1,
                    b64Pad: "="
                };
                try {
                    e.hasOwnProperty("outputUpper") && (t.outputUpper = e.outputUpper), e.hasOwnProperty("b64Pad") && (t.b64Pad = e.b64Pad)
                } catch (e) {}
                if ("boolean" != typeof t.outputUpper) throw "Invalid outputUpper formatting option";
                if ("string" != typeof t.b64Pad) throw "Invalid b64Pad formatting option";
                return t
            }

            function p(e, t) {
                return e << t | e >>> 32 - t
            }

            function h(e, t) {
                return e >>> t | e << 32 - t
            }

            function f(e, t) {
                var i = null,
                    i = new r(e.a, e.b);
                return i = 32 >= t ? new r(i.a >>> t | i.b << 32 - t & 4294967295, i.b >>> t | i.a << 32 - t & 4294967295) : new r(i.b >>> t - 32 | i.a << 64 - t & 4294967295, i.a >>> t - 32 | i.b << 64 - t & 4294967295)
            }

            function d(e, t) {
                var i = null;
                return i = 32 >= t ? new r(e.a >>> t, e.b >>> t | e.a << 32 - t & 4294967295) : new r(0, e.a >>> t - 32)
            }

            function y(e, t, i) {
                return e ^ t ^ i
            }

            function g(e, t, i) {
                return e & t ^ ~e & i
            }

            function m(e, t, i) {
                return new r(e.a & t.a ^ ~e.a & i.a, e.b & t.b ^ ~e.b & i.b)
            }

            function v(e, t, i) {
                return e & t ^ e & i ^ t & i
            }

            function E(e, t, i) {
                return new r(e.a & t.a ^ e.a & i.a ^ t.a & i.a, e.b & t.b ^ e.b & i.b ^ t.b & i.b)
            }

            function _(e) {
                return h(e, 2) ^ h(e, 13) ^ h(e, 22)
            }

            function S(e) {
                var t = f(e, 28),
                    i = f(e, 34);
                return (e = f(e, 39), new r(t.a ^ i.a ^ e.a, t.b ^ i.b ^ e.b))
            }

            function T(e) {
                return h(e, 6) ^ h(e, 11) ^ h(e, 25)
            }

            function b(e) {
                var t = f(e, 14),
                    i = f(e, 18);
                return (e = f(e, 41), new r(t.a ^ i.a ^ e.a, t.b ^ i.b ^ e.b))
            }

            function I(e) {
                return h(e, 7) ^ h(e, 18) ^ e >>> 3
            }

            function w(e) {
                var t = f(e, 1),
                    i = f(e, 8);
                return (e = d(e, 7), new r(t.a ^ i.a ^ e.a, t.b ^ i.b ^ e.b))
            }

            function R(e) {
                return h(e, 17) ^ h(e, 19) ^ e >>> 10
            }

            function C(e) {
                var t = f(e, 19),
                    i = f(e, 61);
                return (e = d(e, 6), new r(t.a ^ i.a ^ e.a, t.b ^ i.b ^ e.b))
            }

            function P(e, t) {
                var i = (65535 & e) + (65535 & t);
                return ((e >>> 16) + (t >>> 16) + (i >>> 16) & 65535) << 16 | 65535 & i
            }

            function A(e, t, i, n) {
                var r = (65535 & e) + (65535 & t) + (65535 & i) + (65535 & n);
                return ((e >>> 16) + (t >>> 16) + (i >>> 16) + (n >>> 16) + (r >>> 16) & 65535) << 16 | 65535 & r
            }

            function O(e, t, i, n, r) {
                var o = (65535 & e) + (65535 & t) + (65535 & i) + (65535 & n) + (65535 & r);
                return ((e >>> 16) + (t >>> 16) + (i >>> 16) + (n >>> 16) + (r >>> 16) + (o >>> 16) & 65535) << 16 | 65535 & o
            }

            function N(e, t) {
                var i, n, o;
                return (i = (65535 & e.b) + (65535 & t.b), n = (e.b >>> 16) + (t.b >>> 16) + (i >>> 16), o = (65535 & n) << 16 | 65535 & i, i = (65535 & e.a) + (65535 & t.a) + (n >>> 16), n = (e.a >>> 16) + (t.a >>> 16) + (i >>> 16), new r((65535 & n) << 16 | 65535 & i, o))
            }

            function x(e, t, i, n) {
                var o, s, a;
                return (o = (65535 & e.b) + (65535 & t.b) + (65535 & i.b) + (65535 & n.b), s = (e.b >>> 16) + (t.b >>> 16) + (i.b >>> 16) + (n.b >>> 16) + (o >>> 16), a = (65535 & s) << 16 | 65535 & o, o = (65535 & e.a) + (65535 & t.a) + (65535 & i.a) + (65535 & n.a) + (s >>> 16), s = (e.a >>> 16) + (t.a >>> 16) + (i.a >>> 16) + (n.a >>> 16) + (o >>> 16), new r((65535 & s) << 16 | 65535 & o, a))
            }

            function M(e, t, i, n, o) {
                var s, a, l;
                return (s = (65535 & e.b) + (65535 & t.b) + (65535 & i.b) + (65535 & n.b) + (65535 & o.b), a = (e.b >>> 16) + (t.b >>> 16) + (i.b >>> 16) + (n.b >>> 16) + (o.b >>> 16) + (s >>> 16), l = (65535 & a) << 16 | 65535 & s, s = (65535 & e.a) + (65535 & t.a) + (65535 & i.a) + (65535 & n.a) + (65535 & o.a) + (a >>> 16), a = (e.a >>> 16) + (t.a >>> 16) + (i.a >>> 16) + (n.a >>> 16) + (o.a >>> 16) + (s >>> 16), new r((65535 & a) << 16 | 65535 & s, l))
            }

            function L(e, t) {
                var i, n, r, o, s, a, l, c, u, h = [],
                    f = g,
                    d = y,
                    m = v,
                    E = p,
                    _ = P,
                    S = O,
                    T = [1732584193, 4023233417, 2562383102, 271733878, 3285377520];
                for (e[t >>> 5] |= 128 << 24 - t % 32, e[(t + 65 >>> 9 << 4) + 15] = t, u = e.length, l = 0; l < u; l += 16) {
                    for (i = T[0], n = T[1], r = T[2], o = T[3], s = T[4], c = 0; 80 > c; c += 1) h[c] = 16 > c ? e[c + l] : E(h[c - 3] ^ h[c - 8] ^ h[c - 14] ^ h[c - 16], 1), a = 20 > c ? S(E(i, 5), f(n, r, o), s, 1518500249, h[c]) : 40 > c ? S(E(i, 5), d(n, r, o), s, 1859775393, h[c]) : 60 > c ? S(E(i, 5), m(n, r, o), s, 2400959708, h[c]) : S(E(i, 5), d(n, r, o), s, 3395469782, h[c]), s = o, o = r, r = E(n, 30), n = i, i = a;
                    T[0] = _(i, T[0]), T[1] = _(n, T[1]), T[2] = _(r, T[2]), T[3] = _(o, T[3]), T[4] = _(s, T[4])
                }
                return T
            }

            function k(e, t, i) {
                var n, o, s, a, l, c, u, p, h, f, d, y, L, k, D, U, F, H, W, V, B, q, j, z, G, Y, K = [],
                    X = [1116352408, 1899447441, 3049323471, 3921009573, 961987163, 1508970993, 2453635748, 2870763221, 3624381080, 310598401, 607225278, 1426881987, 1925078388, 2162078206, 2614888103, 3248222580, 3835390401, 4022224774, 264347078, 604807628, 770255983, 1249150122, 1555081692, 1996064986, 2554220882, 2821834349, 2952996808, 3210313671, 3336571891, 3584528711, 113926993, 338241895, 666307205, 773529912, 1294757372, 1396182291, 1695183700, 1986661051, 2177026350, 2456956037, 2730485921, 2820302411, 3259730800, 3345764771, 3516065817, 3600352804, 4094571909, 275423344, 430227734, 506948616, 659060556, 883997877, 958139571, 1322822218, 1537002063, 1747873779, 1955562222, 2024104815, 2227730452, 2361852424, 2428436474, 2756734187, 3204031479, 3329325298];
                if (f = [3238371032, 914150663, 812702999, 4144912697, 4290775857, 1750603025, 1694076839, 3204075428], o = [1779033703, 3144134277, 1013904242, 2773480762, 1359893119, 2600822924, 528734635, 1541459225], "SHA-224" === i || "SHA-256" === i) d = 64, n = (t + 65 >>> 9 << 4) + 15, k = 16, D = 1, G = Number, U = P, F = A, H = O, W = I, V = R, B = _, q = T, z = v, j = g, f = "SHA-224" === i ? f : o;
                else {
                    if ("SHA-384" !== i && "SHA-512" !== i) throw "Unexpected error in SHA-2 implementation";
                    d = 80, n = (t + 128 >>> 10 << 5) + 31, k = 32, D = 2, G = r, U = N, F = x, H = M, W = w, V = C, B = S, q = b, z = E, j = m, X = [new G(X[0], 3609767458), new G(X[1], 602891725), new G(X[2], 3964484399), new G(X[3], 2173295548), new G(X[4], 4081628472), new G(X[5], 3053834265), new G(X[6], 2937671579), new G(X[7], 3664609560), new G(X[8], 2734883394), new G(X[9], 1164996542), new G(X[10], 1323610764), new G(X[11], 3590304994), new G(X[12], 4068182383), new G(X[13], 991336113), new G(X[14], 633803317), new G(X[15], 3479774868), new G(X[16], 2666613458), new G(X[17], 944711139), new G(X[18], 2341262773), new G(X[19], 2007800933), new G(X[20], 1495990901), new G(X[21], 1856431235), new G(X[22], 3175218132), new G(X[23], 2198950837), new G(X[24], 3999719339), new G(X[25], 766784016), new G(X[26], 2566594879), new G(X[27], 3203337956), new G(X[28], 1034457026), new G(X[29], 2466948901), new G(X[30], 3758326383), new G(X[31], 168717936), new G(X[32], 1188179964), new G(X[33], 1546045734), new G(X[34], 1522805485), new G(X[35], 2643833823), new G(X[36], 2343527390), new G(X[37], 1014477480), new G(X[38], 1206759142), new G(X[39], 344077627), new G(X[40], 1290863460), new G(X[41], 3158454273), new G(X[42], 3505952657), new G(X[43], 106217008), new G(X[44], 3606008344), new G(X[45], 1432725776), new G(X[46], 1467031594), new G(X[47], 851169720), new G(X[48], 3100823752), new G(X[49], 1363258195), new G(X[50], 3750685593), new G(X[51], 3785050280), new G(X[52], 3318307427), new G(X[53], 3812723403), new G(X[54], 2003034995), new G(X[55], 3602036899), new G(X[56], 1575990012), new G(X[57], 1125592928), new G(X[58], 2716904306), new G(X[59], 442776044), new G(X[60], 593698344), new G(X[61], 3733110249), new G(X[62], 2999351573), new G(X[63], 3815920427), new G(3391569614, 3928383900), new G(3515267271, 566280711), new G(3940187606, 3454069534), new G(4118630271, 4000239992), new G(116418474, 1914138554), new G(174292421, 2731055270), new G(289380356, 3203993006), new G(460393269, 320620315), new G(685471733, 587496836), new G(852142971, 1086792851), new G(1017036298, 365543100), new G(1126000580, 2618297676), new G(1288033470, 3409855158), new G(1501505948, 4234509866), new G(1607167915, 987167468), new G(1816402316, 1246189591)], f = "SHA-384" === i ? [new G(3418070365, f[0]), new G(1654270250, f[1]), new G(2438529370, f[2]), new G(355462360, f[3]), new G(1731405415, f[4]), new G(41048885895, f[5]), new G(3675008525, f[6]), new G(1203062813, f[7])] : [new G(o[0], 4089235720), new G(o[1], 2227873595), new G(o[2], 4271175723), new G(o[3], 1595750129), new G(o[4], 2917565137), new G(o[5], 725511199), new G(o[6], 4215389547), new G(o[7], 327033209)]
                }
                for (e[t >>> 5] |= 128 << 24 - t % 32, e[n] = t, Y = e.length, y = 0; y < Y; y += k) {
                    for (t = f[0], n = f[1], o = f[2], s = f[3], a = f[4], l = f[5], c = f[6], u = f[7], L = 0; L < d; L += 1) K[L] = 16 > L ? new G(e[L * D + y], e[L * D + y + 1]) : F(V(K[L - 2]), K[L - 7], W(K[L - 15]), K[L - 16]), p = H(u, q(a), j(a, l, c), X[L], K[L]), h = U(B(t), z(t, n, o)), u = c, c = l, l = a, a = U(s, p), s = o, o = n, n = t, t = U(p, h);
                    f[0] = U(t, f[0]), f[1] = U(n, f[1]), f[2] = U(o, f[2]), f[3] = U(s, f[3]), f[4] = U(a, f[4]), f[5] = U(l, f[5]), f[6] = U(c, f[6]), f[7] = U(u, f[7])
                }
                if ("SHA-224" === i) e = [f[0], f[1], f[2], f[3], f[4], f[5], f[6]];
                else if ("SHA-256" === i) e = f;
                else if ("SHA-384" === i) e = [f[0].a, f[0].b, f[1].a, f[1].b, f[2].a, f[2].b, f[3].a, f[3].b, f[4].a, f[4].b, f[5].a, f[5].b];
                else {
                    if ("SHA-512" !== i) throw "Unexpected error in SHA-2 implementation";
                    e = [f[0].a, f[0].b, f[1].a, f[1].b, f[2].a, f[2].b, f[3].a, f[3].b, f[4].a, f[4].b, f[5].a, f[5].b, f[6].a, f[6].b, f[7].a, f[7].b]
                }
                return e
            }
            "function" != typeof define || (define.amd, 0) ? "undefined" != typeof i ? "undefined" != typeof t && t.exports ? t.exports = i = n : i = n : e.jsSHA = n : define(function() {
                return n
            })
        }(this)
    }, {}],
    287: [function(e, t, i) {
        ! function(e, t) {
            if ("object" == typeof i && i) t(i);
            else {
                var n = {};
                t(n), "function" == typeof define && define.amd ? define(n) : e.Mustache = n
            }
        }(this, function(e) {
            function t(e, t) {
                return f.call(e, t)
            }

            function i(e) {
                return !t(d, e)
            }

            function n(e) {
                return "function" == typeof e
            }

            function r(e) {
                return e.replace(/[\-\[\]{}()*+?.,\\\^$|#\s]/g, "\\$&")
            }

            function o(e) {
                return String(e).replace(/[&<>"'\/]/g, function(e) {
                    return m[e]
                })
            }

            function s(e) {
                if (!g(e) || 2 !== e.length) throw new Error("Invalid tags: " + e);
                return [new RegExp(r(e[0]) + "\\s*"), new RegExp("\\s*" + r(e[1]))]
            }

            function a(t, n) {
                function o() {
                    if (R && !C)
                        for (; w.length;) delete I[w.pop()];
                    else w = [];
                    R = !1, C = !1
                }
                n = n || e.tags, t = t || "", "string" == typeof n && (n = n.split(E));
                for (var a, p, h, f, d, y, g = s(n), m = new u(t), b = [], I = [], w = [], R = !1, C = !1; !m.eos();) {
                    if (a = m.pos, h = m.scanUntil(g[0]))
                        for (var P = 0, A = h.length; P < A; ++P) f = h.charAt(P), i(f) ? w.push(I.length) : C = !0, I.push(["text", f, a, a + 1]), a += 1, "\n" === f && o();
                    if (!m.scan(g[0])) break;
                    if (R = !0, p = m.scan(T) || "name", m.scan(v), "=" === p ? (h = m.scanUntil(_), m.scan(_), m.scanUntil(g[1])) : "{" === p ? (h = m.scanUntil(new RegExp("\\s*" + r("}" + n[1]))), m.scan(S), m.scanUntil(g[1]), p = "&") : h = m.scanUntil(g[1]), !m.scan(g[1])) throw new Error("Unclosed tag at " + m.pos);
                    if (d = [p, h, a, m.pos], I.push(d), "#" === p || "^" === p) b.push(d);
                    else if ("/" === p) {
                        if (y = b.pop(), !y) throw new Error('Unopened section "' + h + '" at ' + a);
                        if (y[1] !== h) throw new Error('Unclosed section "' + y[1] + '" at ' + a)
                    } else "name" === p || "{" === p || "&" === p ? C = !0 : "=" === p && (g = s(n = h.split(E)))
                }
                if (y = b.pop()) throw new Error('Unclosed section "' + y[1] + '" at ' + m.pos);
                return c(l(I))
            }

            function l(e) {
                for (var t, i, n = [], r = 0, o = e.length; r < o; ++r) t = e[r], t && ("text" === t[0] && i && "text" === i[0] ? (i[1] += t[1], i[3] = t[3]) : (n.push(t), i = t));
                return n
            }

            function c(e) {
                for (var t, i, n = [], r = n, o = [], s = 0, a = e.length; s < a; ++s) switch (t = e[s], t[0]) {
                    case "#":
                    case "^":
                        r.push(t), o.push(t), r = t[4] = [];
                        break;
                    case "/":
                        i = o.pop(), i[5] = t[2], r = o.length > 0 ? o[o.length - 1][4] : n;
                        break;
                    default:
                        r.push(t)
                }
                return n
            }

            function u(e) {
                this.string = e, this.tail = e, this.pos = 0
            }

            function p(e, t) {
                this.view = null == e ? {} : e, this.cache = {
                    ".": this.view
                }, this.parent = t
            }

            function h() {
                this.cache = {}
            }
            var f = RegExp.prototype.test,
                d = /\S/,
                y = Object.prototype.toString,
                g = Array.isArray || function(e) {
                    return "[object Array]" === y.call(e)
                },
                m = {
                    "&": "&amp;",
                    "<": "&lt;",
                    ">": "&gt;",
                    '"': "&quot;",
                    "'": "&#39;",
                    "/": "&#x2F;"
                },
                v = /\s*/,
                E = /\s+/,
                _ = /\s*=/,
                S = /\s*\}/,
                T = /#|\^|\/|>|\{|&|=|!/;
            u.prototype.eos = function() {
                return "" === this.tail
            }, u.prototype.scan = function(e) {
                var t = this.tail.match(e);
                if (t && 0 === t.index) {
                    var i = t[0];
                    return (this.tail = this.tail.substring(i.length), this.pos += i.length, i)
                }
                return ""
            }, u.prototype.scanUntil = function(e) {
                var t, i = this.tail.search(e);
                switch (i) {
                    case -1:
                        t = this.tail, this.tail = "";
                        break;
                    case 0:
                        t = "";
                        break;
                    default:
                        t = this.tail.substring(0, i), this.tail = this.tail.substring(i)
                }
                return (this.pos += t.length, t)
            }, p.prototype.push = function(e) {
                return new p(e, this)
            }, p.prototype.lookup = function(e) {
                var t;
                if (e in this.cache) t = this.cache[e];
                else {
                    for (var i = this; i;) {
                        if (e.indexOf(".") > 0) {
                            t = i.view;
                            for (var r = e.split("."), o = 0; null != t && o < r.length;) t = t[r[o++]]
                        } else t = i.view[e];
                        if (null != t) break;
                        i = i.parent
                    }
                    this.cache[e] = t
                }
                return (n(t) && (t = t.call(this.view)), t)
            }, h.prototype.clearCache = function() {
                this.cache = {}
            }, h.prototype.parse = function(e, t) {
                var i = this.cache,
                    n = i[e];
                return (null == n && (n = i[e] = a(e, t)), n)
            }, h.prototype.render = function(e, t, i) {
                var n = this.parse(e),
                    r = t instanceof p ? t : new p(t);
                return this.renderTokens(n, r, i, e)
            }, h.prototype.renderTokens = function(t, i, r, o) {
                function s(e) {
                    return u.render(e, i, r)
                }
                for (var a, l, c = "", u = this, p = 0, h = t.length; p < h; ++p) switch (a = t[p], a[0]) {
                    case "#":
                        if (l = i.lookup(a[1]), !l) continue;
                        if (g(l))
                            for (var f = 0, d = l.length; f < d; ++f) c += this.renderTokens(a[4], i.push(l[f]), r, o);
                        else if ("object" == typeof l || "string" == typeof l) c += this.renderTokens(a[4], i.push(l), r, o);
                        else if (n(l)) {
                            if ("string" != typeof o) throw new Error("Cannot use higher-order sections without the original template");
                            l = l.call(i.view, o.slice(a[3], a[5]), s), null != l && (c += l)
                        } else c += this.renderTokens(a[4], i, r, o);
                        break;
                    case "^":
                        l = i.lookup(a[1]), (!l || g(l) && 0 === l.length) && (c += this.renderTokens(a[4], i, r, o));
                        break;
                    case ">":
                        if (!r) continue;
                        l = n(r) ? r(a[1]) : r[a[1]], null != l && (c += this.renderTokens(this.parse(l), i, r, l));
                        break;
                    case "&":
                        l = i.lookup(a[1]), null != l && (c += l);
                        break;
                    case "name":
                        l = i.lookup(a[1]), null != l && (c += e.escape(l));
                        break;
                    case "text":
                        c += a[1]
                }
                return c
            }, e.name = "mustache.js", e.version = "0.8.1", e.tags = ["{{", "}}"];
            var b = new h;
            e.clearCache = function() {
                return b.clearCache()
            }, e.parse = function(e, t) {
                return b.parse(e, t)
            }, e.render = function(e, t, i) {
                return b.render(e, t, i)
            }, e.to_html = function(t, i, r, o) {
                var s = e.render(t, i, r);
                return n(o) ? void o(s) : s
            }, e.escape = o, e.Scanner = u, e.Context = p, e.Writer = h
        })
    }, {}],
    288: [function(e, t, i) {
        (function() {
            function i(e, t, i) {
                var n = t && i || 0,
                    r = 0;
                for (t = t || [], e.toLowerCase().replace(/[0-9a-f]{2}/g, function(e) {
                        r < 16 && (t[n + r++] = f[e])
                    }); r < 16;) t[n + r++] = 0;
                return t
            }

            function n(e, t) {
                var i = t || 0,
                    n = h;
                return n[e[i++]] + n[e[i++]] + n[e[i++]] + n[e[i++]] + "-" + n[e[i++]] + n[e[i++]] + "-" + n[e[i++]] + n[e[i++]] + "-" + n[e[i++]] + n[e[i++]] + "-" + n[e[i++]] + n[e[i++]] + n[e[i++]] + n[e[i++]] + n[e[i++]] + n[e[i++]]
            }

            function r(e, t, i) {
                var r = t && i || 0,
                    o = t || [];
                e = e || {};
                var s = null != e.clockseq ? e.clockseq : m,
                    a = null != e.msecs ? e.msecs : (new Date).getTime(),
                    l = null != e.nsecs ? e.nsecs : E + 1,
                    c = a - v + (l - E) / 1e4;
                if (c < 0 && null == e.clockseq && (s = s + 1 & 16383), (c < 0 || a > v) && null == e.nsecs && (l = 0), l >= 1e4) throw new Error("uuid.v1(): Can't create more than 10M uuids/sec");
                v = a, E = l, m = s, a += 122192928e5;
                var u = (1e4 * (268435455 & a) + l) % 4294967296;
                o[r++] = u >>> 24 & 255, o[r++] = u >>> 16 & 255, o[r++] = u >>> 8 & 255, o[r++] = 255 & u;
                var p = a / 4294967296 * 1e4 & 268435455;
                o[r++] = p >>> 8 & 255, o[r++] = 255 & p, o[r++] = p >>> 24 & 15 | 16, o[r++] = p >>> 16 & 255, o[r++] = s >>> 8 | 128, o[r++] = 255 & s;
                for (var h = e.node || g, f = 0; f < 6; f++) o[r + f] = h[f];
                return t ? t : n(o)
            }

            function o(e, t, i) {
                var r = t && i || 0;
                "string" == typeof e && (t = "binary" == e ? new p(16) : null, e = null), e = e || {};
                var o = e.random || (e.rng || s)();
                if (o[6] = 15 & o[6] | 64, o[8] = 63 & o[8] | 128, t)
                    for (var a = 0; a < 16; a++) t[r + a] = o[a];
                return t || n(o)
            }
            var s, a = this;
            if ("function" == typeof e) try {
                var l = e("crypto").randomBytes;
                s = l && function() {
                    return l(16)
                }
            } catch (e) {}
            if (!s && a.crypto && crypto.getRandomValues) {
                var c = new Uint8Array(16);
                s = function() {
                    return (crypto.getRandomValues(c), c)
                }
            }
            if (!s) {
                var u = new Array(16);
                s = function() {
                    for (var e, t = 0; t < 16; t++) 0 === (3 & t) && (e = 4294967296 * Math.random()), u[t] = e >>> ((3 & t) << 3) & 255;
                    return u
                }
            }
            for (var p = "function" == typeof Buffer ? Buffer : Array, h = [], f = {}, d = 0; d < 256; d++) h[d] = (d + 256).toString(16).substr(1), f[h[d]] = d;
            var y = s(),
                g = [1 | y[0], y[1], y[2], y[3], y[4], y[5]],
                m = 16383 & (y[6] << 8 | y[7]),
                v = 0,
                E = 0,
                _ = o;
            if (_.v1 = r, _.v4 = o, _.parse = i, _.unparse = n, _.BufferClass = p, "function" == typeof define && define.amd) define(function() {
                return _
            });
            else if ("undefined" != typeof t && t.exports) t.exports = _;
            else {
                var S = a.uuid;
                _.noConflict = function() {
                    return (a.uuid = S, _)
                }, a.uuid = _
            }
        }).call(this)
    }, {
        undefined: void 0
    }],
    289: [function(e, t, i) {
        ! function(e) {
            "use strict";
            if ("function" == typeof bootstrap) bootstrap("promise", e);
            else if ("object" == typeof i && "object" == typeof t) t.exports = e();
            else if ("function" == typeof define && define.amd) define(e);
            else if ("undefined" != typeof ses) {
                if (!ses.ok()) return;
                ses.makeQ = e
            } else {
                if ("undefined" == typeof window && "undefined" == typeof self) throw new Error("This environment was not anticipated by Q. Please file a bug.");
                var n = "undefined" != typeof window ? window : self,
                    r = n.Q;
                n.Q = e(), n.Q.noConflict = function() {
                    return (n.Q = r, this)
                }
            }
        }(function() {
            "use strict";

            function e(e) {
                return function() {
                    return Y.apply(e, arguments)
                }
            }

            function t(e) {
                return e === Object(e)
            }

            function i(e) {
                return "[object StopIteration]" === te(e) || e instanceof q
            }

            function n(e, t) {
                if (V && t.stack && "object" == typeof e && null !== e && e.stack && e.stack.indexOf(ie) === -1) {
                    for (var i = [], n = t; n; n = n.source) n.stack && i.unshift(n.stack);
                    i.unshift(e.stack);
                    var o = i.join("\n" + ie + "\n");
                    e.stack = r(o)
                }
            }

            function r(e) {
                for (var t = e.split("\n"), i = [], n = 0; n < t.length; ++n) {
                    var r = t[n];
                    a(r) || o(r) || !r || i.push(r)
                }
                return i.join("\n")
            }

            function o(e) {
                return e.indexOf("(module.js:") !== -1 || e.indexOf("(node.js:") !== -1
            }

            function s(e) {
                var t = /at .+ \((.+):(\d+):(?:\d+)\)$/.exec(e);
                if (t) return [t[1], Number(t[2])];
                var i = /at ([^ ]+):(\d+):(?:\d+)$/.exec(e);
                if (i) return [i[1], Number(i[2])];
                var n = /.*@(.+):(\d+)$/.exec(e);
                return n ? [n[1], Number(n[2])] : void 0
            }

            function a(e) {
                var t = s(e);
                if (!t) return !1;
                var i = t[0],
                    n = t[1];
                return i === B && n >= j && n <= ae
            }

            function l() {
                if (V) try {
                    throw new Error
                } catch (n) {
                    var e = n.stack.split("\n"),
                        t = e[0].indexOf("@") > 0 ? e[1] : e[2],
                        i = s(t);
                    if (!i) return;
                    return (B = i[0], i[1])
                }
            }

            function c(e, t, i) {
                return function() {
                    return ("undefined" != typeof console && "function" == typeof console.warn && console.warn(t + " is deprecated, use " + i + " instead.", new Error("").stack), e.apply(e, arguments))
                }
            }

            function u(e) {
                return e instanceof d ? e : v(e) ? C(e) : R(e)
            }

            function p() {
                function e(e) {
                    t = e, o.source = e, X(i, function(t, i) {
                        u.nextTick(function() {
                            e.promiseDispatch.apply(e, i)
                        })
                    }, void 0), i = void 0, n = void 0
                }
                var t, i = [],
                    n = [],
                    r = Z(p.prototype),
                    o = Z(d.prototype);
                if (o.promiseDispatch = function(e, r, o) {
                        var s = K(arguments);
                        i ? (i.push(s), "when" === r && o[1] && n.push(o[1])) : u.nextTick(function() {
                            t.promiseDispatch.apply(t, s)
                        })
                    }, o.valueOf = function() {
                        if (i) return o;
                        var e = g(t);
                        return (m(e) && (t = e), e)
                    }, o.inspect = function() {
                        return t ? t.inspect() : {
                            state: "pending"
                        }
                    }, u.longStackSupport && V) try {
                    throw new Error
                } catch (e) {
                    o.stack = e.stack.substring(e.stack.indexOf("\n") + 1)
                }
                return (r.promise = o, r.resolve = function(i) {
                    t || e(u(i))
                }, r.fulfill = function(i) {
                    t || e(R(i))
                }, r.reject = function(i) {
                    t || e(w(i))
                }, r.notify = function(e) {
                    t || X(n, function(t, i) {
                        u.nextTick(function() {
                            i(e)
                        })
                    }, void 0)
                }, r)
            }

            function h(e) {
                if ("function" != typeof e) throw new TypeError("resolver must be a function.");
                var t = p();
                try {
                    e(t.resolve, t.reject, t.notify)
                } catch (e) {
                    t.reject(e)
                }
                return t.promise
            }

            function f(e) {
                return h(function(t, i) {
                    for (var n = 0, r = e.length; n < r; n++) u(e[n]).then(t, i)
                })
            }

            function d(e, t, i) {
                void 0 === t && (t = function(e) {
                    return w(new Error("Promise does not support operation: " + e));
                }), void 0 === i && (i = function() {
                    return {
                        state: "unknown"
                    }
                });
                var n = Z(d.prototype);
                if (n.promiseDispatch = function(i, r, o) {
                        var s;
                        try {
                            s = e[r] ? e[r].apply(n, o) : t.call(n, r, o)
                        } catch (e) {
                            s = w(e)
                        }
                        i && i(s)
                    }, n.inspect = i, i) {
                    var r = i();
                    "rejected" === r.state && (n.exception = r.reason), n.valueOf = function() {
                        var e = i();
                        return "pending" === e.state || "rejected" === e.state ? n : e.value
                    }
                }
                return n
            }

            function y(e, t, i, n) {
                return u(e).then(t, i, n)
            }

            function g(e) {
                if (m(e)) {
                    var t = e.inspect();
                    if ("fulfilled" === t.state) return t.value
                }
                return e
            }

            function m(e) {
                return e instanceof d
            }

            function v(e) {
                return t(e) && "function" == typeof e.then
            }

            function E(e) {
                return m(e) && "pending" === e.inspect().state
            }

            function _(e) {
                return !m(e) || "fulfilled" === e.inspect().state
            }

            function S(e) {
                return m(e) && "rejected" === e.inspect().state
            }

            function T() {
                ne.length = 0, re.length = 0, se || (se = !0)
            }

            function b(e, t) {
                se && ("object" == typeof process && "function" == typeof process.emit && u.nextTick.runAfter(function() {
                    Q(re, e) !== -1 && (process.emit("unhandledRejection", t, e), oe.push(e))
                }), re.push(e), t && "undefined" != typeof t.stack ? ne.push(t.stack) : ne.push("(no stack) " + t))
            }

            function I(e) {
                if (se) {
                    var t = Q(re, e);
                    t !== -1 && ("object" == typeof process && "function" == typeof process.emit && u.nextTick.runAfter(function() {
                        var i = Q(oe, e);
                        i !== -1 && (process.emit("rejectionHandled", ne[t], e), oe.splice(i, 1))
                    }), re.splice(t, 1), ne.splice(t, 1))
                }
            }

            function w(e) {
                var t = d({
                    when: function(t) {
                        return (t && I(this), t ? t(e) : this)
                    }
                }, function() {
                    return this
                }, function() {
                    return {
                        state: "rejected",
                        reason: e
                    }
                });
                return (b(t, e), t)
            }

            function R(e) {
                return d({
                    when: function() {
                        return e
                    },
                    get: function(t) {
                        return e[t]
                    },
                    set: function(t, i) {
                        e[t] = i
                    },
                    "delete": function(t) {
                        delete e[t]
                    },
                    post: function(t, i) {
                        return null === t || void 0 === t ? e.apply(void 0, i) : e[t].apply(e, i)
                    },
                    apply: function(t, i) {
                        return e.apply(t, i)
                    },
                    keys: function() {
                        return ee(e)
                    }
                }, void 0, function() {
                    return {
                        state: "fulfilled",
                        value: e
                    }
                })
            }

            function C(e) {
                var t = p();
                return (u.nextTick(function() {
                    try {
                        e.then(t.resolve, t.reject, t.notify)
                    } catch (e) {
                        t.reject(e)
                    }
                }), t.promise)
            }

            function P(e) {
                return d({
                    isDef: function() {}
                }, function(t, i) {
                    return L(e, t, i)
                }, function() {
                    return u(e).inspect()
                })
            }

            function A(e, t, i) {
                return u(e).spread(t, i)
            }

            function O(e) {
                return function() {
                    function t(e, t) {
                        var s;
                        if ("undefined" == typeof StopIteration) {
                            try {
                                s = n[e](t)
                            } catch (e) {
                                return w(e)
                            }
                            return s.done ? u(s.value) : y(s.value, r, o)
                        }
                        try {
                            s = n[e](t)
                        } catch (e) {
                            return i(e) ? u(e.value) : w(e)
                        }
                        return y(s, r, o)
                    }
                    var n = e.apply(this, arguments),
                        r = t.bind(t, "next"),
                        o = t.bind(t, "throw");
                    return r()
                }
            }

            function N(e) {
                u.done(u.async(e)())
            }

            function x(e) {
                throw new q(e)
            }

            function M(e) {
                return function() {
                    return A([this, k(arguments)], function(t, i) {
                        return e.apply(t, i)
                    })
                }
            }

            function L(e, t, i) {
                return u(e).dispatch(t, i)
            }

            function k(e) {
                return y(e, function(e) {
                    var t = 0,
                        i = p();
                    return (X(e, function(n, r, o) {
                        var s;
                        m(r) && "fulfilled" === (s = r.inspect()).state ? e[o] = s.value : (++t, y(r, function(n) {
                            e[o] = n, 0 === --t && i.resolve(e)
                        }, i.reject, function(e) {
                            i.notify({
                                index: o,
                                value: e
                            })
                        }))
                    }, void 0), 0 === t && i.resolve(e), i.promise)
                })
            }

            function D(e) {
                if (0 === e.length) return u.resolve();
                var t = u.defer(),
                    i = 0;
                return (X(e, function(n, r, o) {
                    function s(e) {
                        t.resolve(e)
                    }

                    function a() {
                        i--, 0 === i && t.reject(new Error("Can't get fulfillment value from any promise, all promises were rejected."))
                    }

                    function l(e) {
                        t.notify({
                            index: o,
                            value: e
                        })
                    }
                    var c = e[o];
                    i++, y(c, s, a, l)
                }, void 0), t.promise)
            }

            function U(e) {
                return y(e, function(e) {
                    return (e = J(e, u), y(k(J(e, function(e) {
                        return y(e, z, z)
                    })), function() {
                        return e
                    }))
                })
            }

            function F(e) {
                return u(e).allSettled()
            }

            function H(e, t) {
                return u(e).then(void 0, void 0, t)
            }

            function W(e, t) {
                return u(e).nodeify(t)
            }
            var V = !1;
            try {
                throw new Error
            } catch (e) {
                V = !!e.stack
            }
            var B, q, j = l(),
                z = function() {},
                G = function() {
                    function e() {
                        for (var e, n; i.next;) i = i.next, e = i.task, i.task = void 0, n = i.domain, n && (i.domain = void 0, n.enter()), t(e, n);
                        for (; a.length;) e = a.pop(), t(e);
                        r = !1
                    }

                    function t(t, i) {
                        try {
                            t()
                        } catch (t) {
                            if (s) throw (i && i.exit(), setTimeout(e, 0), i && i.enter(), t);
                            setTimeout(function() {
                                throw t
                            }, 0)
                        }
                        i && i.exit()
                    }
                    var i = {
                            task: void 0,
                            next: null
                        },
                        n = i,
                        r = !1,
                        o = void 0,
                        s = !1,
                        a = [];
                    if (G = function(e) {
                            n = n.next = {
                                task: e,
                                domain: s && process.domain,
                                next: null
                            }, r || (r = !0, o())
                        }, "object" == typeof process && "[object process]" === process.toString() && process.nextTick) s = !0, o = function() {
                        process.nextTick(e)
                    };
                    else if ("function" == typeof setImmediate) o = "undefined" != typeof window ? setImmediate.bind(window, e) : function() {
                        setImmediate(e)
                    };
                    else if ("undefined" != typeof MessageChannel) {
                        var l = new MessageChannel;
                        l.port1.onmessage = function() {
                            o = c, l.port1.onmessage = e, e()
                        };
                        var c = function() {
                            l.port2.postMessage(0)
                        };
                        o = function() {
                            setTimeout(e, 0), c()
                        }
                    } else o = function() {
                        setTimeout(e, 0)
                    };
                    return (G.runAfter = function(e) {
                        a.push(e), r || (r = !0, o())
                    }, G)
                }(),
                Y = Function.call,
                K = e(Array.prototype.slice),
                X = e(Array.prototype.reduce || function(e, t) {
                    var i = 0,
                        n = this.length;
                    if (1 === arguments.length)
                        for (;;) {
                            if (i in this) {
                                t = this[i++];
                                break
                            }
                            if (++i >= n) throw new TypeError
                        }
                    for (; i < n; i++) i in this && (t = e(t, this[i], i));
                    return t
                }),
                Q = e(Array.prototype.indexOf || function(e) {
                    for (var t = 0; t < this.length; t++)
                        if (this[t] === e) return t;
                    return -1
                }),
                J = e(Array.prototype.map || function(e, t) {
                    var i = this,
                        n = [];
                    return (X(i, function(r, o, s) {
                        n.push(e.call(t, o, s, i))
                    }, void 0), n)
                }),
                Z = Object.create || function(e) {
                    function t() {}
                    return (t.prototype = e, new t)
                },
                $ = e(Object.prototype.hasOwnProperty),
                ee = Object.keys || function(e) {
                    var t = [];
                    for (var i in e) $(e, i) && t.push(i);
                    return t
                },
                te = e(Object.prototype.toString);
            q = "undefined" != typeof ReturnValue ? ReturnValue : function(e) {
                this.value = e
            };
            var ie = "From previous event:";
            u.resolve = u, u.nextTick = G, u.longStackSupport = !1, "object" == typeof process && process && process.env && process.env.Q_DEBUG && (u.longStackSupport = !0), u.defer = p, p.prototype.makeNodeResolver = function() {
                var e = this;
                return function(t, i) {
                    t ? e.reject(t) : arguments.length > 2 ? e.resolve(K(arguments, 1)) : e.resolve(i)
                }
            }, u.Promise = h, u.promise = h, h.race = f, h.all = k, h.reject = w, h.resolve = u, u.passByCopy = function(e) {
                return e
            }, d.prototype.passByCopy = function() {
                return this
            }, u.join = function(e, t) {
                return u(e).join(t)
            }, d.prototype.join = function(e) {
                return u([this, e]).spread(function(e, t) {
                    if (e === t) return e;
                    throw new Error("Can't join: not the same: " + e + " " + t)
                })
            }, u.race = f, d.prototype.race = function() {
                return this.then(u.race)
            }, u.makePromise = d, d.prototype.toString = function() {
                return "[object Promise]"
            }, d.prototype.then = function(e, t, i) {
                function r(t) {
                    try {
                        return "function" == typeof e ? e(t) : t
                    } catch (e) {
                        return w(e)
                    }
                }

                function o(e) {
                    if ("function" == typeof t) {
                        n(e, a);
                        try {
                            return t(e)
                        } catch (e) {
                            return w(e)
                        }
                    }
                    return w(e)
                }

                function s(e) {
                    return "function" == typeof i ? i(e) : e
                }
                var a = this,
                    l = p(),
                    c = !1;
                return (u.nextTick(function() {
                    a.promiseDispatch(function(e) {
                        c || (c = !0, l.resolve(r(e)))
                    }, "when", [function(e) {
                        c || (c = !0, l.resolve(o(e)))
                    }])
                }), a.promiseDispatch(void 0, "when", [void 0, function(e) {
                    var t, i = !1;
                    try {
                        t = s(e)
                    } catch (e) {
                        if (i = !0, !u.onerror) throw e;
                        u.onerror(e)
                    }
                    i || l.notify(t)
                }]), l.promise)
            }, u.tap = function(e, t) {
                return u(e).tap(t)
            }, d.prototype.tap = function(e) {
                return (e = u(e), this.then(function(t) {
                    return e.fcall(t).thenResolve(t)
                }))
            }, u.when = y, d.prototype.thenResolve = function(e) {
                return this.then(function() {
                    return e
                })
            }, u.thenResolve = function(e, t) {
                return u(e).thenResolve(t)
            }, d.prototype.thenReject = function(e) {
                return this.then(function() {
                    throw e
                })
            }, u.thenReject = function(e, t) {
                return u(e).thenReject(t)
            }, u.nearer = g, u.isPromise = m, u.isPromiseAlike = v, u.isPending = E, d.prototype.isPending = function() {
                return "pending" === this.inspect().state
            }, u.isFulfilled = _, d.prototype.isFulfilled = function() {
                return "fulfilled" === this.inspect().state
            }, u.isRejected = S, d.prototype.isRejected = function() {
                return "rejected" === this.inspect().state
            };
            var ne = [],
                re = [],
                oe = [],
                se = !0;
            u.resetUnhandledRejections = T, u.getUnhandledReasons = function() {
                return ne.slice()
            }, u.stopUnhandledRejectionTracking = function() {
                T(), se = !1
            }, T(), u.reject = w, u.fulfill = R, u.master = P, u.spread = A, d.prototype.spread = function(e, t) {
                return this.all().then(function(t) {
                    return e.apply(void 0, t)
                }, t)
            }, u.async = O, u.spawn = N, u["return"] = x, u.promised = M, u.dispatch = L, d.prototype.dispatch = function(e, t) {
                var i = this,
                    n = p();
                return (u.nextTick(function() {
                    i.promiseDispatch(n.resolve, e, t)
                }), n.promise)
            }, u.get = function(e, t) {
                return u(e).dispatch("get", [t])
            }, d.prototype.get = function(e) {
                return this.dispatch("get", [e])
            }, u.set = function(e, t, i) {
                return u(e).dispatch("set", [t, i])
            }, d.prototype.set = function(e, t) {
                return this.dispatch("set", [e, t])
            }, u.del = u["delete"] = function(e, t) {
                return u(e).dispatch("delete", [t])
            }, d.prototype.del = d.prototype["delete"] = function(e) {
                return this.dispatch("delete", [e])
            }, u.mapply = u.post = function(e, t, i) {
                return u(e).dispatch("post", [t, i])
            }, d.prototype.mapply = d.prototype.post = function(e, t) {
                return this.dispatch("post", [e, t])
            }, u.send = u.mcall = u.invoke = function(e, t) {
                return u(e).dispatch("post", [t, K(arguments, 2)])
            }, d.prototype.send = d.prototype.mcall = d.prototype.invoke = function(e) {
                return this.dispatch("post", [e, K(arguments, 1)])
            }, u.fapply = function(e, t) {
                return u(e).dispatch("apply", [void 0, t])
            }, d.prototype.fapply = function(e) {
                return this.dispatch("apply", [void 0, e])
            }, u["try"] = u.fcall = function(e) {
                return u(e).dispatch("apply", [void 0, K(arguments, 1)])
            }, d.prototype.fcall = function() {
                return this.dispatch("apply", [void 0, K(arguments)])
            }, u.fbind = function(e) {
                var t = u(e),
                    i = K(arguments, 1);
                return function() {
                    return t.dispatch("apply", [this, i.concat(K(arguments))])
                }
            }, d.prototype.fbind = function() {
                var e = this,
                    t = K(arguments);
                return function() {
                    return e.dispatch("apply", [this, t.concat(K(arguments))])
                }
            }, u.keys = function(e) {
                return u(e).dispatch("keys", [])
            }, d.prototype.keys = function() {
                return this.dispatch("keys", [])
            }, u.all = k, d.prototype.all = function() {
                return k(this)
            }, u.any = D, d.prototype.any = function() {
                return D(this)
            }, u.allResolved = c(U, "allResolved", "allSettled"), d.prototype.allResolved = function() {
                return U(this)
            }, u.allSettled = F, d.prototype.allSettled = function() {
                return this.then(function(e) {
                    return k(J(e, function(e) {
                        function t() {
                            return e.inspect()
                        }
                        return (e = u(e), e.then(t, t))
                    }))
                })
            }, u.fail = u["catch"] = function(e, t) {
                return u(e).then(void 0, t)
            }, d.prototype.fail = d.prototype["catch"] = function(e) {
                return this.then(void 0, e)
            }, u.progress = H, d.prototype.progress = function(e) {
                return this.then(void 0, void 0, e)
            }, u.fin = u["finally"] = function(e, t) {
                return u(e)["finally"](t)
            }, d.prototype.fin = d.prototype["finally"] = function(e) {
                return (e = u(e), this.then(function(t) {
                    return e.fcall().then(function() {
                        return t
                    })
                }, function(t) {
                    return e.fcall().then(function() {
                        throw t
                    })
                }))
            }, u.done = function(e, t, i, n) {
                return u(e).done(t, i, n)
            }, d.prototype.done = function(e, t, i) {
                var r = function(e) {
                        u.nextTick(function() {
                            if (n(e, o), !u.onerror) throw e;
                            u.onerror(e)
                        })
                    },
                    o = e || t || i ? this.then(e, t, i) : this;
                "object" == typeof process && process && process.domain && (r = process.domain.bind(r)), o.then(void 0, r)
            }, u.timeout = function(e, t, i) {
                return u(e).timeout(t, i)
            }, d.prototype.timeout = function(e, t) {
                var i = p(),
                    n = setTimeout(function() {
                        t && "string" != typeof t || (t = new Error(t || "Timed out after " + e + " ms"), t.code = "ETIMEDOUT"), i.reject(t)
                    }, e);
                return (this.then(function(e) {
                    clearTimeout(n), i.resolve(e)
                }, function(e) {
                    clearTimeout(n), i.reject(e)
                }, i.notify), i.promise)
            }, u.delay = function(e, t) {
                return (void 0 === t && (t = e, e = void 0), u(e).delay(t))
            }, d.prototype.delay = function(e) {
                return this.then(function(t) {
                    var i = p();
                    return (setTimeout(function() {
                        i.resolve(t)
                    }, e), i.promise)
                })
            }, u.nfapply = function(e, t) {
                return u(e).nfapply(t)
            }, d.prototype.nfapply = function(e) {
                var t = p(),
                    i = K(e);
                return (i.push(t.makeNodeResolver()), this.fapply(i).fail(t.reject), t.promise)
            }, u.nfcall = function(e) {
                var t = K(arguments, 1);
                return u(e).nfapply(t)
            }, d.prototype.nfcall = function() {
                var e = K(arguments),
                    t = p();
                return (e.push(t.makeNodeResolver()), this.fapply(e).fail(t.reject), t.promise)
            }, u.nfbind = u.denodeify = function(e) {
                var t = K(arguments, 1);
                return function() {
                    var i = t.concat(K(arguments)),
                        n = p();
                    return (i.push(n.makeNodeResolver()), u(e).fapply(i).fail(n.reject), n.promise)
                }
            }, d.prototype.nfbind = d.prototype.denodeify = function() {
                var e = K(arguments);
                return (e.unshift(this), u.denodeify.apply(void 0, e))
            }, u.nbind = function(e, t) {
                var i = K(arguments, 2);
                return function() {
                    function n() {
                        return e.apply(t, arguments)
                    }
                    var r = i.concat(K(arguments)),
                        o = p();
                    return (r.push(o.makeNodeResolver()), u(n).fapply(r).fail(o.reject), o.promise)
                }
            }, d.prototype.nbind = function() {
                var e = K(arguments, 0);
                return (e.unshift(this), u.nbind.apply(void 0, e))
            }, u.nmapply = u.npost = function(e, t, i) {
                return u(e).npost(t, i)
            }, d.prototype.nmapply = d.prototype.npost = function(e, t) {
                var i = K(t || []),
                    n = p();
                return (i.push(n.makeNodeResolver()), this.dispatch("post", [e, i]).fail(n.reject), n.promise)
            }, u.nsend = u.nmcall = u.ninvoke = function(e, t) {
                var i = K(arguments, 2),
                    n = p();
                return (i.push(n.makeNodeResolver()), u(e).dispatch("post", [t, i]).fail(n.reject), n.promise)
            }, d.prototype.nsend = d.prototype.nmcall = d.prototype.ninvoke = function(e) {
                var t = K(arguments, 1),
                    i = p();
                return (t.push(i.makeNodeResolver()), this.dispatch("post", [e, t]).fail(i.reject), i.promise)
            }, u.nodeify = W, d.prototype.nodeify = function(e) {
                return e ? void this.then(function(t) {
                    u.nextTick(function() {
                        e(null, t)
                    })
                }, function(t) {
                    u.nextTick(function() {
                        e(t)
                    })
                }) : this
            }, u.noConflict = function() {
                throw new Error("Q.noConflict only works when Q is used as a global")
            };
            var ae = l();
            return u
        })
    }, {}],
    290: [function(e, t, i) {
        var n = e(292),
            r = e(291);
        t.exports = {
            stringify: n,
            parse: r
        }
    }, {
        291: 291,
        292: 292
    }],
    291: [function(e, t, i) {
        var n = e(293),
            r = {
                delimiter: "&",
                depth: 5,
                arrayLimit: 20,
                parameterLimit: 1e3,
                strictNullHandling: !1,
                plainObjects: !1,
                allowPrototypes: !1,
                allowDots: !1
            };
        r.parseValues = function(e, t) {
            for (var i = {}, r = e.split(t.delimiter, t.parameterLimit === 1 / 0 ? void 0 : t.parameterLimit), o = 0, s = r.length; o < s; ++o) {
                var a = r[o],
                    l = a.indexOf("]=") === -1 ? a.indexOf("=") : a.indexOf("]=") + 1;
                if (l === -1) i[n.decode(a)] = "", t.strictNullHandling && (i[n.decode(a)] = null);
                else {
                    var c = n.decode(a.slice(0, l)),
                        u = n.decode(a.slice(l + 1));
                    Object.prototype.hasOwnProperty.call(i, c) ? i[c] = [].concat(i[c]).concat(u) : i[c] = u
                }
            }
            return i
        }, r.parseObject = function(e, t, i) {
            if (!e.length) return t;
            var n, o = e.shift();
            if ("[]" === o) n = [], n = n.concat(r.parseObject(e, t, i));
            else {
                n = i.plainObjects ? Object.create(null) : {};
                var s = "[" === o[0] && "]" === o[o.length - 1] ? o.slice(1, o.length - 1) : o,
                    a = parseInt(s, 10),
                    l = "" + a;
                !isNaN(a) && o !== s && l === s && a >= 0 && i.parseArrays && a <= i.arrayLimit ? (n = [], n[a] = r.parseObject(e, t, i)) : n[s] = r.parseObject(e, t, i)
            }
            return n
        }, r.parseKeys = function(e, t, i) {
            if (e) {
                i.allowDots && (e = e.replace(/\.([^\.\[]+)/g, "[$1]"));
                var n = /^([^\[\]]*)/,
                    o = /(\[[^\[\]]*\])/g,
                    s = n.exec(e),
                    a = [];
                if (s[1]) {
                    if (!i.plainObjects && Object.prototype.hasOwnProperty(s[1]) && !i.allowPrototypes) return;
                    a.push(s[1])
                }
                for (var l = 0; null !== (s = o.exec(e)) && l < i.depth;) ++l, (i.plainObjects || !Object.prototype.hasOwnProperty(s[1].replace(/\[|\]/g, "")) || i.allowPrototypes) && a.push(s[1]);
                return (s && a.push("[" + e.slice(s.index) + "]"), r.parseObject(a, t, i))
            }
        }, t.exports = function(e, t) {
            if (t = t || {}, t.delimiter = "string" == typeof t.delimiter || n.isRegExp(t.delimiter) ? t.delimiter : r.delimiter, t.depth = "number" == typeof t.depth ? t.depth : r.depth, t.arrayLimit = "number" == typeof t.arrayLimit ? t.arrayLimit : r.arrayLimit, t.parseArrays = t.parseArrays !== !1, t.allowDots = "boolean" == typeof t.allowDots ? t.allowDots : r.allowDots, t.plainObjects = "boolean" == typeof t.plainObjects ? t.plainObjects : r.plainObjects, t.allowPrototypes = "boolean" == typeof t.allowPrototypes ? t.allowPrototypes : r.allowPrototypes, t.parameterLimit = "number" == typeof t.parameterLimit ? t.parameterLimit : r.parameterLimit, t.strictNullHandling = "boolean" == typeof t.strictNullHandling ? t.strictNullHandling : r.strictNullHandling, "" === e || null === e || "undefined" == typeof e) return t.plainObjects ? Object.create(null) : {};
            for (var i = "string" == typeof e ? r.parseValues(e, t) : e, o = t.plainObjects ? Object.create(null) : {}, s = Object.keys(i), a = 0, l = s.length; a < l; ++a) {
                var c = s[a],
                    u = r.parseKeys(c, i[c], t);
                o = n.merge(o, u, t)
            }
            return n.compact(o)
        }
    }, {
        293: 293
    }],
    292: [function(e, t, i) {
        var n = e(293),
            r = {
                delimiter: "&",
                arrayPrefixGenerators: {
                    brackets: function(e, t) {
                        return e + "[]"
                    },
                    indices: function(e, t) {
                        return e + "[" + t + "]"
                    },
                    repeat: function(e, t) {
                        return e
                    }
                },
                strictNullHandling: !1,
                skipNulls: !1,
                encode: !0
            };
        r.stringify = function(e, t, i, o, s, a, l, c) {
            if ("function" == typeof l) e = l(t, e);
            else if (n.isBuffer(e)) e = e.toString();
            else if (e instanceof Date) e = e.toISOString();
            else if (null === e) {
                if (o) return a ? n.encode(t) : t;
                e = ""
            }
            if ("string" == typeof e || "number" == typeof e || "boolean" == typeof e) return a ? [n.encode(t) + "=" + n.encode(e)] : [t + "=" + e];
            var u = [];
            if ("undefined" == typeof e) return u;
            var p;
            if (Array.isArray(l)) p = l;
            else {
                var h = Object.keys(e);
                p = c ? h.sort(c) : h
            }
            for (var f = 0, d = p.length; f < d; ++f) {
                var y = p[f];
                s && null === e[y] || (u = Array.isArray(e) ? u.concat(r.stringify(e[y], i(t, y), i, o, s, a, l)) : u.concat(r.stringify(e[y], t + "[" + y + "]", i, o, s, a, l)))
            }
            return u
        }, t.exports = function(e, t) {
            t = t || {};
            var i, n, o = "undefined" == typeof t.delimiter ? r.delimiter : t.delimiter,
                s = "boolean" == typeof t.strictNullHandling ? t.strictNullHandling : r.strictNullHandling,
                a = "boolean" == typeof t.skipNulls ? t.skipNulls : r.skipNulls,
                l = "boolean" == typeof t.encode ? t.encode : r.encode,
                c = "function" == typeof t.sort ? t.sort : null;
            "function" == typeof t.filter ? (n = t.filter, e = n("", e)) : Array.isArray(t.filter) && (i = n = t.filter);
            var u = [];
            if ("object" != typeof e || null === e) return "";
            var p;
            p = t.arrayFormat in r.arrayPrefixGenerators ? t.arrayFormat : "indices" in t ? t.indices ? "indices" : "repeat" : "indices";
            var h = r.arrayPrefixGenerators[p];
            i || (i = Object.keys(e)), c && i.sort(c);
            for (var f = 0, d = i.length; f < d; ++f) {
                var y = i[f];
                a && null === e[y] || (u = u.concat(r.stringify(e[y], y, h, s, a, l, n, c)))
            }
            return u.join(o)
        }
    }, {
        293: 293
    }],
    293: [function(e, t, i) {
        var n = {};
        n.hexTable = new Array(256);
        for (var r = 0; r < 256; ++r) n.hexTable[r] = "%" + ((r < 16 ? "0" : "") + r.toString(16)).toUpperCase();
        i.arrayToObject = function(e, t) {
            for (var i = t.plainObjects ? Object.create(null) : {}, n = 0, r = e.length; n < r; ++n) "undefined" != typeof e[n] && (i[n] = e[n]);
            return i
        }, i.merge = function(e, t, n) {
            if (!t) return e;
            if ("object" != typeof t) return (Array.isArray(e) ? e.push(t) : "object" == typeof e ? e[t] = !0 : e = [e, t], e);
            if ("object" != typeof e) return e = [e].concat(t);
            Array.isArray(e) && !Array.isArray(t) && (e = i.arrayToObject(e, n));
            for (var r = Object.keys(t), o = 0, s = r.length; o < s; ++o) {
                var a = r[o],
                    l = t[a];
                Object.prototype.hasOwnProperty.call(e, a) ? e[a] = i.merge(e[a], l, n) : e[a] = l
            }
            return e
        }, i.decode = function(e) {
            try {
                return decodeURIComponent(e.replace(/\+/g, " "))
            } catch (t) {
                return e
            }
        }, i.encode = function(e) {
            if (0 === e.length) return e;
            "string" != typeof e && (e = "" + e);
            for (var t = "", i = 0, r = e.length; i < r; ++i) {
                var o = e.charCodeAt(i);
                45 === o || 46 === o || 95 === o || 126 === o || o >= 48 && o <= 57 || o >= 65 && o <= 90 || o >= 97 && o <= 122 ? t += e[i] : o < 128 ? t += n.hexTable[o] : o < 2048 ? t += n.hexTable[192 | o >> 6] + n.hexTable[128 | 63 & o] : o < 55296 || o >= 57344 ? t += n.hexTable[224 | o >> 12] + n.hexTable[128 | o >> 6 & 63] + n.hexTable[128 | 63 & o] : (++i, o = 65536 + ((1023 & o) << 10 | 1023 & e.charCodeAt(i)), t += n.hexTable[240 | o >> 18] + n.hexTable[128 | o >> 12 & 63] + n.hexTable[128 | o >> 6 & 63] + n.hexTable[128 | 63 & o])
            }
            return t
        }, i.compact = function(e, t) {
            if ("object" != typeof e || null === e) return e;
            t = t || [];
            var n = t.indexOf(e);
            if (n !== -1) return t[n];
            if (t.push(e), Array.isArray(e)) {
                for (var r = [], o = 0, s = e.length; o < s; ++o) "undefined" != typeof e[o] && r.push(e[o]);
                return r
            }
            var a = Object.keys(e);
            for (o = 0, s = a.length; o < s; ++o) {
                var l = a[o];
                e[l] = i.compact(e[l], t)
            }
            return e
        }, i.isRegExp = function(e) {
            return "[object RegExp]" === Object.prototype.toString.call(e)
        }, i.isBuffer = function(e) {
            return null !== e && "undefined" != typeof e && !!(e.constructor && e.constructor.isBuffer && e.constructor.isBuffer(e))
        }
    }, {}],
    294: [function(e, t, i) {
        (function() {
            function e(e) {
                function t(t, i, n, r, o, s) {
                    for (; o >= 0 && o < s; o += e) {
                        var a = r ? r[o] : o;
                        n = i(n, t[a], a, t)
                    }
                    return n
                }
                return function(i, n, r, o) {
                    n = S(n, o, 4);
                    var s = !P(i) && _.keys(i),
                        a = (s || i).length,
                        l = e > 0 ? 0 : a - 1;
                    return (arguments.length < 3 && (r = i[s ? s[l] : l], l += e), t(i, n, r, s, l, a))
                }
            }

            function n(e) {
                return function(t, i, n) {
                    i = T(i, n);
                    for (var r = C(t), o = e > 0 ? 0 : r - 1; o >= 0 && o < r; o += e)
                        if (i(t[o], o, t)) return o;
                    return -1
                }
            }

            function r(e, t, i) {
                return function(n, r, o) {
                    var s = 0,
                        a = C(n);
                    if ("number" == typeof o) e > 0 ? s = o >= 0 ? o : Math.max(o + a, s) : a = o >= 0 ? Math.min(o + 1, a) : o + a + 1;
                    else if (i && o && a) return (o = i(n, r), n[o] === r ? o : -1);
                    if (r !== r) return (o = t(h.call(n, s, a), _.isNaN), o >= 0 ? o + s : -1);
                    for (o = e > 0 ? s : a - 1; o >= 0 && o < a; o += e)
                        if (n[o] === r) return o;
                    return -1
                }
            }

            function o(e, t) {
                var i = M.length,
                    n = e.constructor,
                    r = _.isFunction(n) && n.prototype || c,
                    o = "constructor";
                for (_.has(e, o) && !_.contains(t, o) && t.push(o); i--;) o = M[i], o in e && e[o] !== r[o] && !_.contains(t, o) && t.push(o)
            }
            var s = this,
                a = s._,
                l = Array.prototype,
                c = Object.prototype,
                u = Function.prototype,
                p = l.push,
                h = l.slice,
                f = c.toString,
                d = c.hasOwnProperty,
                y = Array.isArray,
                g = Object.keys,
                m = u.bind,
                v = Object.create,
                E = function() {},
                _ = function(e) {
                    return e instanceof _ ? e : this instanceof _ ? void(this._wrapped = e) : new _(e)
                };
            "undefined" != typeof i ? ("undefined" != typeof t && t.exports && (i = t.exports = _), i._ = _) : s._ = _, _.VERSION = "1.8.3";
            var S = function(e, t, i) {
                    if (void 0 === t) return e;
                    switch (null == i ? 3 : i) {
                        case 1:
                            return function(i) {
                                return e.call(t, i)
                            };
                        case 2:
                            return function(i, n) {
                                return e.call(t, i, n)
                            };
                        case 3:
                            return function(i, n, r) {
                                return e.call(t, i, n, r)
                            };
                        case 4:
                            return function(i, n, r, o) {
                                return e.call(t, i, n, r, o)
                            }
                    }
                    return function() {
                        return e.apply(t, arguments)
                    }
                },
                T = function(e, t, i) {
                    return null == e ? _.identity : _.isFunction(e) ? S(e, t, i) : _.isObject(e) ? _.matcher(e) : _.property(e)
                };
            _.iteratee = function(e, t) {
                return T(e, t, 1 / 0)
            };
            var b = function(e, t) {
                    return function(i) {
                        var n = arguments.length;
                        if (n < 2 || null == i) return i;
                        for (var r = 1; r < n; r++)
                            for (var o = arguments[r], s = e(o), a = s.length, l = 0; l < a; l++) {
                                var c = s[l];
                                t && void 0 !== i[c] || (i[c] = o[c])
                            }
                        return i
                    }
                },
                I = function(e) {
                    if (!_.isObject(e)) return {};
                    if (v) return v(e);
                    E.prototype = e;
                    var t = new E;
                    return (E.prototype = null, t)
                },
                w = function(e) {
                    return function(t) {
                        return null == t ? void 0 : t[e]
                    }
                },
                R = Math.pow(2, 53) - 1,
                C = w("length"),
                P = function(e) {
                    var t = C(e);
                    return "number" == typeof t && t >= 0 && t <= R
                };
            _.each = _.forEach = function(e, t, i) {
                t = S(t, i);
                var n, r;
                if (P(e))
                    for (n = 0, r = e.length; n < r; n++) t(e[n], n, e);
                else {
                    var o = _.keys(e);
                    for (n = 0, r = o.length; n < r; n++) t(e[o[n]], o[n], e)
                }
                return e
            }, _.map = _.collect = function(e, t, i) {
                t = T(t, i);
                for (var n = !P(e) && _.keys(e), r = (n || e).length, o = Array(r), s = 0; s < r; s++) {
                    var a = n ? n[s] : s;
                    o[s] = t(e[a], a, e)
                }
                return o
            }, _.reduce = _.foldl = _.inject = e(1), _.reduceRight = _.foldr = e(-1), _.find = _.detect = function(e, t, i) {
                var n;
                if (n = P(e) ? _.findIndex(e, t, i) : _.findKey(e, t, i), void 0 !== n && n !== -1) return e[n]
            }, _.filter = _.select = function(e, t, i) {
                var n = [];
                return (t = T(t, i), _.each(e, function(e, i, r) {
                    t(e, i, r) && n.push(e)
                }), n)
            }, _.reject = function(e, t, i) {
                return _.filter(e, _.negate(T(t)), i)
            }, _.every = _.all = function(e, t, i) {
                t = T(t, i);
                for (var n = !P(e) && _.keys(e), r = (n || e).length, o = 0; o < r; o++) {
                    var s = n ? n[o] : o;
                    if (!t(e[s], s, e)) return !1
                }
                return !0
            }, _.some = _.any = function(e, t, i) {
                t = T(t, i);
                for (var n = !P(e) && _.keys(e), r = (n || e).length, o = 0; o < r; o++) {
                    var s = n ? n[o] : o;
                    if (t(e[s], s, e)) return !0
                }
                return !1
            }, _.contains = _.includes = _.include = function(e, t, i, n) {
                return (P(e) || (e = _.values(e)), ("number" != typeof i || n) && (i = 0), _.indexOf(e, t, i) >= 0)
            }, _.invoke = function(e, t) {
                var i = h.call(arguments, 2),
                    n = _.isFunction(t);
                return _.map(e, function(e) {
                    var r = n ? t : e[t];
                    return null == r ? r : r.apply(e, i)
                })
            }, _.pluck = function(e, t) {
                return _.map(e, _.property(t))
            }, _.where = function(e, t) {
                return _.filter(e, _.matcher(t))
            }, _.findWhere = function(e, t) {
                return _.find(e, _.matcher(t))
            }, _.max = function(e, t, i) {
                var n, r, o = -(1 / 0),
                    s = -(1 / 0);
                if (null == t && null != e) {
                    e = P(e) ? e : _.values(e);
                    for (var a = 0, l = e.length; a < l; a++) n = e[a], n > o && (o = n)
                } else t = T(t, i), _.each(e, function(e, i, n) {
                    r = t(e, i, n), (r > s || r === -(1 / 0) && o === -(1 / 0)) && (o = e, s = r)
                });
                return o
            }, _.min = function(e, t, i) {
                var n, r, o = 1 / 0,
                    s = 1 / 0;
                if (null == t && null != e) {
                    e = P(e) ? e : _.values(e);
                    for (var a = 0, l = e.length; a < l; a++) n = e[a], n < o && (o = n)
                } else t = T(t, i), _.each(e, function(e, i, n) {
                    r = t(e, i, n), (r < s || r === 1 / 0 && o === 1 / 0) && (o = e, s = r)
                });
                return o
            }, _.shuffle = function(e) {
                for (var t, i = P(e) ? e : _.values(e), n = i.length, r = Array(n), o = 0; o < n; o++) t = _.random(0, o), t !== o && (r[o] = r[t]), r[t] = i[o];
                return r
            }, _.sample = function(e, t, i) {
                return null == t || i ? (P(e) || (e = _.values(e)), e[_.random(e.length - 1)]) : _.shuffle(e).slice(0, Math.max(0, t))
            }, _.sortBy = function(e, t, i) {
                return (t = T(t, i), _.pluck(_.map(e, function(e, i, n) {
                    return {
                        value: e,
                        index: i,
                        criteria: t(e, i, n)
                    }
                }).sort(function(e, t) {
                    var i = e.criteria,
                        n = t.criteria;
                    if (i !== n) {
                        if (i > n || void 0 === i) return 1;
                        if (i < n || void 0 === n) return -1
                    }
                    return e.index - t.index
                }), "value"))
            };
            var A = function(e) {
                return function(t, i, n) {
                    var r = {};
                    return (i = T(i, n), _.each(t, function(n, o) {
                        var s = i(n, o, t);
                        e(r, n, s)
                    }), r)
                }
            };
            _.groupBy = A(function(e, t, i) {
                _.has(e, i) ? e[i].push(t) : e[i] = [t]
            }), _.indexBy = A(function(e, t, i) {
                e[i] = t
            }), _.countBy = A(function(e, t, i) {
                _.has(e, i) ? e[i]++ : e[i] = 1
            }), _.toArray = function(e) {
                return e ? _.isArray(e) ? h.call(e) : P(e) ? _.map(e, _.identity) : _.values(e) : []
            }, _.size = function(e) {
                return null == e ? 0 : P(e) ? e.length : _.keys(e).length
            }, _.partition = function(e, t, i) {
                t = T(t, i);
                var n = [],
                    r = [];
                return (_.each(e, function(e, i, o) {
                    (t(e, i, o) ? n : r).push(e)
                }), [n, r])
            }, _.first = _.head = _.take = function(e, t, i) {
                if (null != e) return null == t || i ? e[0] : _.initial(e, e.length - t)
            }, _.initial = function(e, t, i) {
                return h.call(e, 0, Math.max(0, e.length - (null == t || i ? 1 : t)))
            }, _.last = function(e, t, i) {
                if (null != e) return null == t || i ? e[e.length - 1] : _.rest(e, Math.max(0, e.length - t))
            }, _.rest = _.tail = _.drop = function(e, t, i) {
                return h.call(e, null == t || i ? 1 : t)
            }, _.compact = function(e) {
                return _.filter(e, _.identity)
            };
            var O = function(e, t, i, n) {
                for (var r = [], o = 0, s = n || 0, a = C(e); s < a; s++) {
                    var l = e[s];
                    if (P(l) && (_.isArray(l) || _.isArguments(l))) {
                        t || (l = O(l, t, i));
                        var c = 0,
                            u = l.length;
                        for (r.length += u; c < u;) r[o++] = l[c++]
                    } else i || (r[o++] = l)
                }
                return r
            };
            _.flatten = function(e, t) {
                return O(e, t, !1)
            }, _.without = function(e) {
                return _.difference(e, h.call(arguments, 1))
            }, _.uniq = _.unique = function(e, t, i, n) {
                _.isBoolean(t) || (n = i, i = t, t = !1), null != i && (i = T(i, n));
                for (var r = [], o = [], s = 0, a = C(e); s < a; s++) {
                    var l = e[s],
                        c = i ? i(l, s, e) : l;
                    t ? (s && o === c || r.push(l), o = c) : i ? _.contains(o, c) || (o.push(c), r.push(l)) : _.contains(r, l) || r.push(l)
                }
                return r
            }, _.union = function() {
                return _.uniq(O(arguments, !0, !0))
            }, _.intersection = function(e) {
                for (var t = [], i = arguments.length, n = 0, r = C(e); n < r; n++) {
                    var o = e[n];
                    if (!_.contains(t, o)) {
                        for (var s = 1; s < i && _.contains(arguments[s], o); s++);
                        s === i && t.push(o)
                    }
                }
                return t
            }, _.difference = function(e) {
                var t = O(arguments, !0, !0, 1);
                return _.filter(e, function(e) {
                    return !_.contains(t, e)
                })
            }, _.zip = function() {
                return _.unzip(arguments)
            }, _.unzip = function(e) {
                for (var t = e && _.max(e, C).length || 0, i = Array(t), n = 0; n < t; n++) i[n] = _.pluck(e, n);
                return i
            }, _.object = function(e, t) {
                for (var i = {}, n = 0, r = C(e); n < r; n++) t ? i[e[n]] = t[n] : i[e[n][0]] = e[n][1];
                return i
            }, _.findIndex = n(1), _.findLastIndex = n(-1), _.sortedIndex = function(e, t, i, n) {
                i = T(i, n, 1);
                for (var r = i(t), o = 0, s = C(e); o < s;) {
                    var a = Math.floor((o + s) / 2);
                    i(e[a]) < r ? o = a + 1 : s = a
                }
                return o
            }, _.indexOf = r(1, _.findIndex, _.sortedIndex), _.lastIndexOf = r(-1, _.findLastIndex), _.range = function(e, t, i) {
                null == t && (t = e || 0, e = 0), i = i || 1;
                for (var n = Math.max(Math.ceil((t - e) / i), 0), r = Array(n), o = 0; o < n; o++, e += i) r[o] = e;
                return r
            };
            var N = function(e, t, i, n, r) {
                if (!(n instanceof t)) return e.apply(i, r);
                var o = I(e.prototype),
                    s = e.apply(o, r);
                return _.isObject(s) ? s : o
            };
            _.bind = function(e, t) {
                if (m && e.bind === m) return m.apply(e, h.call(arguments, 1));
                if (!_.isFunction(e)) throw new TypeError("Bind must be called on a function");
                var i = h.call(arguments, 2),
                    n = function() {
                        return N(e, n, t, this, i.concat(h.call(arguments)))
                    };
                return n
            }, _.partial = function(e) {
                var t = h.call(arguments, 1),
                    i = function() {
                        for (var n = 0, r = t.length, o = Array(r), s = 0; s < r; s++) o[s] = t[s] === _ ? arguments[n++] : t[s];
                        for (; n < arguments.length;) o.push(arguments[n++]);
                        return N(e, i, this, this, o)
                    };
                return i
            }, _.bindAll = function(e) {
                var t, i, n = arguments.length;
                if (n <= 1) throw new Error("bindAll must be passed function names");
                for (t = 1; t < n; t++) i = arguments[t], e[i] = _.bind(e[i], e);
                return e
            }, _.memoize = function(e, t) {
                var i = function(n) {
                    var r = i.cache,
                        o = "" + (t ? t.apply(this, arguments) : n);
                    return (_.has(r, o) || (r[o] = e.apply(this, arguments)), r[o])
                };
                return (i.cache = {}, i)
            }, _.delay = function(e, t) {
                var i = h.call(arguments, 2);
                return setTimeout(function() {
                    return e.apply(null, i)
                }, t)
            }, _.defer = _.partial(_.delay, _, 1), _.throttle = function(e, t, i) {
                var n, r, o, s = null,
                    a = 0;
                i || (i = {});
                var l = function() {
                    a = i.leading === !1 ? 0 : _.now(), s = null, o = e.apply(n, r), s || (n = r = null)
                };
                return function() {
                    var c = _.now();
                    a || i.leading !== !1 || (a = c);
                    var u = t - (c - a);
                    return (n = this, r = arguments, u <= 0 || u > t ? (s && (clearTimeout(s), s = null), a = c, o = e.apply(n, r), s || (n = r = null)) : s || i.trailing === !1 || (s = setTimeout(l, u)), o)
                }
            }, _.debounce = function(e, t, i) {
                var n, r, o, s, a, l = function() {
                    var c = _.now() - s;
                    c < t && c >= 0 ? n = setTimeout(l, t - c) : (n = null, i || (a = e.apply(o, r), n || (o = r = null)))
                };
                return function() {
                    o = this, r = arguments, s = _.now();
                    var c = i && !n;
                    return (n || (n = setTimeout(l, t)), c && (a = e.apply(o, r), o = r = null), a)
                }
            }, _.wrap = function(e, t) {
                return _.partial(t, e)
            }, _.negate = function(e) {
                return function() {
                    return !e.apply(this, arguments)
                }
            }, _.compose = function() {
                var e = arguments,
                    t = e.length - 1;
                return function() {
                    for (var i = t, n = e[t].apply(this, arguments); i--;) n = e[i].call(this, n);
                    return n
                }
            }, _.after = function(e, t) {
                return function() {
                    if (--e < 1) return t.apply(this, arguments)
                }
            }, _.before = function(e, t) {
                var i;
                return function() {
                    return (--e > 0 && (i = t.apply(this, arguments)), e <= 1 && (t = null), i)
                }
            }, _.once = _.partial(_.before, 2);
            var x = !{
                    toString: null
                }.propertyIsEnumerable("toString"),
                M = ["valueOf", "isPrototypeOf", "toString", "propertyIsEnumerable", "hasOwnProperty", "toLocaleString"];
            _.keys = function(e) {
                if (!_.isObject(e)) return [];
                if (g) return g(e);
                var t = [];
                for (var i in e) _.has(e, i) && t.push(i);
                return (x && o(e, t), t)
            }, _.allKeys = function(e) {
                if (!_.isObject(e)) return [];
                var t = [];
                for (var i in e) t.push(i);
                return (x && o(e, t), t)
            }, _.values = function(e) {
                for (var t = _.keys(e), i = t.length, n = Array(i), r = 0; r < i; r++) n[r] = e[t[r]];
                return n
            }, _.mapObject = function(e, t, i) {
                t = T(t, i);
                for (var n, r = _.keys(e), o = r.length, s = {}, a = 0; a < o; a++) n = r[a], s[n] = t(e[n], n, e);
                return s
            }, _.pairs = function(e) {
                for (var t = _.keys(e), i = t.length, n = Array(i), r = 0; r < i; r++) n[r] = [t[r], e[t[r]]];
                return n
            }, _.invert = function(e) {
                for (var t = {}, i = _.keys(e), n = 0, r = i.length; n < r; n++) t[e[i[n]]] = i[n];
                return t
            }, _.functions = _.methods = function(e) {
                var t = [];
                for (var i in e) _.isFunction(e[i]) && t.push(i);
                return t.sort()
            }, _.extend = b(_.allKeys), _.extendOwn = _.assign = b(_.keys), _.findKey = function(e, t, i) {
                t = T(t, i);
                for (var n, r = _.keys(e), o = 0, s = r.length; o < s; o++)
                    if (n = r[o], t(e[n], n, e)) return n
            }, _.pick = function(e, t, i) {
                var n, r, o = {},
                    s = e;
                if (null == s) return o;
                _.isFunction(t) ? (r = _.allKeys(s), n = S(t, i)) : (r = O(arguments, !1, !1, 1), n = function(e, t, i) {
                    return t in i
                }, s = Object(s));
                for (var a = 0, l = r.length; a < l; a++) {
                    var c = r[a],
                        u = s[c];
                    n(u, c, s) && (o[c] = u)
                }
                return o
            }, _.omit = function(e, t, i) {
                if (_.isFunction(t)) t = _.negate(t);
                else {
                    var n = _.map(O(arguments, !1, !1, 1), String);
                    t = function(e, t) {
                        return !_.contains(n, t)
                    }
                }
                return _.pick(e, t, i)
            }, _.defaults = b(_.allKeys, !0), _.create = function(e, t) {
                var i = I(e);
                return (t && _.extendOwn(i, t), i)
            }, _.clone = function(e) {
                return _.isObject(e) ? _.isArray(e) ? e.slice() : _.extend({}, e) : e
            }, _.tap = function(e, t) {
                return (t(e), e)
            }, _.isMatch = function(e, t) {
                var i = _.keys(t),
                    n = i.length;
                if (null == e) return !n;
                for (var r = Object(e), o = 0; o < n; o++) {
                    var s = i[o];
                    if (t[s] !== r[s] || !(s in r)) return !1
                }
                return !0
            };
            var L = function(e, t, i, n) {
                if (e === t) return 0 !== e || 1 / e === 1 / t;
                if (null == e || null == t) return e === t;
                e instanceof _ && (e = e._wrapped), t instanceof _ && (t = t._wrapped);
                var r = f.call(e);
                if (r !== f.call(t)) return !1;
                switch (r) {
                    case "[object RegExp]":
                    case "[object String]":
                        return "" + e == "" + t;
                    case "[object Number]":
                        return +e !== +e ? +t !== +t : 0 === +e ? 1 / +e === 1 / t : +e === +t;
                    case "[object Date]":
                    case "[object Boolean]":
                        return +e === +t
                }
                var o = "[object Array]" === r;
                if (!o) {
                    if ("object" != typeof e || "object" != typeof t) return !1;
                    var s = e.constructor,
                        a = t.constructor;
                    if (s !== a && !(_.isFunction(s) && s instanceof s && _.isFunction(a) && a instanceof a) && "constructor" in e && "constructor" in t) return !1
                }
                i = i || [], n = n || [];
                for (var l = i.length; l--;)
                    if (i[l] === e) return n[l] === t;
                if (i.push(e), n.push(t), o) {
                    if (l = e.length, l !== t.length) return !1;
                    for (; l--;)
                        if (!L(e[l], t[l], i, n)) return !1
                } else {
                    var c, u = _.keys(e);
                    if (l = u.length, _.keys(t).length !== l) return !1;
                    for (; l--;)
                        if (c = u[l], !_.has(t, c) || !L(e[c], t[c], i, n)) return !1
                }
                return (i.pop(), n.pop(), !0)
            };
            _.isEqual = function(e, t) {
                return L(e, t)
            }, _.isEmpty = function(e) {
                return null == e || (P(e) && (_.isArray(e) || _.isString(e) || _.isArguments(e)) ? 0 === e.length : 0 === _.keys(e).length)
            }, _.isElement = function(e) {
                return !(!e || 1 !== e.nodeType)
            }, _.isArray = y || function(e) {
                return "[object Array]" === f.call(e)
            }, _.isObject = function(e) {
                var t = typeof e;
                return "function" === t || "object" === t && !!e
            }, _.each(["Arguments", "Function", "String", "Number", "Date", "RegExp", "Error"], function(e) {
                _["is" + e] = function(t) {
                    return f.call(t) === "[object " + e + "]"
                }
            }), _.isArguments(arguments) || (_.isArguments = function(e) {
                return _.has(e, "callee")
            }), "function" != typeof /./ && "object" != typeof Int8Array && (_.isFunction = function(e) {
                return "function" == typeof e || !1
            }), _.isFinite = function(e) {
                return isFinite(e) && !isNaN(parseFloat(e))
            }, _.isNaN = function(e) {
                return _.isNumber(e) && e !== +e
            }, _.isBoolean = function(e) {
                return e === !0 || e === !1 || "[object Boolean]" === f.call(e)
            }, _.isNull = function(e) {
                return null === e
            }, _.isUndefined = function(e) {
                return void 0 === e
            }, _.has = function(e, t) {
                return null != e && d.call(e, t);
            }, _.noConflict = function() {
                return (s._ = a, this)
            }, _.identity = function(e) {
                return e
            }, _.constant = function(e) {
                return function() {
                    return e
                }
            }, _.noop = function() {}, _.property = w, _.propertyOf = function(e) {
                return null == e ? function() {} : function(t) {
                    return e[t]
                }
            }, _.matcher = _.matches = function(e) {
                return (e = _.extendOwn({}, e), function(t) {
                    return _.isMatch(t, e)
                })
            }, _.times = function(e, t, i) {
                var n = Array(Math.max(0, e));
                t = S(t, i, 1);
                for (var r = 0; r < e; r++) n[r] = t(r);
                return n
            }, _.random = function(e, t) {
                return (null == t && (t = e, e = 0), e + Math.floor(Math.random() * (t - e + 1)))
            }, _.now = Date.now || function() {
                return (new Date).getTime()
            };
            var k = {
                    "&": "&amp;",
                    "<": "&lt;",
                    ">": "&gt;",
                    '"': "&quot;",
                    "'": "&#x27;",
                    "`": "&#x60;"
                },
                D = _.invert(k),
                U = function(e) {
                    var t = function(t) {
                            return e[t]
                        },
                        i = "(?:" + _.keys(e).join("|") + ")",
                        n = RegExp(i),
                        r = RegExp(i, "g");
                    return function(e) {
                        return (e = null == e ? "" : "" + e, n.test(e) ? e.replace(r, t) : e)
                    }
                };
            _.escape = U(k), _.unescape = U(D), _.result = function(e, t, i) {
                var n = null == e ? void 0 : e[t];
                return (void 0 === n && (n = i), _.isFunction(n) ? n.call(e) : n)
            };
            var F = 0;
            _.uniqueId = function(e) {
                var t = ++F + "";
                return e ? e + t : t
            }, _.templateSettings = {
                evaluate: /<%([\s\S]+?)%>/g,
                interpolate: /<%=([\s\S]+?)%>/g,
                escape: /<%-([\s\S]+?)%>/g
            };
            var H = /(.)^/,
                W = {
                    "'": "'",
                    "\\": "\\",
                    "\r": "r",
                    "\n": "n",
                    "\u2028": "u2028",
                    "\u2029": "u2029"
                },
                V = /\\|'|\r|\n|\u2028|\u2029/g,
                B = function(e) {
                    return "\\" + W[e]
                };
            _.template = function(e, t, i) {
                !t && i && (t = i), t = _.defaults({}, t, _.templateSettings);
                var n = RegExp([(t.escape || H).source, (t.interpolate || H).source, (t.evaluate || H).source].join("|") + "|$", "g"),
                    r = 0,
                    o = "__p+='";
                e.replace(n, function(t, i, n, s, a) {
                    return (o += e.slice(r, a).replace(V, B), r = a + t.length, i ? o += "'+\n((__t=(" + i + "))==null?'':_.escape(__t))+\n'" : n ? o += "'+\n((__t=(" + n + "))==null?'':__t)+\n'" : s && (o += "';\n" + s + "\n__p+='"), t)
                }), o += "';\n", t.variable || (o = "with(obj||{}){\n" + o + "}\n"), o = "var __t,__p='',__j=Array.prototype.join,print=function(){__p+=__j.call(arguments,'');};\n" + o + "return __p;\n";
                try {
                    var s = new Function(t.variable || "obj", "_", o)
                } catch (e) {
                    throw (e.source = o, e)
                }
                var a = function(e) {
                        return s.call(this, e, _)
                    },
                    l = t.variable || "obj";
                return (a.source = "function(" + l + "){\n" + o + "}", a)
            }, _.chain = function(e) {
                var t = _(e);
                return (t._chain = !0, t)
            };
            var q = function(e, t) {
                return e._chain ? _(t).chain() : t
            };
            _.mixin = function(e) {
                _.each(_.functions(e), function(t) {
                    var i = _[t] = e[t];
                    _.prototype[t] = function() {
                        var e = [this._wrapped];
                        return (p.apply(e, arguments), q(this, i.apply(_, e)))
                    }
                })
            }, _.mixin(_), _.each(["pop", "push", "reverse", "shift", "sort", "splice", "unshift"], function(e) {
                var t = l[e];
                _.prototype[e] = function() {
                    var i = this._wrapped;
                    return (t.apply(i, arguments), "shift" !== e && "splice" !== e || 0 !== i.length || delete i[0], q(this, i))
                }
            }), _.each(["concat", "join", "slice"], function(e) {
                var t = l[e];
                _.prototype[e] = function() {
                    return q(this, t.apply(this._wrapped, arguments))
                }
            }), _.prototype.value = function() {
                return this._wrapped
            }, _.prototype.valueOf = _.prototype.toJSON = _.prototype.value, _.prototype.toString = function() {
                return "" + this._wrapped
            }, "function" == typeof define && define.amd && define("underscore", [], function() {
                return _
            })
        }).call(this)
    }, {}]
}, {}, [55]);
/* ******************************************************************************

This software incorporates the following open-source software:

check-preconditions
https://github.com/pnann/check-preconditions

The MIT License (MIT)

Copyright (c) 2015 Paul Nann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

EventEmitter2
https://github.com/asyncly/EventEmitter2

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


jQuery
http://jquery.com/

Copyright 2005, 2014 jQuery Foundation and other contributors,
https://jquery.org/

This software consists of voluntary contributions made by many
individuals. For exact contribution history, see the revision history
available at https://github.com/jquery/jquery

The following license applies to all parts of this software except as
documented below:

====

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

====

All files located in the node_modules and external directories are
externally maintained libraries used by this software which have their
own licenses; we recommend you read them, as their terms may differ from
the terms above.


js-logger
https://github.com/jonnyreeves/js-logger

Copyright (c) 2012 Jonny Reeves, http://jonnyreeves.co.uk/

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


json-stringify-safe
https://github.com/isaacs/json-stringify-safe

Copyright (c) Isaac Z. Schlueter ("Author")
All rights reserved.

The BSD License

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


jsSHA
https://github.com/Caligatio/jsSHA

Copyright (c) 2008-2013, Brian Turek
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * The names of the contributors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
ANDANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIEDWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING,BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE,DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OFLIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCEOR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISEDOF THE POSSIBILITY OF SUCH DAMAGE.


mustache.js
https://github.com/janl/mustache.js

The MIT License

Copyright (c) 2009 Chris Wanstrath (Ruby)
Copyright (c) 2010-2014 Jan Lehnardt (JavaScript)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


node-uuid
https://github.com/broofa/node-uuid

Copyright (c) 2010-2012 Robert Kieffer
MIT License - http://opensource.org/licenses/mit-license.php


qs
https://github.com/hapijs/qs

Copyright (c) 2014 Nathan LaFreniere and other contributors.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * The names of any contributors may not be used to endorse or promote
      products derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

                                  *   *   *

The complete list of contributors can be found at: https://github.com/hapijs/qs/graphs/contributors


Silverlight.js

This file is provided by Microsoft as a helper file for websites that
incorporate Silverlight Objects. This file is provided under the Microsoft
Public License available at
http://code.msdn.microsoft.com/silverlightjs/Project/License.aspx.
You may not use or distribute this file or the code in this file except as
expressly permitted under that license.

Copyright (c) Microsoft Corporation. All rights reserved.


Underscore.js
http://underscorejs.org

Copyright (c) 2009-2014 Jeremy Ashkenas, DocumentCloud and Investigative
Reporters & Editors

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

****************************************************************************** */