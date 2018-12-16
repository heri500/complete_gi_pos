var oTable;
var oTable2;
var oTable3;
var pathutama = '';
var urutan = 0;
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

function tampiltabeljual(){
    oTable = $("#tabel_reproduksi").dataTable( {
        "bJQueryUI": true,
        "bAutoWidth": false,
        "sPaginationType": "full_numbers",
        "bInfo": false,
        "aLengthMenu": [[100, 200, 300, -1], [100, 200, 300, "All"]],
        "iDisplayLength": 100,
        "aaSorting": [[urutan, "desc"]],
        "sDom": '<"space"T><C><"clear"><"H"lfr>t<"F"ip>',
    "oColVis": {
        "activate": "mouseover"
    }
});
}
function tampiltabeldetailrepro(){
    oTable2 = $("#tabel_detail_reproduksi").dataTable( {
        "bJQueryUI": true,
        "bAutoWidth": false,
        "bPaginate": false,
        "bLengthChange": false,
        "bInfo": false,
        "aaSorting": [[0, "asc"]],
        "sDom": '<"H"<"toolbar">fr>t<"F"ip>'
});
}
function tampiltabeldetailhasilrepro(){
    oTable3 = $("#tabel_detail_hasilreproduksi").dataTable( {
        "bJQueryUI": true,
        "bAutoWidth": false,
        "bPaginate": false,
        "bLengthChange": false,
        "bInfo": false,
        "aaSorting": [[0, "asc"]],
        "sDom": '<"H"<"toolbar">fr>t<"F"ip>'
});
}
function view_detail(idreproduksi,nonota){
    var request = new Object();
    request.idreproduksi = idreproduksi;
    alamat = pathutama + "reproduksi/detailreproduksi";
    $.ajax({
        type: "POST",
        url: alamat,
        data: request,
        cache: false,
        success: function(data){
            $("#divdetailrepro").html(data);
            tampiltabeldetailrepro();
            $("div.toolbar").html("No. Nota : "+ nonota);
            var request2 = new Object();
            request2.idreproduksi = idreproduksi;
            alamat = pathutama + "hasilreproduksi/detailhasilreproduksi";
            $.ajax({
                type: "POST",
                url: alamat,
                data: request,
                cache: false,
                success: function(data){
                    $("#divdetailhasilrepro").html(data);
                    tampiltabeldetailhasilrepro();
                    $("#dialogdetail").dialog("open");
                }
            });
        }
    });
}
function add_hasil_pack(idreproduksi, nonota){
    if (idreproduksi > 0){
        window.location = pathutama + "hasilreproduksi/kasir?idreproduksi="+ idreproduksi +"&nonota="+ nonota;
    }
}
$(document).ready(function(){
    pathutama = Drupal.settings.basePath;
    urutan = Drupal.settings.urutan;

    currSym = Drupal.settings.currSym;
    tSep = Drupal.settings.tSep;
    dSep = Drupal.settings.dSep;

    tampilData = Drupal.settings.tampilData;
    tglAwal = Drupal.settings.tglAwal;
    tglAkhir = Drupal.settings.tglAkhir;

    $("#dialogdetail").dialog({
        modal: true,
        width: 850,
        resizable: false,
        autoOpen: false,
        position: ["auto","auto"]
    });
    $("button").button();
    TableToolsInit.sSwfPath = pathutama +"misc/media/datatables/swf/ZeroClipboard.swf";
    tampiltabeljual();
    $("#tgl1").datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: "yy-mm-dd"
    });
    $("#tgl2").datepicker({
        changeMonth: true,
        changeYear: true,
        dateFormat: "yy-mm-dd"
    });
})