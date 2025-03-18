"use strict";

var bodyParser = require('body-parser'),
    nodemailer = require('nodemailer'),
    softactivate = require('./softactivate.js');

/** 
 * @constructor
 */
function paymentServiceController(app) {
    
    if (!(this instanceof paymentServiceController)) {
        return new paymentServiceController(app);
    }
    
    var db = app.connectionPool;
    var currentDate = new Date();
      
    this.processOrderNotificationRequest = function (requestId, productId, quantity, totalPrice, currency, firstName, lastName,
                                                     companyName, email, phoneNumber, street, zipCode, city, state, countryCode, sendOrderEmail, callback) {
        
        db.query('SELECT * FROM paymentservicemappings WHERE PaymentServiceProductId = \'' + productId + '\' LIMIT 1', function (err, rows, fields) {
            
            if (err || rows.length == 0) {
                if (err) console.log(err);
                callback(err);
                return;
            }
            
            db.query('SELECT * FROM products WHERE Id = ' + rows[0]['ProductId'], function (err, rows, fields) {
                
                if (err || !rows.length) {
                    if (err) console.log(err);
                    callback(err);
                    return;
                }
                
                var productId = rows[0]['Id'];
                
                var name = (lastName) ? firstName + ' ' + lastName : firstName;
                
                var expires = new Date();
                
                if (rows[0]['DefaultLicenseDuration'])
                    expires.setDate(expires.getDate() + rows[0]['DefaultLicenseDuration']);
                
                var keys = generateLicenseKeys((quantity) ? quantity : 1,
                                                rows[0]['LicenseKeyTemplate'],
                                                rows[0]['Id'],
                                                name,
                                                email,
                                                expires);
                
                if (!keys) {
                    console.log('null keys');
                    callback('null keys');
                    return;
                }
                
                db.query('INSERT INTO orders SET ?',
                    {
                    'TransactionId': requestId,
                    'OrderTotal': totalPrice,
                    'OrderTotalExclFees': totalPrice,
                    'Currency': currency,
                    'FirstName': firstName,
                    'LastName': lastName,
                    'Company': companyName,
                    'Email': email,
                    'PhoneNumber': phoneNumber,
                    'Address': street,
                    'ZipCode': zipCode,
                    'City': city,
                    'State': state,
                    'CountryCode': countryCode,
                    'Date': currentDate
                }, function (err, result) {
                    
                    if (err) {
                        console.log(err);
                        callback(err);
                        return;
                    }
                    
                    db.query('INSERT INTO orderitems SET ?',
                            {
                        'OrderId': result.insertId,
                        'ProductId': productId,
                        'Quantity': quantity || 1,
                        'TotalPrice': totalPrice,
                        'TotalPriceExclFees': totalPrice,
                        'OriginalProductPrice': totalPrice,
                        'OriginalProductCurrency': currency
                    }, function (err, result) {
                        
                        if (err) {
                            console.log(err);
                            callback(err);
                            return;
                        }
                        
                        for (var i = 0; i < keys.length; i++) {
                            db.query('INSERT INTO licensekeys SET ?',
                                {
                                'OrderItemId': result.insertId,
                                'ProductId': productId,
                                'DateAdded': currentDate,
                                'Key': keys[i]
                            }, function (err, result) {
                                if (err) {
                                    console.log(err);
                                    callback(err);
                                    return;
                                } else {
                                    callback(undefined, keys);
                                }
                            });
                        }
                    });
                });
                
                if (sendOrderEmail && app['mailer']) {
                    var orderEmail = buildOrderEmail(
                        rows[0]['OrderEmailTemplate'], 
                    {
                            '##PRODUCT_NAME##': rows[0]['Name'],
                            '##LICENSE_KEY##': keys[0],
                            '##REGISTRATION_NAME##': name,
                            '##REGISTRATION_EMAIL##': email
                        }
                    );
                    
                    var mailOptions = {
                        'from': app.config['email']['from'],
                        'to': email,
                        'subject': orderEmail['subject'],
                        'html': orderEmail['body']
                    };
                    
                    if (app.config['email']['bcc']) {
                        mailOptions['bcc'] = app.config['email']['bcc'];
                    }
                    
                    app['mailer'].sendMail(mailOptions, function (err, info) {
                        if (err) {
                            console.log(err);
                            callback(err);
                        } else {
                            console.log('Message sent: ' + info.response);
                        }
                    });
                }
            });
        });
    }
       
    function buildOrderEmail(template, replacements) {
        
        var s = template;
        
        for (var name in replacements) {
            s = s.replace(new RegExp(name, 'g'), replacements[name]);
        }
        
        return { 'subject': s.match('<title>(.*)</title>')[1], 'body': s };
    }
    
    function generateLicenseKeys(count, template, pid, name, email, expires) {
        
        var licenseTemplate = new softactivate.LicenseTemplate();
        
        if (!licenseTemplate)
            return null;
        
        var err = licenseTemplate.loadXml(template);
        
        if (err != 0)
            return null;
        
        var keyGenerator = new softactivate.KeyGenerator();
        
        if (!keyGenerator)
            return null;
        
        err = keyGenerator.setLicenseTemplate(licenseTemplate);
        
        if (err != 0)
            return null;
        
        var data = {};
        
        err = licenseTemplate.getDataField('ProductId', data);
        
        if (err == 0) {
            err = keyGenerator.setIntKeyData('ProductId', pid);
            if (err != 0)
                return null;
        }
        
        err = licenseTemplate.getDataField('ExpirationDate', data);
        
        if (err == 0) {
            err = keyGenerator.setDateKeyData('ExpirationDate', expires.getUTCFullYear(), 1 + expires.getUTCMonth(), expires.getUTCDate());
            if (err != 0)
                return null;
        }
        
        err = licenseTemplate.getValidationField('RegistrationName', data);
        
        if (err == 0) {
            err = keyGenerator.setStringValidationData('RegistrationName', name);
            if (err != 0)
                return null;
        }
        
        err = licenseTemplate.getValidationField('RegistrationEmail', data);
        
        if (err == 0) {
            err = keyGenerator.setStringValidationData('RegistrationEmail', email);
            if (err != 0)
                return null;
        }
        
        var key;
        var keys = [];
        
        for (var i = 0; i < count; i++) {
            key = keyGenerator.generateKey();
            
            if (key)
                keys.push(key);
            else
                break;
        }
        
        keyGenerator._destroy();
        licenseTemplate._destroy();
        
        return (keys.length == count) ? keys : null;
    }
}

module.exports = paymentServiceController;
