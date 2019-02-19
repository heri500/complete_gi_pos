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
var currSym = '';
var tSep = '.';
var dSep = ',';
var dDigit = 0;

function tampilkantabelkasir(){
	oTable = $("#tabel_kasir").dataTable( {
		"bJQueryUI": true,
		"bPaginate": false,
		"bLengthChange": false,
		"bFilter": true,
		"bInfo": false,
		"sScrollY": "270px",
		"aoColumns": [
		{ "bSortable": false },{ "bSortable": false },null,null,null,{ "bSortable": false },null,null,{ "bSortable": false }
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
function ubah_harga_produk(posisi,nTr,idproduk){
    barisrubah = nTr;
    $("#dialogubahharga2").dialog("open");
}
function cek_ppn(PpnObj, IdProduk){
    IdProduk = PpnObj.val();
    if (PpnObj.is(':checked')){
        var RowVal = $('#cekbox_'+ IdProduk).val();
        var SplitVal = RowVal.split('___');
        var HargaJual = 0;
        var QtyItem = 1;
        var DiskonItem = 0;
        if (SplitVal.length > 0){
            var subtotallama = SplitVal[1] * (SplitVal[2] - (SplitVal[2]*SplitVal[3]/100));
            var subtotal = subtotallama + (subtotallama * 10/100);
            $('#subtotal-'+ IdProduk).html(number_format(subtotal,dDigit,dSep,tSep));
            totalbelanja = totalbelanja - subtotallama + subtotal;
            $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
            var NewValue = '';
            for (var i = 0;i < SplitVal.length;i++){
                if (i == 0){
                    NewValue = SplitVal[i];
                }else{
                    if (i < 5){
                        NewValue += '___'+ SplitVal[i];
                    }else{
                        NewValue += '___10';
                    }
                }
            }
            $('#cekbox_'+ IdProduk).val(NewValue);
        }
    }else{
        var RowVal = $('#cekbox_'+ IdProduk).val();
        var SplitVal = RowVal.split('___');
        var HargaJual = 0;
        var QtyItem = 1;
        var DiskonItem = 0;
        if (SplitVal.length > 0){
            var subtotal = SplitVal[1] * (SplitVal[2] - (SplitVal[2]*SplitVal[3]/100));
            var subtotalppn = subtotal + (subtotal * 10/100);
            $('#subtotal-'+ IdProduk).html(number_format(subtotal,dDigit,dSep,tSep));
            totalbelanja = totalbelanja - subtotalppn + subtotal;
            $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
            var NewValue = '';
            for (var i = 0;i < SplitVal.length;i++){
                if (i == 0){
                    NewValue = SplitVal[i];
                }else{
                    if (i < 5){
                        NewValue += '___'+ SplitVal[i];
                    }else{
                        NewValue += '___0';
                    }
                }
            }
            $('#cekbox_'+ IdProduk).val(NewValue);
        }
    }
}
function tambahproduk(qtyAdd, batch_code){
	var request = new Object();
	var katacari = $("#barcode").val();
	var pecahkatacari = katacari.split("--->");
	request.barcode = pecahkatacari[0];
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
				nilaikirim = pecahdata[0].trim() +"___"+ qtyAdd +"___"+ pecahdata[2] +"___"+ pecahdata[3] ;
                nilaikirim += "___"+ batch_code +"___0";
				index_cek_box = pecahdata[0].trim();
				namacekbox = "cekbox_"+ index_cek_box;
				if($("#"+ namacekbox).val()){
					var nilaicekbox = $("#"+ namacekbox).val();
					var pecahnilai = nilaicekbox.split("___");
					var qtybaru = parseFloat(pecahnilai[1]) + 1;
					var subtotallama = pecahnilai[1] * (pecahnilai[2] - (pecahnilai[2]*pecahnilai[3]/100));
					var subtotal = qtybaru * (pecahnilai[2] - (pecahnilai[2]*pecahnilai[3]/100));
					totalbelanja = totalbelanja - subtotallama + subtotal;
					$("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
					var nTr = $("#"+ namacekbox).parent().parent().get(0);
					var posisibaris = oTable.fnGetPosition(nTr);
					oTable.fnUpdate(qtybaru, posisibaris, 6 );
					nilaikirim = pecahnilai[0].trim() +"___"+ qtybaru +"___"+ pecahnilai[2] +"___"+ pecahnilai[3];
                    nilaikirim += "___"+ pecahnilai[4] +"___"+ pecahnilai[5];
					checkboxnilai = "<input class=\"nilai_kirim\" checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
					oTable.fnUpdate('<span id="subtotal-'+ index_cek_box +'">'+ number_format(subtotal,dDigit,dSep,tSep) +"</span> "+ checkboxnilai, posisibaris, 7 );
                    //$('#subtotal-'+ index_cek_box).html(number_format(subtotal,dDigit,dSep,tSep));
					posisiakhir = totalproduk-1;
					if (posisibaris == posisiakhir){
						$("#lastqty").val(qtybaru);
					}
				}else {
                    var icondelete = "<img onclick=\"hapus_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk menghapus\" src=\"" + pathutama + "misc/media/images/close.ico\" width=\"24\">";
                    var iconubah = "<img onclick=\"ubah_qty_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah qty produk ini\" src=\"" + pathutama + "misc/media/images/edit.ico\" width=\"24\">";
                    var iconharga = "<img onclick=\"ubah_harga_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah qty produk ini\" src=\"" + pathutama + "misc/media/images/money2.png\" width=\"24\">";
                    var checkboxppn = "<input onclick=\"cek_ppn($(this), "+ pecahdata[0].trim() +")\" id=\"ppn-" + pecahdata[0].trim() + "\" name=\"ppn-" + pecahdata[0].trim() + "\" type=\"checkbox\" value=\"" + pecahdata[0].trim() + "\" />";

                    $("#lastharga").val(pecahdata[2]);
                    $("#lastdiskon").val(pecahdata[3]);
                    $("#last_id").val(pecahdata[0].trim());
                    $("#lastqty").val(qtyAdd);
                    var checkboxnilai = "<input class=\"nilai_kirim\" checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" />";
                    var row = "<tr id=\"" + index_cek_box + "\"><td>" + icondelete + "</td><td>" + iconubah + "</td>";
                    row += "<td>" + pecahdata[1] + "</td><td class=\"angka\">" + pecahdata[2] + "</td>";
                    row += "<td id=\"diskon-" + pecahdata[0].trim() + "\" class=\"angka diskon-produk\">" + pecahdata[3] + "</td>";
                    row += "<td class=\"angka\">" + checkboxppn + "</td>";
                    row += "<td class=\"angka\">" + qtyAdd + "</td>";
                    row += "<td class=\"angka\"><span id=\"subtotal-"+ pecahdata[0].trim() +"\">" + number_format(subtotal,dDigit,dSep,tSep) + "</span> " + checkboxnilai + "</td>";
                    row += "<td>" + iconharga + "</td></tr>";
                    $("#tabel_kasir").dataTable().fnAddTr($(row)[0]);
                    giCount++;
                    totalproduk++;
                    totalbelanja = totalbelanja + nilaisubtotal;
                    $("#totalbelanja").html("Total : "+ currSym +" " + number_format(totalbelanja,dDigit,dSep,tSep));
                    //$("#diskon-" + pecahdata[0].trim()).editable('destroy');
                    $('#diskon-' + pecahdata[0].trim()).editable(function (value, settings) {
                        var RowVal = $('#cekbox_'+ pecahdata[0].trim()).val();
                        var SplitVal = RowVal.split('___');
                        var HargaJual = 0;
                        var QtyItem = 1;
                        if (SplitVal.length > 0){
                            HargaJual = SplitVal[2];
                            QtyItem = SplitVal[1];
                        	var NewVal = '';
                        	for (var i = 0;i < SplitVal.length;i++){
                        		if (i == 0) {
                                    NewVal = SplitVal[i];
                                }else if(i == 3){
                                    NewVal += '___' + value;
                                }else{
                                    NewVal += '___' + SplitVal[i];
                                }
							}
						}
                        $('#cekbox_'+ pecahdata[0].trim()).val(NewVal);
                        var PpnValue = 0;
                        if ($('#ppn-'+ pecahdata[0].trim()).is(':checked')){
                            PpnValue = 10;
                        }
                        var subtotallama = SplitVal[1] * (SplitVal[2] - (SplitVal[2]*SplitVal[3]/100));
                        subtotallama = subtotallama + (subtotallama * PpnValue /100);
                        var subtotal = SplitVal[1] * (SplitVal[2] - (SplitVal[2]*value/100));
                        subtotal = subtotal + (subtotal * PpnValue / 100);
                        $('#subtotal-'+ pecahdata[0].trim()).html(number_format(subtotal,dDigit,dSep,tSep));
                        totalbelanja = totalbelanja - subtotallama + subtotal;
                        $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
                        if (totalproduk > 0) {
                            var nTr = oTable.fnGetNodes(totalproduk - 1);
                            IdProduk = nTr.getAttribute("id");
                            if (IdProduk == pecahdata[0].trim()){
                                $("#lastdiskon").val(value);
							}
                        }
                        return (value);
                    }, {
                    	'select': true,
                        'width': '40px',
                        'height':'20px',
                        'submit': 'Ok'
                    });
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
function kirim_data(cetaknota){
	cetakstruk = cetaknota;
	if (totalproduk > 0){
        var sData = $("input.nilai_kirim", oTable.fnGetNodes()).serialize();
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
        var PpnValue = 0;
        if ($('#ppn-'+ idproduk).is(':checked')){
            PpnValue = 10;
        }
        var SubTotalLama = ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
        SubTotalLama = SubTotalLama + (SubTotalLama * PpnValue/100);
        totalbelanja = totalbelanja - SubTotalLama;
        $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
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
		}else{
			$("#lastdiskon").val("");
			$("#lastharga").val("");
			$("#lastqty").val("");
			$("#last_id").val("");
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
    var PpnValue = 0;
    if ($('#ppn-'+ idproduk).is(':checked')){
        PpnValue = 10;
    }
	var SubTotalLama = (pecahnilai[1]*(pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100)));
    SubTotalLama = SubTotalLama + (SubTotalLama * PpnValue/100);
    totalbelanja = totalbelanja - SubTotalLama;
	$("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
	oTable.fnDeleteRow(posisibaris,focusbarcode);
	totalproduk--;
	if (totalproduk > 0){
		var nilaidataakhir = $("#cekbox_"+ totalproduk).val();
		var pecahnilaiakhir = nilaidataakhir.split("___");
		$("#lastdiskon").val(pecahnilaiakhir[3]);
		$("#lastharga").val(pecahnilaiakhir[2]);
		$("#lastqty").val(pecahnilaiakhir[1]);
		$("#last_id").val(pecahnilaiakhir[0]);
	}else{
		$("#lastdiskon").val("");
		$("#lastharga").val("");
		$("#lastqty").val("");
		$("#last_id").val("");
	}
	$("#barcode").focus();
	$("#barcode").select();
}
function akhiri_belanja(cetak) {
    var request = new Object();
    if (typeof Drupal.settings.idtitipanlaundry != 'undefined') {
        if (alamatasal == 'viewlaundry' || alamatasal == 'laundrykeluar') {
            request.idtitipanlaundry = Drupal.settings.idtitipanlaundry;
        } else if (alamatasal == 'viewcustomerorder' || alamatasal == 'orderkeluar') {
            request.idcustomerorder = Drupal.settings.idtitipanlaundry;
        }

    }
    request.detail_produk = $("#nilaikirim").val();
    $("#idpelanggan").removeAttr("disabled");
    request.idpelanggan = $("#idpelanggan").val();
    request.totalbelanja = totalbelanja;
    request.carareturn = $("#carabayar").val();
    request.bayar = $("#nilaibayar").val();
    request.tglreturn = $("#tgljualkirim").val();
    //if ($("#idpelanggan").val() != 0){
    alamat = pathutama + "penjualan/simpanreturn";
    $.ajax({
        type: "POST",
        url: alamat,
        data: request,
        cache: false,
        success: function (data) {
            var returndata = data.trim();
            if (returndata != "error") {
                if (cetak == 1) {
                    window.open(pathutama + "print/6?idreturnjual=" + returndata);
                }
                if (typeof Drupal.settings.idtitipanlaundry != 'undefined' && Drupal.settings.idtitipanlaundry > 0) {
                    window.location = pathutama + 'penjualan/' + alamatasal;
                } else {
                    window.location = pathutama + "penjualan/return?tanggal=" + request.tgljual + '&afterinsert=1';
                }

            } else {
                alert("Ada masalah dalam penyimpanan data...!!!");
            }
        }
    });
    //}else{
    //	alert("Mohon pilih pelanggan terlebih dulu ...!!!");
    //}
}
function inisialulang(){
	if (totalproduk > 0){
		var tanya = confirm("Merubah pelanggan berarti menghapus pembelian yang terinput, YAKIN..!!");
		if (tanya != 0){
			window.location = pathutama + "penjualan/return?idpelanggan="+ $("#idpelanggan").val();
		}else{
			$("#barcode").select();
		}
	}else{
		$("#barcode").select();
	}
}
function pilih_pelanggan(){
	$('#idpelanggan').trigger('chosen:activate');
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
			width: 930,
			closeOnEscape: false,
			height: 650,
			resizable: false,
			autoOpen: false,
			open: function (event, ui) {
				$('#tempatjam').clock({'format': '24', 'calendar': 'false'});
                $('#info-kasir-waktu input[type="text"]').css('width','292px');
				$('#barcode').focus();
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
			width: 930,
			closeOnEscape: false,
			height: 630,
			resizable: false,
			autoOpen: false,
			open: function (event, ui) {
				$("#tempatjam").clock({"format": "24", "calendar": "false"});
                $('#info-kasir-waktu input[type="text"]').css('width','292px');
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
		width: 550,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$("#totalbelanjauser").val(currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
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
			kembali = $("#nilaibayar").val()-totalbelanja;
			$("#kembali").val(currSym +" "+ number_format(kembali,dDigit,dSep,tSep));
			$("#nilaibayar").keyup();
			$("#totalbelanjauser").select();
			$("#idpelanggan").removeAttr("disabled");
			if ($("#idpelanggan").val() != 0){
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
							$("#carabayar option").each(function(){
								if ($(this).attr('value') == 'DEPOSIT'){
									$(this).removeAttr('disabled');
								}
							});
						}else{
							$('#label-deposit').html('Hutang');
							$("#carabayar option").each(function(){
								if ($(this).attr('value') == 'DEPOSIT'){
									$(this).attr('disabled', 'disabled');
								}
							});
						}
						$('#depositpelanggan').val(currSym +" "+ number_format(Math.abs(totalHutang),dDigit,dSep,tSep));
						$("#idpelanggan").attr("disabled","disabled");
					}
				});
                $('#carabayar').change();
			}else{
				$("#carabayar option").each(function(){
					if ($(this).attr('value') == 'DEPOSIT' || $(this).attr('value') == 'HUTANG'){
						$(this).attr('disabled', 'disabled');
					}
				});
			}
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
			tambahproduk(1, ui.item.barcode_batch);
		}
	});
	$("#newharga").keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = totalproduk-1;
            var PpnValue = 0;
            if ($('#ppn-'+ $("#last_id").val()).is(':checked')){
                PpnValue = 10;
            }
            var SubTotalLama = ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
            SubTotalLama = SubTotalLama + (SubTotalLama * PpnValue /100);
            totalbelanja = totalbelanja - SubTotalLama;
			var nilaiubah = $("#newharga").val();
			oTable.fnUpdate(nilaiubah, baris_int, 3 );
			nilaisubtotal = (nilaiubah-(nilaiubah*$("#lastdiskon").val()/100))*$("#lastqty").val();
            nilaisubtotal = nilaisubtotal + (nilaisubtotal * PpnValue/100);
            subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
			var namacekbox = "cekbox_"+ $("#last_id").val();
			var splitNilaiCekBox = $('#'+ namacekbox).val().split('___');
			var nilaikirim = $("#last_id").val() +"___"+ $("#lastqty").val() +"___"+ nilaiubah +"___"+ $("#lastdiskon").val();
            nilaikirim += "___"+ splitNilaiCekBox[4] +'___'+ splitNilaiCekBox[5];
            var checkboxnilai = "<input class=\"nilai_kirim\" checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
			oTable.fnUpdate('<span id="subtotal-'+ $('#last_id').val().trim() +'">'+ subtotalbaru +'</span> '+ checkboxnilai, baris_int, 7 );
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
            oTable.fnUpdate(nilaiubah, baris_int, 3 );
            nilaisubtotal = (nilaiubah-(nilaiubah*pecahnilai[3]/100))*pecahnilai[1];
            nilaisubtotal = nilaisubtotal + (nilaisubtotal * PpnValue/100);
            subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
            var namacekbox = "cekbox_"+ idproduknya;
            var nilaikirim = idproduknya +"___"+ pecahnilai[1] +"___"+ nilaiubah +"___"+ pecahnilai[3];
            nilaikirim += "___"+ pecahnilai[4] +"___"+ pecahnilai[5] ;
            var checkboxnilai = "<input class=\"nilai_kirim\" checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
            oTable.fnUpdate('<span id="subtotal-'+ idproduknya +'">'+ subtotalbaru +'</span> '+ checkboxnilai, baris_int, 7 );
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
            var PpnValue = 0;
            if ($('#ppn-'+ $("#last_id").val()).is(':checked')){
                PpnValue = 10;
            }
            var SubTotalLama = ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
            SubTotalLama = SubTotalLama + (SubTotalLama * PpnValue/100);
			totalbelanja = totalbelanja - SubTotalLama;
			var nilaiubah = $("#newqty").val();
			oTable.fnUpdate(nilaiubah, baris_int, 6 );
			nilaisubtotal = ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*nilaiubah;
            nilaisubtotal = nilaisubtotal + (nilaisubtotal * PpnValue/100);
			subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
			var namacekbox = "cekbox_"+ $("#last_id").val();
            var splitNilaiCekBox = $('#'+ namacekbox).val().split('___');
			var nilaikirim = $("#last_id").val() +"___"+ nilaiubah +"___"+ $("#lastharga").val();
            nilaikirim += "___"+ $("#lastdiskon").val() +"___"+ splitNilaiCekBox[4] +"___"+ splitNilaiCekBox[5];
			var checkboxnilai = "<input class=\"nilai_kirim\" checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
			oTable.fnUpdate('<span id="subtotal-'+ $('#last_id').val() +'">'+ subtotalbaru +'</span> '+ checkboxnilai, baris_int, 7 );
			$("#lastqty").val(nilaiubah);
			totalbelanja = totalbelanja + nilaisubtotal;
			$("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
			$("#dialogubahqty").dialog("close");
		}
	});
	$("#newqty2").keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = oTable.fnGetPosition(barisrubah);
			var idproduknya = barisrubah.getAttribute("id");
			var nilaidata = $("#cekbox_"+ idproduknya).val();
            var PpnValue = 0;
            if ($('#ppn-'+ idproduknya).is(':checked')){
                PpnValue = 10;
            }
			var pecahnilai = nilaidata.split("___");
            var SubTotalLama = (pecahnilai[1]*(pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100)));
            SubTotalLama = SubTotalLama + (SubTotalLama * PpnValue/100);
			totalbelanja = totalbelanja - SubTotalLama;
			var nilaiubah = $("#newqty2").val();
			oTable.fnUpdate(nilaiubah, baris_int, 6 );
			nilaisubtotal = (pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100))*nilaiubah;
            nilaisubtotal = nilaisubtotal + (nilaisubtotal * PpnValue/100);
			subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
			var namacekbox = "cekbox_"+ idproduknya;
            var splitNilaiCekBox = $('#'+ namacekbox).val().split('___');
			var nilaikirim = idproduknya +"___"+ nilaiubah +"___"+ pecahnilai[2] +"___"+ pecahnilai[3];
            nilaikirim += "___"+ splitNilaiCekBox[4] +"___"+ splitNilaiCekBox[5];
			var checkboxnilai = "<input class=\"nilai_kirim\" checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
			oTable.fnUpdate('<span id="subtotal-'+ idproduknya +'">' + subtotalbaru +'</span> '+ checkboxnilai, baris_int, 7 );
			totalbelanja = totalbelanja + nilaisubtotal;
			posisiakhir = totalproduk-1;
			if (baris_int == posisiakhir){
				$("#lastqty").val(nilaiubah);
			}
			$("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
			$("#dialogubahqty2").dialog("close");
		}
	});
	//$("#nilaibayar").autotab({ format : 'alphanumeric' });
	$("#nilaibayar").keyup(function(e){
		kembali = $("#nilaibayar").val() - totalbelanja;
		$("#kembali").val(currSym +" "+ number_format(kembali,dDigit,dSep,tSep));
		if (kembali > 0){
			$("#field_kembalian").show();
		}else{
			$("#field_kembalian").hide();
		}
		if (e.keyCode == 13){
            var konfirmasi = confirm('Simpan return barang total : '+ currSym +' '+ number_format(totalbelanja,dDigit,dSep,tSep) +' dengan cara : '+ $('#nilaibayar').text() +'..??!');
            if (konfirmasi) {
                akhiri_belanja(cetakstruk);
            }
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
	$("#tgljual,#kasir").css("width","177px");
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
					$('#barcode').val(dataDetail.barcode);
				}else{
					$('#barcode').val(dataDetail.namaproduct);
				}
				tambahproduk(dataDetail.sisa,0);
			}
		}
	}
    $('#carabayar').change(function(){
        if ($('#carabayar').val() == 3){
            $('#nilaibayar').val(totalbelanja);
            kembali = totalbelanja;
            $('#cashback').show();
            //$('#kembali').val('Rp. '+ number_format(kembali,0,',','.'));
            //$('#nilaibayar').removeAttr('readonly','readonly');
            $('#nilaibayar').select();
        }else{
            if ($('#carabayar').val() == 2){
                $('#nilaibayar').val(totalbelanja);
                $('#kembali').val(0);
                $('#cashback').show();
                $('#nilaibayar').attr('readonly','readonly');
                $('#nilaibayar').select();
            }else{
                $('#cashback').hide();
                $('#nilaibayar').val(0);
            }
            $('#bariskembali').hide();
        }
    });
    $('#carabayar,#totalbelanjauser').keyup(function(e) {
        if (e.keyCode == 13) {
            var konfirmasi = confirm('Simpan pemulangan produk total : '+ currSym +' '+ number_format(totalbelanja,dDigit,dSep,tSep) +' dengan cara : '+ $('#nilaibayar').text() +'..??!');
            if (konfirmasi) {
                akhiri_belanja();
            }
        }
    });
	$('#info-kasir-waktu').css('background','url('+ Drupal.settings.logo +') 99% 50% no-repeat');
	$('#info-kasir-waktu').css('background-size','75px 75px');
	$('#tempattombolkasir').css('height','330px');
})