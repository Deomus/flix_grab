browser.webRequest.onBeforeRequest.addListener(
    function(details) {
        console.log("Url: " + details.url + " redirect: "+chrome.extension.getURL("cadmium-playercore-6.0009.457.011-1080p.js"));
        return {
            redirectUrl: chrome.extension.getURL("cadmium-playercore-6.0009.457.011-1080p.js")
        };
    }, {
        urls: [
            "*://assets.nflxext.com/*/ffe/player/html/*",
            "*://www.assets.nflxext.com/*/ffe/player/html/*",
            "*://codex.nflxext.com/%5E2.0.0/truthBundle/webui/0.0.1-akira-js-mk-*"
        ]
    }, ["blocking"]
);
