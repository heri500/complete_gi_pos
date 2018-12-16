<?php

function array_bahasa_premis(){
    return array(
        'Indonesia' => 'Indonesia',
        'Malaysia' => 'Malaysia',
        'English' => 'English',
    );
}

function trans($Kata = '', $Bahasa = 'Indonesia'){
    require 'kumpulankata.json.inc';
    $Terjemahan = $Kata;
    if (!empty($Kata)){
        if (isset($TerjemahKata[strtolower($Kata)]) && isset($TerjemahKata[strtolower($Kata)][$Bahasa]))
        $Terjemahan = $TerjemahKata[strtolower($Kata)][$Bahasa];
    }
    return $Terjemahan;
}

function array_ppn_type(){
    return array(
        0 => 'Include',
        1 => 'Exlude',
    );
}