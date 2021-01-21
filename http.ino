
enum { NONE=0, INDEX, XML };

 void modeHttp(void)
 {
  WiFiClient client = server.available();   // listen for incoming clients
 int pos, page = 0;
 
  if (client) {                             // if you get a client,
 //   Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
//        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            if(page==INDEX) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              // the content of the HTTP response follows the header:
  
              client.println("<!DOCTYPE HTML><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>ESP32LR42</title></head><body>");
              client.println("<p style='text-align:center'><b>ESP32LR42</b> <br></p>");
              client.println("<style>button{width:44%;height:35px;margin:5px;}</style>");
              
              client.println("<button id='Rly1'; onclick='setRly(1)'; >Relay 1</button>");
              client.println("<button id='Rly2'; onclick='setRly(2)'; >Relay 2</button>");
              client.println("<button id='Rly3'; onclick='setRly(3)'; >Relay 3</button>");
              client.println("<button id='Rly4'; onclick='setRly(4)'; >Relay 4</button>");

              client.println("<br><br><div> Inputs 1<input type='radio' value='' id='Inp1'>");
              client.println("<input type='radio' id='Inp2'>2 </div>");
              
              client.println("<script>");
              client.println("var xhttp = new XMLHttpRequest();");
              client.println("var xmlDoc;");
              client.println("var Rly=0;");
              client.println("var Working=50;");
              client.println("setInterval(toggleRly, 10);");              
              client.println("xhttp.onreadystatechange = function() {");
              client.println("    if (this.readyState == 4 && this.status == 200) {");
              client.println("        myFunction(this);");
              client.println("    }");
              client.println("};");

              client.println("function setRly(num) {");
              client.println("    Rly = num");
              client.println("};");
              
              client.println("function getValue(tag) {");
              client.println("    var x = xmlDoc.getElementsByTagName(tag)[0];");
              client.println("    var y = x.childNodes[0].nodeValue;");
              client.println("    return y;");
              client.println("};");
              client.println("function toggleRly() {");
              client.println("  var file = '';");
              client.println("  if(Working>0) { --Working; return; }");
              client.println("  switch(Rly) {");
              client.println("    case 0: file = \"status.xml\"; break;");
              client.println("    case 1: file = \"?Rly1=2\"; break;");
              client.println("    case 2: file = \"?Rly2=2\"; break;");
              client.println("    case 3: file = \"?Rly3=2\"; break;");
              client.println("    case 4: file = \"?Rly4=2\"; break;");
              client.println("  };");
              client.println("  Rly = 0; Working = 500;");
              client.println("  xhttp.open(\"GET\", file, true);");
              client.println("  xhttp.send();");
              client.println("}");
              client.println("function myFunction(xml) {");
              client.println("  xmlDoc = xml.responseXML;");
              client.println("  document.getElementById('Rly1').style.backgroundColor = (getValue('RLY1')=='on') ? 'rgba(255,0,0,0.2)' : 'rgba(0,0,255,0.2)';");
              client.println("  document.getElementById('Rly2').style.backgroundColor = (getValue('RLY2')=='on') ? 'rgba(255,0,0,0.2)' : 'rgba(0,0,255,0.2)';");
              client.println("  document.getElementById('Rly3').style.backgroundColor = (getValue('RLY3')=='on') ? 'rgba(255,0,0,0.2)' : 'rgba(0,0,255,0.2)';");
              client.println("  document.getElementById('Rly4').style.backgroundColor = (getValue('RLY4')=='on') ? 'rgba(255,0,0,0.2)' : 'rgba(0,0,255,0.2)';");
              client.println("  if(getValue('INP1')=='1') document.getElementById('Inp1').checked = true; else document.getElementById('Inp1').checked = false;");
              client.println("  if(getValue('INP2')=='1') document.getElementById('Inp2').checked = true; else document.getElementById('Inp2').checked = false;");
              client.println("  Working=0;");
              client.println("}");

              client.println("</script>");
              client.println("");
  
              client.print("</body>");
              
              // The HTTP response ends with another blank line:
              client.println();             
            }
            else if(page==XML) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:application/xml");
              client.println();
              client.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
              client.println("<ESP32LR42DATA>");
              client.println("<RELAYS>");
              if(digitalRead(Rly1)) client.println("<RLY1>on</RLY1>"); else client.println("<RLY1>off</RLY1>");
              if(digitalRead(Rly2)) client.println("<RLY2>on</RLY2>"); else client.println("<RLY2>off</RLY2>");
              if(digitalRead(Rly3)) client.println("<RLY3>on</RLY3>"); else client.println("<RLY3>off</RLY3>");
              if(digitalRead(Rly4)) client.println("<RLY4>on</RLY4>"); else client.println("<RLY4>off</RLY4>");
              client.println("</RELAYS>");
              client.println("<INPUTS>");
              if(digitalRead(Inp1)) client.println("<INP1>1</INP1>"); else client.println("<INP1>0</INP1>");
              if(digitalRead(Inp2)) client.println("<INP2>1</INP2>"); else client.println("<INP2>0</INP2>");
              client.println("</INPUTS>");
              client.println("</ESP32LR42DATA>");
              client.println();                  
            }
            else {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              client.println();              
            }
            // break out of the while loop:
            break;
          } else {    // check for a GET command
            if(currentLine.startsWith("GET / ")) page=INDEX;
            else if(currentLine.startsWith("GET /INDEX.HTM")) page=INDEX;
            else if(currentLine.startsWith("GET /STATUS.XML")) page=XML;
            else if(currentLine.startsWith("GET /?RLY")) page=XML;
            currentLine = "";
          }
          
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          c = toupper(c);
          currentLine += c;      // add it to the end of the currentLine
        }

        if(currentLine.length()==12) {
          pos = currentLine.indexOf('?');
          if( (currentLine.substring(pos, pos+4) == "?RLY") && (pos>0) ) {
            int rly = currentLine.charAt(pos+4)-0x30;
            int action = currentLine.charAt(pos+6)-0x30;
            switch(rly) {
              case 1:
                switch(action) {
                  case 0: digitalWrite(Rly1, 0); break;
                  case 1: digitalWrite(Rly1, 1); break;
                  case 2: digitalWrite(Rly1, !digitalRead(Rly1)); break;
                }
                break;
              case 2:
                switch(action) {
                  case 0: digitalWrite(Rly2, 0); break;
                  case 1: digitalWrite(Rly2, 1); break;
                  case 2: digitalWrite(Rly2, !digitalRead(Rly2)); break;
                }
                break;
              case 3:
                switch(action) {
                  case 0: digitalWrite(Rly3, 0); break;
                  case 1: digitalWrite(Rly3, 1); break;
                  case 2: digitalWrite(Rly3, !digitalRead(Rly3)); break;
                }
                break;
              case 4:
                switch(action) {
                  case 0: digitalWrite(Rly4, 0); break;
                  case 1: digitalWrite(Rly4, 1); break;
                  case 2: digitalWrite(Rly4, !digitalRead(Rly4)); break;
                }
                break;
            }
          }  
        }
      }
    }
    // close the connection:
    client.stop();
//Serial.println("Client Disconnected.");
  }
 }
