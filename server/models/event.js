function Event(event_code) {
    this.code = event_code;
    this.date = new Date().toISOString();
}

Event.prototype.from_json = function (json) {
    return Object.assign(new Event(), JSON.parse(json));
}

module.exports = Event;
