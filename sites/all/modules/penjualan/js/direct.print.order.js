$(document).ready(function() {
    pathutama = Drupal.settings.basePath;
    jsPrintSetup.setPrinter(Drupal.settings.printer_name);
    jsPrintSetup.setOption('orientation', jsPrintSetup.kPortraitOrientation);
    jsPrintSetup.setOption('marginTop', 0);
    jsPrintSetup.setOption('marginBottom', 0);
    jsPrintSetup.setOption('marginLeft', 0);
    jsPrintSetup.setOption('marginRight', 0);
    // Suppress print dialog
    jsPrintSetup.setSilentPrint(true);
    /** Set silent printing */
    //Select Printer
    // Do Print
    jsPrintSetup.print();
});
