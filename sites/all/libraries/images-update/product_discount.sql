CREATE TABLE `product_discount` ( `id` BIGINT NOT NULL AUTO_INCREMENT ,  `idproduct` BIGINT NULL ,  `date_from` INT NULL ,  `date_thru` INT NULL ,  `price` DOUBLE NULL ,    PRIMARY KEY  (`id`)) ENGINE = MyISAM;
ALTER TABLE `product_discount` ADD `normal_price` DOUBLE NULL DEFAULT NULL AFTER `price`;
alter table product_discount modify date_from date null;
alter table product_discount modify date_thru date null;
alter table product_discount add active tinyint default 0 null;
alter table historyhargajual add idproduct bigint not null;
alter table historyhargabeli add idproduct bigint not null;