$(document).ready(function(){
    pathutama = Drupal.settings.basePath;
    jsPrintSetup.setPrinter(Drupal.settings.printer_name);
	jsPrintSetup.setOption('orientation', jsPrintSetup.kPortraitOrientation);
	jsPrintSetup.setOption('marginTop', 0);
	jsPrintSetup.setOption('marginBottom', 0);
	jsPrintSetup.setOption('marginLeft', 0);
	jsPrintSetup.setOption('marginRight', 0);
	// Suppress print dialog
	jsPrintSetup.setSilentPrint(true);/** Set silent printing */
	//Select Printer
	// Do Print
	jsPrintSetup.print();
    var request = new Object();
    request.idorder = Drupal.settings.id_order;
    alamat = pathutama + "penjualan/getcoarrayprinter";
    $.ajax({
        type: "POST",
        url: alamat,
        data: request,
        cache: false,
        success: function (data) {
            var ArrayPrinter = eval(data);
            var print_idx = parseInt(Drupal.settings.print_idx) + 1;
            if (print_idx < ArrayPrinter.length){
                window.open(pathutama + 'print/6?idghordermultiprint='+ Drupal.settings.id_order +'&printername='+ ArrayPrinter[print_idx].trim() +'&print_idx='+ print_idx);
                close();
			}else {
                alamat = pathutama + "penjualan/getnewandroidorderpending";
                $.ajax({
                    type: "POST",
                    url: alamat,
                    cache: false,
                    success: function (data) {
                        close();
                    }
                });
            }
        }
    });
	// Restore print dialog
	//jsPrintSetup.setSilentPrint(false);
});