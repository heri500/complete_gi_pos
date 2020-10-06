<?php

function data_supplier(){
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
    $dataPremis = get_data_premis();
    $gstSym = get_gst_symbols(false);
    $DecDigit = $dataPremis->decimal_digit;
    drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
    drupal_add_css('misc/media/datatables.1.10/media/css/dataTables.jqueryui.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.dataTables.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.jqueryui.css');
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.js');
    drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine-en.js');
    drupal_add_js('misc/media/jqueryValidate/js/jquery.validationEngine.js');
    drupal_add_js('misc/media/datatables.1.10/jquery/jquery-ui.js');
    drupal_add_js('misc/media/datatables.1.10/media/js/jquery.dataTables.js');
    drupal_add_js('misc/media/datatables.1.10/media/js/dataTables.jqueryui.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/dataTables.buttons.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.flash.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.html5.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.print.js');
    drupal_add_js('misc/media/datatables.1.10/extensions/Buttons/js/buttons.colVis.min.js');
    drupal_add_js('misc/media/datatables/js/number.format.js');
    _addChosenJqueryPlugins();
    _addJeditablePlugins();
    _addTimePickerPlugins();
    _addAutotabPlugins();
    $PaymentType = array_payment_type();
    drupal_add_js(
        array(
            'filepath' => file_directory_path(),
            'status_options' => array('External','Internal'),
            'payment_type' => $PaymentType,
            'dec_digit' => $DecDigit,
            'gstSym' => $gstSym,
            'dDigit' => $DecDigit,
        ),
        'setting');
	$modulePath = drupal_get_path('module','datasupplier');
	$jsPath = $modulePath.'/js/supplier.js';
	drupal_add_js($jsPath);
	$viewtabelsupplier = '<form id="formsupplier" action="'.base_path().'datasupplier/simpansupplier" method="post"><div id="formplace" class="supplier">';
	$viewtabelsupplier .= '<label>'.strtoupper(trans('kode', $dataPremis->bahasa)).'</label><input type="text" class=\"validate[required]\" id="kodesupplier" name="kodesupplier">';
	$viewtabelsupplier .= '<label>SUPPLIER</label><input class=\"validate[required]\" type="text" id="namasupplier" name="namasupplier">';
	$viewtabelsupplier .= '<label>TELEPON</label><input type="text" id="telp" name="telp">';
	$viewtabelsupplier .= '<label>ALAMAT</label><input type="text" id="alamat" name="alamat">';
	$viewtabelsupplier .= '<label>EMAIL</label><input type="text" id="email" name="email">';
    $StatusSupplier = '<select id="status_pelanggan" name="status_pelanggan">';
    $StatusSupplier .= '<option value="0">Eksternal</option>';
    $StatusSupplier .= '<option value="1">Internal</option>';
    $StatusSupplier .= '</select>';
    $viewtabelsupplier .= '<label>STATUS</label>'.$StatusSupplier;
    $PaymentTypeOpt = '<select id="payment_type" name="payment_type">';
    for ($i = 0; $i < count($PaymentType);$i++) {
        $PaymentTypeOpt .= '<option value="'.$i.'">'.$PaymentType[$i].'</option>';
    }
    $PaymentTypeOpt .= '</select>';
    $viewtabelsupplier .= '<label>PAYMENT</label>'.$PaymentTypeOpt;
    $viewtabelsupplier .= '<label>TERM</label><input type="text" id="payment_term" name="payment_term">';
    $viewtabelsupplier .= '<label>%KONSIMEN</label><input type="text" id="konsimen_margin" name="konsimen_margin">';
	$viewtabelsupplier .= '<div class="button-save"><button style="font-size: 12px;">Tambah Supplier</button></div></div></form>';
    $viewtabelsupplier .= '<div class="button-sync"><button id="sync-hutang-supplier" style="font-size: 12px;">Sync Hutang Supplier</button></div>';
	$viewtabelsupplier .= '<div>'.tabel_supplier().'</div>';
	$viewtabelsupplier .= '<div id="dialogdetailhutang" title="DETAIL HUTANG DAN PEMBAYARAN"><div id="tempatnilaihutang"></div><div id="detailpiutang"></div><div id="detailpembayaran">a</div></div>';
	$viewtabelsupplier .= '<div id="dialogdetail" title="DETAIL PEMBELIAN"></div>';
	$viewtabelsupplier .= '<div id="dialogpembayaran" title="PEMBAYARAN HUTANG">';
	$viewtabelsupplier .= '<div id="totalhutangpelanggan"></div>';
	$viewtabelsupplier .= '<div class="baris"><label>'.ucfirst(trans('tanggal', $dataPremis->bahasa)).' Bayar</label><input readonly="readonly" type="text" id="tglbayar" name="tglbayar" value="'.date("d-m-Y").'"><input type="hidden" id="tglbayarkirim" name="tglbayarkirim" value="'.date("Y-m-d").'"></div>';
	$viewtabelsupplier .= '<div class="baris"><label>Pembayaran</label><input type="text" id="nilaipembayaran" name="nilaipembayaran"></div>';
    $viewtabelsupplier .= '<div class="baris">';
    $viewtabelsupplier .= '<label>'.ucfirst(trans('diskon', $dataPremis->bahasa)).'(in %)</label>';
    $viewtabelsupplier .= '<input type="text" id="discount_persen" name="discount_persen" style="width: 30px;margin-right: 6px;" value="0">';
    $viewtabelsupplier .= '<input type="text" id="discount_value" name="discount_value" readonly="readonly" style="width: 150px;margin-right: 6px;">';
    $viewtabelsupplier .= '<input type="text" id="value_after_discount" name="value_after_discount" readonly="readonly" style="width: 150px;">';
    $viewtabelsupplier .= '</div>';
    $viewtabelsupplier .= '<div class="baris"><label>Keterangan</label><textarea id="keterangan" name="keterangan"></textarea></div>';
    $viewtabelsupplier .= '<div class="baris"><label>Related Invoice/Nota</label><select id="related_nota" name="related_nota" multiple="multiple"></select></div>';
	$viewtabelsupplier .= '<div class="baris"><button id="bayarhutang" onclick="do_pembayaran();">BAYAR</button>';
	$viewtabelsupplier .= '<input type="hidden" id="idsupplierbayar" name="idsupplierbayar">';
	$viewtabelsupplier .= '<input type="hidden" id="tothutang" name="tothutang"></div>';
	$viewtabelsupplier .= '</div>';
	return $viewtabelsupplier;
}

function tabel_supplier(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
	$tabelsupplier ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_supplier">';
	$tabelsupplier .= '<thead>';
	$tabelsupplier .= '<tr>';
	$tabelsupplier .= '<th class="tablebutton"></th>';
	$tabelsupplier .= '<th class="tablebutton"></th>';
	$tabelsupplier .= '<th style="width: 150px;">'.strtoupper(trans('kode', $dataPremis->bahasa)).'</th>';
	$tabelsupplier .= '<th>NAMA SUPPLIER</th>';
	$tabelsupplier .= '<th>TELP</th>';
	$tabelsupplier .= '<th>ALAMAT</th>';
	$tabelsupplier .= '<th>EMAIL</th>';
	$tabelsupplier .= '<th>HUTANG</th>';
	$tabelsupplier .= '<th>AKHIR</th>';
	$tabelsupplier .= '<th class="tanggal">UPDATE</th>';
    $tabelsupplier .= '<th>STATUS</th>';
    $tabelsupplier .= '<th>PAYMENT</th>';
    $tabelsupplier .= '<th>TERM</th>';
    $tabelsupplier .= '<th>%KONSIMEN</th>';
	$tabelsupplier .= '</tr>';
	$tabelsupplier .= '</thead>';
	$tabelsupplier .= '<tbody>';
	$totalhutang = 0;
    $StatusSupplier = array('Eksternal','Internal');
    $PaymentType = array_payment_type();
	$result = db_query("SELECT idsupplier, kodesupplier, namasupplier, telp, alamat, email, 
    status_supplier, payment_type, payment_term, konsimen_margin 
    FROM supplier ORDER BY kodesupplier,namasupplier");
	while ($data = db_fetch_object($result)){
		$tabelsupplier .= '<tr id="'.$data->idsupplier.'">';
		$resulthutang = db_query("SELECT besarhutang,pembayaranterakhir ,SUBSTR(last_update,1,10) AS bayarterakhir 
		FROM hutang WHERE idsupplier='%d'",$data->idsupplier);
		$datahutang = db_fetch_object($resulthutang);
		$totalhutang = $totalhutang + $datahutang->besarhutang;
		$tampilhutang = number_format($datahutang->besarhutang,$DecDigit,$decimalSep,$thousandSep);
		$lihathutang = '<img title="Klik untuk melihat detail hutang dan pembayaran ke Supplier '.$data->namasupplier.'" src="'.base_path().'misc/media/images/forward_enabled.ico" onclick="view_detail_hutang(\''.$data->idsupplier.'\',\''.$data->namasupplier.'\',\''.$tampilhutang.'\')">';
		$bayarhutang = '<img title="Klik untuk mengisi form pembayaran hutang" src="'.base_path().'misc/media/images/money2_24.png" onclick="pembayaran(\''.$data->idsupplier.'\',\''.$data->namasupplier.'\',\''.number_format($datahutang->besarhutang,$DecDigit,",",".").'\',\''.$datahutang->besarhutang.'\')">';
		$tabelsupplier .= '<td class="center">'.$lihathutang.'</td>';
		$tabelsupplier .= '<td class="center">'.$bayarhutang.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->kodesupplier.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->namasupplier.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->telp.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->alamat.'</td>';
		$tabelsupplier .= '<td class="editable">'.$data->email.'</td>';
		$tabelsupplier .= '<td class="angka">'.number_format($datahutang->besarhutang,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelsupplier .= '<td class="angka">'.number_format($datahutang->pembayaranterakhir,$DecDigit,$decimalSep,$thousandSep).'</td>';
		$tabelsupplier .= '<td class="center">'.$datahutang->bayarterakhir.'</td>';
        $tabelsupplier .= '<td class="editable2">'.$StatusSupplier[$data->status_supplier].'</td>';
        $tabelsupplier .= '<td class="editable3">'.$PaymentType[$data->payment_type].'</td>';
        $tabelsupplier .= '<td class="editable">'.$data->payment_term.'</td>';
        $tabelsupplier .= '<td class="editable">'.$data->konsimen_margin.'</td>';
		$tabelsupplier .= '</tr>';
	}
	$tabelsupplier .= '</tbody>';
	$tabelsupplier .= '<tfoot>';
	$tabelsupplier .= '<tr>';
	$tabelsupplier .= '<th style="text-align:right" colspan="7">Total:&nbsp;&nbsp;</th>';
	$tabelsupplier .= '<th style="text-align: right">'.$currencySym.' '.number_format($totalhutang,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
    $tabelsupplier .= '<th colspan="6">&nbsp;</th>';
	$tabelsupplier .= '</tr>';
	$tabelsupplier .= '</tfoot>';
	$tabelsupplier .= '</table>';
	return $tabelsupplier;
}

function simpan_supplier(){
	if ($_POST["kodesupplier"] AND $_POST["namasupplier"]){
		db_query("INSERT INTO supplier(kodesupplier,namasupplier,telp,alamat,email, 
        status_supplier,payment_type, payment_term, konsimen_margin) VALUES ('%s','%s','%s','%s','%s','%d','%d','%d','%d')",
		strtoupper($_POST["kodesupplier"]),$_POST["namasupplier"],$_POST["telp"],$_POST["alamat"],
        $_POST["email"],$_POST["status_supplier"],$_POST["payment_type"],$_POST["payment_term"],$_POST["konsimen_margin"]);
		if ($_POST["asal"]){
			/*$result = db_query("SELECT idsupplier FROM supplier WHERE kodesupplier='%s' AND namasupplier='%s'",
			strtoupper($_POST["kodesupplier"]),$_POST["namasupplier"]);
			$data = db_fetch_object($result);*/
			$IdSupplier = db_last_insert_id('supplier','idsupplier');
			print $IdSupplier;
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
	}elseif ($KOLOM == "10"){
        $sql_update = "UPDATE supplier SET status_supplier='%d' WHERE idsupplier='%d'";
    }elseif ($KOLOM == "11"){
        $sql_update = "UPDATE supplier SET payment_type='%d' WHERE idsupplier='%d'";
    }elseif ($KOLOM == "12"){
        $sql_update = "UPDATE supplier SET payment_term='%d' WHERE idsupplier='%d'";
    }elseif ($KOLOM == "13"){
        $sql_update = "UPDATE supplier SET konsimen_margin='%f' WHERE idsupplier='%d'";
    }
    db_query($sql_update,$NILAI,$SUPPLIER_ID);
    if ($KOLOM == "10"){
        $ArrStatus = array('External','Internal');
        $NILAI = $ArrStatus[$NILAI];
    }else if($KOLOM == "11"){
        $PaymentType = array_payment_type();
        $NILAI = $PaymentType[$NILAI];
    }
    echo $NILAI;
	exit();	
}
function bayar_hutang_supplier(){
	//idpelanggan, nilaisebelumbayar, pembayaran, uid, tglbayar
	if ($_POST["idsupplier"] AND $_POST["pembayaran"]){
		global $user;
		$RelatedNota = isset($_REQUEST['related_nota']) && !empty($_REQUEST['related_nota']) ? $_REQUEST['related_nota'] : null;
		$Keterangan = isset($_POST['keterangan']) && !empty($_POST['keterangan']) ? $_POST['keterangan'] : '';
		db_query("INSERT INTO pembayaranhutang (idsupplier, hutangsebelumbayar, pembayaran, uid, tglbayar, keterangan, 
        discount_persen, discount_value) VALUES ('%d', '%f', '%f', '%d', '%s', '%s', '%d', '%f')",
        $_POST["idsupplier"],$_POST["hutang"],$_POST["pembayaran"],
        $user->uid,$_POST["tglbayar"],$Keterangan,$_POST["discount_persen"], $_POST["discount_value"]);
		$IdPembayaran = db_last_insert_id('pembayaranhutang','id');
		$sisahutang = $_POST["hutang"] - $_POST["pembayaran"];
		$tglsekarang = date("Y-m-d H:i:s");
		db_query("UPDATE hutang SET besarhutang='%f',pembayaranterakhir='%f',last_update='%s' WHERE idsupplier='%d'",
		$sisahutang,$_POST["pembayaran"],$tglsekarang,$_POST["idsupplier"]);
		if (!empty($RelatedNota)){
		    $TotalBayar = $_POST["pembayaran"];
		    for ($i = 0;$i < count($RelatedNota);$i++){
		        $StrSql = 'SELECT payment_paid FROM pembelian WHERE idpembelian = %d';
		        $TotalDibayar = db_result(db_query($StrSql, $RelatedNota[$i]));
                $StrSql = 'SELECT total FROM pembelian WHERE idpembelian = %d';
		        $TotalNota = db_result(db_query($StrSql, $RelatedNota[$i]));
		        if (is_null($TotalDibayar)){
                    $TotalDibayar = 0;
                }
                if ($TotalBayar >= ($TotalNota - $TotalDibayar)){
                    $StrSqlUpdate = "UPDATE pembelian SET payment_status=%d, ";
                    $StrSqlUpdate .= "payment_paid='%f', uid_changed=%d WHERE idpembelian=%d";
                    db_query($StrSqlUpdate, 2, $TotalNota, $user->uid, $RelatedNota[$i]);
                    $StrSqlInsert = "INSERT INTO pembayaranhutang_nota (idpembayaran, idpembelian, status_pembayaran, total_bayar)";
                    $StrSqlInsert .= "VALUES (%d, %d, %d,'%f')";
                    db_query($StrSqlInsert, $IdPembayaran, $RelatedNota[$i],2, ($TotalNota - $TotalDibayar));
                    $TotalBayar = $TotalBayar - ($TotalNota - $TotalDibayar);
                }else{
                    if ($TotalBayar > 0){
                        $StrSqlUpdate = "UPDATE pembelian SET payment_status=%d, ";
                        $StrSqlUpdate .= "payment_paid='%f', uid_changed=%d WHERE idpembelian=%d";
                        db_query($StrSqlUpdate, 1, $TotalBayar, $user->uid, $RelatedNota[$i]);
                        $StrSqlInsert = "INSERT INTO pembayaranhutang_nota (idpembayaran, idpembelian, status_pembayaran, total_bayar)";
                        $StrSqlInsert .= "VALUES (%d, %d, %d,'%f')";
                        db_query($StrSqlInsert, $IdPembayaran, $RelatedNota[$i],1, $TotalBayar);
                        $TotalBayar = 0;
                    }
                }
            }
        }
	}
	exit();	
}
function detail_hutang(){
	if ($_POST["idsupplier"]){
        $dataPremis = get_data_premis(false);
		get_number_format($currencySym, $thousandSep, $decimalSep, false);
        $DecDigit = $dataPremis->decimal_digit;
		$tabelhutang ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_hutang">';
		$tabelhutang .= '<thead>';
		$tabelhutang .= '<tr>';
		$tabelhutang .= '<th class="tablebutton"></th>';
		$tabelhutang .= '<th style="width: 150px;">NO. NOTA</th>';
		$tabelhutang .= '<th>TOTAL</th>';
		$tabelhutang .= '<th>DP</th>';
		$tabelhutang .= '<th>HUTANG</th>';
		$tabelhutang .= '<th class="tanggal">TRH JUAL</th>';
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
			$tabelhutang .= '<td class="angka">'.number_format($data->total,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelhutang .= '<td class="angka">'.number_format($data->bayar,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelhutang .= '<td class="angka">'.number_format($data->besarhutang,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelhutang .= '<td class="center">'.$data->tglbeli.'</td>';
			$tabelhutang .= '</tr>';
		}
		$tabelhutang .= '</tbody>';
		$tabelhutang .= '<tfoot>';
		$tabelhutang .= '<tr>';
		$tabelhutang .= '<th style="text-align:right" colspan="2">Total:&nbsp;&nbsp;</th>';
		$tabelhutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totalbelanja,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelhutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totaldp,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelhutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totalhutang,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
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
        $dataPremis = get_data_premis(false);
        $DecDigit = $dataPremis->decimal_digit;
		$tabelpembayaran ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="history_pembayaran">';
		$tabelpembayaran .= '<thead>';
		$tabelpembayaran .= '<tr>';
        $tabelpembayaran .= '<th class="tablebutton">&nbsp;</th>';
		$tabelpembayaran .= '<th class="tanggal">TRH BAYAR</th>';
		$tabelpembayaran .= '<th>KETERANGAN</th>';
		$tabelpembayaran .= '<th>NILAI</th>';
        $tabelpembayaran .= '<th>'.strtoupper(trans('faktur', $dataPremis->bahasa)).'</th>';
		$tabelpembayaran .= '</tr>';
		$tabelpembayaran .= '</thead>';
		$tabelpembayaran .= '<tbody>';
		$StrSql = "SELECT ph.id, ph.idsupplier, ph.keterangan, ph.pembayaran, SUBSTR(ph.last_update,1,10) AS tglbayar, ";
        $StrSql .= "(SELECT GROUP_CONCAT(CONCAT(pemb.nonota,'[',(case phn.status_pembayaran when 2 then 'LUNAS' when 1 then 'SEBAGIAN' end),']') SEPARATOR ', ') FROM pembayaranhutang_nota AS phn ";
        $StrSql .= "LEFT JOIN pembelian AS pemb ON phn.idpembelian = pemb.idpembelian WHERE phn.idpembayaran = ph.id) AS related_nota ";
        $StrSql .= "FROM pembayaranhutang AS ph WHERE ph.idsupplier='%d'";
		$result = db_query($StrSql,$_POST["idsupplier"]);
		$totalbayar = 0;
		while ($data = db_fetch_object($result)){
			$tabelpembayaran .= '<tr>';
            $TombolHapus = '<img title="Klik untuk menghapus pembayaran hutang" onclick="delete_pembayaran('.$data->id.','.$data->idsupplier.');" src="'.base_path().'misc/media/images/del.ico" width="22">';
            $tabelpembayaran .= '<td class="center">'.$TombolHapus.'</td>';
			$tabelpembayaran .= '<td class="center">'.$data->tglbayar.'</td>';
			$tabelpembayaran .= '<td>'.$data->keterangan.'</td>';
			$totalbayar = $totalbayar + $data->pembayaran;
			$tabelpembayaran .= '<td class="angka">'.number_format($data->pembayaran,$DecDigit,$decimalSep,$thousandSep).'</td>';
            $tabelpembayaran .= '<td class="angka">'.$data->related_nota.'</td>';
			$tabelpembayaran .= '</tr>';
		}
		$tabelpembayaran .= '</tbody>';
		$tabelpembayaran .= '<tfoot>';
		$tabelpembayaran .= '<tr>';
		$tabelpembayaran .= '<th style="text-align:right">Total:&nbsp;&nbsp;</th>';
		$tabelpembayaran .= '<th colspan="4" style="text-align: right" >'.$currencySym.' '.number_format($totalbayar,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelpembayaran .= '</tr>';
		$tabelpembayaran .= '</tfoot>';
		$tabelpembayaran .= '</table>';
		$tabelpembayaran .= '<br><br>';
		print $tabelpembayaran;
	}
	exit();	
}
function delete_pembayaran_supplier(){
    if (isset($_GET['idsupplier']) && !empty($_GET['idsupplier']) && isset($_GET['idpembayaran']) && !empty($_GET['idpembayaran'])){
        $IdSupplier = $_GET['idsupplier'];
        $IdPembayaran = $_GET['idpembayaran'];
        db_query('DELETE FROM pembayaranhutang WHERE id=%d', $IdPembayaran);
        $StrSql = 'SELECT idpembayaran,idpembelian,status_pembayaran, total_bayar FROM pembayaranhutang_nota WHERE idpembayaran=%d';
        $result = db_query($StrSql, $IdPembayaran);
        while ($data = db_fetch_object($result)) {
            $StrSql2 = 'SELECT total FROM pembelian WHERE idpembelian=%d';
            $TotalPembelian = db_result(db_query($StrSql2, $data->idpembelian));
            $StrSql2 = 'SELECT payment_paid FROM pembelian WHERE idpembelian=%d';
            $TotalPaid = db_result(db_query($StrSql2, $data->idpembelian));
            $TotalPaid = $TotalPaid - $data->total_bayar;
            $PaymentStatus = 0;
            if ($TotalPaid == 0) {
                $PaymentStatus = 0;
            } else if ($TotalPaid < $TotalPembelian) {
                $PaymentStatus = 1;
            }
            $StrSqlUpdate = "UPDATE pembelian SET payment_status=%d, payment_paid='%f' WHERE idpembelian=%d";
            db_query($StrSqlUpdate, $PaymentStatus, $TotalPaid, $data->idpembelian);
            $StrSqlDelete = 'DELETE FROM pembayaranhutang_nota WHERE idpembayaran=%d AND idpembelian=%d AND status_pembayaran=%d';
            db_query($StrSqlDelete, $data->idpembayaran, $data->idpembelian, $data->status_pembayaran);
        }
        syncHutangSupplier($IdSupplier);
    }
    drupal_goto('datasupplier/supplier');
    exit();
}