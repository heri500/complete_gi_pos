<?php

function petty_cash_input(){
    global $user;
    set_default_time_zone();
    $Today = mktime(0,0,0, date('n'), date('j'), date('Y'));
    $IpAddress = ip_address();
    $PettyCashExists = db_result(
        db_query(
            "SELECT COUNT(*) FROM user_petty_cash WHERE uid = %d AND ip = '%s' AND login >= %d AND log_out IS NULL",
                $user->uid, $IpAddress, $Today
        )
    );
    if (empty($PettyCashExists)) {
        _addCustomCSS();
        _addJqueryPlugins();
        drupal_add_css('misc/media/themes/jquery-ui-1.8.11.custom.css');
        drupal_add_js('misc/media/jquery-ui-1.8.11.custom.min.js');
        $js_path = drupal_get_path('module', 'penjualan') . '/js/inputpettycash.js';
        drupal_add_js($js_path);
        $formPettyCash = drupal_get_form('petty_cash_input_form');
        return $formPettyCash;
    }else{
        drupal_goto('penjualan/kasir');
    }
    return false;
}

function petty_cash_input_form(&$form_state){
    $form['#attributes'] = array('class' => 'form-item-lined-up');
    $form['petty_cash'] = array(
        '#title' => t('Petty Cash'),
        '#type' => 'textfield',
        '#description' => t('Key in petty cash'),
        '#size' => 10,
        '#default_value' => 200,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Simpan')
    );
    return $form;
}

function petty_cash_input_form_validate($form, &$form_state) {
    if (!is_numeric($form_state['values']['petty_cash'])) {
        form_set_error('', t('Wajib key in jumlah(numeric value)...!!?'));
    }
}

function petty_cash_input_form_submit($form, &$form_state)
{
    global $user;
    $formValues = $form_state['values'];
    db_query("INSERT INTO user_petty_cash (uid, login, petty_cash, ip) VALUES (%d, %d, '%f','%s')",
        $user->uid, $user->login, $formValues['petty_cash'], ip_address());
    drupal_set_message(t('Petty cash berhasil disimpan...!!'));
    delete_all_temp_penjualan();
    drupal_goto('penjualan/kasir');
}
