/**
 * Created by HPEnvy on 8/24/2018.
 */

var dDigit = 0;
var tSep = ',';
var dSep = '.';
var currSym = '';
function checkproduk() {
    var request = new Object();
    request.barcode = $('#barcode-check').val();
    alamatcariproduk = Drupal.settings.basePath +"penjualan/cariproduk";
    $.ajax({
        type: "POST",
        url: alamatcariproduk,
        data: request,
        cache: false,
        success: function (data) {
            var pecahdata = new Array();
            if (data.trim() != 'error') {
                pecahdata = data.split(";");
                var barcode_number = pecahdata[7];
                JsBarcode("#barcode-place", barcode_number, {
                    format: "ean13",
                    flat: true,
                    width: 7,
                    height: 150,
                    margin: 0,
                    displayValue: false
                });
                $("#product-name").css("font-size","100px");
                $("#product-name").html(pecahdata[1].toUpperCase());
                $("#product-price").html(currSym + ' ' + number_format(pecahdata[2], dDigit, dSep, tSep));
                $('#barcode-check').select();
            }else{
                $("#product-name").css("font-size","55px");
                $("#product-name").html('MAAF, PRODUK TIDAK BERDAFTAR');
                $("#product-price").html('');
                $("#barcode-place").html('');
                $('#barcode-check').select();
            }
        }
    });
}

$(document).ready(function() {
    dDigit = Drupal.settings.dec_digit;
    currSym = Drupal.settings.currSym;
    tSep = Drupal.settings.tSep;
    dSep = Drupal.settings.dSep;
    $('#barcode-check').select();
    $("#barcode-check").keypress(function(e) {
        if (e.keyCode == 13){
            if ($("#barcode-check").val() != ""){
                checkproduk();
            }
        }
    });
})