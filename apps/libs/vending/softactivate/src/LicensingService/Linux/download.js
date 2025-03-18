"use strict";

var softactivate = require('./softactivate.js');
var cookieParser = require('cookie-parser');
var UAParser = require('ua-parser-js');
var uaParser = new UAParser();

/**
 * @constructor
 */
function downloadController(app) {
    
    if (!(this instanceof downloadController)) {
        return new downloadController(app);
    }
    
    var routeHandler = app.express.Router();

    routeHandler.use(cookieParser());
    
    routeHandler.use(function (req, res, next) {
        for (var key in req.query) {
            req.query[key.toLowerCase()] = req.query[key];
        }
        next();
    });    

    routeHandler.get('/:file', function (req, res) {
               
        var licenseKey = req.query['licensekey'];
        
        if (!licenseKey)
            licenseKey = '';
        
        app.connectionPool.query('SELECT D.IsPublic, O.Date, L.LicenseDuration, P.DefaultLicenseDuration, P.LicenseKeyTemplate FROM downloads D LEFT JOIN products P ON D.ProductId = P.Id LEFT JOIN licensekeys L ON D.ProductId = L.ProductId LEFT JOIN orderitems I ON L.OrderItemId = I.Id LEFT JOIN orders O ON I.OrderId = O.Id WHERE D.LocalPath = \'' + req.params.file + '\'', function (err, rows, fields) {
                       
            if (err || rows.length == 0) {
                res.status(404).end();
                return;
            } else {

                var r = rows[0];

                if (!r['IsPublic']) {
                    if (r['Date'] && (r['LicenseDuration'] || r['DefaultLicenseDuration'])) {
                        
                        var currentDate = new Date();
                        
                        var expirationDate = new Date(r['Date']);
                        expirationDate.setDate(expirationDate.getDate() +((r['LicenseDuration']) ? r['LicenseDuration'] : r['DefaultLicenseDuration']));
                        
                        if (expirationDate < currentDate) {
                            res.status(404).end();
                            return;
                        }
                    } else {
                        var licenseTemplate = new softactivate.LicenseTemplate();
                        
                        if (!licenseTemplate) {
                            res.status(404).end();
                            return;
                        }
                        
                        var err = licenseTemplate.loadXml(r['LicenseKeyTemplate']);
                        
                        if (err != 0) {
                            res.status(404).end();
                            return;
                        }
                        
                        var keyValidator = new softactivate.KeyValidator();
                        
                        if (!keyValidator) {
                            res.status(404).end();
                            return;
                        }
                        
                        err = keyValidator.setLicenseTemplate(licenseTemplate);
                        
                        if (err != 0) {
                            res.status(404).end();
                            return;
                        }
                        
                        err = keyValidator.setKey(licenseKey);
                        
                        if (err != 0) {
                            res.status(404).end();
                            return;
                        }
                                               
                        err = keyValidator.isKeyValid();
                        
                        if (err != 0) {
                            res.status(404).end();
                            return;
                        }
                                               
                        var data = {};
                        err = licenseTemplate.getDataField('ExpirationDate', data);
                        
                        if (err == 0) {
                            
                            var data = {};
                            err = keyValidator.queryDateKeyData('ExpirationDate', data);
                            
                            if (err != 0) {
                                res.status(404).end();
                                return;
                            }
                            
                            if (new Date(data.year, data.month, data.day) < new Date()) {
                                res.status(404).end();
                                return;
                            }
                        }
                    }
                }       
                
                res.set('Content-Disposition', 'attachment; filename="' + req.params.file + '"');
                res.sendFile(app.config['download']['folder'] + '/' + req.params.file);                
                
                if (app.config['email']['downloadNotifications'] && app['mailer'] && shouldTrackDownload(req.get('Range'))) {
                    
                    var ip = req.headers['x-forwarded-for'] ||
                        req.connection.remoteAddress || 
                        req.socket.remoteAddress ||
                        req.connection.socket.remoteAddress;
                    
                    var country = 'unknown country';
                    var city = 'unknown city';
                    
                    if (ip && app['geoip']) {
                        var geo = app['geoip'].lookup(ip);
                        if (geo) {
                            if (geo['country'])
                                country = geo['country'];
                            
                            if (geo['city'])
                                city = geo['city'];
                        }
                    } else
                        ip = "unknown IP";
                                                          
			        var flags = (req.cookies['ref'] == 'ppc') ? '[P]' : ''; 

                    var mailOptions = {
                        'from': app.config['email']['from'],
                        'to': app.config['email']['downloadNotifications'],
                        'subject': '[DL] ' + flags + country + '(' + city + '), ' + req.params.file,
                        text: 'File ' + req.params.file + ' was downloaded from ' + ip + ', ' + country + ' - ' + city + '.\r\nDetailed client information:\r\n' + JSON.stringify(uaParser.setUA(req.get('user-agent')).getResult(), null, 4)
                    };

                    app['mailer'].sendMail(mailOptions, function (error, info) {
                        if (error) {
                            console.log(error);
                        } else {
                            console.log('Email sent: ' + info.response);
                        }
                    });
                } 
            }
        });
    });
    
    this.routeHandler = routeHandler;
}

function shouldTrackDownload(rangeRequest){
  if (!rangeRequest){ return true; }
 
  var matches = rangeRequest.match(/(.*)=(\d*)-(\d*)/);
  var rangeStart = parseInt(matches[2], 10);
  var shouldTrack = (rangeStart === 0);
 
  return shouldTrack;
}

module.exports = downloadController;
