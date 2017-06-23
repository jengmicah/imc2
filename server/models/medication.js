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
                brand: "Viekira Pak",
                packaging: "XR",
                amount: ""
            }
        } else if (id >= 200 && id < 300) {
            return {
                brand: "Kaletra",
                packaging: "Oral Solution",
                amount: "80mg"
            }    
        } else if (id >= 300 && id < 400) {
            return {
                brand: "Androgel",
                packaging: "1.62%",
                amount: ""
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