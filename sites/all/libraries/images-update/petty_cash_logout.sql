ALTER TABLE `user_petty_cash` ADD `log_out` INT NULL AFTER `petty_cash`;
ALTER TABLE `user_petty_cash` ADD `ip` VARCHAR(100) NULL AFTER `log_out`;