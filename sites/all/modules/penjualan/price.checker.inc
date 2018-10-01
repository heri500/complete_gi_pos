<?php

function price_checker(){
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    _addBarcodePlugins2();
    drupal_add_js('misc/media/datatables/js/number.format.js');
    drupal_add_js(
        array(
            'dec_digit' => $DecDigit,
        ),
        'setting');
    $js_path = drupal_get_path('module','penjualan').'/js/price.checker.js';
    drupal_add_js($js_path);
    $PriceChecker = '<input type="text" style="height: 25px;width: 100%;" placeholder="scan barcode here" id="barcode-check">';
    $PriceChecker .= '<div align="center" style="width: 100%;min-height: 400px;">';
    $PriceChecker .= '<div id="product-name" style="font-size: 100px;padding: 20px;line-height: 1;"></div>';
    $PriceChecker .= '<svg id="barcode-place" class="barcode"></svg>';
    $PriceChecker .= '<div id="product-price" style="font-size: 125px;padding: 50px;font-weight: bold;"></div>';
    $PriceChecker .= '</div>';
    return $PriceChecker;
}