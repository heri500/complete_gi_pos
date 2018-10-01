<?php

function data_supplier(){
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
    drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
    drupal_add_css('misc/media/datatables.1.10/media/css/dataTables.jqueryui.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.dataTables.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.jqueryui.css');
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
    drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine-en.js');
    drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine.js');
    drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
    _addJeditablePlugins();
    _addTimePickerPlugins();
    _addAutotabPlugins();
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.dataTables.js');
    drupal_add_js('misc/media/datatables.1.10/media/js/dataTables.jqueryui.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/dataTables.buttons.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.flash.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.html5.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.print.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.colVis.min.js');
	drupal_add_js(
		array(
			'filePath' => file_directory_path(),
            'gstSym' => $gstSym,
            'dDigit' => $DecDigit,
		),
		'setting');
    drupal_add_js('misc/media/datatables/js/number.format.js');
	$modulePath = drupal_get_path('module','datasupplier');
	$jsPath = $modulePath.'/js/supplier.js';
	drupal_add_js($jsPath);
	$viewtabelsupplier = '<form id="formsupplier" action="'.base_path().'datasupplier/simpansupplier" method="post"><div id="formplace" class="supplier">';
	$viewtabelsupplier .= '<label>KODE</label><input type="text" class=\"validate[required]\" id="kodesupplier" name="kodesupplier">';
	$viewtabelsupplier .= '<label>SUPPLIER</label><input class=\"validate[required]\" type="text" id="namasupplier" name="namasupplier">';
	$viewtabelsupplier .= '<label>TELEPON</label><input type="text" id="telp" name="telp">';
	$viewtabelsupplier .= '<label>ALAMAT</label><input type="text" id="alamat" name="alamat">';
	$viewtabelsupplier .= '<label>EMAIL</label><input type="text" id="email" name="email">';
	$viewtabelsupplier .= '<button style="font-size: 12px;">Tambah Supplier</button></div></form>';
    $viewtabelsupplier .= '<div class="button-sync"><button id="sync-hutang-supplier" style="font-size: 12px;">Sync Hutang Supplier</button></div>';
	$viewtabelsupplier .= '<div style="width: 80%;">'.tabel_supplier().'</div>';
	$viewtabelsupplier .= '<div id="dialogdetailhutang" title="DETAIL HUTANG DAN PEMBAYARAN"><div id="tempatnilaihutang"></div><div id="detailpiutang"></div><div id="detailpembayaran">a</div></div>';
	$viewtabelsupplier .= '<div id="dialogdetail" title="DETAIL PEMBELIAN"></div>';
	$viewtabelsupplier .= '<div id="dialogpembayaran" title="PEMBAYARAN HUTANG">';
	$viewtabelsupplier .= '<div id="totalhutangpelanggan"></div>';
	$viewtabelsupplier .= '<div class="baris"><label>Tgl Bayar</label><input readonly="readonly" type="text" id="tglbayar" name="tglbayar" value="'.date("d-m-Y").'"><input type="hidden" id="tglbayarkirim" name="tglbayarkirim" value="'.date("Y-m-d").'"></div>';
	$viewtabelsupplier .= '<div class="baris"><label>Pembayaran</label><input type="text" id="nilaipembayaran" name="nilaipembayaran"></div>';
	$viewtabelsupplier .= '<div class="baris"><button id="bayarhutang" onclick="do_pembayaran();">BAYAR</button>';
	$viewtabelsupplier .= '<input type="hidden" id="idsupplierbayar" name="idsupplierbayar">';
	$viewtabelsupplier .= '<input type="hidden" id="tothutang" name="tothutang"></div>';
	$viewtabelsupplier .= '</div>';
	return $viewtabelsupplier;
}

function tabel_supplier(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
	$tabelsupplier ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_supplier">';
	$tabelsupplier .= '<thead>';
	$tabelsupplier .= '<tr>';
	$tabelsupplier .= '<th class="tablebutton"></th>';
	$tabelsupplier .= '<th class="tablebutton"></th>';
	$tabelsupplier .= '<th style="width: 150px;">KODE</th>';
	$tabelsupplier .= '<th>NAMA SUPPLIER</th>';
	$tabelsupplier .= '<th>TELP</th>';
	$tabelsupplier .= '<th>ALAMAT</th>';
	$tabelsupplier .= '<th>EMAIL</th>';
	$tabelsupplier .= '<th>HUTANG</th>';
	$tabelsupplier .= '<th>AKHIR</th>';
	$tabelsupplier .= '<th class="tanggal">UPDATE</th>';
	$tabelsupplier .= '</tr>';
	$tabelsupplier .= '</thead>';
	$tabelsupplier .= '<tbody>';
	$totalhutang = 0;
	$result = db_query("SELECT idsupplier,kodesupplier,namasupplier,telp,alamat,email FROM supplier ORDER BY kodesupplier,namasupplier");
	while ($data = db_fetch_object($result)){
		$tabelsupplier .= '<tr id="'.$data->idsupplier.'">';
		$resulthutang = db_query("SELECT besarhutang,pembayaranterakhir ,SUBSTR(last_update,1,10) AS bayarterakhir 
		FROM hutang WHERE idsupplier='%d'",$data->idsupplier);
		$datahutang = db_fetch_object($resulthutang);
		$totalhutang = $totalhutang + $datahutang->besarhutang;
		$tampilhutang = number_format($datahutang->besarhutang,0,$decimalSep,$thousandSep);
		$lihathutang = '<img title="Klik untuk melihat detail hutang dan pembayaran ke Supplier '.$data->namasupplier.'" src="'.base_path().'misc/media/images/forward_enabled.ico" onclick="view_detail_hutang(\''.$data->idsupplier.'\',\''.$data->namasupplier.'\',\''.$tampilhutang.'\')">';
		$bayarhutang = '<img title="Klik untuk mengisi form pembayaran hutang" src="'.base_path().'misc/media/images/money2_24.png" onclick="pembayaran(\''.$data->idsupplier.'\',\''.$data->namasupplier.'\',\''.number_format($datahutang->besarhutang,0,",",".").'\',\''.$datahutang->besarhutang.'\')">';
		$tabelsupplier .= '<td class="center">'.$lihathutang.'</td>';
		$tabelsupplier .= '<td class="center">'.$bayarhutang.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->kodesupplier.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->namasupplier.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->telp.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->alamat.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->email.'</td>';
		$tabelsupplier .= '<td class="angka">'.number_format($datahutang->besarhutang,0,$decimalSep,$thousandSep).'</td>';
		$tabelsupplier .= '<td class="angka">'.number_format($datahutang->pembayaranterakhir,0,$decimalSep,$thousandSep).'</td>';
		$tabelsupplier .= '<td class="center">'.$datahutang->bayarterakhir.'</td>';
		$tabelsupplier .= '</tr>';
	}
	$tabelsupplier .= '</tbody>';
	$tabelsupplier .= '<tfoot>';
	$tabelsupplier .= '<tr>';
	$tabelsupplier .= '<th style="text-align:right" colspan="7">Total:&nbsp;&nbsp;</th>';
	$tabelsupplier .= '<th style="text-align: right" colspan="3">'.$currencySym.' '.number_format($totalhutang,0,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
	$tabelsupplier .= '</tr>';
	$tabelsupplier .= '</tfoot>';
	$tabelsupplier .= '</table>';
	return $tabelsupplier;
}

function simpan_supplier(){
	if ($_POST["kodesupplier"] AND $_POST["namasupplier"]){
		db_query("INSERT INTO supplier(kodesupplier,namasupplier,telp,alamat,email) VALUES('%s','%s','%s','%s','%s')",
		strtoupper($_POST["kodesupplier"]),$_POST["namasupplier"],$_POST["telp"],$_POST["alamat"],$_POST["email"]);
		if ($_POST["asal"]){
			$result = db_query("SELECT idsupplier FROM supplier WHERE kodesupplier='%s' AND namasupplier='%s'",
			strtoupper($_POST["kodesupplier"]),$_POST["namasupplier"]);
			$data = db_fetch_object($result);
			print $data->idsupplier;
		}else{
			drupal_goto("datasupplier/supplier");
		}
	}
	exit();	
}

function update_supplier(){
	$NILAI = $_POST['value'];
	$KOLOM = $_POST['kol_id'];
	$SUPPLIER_ID = $_POST['row_id'];
	if ($KOLOM == "2"){
		$NILAI = strtoupper($NILAI);
		$sql_update = "UPDATE supplier SET kodesupplier='%s' WHERE idsupplier='%d'";
	}elseif ($KOLOM == "3"){
		$sql_update = "UPDATE supplier SET namasupplier='%s' WHERE idsupplier='%d'";
	}elseif ($KOLOM == "4"){
		$sql_update = "UPDATE supplier SET telp='%s' WHERE idsupplier='%d'";
	}elseif ($KOLOM == "5"){
		$sql_update = "UPDATE supplier SET alamat='%s' WHERE idsupplier='%d'";
	}elseif ($KOLOM == "6"){
		$sql_update = "UPDATE supplier SET email='%s' WHERE idsupplier='%d'";
	}
	db_query($sql_update,$NILAI,$SUPPLIER_ID);
	echo $NILAI;
	exit();	
}
function bayar_hutang_supplier(){
	//idpelanggan, nilaisebelumbayar, pembayaran, uid, tglbayar
	if ($_POST["idsupplier"] AND $_POST["pembayaran"]){
		global $user;
		db_query("INSERT INTO pembayaranhutang (idsupplier, hutangsebelumbayar, pembayaran, uid, tglbayar) VALUES 
		('%d', '%f', '%f', '%d', '%s')",$_POST["idsupplier"],$_POST["hutang"],$_POST["pembayaran"],$user->uid,$_POST["tglbayar"]);
		$sisahutang = $_POST["hutang"] - $_POST["pembayaran"];
		$tglsekarang = date("Y-m-d H:i:s");
		db_query("UPDATE hutang SET besarhutang='%f',pembayaranterakhir='%f',last_update='%s' WHERE idsupplier='%d'",
		$sisahutang,$_POST["pembayaran"],$tglsekarang,$_POST["idsupplier"]);
	}
	exit();	
}
function detail_hutang(){
	if ($_POST["idsupplier"]){
		get_number_format($currencySym, $thousandSep, $decimalSep, false);
		$tabelhutang ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_hutang">';
		$tabelhutang .= '<thead>';
		$tabelhutang .= '<tr>';
		$tabelhutang .= '<th class="tablebutton"></th>';
		$tabelhutang .= '<th style="width: 150px;">NO. NOTA</th>';
		$tabelhutang .= '<th>TOTAL</th>';
		$tabelhutang .= '<th>DP</th>';
		$tabelhutang .= '<th>HUTANG</th>';
		$tabelhutang .= '<th class="tanggal">TGL JUAL</th>';
		$tabelhutang .= '</tr>';
		$tabelhutang .= '</thead>';
		$tabelhutang .= '<tbody>';
		$result = db_query("SELECT a.idsupplier, a.idpembelian,a.besarhutang,b.nonota,b.total,b.bayar,SUBSTR(b.tglpembelian,1,10) AS tglbeli FROM 
		detailhutang a, pembelian b WHERE a.idpembelian = b.idpembelian AND a.idsupplier='%d'",
		$_POST["idsupplier"]);
		$totalbelanja = 0;
		$totaldp = 0;
		$totalhutang = 0;
		while ($data = db_fetch_object($result)){
			$lihatdetailpembelian = '<img title="Klik untuk melihat detail pembelian" onclick="view_detail(\''.$data->idpembelian.'\',\''.$data->nonota.'\');" src="'.base_path().'misc/media/images/forward_enabled.ico">';
			$tabelhutang .= '<tr id="'.$data->idpembelian.'">';
			$tabelhutang .= '<td class="center">'.$lihatdetailpembelian.'</td>';
			$tabelhutang .= '<td class="center">'.$data->nonota.'</td>';
			$totalbelanja = $totalbelanja + $data->total;
			$totaldp = $totaldp + $data->bayar;
			$totalhutang = $totalhutang + $data->besarhutang;
			$tabelhutang .= '<td class="angka">'.number_format($data->total,0,$decimalSep,$thousandSep).'</td>';
			$tabelhutang .= '<td class="angka">'.number_format($data->bayar,0,$decimalSep,$thousandSep).'</td>';
			$tabelhutang .= '<td class="angka">'.number_format($data->besarhutang,0,$decimalSep,$thousandSep).'</td>';
			$tabelhutang .= '<td class="center">'.$data->tglbeli.'</td>';
			$tabelhutang .= '</tr>';
		}
		$tabelhutang .= '</tbody>';
		$tabelhutang .= '<tfoot>';
		$tabelhutang .= '<tr>';
		$tabelhutang .= '<th style="text-align:right" colspan="2">Total:&nbsp;&nbsp;</th>';
		$tabelhutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totalbelanja,0,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelhutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totaldp,0,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelhutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totalhutang,0,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelhutang .= '<th></th>';
		$tabelhutang .= '</tr>';
		$tabelhutang .= '</tfoot>';
		$tabelhutang .= '</table>';
		$tabelhutang .= '<br><br>';
		print $tabelhutang;
	}
	exit();	
}

function detail_pembayaran_hutang(){
	//idlangganan, pembayaran, last_update
	if ($_POST["idsupplier"]){
		get_number_format($currencySym, $thousandSep, $decimalSep, false);
		$tabelpembayaran ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="history_pembayaran">';
		$tabelpembayaran .= '<thead>';
		$tabelpembayaran .= '<tr>';
        $tabelpembayaran .= '<th class="tablebutton">&nbsp;</th>';
		$tabelpembayaran .= '<th class="tanggal">TGL BAYAR</th>';
		$tabelpembayaran .= '<th>KETERANGAN</th>';
		$tabelpembayaran .= '<th>NILAI</th>';
		$tabelpembayaran .= '</tr>';
		$tabelpembayaran .= '</thead>';
		$tabelpembayaran .= '<tbody>';
		$result = db_query("SELECT id, idsupplier, keterangan, pembayaran, SUBSTR(last_update,1,10) AS tglbayar FROM pembayaranhutang WHERE idsupplier='%d'",
		$_POST["idsupplier"]);
		$totalbayar = 0;
		while ($data = db_fetch_object($result)){
			$tabelpembayaran .= '<tr>';
            $TombolHapus = '<img title="Klik untuk menghapus pembayaran hutang" onclick="delete_pembayaran('.$data->id.','.$data->idsupplier.');" src="'.base_path().'misc/media/images/del.ico" width="22">';
            $tabelpembayaran .= '<td class="center">'.$TombolHapus.'</td>';
			$tabelpembayaran .= '<td class="center">'.$data->tglbayar.'</td>';
			$tabelpembayaran .= '<td>'.$data->keterangan.'</td>';
			$totalbayar = $totalbayar + $data->pembayaran;
			$tabelpembayaran .= '<td class="angka">'.number_format($data->pembayaran,0,$decimalSep,$thousandSep).'</td>';
			$tabelpembayaran .= '</tr>';
		}
		$tabelpembayaran .= '</tbody>';
		$tabelpembayaran .= '<tfoot>';
		$tabelpembayaran .= '<tr>';
		$tabelpembayaran .= '<th style="text-align:right">Total:&nbsp;&nbsp;</th>';
		$tabelpembayaran .= '<th colspan="3" style="text-align: right" >'.$currencySym.' '.number_format($totalbayar,0,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelpembayaran .= '</tr>';
		$tabelpembayaran .= '</tfoot>';
		$tabelpembayaran .= '</table>';
		$tabelpembayaran .= '<br><br>';
		print $tabelpembayaran;
	}
	exit();	
}
function delete_pembayaran_supplier(){
    if (isset($_GET['idsupplier']) && !empty($_GET['idsupplier']) &&
        isset($_GET['idpembayaran']) && !empty($_GET['idpembayaran'])){
        $IdSupplier = $_GET['idsupplier'];
        $IdPembayaran = $_GET['idpembayaran'];
        db_query('DELETE FROM pembayaranhutang WHERE id=%d', $IdPembayaran);
        syncHutangSupplier($IdSupplier);
    }
    drupal_goto('datasupplier/supplier');
    return true;
}