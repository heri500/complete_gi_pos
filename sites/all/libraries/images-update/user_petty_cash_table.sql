CREATE TABLE `pisb`.`user_petty_cash` ( `uid` INT NOT NULL , `login` INT NULL DEFAULT NULL , `petty_cash` DOUBLE NOT NULL ) ENGINE = InnoDB;
ALTER TABLE `user_petty_cash` ADD `id` BIGINT NOT NULL AUTO_INCREMENT FIRST, ADD PRIMARY KEY (`id`);