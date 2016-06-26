///Functions to operate the device


//////////////////////////////////////////////////////////////////////////////////////
//web_parser
//the first argument determines the function that is called next.
//The arguments are loaded into the web_buffer array
//The array length is fixed at 10
void web_parser(){ 
  int next_fun = webServer.arg(0).toInt();  
  for(int i = 1; i<webServer.args(); i++){
    web_buffer[i-1] = webServer.arg(i).toInt();
    Serial.println(web_buffer[i-1]);
  }
  switch (next_fun) {
    case 1: //set the device clock
      set_rtc();
      break;
    case 2:
      //set the start delay in seconds
      set_start_delay();
      break;
    case 3:
      //set the start delay in seconds
      set_valve_duration();
    break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }
}

//set the device clock
void set_rtc(){
  ocvc.getTime();
  ocvc.printBuffer();
  ocvc.setTimeArray(web_buffer);
  ocvc.setTime();
  ocvc.getTime();
  Serial.println(ocvc.getTimeString());
  show_clock();
}
//////////////////////////////////////////////
//load values into the valve array
void set_up_valve_array(){
  for(int k = 0; k < 8; k++){
    valve_array[k].duration = 20;
  }
  set_up_valve_array_time();
}

//helper function to handle time
void set_up_valve_array_time(){
  int offset = start_delay;
  for(int k = 0; k<8; k++){
    String offset_time = ocvc.getTimeOffset(offset);
    long offset_unix = ocvc.getUnixTime();
    valve_array[k].start_time = offset_time;
    valve_array[k].start_unix = offset_unix;
    //prepare for next iteration
    offset += valve_array[k].duration;
  }
}

//change parameters for valves
////////////////////////////////////////////////////////////////
//change start delay
void set_start_delay(){
  if(web_buffer[0]==-1){
    start_delay = web_buffer[0];
  }
  if(web_buffer[0]==0){
      start_delay = 30;
  }
  if(web_buffer[0]>0){
     start_delay = web_buffer[0]*60;
  }
  //go back to valve setup
  set_up_valves();
}
/////////////////////////////////////////////////////////////////
//set valve duration
void set_valve_duration(){
  for(int k = 0; k<8; k++){
    //the case for all valves
    if(web_buffer[0] ==0) valve_array[k].duration = web_buffer[1];
    if(web_buffer[0]>0 && k==web_buffer[0]-1) valve_array[k].duration = web_buffer[1];
  }
  //go back to valve setup
  set_up_valves();
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//Control functions
/////////////////////////////////////////////////////////////////////////////
void launch_run(){
  set_up_valve_array_time();
  run_status = 1;
  show_status();
}


