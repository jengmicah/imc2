var event_types = require('../models/event_types');
var Event = require('../models/event');

function registry() {
    var medications = [];

    return {
        get_medications: get_medications,
        get_medication: get_medication,
        add_medication: add_medication,
        record_event: record_event
    };

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