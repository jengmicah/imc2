import { Component, OnInit, Input, OnChanges } from '@angular/core';
import * as moment from 'moment';

@Component({
  selector: 'app-date-since',
  templateUrl: './date-since.component.html',
  styleUrls: ['./date-since.component.css']
})
export class DateSinceComponent implements OnChanges {

  @Input()
  date: string;
  formattedDate: string;

  constructor() { }

  ngOnChanges() {
    var momentDate = moment(this.date);
    this.formattedDate = momentDate.format("dddd, MMMM Do, YYYY");
  }

}
