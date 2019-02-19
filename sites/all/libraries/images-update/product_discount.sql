CREATE TABLE `product_discount` ( `id` BIGINT NOT NULL AUTO_INCREMENT ,  `idproduct` BIGINT NULL ,  `date_from` INT NULL ,  `date_thru` INT NULL ,  `price` DOUBLE NULL ,    PRIMARY KEY  (`id`)) ENGINE = MyISAM;
ALTER TABLE `product_discount` ADD `normal_price` DOUBLE NULL DEFAULT NULL AFTER `price`;
