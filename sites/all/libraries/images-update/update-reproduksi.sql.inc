ALTER TABLE `hasilreproduksi` ADD `ref_idreproduksi` INT NULL AFTER `keterangan`;
ALTER TABLE `detailhasilreproduksi` ADD `idhasilreproduksi` INT NOT NULL FIRST;