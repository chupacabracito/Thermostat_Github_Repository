/* 
 Node.js server script 
 required node packages: express, redis, socket.io, serialport
*/


// MODULES DEPENDENCIES
var express = require('express');         // express module
var http = require('http');               // http module -> test this
var io = require('socket.io');            // socket.io package
var redis = require('redis');             // redis database package
var routes = require('./routes/routes');  // import functions javascript files for node
var serialport = require('serialport');   // serialport package
var sys = require('sys');                 // to print debug messages


// ARRAY TO HOLD THE VALUES READ IN FROM THE PART
// readData can be use this later to parse data to client, or send data as an object
// from the Arduino (look into this)
// currentTime stores the current time value
var readData = '';
var setpointsTime = [];
var timeToID = [];
var currentTime = 0;


// SETUP SERIAL PORT
var portName = "/dev/tty.usbserial-FTFOKTFX";
//var portName = "/dev/tty.usbserial-AM01QMOX";   // Arduino local system serial port
var SerialPort = serialport.SerialPort;         // localize object constructor
var sp = new SerialPort(portName, {
                        baudrate: 115200,
                        parser: serialport.parsers.readline("\r\n")
                        });


// SET UP HTTP SERVER VIA EXPRESS
var app = express();

// APP CONFIGURATION
app.set('views', __dirname + '/views');
app.set('view engine', 'jade');
app.use(express.static(__dirname + '/public'));

app.get('/', routes.index);


// SET UP REDIS CLIENT
var client = redis.createClient();

// SERVER CODE
// Start listening on port 8080
var server = app.listen(8080);                      // This is where we create the server
var io = require('socket.io').listen(server, { log: false});       // Bind socket.io module to server

// LISTENERS AND EMITTERS
io.sockets.on('connection', function (socket) {
              // This is all data that is going to get sent to the serial
              
              // Queries time from via JavaScript
              socket.on('timeQuery', function(data) {
                        currentTime = data;
                        console.log(parseInt(currentTime));
                        console.log("setpointsTime.indexOf("+currentTime+"): " + setpointsTime.indexOf(parseInt(currentTime)));
                        console.log(setpointsTime);
                        if (setpointsTime.indexOf(parseInt(currentTime)) != -1) {
                        console.log("works")
                        //var temp;
                        //client.hgetall(currentTime, function(err, res) {
                        //           console.log("works");
                        //           console.log(res);
                        //           });
                        
                        //client.hget(currentTime, "id", function(err, res) {
                        //            console.log("check id");
                        //            console.log(res);
                        //            });
                        client.hget(parseInt(currentTime), "temp", function(err, res) {
                                    console.log("check temperature");
                                    console.log("currentTime: " + currentTime + ", temp: " + res);
                                    sp.write(res);
                                    });
                        } else {
                        console.log("not works");
                        }
                        //client.flushdb(); // for cleaning the entire database
                        });
              // Sets up the key-value entry for a given setpoint to the database
              // For both when one enters a setpoint and when one modifies the values
              socket.on('spValue', function (data) {
                        console.log("testing parseInt: " + setpointsTime.indexOf(parseInt(data.time)));
                        setpointsTime[parseInt(data.id)] = parseInt(data.time);
                        //timeToID[parseInt(data.time)] = parseInt(data.id);
                        console.log(setpointsTime);
                        console.log("spValue: [ id: " + data.id + ", time: "
                                    + data.time + ", temp: " + data.temp + " ]");
                        client.hmset(parseInt(data.time),
                                     "id", parseInt(data.id),
                                     "temp", parseInt(data.temp));
                        //client.hgetall(data.time, function(err, obj) { // test
                        //            console.dir(obj);
                        //               });
                        console.log("parseInt(" + data.time + "): " + parseInt(data.time));
                        });
              
              // Removes the entry for the setpoint from the database
              socket.on('spRemove', function(data) {
                        client.del(parseInt(data));
                        console.log("parseInt(" + data + "): " + parseInt(data));
                        var index = setpointsTime.indexOf(parseInt(data));
                        console.log("index: " + index);
                        if (index > -1) {
                        console.log("splice");
                        setpointsTime.splice(index,1);
                        } else {
                        console.log("no splice");
                        }
                        //client.hgetall(data.id, function(err, obj) { // test
                        //               console.dir(obj);
                        //               });
                        });
              
              // Sets the binary heating values for the Arduino
              socket.on('heatingOnOff', function (data) {
                        sp.write(data, function(err, results) {
                                 console.log("heatingOnOff: " + data);
                                 });
                        });
              
              // Sets the binary cooling values for the Arduino
              socket.on('coolingOnOff', function (data) {
                        sp.write(data, function(err, results) {
                                 console.log("coolingOnOff: " + data);
                                 });
                        });
              });


sp.on('open',function() { console.log('Port open'); });
sp.on('close', function (err) { console.log('Port closed'); });
sp.on('error', function (err) { console.error("error", err); });

// when data arrives on the serial port, relay it via socket
// to page as message "sensorReading"
// TODO: Check with Prof. Hartmann about multiple data i/o
// Actually, I may have a solution for this that's better than current parser
sp.on("data", function (data) {
      var rTemp = data.substr(0,data.indexOf(" "));
      var aStr = data.substr(data.indexOf(" ")+1);
      var aTemp = aStr.substr(0,aStr.indexOf(" "));
      var oStr = aStr.substr(aStr.indexOf(" ")+1);
      var oTemp = oStr.substr(0, oStr.indexOf(" "));
      var heatStr = oStr.substr(oStr.indexOf(" ")+1);
      var setHeat = heatStr.substr(0,heatStr.indexOf(" "));
      var coolStr = heatStr.substr(heatStr.indexOf(" ")+1);
      var setCool = coolStr.substr(0, coolStr.indexOf(" "));
      var boolHStr = coolStr.substr(coolStr.indexOf(" ")+1);
      var boolHeat = boolHStr.substr(0, boolHStr.indexOf(" "));
      var boolCool = boolHStr.substr(boolHStr.indexOf(" ")+1);
      
      //sys.puts("received value for air temp: "+aTemp);
      //sys.puts("received value for rad temp: "+rTemp);
      //sys.puts("received value for op temp: "+oTemp);
      io.sockets.emit("sensorReadingAir",aTemp);
      io.sockets.emit("sensorReadingRad",rTemp);
      io.sockets.emit("sensorReadingOp",oTemp);
      io.sockets.emit("sensorReadingHeat",setHeat);
      io.sockets.emit("sensorReadingCool",setCool);
      io.sockets.emit("sensorReadingBoolHeat",boolHeat);
      io.sockets.emit("sensorReadingBoolCool",boolCool);
      
      });