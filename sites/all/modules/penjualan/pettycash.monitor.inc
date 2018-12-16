<?php

function get_active_cashier(){
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
    drupal_add_css('misc/media/datatables.1.10/media/css/dataTables.jqueryui.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.dataTables.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.jqueryui.css');
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
    drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
    _addJeditablePlugins();
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.dataTables.js');
    drupal_add_js('misc/media/datatables.1.10/media/js/dataTables.jqueryui.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/dataTables.buttons.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.flash.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.html5.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.print.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.colVis.min.js');
    drupal_add_js(
        array(
            'gstSym' => $gstSym,
            'dDigit' => $DecDigit,
            'urutan' => 1,
        ),
        'setting');
    drupal_add_js('misc/media/datatables/js/number.format.js');
    $modulePath = drupal_get_path('module','penjualan');
    $jsPath = $modulePath.'/js/tabel.cashmonitor.js';
    drupal_add_js($jsPath);
    $ViewTable = table_cash_monitor();
    $CashMonitor = '<div id="viewcashmonitor">'.$ViewTable.'</div>';
    return $CashMonitor;
}

function table_cash_monitor(){
    get_number_format($currencySym, $thousandSep, $decimalSep,false);
    $TabelMonitor ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_cash_monitor">';
    $TabelMonitor .= '<thead>';
    $TabelMonitor .= '<tr>';
    $TabelMonitor .= '<th>USER</th>';
    $TabelMonitor .= '<th>LOGIN</th>';
    $TabelMonitor .= '<th class="tanggal">IP</th>';
    $TabelMonitor .= '<th>PETTY CASH</th>';
    $StrSqlCaraBayar = 'SELECT carabayar FROM carabayar';
    $result = db_query($StrSqlCaraBayar);
    while ($data = db_fetch_object($result)) {
        $TabelMonitor .= '<th>'.$data->carabayar.'</th>';
    }
    $TabelMonitor .= '<th>KEMBALI</th>';
    $TabelMonitor .= '<th>TOTAL CASH</th>';
    $TabelMonitor .= '<th class="tablebutton">&nbsp;</th>';
    $TabelMonitor .= '<th class="tablebutton">&nbsp;</th>';
    $TabelMonitor .= '</tr>';
    $TabelMonitor .= '</thead>';
    $TabelMonitor .= '<tbody>';
    $TabelMonitor .= '</tbody>';
    $TabelMonitor .= '<tfoot>';
    $TabelMonitor .= '<tr>';
    $TabelMonitor .= '</tr>';
    $TabelMonitor .= '</tfoot>';
    $TabelMonitor .= '</table>';
    return $TabelMonitor;
}

function cashier_log(){
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
    drupal_add_css('misc/media/datatables.1.10/media/css/dataTables.jqueryui.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.dataTables.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.jqueryui.css');
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
    drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
    _addJeditablePlugins();
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.dataTables.js');
    drupal_add_js('misc/media/datatables.1.10/media/js/dataTables.jqueryui.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/dataTables.buttons.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.flash.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.html5.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.print.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.colVis.min.js');
    set_default_time_zone();
    $tgl1 = date('Y-m-d', mktime(0,0,0, date('n'), date('j') - 2, date('Y')));
    $tgl2 = date('Y-m-d');
    if ($_POST["tgl1"] AND $_POST["tgl2"]){
        $tgl1 = $_POST["tgl1"];
        $tgl2 = $_POST["tgl2"];
    }
    drupal_add_js(
        array(
            'gstSym' => $gstSym,
            'dDigit' => $DecDigit,
            'urutan' => 1,
            'date_from' => $tgl1.' 00:00',
            'date_thru' => $tgl2.' 23:59',
        ),
        'setting');
    drupal_add_js('misc/media/datatables/js/number.format.js');
    $modulePath = drupal_get_path('module','penjualan');
    $jsPath = $modulePath.'/js/tabel.cashierlog.js';
    drupal_add_js($jsPath);
    $pilihperiode = '<form action="'.base_path().'penjualan/cashierlog" method="post"><div id="pilihanperiode"><label>PILIH LOG PERIOD </label><input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.$tgl1.'">';
    $pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.$tgl2.'">'.$pilihantampil.'&nbsp;'.$addedFilter.'<button>LIHAT PENJUALAN</button></div></form>';
    $ViewTable = table_cashier_log();
    $CashMonitor = $pilihperiode.'<div id="viewcashmonitor">'.$ViewTable.'</div>';
    return $CashMonitor;
}

function table_cashier_log(){
    $TabelMonitor ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_cash_monitor">';
    $TabelMonitor .= '<thead>';
    $TabelMonitor .= '<tr>';
    $TabelMonitor .= '<th>USER</th>';
    $TabelMonitor .= '<th>LOGIN</th>';
    $TabelMonitor .= '<th>LOGOUT</th>';
    $TabelMonitor .= '<th class="tanggal">IP</th>';
    $TabelMonitor .= '<th>PETTY CASH</th>';
    $StrSqlCaraBayar = 'SELECT carabayar FROM carabayar';
    $result = db_query($StrSqlCaraBayar);
    while ($data = db_fetch_object($result)) {
        $TabelMonitor .= '<th>'.$data->carabayar.'</th>';
    }
    $TabelMonitor .= '<th>KEMBALI</th>';
    $TabelMonitor .= '<th>TOTAL CASH</th>';
    $TabelMonitor .= '<th class="tablebutton">&nbsp;</th>';
    $TabelMonitor .= '</tr>';
    $TabelMonitor .= '</thead>';
    $TabelMonitor .= '<tbody>';
    $TabelMonitor .= '</tbody>';
    $TabelMonitor .= '<tfoot>';
    $TabelMonitor .= '<tr>';
    $TabelMonitor .= '</tr>';
    $TabelMonitor .= '</tfoot>';
    $TabelMonitor .= '</table>';
    return $TabelMonitor;
}