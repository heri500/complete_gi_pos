<?php

function meja(){
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
	drupal_add_js(
		array(
			'filePath' => $filePath,
			'idSubKategori' => $idSubKategori,
		),
		'setting');
	$js_path = drupal_get_path('module','dataproduk').'/datameja.js';
	drupal_add_js($js_path);
	$viewtabelmeja = '<form id="formmeja" action="'.base_path().'dataproduk/simpanmeja" method="post"><div id="formplace">';
	$viewtabelmeja .= '<label>BARCODE</label><input type="text" class=\"validate[required]\" id="barcode" name="barcode">';
	$viewtabelmeja .= '<label>KODE</label><input type="text" class=\"validate[required]\" id="kodemeja" name="kodemeja">';
	$viewtabelmeja .= '<label>MEJA</label><input class=\"validate[required]\" type="text" id="meja" name="meja">';
	$viewtabelmeja .= '<label>KETERANGAN</label><input type="text" id="keterangan" name="keterangan">';
	$viewtabelmeja .= '<button id="tambahmeja">Tambah Meja</button><button id="genbarcodemeja">Generate Barcode</button></div></form>';
	$viewtabelmeja .= '<div style="width: 75%;">'.tabel_meja().'</div>';
	return $viewtabelmeja;
}

function tabel_meja(){
	$tabelmeja ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_meja">';
	$tabelmeja .= '<thead>';
	$tabelmeja .= '<tr>';
	$tabelmeja .= '<th style="width: 150px;">BARCODE</th>';
	$tabelmeja .= '<th style="width: 150px;">KODE</th>';
	$tabelmeja .= '<th>MEJA</th>';
	$tabelmeja .= '<th>KETERANGAN</th>';
	$tabelmeja .= '<th class="tablebutton">&nbsp;</th>';
	$tabelmeja .= '</tr>';
	$tabelmeja .= '</thead>';
	$tabelmeja .= '<tbody>';
	$result = db_query("SELECT id,barcodemeja, kodemeja,meja,keterangan FROM meja ORDER BY kodemeja,meja");
	while ($data = db_fetch_object($result)){
		$tabelmeja .= '<tr id="'.$data->id.'">';
		$tabelmeja .= '<td class="editable">'.$data->barcodemeja.'</td>';
		$tabelmeja .= '<td class="editable">'.$data->kodemeja.'</td>';
		$tabelmeja .= '<td class="editable">'.$data->meja.'</td>';
		$tabelmeja .= '<td class="editable">'.$data->keterangan.'</td>';
		$tabelmeja .= '<td class="center"><input type="checkbox" class="selected-meja" id="checkbox-'.$data->id.'" name="checkbox-'.$data->id.'" value="'.$data->id.'"></td>';
		$tabelmeja .= '</tr>';
	}
	$tabelmeja .= '</tbody>';
	$tabelmeja .= '</table>';
	return $tabelmeja;
}

function simpan_meja(){
	if ($_POST["kodemeja"] AND $_POST["meja"]){
		db_query("INSERT INTO meja(kodemeja,barcodemeja,meja,keterangan) VALUES('%s','%s','%s','%s')",
		strtoupper($_POST["kodemeja"]),$_POST["barcode"],$_POST["meja"],$_POST["keterangan"]);
		if ($_POST["asal"]){
			$result = db_query("SELECT id FROM meja WHERE kodemeja='%s' AND meja='%s'",
			strtoupper($_POST["kodemeja"]),$_POST["meja"]);
			$data = db_fetch_object($result);
			echo $data->id;
		}else{
			drupal_goto("dataproduk/meja");
		}
	}
	exit();	
}

function update_meja(){
	$NILAI = $_POST['value'];
	$KOLOM = $_POST['kol_id'];
	$MEJA_ID = $_POST['row_id'];
	if ($KOLOM == "0"){
		$NILAI = strtoupper($NILAI);
		$sql_update = "UPDATE meja SET kodemeja='%s' WHERE id='%d'";
	}elseif ($KOLOM == "1"){
		$sql_update = "UPDATE meja SET barcodemeja='%s' WHERE id='%d'";
	}elseif ($KOLOM == "2"){
		$sql_update = "UPDATE meja SET meja='%s' WHERE id='%d'";
	}elseif ($KOLOM == "3"){
		$sql_update = "UPDATE meja SET keterangan='%s' WHERE id='%d'";
	}
	db_query($sql_update,$NILAI,$MEJA_ID);
	echo $NILAI;
	exit();	
}