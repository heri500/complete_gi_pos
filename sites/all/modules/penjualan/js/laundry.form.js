var oTable;
var pathutama = '';
var giCount = 1;
var totalbelanja = 0;
var totalproduk = 0;
var barisrubah;
var tglsekarang = "'.$tglsekarang.'";
var tgltampil = "'.$tgltampil.'";
var pelanggansaatini = 0;
function tampilkantabelkasir(){
	oTable = $("#tabel_kasir").dataTable( {
		"bJQueryUI": true,
		"bPaginate": false,
		"bLengthChange": false,
		"bFilter": true,
		"bInfo": false,
		"sScrollY": "230px",
		"aoColumns": [
		{ "bSortable": false },{ "bSortable": false },null,null,null,null,null,null
		],
		"bAutoWidth": false
	});
}
function munculkankasir(){
	$("#dialogkasir").dialog("open");
}
function ubahqty(){
	if (totalproduk > 0){
		$("#dialogubahqty").dialog("open");
	}else{
		$("#pesantext").text("Mohon pilih produk terlebih dahulu...!!!");
		$("#dialogwarning").dialog("open");
	}
}
function ubah_qty_produk(posisi,nTr,idproduk){
	barisrubah = nTr;
	$("#dialogubahqty2").dialog("open");
}
function tambahproduk(){
	var request = new Object();
	var katacari = $("#barcode").val();
	var pecahkatacari = katacari.split("--->");
	request.katacari = pecahkatacari[0];
	request.idpelanggan = $("#idpelanggan").val();
	var tanggalJual = $("#tgljual").val();
	var splitTanggalJual = tanggalJual.split('-');
	tanggalJual = splitTanggalJual[2]+ '-' + splitTanggalJual[1] +'-'+ splitTanggalJual[0];
	request.waktumasuk = tanggalJual +' '+ $('#tempatjam span.clocktime').html();
	alamatcariproduk = pathutama +"penjualan/cariproduk";
	$.ajax({
		type: "POST",
		url: alamatcariproduk,
		data: request,
		cache: false,
		success: function(data){
			var pecahdata = new Array();
			pecahdata = data.split(";");
			if (pecahdata[0] != "error"){
				nilaisubtotal = pecahdata[2] - ((pecahdata[2]*pecahdata[3])/100);
				subtotal = number_format(nilaisubtotal,0,",",".");
				nilaikirim = pecahdata[0] +"___1___"+ pecahdata[2] +"___"+ pecahdata[3] +"___"+ pecahdata[5];
				index_cek_box = pecahdata[0];
				namacekbox = "cekbox_"+ index_cek_box;
				if($("#"+ namacekbox).val()){
					var nilaicekbox = $("#"+ namacekbox).val();
					var pecahnilai = nilaicekbox.split("___");
					var qtybaru = parseInt(pecahnilai[1]) + 1;
					var subtotallama = pecahnilai[1] * (pecahnilai[2] - (pecahnilai[2]*pecahnilai[3]/100));
					var subtotal = qtybaru * (pecahnilai[2] - (pecahnilai[2]*pecahnilai[3]/100));
					totalbelanja = totalbelanja - subtotallama + subtotal;
					$("#totalbelanja").html("Total Belanja : Rp. "+ number_format(totalbelanja,0,",","."));
					var nTr = $("#"+ namacekbox).parent().parent().get(0);
					var posisibaris = oTable.fnGetPosition(nTr);
					oTable.fnUpdate(qtybaru, posisibaris, 5 );
					nilaikirim = pecahnilai[0] +"___"+ qtybaru +"___"+ pecahnilai[2] +"___"+ pecahnilai[3] +"___"+ pecahnilai[4];
					checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
					oTable.fnUpdate(number_format(subtotal,0,",",".") +" "+ checkboxnilai, posisibaris, 6 );
					posisiakhir = totalproduk-1;
					if (posisibaris == posisiakhir){
						$("#lastqty").val(qtybaru);
					}
				}else{
					var icondelete = "<img onclick=\"hapus_produk(\'"+ index_cek_box +"\',this.parentNode.parentNode,\'"+ pecahdata[0] +"\')\" title=\"Klik untuk menghapus\" src=\""+ pathutama +"misc/media/images/close.ico\" width=\"24\">";
					var iconubah = "<img onclick=\"ubah_qty_produk(\'"+ index_cek_box +"\',this.parentNode.parentNode,\'"+ pecahdata[0] +"\')\" title=\"Klik untuk mengubah qty produk ini\" src=\""+ pathutama +"misc/media/images/edit.ico\" width=\"24\">";
					$("#lastharga").val(pecahdata[2]);
					$("#lastdiskon").val(pecahdata[3]);
					$("#last_id").val(pecahdata[0]);
					$("#lastqty").val("1");
					$("#lastperkiraan").val(pecahdata[5]);
					checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
					var row = "<tr id=\""+ index_cek_box +"\"><td>"+ icondelete +"</td><td>"+ iconubah +"</td><td>"+ pecahdata[1] +"</td><td class=\"angka\">"+ pecahdata[2] +"</td><td class=\"angka\">"+ pecahdata[3] +"</td><td class=\"angka\">1</td><td class=\"angka\">"+ subtotal +" "+ checkboxnilai +"</td><td>"+ pecahdata[4] +"</td></tr>";
					$("#tabel_kasir").dataTable().fnAddTr($(row)[0]);
					giCount++;
					totalproduk++;
					totalbelanja = totalbelanja + nilaisubtotal;
					$("#totalbelanja").html("Total Belanja : Rp. "+ number_format(totalbelanja,0,",","."));
				}
				$('.dataTables_scrollBody').scrollTop($('.dataTables_scrollBody')[0].scrollHeight);
				$("#barcode").autocomplete("close");
				$("#barcode").select();
			}else{
				$("#pesantext").text("Produk yang dicari atau diinput tidak ada, silahkan masukkan barcode atau kode atau nama produk yang lain...!!!");
				$("#dialogwarning").dialog("open");
			}
		}
	});
}
function kirim_data(){
	if (totalproduk > 0 && $("#idpelanggan").val() != 0){
		var sData = $("input", oTable.fnGetNodes()).serialize();
		$("#nilaikirim").val(decodeURIComponent(sData));
		$("#dialogbayar").dialog("open");
	}else{
		if (totalproduk < 0 && $("#idpelanggan").val() != 0){
			$("#pesantext").text("Mohon pilih produk/jasa laundry terlebih dahulu...!!!");
		}else if(totalproduk > 0 && $("#idpelanggan").val() == 0){
			$("#pesantext").text("Mohon pilih pelanggan terlebih dahulu...!!!");
		}else{
			$("#pesantext").text("Mohon pilih produk/jasa laundry dan pelanggan terlebih dahulu...!!!");
		}
		$("#dialogwarning").dialog("open");
	}
}
function tutupwarning(){
	$("#dialogwarning").dialog("close");
}
function hapus_latest_produk(){
	if (totalproduk > 0){
		oTable.fnDeleteRow(totalproduk-1);
		totalproduk--;
		if (totalproduk >= 0){
			totalbelanja = totalbelanja - ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
			$("#totalbelanja").html("Total Belanja : Rp. "+ number_format(totalbelanja,0,",","."));
			var nTr = oTable.fnGetNodes(totalproduk-1);
			idproduknya = nTr.getAttribute("id");
			var nilaidataakhir = $("#cekbox_"+ idproduknya).val();
			var pecahnilaiakhir = nilaidataakhir.split("___");
			$("#lastdiskon").val(pecahnilaiakhir[3]);
			$("#lastharga").val(pecahnilaiakhir[2]);
			$("#lastqty").val(pecahnilaiakhir[1]);
			$("#last_id").val(pecahnilaiakhir[0]);
			$("#lastperkiraan").val(pecahnilaiakhir[4]);
		}else{
			$("#lastdiskon").val("");
			$("#lastharga").val("");
			$("#lastqty").val("");
			$("#last_id").val("");
			$("#lastperkiraan").val("");
		}
	}else{
		$("#pesantext").text("Mohon pilih produk terlebih dahulu...!!!");
		$("#dialogwarning").dialog("open");
	}
}
function focusbarcode(){
	$("#barcode").select();
}
function hapus_produk(posisi,nTr,idproduk){
	var posisibaris = oTable.fnGetPosition(nTr);
	var nilaidata = $("#cekbox_"+ idproduk).val();
	var pecahnilai = nilaidata.split("___");
	totalbelanja = totalbelanja - (pecahnilai[1]*(pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100)));
	$("#totalbelanja").html("Total Belanja : Rp. "+ number_format(totalbelanja,0,",","."));
	oTable.fnDeleteRow(posisibaris,focusbarcode);
	totalproduk--;
	if (totalproduk > 0){
		var nilaidataakhir = $("#cekbox_"+ totalproduk).val();
		var pecahnilaiakhir = nilaidataakhir.split("___");
		$("#lastdiskon").val(pecahnilaiakhir[3]);
		$("#lastharga").val(pecahnilaiakhir[2]);
		$("#lastqty").val(pecahnilaiakhir[1]);
		$("#last_id").val(pecahnilaiakhir[0]);
		$("#lastperkiraan").val(pecahnilaiakhir[4]);
	}else{
		$("#lastdiskon").val("");
		$("#lastharga").val("");
		$("#lastqty").val("");
		$("#last_id").val("");
		$("#lastperkiraan").val("");
	}
	$("#barcode").focus();
	$("#barcode").select();
}
function akhiri_belanja(cetak){
	var request = new Object();
	request.detail_produk = $("#nilaikirim").val();
	request.idpelanggan = $("#idpelanggan").val();
	request.totalbelanja = totalbelanja;
	if ($("#carabayar").val() == 'KEMUDIAN'){
		request.bayar = 0;
	}else{
		request.bayar = $("#nilaibayar").val();
	}
	request.carabayar = $("#carabayar").val();
	request.nomerkartu = $("#nomerkartu").val();
	request.tgljual = $("#tgljualkirim").val();
	request.keterangan = $("#keterangan").val();
	alamat = pathutama + "penjualan/simpanlaundry";
	$.ajax({
		type: "POST",
		url: alamat,
		data: request,
		cache: false,
		success: function(data){
			var returndata = data.trim();
			if (returndata != "error"){
                                if (cetak == 1){
					window.open(pathutama + "print/6?nidlaundry="+ returndata);
				}
				window.location = pathutama + "penjualan/laundry?tanggal="+ request.tgljual;
			}else{
				alert("Ada masalah dalam penyimpanan data...!!!");
			}
		}
	});
}
function hitung_omset(){
	var request = new Object();
	request.tglpost = tglsekarang;
	alamat = pathutama + "penjualan/hitungomset";
	$.ajax({
		type: "POST",
		url: alamat,
		data: request,
		cache: false,
		success: function(data){
			var omsetsekarang = number_format(data,0,",",".");
			$("#pesantext").text("OMSET HARI INI ["+ tgltampil +"] : Rp. "+ omsetsekarang);
			$("#dialogwarning").dialog("open");
		}
	});
}
function inisialulang(){
	if (totalproduk > 0){
		if (pelanggansaatini != 0){
			var tanya = confirm("Merubah pelanggan berarti menghapus pembelian yang terinput, YAKIN..!!");
			if (tanya != 0){
				window.location = pathutama + "penjualan/kasir?idpelanggan="+ $("#idpelanggan").val();
			}else{
				$("#barcode").select();
			}
		}else{
			pelanggansaatini = $("#idpelanggan").val();
		}
	}else{
		$("#barcode").select();
	}
}
function ubahharga(){
	if (totalproduk > 0){
		$("#dialogubahharga").dialog("open");
	}else{
		$("#pesantext").text("Mohon pilih produk terlebih dahulu...!!!");
		$("#dialogwarning").dialog("open");
	}
}
$(document).ready(function(){
	pathutama = Drupal.settings.basePath;
	$("#dialogkasir").dialog({
		modal: true,
		width: 925,
		closeOnEscape: false,
		height: 560,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$("#tempatjam").clock({"format":"24","calendar":"false"});
			$("#barcode").focus();
		},
		position: ["auto","auto"]
	});
	$("#dialogwarning").dialog({
		modal: true,
		width: 400,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$("#tutupdialog").focus();
		},
		close: function(){
			$("#barcode").select();
		},
		position: ["auto","auto"]
	});
	$("#dialogubahharga").dialog({
		modal: true,
		width: 250,
		height: 100,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$("#newharga").val($("#lastharga").val());
			$("#newharga").select();
		},
		close: function(){
			$("#barcode").select();
		},
		position: ["auto","auto"]
	});
	$("#dialogubahqty").dialog({
		modal: true,
		width: 250,
		height: 100,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$("#newqty").val($("#lastqty").val());
			$("#newqty").select();
		},
		close: function(){
			$("#barcode").select();
		},
		position: ["auto","auto"]
	});
	$("#dialogubahqty2").dialog({
		modal: true,
		width: 250,
		height: 100,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			idproduknya = barisrubah.getAttribute("id");
			var nilaidata = $("#cekbox_"+ idproduknya).val();
			var pecahnilai = nilaidata.split("___");
			$("#newqty2").val(pecahnilai[1]);
			$("#newqty2").select();
		},
		close: function(){
			$("#barcode").select();
		},
		position: ["auto","auto"]
	});
	$("#dialogbayar").dialog({
		modal: true,
		width: 550,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$("#totalbelanjauser").val("Rp. "+ number_format(totalbelanja,0,",","."));
			if (totalbelanja > 0 && totalbelanja <= 10000){
				$("#nilaibayar").val("10000");
			}else if(totalbelanja > 10000 && totalbelanja <= 20000){
				$("#nilaibayar").val("20000");
			}else if(totalbelanja > 20000 && totalbelanja <= 50000){
				$("#nilaibayar").val("50000");
			}else if(totalbelanja > 50000 && totalbelanja <= 100000){
				$("#nilaibayar").val("100000");
			}else if(totalbelanja > 100000 && totalbelanja <= 110000){
				$("#nilaibayar").val("110000");
			}else if(totalbelanja > 110000 && totalbelanja <= 120000){
				$("#nilaibayar").val("120000");
			}else if(totalbelanja > 120000 && totalbelanja <= 150000){
				$("#nilaibayar").val("150000");
			}else if(totalbelanja > 150000 && totalbelanja <= 200000){
				$("#nilaibayar").val("200000");
			}else{
				$("#nilaibayar").val(totalbelanja);
			}
			kembali = $("#nilaibayar").val() - totalbelanja;
			$("#kembali").val("Rp. "+ number_format(kembali,0,",","."));
			$("#nilaibayar").select();
			alamat = pathutama + "datapelanggan/gettotalhutang/"+ $("#idpelanggan").val();
			$.ajax({
				type: "POST",
				url: alamat,
				cache: false,
				success: function(data){
					var returnData = eval(data);
					var totalHutang = returnData[0];
					if (totalHutang < 0){
						$('#label-deposit').html('Deposit');
					}else{
						$('#label-deposit').html('Hutang');
					}
					$('#depositpelanggan').val(Math.abs(totalHutang));
				}
			})
		},
		close: function(){
			$("#barcode").select();
		},
		position: ["auto","auto"]
	});
	tampilkantabelkasir();
	$("#dialogkasir").dialog("open");
	$(".ui-dialog-titlebar").css("font-size","14px");
	$("button").button();
	$("#barcode").keypress(function(e) {
		console.log(e.keyCode);
		if (e.keyCode == 114){
			$("#tombolubahqty").click();
		}else if (e.keyCode == 13){
			if ($("#barcode").val() != ""){
				tambahproduk();
			}else{
				$("#pesantext").text("Mohon isi barcode atau kode produk atau nama produk yang ingin dicari...!!!");
				$("#dialogwarning").dialog("open");
			}
		}else if (e.keyCode == 116 || e.keyCode == 117){
			kirim_data();
		}else if (e.keyCode == 115){
			hapus_latest_produk();
		}else if (e.keyCode == 119){
			hitung_omset();
		}else if (e.keyCode == 113){
			if ($("#idpelanggan").val() == 0){
				$("#tombolubahharga").click();
			}else{
				$("#pesantext").text("Perubahan harga hanya untuk pelanggan UMUM...!!!");
				$("#dialogwarning").dialog("open");
			}
		}
	});
	$("#barcode").autocomplete({
		source: pathutama + "penjualan/autocaribarang",
        minLength: 2,
		select: function (event, ui) {
			console.log(ui.item)
			if (ui.item.barcode){
				$("#barcode").val(ui.item.barcode);
			}else if(!ui.item.barcode && ui.item.alt_code){
				$("#barcode").val(ui.item.alt_code);
			}else if(!ui.item.barcode && !ui.item.alt_code){
				$("#barcode").val(ui.item.value);
			}
			tambahproduk(1);
		}
	});
	$("#newqty").autotab_filter({
		format: "numeric",
		nospace: true
	});
	$("#newharga").keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = totalproduk-1;
			totalbelanja = totalbelanja - ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
			var nilaiubah = $("#newharga").val();
			oTable.fnUpdate(nilaiubah, baris_int, 3 );
			nilaisubtotal = (nilaiubah-(nilaiubah*$("#lastdiskon").val()/100))*$("#lastqty").val();
			subtotalbaru = number_format(nilaisubtotal,0,",",".");
			var namacekbox = "cekbox_"+ $("#last_id").val();
			var nilaikirim = $("#last_id").val() +"___"+ $("#lastqty").val() +"___"+ nilaiubah +"___"+ $("#lastdiskon").val() +"___"+ $("#lastperkiraan").val();
			var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
			oTable.fnUpdate(subtotalbaru +" "+ checkboxnilai, baris_int, 6 );
			$("#lastharga").val(nilaiubah);
			totalbelanja = totalbelanja + nilaisubtotal;
			$("#totalbelanja").html("Total Belanja : Rp. "+ number_format(totalbelanja,0,",","."));
			$("#dialogubahharga").dialog("close");
		}
	});
	$("#newqty").keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = totalproduk-1;
			totalbelanja = totalbelanja - ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
			var nilaiubah = $("#newqty").val();
			oTable.fnUpdate(nilaiubah, baris_int, 5 );
			nilaisubtotal = ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*nilaiubah;
			subtotalbaru = number_format(nilaisubtotal,0,",",".");
			var namacekbox = "cekbox_"+ $("#last_id").val();
			var nilaikirim = $("#last_id").val() +"___"+ nilaiubah +"___"+ $("#lastharga").val() +"___"+ $("#lastdiskon").val() +"___"+ $("#lastperkiraan").val();
			var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
			oTable.fnUpdate(subtotalbaru +" "+ checkboxnilai, baris_int, 6 );
			$("#lastqty").val(nilaiubah);
			totalbelanja = totalbelanja + nilaisubtotal;
			$("#totalbelanja").html("Total Belanja : Rp. "+ number_format(totalbelanja,0,",","."));
			$("#dialogubahqty").dialog("close");
		}
	});
	$("#newqty2").keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = oTable.fnGetPosition(barisrubah);
			var idproduknya = barisrubah.getAttribute("id");
			var nilaidata = $("#cekbox_"+ idproduknya).val();
			var pecahnilai = nilaidata.split("___");
			totalbelanja = totalbelanja - (pecahnilai[1]*(pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100)));
			var nilaiubah = $("#newqty2").val();
			oTable.fnUpdate(nilaiubah, baris_int, 5 );
			nilaisubtotal = (pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100))*nilaiubah;
			subtotalbaru = number_format(nilaisubtotal,0,",",".");
			var namacekbox = "cekbox_"+ idproduknya;
			var nilaikirim = idproduknya +"___"+ nilaiubah +"___"+ pecahnilai[2] +"___"+ pecahnilai[3] +"___"+ pecahnilai[4];
			var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
			oTable.fnUpdate(subtotalbaru +" "+ checkboxnilai, baris_int, 6 );
			totalbelanja = totalbelanja + nilaisubtotal;
			posisiakhir = totalproduk-1;
			if (baris_int == posisiakhir){
				$("#lastqty").val(nilaiubah);
			}
			$("#totalbelanja").html("Total Belanja : Rp. "+ number_format(totalbelanja,0,",","."));
			$("#dialogubahqty2").dialog("close");
		}
	});
	$("#totalbelanjauser").keyup(function(e){
		kembali = $("#nilaibayar").val()-totalbelanja;
		$("#kembali").val("Rp. "+ number_format(kembali,0,",","."));
		if (e.keyCode == 13){
			akhiri_belanja(1);
		}
	});
	$("#tgljual").datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: "dd-mm-yy",
		altField: "#tgljualkirim",
		altFormat: "yy-mm-dd",
		onClose: function(dateText, inst) {
			$("#barcode").select();
		}
	});
	$("#tgljual").css("width","177px");
	$("#carabayar").change(function(){
		if ($(this).val() == 'DEBIT' || $(this).val() == 'GIRO'){
			$("#field_no_kartu").show();
			$("#field_bayar").show();
			$("#nilaibayar").val(totalbelanja).attr('readonly','readonly').removeAttr('disabled'); 
			//$("#field_kembali").hide();
			//$("#kembali").attr('disabled','disabled');
			$("#nomerkartu").select();
		}else if ($(this).val() == 'KEMUDIAN'){
			$("#field_no_kartu").hide();
			$("#field_bayar").hide();
			//$("#field_kembali").hide();
			//$("#kembali").attr('disabled','disabled');
			$("#nilaibayar").attr('disabled','disabled');
		}else{
			$("#field_no_kartu").hide();
			$("#field_bayar").show();
			//$("#field_kembali").show();
			$("#nilaibayar").removeAttr('readonly').removeAttr('disabled');
			//$("#kembali").removeAttr('disabled');
			//$("#kembali").val($("#nilaibayar").val() - totalbelanja);
			$("#nilaibayar").select();
		}
	});
	$("#carabayar").change();
	$("#nilaibayar").keyup(function(e){
		if (e.keyCode == 13){
			$("#simpan-cetak-laundry").click(1);
		}
	});
	$("#simpan-laundry").click(function(){
		akhiri_belanja(0);
	});
        $("#simpan-cetak-laundry").click(function(){
		akhiri_belanja(1);
	});
	$('#idpelanggan').chosen();	
	$("#dialogtambahpelanggan").dialog({
		modal: true,
		width: 550,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			
		},
		close: function(){
			$("#barcode").select();
		},
		position: ["auto","auto"]
	});
	$("#add-pelanggan").click(function(){
		$("#dialogtambahpelanggan").dialog('open');
	});
	$("#save-pelanggan").click(function(e){
		e.preventDefault();
		var request = new Object;
		request.kodepelanggan = $('#kodepelanggan').val();
		request.namapelanggan = $('#namapelanggan').val();
		request.telp = $('#telp').val();
		request.alamat = $('#alamat').val();
		request.asal = 'laundry-masuk';
		alamat = pathutama + "datapelanggan/simpanpelanggan";
		$.ajax({
			type: "POST",
			url: alamat,
			data: request,
			cache: false,
			success: function(data){
				var idPelanggan = parseInt(data.trim());
				$('#kodepelanggan').val('');
				$('#namapelanggan').val('');
				$('#telp').val('');
				$('#alamat').val('');
				$("#dialogtambahpelanggan").dialog("close");
				$('#idpelanggan').append('<option value="'+ idPelanggan +'" selected>'+ request.kodepelanggan +' - '+ request.namapelanggan +' ['+ request.telp +']</option>');
				$('#idpelanggan').trigger('chosen:updated');
			}
		});
	});
	$('#info-kasir-waktu').css('background','url('+ Drupal.settings.logo +') 99% 50% no-repeat');
	$('#info-kasir-waktu').css('background-size','75px 75px');
})