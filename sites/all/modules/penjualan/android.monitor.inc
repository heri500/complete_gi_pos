<?php

function android_monitor_page(){
    db_query('UPDATE cust_view_open SET table_open = 1');
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    $variables['styles'] = drupal_get_css();
    drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
    drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
    $modulePath = drupal_get_path('module','penjualan');
    $jsPath = $modulePath.'/js/android.monitor.js';
    drupal_add_js($jsPath);
    $AndroidMonitor = '<div><span style="margin-top: 6px;"><b>ANDROID MONITOR : ';
    $AndroidMonitor .= '<img style="vertical-align:middle" id="status-android" src="'.base_path().'misc/media/images/on-icon.png">';
    $AndroidMonitor .= '&nbsp;Total Table Open : <span id="total-open">0</span>';
    $AndroidMonitor .= '&nbsp;&nbsp;RESET <img onclick="reset_android()" style="cursor: pointer;vertical-align:middle" src="'.base_path().'misc/media/images/reset-icon.png"></b>';
    $AndroidMonitor .= '</span></div>';
    return $AndroidMonitor;
}