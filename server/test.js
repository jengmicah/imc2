var registry = require('./models/medication_registry');
var Medication = require('./models/medication');
var Event = require('./models/event');

console.log("Registry Count: " + registry.get_medications().length);

registry.add_medication(new Medication(1));

console.log("Registry Count: " + registry.get_medications().length);

console.log(registry.get_medications());

var e = new Event(1234);
console.log(e);

registry.record_event(1, e);

console.log(JSON.stringify(registry.get_medications()));
