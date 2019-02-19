<?php

function customerorder(){
    get_number_format($currencySym, $thousandSep, $decimalSep);
    createLogoImagePath();
    $gstSym = get_gst_symbols();
    $dataPremis = get_data_premis(false);
    $DecDigit = $dataPremis->decimal_digit;
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    $variables['styles'] = drupal_get_css();
    drupal_add_css('misc/media/datatables/css/demo_table_jui.css');
    drupal_add_css('misc/media/themes/jquery-ui-1.8.11.custom.css');
    drupal_add_css('misc/media/css/validationEngine.jquery.css');
    //drupal_add_js('misc/media/jquery-1.4.4.min.js');
    _addJqueryPlugins();
    drupal_add_js('misc/media/jqClock.min.js');
    drupal_add_js('misc/media/jquery.validationEngine-en.js');
    drupal_add_js('misc/media/jquery.validationEngine.js');
    drupal_add_js('misc/media/jquery-ui-1.8.11.custom.min.js');
    drupal_add_js('misc/media/datatables/js/jquery.dataTables.min.js');
    drupal_add_js('misc/media/datatables/js/dataTables.addtr.js');
    drupal_add_js('misc/media/datatables/js/number.format.js');
    drupal_add_js('misc/media/jquery.autotab-1.1b.js');
    _add_chosen_jquery_plugins_old();
    $tglsekarang = date("Y-m-d");
    $tgltampil = date("d-m-Y");
    $DecDigitCashier = variable_get('round_decimal_digit_cashier',0);
    drupal_add_js(
        array(
            'tglsekarang' => $tglsekarang,
            'tgltampil' => $tgltampil,
            'dec_digit' => $DecDigit,
            'dec_digit_cashier' => $DecDigitCashier,
        ),
        'setting');
    $js_path = drupal_get_path('module','penjualan').'/js/customerorder.form.js';
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
    $tombolselesai = '<a class="buttonselesai" onclick="kirim_data(0)">F5:SIMPAN</a>';
    $tombolprint = '<a class="buttonprint" onclick="kirim_data(1)">F6:SIMPAN&PRINT</a>';
    //$tomboltutupkasir = '<a class="buttontutup">F7:TUTUP KASIR</a>';
    $tombollihatpelanggan = '<a class="buttonpelanggan" onclick="pilih_pelanggan();">F9:PELANGGAN</a>';
    $tombollihatomset = '<a class="buttonomset" onclick="hitung_omset();">F8:TOTAL</a>';
    $penjualan = '<button onclick="munculkankasir();" style="font-size:12px;border: 1px solid orangered;">Klik untuk memunculkan kasir</button>';
    $penjualan .= '<div id="dialogkasir" style="font-size: 12px;" title="GLOBAL POS CUSTOMER ORDER ['.$tanggal.']">';
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
    $penjualan .= '<div id="barcode_place">SCAN ATAU INPUT BARCODE, KODE ATAU NAMA PRODUK : <input type="text" id="barcode" name="barcode" class="barcodekasir"><input type="hidden" id="hiddenbarcode" name="hiddenbarcode"></div>';
    $penjualan .= '<div id="tempattabelkasir">'.view_kasir_customerorder().'</div>';
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
    $penjualan .= $tombolprint;
    //$penjualan .= $tombolprint;
    //$penjualan .= $tomboltutupkasir;
    $penjualan .= $tombollihatomset;
    $penjualan .= $tombollihatpelanggan;
    $penjualan .= '</div>';
    $penjualan .= '<div id="tempattotal"><div id="totalbelanja">Total Order : '.$currencySym.' 0</div></div>';
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
    $result = db_query("SELECT carabayar FROM carabayartitipanlaundry ORDER BY weight");
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
    $meja = '<input type="text" id="nomormeja" name="nomormeja">';
    $meja .= '<input type="hidden" id="idmeja" name="idmeja">';
    //$penjualan .= '<div class="barisbayar"><label>Pelanggan</label>'.$pelanggan.'</div>';
    $penjualan .= '<div class="barisbayar"><label>Total Order</label>'.$totalbelanja.'</div>';
    $totalppn = '<input type="hidden" id="ppn_value" name="ppn_value" value="'.$dataPremis->ppn_value.'">';
    $totalppn .= '<input type="text" id="total_ppn" name="total_ppn" value="0" disabled="disabled">';
    $ceklistPpn = '<input type="checkbox" id="use-ppn" name="use-ppn" value="1" checked="checked" class="checkbox-ppn">';
    $penjualan .= '<div class="barisbayar"><label>'.$gstSym.' ('.$dataPremis->ppn_value.'%)&nbsp;'.$ceklistPpn.'</label>'.$totalppn.'</div>';
    $totalplusppn = '<input type="text" id="total_plus_ppn" name="total_plus_ppn" value="0" disabled="disabled">';
    $penjualan .= '<div class="barisbayar"><label>TOTAL + '.$gstSym.'</label>'.$totalplusppn.'</div>';
    $penjualan .= '<div id="baris-deposit" class="barisbayar"><label id="label-deposit">Deposit</label>'.$depositpelanggan.'</div>';
    $penjualan .= '<div class="barisbayar"><label>Cara Bayar</label>'.$carabayar.'</div>';
    $penjualan .= '<div id="field_no_kartu" class="barisbayar" style="display: none;"><label>No. Kartu/Giro</label>'.$nokartudebit.'</div>';
    $penjualan .= '<div id="field_bayar" class="barisbayar"><label>Pembayaran</label>'.$nilaibayar.'</div>';
    //$penjualan .= '<div id="keterangan-wrapper" class="keterangan-div">';
    $penjualan .= '<div class="barisbayar"><label>Keterangan</label>'.$keterangan.'</div>';
    //$penjualan .= '</div>';
    $penjualan .= '<div class="barisbayar"><label>Meja</label>'.$meja.'</div>';
    $penjualan .= '<div id="field_kembali" class="barisbayar" style="display: none;"><label>Kembali</label>'.$kembali.'</div>';
    //$penjualan .= '<div class="barisbayar" style="text-align: center;">';
    //$penjualan .= '<button id="simpan-cetak-customerorder" style="margin-top: 10px;font-size: 0.8em;">SIMPAN & PRINT</button>&nbsp;&nbsp;';
    //$penjualan .= '<button id="simpan-customerorder" style="margin-top: 10px;font-size: 0.8em;">SIMPAN</button>';
    //$penjualan .= '</div>';
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

function simpan_customerorder(){
    global $user;
    //nonota, idpemakai, total, bayar, kembali
    if (isset($_POST["detail_produk"]) && isset($_POST["totalbelanja"]) && isset($_POST['totalbelanjappn'])){
        $result = db_query("SELECT id FROM customer_order ORDER BY id DESC LIMIT 1");
        $data = db_fetch_object($result);
        if ($data->id > 0){
            $next_id = $data->id + 1;
        }else{
            $next_id = 1;
        }
        $no_nota = buat_nota_customerorder($next_id);
        $carabayar = $_POST["carabayar"];
        $nokartu = $_POST["nomerkartu"];
        date_default_timezone_set($user->timezone_name);
        $waktujual = $_POST["tgljual"]." ".date("H:i:s");
        $splitTanggal = explode('-', $_POST["tgljual"]);
        $splitJam = explode(':',date("H:i:s"));
        $intTanggal = mktime($splitJam[0],$splitJam[1],$splitJam[2],$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
        $intTanggal = mktime($splitJam[0],$splitJam[1],$splitJam[2],$splitTanggal[1],$splitTanggal[2],$splitTanggal[0]);
        $tanggalTerakhir = substr(db_result(db_query("SELECT tglorder FROM customer_order ORDER BY id DESC LIMIT 1")),0, 10);
        if ($tanggalTerakhir != $_POST["tgljual"]){
            $OrderNumber = 1;
        }else{
            $LastOrderNumber = db_result(db_query("SELECT idorderpanggil FROM customer_order ORDER BY id DESC LIMIT 1"));
            $OrderNumber = $LastOrderNumber + 1;
        }
        global $user;
        db_query("INSERT INTO customer_order (nonota, idpemakai, total, carabayar, 
        bayar, nokartu, tglorder, idpelanggan, keterangan, idmeja, ppn, total_plus_ppn, idorderpanggil)
		VALUES ('%s', '%d', '%f', '%s', '%f', '%s', '%s', '%d', '%s','%d', '%f', '%f', '%d')",
            $no_nota, $user->uid, $_POST["totalbelanja"], $carabayar, $_POST["bayar"],
            $nokartu, $waktujual,$_POST["idpelanggan"],$_POST["keterangan"],$_POST["idmeja"],
            $_POST["ppn"],$_POST["totalbelanjappn"], $OrderNumber);
        $next_id = db_last_insert_id('customer_order', 'id');
        print $next_id;
        if (isset($_POST["bayar"]) && isset($_POST["idpelanggan"]) && (int)$_POST["idpelanggan"] > 0){
            if ((int)$_POST["bayar"] > 0){
                $variables['idpelanggan'] = $_POST["idpelanggan"];
                $variables['hutang'] = getTotalHutang($_POST["idpelanggan"]);
                $variables['pembayaran'] = $_POST["bayar"];
                $variables['tglbayar'] = $waktujual;
                $variables['idcustomerorder'] = $next_id;
                $variables['keterangan'] = 'Deposit Uang Untuk Nota Customer Order : '.$no_nota.' Tanggal : '.$waktujual;
                pembayaranhutang($variables);
            }
        }
        $pecahdetail = explode("&",$_POST["detail_produk"]);
        $totalmodal = 0;
        foreach ($pecahdetail as $parameter){
            $pecahparameter = explode("=",$parameter);
            $pecahnilai = explode("___",$pecahparameter[1]);
            $IDPRODUK =	$pecahnilai[0];
            $QTY = $pecahnilai[1];
            $HARGAJUAL = $pecahnilai[2];
            $DISKON = $pecahnilai[3];
            $result = db_query("SELECT hargapokok,stok FROM product WHERE idproduct='%d'",$IDPRODUK);
            $data = db_fetch_object($result);
            $HARGAPOKOK = $data->hargapokok;
            $totalmodal = $totalmodal + ($HARGAPOKOK*$QTY);
            $detailbarcode = createEAN13Code(getRandomString(9));
            db_query("INSERT INTO detailcustomerorder(idcustomerorder, idproduct, jumlah,
			hargapokok, hargajual, diskon, sisa, masuk, perkiraan_ambil, detailbarcode,
			outstanding) VALUES ('%d', '%d', '%f', '%f', '%f', '%f', '%f','%d','%d','%s','%d')",
                $next_id,$IDPRODUK,$QTY,$HARGAPOKOK,$HARGAJUAL,$DISKON,$QTY,$intTanggal,$pecahnilai[4],
                $detailbarcode,$QTY);
        }
        db_query("UPDATE customer_order SET totalmodal='%f' WHERE id='%d'",$totalmodal,$next_id);
    }else{
        print 'error';
    }
    exit();
}
function buat_nota_customerorder($id){
    $no_nota = createEAN13Code(getRandomString(9));
    return $no_nota;
}
function view_kasir_customerorder(){
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
function order_keluar(){
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    $variables['styles'] = drupal_get_css();
    drupal_add_css('misc/media/themes/jquery-ui-1.8.11.custom.css');
    drupal_add_js('misc/media/jquery-1.4.4.min.js');
    drupal_add_js('misc/media/jquery-ui-1.8.11.custom.min.js');
    $js_path = drupal_get_path('module','penjualan').'/js/orderkeluar.form.js';
    drupal_add_js($js_path);
    $labelData = '<div>Scan/Masukkan nomer nota order/nama pelanggan</div>';
    $returnForm = $labelData.'<input type="text" id="no-nota" name="no-nota" style="width: 275px;font-size: 18px;">';
    return $returnForm;
}
function cari_nota_order(){
    if ($_GET["term"]){
        $KATACARI = '%'.strtolower($_GET["term"]).'%';
        $result = db_query("SELECT custord.id, custord.nonota, custord.tglorder,
        custord.total,plg.namapelanggan,plg.telp,meja.meja FROM customer_order AS custord
        LEFT JOIN pelanggan AS plg ON custord.idpelanggan=plg.idpelanggan
        LEFT JOIN meja AS meja ON custord.idmeja = meja.id
        WHERE LOWER(custord.nonota) LIKE '%s' OR LOWER(plg.namapelanggan) LIKE '%s'
        AND custord.status_order < 2
        ORDER BY custord.tglorder DESC LIMIT 50",$KATACARI,$KATACARI);
        $items = array();
        while ($rowDb = db_fetch_object($result)){
            if (!empty($rowDb->namapelanggan)) {
                $valueData = $rowDb->nonota . ' - ' . $rowDb->namapelanggan;
            }else if (!empty($rowDb->meja)){
                $valueData = $rowDb->nonota . ' - ' . $rowDb->meja;
            }else{
                $valueData = $rowDb->nonota;
            }
            if (!empty($rowDb->telp)){
                $valueData .= ' - '.$rowDb->telp;
            }
            $valueData .= ' - '.number_format($rowDb->total,0,',','.');
            $items[] = array(
                'id'    => $rowDb->id,
                'label' => $valueData,
                'value' => $valueData,
            );
        }
        print json_encode($items);
    }
    exit();
}
function order_selesai(){
    $path = drupal_get_path('theme', 'cti_flex');
    $form_style = $path.'/css/custom-style.css';
    drupal_add_css($form_style, 'theme', 'all', FALSE);
    $variables['styles'] = drupal_get_css();
    _addNewDatabalesPlugins();
    $js_path = drupal_get_path('module','penjualan').'/js/orderselesai.form.js';
    drupal_add_js($js_path);
    $labelData = '<div>Scan/Masukkan nomer kode produksi</div>';
    $returnForm = '<div class="barcode-scan">'.$labelData;
    $returnForm .= '<input type="text" id="detailbarcode" name="detailbarcode" style="width: 275px;font-size: 18px;">';
    $returnForm .= '</div>';
    $tombolSimpan = '<p><button id="simpan-produksi">Simpan Hasil Produksi</button></p>';
    $returnForm .= '<div class="production-table">'.tableFinishedOrder().$tombolSimpan.'</div>';
    return $returnForm;
}
function tableFinishedOrder(){
    $finishedorder ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_finished_order">';
    $finishedorder .= '<thead>';
    $finishedorder .= '<tr>';
    $finishedorder .= '<th class="tablebutton"></th>';
    $finishedorder .= '<th>NO. ORDER</th>';
    $finishedorder .= '<th>TANGGAL</th>';
    $finishedorder .= '<th>PELANGGAN</th>';
    $finishedorder .= '<th>PRODUK</th>';
    $finishedorder .= '<th>QTY PESAN</th>';
    $finishedorder .= '<th>CURRENT OUTSTANDING</th>';
    $finishedorder .= '<th>QTY INPUT</th>';
    $finishedorder .= '<th>QTY OUTSTANDING</th>';
    $finishedorder .= '<th>DETAIL BARCODE</th>';
    $finishedorder .= '</tr>';
    $finishedorder .= '</thead>';
    $finishedorder .= '<tbody>';
    $finishedorder .= '</tbody>';
    $finishedorder .= '</table>';
    return $finishedorder;
}
function cari_nota_produksi(){
    if ($_GET["term"]){
        $searchText = '%'.strtolower($_GET["term"]).'%';
        $strSQL = 'SELECT detOrd.id, detOrd.detailbarcode, custOrd.nonota, custOrd.tglorder,plg.namapelanggan,prod.namaproduct,';
        $strSQL .= 'detOrd.jumlah,detOrd.outstanding FROM detailcustomerorder detOrd ';
        $strSQL .= 'LEFT JOIN customer_order custOrd ON detOrd.idcustomerorder = custOrd.id ';
        $strSQL .= 'LEFT JOIN pelanggan plg ON custOrd.idpelanggan = plg.idpelanggan ';
        $strSQL .= 'LEFT JOIN product prod ON detOrd.idproduct = prod.idproduct ';
        $strSQL .= 'WHERE LOWER(detOrd.detailbarcode) LIKE \'%s\'';
        $result = db_query($strSQL,$searchText);
        $items = array();
        while ($rowDb = db_fetch_object($result)){
            $valueData = $rowDb->nonota.' - '.$rowDb->namapelanggan.'-'.$rowDb->namaproduct;
            $items[] = array(
                'id'    => $rowDb->id,
                'label' => $valueData,
                'value' => $valueData,
                'detailbarcode' => $rowDb->detailbarcode,
                'nonota' => $rowDb->nonota,
                'tglorder' => $rowDb->tglorder,
                'namapelanggan' => $rowDb->namapelanggan,
                'namaproduct' => $rowDb->namaproduct,
                'jumlah' => $rowDb->jumlah,
                'outstanding' => $rowDb->outstanding,
            );
        }
        if (!count($items)){
            $items['sql'] = $strSQL;
        }
        print json_encode($items);
    }
}
function simpan_hasil_produksi(){
    if (isset($_POST['detailbarcode']) && !empty($_POST['detailbarcode']) && isset($_POST['qtyupdate']) && !empty($_POST['qtyupdate'])){
        $strSQL = 'SELECT id, idproduct, idcustomerorder,outstanding FROM detailcustomerorder WHERE detailbarcode = \'%s\'';
        $result = db_query($strSQL,$_POST['detailbarcode']);
        $dataDetailOrder = db_fetch_object($result);
        if ($dataDetailOrder){
            $strSQL = 'SELECT stok FROM product WHERE idproduct=%d';
            $stokSebelum = db_result(db_query($strSQL,$dataDetailOrder->idproduct));
            $stokSesudah = $stokSebelum + $_POST['qtyupdate'];
            $strSQL = 'UPDATE product SET stok = %d WHERE idproduct = %d';
            db_query($strSQL,$stokSesudah,$dataDetailOrder->idproduct);
            $keterangan = 'Hasil Produksi Dari Pesanan Detail Barcode : '.$_POST['detailbarcode'];
            db_query("INSERT INTO transaksistock (idproduk, idcustomerorder, stocksebelum, masuk, stocksetelah, keterangan) VALUES
			('%d', '%d', '%f', '%f', '%f', '%s')",$dataDetailOrder->idproduct,$dataDetailOrder->idcustomerorder,
                $stokSebelum ,$_POST['qtyupdate'],$stokSesudah,$keterangan);
            $strSQL = 'UPDATE detailcustomerorder SET outstanding = outstanding - %d WHERE id = %d';
            db_query($strSQL,$_POST['qtyupdate'],$dataDetailOrder->id);
        }
    }
}
function cari_meja(){
    if ($_GET["term"]){
        $KATACARI = '%'.$_GET["term"].'%';
        $result = db_query("SELECT id, barcodemeja, kodemeja, meja FROM meja WHERE kodemeja LIKE '%s' OR barcodemeja LIKE '%s' OR UPPER(meja) LIKE '%s' LIMIT 50",$KATACARI,$KATACARI,$KATACARI);
        $items = array();
        while ($data = db_fetch_object($result)){
            $items[] = array('value' => $data->meja,'barcodemeja' => $data->barcodemeja,'kodemeja' => $data->kodemeja, 'id' => $data->id);
        }
        print json_encode($items);
    }
    exit();
}
function cari_nota_order_android(){
    $strSQL = 'SELECT id FROM customer_order WHERE android_order=1 AND printed=0 ';
    $strSQL .= 'AND pending_print = 0 ORDER BY id LIMIT 1';
    $idOrder = db_result(db_query($strSQL));
    if ($idOrder > 0){
        db_query('UPDATE customer_order SET pending_print = 1 WHERE android_order=1 AND printed=0 AND id != %d', $idOrder);
        print $idOrder;
    }
    exit();
}
function get_customer_order_printer(){
    if (isset($_POST['idorder']) && !empty($_POST['idorder'])){
        $IpAddress = ip_address();
        $StrSqlPrn = "SELECT selected_printer FROM kategori_printer AS katprn WHERE ";
        $StrSqlPrn .= "katprn.idkategori = kat.idkategori AND katprn.ip_address = '$IpAddress'";
        $StrSql = 'SELECT ('.$StrSqlPrn.') AS chosen_printer  FROM detailcustomerorder AS dco ';
        $StrSql .= 'LEFT JOIN product AS prod ON dco.idproduct = prod.idproduct ';
        $StrSql .= 'LEFT JOIN kategori AS kat ON prod.idkategori = kat.idkategori ';
        $StrSql .= 'WHERE idcustomerorder=%d GROUP BY chosen_printer';
        $SelectedPrinter = array();
        $Result = db_query($StrSql, $_POST['idorder']);
        while ($DataPrinter = db_fetch_object($Result)){
            $SelectedPrinter[] = $DataPrinter->chosen_printer;
        }
        print json_encode($SelectedPrinter);
    }
    exit();
}
function cari_nota_order_android_pending(){
    $strSQL = 'SELECT id FROM customer_order WHERE android_order=1 AND printed=0 ';
    $strSQL .= 'AND pending_print = 1 ORDER BY id LIMIT 1';
    $idOrder = db_result(db_query($strSQL));
    if ($idOrder > 0){
        db_query('UPDATE customer_order SET pending_print = 0 WHERE id = %d', $idOrder);
        print $idOrder;
    }
    exit();
}