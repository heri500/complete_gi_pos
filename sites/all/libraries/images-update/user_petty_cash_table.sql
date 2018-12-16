CREATE TABLE `user_petty_cash` ( `uid` INT NOT NULL , `login` INT NULL DEFAULT NULL , `petty_cash` DOUBLE NOT NULL ) ENGINE = InnoDB;
ALTER TABLE `user_petty_cash` ADD `id` BIGINT NOT NULL AUTO_INCREMENT FIRST, ADD PRIMARY KEY (`id`);

CREATE TABLE `user_petty_cash_ambil` ( `id_petty_cash` BIGINT NOT NULL , `jumlah_ambil` DOUBLE NULL , `jam_ambil` INT NULL , `petugas` VARCHAR(255) NULL ) ENGINE = MyISAM;