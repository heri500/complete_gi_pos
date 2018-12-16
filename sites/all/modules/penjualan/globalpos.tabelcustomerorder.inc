<?php

function view_tabel_customerorder(){
    update_opened_customer_order_table(1);
	get_number_format($currencySym, $thousandSep, $decimalSep);
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
	if (isset($_GET['message'])){
		$strStatus = 'status';
		if (isset($_GET['status'])){
			$strStatus = $_GET['status'];
		}
		drupal_set_message($_GET['message'], $strStatus);
	}
	if ($_POST["tgl1"] AND $_POST["tgl2"]){
		$tgl1 = $_POST["tgl1"];
        $splitTgl = explode('-', $tgl1);
        $tgl1 = $splitTgl[2].'-'.$splitTgl[1].'-'.$splitTgl[0];
		$tgl2 = $_POST["tgl2"];
        $splitTgl = explode('-', $tgl2);
        $tgl2 = $splitTgl[2].'-'.$splitTgl[1].'-'.$splitTgl[0];
	}else{
		$tgl1 = date("Y-m-d");
		$tgl2 = date("Y-m-d");
	}
	if ($_POST["pilihantampil"] == 'NOTA'){
		$nota = 'selected="selected"';
		$tabeltampil = tabel_customerorder($tgl1,$tgl2);
		$urutan = 1;
	}elseif ($_POST["pilihantampil"] == 'PRODUK'){
		$produk = 'selected="selected"';
		$tabeltampil = customerorder_produk($tgl1,$tgl2);
		$urutan = 2;
	}else{
		$tabeltampil = tabel_customerorder($tgl1,$tgl2);
		$nota = 'selected="selected"';
		$urutan = 1;
	}
    drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
    drupal_add_css('misc/media/datatables.1.10/media/css/dataTables.jqueryui.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.dataTables.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.jqueryui.css');
	//_addJqueryPlugins();
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
	//drupal_add_js('misc/media/jquery-ui-1.8.custom.min.js');
	//_addTimePickerPlugins();
	_addJeditablePlugins();
	_addAutotabPlugins();
	_addBarcodePlugins();
	drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.dataTables.js');
    drupal_add_js('misc/media/datatables.1.10/media/js/dataTables.jqueryui.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/dataTables.buttons.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.flash.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.html5.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.print.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.colVis.min.js');
	drupal_add_js(
        array(
            'urutan'  => $urutan,
            'tglawal' => $tgl1,
            'tglakhir'=> $tgl2,
        ),
        'setting'
    );
	$js_path = drupal_get_path('module','penjualan').'/js/tabel.customerorder.js';
	drupal_add_js($js_path);
	$pilihantampil = '<select id="pilihantampil" name="pilihantampil">';
	$pilihantampil .= '<option '.$nota.'>NOTA</option>';
	$pilihantampil .= '<option '.$produk.'>PRODUK</option>';
	$pilihantampil .= '</select>';
	$pilihperiode = '<form action="'.base_path().'penjualan/viewcustomerorder" method="post">';
    $pilihperiode .= '<div id="pilihanperiode"><label>PILIH PERIODE</label>';
    $pilihperiode .= '<input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.date('d-m-Y',strtotime($tgl1)).'">';
	$pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.date('d-m-Y', strtotime($tgl2)).'">';
    $pilihperiode .= $pilihantampil.'<button>LIHAT ORDER</button>';
    $pilihperiode .= '<span style="float: right;margin-top: 6px;"><b>ANDROID MONITOR : ';
    $pilihperiode .= '<img style="vertical-align:middle" id="status-android" src="'.base_path().'misc/media/images/on-icon.png">';
    $pilihperiode .= '&nbsp;Total Table Open : <span id="total-open">0</span>';
    $pilihperiode .= '&nbsp;&nbsp;RESET <img onclick="reset_android()" style="cursor: pointer;vertical-align:middle" src="'.base_path().'misc/media/images/reset-icon.png"></b>';
    $pilihperiode .= '</span>';
    $pilihperiode .= '</div>';
    $pilihperiode .= '</form>';
    $viewcustomerorder = $pilihperiode.'<div id="viewcustomerorder">'.$tabeltampil.'</div>';
	$viewcustomerorder .= '<div id="dialogdetail" title="DETAIL CUSTOMER ORDER">';
	$viewcustomerorder .= '<div id="table-detail-wrapper"></div>';
	$viewcustomerorder .= '<div id="add-new-item">';
	$viewcustomerorder .= '<h5 class="no-margin">ADD NEW ORDER</h5>';
	$inputProduct = '<input type="text" id="new-product" name="new-product" size="35">';
	$inputProduct .= '<input type="hidden" id="idproduct" name="idproduct">';
	$viewcustomerorder .= '<label>Product</label>'.$inputProduct;
	$inputHarga = '<input type="text" id="harga-view" name="harga-view" size="10" readonly="readonly">';
	$inputHarga .= '<input type="hidden" id="hargajual" name="hargajual">';
	$inputHarga .= '<input type="hidden" id="hargapokok" name="hargapokok">';
	$inputHarga .= '<input type="hidden" id="diskon" name="diskon">';
	$inputSubtotal = '<input type="text" id="subtotal-view" name="subtotal-view" size="14" readonly="readonly">';
	$inputQty = '<input type="text" id="qty-new" name="qty-new" size="6">';
	$viewcustomerorder .= $inputHarga.'<label class="small-label">Qty</label>'.$inputQty.$inputSubtotal;
	$buttonAddNew = '<button id="add-new-button">Add Order</button>';
	$buttonPrintSlip = '<button id="print-slip">Print Slip</button>';
	$viewcustomerorder .= $buttonAddNew.'</div>'.$buttonPrintSlip;
	$viewcustomerorder .= '</div>';

	return $viewcustomerorder;
}

function tabel_customerorder($tgl1,$tgl2){
	$tabelcustomerorder ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_customerorder">';
	$tabelcustomerorder .= '<thead>';
	$tabelcustomerorder .= '<tr>';
	$tabelcustomerorder .= '<th class="center tablebutton">&nbsp;</th>';
	$tabelcustomerorder .= '<th class="center tablebutton">&nbsp;</th>';
	$tabelcustomerorder .= '<th class="center tablebutton">&nbsp;</th>';
	$tabelcustomerorder .= '<th class="center tablebutton">&nbsp;</th>';
	$tabelcustomerorder .= '<th>MEJA</th>';
	$tabelcustomerorder .= '<th>BARCODE</th>';
	$tabelcustomerorder .= '<th>NO. NOTA</th>';
	$tabelcustomerorder .= '<th class="center">HARI</th>';
	$tabelcustomerorder .= '<th class="tanggal">TGL</th>';
	$tabelcustomerorder .= '<th class="tanggal">JAM</th>';
	$tabelcustomerorder .= '<th class="col_angka">TOTAL</th>';
    $tabelcustomerorder .= '<th class="col_angka">PPN</th>';
    $tabelcustomerorder .= '<th class="col_angka">TOTAL+PPN</th>';
	$tabelcustomerorder .= '<th class="col_angka">CARA BAYAR</th>';
	$tabelcustomerorder .= '<th class="col_angka">BAYAR</th>';
	$tabelcustomerorder .= '<th class="col_angka">SISA</th>';
	$tabelcustomerorder .= '<th>KASIR</th>';
	$tabelcustomerorder .= '<th>PELANGGAN</th>';
	$tabelcustomerorder .= '<th>STATUS</th>';
	$tabelcustomerorder .= '<th>PERKIRAAN</th>';
	$tabelcustomerorder .= '<th>KETERANGAN</th>';
    $tabelcustomerorder .= '<th class="center tablebutton">&nbsp;</th>';
	$tabelcustomerorder .= '</tr>';
	$tabelcustomerorder .= '</thead>';
	$tabelcustomerorder .= '<tbody>';
	$tabelcustomerorder .= '</tbody>';
	$tabelcustomerorder .= '<tfoot>';
	$tabelcustomerorder .= '<tr>';
	if (cekUserRoles()){
		$tabelcustomerorder .= '<th style="text-align:right" colspan="10">Total:&nbsp;&nbsp;</th>';
	}else{
		$tabelcustomerorder .= '<th style="text-align:right" colspan="9">Total:&nbsp;&nbsp;</th>';
	}
    /*$totaljual = getTotalCustomerOrder('total',$tgl1, $tgl2);
    $totalbayar = getTotalCustomerOrder('bayar',$tgl1, $tgl2);
    $totalSisa = getTotalCustomerOrder('total-bayar',$tgl1, $tgl2);*/
	$tabelcustomerorder .= '<th style="text-align: right">&nbsp;</th>';
    $tabelcustomerorder .= '<th style="text-align: right">&nbsp;</th>';
    $tabelcustomerorder .= '<th style="text-align: right">&nbsp;</th>';
	$tabelcustomerorder .= '<th>&nbsp;</th>';
	$tabelcustomerorder .= '<th style="text-align: right">&nbsp;</th>';
	$tabelcustomerorder .= '<th style="text-align: right">&nbsp;</th>';
	$tabelcustomerorder .= '<th>&nbsp;</th>';
	$tabelcustomerorder .= '<th>&nbsp;</th>';
	$tabelcustomerorder .= '<th>&nbsp;</th>';
	$tabelcustomerorder .= '<th>&nbsp;</th>';
	$tabelcustomerorder .= '<th>&nbsp;</th>';
	$tabelcustomerorder .= '<th>&nbsp;</th>';
	$tabelcustomerorder .= '</tr>';
	$tabelcustomerorder .= '</tfoot>';
	$tabelcustomerorder .= '</table>';
	return $tabelcustomerorder;
}

function detail_customerorder(){
	if ($_POST["idcustomerorder"]){
		$tabelcustomerorder ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_customerorder">';
		$tabelcustomerorder .= '<thead>';
		$tabelcustomerorder .= '<tr>';
		$tabelcustomerorder .= '<th class="center tablebutton">&nbsp;</th>';
		$tabelcustomerorder .= '<th>BARCODE</th>';
		$tabelcustomerorder .= '<th>PRODUK</th>';
		$tabelcustomerorder .= '<th>QTY</th>';
		$tabelcustomerorder .= '<th>DIAMBIL</th>';
		$tabelcustomerorder .= '<th>SISA</th>';
		$tabelcustomerorder .= '<th>H.JUAL</th>';
		$tabelcustomerorder .= '<th>SUBTOTAL</th>';
		$tabelcustomerorder .= '<th class="qty">PERKIRAAN</th>';
		$tabelcustomerorder .= '<th class="qty">TGL AMBIL</th>';
		$tabelcustomerorder .= '</tr>';
		$tabelcustomerorder .= '</thead>';
		$tabelcustomerorder .= '<tbody>';
		$tabelcustomerorder .= '</tbody>';
		$tabelcustomerorder .= '<tfoot>';
		$tabelcustomerorder .= '<tr>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '<th>&nbsp;</th>';
		$tabelcustomerorder .= '</tr>';
		$tabelcustomerorder .= '</tfoot>';
		$tabelcustomerorder .= '</table>';
		print $tabelcustomerorder;
	}
	exit();
}
function customerorder_produk($tgl1,$tgl2){
	$tgl1 = date('Y-m-d', strtotime($tgl1)).' 00:00';
	$tgl2 = date('Y-m-d', strtotime($tgl2)).' 23:59';
	$tabelcustomerorder ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_customerorder">';
	$tabelcustomerorder .= '<thead>';
	$tabelcustomerorder .= '<tr>';
	$tabelcustomerorder .= '<th>BARCODE</th>';
	$tabelcustomerorder .= '<th>PRODUCT</th>';
	$tabelcustomerorder .= '<th class="qty">QTY</th>';
	$tabelcustomerorder .= '<th>H.JUAL</th>';
	$tabelcustomerorder .= '<th>SUBTOTAL</th>';
	$tabelcustomerorder .= '</tr>';
	$tabelcustomerorder .= '</thead>';
	$tabelcustomerorder .= '<tbody>';
	$result = db_query("SELECT d.namasupplier, a.tglorder, c.barcode, c.namaproduct, SUM(b.jumlah) AS totalqty, c.hargajual,
	SUM(b.hargajual*b.jumlah) AS subtotal FROM detailcustomerorder b LEFT JOIN customer_order a ON a.id=b.idcustomerorder
	LEFT JOIN product c ON b.idproduct=c.idproduct LEFT JOIN supplier d ON c.idsupplier=d.idsupplier WHERE  a.tglorder BETWEEN '%s' AND '%s'
	GROUP BY b.idproduct",$tgl1,$tgl2);
	$totaljual = 0;
	$totalmodal = 0;
	$totallaba = 0;
	while($data = db_fetch_object($result)){
		$tabelcustomerorder .= '<tr>';
		$tabelcustomerorder .= '<td>'.$data->barcode.'</td>';
		$tabelcustomerorder .= '<td>'.$data->namaproduct.'</td>';
		$tabelcustomerorder .= '<td class="angka">'.$data->totalqty.'</td>';
		$tabelcustomerorder .= '<td class="angka">'.$data->hargajual.'</td>';
		$tabelcustomerorder .= '<td class="angka">'.$data->subtotal.'</td>';
		$tabelcustomerorder .= '</tr>';
		$totaljual = $totaljual + $data->subtotal;
	}
	$tabelcustomerorder .= '</tbody>';
	$tabelcustomerorder .= '<tfoot>';
	$tabelcustomerorder .= '<tr>';
	$tabelcustomerorder .= '<th style="text-align:right" colspan="4">Total:&nbsp;&nbsp;</th>';
	$tabelcustomerorder .= '<th style="text-align: right">Rp. '.number_format($totaljual,0,",",".").'&nbsp;&nbsp;</th>';
	$tabelcustomerorder .= '</tr>';
	$tabelcustomerorder .= '</tfoot>';
	$tabelcustomerorder .= '</table>';
	return $tabelcustomerorder;
}