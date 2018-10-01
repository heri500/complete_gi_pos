/*$(document).ready(function(){
    var renderer = "bmp";
    var btype = "ean13";
    var settings = {
        output:renderer,
        barWidth: 2,
        barHeight: 30
    };
    $(".barcode-place").each(function(){
        barcode_value = $(this).attr('id');
        $(this).barcode(barcode_value, btype, settings);
    });
})*/
$(document).ready(function(){
    var TotalInRow = Drupal.settings.TotalInRow;
    var BarWidth = 1.7;
    if (TotalInRow > 1){
        BarWidth = 1.1;
    }
    for (var i = 0; i < Drupal.settings.dataProduct.length;i++) {
        var barcode_number = Drupal.settings.dataProduct[i].barcode;
        var barcode_id = Drupal.settings.dataProduct[i].id;
        var barcode_type = Drupal.settings.dataProduct[i].barcode_type;
        if (TotalInRow <= 1) {
            if (barcode_type == 'ean13') {
                BarWidth = 1.7;
            } else if (barcode_type == 'code39') {
                BarWidth = 1.45;
            }
        }else{
            if (barcode_type == 'ean13') {
                BarWidth = 1.1;
            } else if (barcode_type == 'code39') {
                BarWidth = 1;
            }
        }
        JsBarcode("#" + barcode_id, barcode_number, {
            format: barcode_type,
            flat: true,
            width: BarWidth,
            height: 35,
            margin: 0,
            displayValue: false
        });
    }
})