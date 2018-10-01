ALTER TABLE `customer_order` ADD `pending_print` TINYINT NOT NULL DEFAULT '0' AFTER `total_plus_ppn`;
ALTER TABLE `kategori` ADD `selected_printer` VARCHAR(255) NULL AFTER `keterangan`;
