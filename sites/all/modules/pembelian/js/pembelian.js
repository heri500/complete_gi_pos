var oTable;
var pathutama = '';
var giCount = 1;
var totalbelanja = 0;
var totalproduk = 0;
var barisrubah;
var tglsekarang = '';
var tgltampil = '';
var cetakstruk = 0;
var ppnValue = 0;
var currSym = '';
var tSep = '.';
var dSep = ',';
var dDigit = 0;


function tampilkantabelkasir(){
	oTable = $('#tabel_kasir').dataTable( {
		'bJQueryUI': true,
		'bPaginate': false,
		'bLengthChange': false,
		'bFilter': true,
		'bInfo': false,
		'sScrollY': '320px',
		'aoColumns': [
		    { 'bSortable': false }, { 'bSortable': false }, null, null,
            null, null, null, null, { 'bSortable': false }
		],
		'bAutoWidth': false
	});
}
function munculkankasir(){
	$('#dialogkasir').dialog('open');
}
function ubahharga(){
	if (totalproduk > 0){
		$('#dialogubahharga').dialog('open');
	}else{
		$('#pesantext').text('Mohon pilih produk terlebih dahulu...!!!');
		$('#dialogwarning').dialog('open');
	}
}
function ubahqty(){
	if (totalproduk > 0){
		$('#dialogubahqty').dialog('open');
	}else{
		$('#pesantext').text('Mohon pilih produk terlebih dahulu...!!!');
		$('#dialogwarning').dialog('open');
	}
}
function ubah_qty_produk(posisi,nTr,idproduk){
	barisrubah = nTr;
	$('#dialogubahqty2').dialog('open');
}
function tambahproduk(){
	var request = new Object();
	request.katacari = $('#barcode').val();
	alamatcariproduk = pathutama +'pembelian/cariproduk?idsupplier='+ $('#idsupplier').val();
	$.ajax({
		type: 'POST',
		url: alamatcariproduk,
		data: request,
		cache: false,
		success: function(data){
			pecahdata = new Array();
			pecahdata = data.split(';');
			if (pecahdata[0].trim() != 'error'){
				subtotal = number_format(pecahdata[2],dDigit,dSep,tSep);
				nilaikirim = pecahdata[0].trim() +'___1___'+ pecahdata[2] + '___' + Drupal.settings.tglsekarang;
				index_cek_box = pecahdata[0].trim();
				namacekbox = 'cekbox_'+ index_cek_box;
				if($('#'+ namacekbox).val()){
					var nilaicekbox = $('#'+ namacekbox).val();
					var pecahnilai = nilaicekbox.split('___');
					var qtybaru = parseFloat(pecahnilai[1]) + 1;
					var subtotallama = parseFloat(pecahnilai[1]) * parseFloat(pecahnilai[2]);
					var subtotal = qtybaru * parseFloat(pecahnilai[2]);
					totalbelanja = totalbelanja - subtotallama + subtotal;
					$('#totalbelanja').html('Total : '+ currSym +' '+ number_format(totalbelanja,dDigit,dSep,tSep));
					var nTr = $('#'+ namacekbox).parent().parent().get(0);
					var posisibaris = oTable.fnGetPosition(nTr);
					oTable.fnUpdate(qtybaru, posisibaris, 4 );
					nilaikirim = pecahnilai[0].trim() +'___'+ qtybaru +'___'+ pecahnilai[2] + '___'+ pecahnilai[3];
					checkboxnilai = '<input checked="checked" style="display: none;" id="'+ namacekbox +'" name="'+ namacekbox +'" type="checkbox" value="'+ nilaikirim +'" />';
					oTable.fnUpdate(number_format(subtotal,dDigit,dSep,tSep) +' '+ checkboxnilai, posisibaris, 5 );
					posisiakhir = totalproduk-1;
					if (posisibaris == posisiakhir){
						$('#lastqty').val(qtybaru);
					}
				}else{
					var icondelete = '<img onclick="hapus_produk(\''+ index_cek_box +'\',this.parentNode.parentNode,\''+ pecahdata[0].trim() +'\')" title="Klik untuk menghapus" src="'+ pathutama +'misc/media/images/close.ico" width="24">';
					var iconubah = '<img onclick="ubah_qty_produk(\''+ index_cek_box +'\',this.parentNode.parentNode,\''+ pecahdata[0].trim() +'\')" title="Klik untuk mengubah qty produk ini" src="'+ pathutama +'misc/media/images/edit.ico" width="24">';
                    var iconharga = "<img onclick=\"ubah_harga_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah harga produk ini\" src=\"" + pathutama + "misc/media/images/money2.png\" width=\"24\">";

					$('#lastharga').val(pecahdata[2]);
					$('#last_id').val(pecahdata[0].trim());
					$('#lastqty').val('1');
					checkboxnilai = '<input checked="checked" style="display: none;" id="'+ namacekbox +'" name="'+ namacekbox +'" type="checkbox" value="'+ nilaikirim +'" />';
					var row = '<tr id="'+ index_cek_box +'">';
                    row += '<td>'+ icondelete +'</td>';
                    row += '<td>'+ iconubah +'</td>';
                    row += '<td>'+ pecahdata[1] +'</td>';
                    row += '<td class="angka">'+ subtotal +'</td>';
                    row += '<td class="angka">1</td>';
                    row += '<td class="angka">'+ subtotal +' '+ checkboxnilai +'</td>';
                    row += '<td class="angka">'+ pecahdata[3] +'</td>';
                    row += '<td class="center date-col">';
                    row += '<input type="text" id="expire_'+ pecahdata[0].trim() +'" name="expire_'+ pecahdata[0].trim() +'" value="'+ Drupal.settings.tglsekarang +'"></td>';
                    row += '<td>' + iconharga + '</td></tr>';
                    row += '</tr>';
					$('#tabel_kasir').dataTable().fnAddTr($(row)[0]);
					giCount++;
					totalproduk++;
					totalbelanja = totalbelanja + parseFloat(pecahdata[2]);
					$('#totalbelanja').html('Total : '+ currSym +' '+ number_format(totalbelanja,dDigit,dSep,tSep));
                    $('#expire_'+ pecahdata[0].trim()).datepicker({
                        changeMonth: true,
                        changeYear: true,
                        dateFormat: 'yy-mm-dd',
                        onClose: function(dateText, inst) {
                            $('#barcode').select();
                        },
                        onSelect: function(dateText) {
                        	var splitNilaiKirim = $('#'+ namacekbox).val().split('___');
                        	var nilaiBaru = splitNilaiKirim[0].trim() +'___'+ splitNilaiKirim[1] +'___'+ splitNilaiKirim[2] +'___'+ dateText;
                            $('#'+ namacekbox).val(nilaiBaru);
                        }
                    });
				}
				$('.dataTables_scrollBody').scrollTop($('.dataTables_scrollBody')[0].scrollHeight);
				$('#barcode').autocomplete('close');
				$('#barcode').select();
			}else{
				$('#pesantext').text('Produk yang dicari atau diinput tidak ada, silahkan masukkan barcode atau kode atau nama produk yang lain...!!!');
				$('#dialogwarning').dialog('open');
			}
		}
	});
}
function kirim_data(cetak){
    cetakstruk = cetak;
	if (totalproduk > 0){
		var sData = $('input[type=checkbox]', oTable.fnGetNodes()).serialize();
		$('#nilaikirim').val(sData);
		$('#dialogbayar').dialog('open');
	}else{
		$('#pesantext').text('Mohon pilih produk terlebih dahulu...!!!');
		$('#dialogwarning').dialog('open');
	}
}
function tutupwarning(){
	$('#dialogwarning').dialog('close');
}
function hapus_latest_produk(){
	if (totalproduk > 0){
        totalbelanja = totalbelanja - (parseFloat($('#lastharga').val())*parseFloat($('#lastqty').val()));
        $('#totalbelanja').html('Total Belanja : '+ currSym +' '+ number_format(totalbelanja,0,',','.'));
        oTable.fnDeleteRow(totalproduk-1);
		totalproduk--;
		if (totalproduk >= 0){
			var nTr = oTable.fnGetNodes(totalproduk-1);
			idproduknya = nTr.getAttribute('id');
			var nilaidataakhir = $('#cekbox_'+ idproduknya).val();
			var pecahnilaiakhir = nilaidataakhir.split('___');
			$('#lastharga').val(pecahnilaiakhir[2]);
			$('#lastqty').val(pecahnilaiakhir[1]);
			$('#last_id').val(pecahnilaiakhir[0]);
		}else{
			$('#lastharga').val('');
			$('#lastqty').val('');
			$('#last_id').val('');
		}
	}else{
		$('#pesantext').text('Mohon pilih produk terlebih dahulu...!!!');
		$('#dialogwarning').dialog('open');
	}
}
function focusbarcode(){
	$('#barcode').select();
}
function hapus_produk(posisi,nTr,idproduk){
	console.log(idproduk);
	var posisibaris = oTable.fnGetPosition(nTr);
	var nilaidata = $('#cekbox_'+ idproduk).val();
	var pecahnilai = nilaidata.split('___');
	totalbelanja = totalbelanja - (parseFloat(pecahnilai[1])*parseFloat(pecahnilai[2]));
	$('#totalbelanja').html('Total : '+ currSym +' '+ number_format(totalbelanja,dDigit,dSep,tSep));
	oTable.fnDeleteRow(posisibaris,focusbarcode);
	totalproduk--;
	if (totalproduk > 0){
		var nilaidataakhir = $('#cekbox_'+ totalproduk).val();
		var pecahnilaiakhir = nilaidataakhir.split('___');
		$('#lastharga').val(pecahnilaiakhir[2]);
		$('#lastqty').val(pecahnilaiakhir[1]);
		$('#last_id').val(pecahnilaiakhir[0]);
	}else{
		$('#lastharga').val('');
		$('#lastqty').val('');
		$('#last_id').val('');
	}
	$('#barcode').focus();
	$('#barcode').select();
}
function akhiri_belanja(cetak){
	if ($('#carabayar').val() == 'TUNAI'){
		if ($('#nilaibayar').val() >= totalbelanja){
			do_selesai(cetak);
		}else{
			alert('Mohon isi pembayaran dengan nilai yang lebih besar atau sama dengan Total Belanja...!!!');
		}
	}else if ($('#carabayar').val() == 'HUTANG'){
		do_selesai(cetak);
	}
}
function do_selesai(cetak){
	var request = new Object();
	request.idsupplier = $('#idsupplier').val();
	request.detail_produk = $('#nilaikirim').val();
	request.totalbelanja = totalbelanja;
	request.bayar = $('#nilaibayar').val();
	request.tglbeli = $('#tglbelikirim').val();
	request.carabayar = $('#carabayar').val();
	if (request.carabayar == 'HUTANG' && $('#jatuhtempokirim').val() != '') {
        request.jatuh_tempo = $('#jatuhtempokirim').val();
    }
	alamat = pathutama + 'pembelian/simpanpembelian';
	$.ajax({
		type: 'POST',
		url: alamat,
		data: request,
		cache: false,
		success: function(data){
			if (cetak == 1){
                window.open(pathutama + "print/6?idpembelian="+ data.trim());
            }
			if (data != 'error'){
				window.location = pathutama + 'pembelian/kasir?tanggal='+ request.tglbeli;
			}else{
				alert('Ada masalah dalam penyimpanan data...!!!');
			}
		}
	});
}
function inisialulangautocomplete(){
	$('#barcode').autocomplete('destroy');
	$('#barcode').autocomplete({
		source: pathutama + 'pembelian/autocaribarang?idsupplier='+ $('#idsupplier').val(),
		select: function (event, ui) {
			console.log(ui.item)
			if (ui.item.barcode){
				$("#barcode").val(ui.item.barcode);
			}else if(!ui.item.barcode && ui.item.alt_code){
				$("#barcode").val(ui.item.alt_code);
			}else if(!ui.item.barcode && !ui.item.alt_code){
				$("#barcode").val(ui.item.value);
			}
			tambahproduk();
		}
	});
	$('#barcode').select();
}

function ubah_harga_produk(posisi,nTr,idproduk){
    barisrubah = nTr;
    console.log(barisrubah);
    console.log(idproduk);
    $("#dialogubahharga2").dialog("open");
}

$(document).ready(function(){
	var tglsekarang = Drupal.settings.tglsekarang;
	var tgltampil = Drupal.settings.tgltampil;
	pathutama = Drupal.settings.basePath;

    currSym = Drupal.settings.currSym;
    tSep = Drupal.settings.tSep[0];
    dSep = Drupal.settings.dSep[0];
	dDigit = Drupal.settings.dec_digit;

	$('#dialogkasir').dialog({
		modal: true,
		width: 925,
		closeOnEscape: false,
		height: 650,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$('#tempatjam').clock({'format':'24','calendar':'false'});
			$('#barcode').focus();
		},
		position: ['auto','auto']
	});
	$('#dialogwarning').dialog({
		modal: true,
		width: 400,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$('#tutupdialog').focus();
		},
		close: function(){
			$('#barcode').select();
		},
		position: ['auto','auto']
	});
	$('#dialogubahqty').dialog({
		modal: true,
		width: 250,
		height: 100,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$('#newqty').val($('#lastqty').val());
			$('#newqty').select();
		},
		close: function(){
			$('#barcode').select();
		},
		position: ['auto','auto']
	});
	$('#dialogubahharga').dialog({
		modal: true,
		width: 250,
		height: 100,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$('#newharga').val($('#lastharga').val());
			$('#newharga').select();
		},
		close: function(){
			$('#barcode').select();
		},
		position: ['auto','auto']
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
            console.log(nilaidata);
            var pecahnilai = nilaidata.split("___");
            $("#newharga2").val(pecahnilai[2]);
            $("#newharga2").select();
        },
        close: function(){
            $("#barcode").select();
        },
        position: ["auto","auto"]
    });
	$('#dialogubahqty2').dialog({
		modal: true,
		width: 250,
		height: 100,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			idproduknya = barisrubah.getAttribute('id');
			var nilaidata = $('#cekbox_'+ idproduknya).val();
			var pecahnilai = nilaidata.split('___');
			$('#newqty2').val(pecahnilai[1]);
			$('#newqty2').select();
		},
		close: function(){
			$('#barcode').select();
		},
		position: ['auto','auto']
	});
	$('#dialogbayar').dialog({
		modal: true,
		width: 550,
		closeOnEscape: false,
		resizable: false,
		autoOpen: false,
		open: function(event, ui) {
			$('#totalbelanjauser').val(currSym +' '+ number_format(totalbelanja,dDigit,dSep,tSep));
			$('#nilaibayar').val(totalbelanja);
			kembali = $('#nilaibayar').val()-totalbelanja;
			$('#kembali').val(currSym +' '+ number_format(kembali,dDigit,dSep,tSep));
			$('#nilaibayar').select();
		},
		close: function(){
			$('#barcode').select();
		},
		position: ['auto','auto']
	});
	tampilkantabelkasir();
	$('#dialogkasir').dialog('open');
	$('.ui-dialog-titlebar').css('font-size','14px');
	$('button').button();
	$('#barcode').keypress(function(e) {
		if (e.keyCode == 114){
			$('#tombolubahqty').click();
		}else if (e.keyCode == 13){
			if ($('#barcode').val() != ''){
				tambahproduk();
			}else{
				$('#pesantext').text('Mohon isi barcode atau kode produk atau nama produk yang ingin dicari...!!!');
				$('#dialogwarning').dialog('open');
			}
		}else if (e.keyCode == 116 || e.keyCode == 117){
			if (e.keyCode == 117){
                cetakstruk = 1;
			}
			kirim_data(cetakstruk);
		}else if (e.keyCode == 115){
			hapus_latest_produk();
		}else if (e.keyCode == 113){
			$('#tombolubahharga').click();
		}
	});
	$('#barcode').autocomplete({
		source: pathutama + 'pembelian/autocaribarang?idsupplier='+ $('#idsupplier').val(),
		select: function (event, ui) {
			if (ui.item.barcode){
				$("#barcode").val(ui.item.barcode);
			}else if(!ui.item.barcode && ui.item.alt_code){
				$("#barcode").val(ui.item.alt_code);
			}else if(!ui.item.barcode && !ui.item.alt_code){
				$("#barcode").val(ui.item.value);
			}
			tambahproduk();
		}
	});
	$('#newqty').keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = totalproduk-1;
			totalbelanja = totalbelanja - (parseFloat($('#lastharga').val())*parseFloat($('#lastqty').val()));
			var nilaiubah = $('#newqty').val();
			oTable.fnUpdate(nilaiubah, baris_int, 4 );
			nilaisubtotal = parseFloat($('#lastharga').val())*nilaiubah;
			subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
			var namacekbox = 'cekbox_'+ $('#last_id').val();
			var nilaikirim = $('#last_id').val() +'___'+ nilaiubah +'___'+ $('#lastharga').val() + '___' + $('#expire_'+ $('#last_id').val()).val();
			var checkboxnilai = '<input checked="checked" style="display: none;" id="'+ namacekbox +'" name="'+ namacekbox +'" type="checkbox" value="'+ nilaikirim +'" />';
			oTable.fnUpdate(subtotalbaru +' '+ checkboxnilai, baris_int, 5 );
			$('#lastqty').val(nilaiubah);
			totalbelanja = totalbelanja + (parseFloat($('#lastharga').val())*parseFloat($('#lastqty').val()));
			$('#totalbelanja').html('Total : '+ currSym +' '+ number_format(totalbelanja,dDigit,dSep,tSep));
			$('#dialogubahqty').dialog('close');
		}
	});
	$('#newharga').keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = totalproduk-1;
			totalbelanja = totalbelanja - (parseFloat($('#lastharga').val())*parseFloat($('#lastqty').val()));
			var nilaiubah = $('#newharga').val();
			oTable.fnUpdate(number_format(nilaiubah,dDigit,dSep,tSep), baris_int, 3 );
			nilaisubtotal = $('#lastqty').val()*nilaiubah;
			subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
			var namacekbox = 'cekbox_'+ $('#last_id').val();
			var nilaikirim = $('#last_id').val() +'___'+ $('#lastqty').val() +'___'+ nilaiubah + '___' + $('#expire_'+ $('#last_id').val()).val();
			var checkboxnilai = '<input checked="checked" style="display: none;" id="'+ namacekbox +'" name="'+ namacekbox +'" type="checkbox" value="'+ nilaikirim +'" />';
			oTable.fnUpdate(subtotalbaru +' '+ checkboxnilai, baris_int, 5 );
			$('#lastharga').val(nilaiubah);
			totalbelanja = totalbelanja + (parseFloat($('#lastharga').val())*parseFloat($('#lastqty').val()));
			$('#totalbelanja').html('Total : '+ currSym +' '+ number_format(totalbelanja,dDigit,dSep,tSep));
			$('#dialogubahharga').dialog('close');
		}
	});
    $("#newharga2").keypress(function(e) {
        if (e.keyCode == 13){
            var baris_int = oTable.fnGetPosition(barisrubah);
            var idproduknya = barisrubah.getAttribute("id").trim();
            var nilaidata = $("#cekbox_"+ idproduknya).val();
            var pecahnilai = nilaidata.split("___");
            var nilaiubah = parseFloat($("#newharga2").val());
            var SubTotalLama = parseFloat(pecahnilai[1])*parseFloat(pecahnilai[2]);
            console.log(SubTotalLama);
            console.log(totalbelanja);
            totalbelanja = totalbelanja - SubTotalLama;

            oTable.fnUpdate(number_format(nilaiubah,dDigit,dSep,tSep), baris_int, 3 );
            nilaisubtotal = nilaiubah*parseFloat(pecahnilai[1]);
            subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
            var namacekbox = "cekbox_"+ idproduknya;
            var nilaikirim = idproduknya +"___"+ pecahnilai[1] +"___"+ nilaiubah +"___"+ pecahnilai[3];
            var checkboxnilai = "<input class=\"nilai_kirim\" checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" />";
            oTable.fnUpdate('<span id="subtotal-'+ idproduknya +'">'+subtotalbaru +'</span> '+ checkboxnilai, baris_int, 5 );
            totalbelanja = totalbelanja + nilaisubtotal;
            posisiakhir = totalproduk-1;
            if (baris_int == posisiakhir){
                $("#lastharga").val(nilaiubah);
            }
            $("#totalbelanja").html("Total : "+ currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
            $("#dialogubahharga2").dialog("close");
        }
    });
	$('#newqty2').keypress(function(e) {
		if (e.keyCode == 13){
			var baris_int = oTable.fnGetPosition(barisrubah);
			var idproduknya = barisrubah.getAttribute('id');
			var nilaidata = $('#cekbox_'+ idproduknya).val();
			var pecahnilai = nilaidata.split('___');
			totalbelanja = totalbelanja - (parseFloat(pecahnilai[1])*parseFloat(pecahnilai[2]));
			var nilaiubah = $('#newqty2').val();
			oTable.fnUpdate(nilaiubah, baris_int, 4 );
			nilaisubtotal = parseFloat(pecahnilai[2])*nilaiubah;
			subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
			var namacekbox = 'cekbox_'+ idproduknya;
			var nilaikirim = idproduknya +'___'+ nilaiubah +'___'+ pecahnilai[2] + '___' + $('#expire_'+ $('#last_id').val()).val();;
			var checkboxnilai = '<input checked="checked" style="display: none;" id="'+ namacekbox +'" name="'+ namacekbox +'" type="checkbox" value="'+ nilaikirim +'" />';
			oTable.fnUpdate(subtotalbaru +' '+ checkboxnilai, baris_int, 5 );
			totalbelanja = totalbelanja + (parseFloat(pecahnilai[2])*nilaiubah);
			posisiakhir = totalproduk-1;
			if (baris_int == posisiakhir){
				$('#lastqty').val(nilaiubah);
			}
			$('#totalbelanja').html('Total : '+ currSym +' '+ number_format(totalbelanja,dDigit,dSep,tSep));
			$('#dialogubahqty2').dialog('close');
		}
	});
	$('#nilaibayar').keyup(function(e){
		kembali = $('#nilaibayar').val()-totalbelanja;
		$('#kembali').val( currSym +' '+ number_format(kembali,dDigit,dSep,tSep));
		if (e.keyCode == 13){
			akhiri_belanja(cetakstruk);
		}
	});
	$('#tglbeli').datepicker({
		changeMonth: true,
		changeYear: true,
		dateFormat: 'dd-mm-yy',
		altField: '#tglbelikirim',
		altFormat: 'yy-mm-dd',
		onClose: function(dateText, inst) {
			$('#barcode').select();
		}
	});
	$("#tglbeli").css("width","177px");
	$('#carabayar').change(function(){
		if ($(this).val() == 'HUTANG'){
            $('#barisjatuhtempo').show();
			$('#nilaibayar').val(0);
			kembali = $('#nilaibayar').val()-totalbelanja;
			$('#kembali').val(currSym +' '+ number_format(kembali,dDigit,dSep,tSep));
			$('#nilaibayar').select();
		}else{
            $('#barisjatuhtempo').hide();
        }
	});
	$('#info-kasir-waktu').css('background','url('+ Drupal.settings.logo +') 99% 50% no-repeat');
	$('#info-kasir-waktu').css('background-size','75px 75px');
    $('#jatuhtempo').datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: 'dd-mm-yy',
        altField: '#jatuhtempokirim',
        altFormat: 'yy-mm-dd',
        onClose: function(dateText, inst) {
            $('#nilaibayar').select();
        }
    });
})