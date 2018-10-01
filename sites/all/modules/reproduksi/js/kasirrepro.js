var oTable;
var pathutama = '';
var tglsekarang = '';
var tgltampil = '';
var giCount = 1;
var totalbelanja = 0;
var totalproduk = 0;
var barisrubah;

var currSym = '';
var tSep = '.';
var dSep = ',';

function addCommas(nStr){
    nStr += "";
    x = nStr.split(".");
    x1 = x[0];
    x2 = x.length > 1 ? dSep + x[1] : "";
    var rgx = /(\d+)(\d{3})/;
    while (rgx.test(x1)) {
        x1 = x1.replace(rgx, "$1" + tSep + "$2");
    }
    return x1 + x2;
}

function tampilkantabelkasir(){
    oTable = $('#tabel_kasir').dataTable( {
        'bJQueryUI': true,
        'bPaginate': false,
        'bLengthChange': false,
        'bFilter': true,
        'bInfo': false,
        'sScrollY': '399px',
        'aoColumns': [
            { 'bSortable': false },{ 'bSortable': false },null,null,null,null
        ],
        'bAutoWidth': false
    });
}
function munculkankasir(){
    $('#dialogkasir').dialog('open');
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
    request.idpelanggan = $('#idpelanggan').val();
    alamatcariproduk = pathutama +'reproduksi/cariproduk';
    $.ajax({
        type: 'POST',
        url: alamatcariproduk,
        data: request,
        cache: false,
        success: function(data){
            var pecahdata = new Array();
            pecahdata = data.split(';');
            if (pecahdata[0].trim() != 'error'){
                nilaisubtotal = pecahdata[2] - ((pecahdata[2]*pecahdata[3])/100);
                subtotal = addCommas(nilaisubtotal);
                nilaikirim = pecahdata[0].trim() +'___1___'+ pecahdata[2] +'___'+ pecahdata[3];
                index_cek_box = pecahdata[0].trim();
                namacekbox = 'cekbox_'+ index_cek_box;
                if($('#'+ namacekbox).val()){
                    var nilaicekbox = $('#'+ namacekbox).val();
                    var pecahnilai = nilaicekbox.split('___');
                    var qtybaru = parseInt(pecahnilai[1]) + 1;
                    var subtotallama = pecahnilai[1] * (pecahnilai[2] - (pecahnilai[2]*pecahnilai[3]/100));
                    var subtotal = qtybaru * (pecahnilai[2] - (pecahnilai[2]*pecahnilai[3]/100));
                    totalbelanja = totalbelanja - subtotallama + subtotal;
                    var totalbelanjaView = parseFloat(totalbelanja).toFixed(2);
                    $('#totalbelanja').html('Total : '+ currSym +' '+ addCommas(totalbelanjaView));
                    var nTr = $('#'+ namacekbox).parent().parent().get(0);
                    var posisibaris = oTable.fnGetPosition(nTr);
                    oTable.fnUpdate(qtybaru, posisibaris, 4 );
                    nilaikirim = pecahnilai[0].trim() +'___'+ qtybaru +'___'+ pecahnilai[2] +'___'+ pecahnilai[3];
                    checkboxnilai = '<input checked="checked" style="display: none;" id="'+ namacekbox +'" name="'+ namacekbox +'" type="checkbox" value="'+ nilaikirim +'" />';
                    var subtotalView = parseFloat(subtotal).toFixed(2);
                    oTable.fnUpdate(addCommas(subtotalView) +' '+ checkboxnilai, posisibaris, 5 );
                    posisiakhir = totalproduk-1;
                    if (posisibaris == posisiakhir){
                        $('#lastqty').val(qtybaru);
                    }
                }else{
                    var icondelete = '<img onclick="hapus_produk(\''+ index_cek_box +'\',this.parentNode.parentNode,\''+ pecahdata[0].trim() +'\')" title="Klik untuk menghapus" src="'+ pathutama +'misc/media/images/close.ico" width="24">';
                    var iconubah = '<img onclick="ubah_qty_produk(\''+ index_cek_box +'\',this.parentNode.parentNode,\''+ pecahdata[0].trim() +'\')" title="Klik untuk mengubah qty produk ini" src="'+ pathutama +'misc/media/images/edit.ico" width="24">';

                    $('#lastharga').val(pecahdata[2]);
                    $('#lastdiskon').val(pecahdata[3]);
                    $('#last_id').val(pecahdata[0].trim());
                    $('#lastqty').val('1');
                    checkboxnilai = '<input checked="checked" style="display: none;" id="'+ namacekbox +'" name="'+ namacekbox +'" type="checkbox" value="'+ nilaikirim +'" />';
                    var subtotalView = parseFloat(pecahdata[2]).toFixed(2);
                    var row = '<tr id="'+ index_cek_box +'"><td>'+ icondelete +'</td><td>'+ iconubah +'</td><td>'+ pecahdata[1] +'</td><td class="angka">'+ subtotalView +'</td><td class="angka">1</td><td class="angka">'+ subtotalView +' '+ checkboxnilai +'</td></tr>';
                    $('#tabel_kasir').dataTable().fnAddTr($(row)[0]);
                    giCount++;
                    totalproduk++;
                    totalbelanja = totalbelanja + nilaisubtotal;
                    var totalbelanjaView = parseFloat(totalbelanja).toFixed(2);
                    $('#totalbelanja').html('Total : '+ currSym +' '+ addCommas(totalbelanjaView));
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
function kirim_data(){
    if (totalproduk > 0){
        var sData = $('input', oTable.fnGetNodes()).serialize();
        $('#nilaikirim').val(sData);
        akhiri_belanja();
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
        oTable.fnDeleteRow(totalproduk-1);
        totalproduk--;
        if (totalproduk >= 0){
            totalbelanja = totalbelanja - ($('#lastharga').val()-($('#lastharga').val()*$('#lastdiskon').val()/100))*$('#lastqty').val();
            var totalbelanjaView = parseFloat(totalbelanja).toFixed(2);
            $('#totalbelanja').html('Total  : '+ currSym +' '+ addCommas(totalbelanjaView));
            var nTr = oTable.fnGetNodes(totalproduk-1);
            idproduknya = nTr.getAttribute('id');
            var nilaidataakhir = $('#cekbox_'+ idproduknya).val();
            var pecahnilaiakhir = nilaidataakhir.split('___');
            $('#lastdiskon').val(pecahnilaiakhir[3]);
            $('#lastharga').val(pecahnilaiakhir[2]);
            $('#lastqty').val(pecahnilaiakhir[1]);
            $('#last_id').val(pecahnilaiakhir[0]);
        }else{
            $('#lastdiskon').val('');
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
    var posisibaris = oTable.fnGetPosition(nTr);
    var nilaidata = $('#cekbox_'+ idproduk).val();
    var pecahnilai = nilaidata.split('___');
    totalbelanja = totalbelanja - (pecahnilai[1]*(pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100)));
    var totalbelanjaView = parseFloat(totalbelanja).toFixed(2);
    $('#totalbelanja').html('Total : '+ currSym +' '+ addCommas(totalbelanjaView));
    oTable.fnDeleteRow(posisibaris,focusbarcode);
    totalproduk--;
    if (totalproduk > 0){
        var nTr = oTable.fnGetNodes(totalproduk-1);
        idproduknya = nTr.getAttribute('id');
        var nilaidataakhir = $('#cekbox_'+ idproduknya).val();
        var pecahnilaiakhir = nilaidataakhir.split('___');
        $('#lastdiskon').val(pecahnilaiakhir[3]);
        $('#lastharga').val(pecahnilaiakhir[2]);
        $('#lastqty').val(pecahnilaiakhir[1]);
        $('#last_id').val(pecahnilaiakhir[0]);
    }else{
        $('#lastdiskon').val('');
        $('#lastharga').val('');
        $('#lastqty').val('');
        $('#last_id').val('');
    }
    $('#barcode').focus();
    $('#barcode').select();
}
function akhiri_belanja(){
    var request = new Object();
    request.detail_produk = $('#nilaikirim').val();
    request.idpelanggan = 0;
    request.totalbelanja = totalbelanja;
    request.keterangan = $('#keterangan').val();
    request.tglreproduksi = $('#tglreproduksikirim').val();
    alamat = pathutama + 'reproduksi/simpanreproduksi';
    $.ajax({
        type: 'POST',
        url: alamat,
        data: request,
        cache: false,
        success: function(data){
            if (data != 'error'){
                window.location = pathutama + 'reproduksi/kasir';
            }else{
                alert('Ada masalah dalam penyimpanan data...!!!');
            }
        }
    });
}

function inisialulang(){
    if (totalproduk > 0){
        var tanya = confirm('Merubah pelanggan berarti menghapus pembelian yang terinput, YAKIN..!!');
        if (tanya != 0){
            window.location = pathutama + 'reproduksi/kasir?idpelanggan='+ $('#idpelanggan').val();
        }else{
            $('#barcode').select();
        }
    }else{
        $('#barcode').select();
    }
}

$(document).ready(function(){
    pathutama = Drupal.settings.basePath;
    tglsekarang = Drupal.settings.tglsekarang;
    tgltampil = Drupal.settings.tgltampil;

    currSym = Drupal.settings.currSym;
    tSep = Drupal.settings.tSep;
    dSep = Drupal.settings.dSep;
    
    $('#dialogkasir').dialog({
        modal: true,
        width: 925,
        closeOnEscape: false,
        height: 730,
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
            kirim_data();
        }else if (e.keyCode == 115){
            hapus_latest_produk();
        }
    });
    $('#barcode').autocomplete({
        source: pathutama + 'reproduksi/autocaribarang',
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
    $('#newqty').autotab_filter({
        format: 'numeric',
        nospace: true
    });

    $('#newqty').keypress(function(e) {
        if (e.keyCode == 13){
            var baris_int = totalproduk-1;
            totalbelanja = totalbelanja - ($('#lastharga').val()-($('#lastharga').val()*$('#lastdiskon').val()/100))*$('#lastqty').val();
            var nilaiubah = $('#newqty').val();
            oTable.fnUpdate(nilaiubah, baris_int, 4 );
            nilaisubtotal = ($('#lastharga').val()-($('#lastharga').val()*$('#lastdiskon').val()/100))*nilaiubah;
            var subtotalbaru = parseFloat(nilaisubtotal).toFixed(2);
            subtotalbaru = addCommas(subtotalbaru);
            var namacekbox = 'cekbox_'+ $('#last_id').val().trim();
            var nilaikirim = $('#last_id').val() +'___'+ nilaiubah +'___'+ $('#lastharga').val() +'___'+ $('#lastdiskon').val();
            var checkboxnilai = '<input checked="checked" style="display: none;" id="'+ namacekbox +'" name="'+ namacekbox +'" type="checkbox" value="'+ nilaikirim +'" />';
            oTable.fnUpdate(subtotalbaru +' '+ checkboxnilai, baris_int, 5 );
            $('#lastqty').val(nilaiubah);
            totalbelanja = totalbelanja + nilaisubtotal;
            var totalbelanjaView = parseFloat(totalbelanja).toFixed(2);
            $('#totalbelanja').html('Total : '+ currSym +' '+ addCommas(totalbelanjaView));
            $('#dialogubahqty').dialog('close');
        }
    });
    $('#newqty2').keypress(function(e) {
        if (e.keyCode == 13){
            var baris_int = oTable.fnGetPosition(barisrubah);
            var idproduknya = barisrubah.getAttribute('id').trim();
            var nilaidata = $('#cekbox_'+ idproduknya).val();
            var pecahnilai = nilaidata.split('___');
            totalbelanja = totalbelanja - (pecahnilai[1]*(pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100)));
            var nilaiubah = $('#newqty2').val();
            oTable.fnUpdate(nilaiubah, baris_int, 4 );
            nilaisubtotal = (pecahnilai[2]-(pecahnilai[2]*pecahnilai[3]/100))*nilaiubah;
            var subtotalbaru = parseFloat(nilaisubtotal).toFixed(2);
            subtotalbaru = addCommas(subtotalbaru);
            var namacekbox = 'cekbox_'+ idproduknya;
            var nilaikirim = idproduknya +'___'+ nilaiubah +'___'+ pecahnilai[2] +'___'+ pecahnilai[3];
            var checkboxnilai = '<input checked="checked" style="display: none;" id="'+ namacekbox +'" name="'+ namacekbox +'" type="checkbox" value="'+ nilaikirim +'" />';
            oTable.fnUpdate(subtotalbaru +' '+ checkboxnilai, baris_int, 5 );
            totalbelanja = totalbelanja + nilaisubtotal;
            posisiakhir = totalproduk-1;
            if (baris_int == posisiakhir){
                $('#lastqty').val(nilaiubah);
            }
            var totalbelanjaView = parseFloat(totalbelanja).toFixed(2);
            $('#totalbelanja').html('Total : '+ currSym +' '+ addCommas(totalbelanjaView));
            $('#dialogubahqty2').dialog('close');
        }
    });
    $('#nilaibayar').keyup(function(e){
        kembali = $('#nilaibayar').val()-totalbelanja;
        $('#kembali').val(currSym +' '+ addCommas(kembali));
        if (e.keyCode == 13){
            akhiri_belanja();
        }
    });
    $('#tglreproduksi').datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: 'dd-mm-yy',
        altField: '#tglreproduksikirim',
        altFormat: 'yy-mm-dd',
        onClose: function(dateText, inst) {
            $('#barcode').select();
        }
    });
    $('#info-kasir-waktu').css('background','url('+ Drupal.settings.logo +') 99% 50% no-repeat');
    $('#info-kasir-waktu').css('background-size','75px 75px');
})