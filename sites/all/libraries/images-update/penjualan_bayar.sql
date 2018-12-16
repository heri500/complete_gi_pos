CREATE TABLE `penjualan_bayar` ( `idpenjualan` BIGINT NULL , `carabayar` VARCHAR(255) NOT NULL DEFAULT 'TUNAI' ,
  `total_bayar` DOUBLE NOT NULL DEFAULT '0' ) ENGINE = InnoDB;
ALTER TABLE `penjualan_bayar` ADD PRIMARY KEY( `idpenjualan`, `carabayar`);