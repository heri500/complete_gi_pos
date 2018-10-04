var oTable;
var pathutama = '';
var giCount = 1;
var totalbelanja = 0;
var totalproduk = 0;
var barisrubah;
var tglsekarang = '';
var tgltampil = '';
var cetakstruk = 0;
var alamatasal = '';
var ppnValue = 0;
var currSym = '';
var tSep = '.';
var dSep = ',';
var dDigit = 0;
var IdPenjualan = 0;

function tampilkantabelkasir(){
	oTable = $("#tabel_kasir").dataTable( {
		"bJQueryUI": true,
		"bPaginate": false,
		"bLengthChange": false,
		"bFilter": true,
		"bInfo": false,
		"sScrollY": "270px",
		"aoColumns": [
		{ "bSortable": false },{ "bSortable": false },null,null,null,null,null,{ "bSortable": false }
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
function tambahproduk(qtyAdd, batch_code){
	var request = new Object();
	if ($('#hiddenbarcode').val() != ''){
		var katacari = $("#hiddenbarcode").val();
	}else{
		var katacari = $("#barcode").val();
	}
	var pecahkatacari = katacari.split("--->");
	request.katacari = pecahkatacari[0];
	request.idpelanggan = $("#idpelanggan").val();
	alamatcariproduk = pathutama +"penjualan/cariproduk";
	$.ajax({
		type: "POST",
		url: alamatcariproduk,
		data: request,
		cache: false,
		success: function(data){
			var pecahdata = new Array();
			pecahdata = data.split(";");
            if (pecahdata[0].trim() != "error"){
				nilaisubtotal = (pecahdata[2] - ((pecahdata[2]*pecahdata[3])/100)) * qtyAdd;
				subtotal = number_format(nilaisubtotal,dDigit,dSep,tSep);
				nilaikirim = pecahdata[0].trim() +"___"+ qtyAdd +"___"+ pecahdata[2] +"___"+ pecahdata[3] +"___"+ pecahdata[6] ;
				index_cek_box = pecahdata[0].trim();
				namacekbox = "cekbox_"+ index_cek_box;
				if($("#"+ namacekbox).val()){
					var nilaicekbox = $("#"+ namacekbox).val();
					var pecahnilai = nilaicekbox.split("___");
					var qtybaru = parseFloat(pecahnilai[1]) + 1;
					var subtotallama = pecahnilai[1] * (pecahnilai[2] - (pecahnilai[2]*pecahnilai[3]/100));
					var subtotal = qtybaru * (pecahnilai[2] - (pecahnilai[2]*pecahnilai[3]/100));
					totalbelanja = totalbelanja - subtotallama + subtotal;
                    $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(Math.abs(totalbelanja),dDigit,dSep,tSep));
					var nTr = $("#"+ namacekbox).parent().parent().get(0);
					var posisibaris = oTable.fnGetPosition(nTr);
					oTable.fnUpdate(qtybaru, posisibaris, 5 );
					nilaikirim = pecahnilai[0].trim() +"___"+ qtybaru +"___"+ pecahnilai[2] +"___"+ pecahnilai[3] +"___"+ pecahdata[6] ;
					checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" class=\"checkbox-produk\" />";
                    subtotalView = number_format(Math.abs(subtotal),dDigit,dSep,tSep);
					oTable.fnUpdate(subtotalView +" "+ checkboxnilai, posisibaris, 6 );
					posisiakhir = totalproduk-1;
					if (posisibaris == posisiakhir){
						$("#lastqty").val(qtybaru);
					}
					if (typeof pecahnilai[6] != 'undefined'){
						$("#hiddenbarcode").val(pecahnilai[6].trim());
					}
				}else{
					var icondelete = "<img onclick=\"hapus_produk(\'"+ index_cek_box +"\',this.parentNode.parentNode,\'"+ pecahdata[0].trim() +"\')\" title=\"Klik untuk menghapus\" src=\""+ pathutama +"misc/media/images/close.ico\" width=\"24\" class=\"hapus-product\" >";
					var iconubah = "<img onclick=\"ubah_qty_produk(\'"+ index_cek_box +"\',this.parentNode.parentNode,\'"+ pecahdata[0].trim() +"\')\" title=\"Klik untuk mengubah qty produk ini\" src=\""+ pathutama +"misc/media/images/edit.ico\" width=\"24\">";
                    var iconharga = "<img onclick=\"ubah_harga_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah harga produk ini\" src=\"" + pathutama + "misc/media/images/money2.png\" width=\"24\">";

					$("#lastharga").val(pecahdata[2]);
					$("#lastdiskon").val(pecahdata[3]);
					$("#last_id").val(pecahdata[0].trim());
					$("#lastbarcode").val(pecahdata[6]);
					$("#lastqty").val(qtyAdd);
					checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" class=\"checkbox-produk\" />";
					var row = "<tr id=\""+ index_cek_box +"\">";
					row += "<td>"+ icondelete +"</td>";
					row += "<td>"+ iconubah +"</td>";
					row += "<td>"+ pecahdata[1] +"</td>";
					row += "<td class=\"angka\">"+ number_format(pecahdata[2],dDigit,dSep,tSep) +"</td>";
					row += "<td class=\"angka\">"+ pecahdata[3] +"</td>";
					row += "<td class=\"angka\">"+ qtyAdd +"</td>";
					row += "<td class=\"angka\">"+ subtotal +" "+ checkboxnilai +"</td>";
                    row += "<td>" + iconharga + "</td>";
					row += "</tr>";
					$("#tabel_kasir").dataTable().fnAddTr($(row)[0]);
					giCount++;
					totalproduk++;
					totalbelanja = totalbelanja + nilaisubtotal;
                    totalbelanjaView = number_format(Math.abs(totalbelanja),dDigit,dSep,tSep);
					$("#totalbelanja").html("Total : "+ currSym +" "+ totalbelanjaView);
				}
				$('.dataTables_scrollBody').scrollTop($('.dataTables_scrollBody')[0].scrollHeight);
				$("#barcode").autocomplete("close");
				$("#barcode").select();
			}else{
                var BarcodeProduct = request.katacari.substr(0,7);
                var BarcodeCheck = request.katacari.substr(0,2);
                if (BarcodeCheck == '22') {
                    var ProductPrice = parseInt(request.katacari.substr(7, 5)) / 100;
                    var request2 = new Object();
                    prodTotal = parseFloat(ProductPrice.toFixed(2));
                    request2.katacari = BarcodeProduct.substr(2,5);
                    request2.idpelanggan = $("#idpelanggan").val();
                    request2.freshproduct = 1;
                    alamatcariproduk = pathutama + "penjualan/cariproduk";
                    $.ajax({
                        type: "POST",
                        url: alamatcariproduk,
                        data: request2,
                        cache: false,
                        success: function (data2) {
                            var pecahdata = new Array();
                            pecahdata = data2.split(";");
                            if (pecahdata[0].trim() != "error") {
                                var IdProduct = pecahdata[0].trim();
                                qtyAdd = prodTotal/(pecahdata[2]);
                                qtyAdd = qtyAdd.toFixed(3);
                                nilaisubtotal = (pecahdata[2] - ((pecahdata[2] * pecahdata[3]) / 100)) * qtyAdd;
                                subtotal = number_format(nilaisubtotal,dDigit,dSep,tSep);
                                nilaikirim = IdProduct + "___" + qtyAdd + "___" + pecahdata[2] + "___" + pecahdata[3] + "___" + pecahdata[6];
                                index_cek_box = IdProduct;
                                namacekbox = "cekbox_" + index_cek_box;
                                if ($("#" + namacekbox).val()) {
                                    var nilaicekbox = $("#" + namacekbox).val();
                                    var pecahnilai = nilaicekbox.split("___");
                                    var qtybaru = parseFloat(pecahnilai[1]) + qtyAdd;
                                    qtybaru = parseFloat(qtybaru.toFixed(3));
                                    var subtotallama = pecahnilai[1] * (pecahnilai[2] - (pecahnilai[2] * pecahnilai[3] / 100));
                                    var subtotal = qtybaru * (pecahnilai[2] - (pecahnilai[2] * pecahnilai[3] / 100));
                                    var subtotal = parseFloat(subtotal.toFixed(2));
                                    totalbelanja = parseFloat(totalbelanja) - parseFloat(subtotallama) + subtotal;
                                    $("#totalbelanja").html("Total Transaksi : " + currSym + " " + number_format(Math.abs(totalbelanja),dDigit,dSep,tSep));
                                    var nTr = $("#" + namacekbox).parent().parent().get(0);
                                    var posisibaris = oTable.fnGetPosition(nTr);
                                    oTable.fnUpdate(qtybaru, posisibaris, 5);
                                    nilaikirim = pecahnilai[0].trim() + "___" + qtybaru + "___" + pecahnilai[2] + "___" + pecahnilai[3] + "___" + pecahdata[6];
                                    checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" class=\"checkbox-produk\" />";
                                    oTable.fnUpdate(number_format(Math.abs(subtotal),dDigit,dSep,tSep) + " " + checkboxnilai, posisibaris, 6);
                                    posisiakhir = totalproduk - 1;
                                    if (posisibaris == posisiakhir) {
                                        $("#lastqty").val(qtybaru);
                                    }
                                    if (typeof pecahnilai[6] != 'undefined') {
                                        $("#hiddenbarcode").val(pecahnilai[6].trim());
                                    }
                                    $("#barcode").autocomplete("close");
                                    $("#barcode").select();
                                } else {
                                    var icondelete = "<img onclick=\"hapus_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk menghapus\" src=\"" + pathutama + "misc/media/images/close.ico\" width=\"24\" class=\"hapus-product\" >";
                                    var iconubah = "<img onclick=\"ubah_qty_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah qty produk ini\" src=\"" + pathutama + "misc/media/images/edit.ico\" width=\"24\">";
                                    var iconharga = "<img onclick=\"ubah_harga_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah harga produk ini\" src=\"" + pathutama + "misc/media/images/money2.png\" width=\"24\">";

                                    $("#lastharga").val(pecahdata[2]);
                                    $("#lastdiskon").val(pecahdata[3]);
                                    $("#last_id").val(IdProduct);
                                    $("#lastbarcode").val(pecahdata[6]);
                                    $("#lastqty").val(qtyAdd);
                                    checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" class=\"checkbox-produk\" />";
                                    var row = "<tr id=\"" + index_cek_box + "\">";
                                    row += "<td>" + icondelete + "</td>";
                                    row += "<td>" + iconubah + "</td>";
                                    row += "<td>" + pecahdata[1] + "</td>";
                                    row += "<td class=\"angka\">" + pecahdata[2] + "</td>";
                                    row += "<td class=\"angka\">" + pecahdata[3] + "</td>";
                                    row += "<td class=\"angka\">" + qtyAdd + "</td>";
                                    row += "<td class=\"angka\">" + subtotal + " " + checkboxnilai + "</td>";
                                    row += "<td>" + iconharga + "</td>";
                                    row += "</tr>";
                                    $("#tabel_kasir").dataTable().fnAddTr($(row)[0]);
                                    giCount++;
                                    totalproduk++;
                                    totalbelanja = parseFloat(totalbelanja) + parseFloat(nilaisubtotal);
                                    $("#totalbelanja").html("Total Transaksi : " + currSym + " " + number_format(Math.abs(totalbelanja),dDigit,dSep,tSep));
                                }
                                $('.dataTables_scrollBody').scrollTop($('.dataTables_scrollBody')[0].scrollHeight);
                                $("#barcode").autocomplete("close");
                                $("#barcode").select();
                            } else {
                                $("#pesantext").text("Produk yang dicari atau diinput tidak ada, silahkan masukkan barcode atau kode atau nama produk yang lain...!!!");
                                $("#dialogwarning").dialog("open");
                            }
                        }
                    });
                }else{
                    $("#pesantext").text("Produk yang dicari atau diinput tidak ada, silahkan masukkan barcode atau kode atau nama produk yang lain...!!!");
                    $("#dialogwarning").dialog("open");
                }
			}
            $("#hiddenbarcode").val("");
		}
	});
}
function kirim_data(cetaknota){
	cetakstruk = cetaknota;
	if (totalproduk > 0){
		var sData = $("input", oTable.fnGetNodes()).serialize();
		$("#nilaikirim").val(decodeURIComponent(sData));
		$("#dialogbayar").dialog("open");
	}else{
		$("#pesantext").text("Mohon pilih produk terlebih dahulu...!!!");
		$("#dialogwarning").dialog("open");
	}
}
function tutupwarning(){
	$("#dialogwarning").dialog("close");
}
function hapus_latest_produk(){
	if (totalproduk > 0){
        totalbelanja = totalbelanja - ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
        $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(Math.abs(totalbelanja),dDigit,dSep,tSep));
		oTable.fnDeleteRow(totalproduk-1);
		totalproduk--;
		if (totalproduk > 0){
			var nTr = oTable.fnGetNodes(totalproduk-1);
            idproduknya = nTr.getAttribute("id");
            var nilaidataakhir = $("#cekbox_"+ idproduknya).val();
			var pecahnilaiakhir = nilaidataakhir.split("___");
			$("#lastdiskon").val(pecahnilaiakhir[3]);
			$("#lastharga").val(pecahnilaiakhir[2]);
			$("#lastqty").val(pecahnilaiakhir[1]);
			$("#last_id").val(pecahnilaiakhir[0]);
			$("#lastbarcode").val(pecahnilaiakhir[4]);
		}else{
			$("#lastdiskon").val("");
			$("#lastharga").val("");
			$("#lastqty").val("");
			$("#last_id").val("");
			$("#lastbarcode").val("");
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
    $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
	oTable.fnDeleteRow(posisibaris,focusbarcode);
	totalproduk--;
	if (totalproduk > 0){
        var nTr = oTable.fnGetNodes(totalproduk-1);
        idproduknya = nTr.getAttribute("id");
		var nilaidataakhir = $("#cekbox_"+ idproduknya).val();
		var pecahnilaiakhir = nilaidataakhir.split("___");
		$("#lastdiskon").val(pecahnilaiakhir[3]);
		$("#lastharga").val(pecahnilaiakhir[2]);
		$("#lastqty").val(pecahnilaiakhir[1]);
		$("#last_id").val(pecahnilaiakhir[0]);
		$("#lastbarcode").val(pecahnilaiakhir[4]);
	}else{
		$("#lastdiskon").val("");
		$("#lastharga").val("");
		$("#lastqty").val("");
		$("#last_id").val("");
		$("#lastbarcode").val("");
	}
	$("#barcode").focus();
	$("#barcode").select();
}
function akhiri_belanja(cetak){
	var request = new Object();
	if (typeof Drupal.settings.idtitipanlaundry != 'undefined'){
		if (alamatasal == 'viewlaundry' || alamatasal == 'laundrykeluar'){
			request.idtitipanlaundry = Drupal.settings.idtitipanlaundry;
		}else if (alamatasal == 'viewcustomerorder' || alamatasal == 'orderkeluar'){
			request.idcustomerorder = Drupal.settings.idtitipanlaundry;
		}

	}
	request.detail_produk = $("#nilaikirim").val();
	$("#idpelanggan").removeAttr("disabled");
	request.idpelanggan = $("#idpelanggan").val();
	request.totalbelanja = totalbelanja;
	request.ppn = $("#ppn_value").val();
	var totalbelanjappn = (totalbelanja * ($("#ppn_value").val()/100)) + totalbelanja;
	request.totalbelanjappn = totalbelanjappn;
	request.carabayar = $("#carabayar").val().join();
	var BayarMultiple = new Array;
	var CaraBayar = $("#carabayar").val();
	var TotalCounter = CaraBayar.length;
	for (var i = 0;i < TotalCounter;i++){
		if (CaraBayar[i] == 'TUNAI'){
            BayarMultiple.push(CaraBayar[i] +':'+ $('#nilaibayar').val());
		}else if (CaraBayar[i] == 'DEBIT'){
            BayarMultiple.push(CaraBayar[i] +':'+ $('#nilai_bayar_debit').val());
		}else if (CaraBayar[i] == 'KAD KREDIT'){
            BayarMultiple.push(CaraBayar[i] +':'+ $('#nilai_bayar_kredit').val());
		}else if (CaraBayar[i] == 'HUTANG'){
            BayarMultiple.push(CaraBayar[i] +':'+ $('#nilai_bayar_hutang').val());
        }
	}
	request.bayar_multiple = BayarMultiple.join(';');
	var TotalBayar = calculate_total_bayar();
    request.bayar = TotalBayar;
	var kembalian = request.bayar - totalbelanja;
	if (kembalian > 0){
		request.perlakuankembalian = $("#kembalian").val();
	}
	if ($("#idpelanggan").val() == 0 && $("#kembalian").val() == 2){
		request.perlakuankembalian = 0;
	}
	request.tgljual = $("#tgljualkirim").val();
	//request.idmeja = //$('#idmeja').val();
	if ((kembalian < 0 && $("#idpelanggan").val() != 0) || kembalian >= 0){
		alamat = pathutama + "penjualan/simpanpenjualan";
		$.ajax({
			type: "POST",
			url: alamat,
			data: request,
			cache: false,
			success: function(data){
				var returndata = data.trim();
				if (returndata != "error"){
                    IdPenjualan = returndata;
                    $('#dialogbayar').parent().unblock();
                    $('#selesai-transaksi').button("enable");
                    $('#selesai-transaksi').focus();
				}else{
					alert("Ada masalah dalam penyimpanan data...!!!");
				}
			}
		});
	}else{
		alert("Mohon pilih pelanggan terlebih dulu jika pembayaran dengan cara hutang...!!!");
	}
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
            var omsetsekarang = parseFloat(data).toFixed(dDigit);
			$("#pesantext").text("OMSET/JUALAN HARI INI ["+ tgltampil +"] : "+ currSym +" "+ omsetsekarang);
			$("#dialogwarning").dialog("open");
		}
	});
}
function inisialulang(){
	console.log(totalproduk);
	if (totalproduk > 0){
		var addedProduct = new Array;
		var selectedProduct = new Array;
		$('.checkbox-produk').each(function(){
			addedProduct.push($(this).val());
			var splitData = $(this).val().split('___');
			selectedProduct.push(splitData[0]);
		});
		var request = new Object();
		request.idpelanggan = $("#idpelanggan").val();
		request.idproduk = selectedProduct;
		alamat = pathutama + "penjualan/getpelanggandiskon";
		$.ajax({
			type: "POST",
			url: alamat,
			data: request,
			cache: false,
			success: function(data){
				var returnData = eval(data);
				var dataDiskon = returnData[0];
				totalbelanja = 0;
				for (var i = 0;i < addedProduct.length;i++){
					var splitNilai = addedProduct[i].split('___');
					var idProduk = splitNilai[0];
					var diskonProduk = dataDiskon[idProduk];
					if (!diskonProduk){
						diskonProduk = 0;
					}
					var barisdiubah = document.getElementById('cekbox_'+ idProduk).parentNode.parentNode;
					var baris_int = oTable.fnGetPosition(barisdiubah);
					var idproduknya = barisdiubah.getAttribute("id");
					var nilaidata = $("#cekbox_"+ idproduknya).val();
					var pecahnilai = nilaidata.split("___");
					oTable.fnUpdate(diskonProduk, baris_int, 4 );
					nilaisubtotal = (pecahnilai[2]-(pecahnilai[2]*diskonProduk/100))*splitNilai[1];
					totalbelanja += parseFloat(nilaisubtotal);
                    var subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
					var namacekbox = "cekbox_"+ idproduknya;
					var nilaikirim = idproduknya +"___"+ splitNilai[1] +"___"+ pecahnilai[2] +"___"+ diskonProduk +"___"+ pecahnilai[4];
					var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" class=\"checkbox-produk\" />";
					oTable.fnUpdate(subtotalbaru +" "+ checkboxnilai, baris_int, 6 );
					if (i == (addedProduct.length - 1)){
						$("#lastdiskon").val(diskonProduk);
					}
				}
                $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
			}
		});
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

function ubah_harga_produk(posisi,nTr,idproduk){
    barisrubah = nTr;
    $("#dialogubahharga2").dialog("open");
}

function calculate_total_bayar(){
	var TotalBayar = 0;
	if ($('#nilaibayar').is(':visible')){
		if ($('#nilaibayar').val() != ''){
            TotalBayar = TotalBayar + parseFloat($('#nilaibayar').val());
		}
	}
    if ($('#nilai_bayar_kredit').is(':visible')){
		if ($('#nilai_bayar_kredit').val() != ''){
            TotalBayar = TotalBayar + parseFloat($('#nilai_bayar_kredit').val());
		}
    }
    if ($('#nilai_bayar_hutang').is(':visible')){
    	if ($('#nilai_bayar_hutang').val() != ''){
            TotalBayar = TotalBayar + parseFloat($('#nilai_bayar_hutang').val());
		}
    }
    if ($('#nilai_bayar_debit').is(':visible')){
        if ($('#nilai_bayar_debit').val() != '') {
            TotalBayar = TotalBayar + parseFloat($('#nilai_bayar_debit').val());
        }
    }
	return TotalBayar;
}

function next_tab(InputObj, KeyObj){
    if (KeyObj.keyCode == 13){
        var last_tab_index = parseInt(InputObj.attr("tabindex"));
        var ntabindex = parseInt(InputObj.attr("tabindex")) + 1;
        while ($("[tabindex=" + ntabindex + "]").is(":hidden") && ntabindex <= 6){
            ntabindex++;
            if ($("[tabindex=" + ntabindex + "]").is(":visible")){
                last_tab_index = ntabindex;
            }
        }
        if ($("[tabindex=" + ntabindex + "]").is(":visible")){
            $("[tabindex=" + ntabindex + "]").focus();
        }else{
            var TotalBayar = calculate_total_bayar();
            var TotalPlusPpn = (totalbelanja * (parseInt($('#ppn_value').val())/100)) + totalbelanja;

            var CaraBayar = $("#carabayar").val();
            var TotalCounter = CaraBayar.length;
            var Hutang = false;
            for (var i = 0;i < TotalCounter;i++){
                if (CaraBayar[i] == 'HUTANG'){
                    Hutang = true;
                }
            }

            if (TotalBayar >= TotalPlusPpn) {
                $("#status-message").removeClass('message-error').html('');
                $('#simpan-transaksi').button('enable').focus();
            }else{
            	if (Hutang){
                    $("#status-message").removeClass('message-error').html('');
                    $('#simpan-transaksi').button('enable').focus();
				}else {
                    $("#status-message").addClass('message-error').html("<blink>Pembayaran kurang dari total, mohon key in pembayaran dengan betul...!!?</blink>");
                    $("[tabindex=" + last_tab_index + "]").focus();
                }
            }
        }
        return false;
    }else{
        var TotalBayar = calculate_total_bayar();
        var TotalPlusPpn = (totalbelanja * (parseInt($('#ppn_value').val())/100)) + totalbelanja;
        var Kembali = TotalBayar - TotalPlusPpn;
        $('#kembali').val(number_format(Kembali,dDigit,dSep,tSep));
    }
}

$(document).ready(function(){
	pathutama = Drupal.settings.basePath;
	tglsekarang = Drupal.settings.tglsekarang;
	tgltampil = Drupal.settings.tgltampil;
	alamatasal = Drupal.settings.alamatasal;

	currSym = Drupal.settings.currSym;
	tSep = Drupal.settings.tSep[0];
	dSep = Drupal.settings.dSep[0];
    dDigit = Drupal.settings.dec_digit;
	$('#idpelanggan').chosen().change(function(){
		$('#barcode').focus();
	});
	if (typeof Drupal.settings.idtitipanlaundry != 'undefined') {
		$("#dialogkasir").dialog({
			modal: true,
			width: 925,
			closeOnEscape: false,
			height: 650,
			resizable: false,
			autoOpen: false,
			open: function (event, ui) {
				$("#tempatjam").clock({"format": "24", "calendar": "false"});
				$("#barcode").focus();
			},
			position: ["auto", "auto"],
			close: function( event, ui ) {
				if (typeof Drupal.settings.idtitipanlaundry != 'undefined' && Drupal.settings.idtitipanlaundry > 0){
					window.location = pathutama + 'penjualan/' + alamatasal;
				}
			}
		});
	}else{
		$("#dialogkasir").dialog({
			modal: true,
			width: 925,
			closeOnEscape: false,
			height: 630,
			resizable: false,
			autoOpen: false,
			open: function (event, ui) {
				$("#tempatjam").clock({"format": "24", "calendar": "false"});
				$("#barcode").focus();
			},
			position: ["auto", "auto"],
			close: function( event, ui ) {
				if (typeof Drupal.settings.idtitipanlaundry != 'undefined' && Drupal.settings.idtitipanlaundry > 0){
					window.location = pathutama + 'penjualan/' + alamatasal;
				}
			}
		});
	}
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
    $("#dialogubahharga2").dialog({
        modal: true,
        width: 250,
        height: 100,
        closeOnEscape: false,
        resizable: false,
        autoOpen: false,
        open: function(event, ui) {
            idproduknya = barisrubah.getAttribute("id").trim();
            var nilaidata = $("#cekbox_"+ idproduknya).val();
            var pecahnilai = nilaidata.split("___");
            $("#newharga2").val(pecahnilai[2]);
            $("#newharga2").select();
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
		width: 600,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
            $('#dialogbayar input[type=text]').autotab();
            $("#totalbelanjauser").val(currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
			var total_ppn_value = totalbelanja * (parseInt($('#ppn_value').val())/100);
            $("#total_ppn").val(currSym +" "+ number_format(total_ppn_value,dDigit,dSep,tSep));
			var total_plus_ppn = (totalbelanja * (parseInt($('#ppn_value').val())/100)) + totalbelanja;
            $("#total_plus_ppn").val(currSym +" "+ number_format(total_plus_ppn,dDigit,dSep,tSep));
            $("#nilaibayar").val(number_format(total_plus_ppn,dDigit,dSep,tSep));
			$("#kembali").val(currSym +" 0");
			$("#nilaibayar").select();
            //$('#nomormeja').select();
			$("#idpelanggan").removeAttr("disabled");
            if ($("#idpelanggan").val() > 0) {
                $("#baris-deposit").show();
                alamat = pathutama + "datapelanggan/gettotalhutang/" + $("#idpelanggan").val();
                $.ajax({
                    type: "POST",
                    url: alamat,
                    cache: false,
                    success: function (data) {
                        var returnData = eval(data);
                        var totalHutang = returnData[0];
                        if (totalHutang < 0) {
                            $('#label-deposit').html('Deposit');
                            $("#carabayar option").each(function () {
                                if ($(this).attr('value') == 'DEPOSIT') {
                                    $(this).removeAttr('disabled');
                                }
                            });
                        } else {
                            $('#label-deposit').html('Hutang');
                            $("#carabayar option").each(function () {
                                if ($(this).attr('value') == 'DEPOSIT') {
                                    $(this).attr('disabled', 'disabled');
                                }
                            });
                        }
                        $('#depositpelanggan').val(currSym + " " + number_format(Math.abs(totalHutang),dDigit,dSep,tSep));
                        $("#idpelanggan").attr("disabled", "disabled");
                    }
                });
            }else{
                $("#carabayar option").each(function(){
                    if ($(this).attr('value') == 'DEPOSIT' || $(this).attr('value') == 'HUTANG'){
                        $(this).attr('disabled', 'disabled');
                    }
                });
                $("#baris-deposit").hide();
            }
            $("#carabayar").chosen();
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
		if (e.keyCode == 114){
			$("#tombolubahqty").click();
		}else if (e.keyCode == 13){
			if ($("#barcode").val() != ""){
				tambahproduk(1,0);
			}else{
				$("#pesantext").text("Mohon isi barcode atau kode produk atau nama produk yang ingin dicari...!!!");
				$("#dialogwarning").dialog("open");
			}
		}else if (e.keyCode == 116) {
			kirim_data(1);
		}else if (e.keyCode == 117){
			kirim_data(0);
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
		}else if (e.keyCode == 120){
			$('#idpelanggan').trigger('chosen:activate');
		}
	});
	$("#barcode").autocomplete({
		source: pathutama + "penjualan/autocaribarang",
		select: function (event, ui) {
			if (ui.item.barcode){
				$("#barcode").val(ui.item.barcode);
			}else if(!ui.item.barcode && ui.item.alt_code){
				$("#barcode").val(ui.item.alt_code);
			}else if(!ui.item.barcode && !ui.item.alt_code){
				$("#barcode").val(ui.item.value);
			}
			$('#hiddenbarcode').val(ui.item.barcode);
			tambahproduk(1, ui.item.barcode_batch);
		}
	});
	$("#newharga").keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = totalproduk-1;
			totalbelanja = totalbelanja - ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
			var nilaiubah = $("#newharga").val();
			oTable.fnUpdate(number_format(nilaiubah,dDigit,dSep,tSep), baris_int, 3 );
			nilaisubtotal = (nilaiubah-(nilaiubah*$("#lastdiskon").val()/100))*$("#lastqty").val();
            subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
			var namacekbox = "cekbox_"+ $("#last_id").val().trim();
            var splitNilaiCekBox = $('#'+ namacekbox).val().split('___');
            var nilaikirim = $("#last_id").val() +"___"+ $("#lastqty").val() +"___"+ nilaiubah +"___"+ $("#lastdiskon").val() +"___"+ splitNilaiCekBox[4];
			var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" class=\"checkbox-produk\" />";
			oTable.fnUpdate(subtotalbaru +" "+ checkboxnilai, baris_int, 6 );
			$("#lastharga").val(nilaiubah);
			totalbelanja = totalbelanja + nilaisubtotal;
            $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
			$("#dialogubahharga").dialog("close");
		}
	});
    $("#newharga2").keypress(function(e) {
        if (e.keyCode == 13){
            var baris_int = oTable.fnGetPosition(barisrubah);
            var idproduknya = barisrubah.getAttribute("id").trim();
            var PpnValue = 0;
            if ($('#ppn-'+ idproduknya).is(':checked')){
                PpnValue = 10;
            }
            var nilaidata = $("#cekbox_"+ idproduknya).val();
            var pecahnilai = nilaidata.split("___");
            var SubTotalLama = (pecahnilai[1]*(pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100)));
            SubTotalLama = SubTotalLama + (SubTotalLama * PpnValue/100);
            totalbelanja = totalbelanja - SubTotalLama;
            var nilaiubah = $("#newharga2").val();
            oTable.fnUpdate(number_format(nilaiubah,dDigit,dSep,tSep), baris_int, 3 );
            nilaisubtotal = (nilaiubah-(nilaiubah*pecahnilai[3]/100))*pecahnilai[1];
            nilaisubtotal = nilaisubtotal + (nilaisubtotal * PpnValue/100);
            subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
            var namacekbox = "cekbox_"+ idproduknya;
            var nilaikirim = idproduknya +"___"+ pecahnilai[1] +"___"+ nilaiubah +"___"+ pecahnilai[3];
            nilaikirim += "___"+ pecahnilai[4];
            var checkboxnilai = "<input class=\"nilai_kirim\" checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
            oTable.fnUpdate('<span id="subtotal-'+ idproduknya +'">'+subtotalbaru +'</span> '+ checkboxnilai, baris_int, 6 );
            totalbelanja = totalbelanja + nilaisubtotal;
            posisiakhir = totalproduk-1;
            if (baris_int == posisiakhir){
                $("#lastharga").val(nilaiubah);
            }
            $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
            $("#dialogubahharga2").dialog("close");
        }
    });
	$("#newqty").keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = totalproduk-1;
			totalbelanja = totalbelanja - ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
			var nilaiubah = $("#newqty").val();
			oTable.fnUpdate(nilaiubah, baris_int, 5 );
			nilaisubtotal = ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*nilaiubah;
            subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
			var namacekbox = "cekbox_"+ $("#last_id").val().trim();
            var splitNilaiCekBox = $('#'+ namacekbox).val().split('___');
			var nilaikirim = $("#last_id").val().trim() +"___"+ nilaiubah +"___"+ $("#lastharga").val() +"___"+ $("#lastdiskon").val() +"___"+ splitNilaiCekBox[4];
			var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" class=\"checkbox-produk\" />";
			oTable.fnUpdate(subtotalbaru +" "+ checkboxnilai, baris_int, 6 );
			$("#lastqty").val(nilaiubah);
			totalbelanja = totalbelanja + nilaisubtotal;
            $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
			$("#dialogubahqty").dialog("close");
		}
	});
	$("#newqty2").keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = oTable.fnGetPosition(barisrubah);
			var idproduknya = barisrubah.getAttribute("id").trim();
			var nilaidata = $("#cekbox_"+ idproduknya).val();
			var pecahnilai = nilaidata.split("___");
			totalbelanja = totalbelanja - (pecahnilai[1]*(pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100)));
			var nilaiubah = $("#newqty2").val();
			oTable.fnUpdate(nilaiubah, baris_int, 5 );
			nilaisubtotal = (pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100))*nilaiubah;
            subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
			var namacekbox = "cekbox_"+ idproduknya;
			var nilaikirim = idproduknya +"___"+ nilaiubah +"___"+ pecahnilai[2] +"___"+ pecahnilai[3] +"___"+ pecahnilai[4];
			var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" class=\"checkbox-produk\" />";
			oTable.fnUpdate(subtotalbaru +" "+ checkboxnilai, baris_int, 6 );
			totalbelanja = totalbelanja + nilaisubtotal;
			posisiakhir = totalproduk-1;
			if (baris_int == posisiakhir){
				$("#lastqty").val(nilaiubah);
			}
            $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
			$("#dialogubahqty2").dialog("close");
		}
	});
	/*$("#nilaibayar,#nomerkartu").keyup(function(e){
        var total_plus_ppn = (totalbelanja * (parseInt($('#ppn_value').val())/100)) + totalbelanja;
		kembali = $("#nilaibayar").val() - total_plus_ppn;
        $("#kembali").val(currSym +" "+ number_format(kembali,dDigit,dSep,tSep));
		if (kembali > 0){
			$("#field_kembalian").show();
		}else{
			$("#field_kembalian").hide();
		}
		if (e.keyCode == 13){
			if ($('#idmeja').val() == ''){
				alert('Mohon pilih meja terlebih dahulu...!!?');
                $('#nomormeja').select();
			}else{
                akhiri_belanja(cetakstruk);
			}
		}
	});*/
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
	if (typeof Drupal.settings.data_laundry != 'undefined'){
		if (Drupal.settings.data_laundry.length > 0){
			var totaldetaildata = Drupal.settings.data_laundry.length;
			for (var i = 0;i < totaldetaildata;i++){
				var dataDetail = Drupal.settings.data_laundry[i];
				$('#barcode').val(dataDetail.namaproduct);
				tambahproduk(dataDetail.sisa,0);
			}	
		}
	}else if(typeof Drupal.settings.data_order != 'undefined'){
		if (Drupal.settings.data_order.length > 0){
			var totaldetaildata = Drupal.settings.data_order.length;
			for (var i = 0;i < totaldetaildata;i++){
				var dataDetail = Drupal.settings.data_order[i];
				if (dataDetail.barcode != ''){
					$('#barcode').val(dataDetail.barcode.trim());
				}else{
					$('#barcode').val(dataDetail.namaproduct.trim());
				}
				tambahproduk(dataDetail.sisa,0);
			}
		}
	}
	$("#carabayar").change(function(){
		var SplitCaraBayar = $(this).val();
		if (SplitCaraBayar.indexOf('DEBIT') != -1){
			$("#field_no_kartu_debit,#baris_bayar_debit").show();
            $("#nomerkartu").focus();
		}else{
            $("#field_no_kartu_debit,#baris_bayar_debit").hide();
		}
        if (SplitCaraBayar.indexOf('KAD KREDIT') != -1){
            $("#field_no_kartu_kredit,#baris_bayar_kredit").show();
            $("#nomer_kartu_kredit").focus();
        }else{
            $("#field_no_kartu_kredit,#baris_bayar_kredit").hide();
        }
        if (SplitCaraBayar.indexOf('TUNAI') != -1){
            $("#baris_bayar_tunai").show();
            $("#nilaibayar").removeAttr('readonly').removeAttr('disabled');
            $("#nilaibayar").focus();
        }else{
            $("#baris_bayar_tunai").hide();
        }
        if (SplitCaraBayar.indexOf('HUTANG') != -1){
            $("#baris_bayar_hutang").show();
            $("#nilai_bayar_hutang").focus();
        }else{
            $("#baris_bayar_hutang").hide();
        }
    });
		/*if ($(this).val() == 'DEBIT' || $(this).val() == 'KAD KREDIT' || $(this).val() == 'GIRO'){
			$("#field_no_kartu").show();
			$("#field_bayar").show();
            var total_plus_ppn = (totalbelanja * (parseInt($('#ppn_value').val())/100)) + totalbelanja;
			$("#nilaibayar").val(total_plus_ppn).attr('readonly','readonly').removeAttr('disabled');
			$("#nomerkartu").select();
			$("#nilaibayar").keyup();
		}else if($(this).val() == 'DEPOSIT'){
			$("#field_bayar").show();
			$("#field_no_kartu").hide();
            var total_plus_ppn = (totalbelanja * (parseInt($('#ppn_value').val())/100)) + totalbelanja;
			$("#nilaibayar").val(total_plus_ppn).attr('readonly','readonly').removeAttr('disabled');
			$("#nilaibayar").keyup();
			$("#nilaibayar").focus();
		}else{
			$("#field_no_kartu").hide();
			$("#field_bayar").show();
			$("#nilaibayar").removeAttr('readonly').removeAttr('disabled');
			$("#nilaibayar").select();
		}*/

	if (typeof Drupal.settings.idtitipanlaundry != 'undefined'){
		//$("#idpelanggan").attr("disabled","disabled");
	}
	$("#kembalian").change(function(){
		$("#nilaibayar").select();
	});
	$('#info-kasir-waktu').css('background','url('+ Drupal.settings.logo +') 99% 50% no-repeat');
	$('#info-kasir-waktu').css('background-size','75px 75px');
	$('#tempattombolkasir').css('height','330px');
	if (Drupal.settings.upload_data){
		alamat = pathutama + 'datapremis/uploaddata';
		$.ajax({
			type: 'POST',
			url: alamat,
			cache: false,
			success: function (data) {

			}
		});
	}
	$('#use-ppn').click(function(){
		if ($('#ppn_value').val() > 0){
			ppnValue = $('#ppn_value').val();
		}
		var ppnChecked = $('#use-ppn:checkbox:checked').length;
		if (!ppnChecked){
			$('#ppn_value').val(0);
		}else{
			$('#ppn_value').val(ppnValue);
		}
		$("#dialogbayar").dialog('close');
		$("#dialogbayar").dialog('open');
	});
    $('#selesai-transaksi').css('font-size','1.2em').css('font-weight','bold').css('margin-top','10px');
    $('#simpan-transaksi').css('font-size','1.2em').css('font-weight','bold').css('margin-top','10px');
    $('#simpan-transaksi').button("disable").on('click', function() {
        $('#dialogbayar').parent().block();
        $('#simpan-transaksi').off('click');
        akhiri_belanja(cetakstruk);
    });
	$('#selesai-transaksi').button("disable").on('click', function(){
    	if (cetakstruk == 1) {
            window.open(pathutama + "print/6?idpenjualangh=" + IdPenjualan);
        }
        if (typeof Drupal.settings.idtitipanlaundry != 'undefined' && Drupal.settings.idtitipanlaundry > 0){
            window.location = pathutama + 'penjualan/' + alamatasal;
        }else{
            window.location = pathutama + "penjualan/kasir?afterinsert=1";
        }
	});
    $('#nomerkartu').on('keyup', function(e){
        next_tab($(this), e);
	});
    $('#nomer_kartu_kredit').on('keyup', function(e){
        next_tab($(this), e);
    });
    $('#nilaibayar').on('keyup', function(e){
        next_tab($(this), e);
    });
    $('#nilai_bayar_kredit').on('keyup', function(e){
        next_tab($(this), e);
    });
    $('#nilai_bayar_hutang').on('keyup', function(e){
        next_tab($(this), e);
    });
    $('#nilai_bayar_debit').on('keyup', function(e){
        next_tab($(this), e);
    });
    /*$('#nomormeja').autocomplete({
        source: pathutama + "penjualan/autocarimeja",
        select: function (event, ui) {
            $("#nomormeja").val(ui.item.value);
            $("#idmeja").val(ui.item.id);
            $("#nilaibayar").select();
        }
    });*/
})