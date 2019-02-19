<?php

function form_po(){
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
    _add_jquery_datatables_plugins();

}