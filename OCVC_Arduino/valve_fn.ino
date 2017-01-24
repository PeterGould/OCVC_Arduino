void reset_valves(){
  Wire.beginTransmission(VALVE1_I2C);
  Wire.write(0x0);
  Wire.write(0xa1);
  Wire.endTransmission();
  switch_valve(0,0); //turn off all valves
}



void switch_valve(byte valve, byte off_on) {
  //off_on = 0 turn all valves off, valve must also be set zero.
  //off_on = 1 turn one valve off
  //of_on = 2 turn valve on
  int on_bytes = 0;
  int off_bytes = 4096;
  byte valve_addr = 0x6;
  if(off_on == 2){
    on_bytes = 4096;
    off_bytes = 0;
  }
  if(off_on == 0){
    valve_addr = 0xFA;
  }
  Wire.beginTransmission(VALVE1_I2C);
  Wire.write(valve_addr + 4*valve);
  Wire.write(on_bytes);
  Wire.write(on_bytes>>8);
  Wire.write(off_bytes);
  Wire.write(off_bytes>>8);
  Wire.endTransmission();
}

void update_valves(){
  //update time
  ocvc.getTime();
  long now = ocvc.getUnixTime();
  //determine whether any transitions take place
  int update_needed = 0;
  for(int k = 0; k < NUM_VALVES; k++){
    long end_time = valve_array[k].start_unix + long(valve_array[k].duration);
    int new_status = valve_array[k].status;
    //do any valves need to be turned off
    if(valve_array[k].status == 1 && end_time <= now){
      new_status = 0;
      update_needed = 1;
    }
    //do any valves need to be turned on?
    if(valve_array[k].status == 0 && end_time > now && valve_array[k].start_unix <= now){
      new_status = 1;
      update_needed = 1;
    }
    //update status
    valve_array[k].status = new_status;
  }
//update valves if needed
  if(update_needed==1){
    int valves_open = 0;
    //close all valves to start
    switch_valve(0,0);
    for(int k = 0;k<NUM_VALVES; k++){
      //switch a valve on?
      if(valve_array[k].status==1 && valves_open < MAX_VALVES_ON){
        switch_valve(k,2);
        delay(50); //don't hit the power to quickly
        switch_valve((NUM_VALVES*2) - k - 1,2);
        valves_open++;
      }
    }
  }
}
