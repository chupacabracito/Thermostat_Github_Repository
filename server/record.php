<?php

/** thermostat.php
  * Dan Armendariz
  * danallan@cs.berkeley.edu
  *
  * Accepts data from the Arduino via HTTP GET request parameters and
  * stores it as JSON data in data.json
  * On success, this will return HTTP code 200 and a single number representing
  * the current temperature setpoint.
  * On failure, an error HTTP code will be sent with a -1 value as the setpoint
  *
  * This is a basic implementation with support for only one Arduino and
  * makes no security assurances other than verifying the data it's received.
  * Scaling this to include more Arduinos and to protect against fraud will
  * require a system that tracks individual Arduinos and, probably, passes
  * a nonce of some kind for subsequent requests. In addition, the data.json
  * file should be changed to some sort of database like sqlite to store
  * the data from many thermostats.
  */

// all times are assumed to be Pacific time
date_default_timezone_set("America/Los_Angeles");

// a list of parameters with each being:
// [param name, param type, optional]
$params = array(
    array("rf", "degF", false), // radient temp
    array("af", "degF", false), // air temp 
    array("ot", "degF", false), // operative temp
    array("h",  "bool", false), // heating on (optional)
    array("c",  "bool", false), // cooling on
    array("sh", "degF", true),  // set point Heat (optional)
    array("sc", "degF", true)   // set point Cool (optional)
);

// regular expression patterns to verify parameters
$patterns = array(
    "degF" => "/^[0-9]{1,3}$/", // 1-3 digit degrees (integer only)
    "bool" => "/^[01]$/"        // booleans
);

// build an array of data to store
$data = array();

// add our data if the required param is present and valid
foreach ($params as $p) {
    // skip bad optional params, produce an error on bad req'd params
    if(!valid($p)) 
        if ($p[2]) continue;
        else       error("400 Bad Request");

    // store data
    $data[$p[0]] = intval($_GET[$p[0]]);
}

// store the json data
@file_put_contents("data.json", json_encode($data));

// array maps TSV to date items
$t = array(date("s"), date("i"), date("H"), date("j"), date("n"), date("Y"));

// record the probable set point
$max = array(0, 0);

// output the temperature set point
if (($h = fopen("schedule.tsv", "r")) !== FALSE) {
    @fgetcsv($h, 32, "\t"); // consume the header line

    while (($d = fgetcsv($h, 32, "\t")) !== FALSE) {
        $m = array(date("s"), date("i"), date("H"), date("j"), date("n"), date("Y"));
        $seen = false;
        for($i = 0; $i < 6; $i++) {
            if($d[$i] == "*" && !$seen)
                $m[$i] = ($t[$i-1] < $d[$i-1])? $t[$i]-1 : $t[$i];
            else
                $m[$i] = $d[$i];
        }
                
        $date = mktime($m[2], $m[1], $m[0], $m[4], $m[3], $m[5]);
 
        if ($date > time()) continue;

        if ($date > $max[0]) {
            $max[0] = $date;
            $max[1] = $d[6];
        }
    }
    fclose($h);
}

echo $max[1];

// output an http error code if there's a problem
function error($code) {
    header("HTTP/1.0 $code");
    echo "-1";
    exit;
}

// a function that verifies the parameter exists and is valid
function valid($param) {
    global $patterns;
    $p = $param[0];
    return (isset($_GET[$p]) && preg_match($patterns[$param[1]], $_GET[$p]));
}

