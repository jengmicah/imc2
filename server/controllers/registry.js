var registry = require('../models/medication_registry');
var Medication = require('../models/medication');
var Event = require('../models/event');

exports.handle_add_medication = function (req, resp) {
    var id = req.body.id;
    registry.add_medication(new Medication(id));
    resp.sendStatus(200);
}

exports.handle_get_medications = function (req, resp) {
    resp.send(JSON.stringify(registry.get_medications()));
}

exports.handle_get_medication = function (req, resp) {
    var id = req.params.id;
    var medication = registry.get_medication(id);
    if (medication) {
        resp.send(JSON.stringify(medication));
    } else {
        resp.sendStatus(404);
    }
}

exports.handle_log_event = function (req, resp) {
    var medication_id = req.params.id;
    var event = Object.assign(new Event(), req.body);
    registry.record_event(medication_id, event);
    resp.sendStatus(200);
}