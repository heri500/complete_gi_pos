<?php

function laundry(){
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
	_addChosenJqueryPlugins();
	$tglsekarang = date("Y-m-d");
	$tgltampil = date("d-m-Y");
	$js_path = drupal_get_path('module','penjualan').'/js/laundry.form.js';
	drupal_add_js($js_path);
	if (isset($_GET['tanggal'])){
		$tanggal = $_GET['tanggal'];
		$tanggal = date('d-m-Y', strtotime($tanggal));
		$tanggalkirim = date('Y-m-d', strtotime($tanggal));
	}else{
		$tanggal = date('d-m-Y');
		$tanggalkirim = date('Y-m-d');
	}
	$panduantombol = '<a style="font-weight: bold;text-align: center;font-size:12px;margin:0;padding: .4em .3em;">Ctrl+Shift+</a>';
	$tombolcari = '<a id="tombolubahharga" class="cari" onclick="ubahharga()">F2:UBAH HARGA</a>';
	$tombolqty = '<a id="tombolubahqty" onclick="ubahqty()" class="buttonqty">F3:UBAH QTY</a>';
	$tombolhapus = '<a class="buttonhapus" onclick="hapus_latest_produk()">F4:HAPUS ITEM</a>';
	$tombolselesai = '<a class="buttonselesai" onclick="kirim_data()">F5:SIMPAN</a>';
	//$tombolprint = '<a class="buttonprint" onclick="kirim_data()">F6:BAYAR&PRINT</a>';
	//$tomboltutupkasir = '<a class="buttontutup">F7:TUTUP KASIR</a>';
	$tombollihatomset = '<a class="buttonomset" onclick="hitung_omset();">F8:OMSET</a>';
	$penjualan = '<button onclick="munculkankasir();" style="font-size:12px;border: 1px solid orangered;">Klik untuk memunculkan kasir</button>';
	$penjualan .= '<div id="dialogkasir" style="font-size: 12px;" title="GLOBAL POS LAUNDRY MASUK ['.$tanggal.']">';
	$penjualan .= '<div id="info-kasir-waktu">';
	$penjualan .= '<div id="tempattanggalkasir">';
	$penjualan .= '<div id="tempattanggal">';
	$penjualan .= '<label>TANGGAL</label><input readonly="readonly" type="text" id="tgljual" name="tgljual" value="'.$tanggal.'">';
	$penjualan .= '<input type="hidden" id="tgljualkirim" name="tgljualkirim" value="'.$tanggalkirim.'">';
	$penjualan .= '</div>';
	$penjualan .= '<div id="tempatnamakasir">';
	global $user;
	$penjualan .= '<label>KASIR</label><input readonly="readonly" type="text" id="kasir" name="kasir" value="'.$user->name.'">';
	$penjualan .= '</div>';
	$penjualan .= '</div>';
	$penjualan .= '<div id="tempatjam" align="center">';
	$penjualan .= 'Jam';
	$penjualan .= '</div>';
	$penjualan .= '<div id="tempatkosong">';
	$penjualan .= '<div id="tempatsupplier"><label>PELANGGAN</label></div>';
	$pilihpelanggan = '<select id="idpelanggan" name="idpelanggan" onchange="inisialulang()">';
	if ($_GET["idpelanggan"] == 0){
		$pilihpelanggan .= '<option selected value="0">UMUM</option>';	
	}else{
		$pilihpelanggan .= '<option value="0">UMUM</option>';	
	}
	$resultpelanggan = db_query("SELECT idpelanggan,kodepelanggan,namapelanggan,telp FROM pelanggan");
	while ($datapelanggan = db_fetch_object($resultpelanggan)){
		if ($_GET["idpelanggan"] == $datapelanggan->idpelanggan){
			$pilihpelanggan .= '<option selected value="'.$datapelanggan->idpelanggan.'">'.$datapelanggan->kodepelanggan.' - '.$datapelanggan->namapelanggan.' ['.$datapelanggan->telp.']</option>';	
		}else{
			$pilihpelanggan .= '<option value="'.$datapelanggan->idpelanggan.'">'.$datapelanggan->kodepelanggan.' - '.$datapelanggan->namapelanggan.' ['.$datapelanggan->telp.']</option>';	
		}
	}
	$pilihpelanggan .= '</select>';
	$penjualan .= '<div id="tempatsupplier2">'.$pilihpelanggan.' <img id="add-pelanggan" src="'.base_path().'misc/media/images/add-small.ico" style="margin: 4px;cursor:pointer;"></div>';
	$penjualan .= '</div>';
	$penjualan .= '</div>';
	$penjualan .= '<div id="barcode_place">SCAN ATAU INPUT BARCODE, KODE ATAU NAMA PRODUK : <input type="text" id="barcode" name="barcode" class="barcodekasir"></div>';
	$penjualan .= '<div id="tempattotal"><div id="totalbelanja">Total Laundry : Rp. 0</div></div>';
	$penjualan .= '<div id="tempattabelkasir">'.view_kasir_laundry().'</div>';
	$penjualan .= '<input type="hidden" id="last_id" name="last_id">';
	$penjualan .= '<input type="hidden" id="lastqty" name="lastqty" value="1">';
	$penjualan .= '<input type="hidden" id="lastharga" name="lastharga">';
	$penjualan .= '<input type="hidden" id="lastdiskon" name="lastdiskon">';
	$penjualan .= '<input type="hidden" id="lastperkiraan" name="lastperkiraan">';
	$penjualan .= '<input type="hidden" id="nilaikirim" name="nilaikirim">';
	$penjualan .= '<div id="tempattombolkasir">';
	$penjualan .= $panduantombol;
	$penjualan .= $tombolcari;
	$penjualan .= $tombolqty;
	$penjualan .= $tombolhapus;
	$penjualan .= $tombolselesai;
	//$penjualan .= $tombolprint;
	//$penjualan .= $tomboltutupkasir;
	$penjualan .= $tombollihatomset;
	$penjualan .= '</div>';
	$penjualan .= '</div>';
	$penjualan .= '<div title="Informasi" id="dialogwarning">';
	$penjualan .= '<p id="pesantext">Produk yang dicari tidak ada, Silahkan masukkan Barcode/Kode Produk/Nama Produk yang lain...!!!</p>';
	$penjualan .= '<p style="text-align: center"><button id="tutupdialog" class="buttonwarning" onclick="tutupwarning();">CLOSE</button></p>';
	$penjualan .= '</div>';
	$penjualan .= '<div title="UBAH HARGA" id="dialogubahharga">';
	$penjualan .= '<label>Harga Baru : </label><input type="text" id="newharga" name="newharga" value="1">';
	$penjualan .= '</div>';
	$penjualan .= '<div title="UBAH QTY" id="dialogubahqty">';
	$penjualan .= '<label>Qty Baru : </label><input type="text" id="newqty" name="newqty" value="1">';
	$penjualan .= '</div>';
	$penjualan .= '<div title="UBAH QTY" id="dialogubahqty2">';
	$penjualan .= '<label>Qty Baru : </label><input type="text" id="newqty2" name="newqty2">';
	$penjualan .= '</div>';
	$penjualan .= '<div title="PEMBAYARAN" id="dialogbayar">';
	$carabayar = '<select id="carabayar" name="carabayar">';
	$result = db_query("SELECT carabayar FROM carabayartitipanlaundry");
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
	//Pelanggan
	/*$pelanggan = '<select id="idpelanggan" name="idpelanggan">';
	$result = db_query("SELECT idpelanggan,namapelanggan FROM pelanggan");
	$i = 0;
	while($data = db_fetch_object($result)){
		if ($i == 0){
			$pelanggan .= '<option selected="selected" value="'.$data->idpelanggan.'">'.$data->namapelanggan.'</option>';	
		}else{
			$pelanggan .= '<option value="'.$data->idpelanggan.'">'.$data->namapelanggan.'</option>';	
		}
		$i++;
	}
	$pelanggan .= '</select>';*/
	$nilaibayar = '<input type="text" id="nilaibayar" name="nilaibayar">';
	$depositpelanggan = '<input type="text" id="depositpelanggan" name="depositpelanggan" readonly="readonly">';
	$nokartudebit = '<input type="text" id="nomerkartu" name="nomerkartu">';
	$kembali = '<input readonly="readonly" type="text" id="kembali" name="kembali">';
	$keterangan = '<textarea id="keterangan" name="keterangan" style="width: 300px;"></textarea>';
	$totalbelanja = '<input type="text" id="totalbelanjauser" name="totalbelanjauser">';
	//$penjualan .= '<div class="barisbayar"><label>Pelanggan</label>'.$pelanggan.'</div>';
	$penjualan .= '<div class="barisbayar"><label>Total Laundy</label>'.$totalbelanja.'</div>';
	$penjualan .= '<div class="barisbayar"><label id="label-deposit">Deposit</label>'.$depositpelanggan.'</div>';
	$penjualan .= '<div class="barisbayar"><label>Cara Bayar</label>'.$carabayar.'</div>';
	$penjualan .= '<div id="field_no_kartu" class="barisbayar" style="display: none;"><label>No. Kartu/Giro</label>'.$nokartudebit.'</div>';
	$penjualan .= '<div id="field_bayar" class="barisbayar"><label>Pembayaran</label>'.$nilaibayar.'</div>';
	$penjualan .= '<div class="barisbayar"><label>Keterangan</label>'.$keterangan.'</div>';
	//$penjualan .= '<div id="field_kembali" class="barisbayar" style="display: none;"><label>Kembali</label>'.$kembali.'</div>';
	$penjualan .= '<div class="barisbayar" style="text-align: center;">';
	$penjualan .= '<button id="simpan-cetak-laundry" style="margin-top: 10px;font-size: 0.8em;">SIMPAN & PRINT</button>&nbsp;&nbsp;';
	$penjualan .= '<button id="simpan-laundry" style="margin-top: 10px;font-size: 0.8em;">SIMPAN</button>';
	$penjualan .= '</div>';
	$penjualan .= '</div>';
	//Form Tambah Pelanggan
	$formPelanggan = '<div title="TAMBAH PELANGGAN" id="dialogtambahpelanggan" class="form-add-dialog">';
	$formPelanggan .= '<div class="form-row"><label>KODE</label><input type="text" class=\"validate[required]\" id="kodepelanggan" name="kodepelanggan"></div>';
	$formPelanggan .= '<div class="form-row"><label>PELANGGAN</label><input class=\"validate[required]\" type="text" id="namapelanggan" name="namapelanggan" style="width: 300px;"></div>';
	$formPelanggan .= '<div class="form-row"><label>TELEPON</label><input type="text" id="telp" name="telp"></div>';
	$formPelanggan .= '<div class="form-row"><label>ALAMAT</label><input type="text" id="alamat" name="alamat" style="width: 350px;"></div>';
	$formPelanggan .= '<div class="form-row"><button id="save-pelanggan" style="font-size: 12px;">Simpan</button></div>';
	$formPelanggan .= '</div>';
	return $penjualan.$formPelanggan;
}

function simpan_laundry(){
	//nonota, idpemakai, total, bayar, kembali
	if (isset($_POST["detail_produk"]) AND isset($_POST["totalbelanja"])){
		$result = db_query("SELECT idtitipanlaundry FROM titipanlaundry ORDER BY idtitipanlaundry DESC LIMIT 1");
		$data = db_fetch_object($result);
		if ($data->idtitipanlaundry > 0){
			$next_id = $data->idtitipanlaundry + 1;
		}else{
			$next_id = 1;
		}
		$no_nota = buat_nota_laundry($next_id);
		$carabayar = $_POST["carabayar"];
		$nokartu = $_POST["nomerkartu"];
		date_default_timezone_set('Asia/Jakarta');
		$waktujual = $_POST["tgljual"]." ".date("H:i:s");
		$splitTanggal = explode('-', $_POST["tgljual"]);
		$splitJam = explode(':',date("H:i:s"));
		$intTanggal = mktime($splitJam[0],$splitJam[1],$splitJam[2],$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
		global $user;
		db_query("INSERT INTO titipanlaundry (nonota, idpemakai, total, carabayar, bayar, nokartu, tglpenjualan, idpelanggan, keterangan) 
		VALUES ('%s', '%d', '%f', '%s', '%f', '%s', '%s', '%d', '%s')",
		$no_nota, $user->uid, $_POST["totalbelanja"], $carabayar, $_POST["bayar"], $nokartu, $waktujual,$_POST["idpelanggan"],$_POST["keterangan"]);
		$next_id = db_last_insert_id('titipanlaundry', 'idtitipanlaundry');
		print $next_id;
		if (isset($_POST["bayar"])){
			if ((int)$_POST["bayar"] > 0){
				$variables['idpelanggan'] = $_POST["idpelanggan"];
				$variables['hutang'] = getTotalHutang($_POST["idpelanggan"]);
				$variables['pembayaran'] = $_POST["bayar"];
				$variables['tglbayar'] = $waktujual;
				$variables['idtitipanlaundry'] = $next_id;
				$variables['keterangan'] = 'Deposit Uang Untuk Nota Laundry : '.$no_nota.' Tanggal : '.$waktujual;
				pembayaranhutang($variables);
			}
		}
		$pecahdetail = explode("&",$_POST["detail_produk"]);
		$totalmodal = 0;
		foreach ($pecahdetail as $parameter){
			$pecahparameter = explode("=",$parameter);
			$pecahnilai = explode("___",$pecahparameter[1]);
			if (is_string($pecahnilai[0])){
				$IDPRODUK =	substr($pecahnilai[0],3);
			}else if(is_integer($pecahnilai[0])){
				$IDPRODUK =	$pecahnilai[0];
			}
			$QTY = $pecahnilai[1];
			$HARGAJUAL = $pecahnilai[2];
			$DISKON = $pecahnilai[3];
			$result = db_query("SELECT hargapokok,stok FROM product WHERE idproduct='%d'",$IDPRODUK);	
			$data = db_fetch_object($result);
			$HARGAPOKOK = $data->hargapokok;
			$totalmodal = $totalmodal + ($HARGAPOKOK*$QTY);
			db_query("INSERT INTO detaillaundry(idtitipanlaundry, idproduct, jumlah, hargapokok, hargajual, diskon, sisa, masuk, perkiraan_ambil) VALUES
			('%d', '%d', '%f', '%f', '%f', '%f', '%f','%d','%d')",
			$next_id,$IDPRODUK,$QTY,$HARGAPOKOK,$HARGAJUAL,$DISKON,$QTY,$intTanggal,$pecahnilai[4]);
		}
		db_query("UPDATE titipanlaundry SET totalmodal='%f' WHERE idtitipanlaundry='%d'",$totalmodal,$next_id);
	}else{
		print 'error';	
	}
	exit();
}
function buat_nota_laundry($idtitipanlaundry){
	if ($idtitipanlaundry > 0 AND $idtitipanlaundry < 10){
		$no_nota = "Li000000".$idtitipanlaundry;	
	}elseif ($idtitipanlaundry >= 10 AND $idtitipanlaundry < 100){
		$no_nota = "Li00000".$idtitipanlaundry;	
	}elseif ($idtitipanlaundry >= 100 AND $idtitipanlaundry < 1000){
		$no_nota = "Li0000".$idtitipanlaundry;	
	}elseif ($idtitipanlaundry >= 1000 AND $idtitipanlaundry < 10000){
		$no_nota = "Li000".$idtitipanlaundry;	
	}elseif ($idtitipanlaundry >= 10000 AND $idtitipanlaundry < 100000){
		$no_nota = "Li00".$idtitipanlaundry;	
	}elseif ($idtitipanlaundry >= 100000 AND $idtitipanlaundry < 1000000){
		$no_nota = "Li0".$idtitipanlaundry;	
	}elseif ($idtitipanlaundry >= 1000000){
		$no_nota = "Li".$idtitipanlaundry;	
	}
	return $no_nota;
}
function view_kasir_laundry(){
	$kasirtabel ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_kasir">';
	$kasirtabel .= '<thead>';
	$kasirtabel .= '<tr>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th class="tablebutton"></th>';
	$kasirtabel .= '<th>PRODUK</th>';
	$kasirtabel .= '<th>HARGA</th>';
	$kasirtabel .= '<th class="diskon2">%</th>';
	$kasirtabel .= '<th class="diskon2">QTY</th>';
	$kasirtabel .= '<th class="subtotal">SUBTOTAL</th>';
	$kasirtabel .= '<th>AMBIL</th>';
	$kasirtabel .= '</tr>';
	$kasirtabel .= '</thead>';
	$kasirtabel .= '<tbody>';
	$kasirtabel .= '</tbody>';
	$kasirtabel .= '</table>';
	return $kasirtabel;
}
function laundry_keluar(){
	/*$path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    //$variables['styles'] = drupal_get_css();
    drupal_add_css('misc/media/jquery-ui/css/smoothness/jquery-ui-1.10.4.custom.css');
    drupal_add_js('misc/media/jquery-ui/js/jquery-1.10.2.js');
    drupal_add_js('misc/media/jquery-ui/js/jquery-ui-1.10.4.custom.js');*/
	$path = drupal_get_path('theme', 'cti_flex');
	$form_style = $path.'/css/custom-style.css';
	drupal_add_css($form_style, 'theme', 'all', FALSE);
	$variables['styles'] = drupal_get_css();
	drupal_add_css('misc/media/themes/jquery-ui-1.8.11.custom.css');
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	drupal_add_js('misc/media/jquery-ui-1.8.11.custom.min.js');
	$js_path = drupal_get_path('module','penjualan').'/js/laundrykeluar.form.js';
	drupal_add_js($js_path);
	$labelData = '<div>Masukkan nomer nota laundry/nama pelanggan</div>';
	$returnForm = $labelData.'<input type="text" id="no-nota" name="no-nota" style="width: 275px;font-size: 18px;">';
	return $returnForm;
}
function cari_nota_laundry(){
	if ($_GET["term"]){
		$KATACARI = '%'.strtolower($_GET["term"]).'%';
		$result = db_query("SELECT titipan.idtitipanlaundry, titipan.nonota, titipan.tglpenjualan,
        titipan.total,plg.namapelanggan,plg.telp FROM titipanlaundry AS titipan LEFT JOIN
        pelanggan AS plg ON titipan.idpelanggan=plg.idpelanggan WHERE
        LOWER(titipan.nonota) LIKE '%s' OR LOWER(plg.namapelanggan) LIKE '%s'
        AND titipan.status_laundry < 2
        ORDER BY titipan.tglpenjualan DESC LIMIT 50",$KATACARI,$KATACARI);
		$items = array();
		while ($rowDb = db_fetch_object($result)){
			$valueData = $rowDb->nonota.' - '.$rowDb->namapelanggan;
			if (!empty($rowDb->telp)){
				$valueData .= ' - '.$rowDb->telp;
			}
			$valueData .= ' - '.number_format($rowDb->total,0,',','.');
			$items[] = array(
				'id'    => $rowDb->idtitipanlaundry,
				'label' => $valueData,
				'value' => $valueData,
			);
		}
		print json_encode($items);
	}
	exit();
}