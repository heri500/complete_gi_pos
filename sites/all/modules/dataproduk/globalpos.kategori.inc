<?php

function kategori_produk(){
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
	drupal_add_css('misc/media/datatables/css/demo_table_jui.css');
	drupal_add_css('misc/media/datatables/css/TableTools.css');
	drupal_add_css('misc/media/datatables/themes/smoothness/jquery-ui-1.8.4.custom.css');
	drupal_add_css('misc/media/datatables/css/ColVis.css');
	drupal_add_css('misc/media/css/validationEngine.jquery.css');
	drupal_add_js('misc/media/jquery-1.4.4.min.js');
	drupal_add_js('misc/media/jquery.validationEngine-en.js');
	drupal_add_js('misc/media/jquery.validationEngine.js');
	drupal_add_js('misc/media/jquery-ui-1.8.custom.min.js');
	drupal_add_js('misc/media/datatables/js/jquery.dataTables.min.js');
	drupal_add_js('misc/media/datatables/ZeroClipboard/ZeroClipboard.js');
	drupal_add_js('misc/media/datatables/js/TableTools.min.js');
	drupal_add_js('misc/media/jquery.jeditable.js');
	drupal_add_js('misc/media/jquery.autotab-1.1b.js');
    $filePath = base_path().file_directory_path();
    drupal_add_js(
        array(
            'filePath'  => $filePath,
        ),
        'setting'
    );
    $js_path = drupal_get_path('module','dataproduk').'/tabelkategori.js';
    drupal_add_js($js_path);
    $viewtabelkategori = '<form id="formkategori" action="'.base_path().'dataproduk/simpankategori" method="post"><div id="formplace">';
	$viewtabelkategori .= '<label>KODE</label><input type="text" class="validate[required]" id="kodekategori" name="kodekategori">';
	$viewtabelkategori .= '<label>KATEGORI</label><input class="validate[required]" type="text" id="kategori" name="kategori">';
	$viewtabelkategori .= '<label>KETERANGAN</label><input type="text" id="keterangan" name="keterangan">';
    $viewtabelkategori .= '<label>PRINTER</label><select id="selected-printer" name="selected-printer"></select>';
	$viewtabelkategori .= '<button>Tambah Kategori</button></div></form>';
	$viewtabelkategori .= '<div style="width: 80%;">'.tabel_kategori().'</div>';
	return $viewtabelkategori;
}

function tabel_kategori(){
	$tabelkategori ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_kategori">';
	$tabelkategori .= '<thead>';
	$tabelkategori .= '<tr>';
	$tabelkategori .= '<th style="width: 150px;">KODE</th>';
	$tabelkategori .= '<th>KATEGORI</th>';
	$tabelkategori .= '<th>KETERANGAN</th>';
    $tabelkategori .= '<th>PRINTER</th>';
	$tabelkategori .= '</tr>';
	$tabelkategori .= '</thead>';
	$tabelkategori .= '<tbody>';
	$result = db_query("SELECT idkategori,kodekategori,kategori,keterangan, selected_printer FROM kategori ORDER BY kodekategori,kategori");
	while ($data = db_fetch_object($result)){
		$tabelkategori .= '<tr id="'.$data->idkategori.'">';
		$tabelkategori .= '<td class="editable">'.$data->kodekategori.'</td>';
		$tabelkategori .= '<td class="editable">'.$data->kategori.'</td>';
		$tabelkategori .= '<td class="editable">'.$data->keterangan.'</td>';
        $tabelkategori .= '<td class="editable-printer">'.$data->selected_printer.'</td>';
		$tabelkategori .= '</tr>';
	}
	$tabelkategori .= '</tbody>';
	$tabelkategori .= '</table>';
	return $tabelkategori;
}

function simpan_kategori(){
	if ($_POST["kodekategori"] AND $_POST["kategori"]){
		db_query("INSERT INTO kategori(kodekategori,kategori,keterangan) VALUES('%s','%s','%s')",
		strtoupper(trim($_POST["kodekategori"])),trim($_POST["kategori"]),trim($_POST["keterangan"]));
		if ($_POST["asal"]){
			$result = db_query("SELECT idkategori FROM kategori WHERE kodekategori='%s' AND kategori='%s'",
			strtoupper($_POST["kodekategori"]),$_POST["kategori"]);
			$data = db_fetch_object($result);
			echo $data->idkategori;
		}else{
			drupal_goto("dataproduk/kategori");
		}
	}
	exit();	
}

function update_kategori(){
	$NILAI = trim($_POST['value']);
	$KOLOM = $_POST['kol_id'];
	$KATEGORI_ID = $_POST['row_id'];
	if ($KOLOM == "0"){
		$NILAI = strtoupper($NILAI);
		$sql_update = "UPDATE kategori SET kodekategori='%s' WHERE idkategori='%d'";
	}elseif ($KOLOM == "1"){
		$sql_update = "UPDATE kategori SET kategori='%s' WHERE idkategori='%d'";
	}elseif ($KOLOM == "2"){
		$sql_update = "UPDATE kategori SET keterangan='%s' WHERE idkategori='%d'";
	}elseif ($KOLOM == "3"){
        $sql_update = "UPDATE kategori SET selected_printer='%s' WHERE idkategori='%d'";
    }
	db_query($sql_update,$NILAI,$KATEGORI_ID);
	echo $NILAI;
	exit();	
}