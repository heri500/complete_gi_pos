$(document).ready(function(){
    for (var i = 0; i < Drupal.settings.barcodeData.length;i++) {
        var barcode_number = Drupal.settings.barcodeData[i];
        JsBarcode("#barcode-" + barcode_number, barcode_number, {
            format: "ean13",
            flat: true,
            width: 1.7,
            height: 25,
            margin: 0,
            displayValue: false
        });
    }
})
