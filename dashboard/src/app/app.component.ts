import { Component } from '@angular/core';
import { Medication } from './medication';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {

  medication: Medication;

  onMedicationSelection(medication: Medication) {
    this.medication = medication;
  }

}
