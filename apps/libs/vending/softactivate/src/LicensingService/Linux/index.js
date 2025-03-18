"use strict";

var mysql = require('mysql');
var express = require('express');
var nodemailer = require('nodemailer');
var smtpTransport = require('nodemailer-smtp-transport');
var geoip = require('geoip-lite');

var app = express();

var defaultConfig = require('./config.json');
var softactivate = require('./softactivate.js');
var downloadController = require('./download.js');
var activationController = require('./activation.js');
var paypalController = require('./paypal.js');

/**
 * @constructor
 */
function licensingService(config) {

    if (!(this instanceof licensingService)) {
        return new licensingService(config);
    }
    
    if (!config)
        config = defaultConfig;
    
    if (!(config['download'] && config['download']['folder']))
        config['download'] = { 'folder': __dirname + '/downloads' };

    var connectionPool = mysql.createPool({
        'connectionLimit': config['db']['connectionLimit'],
        'database': config['db']['name'],
        'host': config['db']['host'],
        'user': config['db']['user'],
        'password': config['db']['password']
    });

    this.connectionPool = connectionPool;
    this.config = config;
    this.express = express;
    this.app = app;
    
    if (config['email']['smtpUser']) {
        this['mailer'] = nodemailer.createTransport(smtpTransport({
            'host': config['email']['smtpHost'],
            'port': config['email']['smtpPort'],
            'auth': {
                'user': config['email']['smtpUser'],
                'pass': config['email']['smtpPassword']
            },
            'secure': config['email']['smtpSecure']
        }));
    }

    this['geoip'] = geoip;

    var routeHandler = express.Router();

    routeHandler.get('/', function (req, res) {
        res.status(200).end();
    });
    
    var e = softactivate.SDK.setLicenseKey(config['sdk']['licenseKey']);
    
    if (e != 0) {
        return null;
    }  

    var downloadManager = downloadController(this);
    this['downloadManager'] = downloadManager;
    routeHandler.use('/downloads', downloadManager.routeHandler);

    var paypalManager = paypalController(this);
    this['paypalManager'] = paypalManager;
    routeHandler.use('/api/paypal/ordernotification', paypalManager.routeHandler);

    var activationManager = activationController(this);
    this['activationManager'] = activationManager;
    routeHandler.use('/Activate.ashx', activationManager.routeHandler);

    this.routeHandler = routeHandler;
};

if (!module.parent) {
    app.use('/', licensingService().routeHandler);
    app.listen(defaultConfig['web']['httpPort']);
    console.log('licensing service started');
} else
    module.exports = licensingService;
