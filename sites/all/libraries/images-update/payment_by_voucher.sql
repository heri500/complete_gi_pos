ALTER TABLE `penjualan` ADD `android_order` TINYINT NOT NULL DEFAULT '0' AFTER `idmeja`;
ALTER TABLE `penjualan` ADD `voucher_value` DOUBLE NULL AFTER `android_order`, ADD `voucher_number` VARCHAR(255) NULL AFTER `voucher_value`, ADD `total_after_voucher` DOUBLE NULL AFTER `voucher_number`;
ALTER TABLE `penjualan` ADD `total_round` DOUBLE NULL DEFAULT NULL AFTER `total_after_voucher`;
UPDATE penjualan SET total_round = total_plus_ppn WHERE total_round IS NULL;