function Medication(id) {
    this.id = id;
    this.product = lookup_product(id);
    this.event_log = [];

    function lookup_product(id) {
        if (id >= 0 && id < 100) {
            return {
                brand: "Humira",
                packaging: "Pen",
                amount: "40mg"
            }
        } else if (id >= 100 && id < 200) {
            return {
                brand: "Brand X",
                packaging: "Blister Pack",
                amount: "1000mg"
            }
        }
    }
}

Medication.prototype.log_event = function(event) {
    var existing_event = this.event_log.find(e => e.code === event.code);
    if (!existing_event) {
        this.event_log.push(event);
    }
}

module.exports=Medication;