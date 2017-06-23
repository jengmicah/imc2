import { Component, OnInit, Output, EventEmitter } from '@angular/core';
import { RegistryService } from '../registry.service';
import { Medication } from '../medication';

@Component({
  selector: 'app-registry',
  templateUrl: './registry.component.html',
  styleUrls: ['./registry.component.css'],
  providers: [ RegistryService ]
})
export class RegistryComponent implements OnInit {

  medications: Medication[];
  @Output()
  select:EventEmitter<Medication> = new EventEmitter<Medication>();
  selectedId: string;

  constructor(private registryService: RegistryService) { }

  ngOnInit() {
    this.getMedications();
  }

  getMedications() {
    console.log("Get medications.");
    this.registryService.getMedications().then(medications => this.medications = medications);
    setTimeout(() => {
      this.getMedications()
    }, 5000);
  }

  onSelect(medication: Medication) {
    if (this.selectedId == medication.id) {
      this.selectedId = null;
      this.select.emit(null);
    } else {
      this.selectedId = medication.id;
      this.select.emit(medication);
    }
  }

}
