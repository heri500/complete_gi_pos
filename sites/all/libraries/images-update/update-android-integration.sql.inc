ALTER TABLE `customer_order`  ADD `android_order` TINYINT NOT NULL DEFAULT '0'  AFTER `idmeja`,  ADD `printed` TINYINT NOT NULL DEFAULT '0'  AFTER `android_order`;
ALTER TABLE `product` ADD `id_grup_menu` INT NULL AFTER `idsupplier`;

--
-- Table structure for table `grup_menu`
--

CREATE TABLE `grup_menu` (
`id` int(11) NOT NULL,
`kode_grup` varchar(10) NOT NULL,
`nama_grup` varchar(255) NOT NULL,
`keterangan` text,
`weight` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `grup_menu`
--
ALTER TABLE `grup_menu`
ADD PRIMARY KEY (`id`),
ADD KEY `kode_grup` (`kode_grup`),
ADD KEY `nama_grup` (`nama_grup`),
ADD KEY `weight` (`weight`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `grup_menu`
--
ALTER TABLE `grup_menu`
MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;