"use strict";

var os = require('os'),
    ffi = require('ffi'),
    ref = require('ref'),
    voidPtr = ref.refType('void'),
    intPtr = ref.refType('int'),
    charPtr = ref.refType('char'),
    charPtrPtr = ref.refType(charPtr);

var lib = ffi.Library(__dirname + ((os.platform() == 'win32') ? '/bin/Licensing.dll' : '/bin/licensing.so'), {

    'LicenseTemplate_Create': [voidPtr, []],
    'LicenseTemplate_Destroy': ['void', [voidPtr]],
    'LicenseTemplate_LoadXml': ['int', [voidPtr, 'string']],
    'LicenseTemplate_GetNumberOfGroups': ['int', [voidPtr, intPtr]],
    'LicenseTemplate_SetNumberOfGroups': ['int', [voidPtr, 'int']],
    'LicenseTemplate_GetCharactersPerGroup': ['int', [voidPtr, intPtr]],
    'LicenseTemplate_SetCharactersPerGroup': ['int', [voidPtr, 'int']],
    'LicenseTemplate_GetDataSize': ['int', [voidPtr, intPtr]],
    'LicenseTemplate_SetDataSize': ['int', [voidPtr, 'int']],
    'LicenseTemplate_GetSignatureSize': ['int', [voidPtr, intPtr]],
    'LicenseTemplate_SetSignatureSize': ['int', [voidPtr, 'int']],
    'LicenseTemplate_GetPrivateKeyA': ['int', [voidPtr, charPtrPtr]],
    'LicenseTemplate_SetPrivateKeyA': ['int', [voidPtr, 'string']],
    'LicenseTemplate_GetPublicKeyCertificateA': ['int', [voidPtr, charPtrPtr]],
    'LicenseTemplate_SetPublicKeyCertificateA': ['int', [voidPtr, 'string']],
    'LicenseTemplate_AddDataFieldA': ['int', [voidPtr, 'string', 'int', 'int', 'int']],
    'LicenseTemplate_AddValidationFieldA': ['int', [voidPtr, 'string', 'int', 'int', 'int']],
    'LicenseTemplate_GetDataFieldA': ['int', [voidPtr, 'string', intPtr, intPtr, intPtr]],
    'LicenseTemplate_GetValidationFieldA': ['int', [voidPtr, 'string', intPtr, intPtr, intPtr]],
    'LicenseTemplate_SetValidationDataSize': ['int', [voidPtr, 'int']],

    'KeyGenerator_Create': [voidPtr, []],
    'KeyGenerator_Destroy': ['void', [voidPtr]],
    'KeyGenerator_SetKeyTemplate': ['int', [voidPtr, voidPtr]],
    'KeyGenerator_SetKeyDataA': ['int', [voidPtr, 'string', 'pointer', 'int']],
    'KeyGenerator_SetIntKeyDataA': ['int', [voidPtr, 'string', 'int']],
    'KeyGenerator_SetDateKeyDataA': ['int', [voidPtr, 'string', 'int', 'int', 'int']],
    'KeyGenerator_SetValidationDataA': ['int', [voidPtr, 'string', 'pointer', 'int']],
    'KeyGenerator_SetStringValidationDataA': ['int', [voidPtr, 'string', 'string']],
    'KeyGenerator_GenerateKeyA': ['int', [voidPtr, charPtrPtr]],

    'KeyValidator_Create': [voidPtr, []],
    'KeyValidator_Destroy': ['void', [voidPtr]],
    'KeyValidator_SetKeyTemplate': ['int', [voidPtr, voidPtr]],
    'KeyValidator_IsKeyValid': ['int', [voidPtr]],
    'KeyValidator_SetKeyA': ['int', [voidPtr, 'string']],
    'KeyValidator_QueryDateKeyDataA': ['int', [voidPtr, 'string', intPtr, intPtr, intPtr]],
    'KeyValidator_SetValidationDataA': ['int', [voidPtr, 'string', 'pointer', 'int']],
    'KeyValidator_SetStringValidationDataA': ['int', [voidPtr, 'string', 'string']],

    'SDKRegistration_SetLicenseKeyA': ['int', ['string']]

});

module.exports = {
    /**
     * @constructor
     */
    LicenseTemplate: function () {

        var object = lib['LicenseTemplate_Create']();

        if (!object)
            return null;

        this.object = object;

        this._destroy = function () {
            lib['LicenseTemplate_Destroy'](object);
        }

        this.loadXml = function (s) {
            return lib['LicenseTemplate_LoadXml'](object, s);
        }

        this.getNumberOfGroups = function () {
            var rn = ref.alloc('int');

            var e = lib['LicenseTemplate_GetNumberOfGroups'](object, rn);

            var n = rn.deref();

            return (e == 0) ? n : -1;
        }

        this.setNumberOfGroups = function (n) {
            return lib['LicenseTemplate_SetNumberOfGroups'](object, n);
        }

        this.getCharactersPerGroup = function () {
            var rn = ref.alloc('int');

            var e = lib['LicenseTemplate_GetCharactersPerGroup'](object, rn);

            var n = rn.deref();

            return (e == 0) ? n : -1;
        }

        this.setCharactersPerGroup = function (n) {
            return lib['LicenseTemplate_SetCharactersPerGroup'](object, n);
        }

        this.getDataSize = function () {
            var rn = ref.alloc('int');

            var e = lib['LicenseTemplate_GetDataSize'](object, rn);

            var n = rn.deref();

            return (e == 0) ? n : -1;
        }

        this.setDataSize = function (n) {
            return lib['LicenseTemplate_SetDataSize'](object, n);
        }

        this.getSignatureSize = function () {
            var rn = ref.alloc('int');

            var e = lib['LicenseTemplate_GetSignatureSize'](object, rn);

            var n = rn.deref();

            return (e == 0) ? n : -1;
        }

        this.setSignatureSize = function (n) {
            return lib['LicenseTemplate_SetSignatureSize'](object, n);
        }

        this.getPrivateKey = function () {
            var rk = ref.alloc(charPtr);

            var e = lib['LicenseTemplate_GetPrivateKeyA'](object, rk);

            var k = ref.readCString(rk.deref());

            return (e == 0) ? k : null;
        }

        this.setPrivateKey = function (k) {
            return lib['LicenseTemplate_SetPrivateKeyA'](object, k);
        }

        this.getPublicKeyCertificate = function () {
            var rk = ref.alloc(charPtr);

            var e = lib['LicenseTemplate_GetPublicKeyCertificateA'](object, rk);

            var k = ref.readCString(rk.deref());

            return (e == 0) ? k : null;
        }

        this.setPublicKeyCertificate = function (k) {
            return lib['LicenseTemplate_SetPublicKeyCertificateA'](object, k);
        }

        this.addDataField = function (name, type, size, offset) {
            return lib['LicenseTemplate_AddDataFieldA'](object, name, type, size, offset);
        }

        this.getDataField = function (name, data) {
            var rt = ref.alloc('int'), rs = ref.alloc('int'), ro = ref.alloc('int');
            var e = lib['LicenseTemplate_GetDataFieldA'](object, name, rt, rs, ro);
            data.type = rt.deref(); data.size = rs.deref(); data.offset = ro.deref();
            return e;
        }

        this.addValidationField = function (name, type, size, offset) {
            return lib['LicenseTemplate_AddValidationFieldA'](object, name, type, size, offset);
        }

        this.getValidationField = function (name, data) {
            var rt = ref.alloc('int'), rs = ref.alloc('int'), ro = ref.alloc('int');
            var e = lib['LicenseTemplate_GetValidationFieldA'](object, name, rt, rs, ro);
            data.type = rt.deref(); data.size = rs.deref(); data.offset = ro.deref();
            return e;
        }

        this.setValidationDataSize = function (size) {
            return lib['LicenseTemplate_SetValidationDataSize'](object, size);
        }
    },

    /**
     * @constructor
     */
    KeyGenerator: function () {

        var object = lib['KeyGenerator_Create']();

        if (!object)
            return null;

        this.object = object;

        this._destroy = function () {
            lib['KeyGenerator_Destroy'](object);
        }

        this.setLicenseTemplate = function (t) {
            return lib['KeyGenerator_SetKeyTemplate'](object, t.object);
        }

        this.setKeyData = function (name, buf, len) {
            return lib['KeyGenerator_SetKeyDataA'](object, name, buf, len);
        }

        this.setIntKeyData = function (n, v) {
            return lib['KeyGenerator_SetIntKeyDataA'](object, n, v);
        }

        this.setDateKeyData = function (n, y, m, d) {
            return lib['KeyGenerator_SetDateKeyDataA'](object, n, y, m, d);
        }

        this.setValidationData = function (name, buf, len) {
            return lib['KeyGenerator_SetValidationDataA'](object, name, buf, len);
        }

        this.setStringValidationData = function (n, v) {
            return lib['KeyGenerator_SetStringValidationDataA'](object, n, v);
        }

        this.generateKey = function () {

            var rk = ref.alloc(charPtr);

            var e = lib['KeyGenerator_GenerateKeyA'](object, rk);

            return (e == 0) ? ref.readCString(rk.deref()) : null;
        }
    },

    /**
     * @constructor
     */
    KeyValidator: function () {

        var object = lib['KeyValidator_Create']();

        if (!object)
            return null;

        this.object = object;

        this._destroy = function () {
            lib['KeyValidator_Destroy'](object);
        }

        this.setLicenseTemplate = function (t) {
            return lib['KeyValidator_SetKeyTemplate'](object, t.object);
        }

        this.isKeyValid = function () {
            return lib['KeyValidator_IsKeyValid'](object);
        }

        this.setKey = function (k) {
            return lib['KeyValidator_SetKeyA'](object, k);
        }

        this.queryDateKeyData = function (n, data) {
            var ry = ref.alloc('int'), rm = ref.alloc('int'), rd = ref.alloc('int');

            var e = lib['KeyValidator_QueryDateKeyDataA'](object, n, ry, rm, rd);

            data['year'] = ry.deref(); data['month'] = rm.deref(); data['day'] = rd.deref();

            return e;
        }

        this.setValidationData = function (name, buf, len) {
            return lib['KeyValidator_SetValidationDataA'](object, name, buf, len);
        }

        this.setStringValidationData = function (n, v) {
            return lib['KeyValidator_SetStringValidationDataA'](object, n, v);
        }
    },

    SDK: {
        setLicenseKey: function (k) {
            return lib['SDKRegistration_SetLicenseKeyA'](k);
        }
    }
}
