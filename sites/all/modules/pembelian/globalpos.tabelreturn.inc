<?php

function view_tabel_returnsupplier(){
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
        $tabeltampil = tabel_returnsupplier($tgl1,$tgl2);
        $urutan = 2;
    }elseif ($_POST["pilihantampil"] == 'PRODUK'){
        $produk = 'selected="selected"';
        $tabeltampil = returnsupplier_produk($tgl1,$tgl2);
        $urutan = 0;
    }else{
        $tabeltampil = tabel_returnsupplier($tgl1,$tgl2);
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
    $js_path = drupal_get_path('module','pembelian').'/js/tabel.returnsupplier.js';
    drupal_add_js($js_path);
    $pilihantampil = '<select id="pilihantampil" name="pilihantampil">';
    $pilihantampil .= '<option '.$nota.'>NOTA</option>';
    $pilihantampil .= '<option '.$produk.'>PRODUK</option>';
    $pilihantampil .= '</select>';
    $pilihperiode = '<form action="'.base_path().'pembelian/viewreturnsupplier" method="post"><div id="pilihanperiode"><label>PILIH PERIODE RETURN SUPPLIER </label><input readonly="readonly" type="text" id="tgl1" name="tgl1" value="'.date('d-m-Y', strtotime($tgl1)).'">';
    $pilihperiode .= '<input readonly="readonly" type="text" id="tgl2" name="tgl2" value="'.date('d-m-Y', strtotime($tgl2)).'">'.$pilihantampil.'<button>LIHAT RETURN SUPPLIER</button></div></form>';
    $viewreturnsupplier = $pilihperiode.'<div id="viewreturnsupplier">'.$tabeltampil.'</div>';
    $viewreturnsupplier .= '<div id="dialogdetail" title="DETAIL RETURN SUPPLIER"></div>';
    return $viewreturnsupplier;
}

function tabel_returnsupplier($tgl1,$tgl2){
    //nonota, tglreturn, idpemakai, idlangganan, total,
    //totalmodal, carabayar, bayar, kembali
    $tgl1 = date('Y-m-d', strtotime($tgl1));
    $tgl2 = date('Y-m-d', strtotime($tgl2));
    $tabelreturnsupplier ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_returnsupplier">';
    $tabelreturnsupplier .= '<thead>';
    $tabelreturnsupplier .= '<tr>';
    $tabelreturnsupplier .= '<th class="tablebutton"></th>';
    if (cekUserRoles()){
        $tabelreturnsupplier .= '<th class="tablebutton"></th>';
    }
    $tabelreturnsupplier .= '<th>NO. NOTA</th>';
    $tabelreturnsupplier .= '<th class="tanggal">HARI</th>';
    $tabelreturnsupplier .= '<th class="tanggal">TGL</th>';
    $tabelreturnsupplier .= '<th class="tanggal">JAM</th>';
    if (cekUserRoles()){
        $tabelreturnsupplier .= '<th>TOTAL</th>';
        $tabelreturnsupplier .= '<th>CASH BACK</th>';
        $tabelreturnsupplier .= '<th>KASIR</th>';
    }
    $tabelreturnsupplier .= '<th>CARA RETUR</th>';
    $tabelreturnsupplier .= '<th>KETERANGAN</th>';
    $tabelreturnsupplier .= '</tr>';
    $tabelreturnsupplier .= '</thead>';
    $tabelreturnsupplier .= '<tbody>';
    $result = db_query("SELECT idreturn, name, nonota, SUBSTR(tglreturn,1,10) AS tanggal,SUBSTR(tglreturn,11,6)
    AS waktu, idpemakai, total, uang_dikembalikan, keterangan, c.carareturn FROM returnsupplier a
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
        $tabelreturnsupplier .= '<tr>';
        $tomboldetail = '<img title="Klik untuk melihat detail return supplier" onclick="view_detail(\''.$data->idreturn.'\',\''.$data->nonota.'\');" src="'.base_path().'misc/media/images/forward_enabled.ico">';
        $tombolhapus = '<img title="Klik untuk menghapus return supplier" onclick="delete_returnsupplier(\''.$data->idreturn.'\',\''.$data->nonota.'\');" src="'.base_path().'misc/media/images/del.ico" width="22">';
        $tabelreturnsupplier .= '<td class="center">'.$tomboldetail.'</td>';
        if (cekUserRoles()){
            $tabelreturnsupplier .= '<td class="center">'.$tombolhapus.'</td>';
        }
        $tabelreturnsupplier .= '<td class="center">'.$data->nonota.'</td>';
        $indexhari = date('w', strtotime($data->tanggal));
        $tabelreturnsupplier .= '<td id="harireturnsupplier-'.$idreturn.'" class="center">'.$arrayhari[$indexhari].'</td>';
        $tabelreturnsupplier .= '<td id="tglreturn-'.$idreturn.'" class="center edit-tanggal">'.date('d-m-Y', strtotime($data->tanggal)).'</td>';
        $tabelreturnsupplier .= '<td id="jamreturnsupplier-'.$idreturn.'" class="center edit-jam">'.$data->waktu.'</td>';
        if (cekUserRoles()){
            $tabelreturnsupplier .= '<td class="angka">'.number_format($data->total,0,",",".").'</td>';
            $tabelreturnsupplier .= '<td class="angka">'.number_format($data->uang_dikembalikan,0,",",".").'</td>';
            $tabelreturnsupplier .= '<td class="center">'.$data->name.'</td>';
        }
        $tabelreturnsupplier .= '<td class="center">'.$data->carareturn.'</td>';
        $tabelreturnsupplier .= '<td class="center">'.$data->keterangan.'</td>';
        $tabelreturnsupplier .= '</tr>';
        $totalmodal = $totalmodal + $data->total;
        $totalbayar = $totalbayar + $data->bayar;
        $totalkembali = $totalkembali + $data->kembali;
    }
    $tabelreturnsupplier .= '</tbody>';
    if (cekUserRoles()){
        $tabelreturnsupplier .= '<tfoot>';
        $tabelreturnsupplier .= '<tr>';
        $tabelreturnsupplier .= '<th style="text-align:right" colspan="6">Total:&nbsp;&nbsp;</th>';
        $tabelreturnsupplier .= '<th style="text-align: right">Rp. '.number_format($totalmodal,0,",",".").'&nbsp;&nbsp;</th>';
        $tabelreturnsupplier .= '<th style="text-align: right">Rp. '.number_format($totalbayar,0,",",".").'&nbsp;&nbsp;</th>';
        $tabelreturnsupplier .= '<th style="text-align: right">Rp. '.number_format($totalkembali,0,",",".").'&nbsp;&nbsp;</th>';
        $tabelreturnsupplier .= '<th>&nbsp;</th>';
        $tabelreturnsupplier .= '<th>&nbsp;</th>';
        $tabelreturnsupplier .= '</tr>';
        $tabelreturnsupplier .= '</tfoot>';
    }
    $tabelreturnsupplier .= '</table>';
    return $tabelreturnsupplier;
}

function detail_returnsupplier(){
    if ($_POST["idreturn"]){
        $tabelreturnsupplier ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_detail_returnsupplier">';
        $tabelreturnsupplier .= '<thead>';
        $tabelreturnsupplier .= '<tr>';
        $tabelreturnsupplier .= '<th>BARCODE</th>';
        $tabelreturnsupplier .= '<th>PRODUK</th>';
        $tabelreturnsupplier .= '<th class="qty">QTY</th>';
        if (cekUserRoles()){
            $tabelreturnsupplier .= '<th>H.MODAL</th>';
            $tabelreturnsupplier .= '<th>SUBTOTAL</th>';
        }
        $tabelreturnsupplier .= '</tr>';
        $tabelreturnsupplier .= '</thead>';
        $tabelreturnsupplier .= '<tbody>';
        $result = db_query("SELECT c.barcode, c.namaproduct, b.jumlah, b.hargapokok, (b.hargapokok*b.jumlah) AS subtotal FROM
		detailreturn b, product c WHERE b.idproduct=c.idproduct AND b.idreturn='%d'",$_POST["idreturn"]);
        while($data = db_fetch_object($result)){
            $tabelreturnsupplier .= '<tr>';
            $tabelreturnsupplier .= '<td>'.$data->barcode.'</td>';
            $tabelreturnsupplier .= '<td>'.$data->namaproduct.'</td>';
            $tabelreturnsupplier .= '<td class="angka">'.$data->jumlah.'</td>';
            if (cekUserRoles()){
                $tabelreturnsupplier .= '<td class="angka">'.$data->hargapokok.'</td>';
                $tabelreturnsupplier .= '<td class="angka">'.$data->subtotal.'</td>';
            }
            $tabelreturnsupplier .= '</tr>';
        }
        $tabelreturnsupplier .= '</tbody>';
        $tabelreturnsupplier .= '</table><br><br>';
        print $tabelreturnsupplier;
    }
    exit();
}
function returnsupplier_produk($tgl1,$tgl2){
    $tgl1 = date('Y-m-d', strtotime($tgl1)).' 00:00';
    $tgl2 = date('Y-m-d', strtotime($tgl2)).' 23:59';
    $tabelreturnsupplier ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_returnsupplier">';
    $tabelreturnsupplier .= '<thead>';
    $tabelreturnsupplier .= '<tr>';
    $tabelreturnsupplier .= '<th>BARCODE</th>';
    $tabelreturnsupplier .= '<th>PRODUK</th>';
    $tabelreturnsupplier .= '<th>SUPPLIER</th>';
    $tabelreturnsupplier .= '<th class="qty">QTY</th>';
    if (cekUserRoles()){
        $tabelreturnsupplier .= '<th>H.MODAL</th>';
        $tabelreturnsupplier .= '<th>SUBTOTAL</th>';
    }
    $tabelreturnsupplier .= '</tr>';
    $tabelreturnsupplier .= '</thead>';
    $tabelreturnsupplier .= '<tbody>';
    $result = db_query("SELECT d.namasupplier, a.tglreturn, c.barcode, c.namaproduct, SUM(b.jumlah) AS totalqty,
	c.hargapokok, SUM(b.hargapokok*b.jumlah) AS subtotal FROM
	returnsupplier a LEFT JOIN detailreturn b ON a.idreturn = b.idreturn
	LEFT JOIN product c ON b.idproduct=c.idproduct
	LEFT JOIN supplier d ON c.idsupplier=d.idsupplier
	WHERE a.tglreturn BETWEEN '%s' AND '%s' GROUP BY b.idproduct",$tgl1,$tgl2);
    $totalmodal = 0;
    while($data = db_fetch_object($result)){
        $tabelreturnsupplier .= '<tr>';
        $tabelreturnsupplier .= '<td>'.$data->barcode.'</td>';
        $tabelreturnsupplier .= '<td>'.$data->namaproduct.'</td>';
        $tabelreturnsupplier .= '<td>'.$data->namasupplier.'</td>';
        $tabelreturnsupplier .= '<td class="angka">'.$data->totalqty.'</td>';
        if (cekUserRoles()){
            $tabelreturnsupplier .= '<td class="angka">'.$data->hargapokok.'</td>';
            $tabelreturnsupplier .= '<td class="angka">'.$data->subtotal.'</td>';
        }
        $tabelreturnsupplier .= '</tr>';
        $totalmodal = $totalmodal + $data->subtotal;
    }
    $tabelreturnsupplier .= '</tbody>';
    if (cekUserRoles()){
        $tabelreturnsupplier .= '<tfoot>';
        $tabelreturnsupplier .= '<tr>';
        $tabelreturnsupplier .= '<th style="text-align:right" colspan="5">Total:&nbsp;&nbsp;</th>';
        $tabelreturnsupplier .= '<th style="text-align: right">Rp. '.number_format($totalmodal,0,",",".").'&nbsp;&nbsp;</th>';
        $tabelreturnsupplier .= '</tr>';
        $tabelreturnsupplier .= '</tfoot>';
    }
    $tabelreturnsupplier .= '</table>';
    return $tabelreturnsupplier;
}
