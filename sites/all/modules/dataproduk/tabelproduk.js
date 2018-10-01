var oTable;
var barcodesama = false;
var columnSetting = new Array;
var pathutama = '';
var pathfile = '';
function tampilkantabelproduk(){
	oTable = $("#tabel_produk").dataTable( {
		"bJQueryUI": true,
		"bAutoWidth": false,
		"sPaginationType": "full_numbers",
		"bInfo": true,
		"aoColumns": columnSetting,
		"aLengthMenu": [[100, 200, 300, -1], [100, 200, 300, "All"]],
		"iDisplayLength": 100,
		"aaSorting": [[1, "asc"]],
		"sDom": '<"space"T><C><"clear"><"H"lfr>t<"F"ip>',
			"oColVis": {
				"aiExclude": [ 0 ],
				"activate": "mouseover"
			}
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
				$("#idkategori2").append("<option value=\""+ data +"\">"+ request.kodekategori +"-"+ request.kategori +"</option>");
				$("#kodekategori").val("");
				$("#kategori").val("");
				$("#keterangan").val("");
				$("#dialogtambahkategori").dialog("close");
				$("#idkategori").val(data);
				$("#idkategori2").val(data);
				filtersubkategori();
			}
		});
	}else{
		$("#tambahkategoriform").submit();
	}	
}

function simpansubkategori(){
	if ($("#kodesubkategori").val() != "" && $("#subkategori").val() != ""){
		var request = new Object();
		request.idkategori = $("#idkategori2").val();
		request.kodesubkategori = $("#kodesubkategori").val();
		request.subkategori = $("#subkategori").val();
		request.keterangan = $("#keterangansub").val();
		request.asal = "Produk";
		alamat = pathutama + "dataproduk/simpansubkategori";
		$.ajax({ 
			type: "POST",
			url: alamat,
			data: request, 
			cache: false, 
			success: function(data){
				$("#idsubkategori").append("<option value=\""+ data +"\">"+ request.kodesubkategori +"-"+ request.subkategori +"</option>");
				$("#kodesubkategori").val("");
				$("#subkategori").val("");
				$("#keterangansub").val("");
				$("#dialogtambahsubkategori").dialog("close");
				$("#idsubkategori").val(data);
				ubahkodebarang();
			}
		});
	}else{
		$("#tambahsubkategoriform").submit();
	}	
}

function simpansupplier(){
	if ($("#kodesupplier").val() != "" && $("#supplier").val() != ""){
		var request = new Object();
		request.kodesupplier = $("#kodesupplier").val();
		request.namasupplier = $("#namasupplier").val();
		request.keterangan = $("#keterangan").val();
		request.asal = "supplier";
		alamat = pathutama + "datasupplier/simpansupplier";
		$.ajax({ 
			type: "POST",
			url: alamat,
			data: request, 
			cache: false, 
			success: function(data){
				$("#idsupplier").append("<option value=\""+ data +"\">"+ request.kodesupplier +"-"+ request.namasupplier +"</option>");
				$("#kodesupplier").val("");
				$("#supplier").val("");
				$("#keterangan").val("");
				$("#dialogtambahsupplier").dialog("close");
				$("#idsupplier").val(data);
			}
		});
	}else{
		$("#tambahsupplierform").submit();
	}	
}
function simpansatuan(){
	if ($("#namasatuan").val() != ""){
		var request = new Object();
		request.namasatuan = $("#namasatuan").val();
		alamat = pathutama + "dataproduk/simpansatuan";
		$.ajax({ 
			type: "POST",
			url: alamat,
			data: request, 
			cache: false, 
			success: function(data){
				if (data != "error"){
					$("#satuan").append("<option value=\""+ request.namasatuan +"\">"+ request.namasatuan +"</option>");
					$("#namasatuan").val("");
					$("#dialogtambahsatuan").dialog("close");
					$("#satuan").val(data);
				}else{
					alert("Satuan ini sudah ada..!!");	
				}
			}
		});
	}else{
		$("#tambahsatuanform").submit();
	}	
}
function tutupformtambahproduk(){
	$("#formproduk").validationEngine("hide");
	$("#formpenambahanproduk").fadeOut("slow");
}
function filtersubkategori(){
	var request = new Object();
	request.idkategori = 	$("#idkategori").val();
	var alamat = pathutama + "dataproduk/filterkategori";
	$.ajax({ 
		type: "POST",
		url: alamat,
		data: request, 
		cache: false, 
		success: function(data){
			$("#idsubkategori").empty();
			$("#idsubkategori").append(data);
			ubahkodebarang();
		}
	});
}
function ubahkodebarang(){
	var kategori = "";
	$("#idkategori option:selected").each(function () {
		kategori = $(this).text();
	}); 
	var pecahkategori = kategori.split("-");
	var subkategori = "";
	$("#idsubkategori option:selected").each(function () {
		subkategori = $(this).text();
	});
	pecahsubkategori = subkategori.split("-");
	kodebarang = pecahkategori[0] +"-"+ pecahsubkategori[0];
	var request = new Object();
	request.kodealternatif = 	kodebarang;
	var alamat = pathutama + "dataproduk/cekkodealternatif";
	$.ajax({ 
		type: "POST",
		url: alamat,
		data: request, 
		cache: false, 
		success: function(data){
			$("#kodepoduk").val(kodebarang +"-"+ data);
		}
	});
}
function hitungmargin(asal){
	if (asal == "hargapokok"){
		if ($("#margin").val() > 0 && $("#hargapokok").val() > 0){
			var hargajual = $("#hargapokok").val() * (100/(100-$("#margin").val()));
			$("#hargajual").val(Math.round(hargajual));
		}
	}else if (asal == "hargajual"){
		if ($("#hargapokok").val() > 0 && $("#hargajual").val() > 0){
			var margin = (($("#hargajual").val() - $("#hargapokok").val())/$("#hargajual").val())*100;
			$("#margin").val(Math.round(margin));
		}
	}else if (asal == "margin"){
		if ($("#hargapokok").val() > 0 && $("#margin").val() > 0){
			var hargajual = $("#hargapokok").val() * (100/(100-$("#margin").val()));
			$("#hargajual").val(Math.round(hargajual));
		}
	}
}
function cek_barcode(field, rules, i, options){
	if (field.val() != ""){
		$("#validating").html("<img src = \""+ pathutama +"misc/media/images/loading.gif\">");
		$("#validating").fadeIn("fast",function(){
			var request = new Object();
		 	request.barcode = field.val(); 
			cekbarcode = pathutama +"dataproduk/cekbarcode";
			$.ajax({
				type: "POST", 
				url: cekbarcode,
				data: request,
				cache: false,
				success: function(data){
					if (data == "sama"){
						$("#validating").fadeOut("fast",function(){
							$("#barcode").validationEngine("showPrompt", "Barcode ini sudah digunakan..!!", "error", "topRight", true);
							$("#barcode").select();
							barcodesama = true;
						})
					}else{
						if (data == "taksama"){
							barcodesama = false;
						}
						$("#validating").fadeOut("fast");
					}
				}
			});
		})
	}		
}
function simpanproduk(namafile){
  // Tambahkan pengecekan barcode pada if untuk toko yang perlu
	//$("#barcode").val() != "" && !barcodesama && 
	if ($("#namaproduk").val() != "" && $("#hargapokok").val() > 0 && $("#hargajual").val() > 0){
		var request = new Object();
	 	request.barcode = $("#barcode").val();
	 	request.alt_code = $("#kodepoduk").val();
	 	request.idkategori = $("#idkategori").val();
	 	request.idsubkategori = $("#idsubkategori").val();
	 	request.idsupplier = $("#idsupplier").val();
	 	request.namaproduk = $("#namaproduk").val();
	 	request.hargapokok = $("#hargapokok").val();
	 	request.hargajual = $("#hargajual").val();
	 	request.margin = $("#margin").val();
	 	request.minstok = $("#minstok").val();
	 	request.maxstok = $("#maxstok").val();
	 	request.stok = $("#stok").val();
	 	request.satuan = $("#satuan").val();
	 	request.namafile = namafile;
	 	request.keterangan = $("#keteranganproduk").val();
	 	request.type_product = $("#type_product").val();
	 	request.lead_time = $("#lead_time").val();
	 	request.jam_kerja = $("#ikut_jam_kerja").val();
	 	request.sebelum_zuhur = $("#sebelum_zuhur").val();
		alamatsimpan = pathutama +"dataproduk/simpanproduk";
		$.ajax({
			type: "POST", 
			url: alamatsimpan,
			data: request,
			cache: false, 
			success: function(){
				$("#namaproduk").val("");
				$("#hargapokok").val("");
				$("#hargajual").val("");
				$("#margin").val("");
				$("#keteranganproduk").val("");
				$("#stok").val("");
				$("#minstok").val("");
				$("#maxstok").val("");
				ubahkodebarang();
				$("#barcode").val("");
				$("#refreshprodukbaru").click();

				$("#barcode").select();
			}
		});
	}else{
		$("#formproduk").validationEngine("validate");
	}
}
function editproduk(idproduk){
	if (idproduk > 0){
		window.location = pathutama + "dataproduk/editproduk?idproduk="+ idproduk;
	}
}
function view_status(kondisistok){
	window.location = pathutama + "dataproduk/produk?statusstok="+ kondisistok;
}
function save_produk(){
	var jumlah_file = 0;
	$(".uploadifyQueueItem").each(function(){
		jumlah_file++;
	});
	if (jumlah_file > 0){
		$("#uploadify").uploadifyUpload();
	}else{
		simpanproduk("tanpafile");
	}
}
$(document).ready(function() {
	pathutama = Drupal.settings.basePath;
	pathfile = Drupal.settings.globalpos.filepath;
	if (Drupal.settings.globalpos.adminislogin){
		columnSetting = [
			{ "bSortable": false },null,{ "bVisible": false },{ "bVisible": false },null,null,null,null,
			null,{ "bVisible": false },{ "bVisible": false },
			{ "bVisible": false },null,null,null,null,{ "bVisible": false }
		];
	}else{
		columnSetting = [
			null,{ "bVisible": false },{ "bVisible": false },null,null,null,
			null,null,null,null,null,{ "bVisible": false }
		];	
	}
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
	$("#dialogtambahsubkategori").dialog({
		modal: true,
		width: 350,
		resizable: false,
		autoOpen: false,
		position: ["auto","auto"],
		open: function(event, ui) {
			$("#kodesubkategori").focus();
		},
		close: function(){
			$("#tambahsubkategoriform").validationEngine("hide");
		}
	});
	$("#dialogtambahsupplier").dialog({
		modal: true,
		width: 350,
		resizable: false,
		autoOpen: false,
		position: ["auto","auto"],
		open: function(event, ui) {
			$("#kodesupplier").focus();
		},
		close: function(){
			$("#tambahsupplierform").validationEngine("hide");
		}
	});
	$("#dialogtambahsatuan").dialog({
		modal: true,
		width: 350,
		resizable: false,
		autoOpen: false,
		position: ["auto","auto"],
		open: function(event, ui) {
			$("#namasatuan").select();
		},
		close: function(){
			$("#tambahsatuanform").validationEngine("hide");
		}
	});
	TableToolsInit.sSwfPath = pathutama +"misc/media/datatables/swf/ZeroClipboard.swf";
	
	$("#formsubkategori").validationEngine();
	$("#tambahkategoriform").validationEngine();
	$("#tambahsubkategoriform").validationEngine();
	$("button").button();
	$("#addkategori").click(function(){
  	$("#dialogtambahkategori").dialog("open");
  	$(".ui-dialog-title").css("font-size","15px");
  	$("#formproduk").validationEngine("hide");
  	return false;
  });
  $("#addsubkategori").click(function(){
  	$("#dialogtambahsubkategori").dialog("open");
  	$("#idkategori2").val($("#idkategori").val());
  	$(".ui-dialog-title").css("font-size","15px");
  	$("#formproduk").validationEngine("hide");
  	return false;
  });
  $("#addsupplier").click(function(){
  	$("#dialogtambahsupplier").dialog("open");
  	$(".ui-dialog-title").css("font-size","15px");
  	$("#formproduk").validationEngine("hide");
  	return false;
  });
  $("#addsatuan").click(function(){
  	$("#dialogtambahsatuan").dialog("open");
  	$(".ui-dialog-title").css("font-size","15px");
  	$("#formproduk").validationEngine("hide");
  	return false;
  });
  $("#tambahprodukbaru").button({
    icons: {
        primary: "ui-icon-plusthick"
    },
    text: false
  }).click(function(){
  	$("#formpenambahanproduk").fadeIn("slow",function(){
  		filtersubkategori();
  		$("#barcode").select();
  	});
  });
  $("#refreshprodukbaru").button({
    icons: {
        primary: "ui-icon-refresh"
    },
    text: false
  }).click(function(){
  	$("#tempattabel").html("Memuat Data Produk...<img src = \""+ pathutama +"misc/media/images/loading.gif\">");
  	var request = new Object();
	 	request.asal = "produk"; 
		alamattabelproduk = pathutama +"dataproduk/isitableproduk";
		$.ajax({
			type: "POST", 
			url: alamattabelproduk,
			data: request,
			cache: false,
			success: function(data){
				$("#tempattabel").fadeOut("fast",function(){
					$("#tempattabel").html(data);
					tampilkantabelproduk();
					$("#tempattabel").fadeIn("fast");
				});
			}
		});
  });
  $("#kodekategori, #kodesubkategori, #kodesupplier, #namasatuan").autotab_filter({
		format: "alphanumeric",
		uppercase: true,
		nospace: true
	});
	$("#hargapokok,#hargajual,#margin,#minstok,#maxstok,#stok").autotab_filter({
		format: "numeric",
		nospace: true
	});
	$("#main h2").css("width","100%");
	$("#main h2").css("float","left");
	$("#barcode").keypress(function(e) {
	    if(e.keyCode == 13) {
	    	$("#formproduk").validationEngine("validateField", "#barcode");
	    	ubahkodebarang();
	    	$("#idkategori").focus();
	    }
	});
	$("#formproduk").validationEngine();
	$("#uploadify").uploadify({
		"uploader"       : pathutama + "sites/all/libraries/uploadify/uploadify.swf",
		"script"         : pathutama + "sites/all/libraries/uploadify/uploadxls.php",
		"cancelImg"      : pathutama + "sites/all/libraries/uploadify/images/cancel.png",
		"folder"         : pathfile,
		"fileExt"     	 : "*.jpg;*.gif;*.png",
		"fileDesc"    	 : "Image Files",
		"multi"          : false,
		"onComplete"  : function(event, ID, fileObj, response, data) {
			simpanproduk(fileObj.name);	
		},
		"onError" : function (event,ID,fileObj,errorObj) {
			console.log(fileObj.name);
		}
	});
	tampilkantabelproduk();
})