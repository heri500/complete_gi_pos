<?php
$schema['datapremis'] = array(
    'description' => 'Data premis.',
    'fields' => array(
        'id' => array(
            'type' => 'varchar',
            'length' => 13,
            'not null' => TRUE,
            'default' => '8990010000000',
            'description' => t('id premis berupa barcode.'),
        ),
        'zone' => array(
            'type' => 'varchar',
            'length' => 3,
            'not null' => TRUE,
            'default' => '000',
            'description' => t('kode zone.'),
        ),
        'nama' => array(
            'type' => 'varchar',
            'length' => 500,
            'not null' => TRUE,
            'default' => 'Ikhwan Mart',
            'description' => t('Nama premis.'),
        ),
        'alamat' => array(
            'type' => 'varchar',
            'length' => 1000,
            'not null' => TRUE,
            'default' => 'Alamat Premis',
            'description' => t('Alamat premis.'),
        ),
        'telepon' => array(
            'type' => 'varchar',
            'length' => 120,
            'not null' => TRUE,
            'default' => '021-00000000',
            'description' => t('Telepon premis.'),
        ),
        'whatsapp' => array(
            'type' => 'varchar',
            'length' => 120,
            'default' => '021-00000000',
            'description' => t('Nomer Whatsapp premis.'),
        ),
        'bbm' => array(
            'type' => 'varchar',
            'length' => 120,
            'default' => 'ABCDEF',
            'description' => t('Pin bbm premis.'),
        ),
        'telegram' => array(
            'type' => 'varchar',
            'length' => 120,
            'default' => '021-00000000',
            'description' => t('Nomer Telegram premis.'),
        ),
        'email' => array(
            'type' => 'varchar',
            'length' => 255,
            'default' => 'ikhwanmart@ikhwanit.com',
            'description' => t('Email premis.'),
        ),
        'website' => array(
            'type' => 'varchar',
            'length' => 255,
            'default' => 'ikhwanmart.ikhwanit.com',
            'description' => t('Website premis.'),
        ),
        'created' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'default' => 0,
            'description' => t('Waktu record dibuat.'),
        ),
        'changed' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'default' => 0,
            'description' => t('Waktu record diubah/edit.'),
        ),
        'uid' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'default' => 0,
            'description' => t('User ID yang membuat record.'),
        ),
    ),
    'primary key' => array('id'),
    'indexes' => array(
        'idx_zone' => array('zone'),
        'idx_nama' => array('nama'),
    ),
);

