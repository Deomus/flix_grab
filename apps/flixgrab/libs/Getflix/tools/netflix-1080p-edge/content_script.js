// From EME Logger extension

script_urls = [
    'https://cdn.rawgit.com/ricmoo/aes-js/master/index.js',
    'https://cdn.rawgit.com/Caligatio/jsSHA/master/src/sha.js'
]

urls = [
    'cadmium-playercore-6.0009.457.031-formatted.js',
    'get_manifest.js',
    'bootstrap.js',
    'akiraclient.js'
]

for (var i = 0; i < script_urls.length; i++) {
    var script = document.createElement('script');
    script.src = script_urls[i];
    document.documentElement.appendChild(script);
}

for (var i = 0; i < urls.length; i++) {
    var mainScriptUrl = chrome.extension.getURL(urls[i]);
    console.log(mainScriptUrl);
    var mainScript = document.createElement('script');
    mainScript.type = 'application/javascript';
    mainScript.src = mainScriptUrl;
    document.documentElement.appendChild(mainScript);

}


// for (var i = 0; i < urls.length; i++) {
//     var mainScriptUrl = chrome.extension.getURL(urls[i]);
//     console.log(mainScriptUrl);
//     var xhr = new XMLHttpRequest();
//     xhr.open('GET', mainScriptUrl, true);

//     xhr.onload = function(e) {
//         var xhr = e.target;
//         var mainScript = document.createElement('script');
//         mainScript.type = 'application/javascript';
//         if (xhr.status == 200) {
//             mainScript.text = xhr.responseText;
//             document.documentElement.appendChild(mainScript);
//         }
//     };

//   xhr.send();
// }
