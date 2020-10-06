var oTable;
var pathutama = '';
var giCount = 1;
var totalbelanja = 0;
var totalbelanjaround = 0;
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
var dDigC = 0;
var IdPenjualan = 0;
var LastSelectCarabayar2 = '';
var DetailLaundry = [];
function round5(x)
{
	var FixDigit = number_format(x, dDigit, '.', '');
	var StrVal = FixDigit.toString();
	var SplitVal = StrVal.split('.');
	var DesVal = parseInt(SplitVal[1]);
    var RoundDes = (DesVal % 5) >= 5 ? parseInt(DesVal / 5) * 5 + 5 : parseInt(DesVal / 5) * 5;
    var RetVal = parseInt(SplitVal[0]) + (RoundDes/(Math.pow(10,dDigit)));
    return RetVal;
}

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
function ubah_qty_produk(posisi,nTr){
	barisrubah = nTr;
	$("#dialogubahqty2").dialog("open");
}
function tambahproduk(qtyAdd, batch_code, qty_pcs = 1){
	var request = new Object();
	if ($('#hiddenbarcode').val() != ''){
		var katacari = $("#hiddenbarcode").val();
		var katacari2 = $("#barcode").val();
	}else{
		var katacari = $("#barcode").val();
        var katacari2 = 0;
    }
	var pecahkatacari = katacari.split("--->");
	request.barcode = pecahkatacari[0];
	if (katacari2 != 0) {
        var pecahkatacari2 = katacari2.split("--->");
        request.barcode = pecahkatacari2[0];
        request.katacari = pecahkatacari2[1];
    }
    request.idpelanggan = $("#idpelanggan").val();
	var alamatcariproduk = pathutama +"penjualan/cariproduk";
	$.ajax({
		type: "POST",
		url: alamatcariproduk,
		data: request,
		cache: false,
		success: function(data){
			var pecahdata = new Array();
			pecahdata = data.split(";");
            var BarcodeCheck = request.barcode.substr(0,2);
            if (pecahdata[0].trim() != "error" && BarcodeCheck != '22' && typeof pecahdata[1] != 'undefined' && !isNaN(pecahdata[2])) {
                nilaisubtotal = (pecahdata[2] - ((pecahdata[2] * pecahdata[3]) / 100)) * qtyAdd;
                subtotal = number_format(nilaisubtotal, dDigit, dSep, tSep);
                nilaikirim = pecahdata[0].trim() + "___" + qtyAdd + "___" + pecahdata[2] + "___" + pecahdata[3] + "___";
                nilaikirim += pecahdata[7] + "___" + qty_pcs + "___";
                nilaikirim += pecahdata[8] + "___" + pecahdata[9];
                index_cek_box = pecahdata[0].trim();
                namacekbox = "cekbox_" + index_cek_box;
                if ($("#" + namacekbox).val()) {
                    var nilaicekbox = $("#" + namacekbox).val();
                    var pecahnilai = nilaicekbox.split("___");
                    var qtybaru = parseFloat(pecahnilai[1]) + 1;
                    var subtotallama = pecahnilai[1] * (pecahnilai[2] - (pecahnilai[2] * pecahnilai[3] / 100));
                    var subtotal = qtybaru * (pecahnilai[2] - (pecahnilai[2] * pecahnilai[3] / 100));
                    totalbelanja = totalbelanja - subtotallama + subtotal;
                    $("#totalbelanja").html("Total : " + currSym + " " + number_format(Math.abs(totalbelanja), dDigit, dSep, tSep));
                    var nTr = $("#" + namacekbox).parent().parent().get(0);
                    var posisibaris = oTable.fnGetPosition(nTr);
                    oTable.fnUpdate(qtybaru, posisibaris, 5);
                    nilaikirim = pecahnilai[0].trim() + "___" + qtybaru + "___" + pecahnilai[2] + "___";
                    nilaikirim += pecahnilai[3] + "___" + pecahnilai[4] + "___" + pecahnilai[5] + "___";
                    nilaikirim += pecahnilai[6] + "___" + pecahnilai[7];
                    checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" class=\"checkbox-produk\" />";
                    subtotalView = number_format(Math.abs(subtotal), dDigit, dSep, tSep);
                    oTable.fnUpdate(subtotalView + " " + checkboxnilai, posisibaris, 6);
                    posisiakhir = totalproduk - 1;
                    if (posisibaris == posisiakhir) {
                        $("#lastqty").val(qtybaru);
                    }
                    if (typeof pecahnilai[6] != 'undefined') {
                        $("#hiddenbarcode").val(pecahnilai[4].trim());
                    }
                } else {
                    var icondelete = "<img onclick=\"hapus_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk menghapus\" src=\"" + pathutama + "misc/media/images/close.ico\" width=\"24\" class=\"hapus-product\" >";
                    var iconubah = "<img onclick=\"ubah_qty_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah qty produk ini\" src=\"" + pathutama + "misc/media/images/edit.ico\" width=\"24\">";
                    var iconharga = "<img onclick=\"ubah_harga_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah harga produk ini\" src=\"" + pathutama + "misc/media/images/money2.png\" width=\"24\">";

                    $("#lastharga").val(pecahdata[2]);
                    $("#lastdiskon").val(pecahdata[3]);
                    $("#last_id").val(pecahdata[0].trim());
                    $("#lastbarcode").val(pecahdata[7]);
                    $("#lastqty").val(qtyAdd);
                    checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" class=\"checkbox-produk\" />";
                    var row = "<tr id=\"" + index_cek_box + "\">";
                    row += "<td>" + icondelete + "</td>";
                    row += "<td>" + iconubah + "</td>";
                    if (typeof DetailLaundry['bar'+ pecahdata[7]] != 'undefined' && DetailLaundry['bar'+ pecahdata[7]]){
                        row += "<td>" + pecahdata[1] + "[RAK : "+ DetailLaundry['bar'+ pecahdata[7]] +"]</td>";
                    }else{
                        row += "<td>" + pecahdata[1] + "</td>";
                    }
                    row += "<td class=\"angka\">" + number_format(pecahdata[2], dDigit, dSep, tSep) + "</td>";
                    row += "<td class=\"angka\">" + pecahdata[3] + "</td>";
                    row += "<td class=\"angka\">" + qtyAdd + "</td>";
                    row += "<td class=\"angka\">" + subtotal + " " + checkboxnilai + "</td>";
                    row += "<td>" + iconharga + "</td>";
                    row += "</tr>";
                    $("#tabel_kasir").dataTable().fnAddTr($(row)[0]);
                    giCount++;
                    totalproduk++;
                    totalbelanja = totalbelanja + nilaisubtotal;
                    totalbelanjaView = number_format(Math.abs(totalbelanja), dDigit, dSep, tSep);
                    $("#totalbelanja").html("Total : " + currSym + " " + totalbelanjaView);
                }
                $('.dataTables_scrollBody').scrollTop($('.dataTables_scrollBody')[0].scrollHeight);
                $("#barcode").val("");
                $("#kasir").select();
                $("#barcode").select();
            }else{
                var BarcodeProduct = request.barcode.substr(0,7);
                var BarcodeCheck = request.barcode.substr(0,2);
                if (BarcodeCheck == '22') {
                    var ProductPrice = parseInt(request.barcode.substr(7, 5)) / 100;
                    var request2 = new Object();
                    prodTotal = parseFloat(ProductPrice.toFixed(2));
                    request2.barcode = BarcodeProduct.substr(2,5);
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
                            if (pecahdata[0].trim() != "error" && typeof pecahdata[1] != 'undefined' && !isNaN(pecahdata[2])) {
                                var IdProduct = pecahdata[0].trim();
                                qtyAdd = prodTotal/(pecahdata[2]);
                                qtyAdd = qtyAdd.toFixed(3);
                                nilaisubtotal = (pecahdata[2] - ((pecahdata[2] * pecahdata[3]) / 100)) * qtyAdd;
                                subtotal = number_format(nilaisubtotal,dDigit,dSep,tSep);
                                nilaikirim = IdProduct + "___" + qtyAdd + "___" + pecahdata[2] + "___" + pecahdata[3] + "___";
                                nilaikirim += pecahdata[7] + "___" + qty_pcs + "___";
                                nilaikirim += pecahdata[8] + "___" + pecahdata[9];
                                index_cek_box = IdProduct;
                                namacekbox = "cekbox_" + index_cek_box;
                                if ($("#" + namacekbox).val()) {
                                    var nilaicekbox = $("#" + namacekbox).val();
                                    var pecahnilai = nilaicekbox.split("___");
                                    var qtybaru = parseFloat(pecahnilai[1]) + parseFloat(qtyAdd);
                                    qtybaru = parseFloat(qtybaru.toFixed(3));
                                    var subtotallama = pecahnilai[1] * (pecahnilai[2] - (pecahnilai[2] * pecahnilai[3] / 100));
                                    var subtotal = qtybaru * (pecahnilai[2] - (pecahnilai[2] * pecahnilai[3] / 100));
                                    var subtotal = parseFloat(subtotal.toFixed(2));
                                    totalbelanja = parseFloat(totalbelanja) - parseFloat(subtotallama) + subtotal;
                                    $("#totalbelanja").html("Total Transaksi : " + currSym + " " + number_format(Math.abs(totalbelanja),dDigit,dSep,tSep));
                                    var nTr = $("#" + namacekbox).parent().parent().get(0);
                                    var posisibaris = oTable.fnGetPosition(nTr);
                                    oTable.fnUpdate(qtybaru, posisibaris, 5);
                                    nilaikirim = pecahnilai[0].trim() + "___" + qtybaru + "___" + pecahnilai[2] + "___" + pecahnilai[3] + "___" ;
                                    nilaikirim += pecahnilai[4] + "___" + pecahnilai[5] + "___";
                                    nilaikirim += pecahnilai[6] + "___" + pecahnilai[7];
                                    checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" class=\"checkbox-produk\" />";
                                    oTable.fnUpdate(number_format(Math.abs(subtotal),dDigit,dSep,tSep) + " " + checkboxnilai, posisibaris, 6);
                                    posisiakhir = totalproduk - 1;
                                    if (posisibaris == posisiakhir) {
                                        $("#lastqty").val(qtybaru);
                                    }
                                    if (typeof pecahnilai[4] != 'undefined') {
                                        $("#hiddenbarcode").val(pecahnilai[4].trim());
                                    }
                                    $("#barcode").val("");
                                    $("#kasir").select();
                                    $("#barcode").select();
                                } else {
                                    var icondelete = "<img onclick=\"hapus_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk menghapus\" src=\"" + pathutama + "misc/media/images/close.ico\" width=\"24\" class=\"hapus-product\" >";
                                    var iconubah = "<img onclick=\"ubah_qty_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah qty produk ini\" src=\"" + pathutama + "misc/media/images/edit.ico\" width=\"24\">";
                                    var iconharga = "<img onclick=\"ubah_harga_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + pecahdata[0].trim() + "\')\" title=\"Klik untuk mengubah harga produk ini\" src=\"" + pathutama + "misc/media/images/money2.png\" width=\"24\">";

                                    $("#lastharga").val(pecahdata[2]);
                                    $("#lastdiskon").val(pecahdata[3]);
                                    $("#last_id").val(IdProduct);
                                    $("#lastbarcode").val(pecahdata[7]);
                                    $("#lastqty").val(qtyAdd);
                                    checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" class=\"checkbox-produk\" />";
                                    var row = "<tr id=\"" + index_cek_box + "\">";
                                    row += "<td>" + icondelete + "</td>";
                                    row += "<td>" + iconubah + "</td>";
                                    if (typeof DetailLaundry['bar'+ pecahdata[7]] != 'undefined' && DetailLaundry['bar'+ pecahdata[7]]){
                                        row += "<td>" + pecahdata[1] + "[RAK : "+ DetailLaundry['bar'+ pecahdata[7]] +"]</td>";
                                    }else{
                                        row += "<td>" + pecahdata[1] + "</td>";
                                    }
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
                                $("#barcode").val("");
                                $("#kasir").select();
                                $("#barcode").select();
                            } else {
                            	//Last check if there is no fresh mart product but using 22 barcode
                                var request3 = new Object();
                                request3.barcode = request.barcode;
                                request3.idpelanggan = $("#idpelanggan").val();
                                alamatcariproduk = pathutama +"penjualan/cariproduk";
                                $.ajax({
                                    type: "POST",
                                    url: alamatcariproduk,
                                    data: request3,
                                    cache: false,
                                    success: function (data3) {
                                        var PecahDataLast = new Array();
                                        PecahDataLast = data3.split(";");
                                        if (PecahDataLast[0].trim() != "error" && typeof pecahdata[1] != 'undefined' && !isNaN(pecahdata[2])) {
                                            nilaisubtotal = (PecahDataLast[2] - ((PecahDataLast[2]*PecahDataLast[3])/100)) * qtyAdd;
                                            subtotal = number_format(nilaisubtotal,dDigit,dSep,tSep);
                                            nilaikirim = PecahDataLast[0].trim() +"___"+ qtyAdd +"___"+ PecahDataLast[2] +"___";
                                            nilaikirim += PecahDataLast[3] +"___"+ PecahDataLast[7] + "___" + qty_pcs + "___";
                                            nilaikirim += PecahDataLast[8] +"___"+ PecahDataLast[9];
                                            index_cek_box = PecahDataLast[0].trim();
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
                                                nilaikirim = pecahnilai[0].trim() +"___"+ qtybaru +"___"+ pecahnilai[2] +"___";
                                                nilaikirim += pecahnilai[3] +"___"+ pecahnilai[4] +"___"+ pecahnilai[5] +"___";
                                                nilaikirim += pecahnilai[6] +"___"+ pecahnilai[7];
                                                checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" class=\"checkbox-produk\" />";
                                                subtotalView = number_format(Math.abs(subtotal),dDigit,dSep,tSep);
                                                oTable.fnUpdate(subtotalView +" "+ checkboxnilai, posisibaris, 6 );
                                                posisiakhir = totalproduk-1;
                                                if (posisibaris == posisiakhir){
                                                    $("#lastqty").val(qtybaru);
                                                }
                                                if (typeof pecahnilai[4] != 'undefined'){
                                                    $("#hiddenbarcode").val(PecahDataLast[4].trim());
                                                }
                                            }else{
                                                var icondelete = "<img onclick=\"hapus_produk(\'"+ index_cek_box +"\',this.parentNode.parentNode,\'"+ PecahDataLast[0].trim() +"\')\" title=\"Klik untuk menghapus\" src=\""+ pathutama +"misc/media/images/close.ico\" width=\"24\" class=\"hapus-product\" >";
                                                var iconubah = "<img onclick=\"ubah_qty_produk(\'"+ index_cek_box +"\',this.parentNode.parentNode,\'"+ PecahDataLast[0].trim() +"\')\" title=\"Klik untuk mengubah qty produk ini\" src=\""+ pathutama +"misc/media/images/edit.ico\" width=\"24\">";
                                                var iconharga = "<img onclick=\"ubah_harga_produk(\'" + index_cek_box + "\',this.parentNode.parentNode,\'" + PecahDataLast[0].trim() + "\')\" title=\"Klik untuk mengubah harga produk ini\" src=\"" + pathutama + "misc/media/images/money2.png\" width=\"24\">";

                                                $("#lastharga").val(PecahDataLast[2]);
                                                $("#lastdiskon").val(PecahDataLast[3]);
                                                $("#last_id").val(PecahDataLast[0].trim());
                                                $("#lastbarcode").val(PecahDataLast[7]);
                                                $("#lastqty").val(qtyAdd);
                                                checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\""+ namacekbox +"\" name=\""+ namacekbox +"\" type=\"checkbox\" value=\""+ nilaikirim +"\" class=\"checkbox-produk\" />";
                                                var row = "<tr id=\""+ index_cek_box +"\">";
                                                row += "<td>"+ icondelete +"</td>";
                                                row += "<td>"+ iconubah +"</td>";
                                                if (typeof DetailLaundry['bar'+ PecahDataLast[7]] != 'undefined' && DetailLaundry['bar'+ PecahDataLast[7]]){
                                                    row += "<td>" + PecahDataLast[1] + "[RAK : "+ DetailLaundry['bar'+ PecahDataLast[7]] +"]</td>";
                                                }else{
                                                    row += "<td>" + PecahDataLast[1] + "</td>";
                                                }
                                                row += "<td class=\"angka\">"+ number_format(PecahDataLast[2],dDigit,dSep,tSep) +"</td>";
                                                row += "<td class=\"angka\">"+ PecahDataLast[3] +"</td>";
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
                                            $("#barcode").val("");
                                            $("#kasir").select();
                                            $("#barcode").select();
                                        }else{
                                            $("#pesantext").text("Produk yang dicari atau diinput tidak ada, silahkan masukkan barcode atau kode atau nama produk yang lain...!!!");
                                            $("#dialogwarning").dialog("open");
										}
                                    }
                                })
                            }
                            $("#hiddenbarcode").val("");
                        }
                    });
                }else{
                    $("#pesantext").text("Produk yang dicari atau diinput tidak ada, silahkan masukkan barcode atau kode atau nama produk yang lain...!!!");
                    $("#hiddenbarcode").val("");
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
    request.ppn = $("#ppn_value").val();
    var totalbelanjappn = (totalbelanja * ($("#ppn_value").val() / 100)) + totalbelanja;
    request.totalbelanjappn = totalbelanjappn;
    var VoucherChecked = $('#use-voucher:checkbox:checked').length;
    if (VoucherChecked) {
        request.voucher_value = $('#voucher_value').val();
        request.voucher_number = $('#voucher_number').val();
        request.total_after_voucher = parseFloat(totalbelanjappn) - parseFloat(request.voucher_value);
    } else {
        request.voucher_value = 0;
        request.voucher_number = null;
        request.total_after_voucher = totalbelanjappn;
    }
    request.totalbelanjaround = totalbelanjaround;
    request.carabayar = $("#carabayar").val();
    var BayarMultiple = new Array;
    var CaraBayar = $("#carabayar").val();
    if (CaraBayar == 'TUNAI') {
        BayarMultiple.push(CaraBayar + ':' + $('#nilaibayar').val());
    } else if (CaraBayar == 'DEBIT') {
        BayarMultiple.push(CaraBayar + ':' + $('#nilai_bayar_debit').val());
    } else if (CaraBayar == 'KAD KREDIT' || CaraBayar == 'KARTU KREDIT') {
        BayarMultiple.push(CaraBayar + ':' + $('#nilai_bayar_kredit').val());
    } else if (CaraBayar == 'HUTANG') {
        BayarMultiple.push(CaraBayar + ':' + $('#nilai_bayar_hutang').val());
    } else if (CaraBayar == 'DEPOSIT') {
        BayarMultiple.push(CaraBayar + ':' + $('#nilai_bayar_deposit').val());
    }
    var Carabayar2Checked = $('#use-carabayar2:checkbox:checked').length;
    if (Carabayar2Checked){
        if ($('#carabayar2').val() == 'TUNAI'){
            BayarMultiple.push('TUNAI:' + $('#nilaibayar').val());
        }else if($('#carabayar2').val() == 'DEBIT'){
            BayarMultiple.push('DEBIT:' + $('#nilai_bayar_debit').val());
        }else if($('#carabayar2').val() == 'KAD KREDIT' || $('#carabayar2').val() == 'KARTU KREDIT'){
            BayarMultiple.push('KAD KREDIT:' + $('#nilai_bayar_kredit').val());
        }
    }
    request.bayar_multiple = BayarMultiple.join(';');
    var TotalBayar = calculate_total_bayar();
    request.bayar = TotalBayar.toFixed(dDigit);
    var kembalian = parseFloat(request.bayar) - parseFloat(request.totalbelanjaround);
    if (kembalian > 0) {
        request.perlakuankembalian = $("#kembalian").val();
    }
    if ($("#idpelanggan").val() == 0 && $("#kembalian").val() == 2) {
        request.perlakuankembalian = 0;
    }
    request.tgljual = $("#tgljualkirim").val();
    if ($("#nomerkartu").val() != '') {
        request.nomerkartu = $("#nomerkartu").val();
    }
    if ($("#nomer_kartu_kredit").val() != '') {
        request.nomer_kartu_kredit = $("#nomer_kartu_kredit").val();
    }
	//request.idmeja = //$('#idmeja').val();
	if ((kembalian < 0 && $("#idpelanggan").val() != 0) || kembalian >= 0){
		alamat = pathutama + "penjualan/simpanpenjualan";
		$.ajax({
			type: "POST",
			url: alamat,
			data: request,
			cache: false,
			success: function(data){
                try {
                    var returndata = data.trim();
                    if (returndata != "error") {
                        IdPenjualan = returndata;
                        $('#dialogbayar').parent().unblock();
                        if (cetak == 1) {
                            window.open(pathutama + "print/6?idpenjualantemp=" + IdPenjualan);
                        }
                        $('#selesai-transaksi').button("enable");
                        $('#selesai-transaksi').focus();
                    } else {
                        alert("Ada masalah dalam penyimpanan data...!!!");
                    }
                }catch(err) {
                    alert(err.message);
                }
			}
		});
	}else{
        $('#dialogbayar').parent().unblock();
		alert("Mohon pilih pelanggan terlebih dulu jika pembayaran dengan cara hutang...!!!");
        $('#simpan-transaksi').button("disable").on('click', function() {
            $('#dialogbayar').parent().block();
            $('#simpan-transaksi').off('click');
            akhiri_belanja(cetakstruk);
        });
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
					if (diskonProduk <= 0){
						diskonProduk = 0;
					}
					var barisdiubah = document.getElementById('cekbox_'+ idProduk).parentNode.parentNode;
					var baris_int = oTable.fnGetPosition(barisdiubah);
					var idproduknya = barisdiubah.getAttribute("id");
					var nilaidata = $("#cekbox_"+ idproduknya).val();
					var pecahnilai = nilaidata.split("___");
					oTable.fnUpdate(diskonProduk, baris_int, 4 );
					nilaisubtotal = (parseFloat(pecahnilai[2])-(parseFloat(pecahnilai[2])*diskonProduk/100))*parseFloat(splitNilai[1]);
					totalbelanja += parseFloat(nilaisubtotal);
                    var subtotalbaru = number_format(nilaisubtotal,dDigit,dSep,tSep);
					var namacekbox = "cekbox_"+ idproduknya;
					var nilaikirim = idproduknya +"___"+ splitNilai[1] +"___"+ pecahnilai[2];
                   nilaikirim += "___"+ diskonProduk +"___"+ pecahnilai[4] +"___"+ pecahnilai[5];
                    nilaikirim += "___"+ pecahnilai[6] +"___"+ pecahnilai[7];
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
    if ($('#nilai_bayar_deposit').is(':visible')){
    	if ($('#nilai_bayar_deposit').val() != '') {
            TotalBayar = TotalBayar + parseFloat($('#nilai_bayar_deposit').val());
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
            TotalBayar = parseFloat(number_format(TotalBayar,dDigit,'.',''));
            var TotalPlusPpn = (totalbelanja * (parseInt($('#ppn_value').val())/100)) + totalbelanja;
            TotalPlusPpn = parseFloat(number_format(TotalPlusPpn,dDigit,'.',''));
            var VoucherChecked = $('#use-voucher:checkbox:checked').length;
            if (VoucherChecked){
                TotalBelanjaVoucher = parseFloat(TotalPlusPpn) - parseFloat($('#voucher_value').val());
            }else{
                TotalBelanjaVoucher = TotalPlusPpn;
            }
            if (dDigC == 1) {
                totalbelanjaround = round5(TotalBelanjaVoucher);
            }else{
            	totalbelanjaround = TotalBelanjaVoucher;
			}
            var CaraBayar = $("#carabayar").val();
            var Hutang = false;
            if (CaraBayar == 'HUTANG'){
                Hutang = true;
            }
            if (parseFloat(TotalBayar) >= parseFloat(totalbelanjaround)) {
                $("#status-message").removeClass('message-error').html('');
                //if (parseFloat(TotalBayar) > parseFloat(totalbelanjaround)){
                //    $("#field_kembalian").show();
                //    $("#kembalian").focus();
                //}else{
                //    $("#field_kembalian").hide();
                    $('#simpan-transaksi').button('enable').focus();
                //}
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
        var VoucherChecked = $('#use-voucher:checkbox:checked').length;
        if (VoucherChecked){
            TotalBelanjaVoucher = parseFloat(TotalPlusPpn) - parseFloat($('#voucher_value').val());
        }else{
            TotalBelanjaVoucher = parseFloat(TotalPlusPpn)
        }
        if (dDigC == 1) {
            totalbelanjaround = round5(TotalBelanjaVoucher);
        }else{
            totalbelanjaround = TotalBelanjaVoucher;
		}
        var Kembali = TotalBayar - totalbelanjaround;
        $('#kembali').val(number_format(Kembali,dDigit,dSep,tSep));
    }
}

function close_kasir(){
	var konfirmasi = confirm('Yakin..???');
	if (konfirmasi){
		window.location = pathutama + 'print/6?close_cashier=1';
	}
}

function SyncRequest(){
    var AlamatSync = pathutama + 'penjualan/syncpenjualan';
    $.ajax({
        url: AlamatSync,
        type: 'POST',
        cache: false,
        success: function(data){

        },
    });
}

function disableF5(e) { if ((e.which || e.keyCode) == 116) kirim_data(1); };

$(document).on("keydown", disableF5);

$(document).ready(function(){
	pathutama = Drupal.settings.basePath;
	tglsekarang = Drupal.settings.tglsekarang;
	tgltampil = Drupal.settings.tgltampil;
	alamatasal = Drupal.settings.alamatasal;

	currSym = Drupal.settings.currSym;
	tSep = Drupal.settings.tSep[0];
	dSep = Drupal.settings.dSep[0];
    dDigit = Drupal.settings.dec_digit;
    dDigC = Drupal.settings.dec_digit_cashier;

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
            $('.dataTables_scrollBody').scrollTop($('.dataTables_scrollBody')[0].scrollHeight);
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
            $('.dataTables_scrollBody').scrollTop($('.dataTables_scrollBody')[0].scrollHeight);
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
            $('.dataTables_scrollBody').scrollTop($('.dataTables_scrollBody')[0].scrollHeight);
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
            $('.dataTables_scrollBody').scrollTop($('.dataTables_scrollBody')[0].scrollHeight);
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
            totalbelanja = parseFloat(number_format(totalbelanja,dDigit,'.',''));
            $("#totalbelanjauser").val(currSym +" "+ number_format(totalbelanja,dDigit,dSep,tSep));
			var total_ppn_value = totalbelanja * (parseInt($('#ppn_value').val())/100);
            $("#total_ppn").val(currSym +" "+ number_format(total_ppn_value,dDigit,dSep,tSep));
			var total_plus_ppn = (totalbelanja * (parseInt($('#ppn_value').val())/100)) + totalbelanja;
            total_plus_ppn = parseFloat(number_format(total_plus_ppn,dDigit,'.',''));
            var VoucherChecked = $('#use-voucher:checkbox:checked').length;
            var TotalAfterVoucher = total_plus_ppn;
            if (VoucherChecked){
                TotalAfterVoucher = parseFloat(TotalAfterVoucher) - parseFloat($('#voucher_value').val());
			}
			if (dDigC == 1) {
                totalbelanjaround = round5(TotalAfterVoucher);
            }else{
                totalbelanjaround = TotalAfterVoucher;
			}
			$("#total_plus_ppn").val(currSym +" "+ number_format(TotalAfterVoucher,dDigit,dSep,tSep));
            $("#penggenapan").val(currSym +" "+ number_format(totalbelanjaround,dDigit,dSep,tSep));
            $("#nilaibayar").val(totalbelanjaround);
			$("#kembali").val(currSym +" 0");
			$("#nilaibayar").select();
            //$('#nomormeja').select();
			$("#idpelanggan").removeAttr("disabled");
            //$("#carabayar").chosen();
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
                        /*$("#idpelanggan").attr("disabled", "disabled");*/
                        $("#carabayar").trigger("chosen:updated");
                    }
                });
            }else{
                $("#carabayar option").each(function(){
                    if ($(this).attr('value') == 'DEPOSIT' || $(this).attr('value') == 'HUTANG'){
                        $(this).attr('disabled', 'disabled');
                    }
                });
                $("#baris-deposit").hide();
                $("#carabayar").trigger("chosen:updated");
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
        delay: 0,
        minLength: 3,
        autoFocus: true,
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
			var nilaiubah = $("#newharga").val();
			if (!isNaN(nilaiubah * 1)) {
                var baris_int = totalproduk - 1;
                totalbelanja = totalbelanja - ($("#lastharga").val()-($("#lastharga").val()*$("#lastdiskon").val()/100))*$("#lastqty").val();
                oTable.fnUpdate(number_format(nilaiubah, dDigit, dSep, tSep), baris_int, 3);
                nilaisubtotal = (nilaiubah - (nilaiubah * $("#lastdiskon").val() / 100)) * $("#lastqty").val();
                subtotalbaru = number_format(nilaisubtotal, dDigit, dSep, tSep);
                var namacekbox = "cekbox_" + $("#last_id").val().trim();
                var splitNilaiCekBox = $('#' + namacekbox).val().split('___');
                var nilaikirim = $("#last_id").val() + "___" + $("#lastqty").val() + "___" + nilaiubah + "___";
                nilaikirim += $("#lastdiskon").val() + "___" + splitNilaiCekBox[4] + "___" + splitNilaiCekBox[5] + "___";
                nilaikirim += splitNilaiCekBox[6] + "___" + splitNilaiCekBox[7];
                var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" class=\"checkbox-produk\" />";
                oTable.fnUpdate(subtotalbaru + " " + checkboxnilai, baris_int, 6);
                $("#lastharga").val(nilaiubah);
                totalbelanja = totalbelanja + nilaisubtotal;
                $("#totalbelanja").html("Total : " + currSym + " " + number_format(totalbelanja, dDigit, dSep, tSep));
            }else{
			    alert('Wajib input angka/bilangan...!!!');
            }
            $("#dialogubahharga").dialog("close");
		}
	});
    $("#newharga2").keypress(function(e) {
        if (e.keyCode == 13){
            var nilaiubah = $("#newharga2").val();
            if (!isNaN(nilaiubah * 1)) {
                var baris_int = oTable.fnGetPosition(barisrubah);
                var idproduknya = barisrubah.getAttribute("id").trim();
                var PpnValue = 0;
                if ($('#ppn-' + idproduknya).is(':checked')) {
                    PpnValue = 10;
                }
                var nilaidata = $("#cekbox_" + idproduknya).val();
                var pecahnilai = nilaidata.split("___");
                var SubTotalLama = (pecahnilai[1] * (pecahnilai[2] - (pecahnilai[2] * pecahnilai[3] / 100)));
                SubTotalLama = SubTotalLama + (SubTotalLama * PpnValue / 100);
                totalbelanja = totalbelanja - SubTotalLama;
                oTable.fnUpdate(number_format(nilaiubah, dDigit, dSep, tSep), baris_int, 3);
                nilaisubtotal = (nilaiubah - (nilaiubah * pecahnilai[3] / 100)) * pecahnilai[1];
                nilaisubtotal = nilaisubtotal + (nilaisubtotal * PpnValue / 100);
                subtotalbaru = number_format(nilaisubtotal, dDigit, dSep, tSep);
                var namacekbox = "cekbox_" + idproduknya;
                var nilaikirim = idproduknya + "___" + pecahnilai[1] + "___" + nilaiubah + "___" + pecahnilai[3];
                nilaikirim += "___" + pecahnilai[4] + "___" + pecahnilai[5];
                nilaikirim += "___" + pecahnilai[6] + "___" + pecahnilai[7];
                var checkboxnilai = "<input class=\"nilai_kirim\" checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" />";
                oTable.fnUpdate('<span id="subtotal-' + idproduknya + '">' + subtotalbaru + '</span> ' + checkboxnilai, baris_int, 6);
                totalbelanja = totalbelanja + nilaisubtotal;
                posisiakhir = totalproduk - 1;
                if (baris_int == posisiakhir) {
                    $("#lastharga").val(nilaiubah);
                }
                $("#totalbelanja").html("Total : " + currSym + " " + number_format(totalbelanja, dDigit, dSep, tSep));
            }else{
                alert('Wajib input angka/bilangan...!!!');
            }
            $("#dialogubahharga2").dialog("close");
        }
    });
	$("#newqty").keypress(function(e) {
		if (e.keyCode == 13){
            var nilaiubah = $("#newqty").val();
            if (!isNaN(nilaiubah * 1)) {
                var baris_int = totalproduk - 1;
                totalbelanja = totalbelanja - ($("#lastharga").val() - ($("#lastharga").val() * $("#lastdiskon").val() / 100)) * $("#lastqty").val();
                oTable.fnUpdate(nilaiubah, baris_int, 5);
                nilaisubtotal = ($("#lastharga").val() - ($("#lastharga").val() * $("#lastdiskon").val() / 100)) * nilaiubah;
                subtotalbaru = number_format(nilaisubtotal, dDigit, dSep, tSep);
                var namacekbox = "cekbox_" + $("#last_id").val().trim();
                var splitNilaiCekBox = $('#' + namacekbox).val().split('___');
                var nilaikirim = $("#last_id").val().trim() + "___" + nilaiubah + "___" + $("#lastharga").val() + "___";
                nilaikirim += $("#lastdiskon").val() + "___" + splitNilaiCekBox[4] + "___" + splitNilaiCekBox[5];
                nilaikirim += "___" + splitNilaiCekBox[6] + "___" + splitNilaiCekBox[7];
                var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" class=\"checkbox-produk\" />";
                oTable.fnUpdate(subtotalbaru + " " + checkboxnilai, baris_int, 6);
                $("#lastqty").val(nilaiubah);
                totalbelanja = totalbelanja + nilaisubtotal;
                $("#totalbelanja").html("Total : " + currSym + " " + number_format(totalbelanja, dDigit, dSep, tSep));
            }else{
                alert('Wajib input angka/bilangan...!!!');
            }
			$("#dialogubahqty").dialog("close");
		}
	});
	$("#newqty2").keypress(function(e) {
		if (e.keyCode == 13){
            var nilaiubah = $("#newqty2").val();
            if (!isNaN(nilaiubah * 1)) {
                var baris_int = oTable.fnGetPosition(barisrubah);
                var idproduknya = barisrubah.getAttribute("id").trim();
                var nilaidata = $("#cekbox_" + idproduknya).val();
                var pecahnilai = nilaidata.split("___");
                totalbelanja = totalbelanja - (pecahnilai[1] * (pecahnilai[2] - (pecahnilai[2] * pecahnilai[3] / 100)));
                oTable.fnUpdate(nilaiubah, baris_int, 5);
                nilaisubtotal = (pecahnilai[2] - (pecahnilai[2] * pecahnilai[3] / 100)) * nilaiubah;
                subtotalbaru = number_format(nilaisubtotal, dDigit, dSep, tSep);
                var namacekbox = "cekbox_" + idproduknya;
                var nilaikirim = idproduknya + "___" + nilaiubah + "___" + pecahnilai[2] + "___" + pecahnilai[3];
                nilaikirim += "___" + pecahnilai[4] + "___" + pecahnilai[5];
                nilaikirim += "___" + pecahnilai[6] + "___" + pecahnilai[7];
                var checkboxnilai = "<input checked=\"checked\" style=\"display: none;\" id=\"" + namacekbox + "\" name=\"" + namacekbox + "\" type=\"checkbox\" value=\"" + nilaikirim + "\" class=\"checkbox-produk\" />";
                oTable.fnUpdate(subtotalbaru + " " + checkboxnilai, baris_int, 6);
                totalbelanja = totalbelanja + nilaisubtotal;
                posisiakhir = totalproduk - 1;
                if (baris_int == posisiakhir) {
                    $("#lastqty").val(nilaiubah);
                }
                $("#totalbelanja").html("Total : " + currSym + " " + number_format(totalbelanja, dDigit, dSep, tSep));
            }else{
                alert('Wajib input angka/bilangan...!!!');
            }
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
            DetailLaundry = new Array;
			for (var i = 0;i < totaldetaildata;i++) {
                var dataDetail = Drupal.settings.data_laundry[i];
                if (dataDetail.barcode != '') {
                    $('#barcode').val(dataDetail.barcode.trim());
                } else {
                    $('#barcode').val(dataDetail.namaproduct);
                }
                DetailLaundry['bar'+ dataDetail.barcode.trim()] = dataDetail.detail_nomer_rak;
                tambahproduk(dataDetail.sisa, 0);
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
				tambahproduk(dataDetail.jumlah,0, dataDetail.qty_pcs);
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
        if (SplitCaraBayar.indexOf('KAD KREDIT') != -1 || SplitCaraBayar.indexOf('KARTU KREDIT') != -1){
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
        if (SplitCaraBayar.indexOf('DEPOSIT') != -1) {
            $("#baris_bayar_deposit").show();
            $("#nilai_bayar_deposit").focus();
        }else{
            $("#baris_bayar_deposit").hide();
		}
        var Carabayar2Checked = $('#use-carabayar2:checkbox:checked').length;
        if (Carabayar2Checked) {
            $('#use-carabayar2').click();
            $('#use-carabayar2').click();
            if (!Carabayar2Checked) {
                $('#carabayar2').attr('disabled', 'disabled');
            } else {
                $('#carabayar2').removeAttr('disabled');
            }
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
        $('#simpan-transaksi').button('enable').focus();
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
    $('#nilai_bayar_deposit').on('keyup', function(e){
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
    $('#use-voucher').click(function(){
        var VoucherChecked = $('#use-voucher:checkbox:checked').length;
        if (!VoucherChecked){
            $('#voucher_value').attr('disabled','disabled');
            $('#voucher_number').attr('disabled','disabled');
        }else{
            $('#voucher_value').removeAttr('disabled');
            $('#voucher_number').removeAttr('disabled');
            $('#voucher_value').select();
        }
    });
    $('#voucher_value').on('keyup', function(e){
    	if (e.keyCode == 13) {
    		$('#voucher_number').select();
        }else{
            totalbelanja = parseFloat(number_format(totalbelanja,dDigit,'.',''));
            var total_ppn_value = totalbelanja * (parseInt($('#ppn_value').val())/100);
            var total_plus_ppn = (totalbelanja * (parseInt($('#ppn_value').val())/100)) + totalbelanja;
            total_plus_ppn = parseFloat(number_format(total_plus_ppn,dDigit,'.',''));
    		if ($(this).val() != '' && parseInt($(this).val()) > 0) {
                TotalAfterVoucher = parseFloat(total_plus_ppn) - parseFloat($(this).val());
            }else{
                TotalAfterVoucher = total_plus_ppn;
			}
            if (dDigC == 1) {
                totalbelanjaround = round5(TotalAfterVoucher);
            }else{
                totalbelanjaround = TotalAfterVoucher;
            }
            $('#total_plus_ppn').val(number_format(TotalAfterVoucher, dDigit, dSep, tSep));
            $('#penggenapan').val(number_format(totalbelanjaround, dDigit, dSep, tSep));
            $('#nilaibayar').val(number_format(totalbelanjaround, dDigit, dSep, ''));
		}
	});
    $('#voucher_number').on('keydown', function(e){
    	if (e.keyCode == 9) {
            e.preventDefault();
    		if ($('#nilaibayar').is(":visible")){
    			$('#nilaibayar').select();
			}else if($('#nilai_bayar_hutang').is(":visible")){
                $('#nilai_bayar_hutang').select();
			}else if($('#nilai_bayar_debit').is(":visible")){
                $('#nilai_bayar_debit').select();
            }else if($('#nilai_bayar_kredit').is(":visible")){
                $('#nilai_bayar_kredit').select();
            }else if($('#nilai_bayar_deposit').is(":visible")){
                $('#nilai_bayar_deposit').select();
            }
        }
    });
    $('#use-carabayar2').click(function(){
        var Carabayar2Checked = $('#use-carabayar2:checkbox:checked').length;
        if (!Carabayar2Checked){
            $('#carabayar2').attr('disabled','disabled');
            if (LastSelectCarabayar2 == 'TUNAI'){
                $("#baris_bayar_tunai").hide();
			}else if (LastSelectCarabayar2 == 'KAD KREDIT' || LastSelectCarabayar2 == 'KARTU KREDIT'){
                $("#field_no_kartu_kredit,#baris_bayar_kredit").hide();
			}else if (LastSelectCarabayar2 == 'DEBIT'){
                $("#field_no_kartu_debit,#baris_bayar_debit").hide();
            }
		}else{
            $('#carabayar2').removeAttr('disabled');
            $('#carabayar2').select();
            $('#carabayar2 option').each(function(){
                if ($(this).attr('value') == $('#carabayar').val()){
                    $(this).attr('disabled','disabled');
                    $(this).removeAttr('selected');
				}else{
                    if ($(this).attr('value') == 'HUTANG' || $(this).attr('value') == 'DEPOSIT'){
                        $(this).attr('disabled','disabled');
					}else{
                        $(this).removeAttr('disabled');
					}
				}
			});
            $("#carabayar2").change();
        }
    });
    $("#carabayar2").change(function(){
        var CaraBayar1 = $("#carabayar").val();
        var CaraBayar2 = $("#carabayar2").val();
        LastSelectCarabayar2 = CaraBayar2;
        if (CaraBayar2.indexOf('DEBIT') != -1){
            $("#field_no_kartu_debit,#baris_bayar_debit").show();
            $("#nomerkartu").focus();
        }else{
        	if (CaraBayar1 != 'DEBIT') {
                $("#field_no_kartu_debit,#baris_bayar_debit").hide();
            }
        }
        if (CaraBayar2.indexOf('KAD KREDIT') != -1 || CaraBayar2.indexOf('KARTU KREDIT') != -1){
            $("#field_no_kartu_kredit,#baris_bayar_kredit").show();
            $("#nomer_kartu_kredit").focus();
        }else{
            if (CaraBayar1 != 'KAD KREDIT' || CaraBayar1 != 'KARTU KREDIT') {
                $("#field_no_kartu_kredit,#baris_bayar_kredit").hide();
            }
        }
        if (CaraBayar2.indexOf('TUNAI') != -1){
            $("#baris_bayar_tunai").show();
            $("#nilaibayar").removeAttr('readonly').removeAttr('disabled');
            $("#nilaibayar").focus();
        }else{
            if (CaraBayar1 != 'TUNAI') {
                $("#baris_bayar_tunai").hide();
            }
        }
    });
    $("#dialogbayar input, #dialogbayar select").on('keypress', function(e){
    	if (e.keyCode == 116){
    		e.preventDefault();
		}
	});
    if (Drupal.settings.upload_data){
        SyncRequest();
    }
})
