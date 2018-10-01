<?php

function subkategori_produk(){
	$path = drupal_get_path('theme', 'cti_flex');
 	$form_style = $path.'/css/custom-style.css';
 	drupal_add_css($form_style, 'theme', 'all', FALSE);
 	$variables['styles'] = drupal_get_css();
	$result = db_query("SELECT idkategori,kodekategori,kategori FROM kategori");
	$pilihan = array();
	while ($data = db_fetch_object($result)){
		$pilihan[$data->idkategori] = $data->kodekategori."-".$data->kategori;
	}
	$nilai = json_encode($pilihan);
	$nilai = str_replace('"','\'',$nilai);
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
	drupal_add_js('
		var oTable;
		var pathutama = "'.base_path().'";
		var pathfile = "'.base_path().file_directory_path().'/";
		var pilihan = "'.$nilai.'";
		function tampilkantabelsubkategori(){
			oTable = $("#tabel_subkategori").dataTable( {
				"bJQueryUI": true,
				"bAutoWidth": false,
				"sPaginationType": "full_numbers",
				"bInfo": false,
				"aLengthMenu": [[100, 200, 300, -1], [100, 200, 300, "All"]],
				"iDisplayLength": 100,
				"aaSorting": [[0, "asc"]],
				"sDom": \'<"space"T><"clear"><"H"lfr>t<"F"ip>\'
			});
		}
		
		function simpankategori(){
			if ($("#kodekategori").val() != "" && $("#kategori").val() != ""){
				var request = new Object();
				request.kodekategori = $("#kodekategori").val();
				request.kategori = $("#kategori").val();
				request.keterangan = $("#keterangan").val();
				request.asal = "Subkategori";
				alamat = pathutama + "dataproduk/simpankategori";
				$.ajax({ 
					type: "POST",
					url: alamat,
					data: request, 
					cache: false, 
					success: function(data){
						$("#idkategori").append("<option value=\""+ data +"\">"+ request.kodekategori +"-"+ request.kategori +"</option>");
						$("#kodekategori").val("");
						$("#kategori").val("");
						$("#keterangan").val("");
						$("#dialogtambahkategori").dialog("close");
						$("#idkategori").val(data);
					}
				});
			}else{
				$("#tambahkategoriform").submit();
			}	
		}
		
		$(document).ready(function() {
			$("#dialogtambahkategori").dialog({
				modal: true,
				width: 350,
				resizable: false,
				autoOpen: false,
				position: ["auto","auto"],
				open: function(event, ui) {
					$("#kodekategori").focus();
				},
				close: function(){
					$("#tambahkategoriform").validationEngine("hide");
				}
			});
			TableToolsInit.sSwfPath = pathutama +"misc/media/datatables/swf/ZeroClipboard.swf";
			alamatupdate = pathutama + "dataproduk/updatesubkategori";
			$("#tabel_subkategori tbody .editable").editable(alamatupdate, {
				"callback": function( sValue, y ) {
					var aPos = oTable.fnGetPosition( this );
					oTable.fnUpdate( sValue, aPos[0], aPos[1] );
				},
				"submitdata": function ( value, settings ) {
					var aPos = oTable.fnGetPosition( this );
				  return { "row_id": this.parentNode.getAttribute("id"), "kol_id": aPos[1] };
				},
				"height": "20px",
				"submit": "Ok",
				"cancel": "Batal",
				"indicator": "Menyimpan...",
				"tooltip": "Klik untuk mengubah..."
			});
			$("#tabel_subkategori tbody .editable2").editable(alamatupdate, {
				"callback": function( sValue, y ) {
					var aPos = oTable.fnGetPosition( this );
					oTable.fnUpdate( sValue, aPos[0], aPos[1] );
				},
				"submitdata": function ( value, settings ) {
					var aPos = oTable.fnGetPosition( this );
				  return { "row_id": this.parentNode.getAttribute("id"), "kol_id": aPos[1] };
				},
				"type": "select",
				"data": pilihan,		
				"height": "20px",
				"submit": "Ok",
				"cancel": "Batal",
				"indicator": "Menyimpan...",
				"tooltip": "Klik untuk mengubah..."
			});
			tampilkantabelsubkategori();
			$("#formsubkategori").validationEngine();
			$("#tambahkategoriform").validationEngine();
			$("button").button();
			$("#addkategori").button({
	      icons: {
	          primary: "ui-icon-plusthick"
	      },
	      text: false
      }).click(function(){
      	$("#dialogtambahkategori").dialog("open");
      	$(".ui-dialog-title").css("font-size","15px");
      	return false;
      });
      $("#kodekategori, #kodesubkategori").autotab_filter({
				format: "alphanumeric",
				uppercase: true,
				nospace: true
			});
		})
	','inline');
	$result = db_query("SELECT idkategori,kodekategori,kategori FROM kategori ORDER BY kodekategori,kategori");
	$pilihankategori = '<select id="idkategori" name="idkategori">';
	while ($data = db_fetch_object($result)){
		$pilihankategori .= '<option value="'.$data->idkategori.'">'.$data->kodekategori.'-'.$data->kategori.'</option>';
	}
	$pilihankategori .= '</select>';
	$buttonadd = '<button id="addkategori" style="float: left;font-size: 10px;margin:0 6px 0 -4px;">Tambah Kategori</button>';
	$viewtabelsubkategori = '<form id="formsubkategori" action="'.base_path().'dataproduk/simpansubkategori" method="post"><div id="formplace" class="subkategori">';
	$viewtabelsubkategori .= '<label>KATEGORI</label>'.$pilihankategori.$buttonadd;
	$viewtabelsubkategori .= '<label>KODE</label><input type="text" class=\"validate[required]\" id="kodesubkategori" name="kodesubkategori">';
	$viewtabelsubkategori .= '<label>SUBKATEGORI</label><input class=\"validate[required]\" type="text" id="subkategori" name="subkategori">';
	$viewtabelsubkategori .= '<label>KETERANGAN</label><input type="text" id="keterangansub" name="keterangansub">';
	$viewtabelsubkategori .= '<button>Tambah Subkategori</button></div></form>';
	$viewtabelsubkategori .= '<div style="width: 80%;">'.tabel_subkategori().'</div>';
	$viewtabelsubkategori .= '<div id="dialogtambahkategori" title="TAMBAH KATEGORI PRODUK">';
	$viewtabelsubkategori .= '<form id="tambahkategoriform"><div><label>KODE</label><input type="text" class=\"validate[required]\" id="kodekategori" name="kodekategori"></div>';
	$viewtabelsubkategori .= '<div><label>KATEGORI</label><input class=\"validate[required]\" type="text" id="kategori" name="kategori"></div>';
	$viewtabelsubkategori .= '<div><label>KETERANGAN</label><input type="text" id="keterangan" name="keterangan"></div></form>';
	$viewtabelsubkategori .= '<div><button id="tambahkategori" onclick="simpankategori();">Tambah Kategori</button></div>';
	$viewtabelsubkategori .= '</div>';
	return $viewtabelsubkategori;
}

function tabel_subkategori(){
	$tabelsubkategori ='<table cellpadding="0" cellspacing="0" border="0" class="display" id="tabel_subkategori">';
	$tabelsubkategori .= '<thead>';
	$tabelsubkategori .= '<tr>';
	$tabelsubkategori .= '<th style="width: 220px;">KATEGORI</th>';
	$tabelsubkategori .= '<th style="width: 150px;">KODE</th>';
	$tabelsubkategori .= '<th>SUBKATEGORI</th>';
	$tabelsubkategori .= '<th>KETERANGAN</th>';
	$tabelsubkategori .= '</tr>';
	$tabelsubkategori .= '</thead>';
	$tabelsubkategori .= '<tbody>';
	$result = db_query("SELECT a.idsubkategori,a.idkategori,b.kodekategori,b.kategori,a.kodesubkategori,a.subkategori,a.keterangan 
	FROM subkategori a,kategori b WHERE a.idkategori=b.idkategori ORDER BY a.kodesubkategori,a.subkategori");
	while ($data = db_fetch_object($result)){
		$tabelsubkategori .= '<tr id="'.$data->idsubkategori.'">';
		$tabelsubkategori .= '<td class="editable2">'.$data->kodekategori.'-'.$data->kategori.'</td>';
		$tabelsubkategori .= '<td class="editable">'.$data->kodesubkategori.'</td>';
		$tabelsubkategori .= '<td class="editable">'.$data->subkategori.'</td>';
		$tabelsubkategori .= '<td class="editable">'.$data->keterangan.'</td>';
		$tabelsubkategori .= '</tr>';
	}
	$tabelsubkategori .= '</tbody>';
	$tabelsubkategori .= '</table>';
	return $tabelsubkategori;
}

function simpan_subkategori(){
	if ($_POST["idkategori"] AND $_POST["kodesubkategori"] AND $_POST["subkategori"]){
		db_query("INSERT INTO subkategori(idkategori,kodesubkategori,subkategori,keterangan) VALUES('%d','%s','%s','%s')",
		$_POST["idkategori"],strtoupper(trim($_POST["kodesubkategori"])),trim($_POST["subkategori"]),trim($_POST["keterangansub"]));
		if ($_POST["asal"]){
			$result = db_query("SELECT idsubkategori FROm subkategori WHERE idkategori='%d' AND kodesubkategori='%s' AND subkategori='%s'",
			$_POST["idkategori"],strtoupper($_POST["kodesubkategori"]),$_POST["subkategori"]);
			$data = db_fetch_object($result);
			print $data->idsubkategori;
		}else{
			drupal_goto("dataproduk/subkategori");
		}
	}
	exit();	
}

function update_subkategori(){
	$NILAI = trim($_POST['value']);
	$KOLOM = $_POST['kol_id'];
	$SUBKATEGORI_ID = $_POST['row_id'];
	$NILAITAMPIL = $NILAI;
	if ($KOLOM == "1"){
		$NILAI = strtoupper($NILAI);
		$sql_update = "UPDATE subkategori SET kodesubkategori='%s' WHERE idsubkategori='%d'";
	}elseif ($KOLOM == "2"){
		$sql_update = "UPDATE subkategori SET subkategori='%s' WHERE idsubkategori='%d'";
	}elseif ($KOLOM == "3"){
		$sql_update = "UPDATE subkategori SET keterangan='%s' WHERE idsubkategori='%d'";
	}elseif ($KOLOM == "0"){
		$sql_update = "UPDATE subkategori SET idkategori='%d' WHERE idsubkategori='%d'";
		$result = db_query("SELECT kodekategori,kategori FROM kategori WHERE idkategori='%d'",$NILAI);
		$data = db_fetch_object($result);
		$NILAITAMPIL = $data->kodekategori."-".$data->kategori;	
	}
	db_query($sql_update,$NILAI,$SUBKATEGORI_ID);
	echo $NILAITAMPIL;
	exit();	
}