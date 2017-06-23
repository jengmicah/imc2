import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { DateSinceComponent } from './date-since.component';

describe('DateSinceComponent', () => {
  let component: DateSinceComponent;
  let fixture: ComponentFixture<DateSinceComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ DateSinceComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(DateSinceComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should be created', () => {
    expect(component).toBeTruthy();
  });
});
