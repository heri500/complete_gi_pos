<?php

function view_tabel_returnpelanggan(){
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
        $tabeltampil = tabel_returnpelanggan($tgl1,$tgl2);
        $urutan = 2;
    }elseif ($_POST["pilihantampil"] == 'PRODUK'){
        $produk = 'selected="selected"';
        $tabeltampil = returnpelanggan_produk($tgl1,$tgl2);
        $urutan = 0;
    }else{
        $tabeltampil = tabel_returnpelanggan($tgl1,$tgl2);
        $nota = 'selected="selected"';
        $urutan = 2;
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
    $js_path = drupal_get_path('module','penjualan').'/js/tabel.returnpelanggan.js';
    drupal_add_js($js_path);
    $pilihantampil = '<select id="pilihantampil" name="pilihantampil">';
    $pilihantampil .= '<option '.$nota.'>NOTA</option>';
    $pilihantampil .= '<option '.$produk.'>PRODUK</option>';
    $pilihantampil .= '</select>';
    $pilihperiode = '<form action="'.base_path().'penjualan/viewreturn" method="post"><div id="pilihanperiode"><label>PILIH PERIODE RETURN SUPPLIER </label><input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.date('d-m-Y', strtotime($tgl1)).'">';
    $pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.date('d-m-Y', strtotime($tgl2)).'">'.$pilihantampil.'<button>LIHAT RETURN SUPPLIER</button></div></form>';
    $viewreturnpelanggan = $pilihperiode.'<div id="viewreturnpelanggan">'.$tabeltampil.'</div>';
    $viewreturnpelanggan .= '<div id="dialogdetail" title="DETAIL RETURN SUPPLIER"></div>';
    return $viewreturnpelanggan;
}

function tabel_returnpelanggan($tgl1,$tgl2){
    //nonota, tglreturn, idpemakai, idlangganan, total,
    //totalmodal, carabayar, bayar, kembali
    $tgl1 = date('Y-m-d', strtotime($tgl1));
    $tgl2 = date('Y-m-d', strtotime($tgl2));
    $tabelreturnpelanggan ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_returnpelanggan">';
    $tabelreturnpelanggan .= '<thead>';
    $tabelreturnpelanggan .= '<tr>';
    $tabelreturnpelanggan .= '<th class="tablebutton"></th>';
    if (cekUserRoles()){
        $tabelreturnpelanggan .= '<th class="tablebutton"></th>';
    }
    $tabelreturnpelanggan .= '<th>NO. NOTA</th>';
    $tabelreturnpelanggan .= '<th class="tanggal">HARI</th>';
    $tabelreturnpelanggan .= '<th class="tanggal">TGL</th>';
    $tabelreturnpelanggan .= '<th class="tanggal">JAM</th>';
    if (cekUserRoles()){
        $tabelreturnpelanggan .= '<th>TOTAL</th>';
        $tabelreturnpelanggan .= '<th>CASH BACK</th>';
        $tabelreturnpelanggan .= '<th>KASIR</th>';
    }
    $tabelreturnpelanggan .= '<th>CARA RETUR</th>';
    $tabelreturnpelanggan .= '<th>KETERANGAN</th>';
    $tabelreturnpelanggan .= '</tr>';
    $tabelreturnpelanggan .= '</thead>';
    $tabelreturnpelanggan .= '<tbody>';
    $result = db_query("SELECT idreturn, name, nonota, SUBSTR(tglreturn,1,10) AS tanggal,SUBSTR(tglreturn,11,6)
    AS waktu, idpemakai, total, kembali, keterangan, c.carareturn FROM returnpelanggan a
    LEFT JOIN cms_users b ON a.idpemakai = b.uid
    LEFT JOIN carareturn c ON a.carareturn = c.id
    WHERE SUBSTR(tglreturn,1,10) BETWEEN '%s' AND '%s' ORDER BY idreturn",
        $tgl1,$tgl2);
    $totalmodal = 0;
    $totalbayar = 0;
    $totalkembali = 0;
    $arrayhari = arrayHari();
    drupal_add_js(
        array(
            'namahari' => $arrayhari,
        ),
        'setting');
    while($data = db_fetch_object($result)){
        $idreturn = $data->idreturn;
        $tabelreturnpelanggan .= '<tr>';
        $tomboldetail = '<img title="Klik untuk melihat detail return supplier" onclick="view_detail(\''.$data->idreturn.'\',\''.$data->nonota.'\');" src="'.base_path().'misc/media/images/forward_enabled.ico">';
        $tombolhapus = '<img title="Klik untuk menghapus return supplier" onclick="delete_returnpelanggan(\''.$data->idreturn.'\',\''.$data->nonota.'\');" src="'.base_path().'misc/media/images/del.ico" width="22">';
        $tabelreturnpelanggan .= '<td class="center">'.$tomboldetail.'</td>';
        if (cekUserRoles()){
            $tabelreturnpelanggan .= '<td class="center">'.$tombolhapus.'</td>';
        }
        $tabelreturnpelanggan .= '<td class="center">'.$data->nonota.'</td>';
        $indexhari = date('w', strtotime($data->tanggal));
        $tabelreturnpelanggan .= '<td id="harireturnpelanggan-'.$idreturn.'" class="center">'.$arrayhari[$indexhari].'</td>';
        $tabelreturnpelanggan .= '<td id="tglreturn-'.$idreturn.'" class="center edit-tanggal">'.date('d-m-Y', strtotime($data->tanggal)).'</td>';
        $tabelreturnpelanggan .= '<td id="jamreturnpelanggan-'.$idreturn.'" class="center edit-jam">'.$data->waktu.'</td>';
        if (cekUserRoles()){
            $tabelreturnpelanggan .= '<td class="angka">'.number_format($data->total,0,",",".").'</td>';
            $tabelreturnpelanggan .= '<td class="angka">'.number_format($data->kembali,0,",",".").'</td>';
            $tabelreturnpelanggan .= '<td class="center">'.$data->name.'</td>';
        }
        $tabelreturnpelanggan .= '<td class="center">'.$data->carareturn.'</td>';
        $tabelreturnpelanggan .= '<td class="center">'.$data->keterangan.'</td>';
        $tabelreturnpelanggan .= '</tr>';
        $totalmodal = $totalmodal + $data->total;
        $totalbayar = $totalbayar + $data->bayar;
        $totalkembali = $totalkembali + $data->kembali;
    }
    $tabelreturnpelanggan .= '</tbody>';
    if (cekUserRoles()){
        $tabelreturnpelanggan .= '<tfoot>';
        $tabelreturnpelanggan .= '<tr>';
        $tabelreturnpelanggan .= '<th style="text-align:right" colspan="6">Total:&nbsp;&nbsp;</th>';
        $tabelreturnpelanggan .= '<th style="text-align: right">Rp. '.number_format($totalmodal,0,",",".").'&nbsp;&nbsp;</th>';
        $tabelreturnpelanggan .= '<th style="text-align: right">Rp. '.number_format($totalbayar,0,",",".").'&nbsp;&nbsp;</th>';
        $tabelreturnpelanggan .= '<th style="text-align: right">Rp. '.number_format($totalkembali,0,",",".").'&nbsp;&nbsp;</th>';
        $tabelreturnpelanggan .= '<th>&nbsp;</th>';
        $tabelreturnpelanggan .= '<th>&nbsp;</th>';
        $tabelreturnpelanggan .= '</tr>';
        $tabelreturnpelanggan .= '</tfoot>';
    }
    $tabelreturnpelanggan .= '</table>';
    return $tabelreturnpelanggan;
}

function detail_returnpelanggan(){
    if ($_POST["idreturn"]){
        $tabelreturnpelanggan ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_returnpelanggan">';
        $tabelreturnpelanggan .= '<thead>';
        $tabelreturnpelanggan .= '<tr>';
        $tabelreturnpelanggan .= '<th>BARCODE</th>';
        $tabelreturnpelanggan .= '<th>PRODUK</th>';
        $tabelreturnpelanggan .= '<th class="qty">QTY</th>';
        if (cekUserRoles()){
            $tabelreturnpelanggan .= '<th>H.JUAL</th>';
            $tabelreturnpelanggan .= '<th>SUBTOTAL</th>';
        }
        $tabelreturnpelanggan .= '</tr>';
        $tabelreturnpelanggan .= '</thead>';
        $tabelreturnpelanggan .= '<tbody>';
        $result = db_query("SELECT c.barcode, c.namaproduct, b.jumlah, b.hargajual, (b.hargajual*b.jumlah) AS subtotal FROM
		detailreturnpelanggan b, product c WHERE b.idproduct=c.idproduct AND b.idreturn='%d'",$_POST["idreturn"]);
        while($data = db_fetch_object($result)){
            $tabelreturnpelanggan .= '<tr>';
            $tabelreturnpelanggan .= '<td>'.$data->barcode.'</td>';
            $tabelreturnpelanggan .= '<td>'.$data->namaproduct.'</td>';
            $tabelreturnpelanggan .= '<td class="angka">'.$data->jumlah.'</td>';
            if (cekUserRoles()){
                $tabelreturnpelanggan .= '<td class="angka">'.$data->hargajual.'</td>';
                $tabelreturnpelanggan .= '<td class="angka">'.$data->subtotal.'</td>';
            }
            $tabelreturnpelanggan .= '</tr>';
        }
        $tabelreturnpelanggan .= '</tbody>';
        $tabelreturnpelanggan .= '</table><br><br>';
        print $tabelreturnpelanggan;
    }
    exit();
}
function returnpelanggan_produk($tgl1,$tgl2){
    $tgl1 = date('Y-m-d', strtotime($tgl1)).' 00:00';
    $tgl2 = date('Y-m-d', strtotime($tgl2)).' 23:59';
    $tabelreturnpelanggan ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_returnpelanggan">';
    $tabelreturnpelanggan .= '<thead>';
    $tabelreturnpelanggan .= '<tr>';
    $tabelreturnpelanggan .= '<th>BARCODE</th>';
    $tabelreturnpelanggan .= '<th>PRODUK</th>';
    $tabelreturnpelanggan .= '<th>SUPPLIER</th>';
    $tabelreturnpelanggan .= '<th class="qty">QTY</th>';
    if (cekUserRoles()){
        $tabelreturnpelanggan .= '<th>H.MODAL</th>';
        $tabelreturnpelanggan .= '<th>SUBTOTAL</th>';
    }
    $tabelreturnpelanggan .= '</tr>';
    $tabelreturnpelanggan .= '</thead>';
    $tabelreturnpelanggan .= '<tbody>';
    $result = db_query("SELECT d.namasupplier, a.tglreturn, c.barcode, c.namaproduct, SUM(b.jumlah) AS totalqty,
	c.hargapokok, SUM(b.hargapokok*b.jumlah) AS subtotal FROM
	returnpelanggan a LEFT JOIN detailreturnpelanggan b ON a.idreturn = b.idreturn
	LEFT JOIN product c ON b.idproduct=c.idproduct
	LEFT JOIN supplier d ON c.idsupplier=d.idsupplier
	WHERE a.tglreturn BETWEEN '%s' AND '%s' GROUP BY b.idproduct",$tgl1,$tgl2);
    $totalmodal = 0;
    while($data = db_fetch_object($result)){
        $tabelreturnpelanggan .= '<tr>';
        $tabelreturnpelanggan .= '<td>'.$data->barcode.'</td>';
        $tabelreturnpelanggan .= '<td>'.$data->namaproduct.'</td>';
        $tabelreturnpelanggan .= '<td>'.$data->namasupplier.'</td>';
        $tabelreturnpelanggan .= '<td class="angka">'.$data->totalqty.'</td>';
        if (cekUserRoles()){
            $tabelreturnpelanggan .= '<td class="angka">'.$data->hargapokok.'</td>';
            $tabelreturnpelanggan .= '<td class="angka">'.$data->subtotal.'</td>';
        }
        $tabelreturnpelanggan .= '</tr>';
        $totalmodal = $totalmodal + $data->subtotal;
    }
    $tabelreturnpelanggan .= '</tbody>';
    if (cekUserRoles()){
        $tabelreturnpelanggan .= '<tfoot>';
        $tabelreturnpelanggan .= '<tr>';
        $tabelreturnpelanggan .= '<th style="text-align:right" colspan="5">Total:&nbsp;&nbsp;</th>';
        $tabelreturnpelanggan .= '<th style="text-align: right">Rp. '.number_format($totalmodal,0,",",".").'&nbsp;&nbsp;</th>';
        $tabelreturnpelanggan .= '</tr>';
        $tabelreturnpelanggan .= '</tfoot>';
    }
    $tabelreturnpelanggan .= '</table>';
    return $tabelreturnpelanggan;
}