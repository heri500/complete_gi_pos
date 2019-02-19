ALTER TABLE `detailpembelian` ADD `idsupplier` INT NULL AFTER `idproduct`;
ALTER TABLE `detailpenjualan` ADD `idsupplier` INT NULL AFTER `idproduct`;
ALTER TABLE `supplier` ADD `payment_type` TINYINT NOT NULL DEFAULT '0' AFTER `status_supplier`;
ALTER TABLE `supplier` ADD `payment_term` INT NOT NULL DEFAULT '0' AFTER `payment_type`;