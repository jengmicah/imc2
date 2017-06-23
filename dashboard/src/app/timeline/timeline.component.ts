import { Component, OnInit, Input, OnChanges } from '@angular/core';
import { Medication } from '../medication';

@Component({
  selector: 'app-timeline',
  templateUrl: './timeline.component.html',
  styleUrls: ['./timeline.component.css']
})
export class TimelineComponent implements OnChanges {

  @Input()
  medication: Medication;

  constructor() { }

  ngOnChanges() {
  }

  getSeverityClass(eventCode: string): string {
    switch (eventCode.toString()) {
      case "0":
        return "success";
      case "1":
        return "danger";
      case "2":
        return "";
      case "3":
        return "danger";
      case "4":
        return "warning";
      default:
        return "success";
    }
  }

}
