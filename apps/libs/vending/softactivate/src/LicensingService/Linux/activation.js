"use strict";

var softactivate = require('./softactivate.js'),
    atob = require('atob');

/**
 * @constructor
 */
function activationController(app) {
    
    if (!(this instanceof activationController)) {
        return new activationController(app);
    }
    
    var db = app.connectionPool;

    var routeHandler = app.express.Router();
    
    routeHandler.use(function (req, res, next) {
        for (var key in req.query) {
            req.query[key.toLowerCase()] = req.query[key];
        }
        next();
    });
    
    routeHandler.get('/', function (req, res) {
        
        var licenseKey = req.query['licensekey'];
        var hardwareId = req.query['hardwareid'];

        if (!hardwareId) {
            res.status(400).send('Invalid activation query(1): invalid hardware id');
            return;
        }
        
        if (!licenseKey) {
            res.status(400).send('Invalid activation query(2): null or empty license key provided');
            return;
        }

        var productId = req.query['productid'] || -1;

        var licenseKeyValidationData = (req.query['licensekeyvalidationdata']) ? new Buffer(atob(req.query['licensekeyvalidationdata'])) : null;
        var ip = req.headers['x-forwarded-for'] || 
                        req.connection.remoteAddress || 
                        req.socket.remoteAddress ||
                        req.connection.socket.remoteAddress;
      
        db.query(
            '(SELECT 1 AS Query, licensekeys.Id AS LicenseKeyId, ProductId, DeviceLimit, DeviceCount, ActivationLimit, ActivationCount, PerDeviceActivationLimit, LicenseDuration, DefaultMaxUniqueHardwareIdsPerLicenseKey, DefaultMaxactivationsPerHardwareId, DefaultMaxactivationsPerLicenseKey, DefaultLicenseDuration, LicenseKeyTemplate, NULL AS ActivationDate FROM licensekeys LEFT JOIN products ON licensekeys.ProductId = products.Id ' + 
                'WHERE licensekeys.`Key` =\'' + licenseKey + '\' LIMIT 1) ' +
            'UNION ALL ' +
            '(SELECT 2 AS Query, NULL, products.Id, DeviceLimit, DeviceCount, ActivationLimit, ActivationCount, PerDeviceActivationLimit, LicenseDuration, DefaultMaxUniqueHardwareIdsPerLicenseKey, DefaultMaxactivationsPerHardwareId, DefaultMaxactivationsPerLicenseKey, DefaultLicenseDuration, LicenseKeyTemplate, NULL FROM licensekeys RIGHT JOIN products ON licensekeys.ProductId = products.Id ' + 
                'WHERE products.Id = ' + productId + ' LIMIT 1) ' +
            'UNION ALL ' +
            '(SELECT 3 AS Query, NULL, NULL, NULL, NULL, NULL, COUNT(*), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL FROM activations INNER JOIN licensekeys ON activations.LicenseKeyId = licensekeys.Id ' + 
                'WHERE licensekeys.`Key` = \'' + licenseKey + '\' AND activations.HardwareId = \'' + hardwareId + '\' LIMIT 1) ' +
            'UNION ALL ' +
            '(SELECT 4 AS Query, NULL, NULL, NULL, COUNT(DISTINCT HardwareId), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL FROM activations INNER JOIN licensekeys ON activations.LicenseKeyId = licensekeys.Id ' + 
                'WHERE licensekeys.`Key` = \'' + licenseKey + '\' LIMIT 1) ' +
            'UNION ALL ' +
            '(SELECT 5 AS Query, NULL, NULL, NULL, NULL, NULL, COUNT(*), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL FROM activations INNER JOIN licensekeys ON activations.LicenseKeyId = licensekeys.Id ' + 
                'WHERE licensekeys.`Key` = \'' + licenseKey + '\' LIMIT 1) ' + 
            'UNION ALL ' + 
            '(SELECT 6 AS Query, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ActivationDate FROM activations INNER JOIN licensekeys ON LicenseKeyId = licensekeys.Id WHERE licensekeys.`Key` = \'' + licenseKey + '\' AND HardwareId = \'' + hardwareId + '\' ORDER BY ActivationDate ASC LIMIT 1)',
            
            function (err, rows, fields) {
                if (err || rows.length < 1) {
                    if (err) console.log(err);
                    res.status(404).end();
                    return;
                }
                
                var productId = productId || -1;
                var licenseTemplate;
                var deviceLimit;
                var activationLimit;
                var perDeviceActivationLimit;

                var deviceCount;
                var activationCount;
                var currentDeviceActivationCount;
                
                var firstActivationDate;
                var licenseDuration;
                var licenseKeyId;

                for (var i = 0, r; i < rows.length; i++) {
                    
                    r = rows[i];
                    
                    switch (r['Query']) {

                        case 1:
                            licenseKeyId = r['LicenseKeyId'];                        
                            deviceLimit = r['DeviceLimit'];
                            activationLimit = r['ActivationLimit'];
                            perDeviceActivationLimit = r['PerDeviceActivationLimit'];
                            licenseDuration = r['LicenseDuration'];

                            licenseTemplate = r['LicenseKeyTemplate'];
                            
                            break;

                        case 2:
                            if (r['ProductId'])
                                if (productId >= 0 && r['ProductId'] != productId) {
                                    res.status(404).end();
                                    return;
                                }
                                else
                                    productId = r['ProductId'];
                            
                            if (!deviceLimit && r['DefaultMaxUniqueHardwareIdsPerLicenseKey'])
                                deviceLimit = r['DefaultMaxUniqueHardwareIdsPerLicenseKey'];
                            
                            if (!activationLimit && r['DefaultMaxactivationsPerLicenseKey'])
                                activationLimit = r['DefaultMaxactivationsPerLicenseKey'];
                            
                            if (!perDeviceActivationLimit && r['DefaultMaxactivationsPerHardwareId'])
                                perDeviceActivationLimit = r['DefaultMaxactivationsPerHardwareId'];
                            
                            if (!licenseDuration && r['DefaultLicenseDuration'])
                                licenseDuration = r['DefaultLicenseDuration'];
                            
			    if (r['LicenseKeyTemplate'])
                            	licenseTemplate = r['LicenseKeyTemplate'];

                            break;

                        case 3:
                            currentDeviceActivationCount = r['ActivationCount'];    
                            break;
                        
                        case 4:
                            deviceCount = r['DeviceCount'];
                            break;

                        case 5:
                            activationCount = r['ActivationCount'];
                            break;

                        case 6:
                            if (r['ActivationDate'])
                                firstActivationDate = r['ActivationDate'];
                            break;
                    }
                }
                
                if (productId < 0 || !licenseTemplate) {
                    res.status(403).send('Internal error (1)');
                    return;
                }
                
                if (!currentDeviceActivationCount) {
                    if (deviceCount && deviceLimit && deviceCount >= deviceLimit) {
                        res.status(403).send('Device limit exceeded');
                        return;
                    }
                } else {
                    if (perDeviceActivationLimit) // non-null and > 0
                        if (currentDeviceActivationCount >= perDeviceActivationLimit) {
                            res.status(403).send('Activation limit exceeded for this key');
                            return;
                        }
                }

                if (activationCount && activationLimit && activationCount >= activationLimit) {
                    res.status(403).send('Activation limit exceeded for this key');
                    return;
                }
                
                var currentDate = new Date();

                var expirationDate;
                
                if (firstActivationDate)
                    expirationDate = new Date(firstActivationDate);
                
                if (!expirationDate)
                    expirationDate = new Date(currentDate);
                
                if (licenseDuration)
                    expirationDate.setUTCDate(expirationDate.getUTCDate() + licenseDuration);

                var activationKey = generateActivationKey(licenseTemplate, licenseKey, licenseKeyValidationData, hardwareId, expirationDate);
              
                if (activationKey) {
                    if (!licenseKeyId) {
                        db.query(
                            'INSERT INTO licensekeys SET ?',
                            {
                                'Key': licenseKey,
                                'ValidationData': licenseKeyValidationData,
                                'ProductId': productId,
                                'DateAdded': currentDate,
                                'ActivationCount': 1
                            }, function (err, result) {
                                if (!err) {
                                    db.query(
                                        'INSERT INTO activations SET ?', 
                                        {
                                            'ActivationKey': activationKey,
                                            'LicenseKeyId': result.insertId,
                                            'HardwareId': hardwareId,
                                            'ExpirationDate': expirationDate,
                                            'ActivationDate': currentDate,
                                            'IPAddress': ip
                                        }, function (err, result) {
                                            if (!err) {
                                                res.status(200).send(activationKey);
                                            } else
                                                res.status(404).send('Not Found');
                                        });
                                }
                            }
                        );
                    } else {
                        db.query(
                            'UPDATE licensekeys SET ActivationCount = IFNULL(ActivationCount, 0) + 1, LastActivationTime = ? WHERE Id = ?',
                            [
                                currentDate,
                                licenseKeyId 
                            ], function (err, result) {
                                if (!err) {
                                    db.query(
                                        'INSERT INTO activations SET ?', 
                                        {
                                            'ActivationKey': activationKey,
                                            'LicenseKeyId': licenseKeyId,
                                            'HardwareId': hardwareId,
                                            'ExpirationDate': expirationDate,
                                            'ActivationDate': currentDate,
                                            'IPAddress': ip
                                        }, function (err, result) {
                                            if (!err) {
                                                res.status(200).send(activationKey);
                                            } else
                                                res.status(404).send('Not Found');
                                        });
                                }
                            }
                        );
                    }
                }
                else
                    res.status(404).send('Not Found');
            }
        );
    });

    this.routeHandler = routeHandler;
}

function generateActivationKey(template, licenseKey, licenseKeyValidationData, hardwareId, expirationDate) {
    
    var keyGenerator, keyValidator, licenseTemplate, activationTemplate, activationKey = null, success = false;

    do {
        licenseTemplate = new softactivate.LicenseTemplate();
        if (!licenseTemplate) break;
        
        var e = licenseTemplate.loadXml(template);
        if (e != 0)
            break;
        
        var numGroups = licenseTemplate.getNumberOfGroups();
        if (numGroups <= 0)
            break;
        
        var charsPerGroup = licenseTemplate.getCharactersPerGroup();
        if (charsPerGroup <= 0)
            break;
        
        var dataSize = licenseTemplate.getDataSize();
        if (dataSize <= 0)
            break;
        
        var signatureSize = licenseTemplate.getSignatureSize();
        
        var privateKey = licenseTemplate.getPrivateKey();
        if (!privateKey)
            break;
        
        var publicKey = licenseTemplate.getPublicKeyCertificate();
        if (!publicKey)
            break;
        
        activationTemplate = new softactivate.LicenseTemplate();
        if (!activationTemplate) break;
        
        e = activationTemplate.setNumberOfGroups(numGroups);
        e = activationTemplate.setCharactersPerGroup(charsPerGroup);
        e = activationTemplate.setDataSize(dataSize);
        e = activationTemplate.setSignatureSize(signatureSize);
        e = activationTemplate.setPrivateKey(privateKey);
        e = activationTemplate.setPublicKeyCertificate(publicKey);
        
        e = activationTemplate.addDataField('ExpirationDate', 0, 16, 0);
        
        e = activationTemplate.setValidationDataSize((licenseKey.length + hardwareId.length) * 8);
        
        e = activationTemplate.addValidationField('LicenseKey', 2, 8 * licenseKey.length, 0);
        e = activationTemplate.addValidationField('HardwareId', 2, 8 * hardwareId.length, 8 * licenseKey.length);
        
        if (licenseKeyValidationData) {
            activationTemplate.setValidationDataSize((licenseKey.length + hardwareId.length + licenseKeyValidationData.length) * 8);
            e = activationTemplate.addValidationField('LicenseKeyValidationData', 0, licenseKeyValidationData.length * 8, 8 * (licenseKey.length + hardwareId.length));
        }

        keyGenerator = new softactivate.KeyGenerator();
        if (!keyGenerator) break;
        
        e = keyGenerator.setLicenseTemplate(activationTemplate);
        
        if (expirationDate) {
            
            var dateBuf = new Buffer(2);
            var y = expirationDate.getUTCFullYear(), m = 1 + expirationDate.getUTCMonth(), d = expirationDate.getUTCDate();
            var expDateBits = ((y - 2000) << 9) | (m << 5) | d;
            dateBuf[0] = (expDateBits >> 8) & 255; dateBuf[1] = expDateBits & 255; 

            e = keyGenerator.setKeyData('ExpirationDate', dateBuf, dateBuf.length);
        }
        
        e = keyGenerator.setStringValidationData('LicenseKey', licenseKey);
        
        e = keyGenerator.setStringValidationData('HardwareId', hardwareId);
        
        if (licenseKeyValidationData)
            e = keyGenerator.setValidationData('LicenseKeyValidationData', licenseKeyValidationData, licenseKeyValidationData.length);
        
        activationKey = keyGenerator.generateKey(activationKey);
    
        keyValidator = new softactivate.KeyValidator();
        if (!keyValidator) break;

        e = keyValidator.setLicenseTemplate(activationTemplate);
        
        e = keyValidator.setKey(activationKey);

        e = keyValidator.setStringValidationData('LicenseKey', licenseKey);
        
        e = keyValidator.setStringValidationData('HardwareId', hardwareId);
        
        if (licenseKeyValidationData) {
            e = keyValidator.setValidationData('LicenseKeyValidationData', licenseKeyValidationData, licenseKeyValidationData.length);
        }

        if (keyValidator.isKeyValid() != 0)
            break;

        success = true;
         
    } while (false);

    if (keyGenerator) keyGenerator._destroy();
    if (keyValidator) keyValidator._destroy();
    if (licenseTemplate) licenseTemplate._destroy();
    if (activationTemplate) activationTemplate._destroy();
    
    return (success) ? activationKey : null;
}

module.exports = activationController;
