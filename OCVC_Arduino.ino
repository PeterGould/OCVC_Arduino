#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "OCVC.h"
#include <Wire.h>


//web setup
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

//Global variables
//buffer to hold web arguments
int web_buffer[10];
int run_status = 0;
int start_delay = 0;
OCVC ocvc;

///struct to hold valve status
struct valve_parms{
  String start_time;
  long start_unix;
  int duration;
};

valve_parms valve_array[8];

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("DNSServer example");
  ocvc.begin();
  set_up_valve_array();
  Serial.begin(9600);
  // modify TTL associated  with the domain name (in seconds)
  // default is 60 seconds
  dnsServer.setTTL(300);
  // set which return code will be used for all other domains (e.g. sending
  // ServerFailure instead of NonExistentDomain will reduce number of queries
  // sent by clients)
  // default is DNSReplyCode::NonExistentDomain
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

  // start DNS server for a specific domain name
  dnsServer.start(DNS_PORT, "www.olycirc.com", apIP);

  //direct web pages and functions
  webServer.on("/",main_page);
  webServer.on("/control",control);
  webServer.on("/web_parser",web_parser);
  webServer.on("/show_status",show_status);
  //status functions
  webServer.on("/show_valve_status",show_valve_status);
  webServer.on("/show_run_status",show_run_status);
  //setup functions
  webServer.on("/set_up",set_up);
  webServer.on("/set_up/set_up_valves",set_up_valves);
  webServer.on("/set_up/set_up_clock",set_up_clock);
  webServer.on("/set_up/show_clock",show_clock);
  //control functions
   webServer.on("/launch_run",launch_run);

  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}







