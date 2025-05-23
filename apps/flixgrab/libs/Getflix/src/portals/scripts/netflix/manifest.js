function arrayBufferToBase64(buffer) {
    var binary = "";
    var bytes = new Uint8Array( buffer );
    var len = bytes.byteLength;
    for (var i = 0; i < len; i++) {
        binary += String.fromCharCode(bytes[i]);
    }

    return btoa(binary);
}

function base64ToArrayBuffer(b64) {
    var byteString = atob(b64);
    var byteArray = new Uint8Array(byteString.length);
    for(var i=0; i < byteString.length; i++) {
        byteArray[i] = byteString.charCodeAt(i);
    }

    return byteArray;
}

function arrayBufferToString(buffer){
    var arr = new Uint8Array(buffer);
    var str = String.fromCharCode.apply(String, arr);
    if(/[\u0080-\uffff]/.test(str)){
        throw new Error("this string seems to contain (still encoded) multibytes");
    }

    return str;
}

function padBase64(b64) {
    var l = b64.length % 4;
    if (l == 2) {
        b64 += "==";
    } else if (l == 3) {
        b64 += "=";
    }

    return b64.replace(/-/g, "+").replace(/_/g, "/");
}

function generateEsn() {
  var text = "";
  var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

  for (var i = 0; i < 30; i++)
    text += possible.charAt(Math.floor(Math.random() * possible.length));

  return text;
}

//var manifestUrl = "https://www.netflix.com/api/msl/cadmium/manifest";
var manifestUrl = "https://www.netflix.com/api/msl/NFCDIE-02-/cadmium/manifest";
var shaktiMetadataUrl = "https://www.netflix.com/api/shakti/d7cab521/metadata?movieid=";
var esn = "NFCDIE-02-" + generateEsn();
var profiles = [
    "playready-h264mpl30-dash",
    "playready-h264mpl31-dash",
    "playready-h264mpl40-dash",
	"playready-h264mpl41-dash",
    "heaac-2-dash",
    "heaac-5.1-dash",
	"ddplus-2.0-dash",
	"ddplus-5.1-dash",
	"dd-5.1-elem",
    "simplesdh",
    "nflx-cmisc",
    "BIF240",
    "BIF320"
];

var messageid = Math.floor(Math.random() * 2**52);
var header = {
    "sender": esn,
    "handshake": true,
    "nonreplayable": false,
    "capabilities": {
        "languages": [
            "en-US"
        ],
        "compressionalgos": [""]
    },
    "recipient": "Netflix",
    "renewable": true,
    "messageid": messageid,
    "timestamp": Math.round((new Date()).getTime() / 1000),

};

async function getViewableId(viewableIdPath) {
    console.log('Getting video metadata for ID ' + viewableIdPath);

    var apiResp = await fetch(
        shaktiMetadataUrl + viewableIdPath,
        {
            credentials: "same-origin",
            method: "GET"
        }
    );

    var apiJson = await apiResp.json();
    console.log('Metadata response:');
    console.log(apiJson);

    var viewableId = apiJson.video.currentEpisode;
    if (!viewableId) {
        viewableId = parseInt(viewableIdPath);
    }

    return viewableId;
}

async function performKeyExchange() {
    var keyPair = await window.crypto.subtle.generateKey(
        {
            name: "RSA-OAEP",
            modulusLength: 2048,
            publicExponent: new Uint8Array([0x01, 0x00, 0x01]),
            hash: {name: "SHA-1"},
        },
        false,
        ["encrypt", "decrypt"]
    );

    var publicKey = await window.crypto.subtle.exportKey(
        "spki",
        keyPair.publicKey
    );

    header.handshake = true;
    delete header.userauthdata;
    header.keyrequestdata = [
        {
            "scheme": "ASYMMETRIC_WRAPPED",
            "keydata": {
                "publickey": arrayBufferToBase64(publicKey),
                "mechanism": "JWK_RSA",
                "keypairid": "superKeyPair"
            }
        }
    ];

    var request = {
        "entityauthdata": {
            "scheme": "NONE",
            "authdata": {
                "identity": esn,
            }
        },
        "signature": "",
    };

    request.headerdata = btoa(JSON.stringify(header));

    var handshakeResp = await fetch(
        manifestUrl,
        {
            body: JSON.stringify(request),
            method: "POST"
        }
    );

    var handshakeJson = await handshakeResp.json();
    if (!handshakeJson.headerdata) {
        console.error(JSON.parse(atob(handshakeJson.errordata)));
        throw("Error parsing key exchange response");
    }

    var headerdata = JSON.parse(atob(handshakeJson.headerdata));
    var encryptionKeyData = await window.crypto.subtle.decrypt(
        {
            name: "RSA-OAEP",
			hash: { name: "SHA-1" }
        },
        keyPair.privateKey,
        base64ToArrayBuffer(headerdata.keyresponsedata.keydata.encryptionkey)
    );

    encryptionKeyData = JSON.parse(arrayBufferToString(encryptionKeyData));

    var signKeyData = await window.crypto.subtle.decrypt(
        {
            name: "RSA-OAEP",
			hash: { name: "SHA-1" }
        },
        keyPair.privateKey,
        base64ToArrayBuffer(headerdata.keyresponsedata.keydata.hmackey)
    );

    signKeyData = JSON.parse(arrayBufferToString(signKeyData));

    return {
        "headerdata": headerdata,
        "encryptionKeyData": encryptionKeyData,
        "signKeyData": signKeyData
    };
}
async function fetchManifest(viewableId, localeId) {
	console.log('Performing key exchange');
    var keyExchangeData = await performKeyExchange();
    console.log('Key exchange data:');
    console.log(keyExchangeData);

    var headerdata = keyExchangeData.headerdata;
    var encryptionKeyData = keyExchangeData.encryptionKeyData;
    var signKeyData = keyExchangeData.signKeyData;
    var mastertoken = headerdata.keyresponsedata.mastertoken;
    var sequenceNumber = JSON.parse(atob(mastertoken.tokendata)).sequencenumber;
    //var viewableIdPath = window.location.pathname.substring(7, 15);
    //var viewableId = await getViewableId(viewableIdPath);

    // var localeId = "en-US";
    // try {
        // localeId = netflix.appContext.state.model.models.memberContext.data.geo.locale.id;
    // } catch (e) {}

	
	/*
	"{"method":"manifest","lookupType":"STANDARD","viewableIds":[70075480],"profiles":["playready-h264mpl30-dash","playready-h264mpl31-dash","playready-h264mpl40-dash","heaac-2-dash","dfxp-ls-sdh","simplesdh","nflx-cmisc","BIF240","BIF320"],"drmSystem":"playready","appId":"15418404979690","sessionParams":{"uiplaycontext":"{\"location\":\"WATCHNOW\",\"rank\":-97,\"request_id\":\"95f208a1-d62b-4ba5-8a23-85e61d6f5333-55601970\",\"row\":-97,\"track_id\":-1,\"video_id\":70075480}","isUIAutoPlay":"false","supportsWatermark":"true","supportsPreReleasePin":"true","preferUnletterboxed":"false"},"sessionId":"15418404979690","trackId":-1,"flavor":"STANDARD","supportPreviewContent":false,"supportPreviewContentPin":true,"supportWatermarking":true,"forceClearStreams":false,"validatePinProtection":false,"usePlayReadyHeaderObject":true,"showAllSubDubTracks":false,"isBranched":false,"uiProfileGuid":"Z2QWAIS3X5CY7NZNMPLZGKX3TQ","securityDescriptors":{"DEVICE_SECURITY_LEVEL":"3000"},"mediaOutputDescriptors":"[{\"type\":\"DigitalVideoOutputDescriptor\",\"supportedHdcpVersions\":[\"1.4\"],\"isHdcpEngaged\":true}]"}"
	*/
	
	var d = new Date();
	var id = d.getTime().toString();
	
    var manifestRequestData = {
        "method": "manifest",
        "lookupType": "STANDARD",
        "viewableIds": [viewableId],
        "profiles": profiles,
        "drmSystem": "widevine",
        //"appId": "15418404979690",
		"appId": id,
        "sessionParams": {
            "pinCapableClient": false,
            "uiplaycontext": "null"
        },
        "sessionId": id,
        "trackId": -1,
        "flavor": "STANDARD",
        "secureUrls": true,
        "supportPreviewContent": true,
        "showAllSubDubTracks": false,
        "forceClearStreams": false,
        "languages": [localeId],
		"securityDescriptors":{"DEVICE_SECURITY_LEVEL":"3000"},
		"mediaOutputDescriptors":"[{\"type\":\"DigitalVideoOutputDescriptor\",\"supportedHdcpVersions\":[\"1.4\"],\"isHdcpEngaged\":true}]",
    };

    header.handshake = false;
    header.userauthdata = {
        "scheme": "NETFLIXID",
        "authdata": {}
    };

    var iv = window.crypto.getRandomValues(new Uint8Array(16));
    var aesCbc = new aesjs.ModeOfOperation.cbc(
        base64ToArrayBuffer(padBase64(encryptionKeyData.k)),
        iv
    );
    var textBytes = aesjs.utils.utf8.toBytes(JSON.stringify(header));
    var encrypted = aesCbc.encrypt(aesjs.padding.pkcs7.pad(textBytes));
    var encryptionEnvelope = {
        "keyid": esn + "_" + sequenceNumber,
        "sha256": "AA==",
        "iv": arrayBufferToBase64(iv),
        "ciphertext": arrayBufferToBase64(encrypted)
    };

    var shaObj = new jsSHA("SHA-256", "TEXT");
    shaObj.setHMACKey(padBase64(signKeyData.k), "B64");
    shaObj.update(JSON.stringify(encryptionEnvelope));
    var signature = shaObj.getHMAC("B64");
    var encryptedHeader = {
        "signature": signature,
        "mastertoken": mastertoken
    };
    
    encryptedHeader.headerdata = btoa(JSON.stringify(encryptionEnvelope));
    
    var serializedData = [{
        }, {
        "headers": {},
        "path": "/cbp/cadmium-13",
        "payload": {
            "data": JSON.stringify(manifestRequestData).replace('"', '\"')
        },
        "query": ""
    }];

    var firstPayload = {
        "messageid": messageid,
        "data": btoa(JSON.stringify(serializedData)),
        "compressionalgos": [""],
        "sequencenumber": 1,
        "endofmsg": true
    };

    iv = window.crypto.getRandomValues(new Uint8Array(16));
    aesCbc = new aesjs.ModeOfOperation.cbc(
        base64ToArrayBuffer(padBase64(encryptionKeyData.k)),
        iv
    );
    
    textBytes = aesjs.utils.utf8.toBytes(JSON.stringify(firstPayload));
    encrypted = aesCbc.encrypt(aesjs.padding.pkcs7.pad(textBytes));    
    
    encryptionEnvelope = {
        "keyid": esn + "_" + sequenceNumber,
        "sha256": "AA==",
        "iv": arrayBufferToBase64(iv),
        "ciphertext": arrayBufferToBase64(encrypted)
    };

    shaObj = new jsSHA("SHA-256", "TEXT");
    shaObj.setHMACKey(padBase64(signKeyData.k), "B64");
    shaObj.update(JSON.stringify(encryptionEnvelope));
    signature = shaObj.getHMAC("B64");

    var firstPayloadChunk = {
        "signature": signature,
        "payload": btoa(JSON.stringify(encryptionEnvelope))
    };
    
    var manifestResp = await fetch(
        manifestUrl,
        {
            body: JSON.stringify(encryptedHeader) + JSON.stringify(firstPayloadChunk),
            credentials: "same-origin",
            method: "POST"
        }
    );

    manifestResp = await manifestResp.text();

    try {
        JSON.parse(manifestResp);
        console.error(JSON.parse(atob(JSON.parse(manifestResp).errordata)));
        throw("Error parsing manifest");
    } catch (e) {}

    var pattern = /,"signature":"[0-9A-Za-z/+=]+"}/;
    var payloadsSplit = manifestResp.split("}}")[1].split(pattern);
    payloadsSplit.pop();
    var payloadChunks = [];
    for (var i = 0; i < payloadsSplit.length; i++) {
        payloadChunks.push(payloadsSplit[i] + "}");
    }

    var chunks = "";
    for (i = 0; i < payloadChunks.length; i++) {
        var payloadchunk = JSON.parse(payloadChunks[i]);
        encryptionEnvelope = atob(payloadchunk.payload);
        aesCbc = new aesjs.ModeOfOperation.cbc(
            base64ToArrayBuffer(padBase64(encryptionKeyData.k)),
            base64ToArrayBuffer(JSON.parse(encryptionEnvelope).iv)
        );

        var ciphertext = base64ToArrayBuffer(
            JSON.parse(encryptionEnvelope).ciphertext
        );

        var plaintext = JSON.parse(
            arrayBufferToString(
                aesjs.padding.pkcs7.strip(
                    aesCbc.decrypt(ciphertext)
                )
            )
        );

        chunks += atob(plaintext.data);
    }

    var manifest = JSON.parse(atob(JSON.parse(chunks)[1].payload.data));

    if (!manifest.success) {
        console.error(manifest);
        throw("Error parsing manifest");
    }

    console.log('Manifest:');
    console.log(manifest);

    return manifest;
}


async function getManifest() {
	
	var viewableIdPath = window.location.pathname.substring(7, 15);
    var viewableId = await getViewableId(viewableIdPath);

    var localeId = "en-US";
    try {
        localeId = netflix.appContext.state.model.models.memberContext.data.geo.locale.id;
    } catch (e) {}
	
	return await fetchManifest(viewableId, localeId);
	
}
