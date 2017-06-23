var event_types = require('../models/event_types');
var Event = require('../models/event');
var Medication = require('../models/medication');

function registry() {
    var medications = [];

    initialize();

    return {
        get_medications: get_medications,
        get_medication: get_medication,
        add_medication: add_medication,
        record_event: record_event
    };

    function initialize() {
        var m = new Medication(345);
        medications.push(m);
        m.log_event({
            code: 0,
            date: "2017-04-20T11:16:05.250Z"
        });
        m.log_event({
            code: 4,
            date: "2017-04-23T11:16:05.250Z"
        });
        m.log_event({
            code: 2,
            date: "2017-04-26T11:16:05.250Z"
        });   

        m = new Medication(160);
        medications.push(m);
        m.log_event({
            code: 0,
            date: "2017-05-02T11:16:05.250Z"
        });
        m.log_event({
            code: 1,
            date: "2017-06-02T11:16:05.250Z"
        });

        m = new Medication(220);
        medications.push(m);
        m.log_event({
            code: 0,
            date: "2017-06-06T11:16:05.250Z"
        });
        m.log_event({
            code: 3,
            date: "2017-06-08T11:16:05.250Z"
        });
    }

    function get_medications() {
        return medications;
    }

    function get_medication(id) {
        var medication = medications.find(m => m.id === id)
        if (medication) {
            return medication;
        }
        return null;
    }

    function add_medication(medication) {
        // don't add if it already was registered
        if (!get_medication(medication.id)) {
            medication.log_event(new Event(event_types.REGISTERED));
            medications.push(medication);
        }
    }

    function record_event(medication_id, event) {
        var medication = medications.find(m => m.id === medication_id)
        if (medication) {
            medication.log_event(event);
        }
    }
}

module.exports = registry();