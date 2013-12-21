<?php

/* schedule.php
 *
 * Dan Armendariz
 * danallan@cs.berkeley.edu
 *
 * An extremely simple script that takes a well-formed schedule TSV file
 * from the front-end and plops it into schedule.tsv.
 */

// prevent empty data and do very basic sanity checking
if(empty($_POST['tsv']) || !preg_match("/^[0-9a-z*\n\t]*$/", $_POST['tsv'])) {
    header("HTTP/1.0 400 Bad Request");
    echo "-1";
    exit;
}

@file_put_contents("schedule.tsv", $_POST['tsv']);

echo "ok";
