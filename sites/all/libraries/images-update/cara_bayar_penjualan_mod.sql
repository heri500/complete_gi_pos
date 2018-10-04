ALTER TABLE `penjualan` CHANGE `carabayar` `carabayar` VARCHAR(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT 'TUNAI';
ALTER TABLE `penjualan` ADD `bayar_multiple` VARCHAR(255) NULL DEFAULT NULL AFTER `bayar`;
