<?php
/**
 * @file
 * taxonomy_vocabularies.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function taxonomy_vocabularies_taxonomy_default_vocabularies() {
  return array(
    'event_types' => array(
      'name' => 'Event Types',
      'machine_name' => 'event_types',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'genres' => array(
      'name' => 'Genres',
      'machine_name' => 'genres',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'organization_types' => array(
      'name' => 'Organization Types',
      'machine_name' => 'organization_types',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'profile_types' => array(
      'name' => 'Profile Types',
      'machine_name' => 'profile_types',
      'description' => 'types of person profiles',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'program_types' => array(
      'name' => 'Program Types',
      'machine_name' => 'program_types',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'station_content_types' => array(
      'name' => 'Station Content Types',
      'machine_name' => 'station_content_types',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'topics' => array(
      'name' => 'Topics',
      'machine_name' => 'topics',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
