/* 
 Node.js server script 
 required node packages: express, redis, socket.io, serialport
*/

// node.js server that reads sensor data from an attached Arduino board and publishes it
// Arduino must run code "nodeSerialServerArduino.ino"
// uses socket.io to communicate with a single page without reloads
// "http://localhost/" renders the page;

// MODULES DEPENDENCIES
var express = require('express');         // express module
var http = require('http');               // http module -> test this
var io = require('socket.io');            // socket.io package
var redis = require('redis');             // redis database package
var routes = require('./routes/routes');  // import functions javascript files for node
var serialport = require('serialport');   // serialport package
var sys = require('sys');                 // to print debug messages


// ARRAY TO HOLD THE VALUES READ IN FROM THE PART <- We'll use this later to parse data to client
var readData = '';


// SETUP SERIAL PORT
var portName = "/dev/tty.usbserial-FTFOKTFX";   // Arduino local system serial port
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

/*
app.configure(function() {

              app.use(express.favicon());
              app.use(express.logger('dev'));
              app.use(express.methodOverride());
              app.use(app.router);
              app.use(express.static(__dirname + '/public'));
              });

app.configure('development', function(){
              app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
              });

app.configure('production', function(){
              app.use(express.errorHandler());
              });
*/

app.get('/', routes.index);
//app.get('/users', users.list);


// SERVER CODE
//var server = http.createServer(app).listen(8080);

// Start listening on port 8080
var server = app.listen(8080);                      // This is where we create the server
var io = require('socket.io').listen(server);       // Bind socket.io module to server



// LISTENERS AND EMITTERS
io.sockets.on('connection', function (socket) {
              // This is all data that is going to get sent to the serial
              // CoolingOn/Off
              socket.on('spValue', function (data) {
                        console.log("spValue: " + data);
                        });
              // heatingOnOff
              socket.on('heatingOnOff', function (data) {
                        console.log("heatingOnOff: " + data);
                        sp.write(data, function(err, results) {
                                 //console.log('err ' + err);
                                 console.log('results ' + results);
                                 });
                        });
              // coolingOnOff
              socket.on('coolingOnOff', function (data) {
                        console.log("coolingOnOff: " + data);
                        sp.write(data, function(err, results) {
                                 //console.log('err ' + err);
                                 console.log('results ' + results);
                                 });
                        });
              });


sp.on('open',function() { console.log('Port open'); });
sp.on('close', function (err) { console.log('Port closed'); });
sp.on('error', function (err) { console.error("error", err); });

// when data arrives on the serial port, relay it via socket
// to page as message "sensorReading"
// TODO: Check with Prof. Hartmann about multiple data i/o
sp.on("data", function (data) {
      var rTemp = data.substr(0,data.indexOf(","));
      var aStr = data.substr(data.indexOf(",")+1)
      var aTemp = aStr.substr(0,data.indexOf(","));
      var oTemp = aStr.substr(data.indexOf(",")+1);
      //sys.puts("received value for air temp: "+aTemp);
      //sys.puts("received value for rad temp: "+rTemp);
      //sys.puts("received value for op temp: "+oTemp);
      io.sockets.emit("sensorReadingAir",aTemp);
      io.sockets.emit("sensorReadingRad",rTemp);
      io.sockets.emit("sensorReadingOp",oTemp);
      });