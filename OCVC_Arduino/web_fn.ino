#include <Arduino.h>

//starting home page
void main_page(){
  String message = "<HTML><HEAD>"
                    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                    "<TITLE>Olympia Circuits</TITLE></HEAD>\n"
                    "<strong> Valve Controller Version 0.1</strong><br> Olympia Circuits peter@olympiacircuits.com <br><br>"
                    "<input type=\"button\" value=\"Status\" onclick=\"top.frames['statusBox'].location.href='/show_status'\">&nbsp&nbsp"
                    "<input type=\"button\" value=\"Control\" onclick=\"top.frames['statusBox'].location.href='/control'\">&nbsp&nbsp"
                    "<input type=\"button\" value=\"Setup\" onclick=\"top.frames['statusBox'].location.href='/set_up'\">"
                    "<br>"
                     "<iframe src='http://www.olycirc.com/show_status' width='90%' height='70%' name='statusBox' id = 'statusBox'>\n"
                      "</iframe><BR></HTML>\n";
  webServer.send(200, "text/html", message);
}


void control(){
  Serial.print("entered control");
  String message = "<input type=\"button\" value=\"Launch\" onclick=\"top.frames['statusBox'].location.href='/launch_run'\">&nbsp&nbsp"
                    "<input type=\"button\" value=\"Stop\" onclick=\"top.frames['statusBox'].location.href='/stop_run'\">&nbsp&nbsp"
                    "<input type=\"button\" value=\"Manual\" onclick=\"top.frames['statusBox'].location.href='/manual'\">";
  webServer.send(200, "text/html", message);
}



//////////////////////////////////////////////////////////////////////////
//The setup page
///////////////////////////////////////////////////////////////////////
void set_up(){
   String message = "<input type=\"button\" value=\"Valves\" onclick=\"window.location.href='/set_up/set_up_valves'\">&nbsp&nbsp"
   "<input type=\"button\" value=\"Vacuum\" onclick=\"window.location.href='/set_up/set_up_vacuum'\">&nbsp&nbsp"
   "<input type=\"button\" value=\"Sync Clock\" onclick=\"window.location.href='/set_up/set_up_clock'\">&nbsp&nbsp";
   webServer.send(200, "text/html", message);
}

void set_up_clock(){
  String message =  "<script type='text/javascript'>\n"
                    "var today = new Date();\n"
                    "var yr = today.getFullYear();\n"
                    "var mnth = today.getMonth()+1;\n"
                    "var dy = today.getDate();\n"
                    "var hr = today.getHours();\n"
                    "var mins = today.getMinutes();\n"
                    "var secs = today.getSeconds();\n"
                    "var xmlHttp = new XMLHttpRequest();\n"
                    "var all = document.getElementsByTagName('*');"
                    "xmlHttp.onreadystatechange=function()"
                    "{"
                    "if (xmlHttp.readyState==4 && xmlHttp.status==200)"
                    "{"
                    "window.location = '/set_up/show_clock';"
                    "}"
                    "}\n"
                    "var full_url = 'http://www.olycirc.com/web_parser?f_type = 1&y=' + yr + '&mo=' + mnth + '&d=' + dy + '&h=' + hr + '&m=' + mins + '&s=' + secs;\n"
                   "xmlHttp.open('GET', full_url, true);\n"
                   "xmlHttp.send();\n"
                    "</script>\n";
  webServer.send(200,"text/html",message);
}

void show_clock(){
   float battery_voltage;
   battery_voltage = float(analogRead(A0))/1023.0f*100.0f*0.1493f;
   String message ="<HTML><HEAD> <meta http-equiv='refresh' content='1'> </head><BODY>"
   "Device time: " + ocvc.getTimeString() + "<br>" +
   "Battery Voltage: " + battery_voltage +
   "</BODY></HTML>";
   webServer.send(200,"text/html",message);
}

//////////////////////////////////////////////////////
//Menu and functions for setting valve durations
////////////////////////////////////////////////////
void set_up_valves(){
  String message =
  "<form action='/set_config'>"
  "<input type='submit' style='width:150px; height:50px; font-size: 16px;' value='Save Settings'>"
  "</form>"
  "<strong><p>Set Initial Delay</p>"
  "<form action='/web_parser?f_type=2'>"
  "<input type='hidden' name='f_type' value='2'>"
  "Delay</strong>&nbsp&nbsp"
  "<select name='valve'>"
  "<option value='-1'>None</option>"
  "<option value='0'>30 sec</option>"
  "<option value='1'>1 min</option>"
  "<option value='5'>5 min</option>"
  "</select>"
  "&nbsp&nbsp"
  "<input type='submit' value='Submit'>"
  "</form>"

  "<form action='/web_parser?f_type=3'>"
  "<input type='hidden' name='f_type' value='3'>"
  "<strong><p>Set Valve Time</p>"
  "&nbsp&nbspValve</strong>&nbsp&nbsp"
  "<select name='valve'>"
  "<option value='0'>All</option>";

  //loop for 8 valves;
  for(int k = 1;k<9;k++){
    String add1 = "<option value='" + String(k) + "'>" + String(k) + "</option>";
    message = message + add1;
  }
  message = message +
            "</select>";
//duration options
  message = message +
  "<strong>&nbsp&nbspDuration</strong>&nbsp&nbsp"
  "<select name='duration'>"
  "<option value='0'>Off</option>"
  "<option value='1'>1</option>";
  int k = 5;
  while(k < 60){
    String add2 = "<option value='" + String(k) + "'>" + String(k) + "</option>";
    message = message + add2;
    k+=5;
  }
  message = message + "</select>"
  "&nbsp&nbsp"
  "<input type='submit' value='Submit'>"
  "</form>"
  "<iframe src = 'http://www.olycirc.com/show_valve_status' width = '100%' height = '80%' id = 'valveBox2'> </iframe><br>";;
  webServer.send(200,"text/html",message);
}


//////////////////////////////////////////////////////
//run vacuum pump
////////////////////////////////////////////////////
void set_up_vacuum(){
  String message =
  "<form action='/web_parser?f_type=4'>"
  "<input type='hidden' name='f_type' value='4'>"
  "Level"
  "<input type='range' name='vacuum' min='0' max='1023'>"
  "<input type='submit'>"
  "&nbsp&nbsp"
  "</form>";
  webServer.send(200,"text/html",message);
}



/////////////////////////////////////////////////////////////////////////////
//status block
////////////////////////////////////////////////////////////////////////////
/////////////////////////////////
void show_status(){
  //show clock at the top
  String message =  "<iframe src='http://www.olycirc.com/set_up/show_clock' width='100%' height='10%' name='clockBox' id = 'clockBox'> </iframe><br>"
                    "<iframe src = 'http://www.olycirc.com/show_run_status' width = '100%' height = '10%' id = 'runBox'> </iframe><br>"
                    "<iframe src = 'http://www.olycirc.com/show_valve_status' width = '100%' height = '80%' id = 'valveBox'> </iframe><br>";
  webServer.send(200, "text/html", message);
}

////////////////////////////////////////////////
void show_run_status(){
  String message;
  if(run_status==0){
    message = "<strong>OFF</strong>";
  }
  if(run_status==1){ //indicates device is running
    message = process_run_status();
  }

  message = "<HTML><HEAD> <meta http-equiv='refresh' content='5'> </head><BODY>" + message + "</BODY></HTML>";
  webServer.send(200,"text/html",message);
}

//helper functions
String process_run_status(){
  //which valve is running
  ocvc.getTime();
  String show_status;
  long now = ocvc.getUnixTime();
  for(int k = 0; k<8; k++){
    long end_time = valve_array[k].start_unix + valve_array[k].duration * 60L;
    Serial.print(now);
    Serial.print(" ");
    Serial.print(valve_array[k].start_unix);
    Serial.print(" ");
    Serial.println(end_time);
    if(valve_array[k].start_unix > now && now < end_time){
      int to_go = int(end_time - now);
      int min_to_go = to_go / 60;
      int sec_to_go = to_go % 60;
      String add1 = "";
      String add2 = "";
      if(min_to_go < 10) add2 = "0";
      if(sec_to_go < 10) add1 = "0" ;
      show_status = "ON: Valve " + String(k) + ":" + String(add2) + String(min_to_go) + ":" + String(add1) + String(sec_to_go);
      break;
    }
  }
  return(show_status);
}

////////////////////////////////////////////////
void show_valve_status(){
  String message = "";
  if(run_status != 1){
    message+= "<strong>Delay:</strong>" + String(start_delay) + "<br>";
  }
  for(int k = 0;k< 8; k++){
    String new_message = "<strong>Valve ";
    new_message = new_message + String(k+1) + "</strong>";
   if(run_status==1) new_message = new_message + " Start " + String(valve_array[k].start_time).substring(10,19);
    new_message = new_message + " Duration " + String(valve_array[k].duration);
    message = message + new_message + "<br>";
  }
  webServer.send(200,"text/html",message);
}


void manual_valve_settings(){
  String message = "<form name='manual_valve' method='get' action='/web_parser?f_type=5'>";
  message = message + "<input type='hidden' name='f_type' value='5'>";
  for(int k = 0; k <= NUM_VALVES;k++){
      message = message + "<input type='radio' name='manual_valve' value='" + String(k) + "'onclick='this.form.submit();'>";
    if(k==0){
      message = message + "All Off<br>";
    }else{
      message = message + "Valve " + String(k) + "<br>";
    }
  }
  message = message + "</form>";
  webServer.send(200,"text/html",message);
}
