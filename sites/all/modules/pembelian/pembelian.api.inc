<?php

function get_supplier_related_nota($IdSupplier = null, $nota_status = array(0,1)){
    $RelatedInvoice = null;
    if (!empty($IdSupplier)) {
        $dataPremis = get_data_premis(false);
        $DecDigit = $dataPremis->decimal_digit;
        if (is_array($nota_status)) {
            $StrSql = 'SELECT idpembelian, nonota, no_invoice, total, payment_paid,tglpembelian FROM pembelian WHERE ';
            $StrSql .= 'payment_status IN (' . db_placeholders($nota_status, 'int') . ') ';
            $StrSql .= 'AND idsupplier = %d ORDER BY idpembelian';
            $result = db_query($StrSql, array_merge($nota_status, array($IdSupplier)));
            $RelatedInvoice = array();
            while ($data = db_fetch_object($result)) {
                $data->total = round($data->total, $DecDigit);
                $RelatedInvoice[] = $data;
            }
        }else{
            $result = db_query('SELECT idpembelian, nonota, no_invoice, total, payment_paid,tglpembelian FROM pembelian WHERE 
            payment_status = %d AND idsupplier = %d ORDER BY idpembelian', $nota_status, $IdSupplier);
            $RelatedInvoice = array();
            while ($data = db_fetch_object($result)) {
                $data->total = round($data->total, $DecDigit);
                $RelatedInvoice[] = $data;
            }
        }
    }
    return $RelatedInvoice;
}

function get_supplier_related_nota_rest(){
    if (isset($_POST['idsupplier']) && !empty($_POST['idsupplier'])){
        $IdSupplier = $_POST['idsupplier'];
        if (isset($_POST['payment_status']) && !empty($_POST['payment_status'])){
            $PaymentStatus = $_POST['payment_status'];
        }else{
            $PaymentStatus = array(0,1);
        }
        $DataNota = get_supplier_related_nota($IdSupplier, $PaymentStatus);
        print json_encode($DataNota);
    }
    exit();
}