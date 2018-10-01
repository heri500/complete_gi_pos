var oTable;
var pathutama = '';
var urutan = '';
var alamatupdatetanggalreturn = '';
var urutanSort = new Array;
function addCommas(nStr){
    nStr += "";
    x = nStr.split(",");
    x1 = x[0];
    x2 = x.length > 1 ? "," + x[1] : "";
    var rgx = /(\d+)(\d{3})/;
    while (rgx.test(x1)) {
        x1 = x1.replace(rgx, "$1" + "." + "$2");
    }
    return x1 + x2;
}
function tampiltabelreturn(){
    oTable = $("#tabel_returnpelanggan").dataTable( {
        "bJQueryUI": true,
        "bAutoWidth": false,
        "sPaginationType": "full_numbers",
        "bInfo": false,
        "aLengthMenu": [[100, 200, 300, -1], [100, 200, 300, "All"]],
        "iDisplayLength": 100,
        "aaSorting": [[ urutan , "desc"]],
        "sDom": '<"space"T><C><"clear"><"H"lfr>t<"F"ip>',
        "oColVis": {
            "activate": "mouseover",
            'aiExclude': [ 0,1 ]
        },
        "aoColumnDefs": [
            { "bSortable": false, "aTargets": urutanSort }
        ]
    });
}
function tampiltabelreturndetail(){
    oTable = $("#tabel_detail_returnpelanggan").dataTable( {
        "bJQueryUI": true,
        "bAutoWidth": false,
        "bPaginate": false,
        "bLengthChange": false,
        "bInfo": false,
        "aaSorting": [[0, "asc"]],
        "sDom": '<"H"<"toolbar">fr>t<"F"ip>'
    });
}
function view_detail(idreturnpelanggan,nonota){
    var request = new Object();
    request.idreturn = idreturnpelanggan;
    alamat = pathutama + "penjualan/detailreturn";
    $.ajax({
        type: "POST",
        url: alamat,
        data: request,
        cache: false,
        success: function(data){
            $("#dialogdetail").html(data);
            tampiltabelreturndetail();
            $("div.toolbar").html("No. Nota : "+ nonota);
            $("#dialogdetail").dialog("open");
        }
    });
}
function delete_returnpelanggan(idreturnpelanggan,nonota){
    var konfirmasi = confirm('Yakin ingin menghapus return pelanggan dengan no nota : '+ nonota +' ini...??!!');
    if (konfirmasi){
        window.location = pathutama + 'pembelian/deletereturnpelanggan/'+ idreturnpelanggan +'?destination=pembelian/viewreturnpelanggan';
    }
}
$(document).ready(function(){
    pathutama = Drupal.settings.basePath;
    alamatupdatetanggalreturn = pathutama + 'penjualan/updatereturnpelanggan';
    urutan = Drupal.settings.urutan;
    if (urutan == 2){
        urutanSort = [ 0,1,3 ];
    }else{
        urutanSort = [ ];
    }
    $("#dialogdetail").dialog({
        modal: true,
        width: 850,
        resizable: false,
        autoOpen: false,
        position: ["auto","auto"]
    });
    $("button").button();
    TableToolsInit.sSwfPath = pathutama +"misc/media/datatables/swf/ZeroClipboard.swf";
    if (urutan == 1){
        $('.edit-tanggal').editable(alamatupdatetanggalreturn,{
            submitdata : function(value, settings) {
                var idreturnpelanggan = $(this).attr('id');
                var splitidreturnpelanggan = idreturnpelanggan.split('-');
                idreturnpelanggan = splitidreturnpelanggan[1];
                var jamreturnpelangganupdate = $('#jamreturnpelanggan-'+ idreturnpelanggan).html();
                return {jamreturnpelanggan: jamreturnpelangganupdate,ubah: 'tanggal'};
            },
            name : 'tanggalreturn',
            width : 130,
            height : 18,
            style   : 'margin: 0',
            tooltip   : 'Klik untuk mengubah tanggal return pelanggan',
            indicator : 'Saving...',
            type: "datepicker",
            datepicker: {
                changeMonth: true,
                changeYear: true,
                dateFormat: "dd-mm-yy"
            },
            callback : function(value, settings) {
                var split_tanggal = value.split('-');
                var tanggalreturn = new Date();
                var bulan = parseInt(split_tanggal[1]) - 1;
                tanggalreturn.setFullYear(split_tanggal[2],bulan,split_tanggal[0]);
                var indexhari = tanggalreturn.getDay();
                var hari = Drupal.settings.namahari[indexhari];
                var idreturnpelanggan = $(this).attr('id');
                var splitidreturnpelanggan = idreturnpelanggan.split('-');
                idreturnpelanggan = splitidreturnpelanggan[1];
                $('#harireturnpelanggan-'+ idreturnpelanggan).html(hari);
            }
        });
        $('.edit-jam').editable(alamatupdatetanggalreturn,{
            name : 'jamreturnpelanggan',
            width : 120,
            height : 18,
            style   : 'margin: 0;',
            type: "timepicker",
            submitdata : function(value, settings) {
                var idreturnpelanggan = $(this).attr('id');
                var splitidreturnpelanggan = idreturnpelanggan.split('-');
                idreturnpelanggan = splitidreturnpelanggan[1];
                var tglreturnpelangganupdate = $('#tglreturnpelanggan-'+ idreturnpelanggan).html();
                return {tanggalreturn: tglreturnpelangganupdate,ubah: 'jam'};
            },
            timepicker: {
                timeOnlyTitle: "PILIH WAKTU",
                timeText: "Waktu",
                hourText: "Jam",
                minuteText: "Menit",
                secondText: "Detik",
                currentText: "Saat Ini",
                showButtonPanel: false
            },
            submit		: "Ok",
            tooltip   : 'Klik untuk mengubah jam return pelanggan',
            indicator : 'Saving...'
        });
    }
    tampiltabelreturn();
    $("#tgl1").datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: "dd-mm-yy"
    });
    $("#tgl2").datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: "dd-mm-yy"
    });
})