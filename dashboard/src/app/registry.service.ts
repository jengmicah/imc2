import { Injectable } from '@angular/core';
import { Headers, Http } from '@angular/http';
import 'rxjs/add/operator/toPromise';
import { Medication } from './medication';
import { LogEvent } from './log-event';

@Injectable()
export class RegistryService {

  private headers;
  private registryUrl;
  
  private static handleError(error: any): Promise<any> {
    return Promise.reject(error.message || error);
  }

  constructor(private http: Http) {
    this.headers = new Headers({'Content-Type': 'application/json'});
    this.registryUrl = "http://ec2-52-14-14-114.us-east-2.compute.amazonaws.com/medications";
  }

  getMedications(): Promise<Medication[]> {
    return this.http.get(this.registryUrl)
      .toPromise()
      .then(response => (response.json() as Medication[]).map( m => this.populateNewMedication(m)))
      .catch(RegistryService.handleError);
  }

  private populateNewMedication(medication: Medication): Medication {
    var m = new Medication();
    m.fillFromJSON(JSON.stringify(medication));
    m.latest_status = this.getLatestStatus(medication.event_log);
    return m;
  }

  private getLatestStatus(events:LogEvent[]):string {
      return events[events.length - 1].code;
  }
}
