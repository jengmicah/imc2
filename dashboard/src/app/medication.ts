import { Product } from './product';
import { LogEvent } from './log-event';
import { Serializable } from './serializable';

export class Medication extends Serializable {
    public id: string;
    public product: Product;
    public event_log: LogEvent[];
    public latest_status: string;

    constructor() {
        super();
    }
}
