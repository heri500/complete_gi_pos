<?php

function pembelian(){
    get_number_format($currencySym, $thousandSep, $decimalSep);
    //$gstSym = get_gst_symbols();
    $dataPremis = getPremisInformation();
    $DecDigit = $dataPremis->decimal_digit;
	createLogoImagePath();
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
	drupal_add_css('misc/media/datatables/css/demo_table_jui.css');
	drupal_add_css('misc/media/themes/jquery-ui-1.8.11.custom.css');
	drupal_add_css('misc/media/css/validationEngine.jquery.css');
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	drupal_add_js('misc/media/jqClock.min.js');
	drupal_add_js('misc/media/jquery.validationEngine-en.js');
	drupal_add_js('misc/media/jquery.validationEngine.js');
	drupal_add_js('misc/media/jquery-ui-1.8.11.custom.min.js');
	drupal_add_js('misc/media/datatables/js/jquery.dataTables.min.js');
	drupal_add_js('misc/media/datatables/js/dataTables.addtr.js');
	drupal_add_js('misc/media/datatables/js/number.format.js');
	drupal_add_js('misc/media/jquery.autotab-1.1b.js');
	if (isset($_GET['tanggal'])){
		$tglsekarang = date("Y-m-d", strtotime($_GET['tanggal']));
		$tgltampil = date("d-m-Y", strtotime($_GET['tanggal']));
	}else{
		$tglsekarang = date("Y-m-d");
		$tgltampil = date("d-m-Y");
	}
	drupal_add_js(
		array(
			'tglsekarang' => $tglsekarang,
			'tgltampil' => $tgltampil,
            'dec_digit' => $DecDigit,
		), 
	'setting');
	$js_path = drupal_get_path('module','pembelian').'/js/pembelian.js';
	drupal_add_js($js_path);
	$panduantombol = '<a style="font-weight: bold;text-align: center;">Ctrl+Shift+</a>';
	$tombolcari = '<a id="tombolubahharga" class="cari" onclick="ubahharga()">F2:UBAH HARGA</a>';
	$tombolqty = '<a id="tombolubahqty" onclick="ubahqty()" class="buttonqty">F3:UBAH QTY</a>';
	$tombolhapus = '<a class="buttonhapus" onclick="hapus_latest_produk()" title="Hapus produk">F4:HAPUS</a>';
	$tombolselesai = '<a class="buttonselesai" onclick="kirim_data(0)">F5:SELESAI</a>';
	$tombolprint = '<a class="buttonprint" onclick="kirim_data(1)">F6:SELESAI & PRINT</a>';
	$pembelian = '<button onclick="munculkankasir();" style="font-size:12px;border: 1px solid orangered;">'.strtoupper("Klik untuk memunculkan form pembelian").'</button>';
	$pembelian .= '<div id="dialogkasir" style="font-size: 12px;" title="GLOBAL POS PEMBELIAN ['.$tgltampil.']">';
	$pembelian .= '<div id="info-kasir-waktu">';
	$pembelian .= '<div id="tempattanggalkasir">';
	$pembelian .= '<div id="tempattanggal">';
	$pembelian .= '<label>'.strtoupper(trans('tanggal', $dataPremis->bahasa)).'</label><input readonly="readonly" type="text" id="tglbeli" name="tglbeli" value="'.$tgltampil.'">';
	$pembelian .= '<input type="hidden" id="tglbelikirim" name="tglbelikirim" value="'.$tglsekarang.'">';
	$pembelian .= '</div>';
	$pembelian .= '<div id="tempatnamakasir">';
	global $user;
	$pembelian .= '<label>'.strtoupper(trans('kasir', $dataPremis->bahasa)).'</label><input readonly="readonly" type="text" id="kasir" name="kasir" value="'.$user->name.'">';
	$pembelian .= '</div>';
	$pembelian .= '</div>';
	$pembelian .= '<div id="tempatjam" align="center">';
	$pembelian .= 'Jam';
	$pembelian .= '</div>';
	$pembelian .= '<div id="tempatkosong">';
	$pembelian .= '<div id="tempatsupplier"><label>SUPPLIER</label></div>';
	$pembelian .= '<div id="tempatsupplier2">'.createSupplierSelection().'</div>';
	$pembelian .= '</div>';
	$pembelian .= '</div>';
	$pembelian .= '<div id="barcode_place">SCAN ATAU INPUT BARCODE, KODE ATAU NAMA PRODUK : <input type="text" id="barcode" name="barcode" class="barcodekasir"></div>';
	$pembelian .= '<div id="tempattabelkasir">'.view_kasir_pembelian().'</div>';
	$pembelian .= '<input type="hidden" id="last_id" name="last_id"><input type="hidden" id="lastqty" name="lastqty" value="1">';
    $pembelian .= '<input type="hidden" id="lastharga" name="lastharga">';
	$pembelian .= '<input type="hidden" id="nilaikirim" name="nilaikirim">';
	$pembelian .= '<div id="tempattombolkasir" style="height: 340px;">';
	$pembelian .= $panduantombol;
	$pembelian .= $tombolcari;
	$pembelian .= $tombolqty;
	$pembelian .= $tombolhapus;
	$pembelian .= $tombolselesai;
	$pembelian .= $tombolprint;
	$pembelian .= '</div>';
    $pembelian .= '<div id="tempattotal"><div id="totalbelanja">Total : '.$currencySym.' 0</div></div>';
	$pembelian .= '</div>';
	$pembelian .= '<div title="Informasi" id="dialogwarning">';
	$pembelian .= '<p id="pesantext">Produk yang dicari tidak ada, Silahkan masukkan Barcode/Kode Produk/Nama Produk yang lain...!!!</p>';
	$pembelian .= '<p style="text-align: center"><button id="tutupdialog" class="buttonwarning" onclick="tutupwarning();">CLOSE</button></p>';
	$pembelian .= '</div>';
	$pembelian .= '<div title="UBAH HARGA BELI" id="dialogubahharga">';
	$pembelian .= '<label>Harga Baru : </label><input type="text" id="newharga" name="newharga" value="1">';
	$pembelian .= '</div>';
	$pembelian .= '<div title="UBAH QTY" id="dialogubahqty">';
	$pembelian .= '<label>Qty Baru : </label><input type="text" id="newqty" name="newqty" value="1">';
	$pembelian .= '</div>';
    $pembelian .= '<div title="UBAH HARGA" id="dialogubahharga2">';
    $pembelian .= '<label>Harga Baru : </label><input type="text" id="newharga2" name="newharga2" value="1">';
    $pembelian .= '</div>';
	$pembelian .= '<div title="UBAH QTY" id="dialogubahqty2">';
	$pembelian .= '<label>Qty Baru : </label><input type="text" id="newqty2" name="newqty2">';
	$pembelian .= '</div>';
	$pembelian .= '<div title="PEMBAYARAN" id="dialogbayar">';
	$carabayar = '<select id="carabayar" name="carabayar">';
	$result = db_query("SELECT carabayar FROM carabayar");
	$i = 0;
	while($data = db_fetch_object($result)){
		if ($i == 0){
			$carabayar .= '<option selected="selected" value="'.$data->carabayar.'">'.$data->carabayar.'</option>';	
		}else{
			$carabayar .= '<option value="'.$data->carabayar.'">'.$data->carabayar.'</option>';	
		}
		$i++;
	}
	$carabayar .= '</select>';
	$nilaibayar = '<input type="text" id="nilaibayar" name="nilaibayar">';
	$kembali = '<input readonly="readonly" type="text" id="kembali" name="kembali">';
	$totalbelanja = '<input type="text" id="totalbelanjauser" name="totalbelanjauser">';
	$JatuhTempo = '<input type="text" id="jatuhtempo" name="jatuhtempo" readonly="readonly">';
    $JatuhTempo .= '<input type="text" id="jatuhtempokirim" name="jatuhtempokirim" style="display: none;">';
	$pembelian .= '<div class="barisbayar"><label>Total</label>'.$totalbelanja.'</div>';
	$pembelian .= '<div class="barisbayar"><label>Cara Bayar</label>'.$carabayar.'</div>';
	$pembelian .= '<div class="barisbayar"><label>Pembayaran</label>'.$nilaibayar.'</div>';
    $pembelian .= '<div id="barisjatuhtempo" class="barisbayar" style="display: none;"><label>'.ucfirst(trans('jatuh tempo', $dataPremis->bahasa)).'</label>'.$JatuhTempo.'</div>';
	$pembelian .= '<div id="bariskembali" class="barisbayar"><label>'.ucfirst(trans('kembali', $dataPremis->bahasa)).'</label>'.$kembali.'</div>';
	$pembelian .= '</div>';
	return $pembelian;
}

function simpan_pembelian(){
    //nonota, idpemakai, total, bayar, kembali
    if ($_POST["detail_produk"] AND $_POST["bayar"] >= 0 AND $_POST["totalbelanja"]){
        $result = db_query("SELECT idpembelian FROM pembelian ORDER BY idpembelian DESC LIMIT 1");
        $data = db_fetch_object($result);
        if ($data->idpembelian > 0){
            $next_id = $data->idpembelian + 1;
        }else{
            $next_id = 1;
        }
        $no_nota = buat_nota($next_id);
        $kembali = $_POST["bayar"] - $_POST["totalbelanja"];
        date_default_timezone_set('Asia/Jakarta');
        $waktujual = $_POST["tglbeli"]." ".date("H:i:s");
        $JatuhTempo = null;
        if (isset($_POST["jatuh_tempo"]) && !empty($_POST["jatuh_tempo"])) {
            $JatuhTempo = $_POST["jatuh_tempo"] . " " . date("H:i:s");
        }
        $caraBayar = $_POST["carabayar"];
        $IdSupplier = $_POST["idsupplier"];
        global $user;
        if (!empty($JatuhTempo)) {
            db_query("INSERT INTO pembelian (nonota, idpemakai, total, bayar, kembali, 
            tglpembelian, carabayar, jatuh_tempo, idsupplier) VALUES ('%s', '%d', '%f', '%f', '%f', '%s', '%s', '%s','%d')",
                $no_nota, $user->uid, $_POST["totalbelanja"], $_POST["bayar"], $kembali, $waktujual,
                $caraBayar, $JatuhTempo, $IdSupplier);
        }else{
            db_query("INSERT INTO pembelian (nonota, idpemakai, total, bayar, kembali, 
            tglpembelian, carabayar, idsupplier) VALUES ('%s', '%d', '%f', '%f', '%f', '%s', '%s', '%d')",
                $no_nota, $user->uid, $_POST["totalbelanja"], $_POST["bayar"], $kembali, $waktujual,
                $caraBayar, $IdSupplier);
        }
        $next_id = db_last_insert_id('pembelian', 'idpembelian');
        print $_POST["detail_produk"];
        if ($kembali < 0){
            db_query("INSERT INTO detailhutang (idsupplier, idpembelian, besarhutang) VALUES ('%d','%d','%f')",
                $_POST["idsupplier"],$next_id,abs($kembali));
            $resultpiutang = db_query("SELECT idsupplier, besarhutang  FROM hutang WHERE idsupplier='%d'",
                $_POST["idsupplier"]);
            $datapiutang = db_fetch_object($resultpiutang);
            if ($datapiutang->idsupplier){
                $besarhutangskrg = $datapiutang->besarhutang + abs($kembali);
                db_query("UPDATE hutang SET besarhutang='%f',uid='%d' WHERE idsupplier='%d'",
                    $besarhutangskrg,$user->uid,$_POST["idsupplier"]);
            }else{
                db_query("INSERT INTO hutang(idsupplier,besarhutang,uid)VALUES('%d','%f','%d')",
                    $_POST["idsupplier"],abs($kembali),$user->uid);
            }
        }
        $pecahdetail = explode("&",$_POST["detail_produk"]);
        foreach ($pecahdetail as $parameter){
            $pecahparameter = explode("=",$parameter);
            $pecahnilai = explode("___",remove_utf8_bom($pecahparameter[1]));
            $IDPRODUK =	$pecahnilai[0];
            $QTY = $pecahnilai[1];
            $HARGABELI = $pecahnilai[2];
            $result = db_query("SELECT hargapokok,stok FROM product WHERE idproduct='%d'",$IDPRODUK);
            $data = db_fetch_object($result);
            $HARGAPOKOK = $data->hargapokok;
            $STOKSEBELUM = $data->stok;
            $ExpiredDate = $pecahnilai[3];
            $barcodeBatch = create_barcode_batch();
            //idpembelian, idproduct, jumlah, hargapokok, hargapokok
            db_query("INSERT INTO detailpembelian(idpembelian, idproduct, jumlah, hargapokok, hargapokoksebelumnya,expired_date,barcode_batch) VALUES
			    ('%d', '%d', '%f', '%f','%f','%s','%s')",
                $next_id, $IDPRODUK, $QTY, $HARGABELI, $HARGAPOKOK, $ExpiredDate, $barcodeBatch);
            if ($HARGAPOKOK <> $HARGABELI) {
                db_query("INSERT INTO historyhargabeli (hargasebelum, hargasesudah, uid) VALUES('%f','%f','%d')",
                    $HARGAPOKOK, $HARGABELI, $user->uid);
                db_query("UPDATE product SET hargapokok='%f' WHERE idproduct='%d'", $HARGABELI, $IDPRODUK);
            }
            $STOKSEKARANG = $STOKSEBELUM + $QTY;
            db_query("UPDATE product SET stok='%f' WHERE idproduct='%d'", $STOKSEKARANG, $IDPRODUK);
            $KETERANGAN = 'Pembelian';
            db_query("INSERT INTO transaksistock (idproduk, idpembelian, stocksebelum, masuk, stocksetelah, keterangan) VALUES 
			    ('%d', '%d', '%f', '%f', '%f', '%s')", $IDPRODUK, $next_id, $STOKSEBELUM, $QTY, $STOKSEKARANG, $KETERANGAN);
        }
    }else{
        print 'error';
    }
    exit();
}
function buat_nota($idpembelian){
	if ($idpembelian > 0 AND $idpembelian < 10){
		$no_nota = "F000000".$idpembelian;	
	}elseif ($idpembelian >= 10 AND $idpembelian < 100){
		$no_nota = "F00000".$idpembelian;	
	}elseif ($idpembelian >= 100 AND $idpembelian < 1000){
		$no_nota = "F0000".$idpembelian;	
	}elseif ($idpembelian >= 1000 AND $idpembelian < 10000){
		$no_nota = "F000".$idpembelian;	
	}elseif ($idpembelian >= 10000 AND $idpembelian < 100000){
		$no_nota = "F00".$idpembelian;	
	}elseif ($idpembelian >= 100000 AND $idpembelian < 1000000){
		$no_nota = "F0".$idpembelian;	
	}elseif ($idpembelian >= 1000000){
		$no_nota = "F".$idpembelian;	
	}
	return $no_nota;
}
function cari_produk_pembelian(){
	if ($_POST["katacari"]){
		$KATACARI = '%'.$_POST["katacari"].'%';
		if ($_GET["idsupplier"]){
            $StrSql = "SELECT prod.idproduct, prod.barcode, prod.namaproduct, ps.harga AS hargapokok, prod.stok FROM product_supplier AS ps ";
            $StrSql .= "LEFT JOIN product AS prod ON ps.idproduct = prod.idproduct ";
            $StrSql .= "WHERE (alt_code LIKE '%s' OR barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s') AND ps.idsupplier=%d ";
			$result = db_query($StrSql,$KATACARI,$KATACARI,$KATACARI,$_GET["idsupplier"]);
		}else{
			$result = db_query("SELECT idproduct,namaproduct,hargapokok,stok FROM product WHERE alt_code LIKE '%s' OR barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s'",$KATACARI,$KATACARI,$KATACARI);
		}
		$data = db_fetch_object($result);
		if ($data->idproduct){
			print $data->idproduct.';'.$data->namaproduct.';'.$data->hargapokok.';'.$data->stok;
		}else{
			print "error";
		}
	}
	exit();	
}
function cari_barang_pembelian(){
	if ($_GET["term"]){
		$KATACARI = '%'.$_GET["term"].'%';
		if (isset($_GET["idsupplier"]) && !empty($_GET["idsupplier"]) && $_GET["idsupplier"] != 'null'){
		    $StrSql = "SELECT prod.barcode, prod.namaproduct FROM product_supplier AS ps ";
            $StrSql .= "LEFT JOIN product AS prod ON ps.idproduct = prod.idproduct ";
            $StrSql .= "WHERE (alt_code LIKE '%s' OR barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s') AND ps.idsupplier=%d ";
            $StrSql .= "LIMIT 50 ";
			$result = db_query($StrSql,$KATACARI,$KATACARI,$KATACARI,$_GET["idsupplier"]);
		}else{
			$result = db_query("SELECT barcode, namaproduct FROM product WHERE alt_code LIKE '%s' OR barcode LIKE '%s' OR UPPER(namaproduct) LIKE '%s' LIMIT 50",$KATACARI,$KATACARI,$KATACARI);
		}
		$items = array();
		while ($data = db_fetch_object($result)){
			$items[] = array('value' => $data->namaproduct,'barcode' => $data->barcode,'alt_code' => $data->alt_code);
		}
		print json_encode($items);
	}
	exit();	
}
function view_kasir_pembelian(){
	$kasirtabel ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_kasir">';
	$kasirtabel .= '<thead>';
	$kasirtabel .= '<tr>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th>PRODUK</th>';
	$kasirtabel .= '<th>HARGA</th>';
	$kasirtabel .= '<th class="qty">QTY</th>';
	$kasirtabel .= '<th class="subtotal">SUBTOTAL</th>';
	$kasirtabel .= '<th class="qty">STOK</th>';
    $kasirtabel .= '<th class="tanggal">EXPIRE</th>';
    $kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '</tr>';
	$kasirtabel .= '</thead>';
	$kasirtabel .= '<tbody>';
	$kasirtabel .= '</tbody>';
	$kasirtabel .= '</table>';
	return $kasirtabel;
}
function view_tabel_pembelian_old(){
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
	if ($_POST["tgl1"] AND $_POST["tgl2"]){
		$tgl1 = $_POST["tgl1"];
		$tgl2 = $_POST["tgl2"];
	}else{
		$tgl1 = date("Y-m-").'01';
		$tgl2 = date("Y-m-d");	
	}
	if ($_POST["pilihantampil"] == 'NOTA'){
		$nota = 'selected="selected"';
		$tabeltampil = tabel_pembelian($tgl1,$tgl2);
		$urutan = 1; 
	}elseif ($_POST["pilihantampil"] == 'PRODUK'){
		$produk = 'selected="selected"';
		$tabeltampil = pembelian_produk($tgl1,$tgl2);
		$urutan = 2;
	}else{
		$tabeltampil = tabel_pembelian($tgl1,$tgl2);
		$nota = 'selected="selected"';
		$urutan = 1;
	}
	drupal_add_css('misc/media/datatables/css/demo_table_jui.css');
	drupal_add_css('misc/media/datatables/css/TableTools.css');
	drupal_add_css('misc/media/datatables/themes/smoothness/jquery-ui-1.8.4.custom.css');
	drupal_add_css('misc/media/datatables/css/ColVis.css');
	_addJqueryPlugins();
	drupal_add_js('misc/media/jquery-ui-1.8.custom.min.js');
	_addTimePickerPlugins();
	_addJeditablePlugins();
	_addAutotabPlugins();
	drupal_add_js('misc/media/datatables/js/jquery.dataTables.min.js');
	drupal_add_js('misc/media/datatables/js/ColVis.min.js');
	drupal_add_js('misc/media/datatables/ZeroClipboard/ZeroClipboard.js');
	drupal_add_js('misc/media/datatables/js/TableTools.min.js');
	drupal_add_js(
		array(
			'urutan' => $urutan,
		), 
	'setting');
	$js_path = drupal_get_path('module','pembelian').'/js/tabel.pembelian.js';
	drupal_add_js($js_path);
	$pilihantampil = '<select id="pilihantampil" name="pilihantampil">';
	$pilihantampil .= '<option '.$nota.'>NOTA</option>';
	$pilihantampil .= '<option '.$produk.'>PRODUK</option>';
	$pilihantampil .= '</select>';
	$pilihperiode = '<form action="'.base_path().'pembelian/viewpembelian" method="post"><div id="pilihanperiode"><label>PILIH PERIODE PEMBELIAN </label><input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.date('d-m-Y', strtotime($tgl1)).'">';
	$pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.date('d-m-Y', strtotime($tgl2)).'">'.$pilihantampil.'<button>LIHAT PEMBELIAN</button></div></form>';
	$viewpembelian = $pilihperiode.'<div id="viewpembelian">'.$tabeltampil.'</div>';
	$viewpembelian .= '<div id="dialogdetail" title="DETAIL PEMBELIAN"></div>';
	return $viewpembelian;
}

function view_tabel_pembelian(){
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    get_number_format($currencySym, $thousandSep, $decimalSep);
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis();
    $DecDigit = $dataPremis->decimal_digit;
    $SelectedTypePelanggan = 0;
    if (isset($_POST["type_pelanggan"])){
        $SelectedTypePelanggan = $_POST["type_pelanggan"];
    }
    if ($_POST["tgl1"] AND $_POST["tgl2"]){
        $tgl1 = $_POST["tgl1"];
        $tgl2 = $_POST["tgl2"];
    }else{
        $tgl1 = date("Y-m-d");
        $tgl2 = date("Y-m-d");
    }
    $addedFilter = '';
    $filterId = 0;
    if ($_POST["pilihantampil"] == 'NOTA'){
        $nota = 'selected="selected"';
        $idSupplier = isset($_POST['idsupplier']) ? $_POST['idsupplier'] : 0;
        $tabeltampil = tabel_pembelian($tgl1,$tgl2, $idSupplier);
        $urutan = 1;
        $tampilData = 0;
        $addedFilter = createSupplierSelection($idSupplier,'ALL', false).'<input type="hidden" id="idsupplier" name="idsupplier" value="0">';
        if(isset($_POST['idsupplier'])){
            $filterId = $_POST['idsupplier'];
        }
    }elseif ($_POST["pilihantampil"] == 'PRODUK'){
        $produk = 'selected="selected"';
        $idSupplier = isset($_POST['idsupplier']) ? $_POST['idsupplier'] : 0;
        $tabeltampil = pembelian_produk($tgl1,$tgl2,$idSupplier);
        $urutan = 3;
        $tampilData = 1;
        $addedFilter = createSupplierSelection($idSupplier,'ALL',false).'<input type="hidden" id="idsupplier" name="idsupplier" value="0">';
        if (isset($_POST['idsupplier'])){
            $filterId = $_POST['idsupplier'];
        }
    }else{
        $idSupplier = isset($_POST['idsupplier']) ? $_POST['idsupplier'] : 0;
        $tabeltampil = tabel_pembelian($tgl1,$tgl2,$idSupplier);
        $nota = 'selected="selected"';
        $urutan = 1;
        $tampilData = 0;
        $addedFilter = createSupplierSelection($idSupplier,'ALL', false).'<input type="hidden" id="idsupplier" name="idsupplier" value="0">';
        if(isset($_POST['idsupplier'])){
            $filterId = $_POST['idsupplier'];
        }
    }
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
    $arrayhari = arrayHari();
    drupal_add_js(
        array(
            'urutan' => $urutan,
            'tampilData' => $tampilData,
            'tglAwal' => $tgl1,
            'tglAkhir' => $tgl2,
            'filterId' => $filterId,
            'namahari' => $arrayhari,
            'type_pelanggan' => $SelectedTypePelanggan,
            'gstSym' => $gstSym,
            'dDigit' => $DecDigit,
        ),
        'setting');
    drupal_add_js('misc/media/datatables/js/number.format.js');
    $modulePath = drupal_get_path('module','pembelian');
    $jsPath = $modulePath.'/js/tabel.pembelian.js';
    drupal_add_js($jsPath);
    $TypePelanggan = '<select id="type_pelanggan" name="type_pelanggan">';
    if ($SelectedTypePelanggan == 0){
        $SelectedExternal = 'selected';
        $SelectedInternal = '';
    }else{
        $SelectedExternal = '';
        $SelectedInternal = 'selected';
    }
    $TypePelanggan .= '<option value="0" '.$SelectedExternal.'>External</option>';
    $TypePelanggan .= '<option value="1" '.$SelectedInternal.'>Internal</option>';
    $TypePelanggan .= '</select>';
    $pilihantampil = '<select id="pilihantampil" name="pilihantampil">';
    $pilihantampil .= '<option '.$nota.'>NOTA</option>';
    $pilihantampil .= '<option '.$produk.'>PRODUK</option>';
    $pilihantampil .= '</select>';
    $pilihperiode = '<form action="'.base_path().'pembelian/viewpembelian" method="post"><div id="pilihanperiode"><label>PILIH PERIODE PEMBELIAN </label><input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.$tgl1.'">';
    $pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.$tgl2.'">'.$pilihantampil.'&nbsp;'.$addedFilter.$TypePelanggan.'<button>LIHAT PEMBELIAN</button></div></form>';
    $viewpembelian = $pilihperiode.'<div id="viewpembelian">'.$tabeltampil.'</div>';
    $viewpembelian .= '<div id="dialogdetail" title="DETAIL PEMBELIAN">';
    $viewpembelian .= '<div id="table-detail-wrapper"></div>';
    $viewpembelian .= '<div id="add-new-item">';
    $viewpembelian .= '<h5 class="no-margin">ADD NEW ITEM</h5>';
    $inputProduct = '<input type="text" id="new-product" name="new-product" size="20"><input type="text" id="expired-date" name="expired-date" size="10" placeholder="expired">';
    $inputProduct .= '<input type="hidden" id="idproduct" name="idproduct">';
    $viewpembelian .= '<label>Product</label>'.$inputProduct;
    $inputHarga = '<input type="text" id="harga-view" name="harga-view" size="10" readonly="readonly">';
    $inputHarga .= '<input type="hidden" id="hargapokok" name="hargapokok">';
    $inputSubtotal = '<input type="text" id="subtotal-view" name="subtotal-view" size="14" readonly="readonly">';
    $inputQty = '<input type="text" id="qty-new" name="qty-new" size="6">';
    $viewpembelian .= $inputHarga.'<label class="small-label">Qty</label>'.$inputQty.$inputSubtotal;
    $buttonAddNew = '<button id="add-new-button">Add Item</button>';
    $buttonPrintSlip = '<button id="print-slip">Print Slip</button>';
    $viewpembelian .= $buttonAddNew.'</div>'.$buttonPrintSlip;
    $viewpembelian .= '</div>';
    return $viewpembelian;
}

function tabel_pembelian()
{
    //nonota, tglpembelian, idpemakai, idlangganan, total,
    //totalmodal, carabayar, bayar, kembali
    $tabelpembelian = '<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_pembelian">';
    $tabelpembelian .= '<thead>';
    $tabelpembelian .= '<tr>';
    $tabelpembelian .= '<th class="tablebutton"></th>';
    $tabelpembelian .= '<th class="tablebutton"></th>';
    $tabelpembelian .= '<th>NO. NOTA</th>';
    $tabelpembelian .= '<th class="tanggal">HARI</th>';
    $tabelpembelian .= '<th class="tanggal">TGL</th>';
    $tabelpembelian .= '<th class="tanggal">JAM</th>';
    $tabelpembelian .= '<th>TOTAL</th>';
    $tabelpembelian .= '<th>CARA BAYAR</th>';
    $tabelpembelian .= '<th>BAYAR</th>';
    $tabelpembelian .= '<th>KEMBALI</th>';
    $tabelpembelian .= '<th>SUPPLIER</th>';
    $tabelpembelian .= '<th>KASIR</th>';
    $tabelpembelian .= '<th class="tablebutton"></th>';
    $tabelpembelian .= '</tr>';
    $tabelpembelian .= '</thead>';
    $tabelpembelian .= '<tbody>';
    $tabelpembelian .= '</tbody>';
    $tabelpembelian .= '<tfoot>';
    $tabelpembelian .= '<tr>';
    $tabelpembelian .= '<th style="text-align:right" colspan="6">Total:&nbsp;&nbsp;</th>';
    $tabelpembelian .= '<th style="text-align: right">&nbsp;&nbsp;</th>';
    $tabelpembelian .= '<th style="text-align: right">&nbsp;&nbsp;</th>';
    $tabelpembelian .= '<th style="text-align: right">&nbsp;&nbsp;</th>';
    $tabelpembelian .= '<th style="text-align: right">&nbsp;&nbsp;</th>';
    $tabelpembelian .= '<th colspan="3">&nbsp;</th>';
    $tabelpembelian .= '</tr>';
    $tabelpembelian .= '</tfoot>';
    $tabelpembelian .= '</table>';
    return $tabelpembelian;
}

function detail_pembelian(){
	if ($_POST["idpembelian"]){
		$tabelpembelian ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_pembelian">';
		$tabelpembelian .= '<thead>';
		$tabelpembelian .= '<tr>';
        $tabelpembelian .= '<th class="center tablebutton">&nbsp;</th>';
		$tabelpembelian .= '<th>BARCODE</th>';
		$tabelpembelian .= '<th>PRODUK</th>';
		$tabelpembelian .= '<th class="qty">QTY</th>';
		$tabelpembelian .= '<th>H.MODAL</th>';
		$tabelpembelian .= '<th>SUBTOTAL</th>';
        $tabelpembelian .= '<th class="center tablebutton">&nbsp;</th>';
		$tabelpembelian .= '</tr>';
		$tabelpembelian .= '</thead>';
		$tabelpembelian .= '<tbody>';
		$tabelpembelian .= '</tbody>';
        $tabelpembelian .= '<tfoot>';
        $tabelpembelian .= '<tr>';
        $tabelpembelian .= '<th class="right" colspan="5">TOTAL</th>';
        $tabelpembelian .= '<th style="text-align: right">&nbsp;</th>';
        $tabelpembelian .= '<th>&nbsp;</th>';
        $tabelpembelian .= '</tr>';
        $tabelpembelian .= '</tfoot>';
		$tabelpembelian .= '</table>';
		print $tabelpembelian;
	}
	exit();	
}
function pembelian_produk($tgl1,$tgl2)
{
    $tgl1 = date('Y-m-d', strtotime($tgl1)) . ' 00:00';
    $tgl2 = date('Y-m-d', strtotime($tgl2)) . ' 23:59';
    $tabelpembelian = '<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_pembelian">';
    $tabelpembelian .= '<thead>';
    $tabelpembelian .= '<tr>';
    $tabelpembelian .= '<th>BARCODE</th>';
    $tabelpembelian .= '<th>PRODUK</th>';
    $tabelpembelian .= '<th>SUPPLIER</th>';
    $tabelpembelian .= '<th class="qty">QTY</th>';
    $tabelpembelian .= '<th>H.MODAL MIN</th>';
    $tabelpembelian .= '<th>H.MODAL MAX</th>';
    $tabelpembelian .= '<th>SUBTOTAL</th>';
    $tabelpembelian .= '</tr>';
    $tabelpembelian .= '</thead>';
    $tabelpembelian .= '<tbody>';
    $tabelpembelian .= '</tbody>';
    $tabelpembelian .= '<tfoot>';
    $tabelpembelian .= '<tr>';
    $tabelpembelian .= '<th style="text-align:right" colspan="6">Total:&nbsp;&nbsp;</th>';
    $totalmodal = 0;
    $tabelpembelian .= '<th style="text-align: right">Rp. ' . number_format($totalmodal, 0, ",", ".") . '&nbsp;&nbsp;</th>';
    $tabelpembelian .= '</tr>';
    $tabelpembelian .= '</tfoot>';
    $tabelpembelian .= '</table>';
    return $tabelpembelian;
}

function DateAdd($v,$d=null , $f="m/d/Y"){ 
 /*
 To Use
 $TGLHITUNG = date("m/d/Y");
 $TGLAKHIRHITUNG = DateAdd(30,$TGLHITUNG);
 */	
 $d=($d?$d:date("Y-m-d")); 
 return date($f,strtotime($v." days",strtotime($d))); 
}