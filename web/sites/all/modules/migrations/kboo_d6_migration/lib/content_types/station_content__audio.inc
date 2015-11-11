<?php


/**
 * Migrates D7 station_content from D6 audio.
 */
class StationContentFromAudio extends NodeMigration {

  /**
   * Constructor
   *
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    $this->addDependencies([
      'Program_from_KbooProgram',
    ]);

    $this->addReferenceMapping(
      'field_produced_for',
      'field_program',
      'Program_from_KbooProgram'
    );

    $this->addDateMapping(
      'field_air_time',
      'field_air_date'
    );

    $this->addFieldMapping(
      'field_short_description',
      'field_short_description'
    );

    $this->addTaxonomyMapping(
      'field_topic_tags',
      5,
      'Taxonomy__topics_from_5'
    );

    $this->addTaxonomyMapping(
      'field_genres',
      2,
      'Taxonomy__genres_from_2'
    );

    parent::__construct($arguments);
  }
}