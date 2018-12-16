<?php

function simpan_customerorder_android(){
    drupal_set_header('Access-Control-Allow-Origin: *');
    $postData = $_REQUEST['postParam'];
    print json_encode($postData);
    exit();
}