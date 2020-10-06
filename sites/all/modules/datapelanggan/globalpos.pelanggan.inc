<?php
/**
 * @return string
 */
function data_pelanggan(){
	get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
	$dataPremis = get_data_premis();
	//drupal_add_css('misc/media/datatables.1.10/media/css/jquery.dataTables.min.css');
	drupal_add_css('misc/media/datatables.1.10/jquery/jquery-ui.css');
	drupal_add_css('misc/media/datatables.1.10/media/css/dataTables.jqueryui.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.dataTables.css');
    drupal_add_css('misc/media/datatables.1.10/extensions/Buttons/css/buttons.jqueryui.css');
    drupal_add_css('misc/media/jqueryValidate/css/validationEngine.jquery.css');
    drupal_add_css('misc/media/jqueryValidate/css/template.css');
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
	drupal_add_js('misc/media/jquery.jeditable.js');
	drupal_add_js('misc/media/jquery.autotab-1.1b.js');
    drupal_add_js('misc/media/datatables/js/number.format.js');
    _addChosenJqueryPlugins();
    _addJeditablePlugins();
    _addBlockUiPlugins(true);
	drupal_add_js(
		array(
			'filepath' => file_directory_path(),
			'datapremis' => $dataPremis,
            'dec_digit' => $DecDigit,
		),
		'setting');
	$modulePath = drupal_get_path('module','datapelanggan');
	$jsPath = $modulePath.'/js/datapelanggan.js';
	drupal_add_js($jsPath);
	$viewtabelpelanggan = '<form id="formpelanggan" action="'.base_path().'datapelanggan/simpanpelanggan" method="post">';
    $viewtabelpelanggan .= '<div id="formplace" class="pelanggan">';
	$viewtabelpelanggan .= '<label>'.strtoupper(trans('kode', $dataPremis->bahasa)).'</label><input type="text" class=\"validate[required]\" id="kodepelanggan" name="kodepelanggan">';
	$viewtabelpelanggan .= '<label>PELANGGAN</label><input class=\"validate[required]\" type="text" id="namapelanggan" name="namapelanggan">';
	$viewtabelpelanggan .= '<label>TELEPON</label><input type="text" id="telp" name="telp">';
	$viewtabelpelanggan .= '<label>ALAMAT</label><input type="text" id="alamat" name="alamat">';
	$viewtabelpelanggan .= '<label>EMAIL</label><input type="text" id="email" name="email">';
	if (module_exists('grup_pelanggan')){
	    $OptionGrup = get_array_data_grup_pelanggan(true);
	    $SelectGrup = '<select id="grup-pelanggan" name="grup-pelanggan">';
	    foreach ($OptionGrup as $Idx => $NamaGrup){
            $SelectGrup .= '<option value="'.$Idx.'">'.$NamaGrup.'</option>';
        }
        $SelectGrup .= '</select>';
        $viewtabelpelanggan .= '<label>GRUP</label>'.$SelectGrup;
    }
	$viewtabelpelanggan .= '<div class="float-left-full"><button style="font-size: 12px;">Tambah Pelanggan</button></div>';
    $viewtabelpelanggan .= '</div></form>';
	$viewtabelpelanggan .= '<div class="button-sync">';
    $viewtabelpelanggan .= '<button id="sync-hutang-pelanggan" style="font-size: 12px;">Sync Hutang Pelanggan</button>';
    $viewtabelpelanggan .= '&nbsp;&nbsp;';
    $viewtabelpelanggan .= '<button id="sync-selected-pelanggan" style="font-size: 12px;">Sync Selected</button>';
    $viewtabelpelanggan .= '</div>';
	$viewtabelpelanggan .= '<div style="width: 80%;">'.tabel_pelanggan($dataPremis).'</div>';
	$viewtabelpelanggan .= '<div id="dialogdetailhutang" title="DETAIL HUTANG DAN PEMBAYARAN"><div id="tempatnilaihutang"></div><div id="detailpiutang"></div><div id="detailpembayaran">a</div></div>';
	$viewtabelpelanggan .= '<div id="dialogdetail" title="DETAIL PENJUALAN"></div>';
	$viewtabelpelanggan .= '<div id="dialogpembayaran" title="PEMBAYARAN HUTANG">';
	$viewtabelpelanggan .= '<div id="totalhutangpelanggan"></div>';
	$viewtabelpelanggan .= '<div class="baris"><label>Tgl Bayar</label><input readonly="readonly" type="text" id="tglbayar" name="tglbayar" value="'.date("d-m-Y").'"><input type="hidden" id="tglbayarkirim" name="tglbayarkirim" value="'.date("Y-m-d").'"></div>';
	$viewtabelpelanggan .= '<div class="baris"><label>Pembayaran</label><input type="text" id="nilaipembayaran" name="nilaipembayaran"></div>';
	$viewtabelpelanggan .= '<div class="baris"><label>Keterangan</label><input type="text" id="keterangan" name="keterangan"></div>';
	$viewtabelpelanggan .= '<div class="baris"><button id="bayarhutang" onclick="do_pembayaran();">BAYAR</button>';
	$viewtabelpelanggan .= '<input type="hidden" id="idpelangganbayar" name="idpelangganbayar">';
	$viewtabelpelanggan .= '<input type="hidden" id="tothutang" name="tothutang"></div>';
    $viewtabelpelanggan .= '<div id="diskonpelanggan">';
    $viewtabelpelanggan .= '<div id="tambahdiskon">';
    $result = db_query("SELECT idkategori,kodekategori,kategori FROM kategori ORDER BY kodekategori,kategori");
    $pilihankategori = '<select id="idkategori" name="idkategori" onchange="get_product()">';
    while ($data = db_fetch_object($result)){
        $pilihankategori .= '<option value="'.$data->idkategori.'">'.$data->kodekategori.'-'.$data->kategori.'</option>';
    }
    $pilihankategori .= '</select>';
    $pilihanproduct = '<select id="idproduct" name="idproduct" multiple="multiple">';
    $pilihanproduct .= '</select>';
    $viewtabelpelanggan .= '<div class="baris"><label>Pilih Kategori Produk</label>'.$pilihankategori.'</div>';
    $viewtabelpelanggan .= '<div class="baris"><label>Pilih Produk</label>'.$pilihanproduct.'</div>';
    $viewtabelpelanggan .= '<div class="baris"><label>Diskon</label><input type="text" id="diskon" name="diskon" value="0"></div>';
    $viewtabelpelanggan .= '<input type="hidden" id="idpelanggan" name="idpelanggan">';
    $viewtabelpelanggan .= '<input type="hidden" id="namapelanggandiskon" name="namapelanggandiskon">';
    $viewtabelpelanggan .= '<div class="baris"><button onclick="tambahdiskon()">Simpan Diskon</button></div>';
    $viewtabelpelanggan .= '</div>';
    $viewtabelpelanggan .= '<div class="baris"></div>';
    $viewtabelpelanggan .= '<div id="tabeldiskon">';
    $viewtabelpelanggan .= '</div>';
    $viewtabelpelanggan .= '</div>';
    $viewtabelpelanggan .= '</div>';
	return $viewtabelpelanggan;
}

/**
 * @return void
 */
function hapus_diskon(){
    if (isset($_REQUEST["id"]) && !empty($_REQUEST["id"])){
        $IdDiskon = $_REQUEST["id"];
        if (is_array($IdDiskon)){
            for ($i = 0;$i < count($IdDiskon);$i++){
                db_query("DELETE FROM diskonkategori WHERE id='%d'", $IdDiskon[$i]);
            }
        }else {
            db_query("DELETE FROM diskonkategori WHERE id='%d'", $IdDiskon);
        }
    }
    echo $IdDiskon;
    exit();
}

/**
 *@return void
 */
function simpan_diskon(){
    if (isset($_POST["idpelanggan"]) && !empty($_POST["idpelanggan"])
        && isset($_POST["idkategori"]) && !empty($_POST["idkategori"])){
        if (isset($_REQUEST["idproduct"]) && !empty($_REQUEST["idproduct"])){
            $ArrayProduct = $_REQUEST["idproduct"];
            for ($i = 0;$i < count($ArrayProduct);$i++) {
                $IdProduct = $ArrayProduct[$i];
                $StrSQL = 'SELECT COUNT(*) AS jumlahdiskon FROM diskonkategori ';
                $StrSQL .= 'WHERE idpelanggan=%d AND idkategori=%d AND idproduct=%d';
                $DiskonExists = db_result(db_query($StrSQL, $_POST["idpelanggan"], $_POST["idkategori"], $IdProduct));
                if ($DiskonExists > 0) {
                    $StrUpdate = "UPDATE diskonkategori SET besardiskon='%f' ";
                    $StrUpdate .= "WHERE idpelanggan=%d AND idkategori=%d AND idproduct=%d";
                    db_query($StrUpdate,
                        $_POST["diskon"], $_POST["idpelanggan"], $_POST["idkategori"], $IdProduct);
                } else {
                    db_query("INSERT INTO diskonkategori (besardiskon,idpelanggan,idkategori, idproduct)VALUES('%f','%d','%d','%d')",
                        $_POST["diskon"], $_POST["idpelanggan"], $_POST["idkategori"], $IdProduct);
                }
            }
        }else {
            $StrSQL = 'SELECT COUNT(*) AS jumlahdiskon FROM diskonkategori ';
            $StrSQL .= 'WHERE idpelanggan=%d AND idkategori=%d AND idproduct IS NULL';
            $DiskonExists = db_result(db_query($StrSQL, $_POST["idpelanggan"], $_POST["idkategori"]));
            if ($DiskonExists > 0) {
                $StrUpdate = "UPDATE diskonkategori SET besardiskon='%f' ";
                $StrUpdate .= "WHERE idpelanggan=%d AND idkategori=%d AND idproduct IS NULL";
                db_query($StrUpdate,
                    $_POST["diskon"], $_POST["idpelanggan"], $_POST["idkategori"]);
            } else {
                db_query("INSERT INTO diskonkategori (besardiskon,idpelanggan,idkategori)VALUES('%f','%d','%d')",
                    $_POST["diskon"], $_POST["idpelanggan"], $_POST["idkategori"]);
            }
        }
        print $DiskonExists->jumlahdiskon;
    }
    exit();
}

/**
 *@return void
 */
function tabel_diskon(){
    if (isset($_POST['idpelanggan']) && !empty($_POST['idpelanggan'])){
        $NamaPelanggan = db_result(db_query('SELECT namapelanggan FROM pelanggan WHERE idpelanggan=%d', $_POST['idpelanggan']));
        $tabeldiskon ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_diskon">';
        $tabeldiskon .= '<thead>';
        $tabeldiskon .= '<tr>';
        $tabeldiskon .= '<th class="tablebutton">&nbsp;</th>';
        $tabeldiskon .= '<th>KATEGORI</th>';
        $tabeldiskon .= '<th>PRODUK</th>';
        $tabeldiskon .= '<th class="diskon">DISKON[%]</th>';
        $tabeldiskon .= '<th class="tablebutton">&nbsp;</th>';
        $tabeldiskon .= '</tr>';
        $tabeldiskon .= '</thead>';
        $tabeldiskon .= '<tbody>';
        $StrSQL = 'SELECT id, besardiskon, diskon.idkategori, kat.kategori, diskon.idproduct,';
        $StrSQL .= 'prod.namaproduct FROM diskonkategori AS diskon ';
        $StrSQL .= 'LEFT JOIN kategori AS kat ON diskon.idkategori = kat.idkategori ';
        $StrSQL .= 'LEFT JOIN product AS prod ON prod.idproduct = diskon.idproduct ';
        $StrSQL .= 'WHERE diskon.idpelanggan = %d';
        $result = db_query($StrSQL,$_POST["idpelanggan"]);
        while($data = db_fetch_object($result)){
            $tabeldiskon .= '<tr>';
            $hapusdiskon = '<img width="24" onclick="hapus_diskon(\''.$_POST["idpelanggan"].'\','.$data->id.',\''.$_POST["namapelanggan"].'\');" title="Klik untuk menghapus diskon pada kategori/product ini" src="'.base_path().'misc/media/images/close.ico">';
            $tabeldiskon .= '<td class="center">'.$hapusdiskon.'</td>';
            $tabeldiskon .= '<td class="center">'.$data->kategori.'</td>';
            $tabeldiskon .= '<td>'.$data->namaproduct.'</td>';
            $tabeldiskon .= '<td id="diskon-'.$data->id.'" class="angka besar-diskon">'.$data->besardiskon.'</td>';
            $Checkbox = '<input class="selected-diskon" type="checkbox" id="select-diskon-'.$data->id.'" name="select-diskon-'.$data->id.'" value="'.$data->id.'">';
            $tabeldiskon .= '<td class="center">'.$Checkbox.'</td>';
            $tabeldiskon .= '</tr>';
        }
        $tabeldiskon .= '</tbody>';
        $tabeldiskon .= '</table>';
        $tabeldiskon .= '<br>';
        $tabeldiskon .= '<div class="baris"><button id="hapus_pilihan" onclick="hapus_pilihan_diskon('.$_POST['idpelanggan'].', \''.$NamaPelanggan.'\')">Hapus Diskon</button></div>';
        print $tabeldiskon;
    }
    exit();
}

/**
 *@return void
 */
function bayar_hutang(){
	//idpelanggan, nilaisebelumbayar, pembayaran, uid, tglbayar
	if (isset($_POST["idpelanggan"]) && isset($_POST["pembayaran"])){
		$variables = array();
		$variables['idpelanggan'] = $_POST["idpelanggan"];
		$variables['pembayaran'] = $_POST["pembayaran"];
		$variables['hutang'] = $_POST["hutang"];
		$variables['tglbayar'] = $_POST["tglbayar"];
		if ($_POST["keterangan"]){
			$variables['keterangan'] = $_POST["keterangan"];
		}else{
			$variables['keterangan'] = 'Pembayaran Hutang';
		}
		$newvariables = pembayaranhutang($variables);
		print json_encode($newvariables);
	}
	exit();
}

/**
 * @return string
 */
function tabel_pelanggan($dataPremis = null){
    get_number_format($currencySym, $thousandSep, $decimalSep,false);
	if (!empty($dataPremis)){
		$currencySym = $dataPremis->currency_symbol;
		$thousandSep = $dataPremis->thousand_separator;
		$decimalSep = $dataPremis->decimal_separator;
	}else{
		$currencySym = 'Rp.';
		$thousandSep = '.';
		$decimalSep = ',';
	}
	$tabelPelanggan ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_pelanggan">';
	$tabelPelanggan .= '<thead>';
	$tabelPelanggan .= '<tr>';
	$tabelPelanggan .= '<th class="tablebutton"></th>';
	$tabelPelanggan .= '<th class="tablebutton"></th>';
	$tabelPelanggan .= '<th class="tablebutton"></th>';
	$tabelPelanggan .= '<th style="width: 150px;">KOD</th>';
	$tabelPelanggan .= '<th>PELANGGAN</th>';
	$tabelPelanggan .= '<th>TELP</th>';
	$tabelPelanggan .= '<th>ALAMAT</th>';
	$tabelPelanggan .= '<th>EMAIL</th>';
	$tabelPelanggan .= '<th>PIUTANG</th>';
	$tabelPelanggan .= '<th>AKHIR</th>';
	$tabelPelanggan .= '<th class="tanggal">UPDATE</th>';
	$ModuleGrupExist = module_exists('grup_pelanggan');
    if ($ModuleGrupExist){
        $tabelPelanggan .= '<th>GRUP</th>';
    }
    $tabelPelanggan .= '<th class="tablebutton">&nbsp;</th>';
	$tabelPelanggan .= '</tr>';
	$tabelPelanggan .= '</thead>';
	$tabelPelanggan .= '<tfoot>';
	$tabelPelanggan .= '<tr>';
	$tabelPelanggan .= '<th style="text-align:right;" colspan="8">Total:&nbsp;&nbsp;</th>';
	$totalhutang = getTotalHutangPelanggan();
    if ($ModuleGrupExist){
        $tabelPelanggan .= '<th class="angka" colspan="5">'.$currencySym.' '.number_format($totalhutang,2,$decimalSep,$thousandSep).'</th>';
    }else{
        $tabelPelanggan .= '<th class="angka" colspan="4">'.$currencySym.' '.number_format($totalhutang,2,$decimalSep,$thousandSep).'</th>';
    }
	$tabelPelanggan .= '</tr>';
	$tabelPelanggan .= '</tfoot>';
	$tabelPelanggan .= '</table>';
	return $tabelPelanggan;
}

/**
 * @return void
 */
function simpan_pelanggan(){
	if (isset($_POST['kodepelanggan']) AND isset($_POST['namapelanggan'])){
	    if (module_exists('grup_pelanggan') && isset($_POST['grup-pelanggan'])){
            db_query("INSERT INTO pelanggan(kodepelanggan,namapelanggan,telp,alamat,email, id_grup) 
            VALUES('%s','%s','%s','%s','%s',%d)",
                strtoupper($_POST['kodepelanggan']),$_POST['namapelanggan'],$_POST['telp'],$_POST['alamat'],
                $_POST['email'], $_POST['grup-pelanggan']);
        }else{
            db_query("INSERT INTO pelanggan(kodepelanggan,namapelanggan,telp,alamat,email) VALUES('%s','%s','%s','%s','%s')",
                strtoupper($_POST['kodepelanggan']),$_POST['namapelanggan'],$_POST['telp'],$_POST['alamat'],$_POST['email']);
        }
		if (isset($_POST['asal'])){
			$result = db_query("SELECT idpelanggan FROM pelanggan WHERE kodepelanggan='%s' AND namapelanggan='%s'",
			strtoupper($_POST['kodepelanggan']),$_POST['namapelanggan']);
			$data = db_fetch_object($result);
			print $data->idpelanggan;
		}else{
			drupal_goto('datapelanggan/pelanggan');
		}
	}
	exit();
}

/**
 * @return void 
 */
function update_pelanggan(){
	$NILAI = $_POST['value'];
	$KOLOM = $_POST['kol_id'];
	$PELANGGAN_ID = $_POST['row_id'];
	$ModuleGrupExist = module_exists('grup_pelanggan');
	if ($KOLOM == '3'){
		$NILAI = strtoupper($NILAI);
		$sql_update = "UPDATE pelanggan SET kodepelanggan='%s' WHERE idpelanggan='%d'";
	}elseif ($KOLOM == '4'){
		$sql_update = "UPDATE pelanggan SET namapelanggan='%s' WHERE idpelanggan='%d'";
	}elseif ($KOLOM == '5'){
		$sql_update = "UPDATE pelanggan SET telp='%s' WHERE idpelanggan='%d'";
	}elseif ($KOLOM == '6'){
		$sql_update = "UPDATE pelanggan SET alamat='%s' WHERE idpelanggan='%d'";
	}elseif ($KOLOM == '7'){
		$sql_update = "UPDATE pelanggan SET email='%s' WHERE idpelanggan='%d'";
	}elseif ($KOLOM == '11'){
        $sql_update = "UPDATE pelanggan SET id_grup='%d' WHERE idpelanggan='%d'";
    }
	db_query($sql_update,$NILAI,$PELANGGAN_ID);
    if ($ModuleGrupExist && $KOLOM == '11'){
        $ArrGrup = get_array_data_grup_pelanggan(true);
        $NILAI = $ArrGrup[$NILAI];
    }
	echo $NILAI;
	exit();
}

/**
 * @return void
 */
function detail_piutang_old(){
	if (isset($_POST['idpelanggan'])){
        get_number_format($currencySym, $thousandSep, $decimalSep,false);
		$tabelpiutang ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_hutang">';
		$tabelpiutang .= '<thead>';
		$tabelpiutang .= '<tr>';
		$tabelpiutang .= '<th></th>';
		$tabelpiutang .= '<th>NO. NOTA</th>';
		$tabelpiutang .= '<th>TOTAL</th>';
		$tabelpiutang .= '<th>DP</th>';
		$tabelpiutang .= '<th>PIUTANG</th>';
		$tabelpiutang .= '<th>TRH JUAL</th>';
		$tabelpiutang .= '</tr>';
		$tabelpiutang .= '</thead>';
		$tabelpiutang .= '<tbody>';
		$result = db_query("SELECT a.idpelanggan, a.idpenjualan,a.besarhutang,b.nonota,b.total,b.bayar,SUBSTR(b.tglpenjualan,1,10) AS tgljual FROM
		detailpiutang a, penjualan b WHERE a.idpenjualan = b.idpenjualan AND a.idpelanggan='%d'",
		$_POST['idpelanggan']);
		$totalbelanja = 0;
		$totaldp = 0;
		$totalhutang = 0;
		while ($data = db_fetch_object($result)){
			$lihatdetailpenjualan = '<img title="Klik untuk melihat detail penjualan" onclick="view_detail(\''.$data->idpenjualan.'\',\''.$data->nonota.'\');" src="'.base_path().'misc/media/images/forward_enabled.ico">';
			$tabelpiutang .= '<tr id="'.$data->idpenjualan.'">';
			$tabelpiutang .= '<td class="center">'.$lihatdetailpenjualan.'</td>';
			$tabelpiutang .= '<td class="center">'.$data->nonota.'</td>';
			$totalbelanja = $totalbelanja + $data->total;
			$totaldp = $totaldp + $data->bayar;
			$totalhutang = $totalhutang + $data->besarhutang;
			$tabelpiutang .= '<td class="angka">'.number_format($data->total,2,$decimalSep,$thousandSep).'</td>';
			$tabelpiutang .= '<td class="angka">'.number_format($data->bayar,2,$decimalSep,$thousandSep).'</td>';
			$tabelpiutang .= '<td class="angka">'.number_format($data->besarhutang,2,$decimalSep,$thousandSep).'</td>';
			$tabelpiutang .= '<td class="center">'.$data->tgljual.'</td>';
			$tabelpiutang .= '</tr>';
		}
		$tabelpiutang .= '</tbody>';
		$tabelpiutang .= '<tfoot>';
		$tabelpiutang .= '<tr>';
		$dataPremis = get_data_premis();
		if (!empty($dataPremis)){
			$currencySym = $dataPremis->currency_symbol;
			$thousandSep = $dataPremis->thousand_separator;
			$decimalSep = $dataPremis->decimal_separator;
		}else{
			$currencySym = 'Rp.';
			$thousandSep = '.';
			$decimalSep = ',';
		}
		$tabelpiutang .= '<th style="text-align:right" colspan="2">Total:&nbsp;&nbsp;</th>';
		$tabelpiutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totalbelanja,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelpiutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totaldp,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelpiutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totalhutang,2,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelpiutang .= '<th></th>';
		$tabelpiutang .= '</tr>';
		$tabelpiutang .= '</tfoot>';
		$tabelpiutang .= '</table>';
		$tabelpiutang .= '<br><br>';
		print $tabelpiutang;
	}
	exit();
}

/**
 * @return void
 */
function detail_piutang(){
    if (isset($_POST['idpelanggan'])){
        get_number_format($currencySym, $thousandSep, $decimalSep,false);
        $DataPremis = get_data_premis();
        $Digit = $DataPremis->decimal_digit;
        $tabelpiutang ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_hutang">';
        $tabelpiutang .= '<thead>';
        $tabelpiutang .= '<tr>';
        $tabelpiutang .= '<th></th>';
        $tabelpiutang .= '<th>NO. NOTA</th>';
        $tabelpiutang .= '<th>TOTAL</th>';
        $tabelpiutang .= '<th>DP</th>';
        $tabelpiutang .= '<th>PIUTANG</th>';
        $tabelpiutang .= '<th>TGL JUAL</th>';
        $tabelpiutang .= '</tr>';
        $tabelpiutang .= '</thead>';
        $tabelpiutang .= '<tbody>';
        $totalbelanja = 0;
        $totaldp = 0;
        $totalhutang = 0;
        $tabelpiutang .= '</tbody>';
        $tabelpiutang .= '<tfoot>';
        $tabelpiutang .= '<tr>';
        $tabelpiutang .= '<th style="text-align:right" colspan="2">Total:&nbsp;&nbsp;</th>';
        $tabelpiutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totalbelanja,$Digit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
        $tabelpiutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totaldp,$Digit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
        $tabelpiutang .= '<th style="text-align: right" >'.$currencySym.' '.number_format($totalhutang,$Digit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
        $tabelpiutang .= '<th></th>';
        $tabelpiutang .= '</tr>';
        $tabelpiutang .= '</tfoot>';
        $tabelpiutang .= '</table>';
        $tabelpiutang .= '<br><br>';
        print $tabelpiutang;
    }
    exit();
}

/**
 *
 */
function detail_pembayaran(){
	//idlangganan, pembayaran, last_update
	if (isset($_POST['idpelanggan'])){
        get_number_format($currencySym, $thousandSep, $decimalSep,false);
        $dataPremis = get_data_premis(false);
        $DecDigit = $dataPremis->decimal_digit;
		$tabelPembayaran ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="history_pembayaran">';
		$tabelPembayaran .= '<thead>';
		$tabelPembayaran .= '<tr>';
        $tabelPembayaran .= '<th class="tablebutton">&nbsp;</th>';
		$tabelPembayaran .= '<th>TGL BAYAR</th>';
		$tabelPembayaran .= '<th>KETERANGAN</th>';
		$tabelPembayaran .= '<th class="col_angka2">NILAI</th>';
		$tabelPembayaran .= '</tr>';
		$tabelPembayaran .= '</thead>';
		$tabelPembayaran .= '<tbody>';
		$result = db_query("SELECT id, pembayaran, SUBSTR(last_update,1,10) AS tglbayar, keterangan FROM historypembayaran WHERE idpelanggan='%d' ORDER BY tglbayar",
		$_POST['idpelanggan']);
		$totalbayar = 0;
		while ($data = db_fetch_object($result)){
			$tabelPembayaran .= '<tr>';
            $TombolHapus = '<img title="Klik untuk menghapus pembayaran hutang" onclick="delete_pembayaran('.$data->id.','.$_POST['idpelanggan'].');" src="'.base_path().'misc/media/images/del.ico" width="22">';
            $tabelPembayaran .= '<td class="center">'.$TombolHapus.'</td>';
			$tabelPembayaran .= '<td class="center">'.$data->tglbayar.'</td>';
			$totalbayar = $totalbayar + $data->pembayaran;
			$tabelPembayaran .= '<td class="angka">'.$data->keterangan.'</td>';
			$tabelPembayaran .= '<td class="angka">'.number_format($data->pembayaran,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelPembayaran .= '</tr>';
		}
		$result = db_query("SELECT penggunaan, SUBSTR(last_update,1,10) AS tglbayar, keterangan FROM historypenggunaandeposit WHERE idpelanggan='%d' ORDER BY tglbayar",
		$_POST["idpelanggan"]);
		while ($data = db_fetch_object($result)){
			$tabelPembayaran .= '<tr>';
            $tabelPembayaran .= '<td class="center">&nbsp;</td>';
			$tabelPembayaran .= '<td class="center">'.$data->tglbayar.'</td>';
			$totalbayar = $totalbayar - $data->penggunaan;
			$tabelPembayaran .= '<td class="angka">'.$data->keterangan.'</td>';
			$tabelPembayaran .= '<td class="angka">-'.number_format($data->penggunaan,$DecDigit,$decimalSep,$thousandSep).'</td>';
			$tabelPembayaran .= '</tr>';
		}
		$tabelPembayaran .= '</tbody>';
		$tabelPembayaran .= '<tfoot>';
		$tabelPembayaran .= '<tr>';
		$tabelPembayaran .= '<th style="text-align:right" colspan="2">Total:&nbsp;&nbsp;</th>';
		get_number_format($currencySym, $thousandSep, $decimalSep);
		$tabelPembayaran .= '<th style="text-align: right" colspan="2">'.$currencySym.' '.number_format($totalbayar,$DecDigit,$decimalSep,$thousandSep).'&nbsp;&nbsp;</th>';
		$tabelPembayaran .= '</tr>';
		$tabelPembayaran .= '</tfoot>';
		$tabelPembayaran .= '</table>';
		$tabelPembayaran .= '<br>';
		$sisaHutangPelanggan = getTotalHutang($_POST["idpelanggan"]) - $totalbayar;
		if ($sisaHutangPelanggan < 0){
			$tabelPembayaran .= '<div style="font-size: 14px;"><strong>DEPOSIT PELANGGAN : <span style="color: blue;">'.$currencySym.' '.number_format(abs($sisaHutangPelanggan),$DecDigit,$decimalSep,$thousandSep).'</span>&nbsp;&nbsp;</div>';
		}else{
			$tabelPembayaran .= '<div style="font-size: 14px;"><strong>HUTANG PELANGGAN : '.$currencySym.' '.number_format($sisaHutangPelanggan,$DecDigit,$decimalSep,$thousandSep).'</span>&nbsp;&nbsp;</div>';
		}
		print $tabelPembayaran;
	}
	exit();	
}

function syncHutangPelangganAjax(){
	if (isset($_POST['idpelanggan'])){
        $idPelanggan = $_REQUEST['idpelanggan'];
		$totalSync = syncHutangPelanggan($idPelanggan);
	}else{
		$totalSync = syncHutangPelanggan();
	}
	print $totalSync;
	exit;
}
function delete_history_pembayaran(){
    if (isset($_POST['id']) && isset($_POST['idpelanggan'])){
        $DelId = $_POST['id'];
        $IdPelanggan = $_POST['idpelanggan'];
        $StrSql = 'DELETE FROM historypembayaran WHERE id=%d';
        db_query($StrSql, $DelId);
        syncHutangPelanggan($IdPelanggan);
    }
    exit();
}

function get_pelanggan_info(){
    if (isset($_POST['idpelanggan']) && !empty($_POST['idpelanggan'])){
        $IdPelanggan = $_POST['idpelanggan'];
        print json_encode(array(get_pelanggan_data($IdPelanggan)));
        exit();
    }
    exit();
}

function update_diskon_kategori_ajax(){
    $Diskon = 0;
    if (isset($_POST['id']) && !empty($_POST['id']) && isset($_POST['value'])){
        $BesarDiskon = $_POST['value'];
        $SplitId = explode('-',$_POST['id']);
        $Diskon = update_diskon_kategori($SplitId[1],$BesarDiskon);
    }
    print $Diskon;
    exit();
}