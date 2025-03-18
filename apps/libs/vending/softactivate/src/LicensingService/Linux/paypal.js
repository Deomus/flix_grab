"use strict";

var bodyParser = require('body-parser'),
    PaymentServiceController = require('./paymentservice.js');

/** 
 * @constructor
 */
function paypalController(app) {
    
    if (!(this instanceof paypalController)) {
        return new paypalController(app);
    }
    
    var paymentServiceController = PaymentServiceController(app);
    
    var routeHandler = app.express.Router();
    
    routeHandler.use(bodyParser.urlencoded({
        'extended': true
    }));
    
    routeHandler.post('/', function (req, res) {
        
        if (req.body['txn_type'] != 'web_accept')
            res.status(200).end();
        else {
            paymentServiceController.processOrderNotificationRequest(req.body['txn_id'], req.body['item_number'], req.body['quantity'], req.body['mc_gross'], req.body['mc_currency'], req.body['first_name'], req.body['last_name'], req.body['payer_business_name'], req.body['payer_email'], req.body['contact_phone'], req.body['address_street'], req.body['address_zip'], req.body['address_city'], req.body['address_state'], req.body['residence_country'], true, function (err, keys) {
                if (err) {
                    res.status(404).end();
                } else {
                    res.status(200).end();
                }
            });
        }
    });
    
    this.routeHandler = routeHandler;
}

module.exports = paypalController;
