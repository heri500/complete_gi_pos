$(document).ready(function(){
    var renderer = "bmp";
    var btype = "ean13";
    var settings = {
        output:renderer,
        barWidth: 1,
        barHeight: 30
    };
    $(".barcode-place").each(function(){
        barcode_value = $(this).attr('id');
        $(this).barcode(barcode_value, btype, settings);
        $(this).css('width','186px');
    });
})
