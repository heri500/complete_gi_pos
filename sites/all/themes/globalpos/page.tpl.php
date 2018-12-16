<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<html>
	<head>
		<title><?php print $head_title ?></title>
		<?php print $head; ?>
		<?php print $styles ?>
		<script type="text/javascript" src="<?php print base_path().drupal_get_path('theme','globalpos'); ?>/js/jquery-1.5.2.min.js"></script>
		<?php print $scripts ?>
		<script type="text/javascript" src="<?php print base_path().drupal_get_path('theme','globalpos'); ?>/js/hoverIntent.js"></script>
		<script type="text/javascript" src="<?php print base_path().drupal_get_path('theme','globalpos'); ?>/js/superfish.js"></script>
		<script type="text/javascript">

		// initialise plugins
		jQuery(function(){
			jQuery('ul.sf-menu').superfish();
		});

		</script>
	</head>
	<body>
		<div id="container">
			<div id="header">
				<?php
          // Prepare header
          $site_fields = array();
          if ($site_name) {
            $site_fields[] = check_plain($site_name);
          }
          if ($site_slogan) {
            $site_fields[] = check_plain($site_slogan);
          }
          $site_title = implode(' ', $site_fields);
          $site_html = implode(' ', $site_fields);

          if ($logo || $site_title) {
            print '<h1><a href="'. check_url($front_page) .'" title="'. $site_title .'">';
            print $site_html .'</a></h1>';
          }
        ?>
        <div id="menuutama">
        <ul class="sf-menu">
					<li class="current">
						<a href="#a">Data Produk</a>
						<ul>
							<li>
								<a href="<?php print base_path(); ?>dataproduk/produk">Tabel Produk</a>
							</li>
							<li class="current">
								<a href="<?php print base_path(); ?>dataproduk/kategori">Kategori</a>
							</li>
							<li>
								<a href="<?php print base_path(); ?>dataproduk/subkategori">Sub Kategori</a>
							</li>
						</ul>
					</li>
					<li>
						<a href="<?php print base_path(); ?>datasupplier/supplier">Data Supplier</a>
					</li>
					<li>
						<a href="<?php print base_path(); ?>datapelanggan/pelanggan">Data Pelanggan</a>
					</li>
					<li>
						<a href="#">Transaksi</a>
						<ul>
							<li>
								<a href="<?php print base_path(); ?>penjualan/viewpenjualan">Penjualan</a>
							</li>
							<li class="current">
								<a href="<?php print base_path(); ?>pembelian/viewpembelian">Pembelian</a>
								<ul>
									<li>
										<a href="<?php print base_path(); ?>pembelian/viewpembelian">Tabel Pembelian</a>
									</li>
									<li class="current">
										<a href="<?php print base_path(); ?>pembelian/kasir">Form Pembelian Produk</a>
									</li>
								</ul>
							</li>
							<li>
								<a href="#">Packing Ulang</a>
								<ul>
									<li>
										<a href="<?php print base_path(); ?>reproduksi/viewreproduksi">Tabel Packing Ulang</a>
									</li>
									<li class="current">
										<a href="<?php print base_path(); ?>reproduksi/kasir">Form Packing Ulang</a>
									</li>
								</ul>
							</li>
						</ul>
					</li>
					<li>
						<a href="<?php print base_path(); ?>penjualan/kasir">Kasir</a>
					</li>
					<?php
					global $user;
					if ($user->uid == 1){
					?>
					<li>
						<a href="<?php print base_path(); ?>admin">Administrator</a>
					</li>
					<?php
					}
					?>
					<li>
						<a href="<?php print base_path(); ?>logout">Keluar</a>
					</li>	
				</ul>
				</div>
			</div>
			<div id="mainblock" <?php print $classnya; ?> >
				<div id="main">
					<?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
          <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
          <?php if ($tabs): print '<ul class="tabs primary">'. $tabs .'</ul></div>'; endif; ?>
          <?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
          <?php if ($show_messages && $messages): print $messages; endif; ?>
          <?php print $help; ?>
					<?php print $content ?>
				</div>
			</div>
			<div id="footer">
				<?php print $footer_message ?>
				<?php print $footer ?>
			</div>
		</div>
	<?php print $closure ?>
	</body>
</html>