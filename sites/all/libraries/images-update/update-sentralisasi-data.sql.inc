ALTER TABLE `penjualan` ADD `uploaded` TINYINT NOT NULL DEFAULT '0' AFTER `insert_date`, ADD `changed` TINYINT NOT NULL DEFAULT '0' AFTER `uploaded`;
ALTER TABLE `product` ADD `uploaded` TINYINT NOT NULL DEFAULT '0' AFTER `status_product`, ADD `changed` TINYINT NOT NULL DEFAULT '0' AFTER `uploaded`;
ALTER TABLE `penjualan` ADD `ppn` DOUBLE NOT NULL AFTER `total`;
ALTER TABLE `penjualan` ADD `total_plus_ppn` DOUBLE NOT NULL AFTER `ppn`;
UPDATE penjualan SET total_plus_ppn = total + (total * (ppn/100));
ALTER TABLE `penjualan` CHANGE `totalmodal` `totalmodal` DOUBLE NOT NULL DEFAULT '0';
ALTER TABLE `transaksistock` CHANGE `masuk` `masuk` DOUBLE NOT NULL DEFAULT '0',CHANGE `keluar` `keluar` DOUBLE NOT NULL DEFAULT '0';
ALTER TABLE `penjualan` CHANGE `nokartu` `nokartu` VARCHAR(100) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL;
ALTER TABLE `penjualan` CHANGE `keterangan` `keterangan` TEXT CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL;
