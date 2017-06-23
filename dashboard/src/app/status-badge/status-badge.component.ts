import { Component, OnInit, Input, OnChanges } from '@angular/core';
import { Medication } from '../medication';

@Component({
  selector: 'app-status-badge',
  templateUrl: './status-badge.component.html',
  styleUrls: ['./status-badge.component.css']
})
export class StatusBadgeComponent implements OnChanges {
  @Input()
  status: string = "none";
  severity: string = "default";
  label: string;

  ngOnChanges() {
    switch (this.status.toString()) {
      case "0":
        this.label = "Registered";
        this.severity = "success"
        break;
      case "1":
        this.label = "Expired";
        this.severity = "danger";
        break;
      case "2":
        this.label = "Used";
        break;
      case "3":
        this.label = "Exception";
        this.severity = "danger"
        break;
      case "4":
        this.label = "Countdown";
        this.severity = "warning";
        break;
      default:
        this.label = "OK";
        this.severity = "success"
        break;
    }
  }

}
