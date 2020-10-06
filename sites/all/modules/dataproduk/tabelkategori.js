var oTable;
var pathutama = "";
var pathfile = "";
function tampilkantabelkategori() {
    oTable = $("#tabel_kategori").dataTable({
        "bJQueryUI": true,
        "bAutoWidth": false,
        "sPaginationType": "full_numbers",
        "bInfo": false,
        "aLengthMenu": [[100, 200, 300, -1], [100, 200, 300, "All"]],
        "iDisplayLength": 100,
        "aaSorting": [[0, "asc"]],
        "sDom": '<"space"T><"clear"><"H"lfr>t<"F"ip>'
    });
}
$(document).ready(function() {
    pathutama = Drupal.settings.basePath;
    pathfile = Drupal.settings.filePath;
    TableToolsInit.sSwfPath = pathutama +"misc/media/datatables/swf/ZeroClipboard.swf";
    alamatupdate = pathutama + "dataproduk/updatekategori";
    var PrinterList = jsPrintSetup.getPrintersList();
    var ArrayPrinter = PrinterList.split(',');
    var OptionPrinter = {};
    for (var i = 0;i < ArrayPrinter.length;i++){
        $('#selected-printer').append('<option value="'+ ArrayPrinter[i] +'">'+ ArrayPrinter[i] +'</option>');
        OptionPrinter[ArrayPrinter[i]] = ArrayPrinter[i];
    }
    $("#tabel_kategori tbody .editable").editable(alamatupdate, {
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
    $("#tabel_kategori tbody .editable-printer").editable(alamatupdate, {
        "callback": function( sValue, y ) {
            var aPos = oTable.fnGetPosition( this );
            oTable.fnUpdate( sValue, aPos[0], aPos[1] );
        },
        "submitdata": function ( value, settings ) {
            var aPos = oTable.fnGetPosition( this );
            return { "row_id": this.parentNode.getAttribute("id"), "kol_id": aPos[1] };
        },
        "data"  : JSON.stringify(OptionPrinter),
        "type"  : "select",
        "height": "20px",
        "submit": "Ok",
        "cancel": "Batal",
        "indicator": "Menyimpan...",
        "tooltip": "Klik untuk mengubah..."
    });
    tampilkantabelkategori();
    $("#formkategori").validationEngine();
    $("button").button();
    $("#kodekategori").autotab_filter({
        format: "alphanumeric",
        uppercase: true,
        nospace: true
    });

})