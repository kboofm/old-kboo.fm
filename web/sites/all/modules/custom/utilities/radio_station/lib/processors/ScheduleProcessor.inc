<?php

class ScheduleProcessor extends TemplateProcessor {

  public function process($dataset, $timestamp, $when, $count = 1) {
    $schedule = [];

    foreach ($dataset as $nid => $item) {
      $wrapped_node = entity_metadata_wrapper('node', $nid);
      $timeslots = $wrapped_node->field_timeslot->value();

      $timeslots = array_filter(
        $timeslots,
        function ($timeslot) use ($when, $timestamp) {
          if ($when == 'after' && $timeslot['value'] <= $timestamp) {
            return false;
          } else if ($when == 'before' && $timeslot['value'] >= $timestamp) {
            return false;
          }

          return true;
        }
      );

      $direction = ($when == 'after') ? 'ASC' : 'DESC';
      $this->sortByStartTime($timeslots, $direction);

      if ($count) {
        $timeslots = array_slice($timeslots, 0, $count);
      }

      $new_schedule = array_map(
        function ($timeslot) use ($wrapped_node) {
          return $this->buildScheduleItem($wrapped_node, $timeslot);
        },
        $timeslots
      );

      $new_schedule = array_filter($new_schedule);

      $schedule = array_merge(
        $schedule,
        $new_schedule
      );
    }

    return $schedule;
  }


  public function processRange($dataset, $start, $end, $groupByDay = false) {
    $schedule = [];

    foreach ($dataset as $nid => $item) {
      $wrapped = entity_metadata_wrapper('node', $nid);
      $timeslots = $wrapped->field_timeslot->value();

      $timeslots = array_filter(
        $timeslots,
        function ($timeslot) use ($start, $end) {
          if ($timeslot['value'] < $start) {
            return false;
          } else if ($timeslot['value'] > $end) {
            return false;
          }

          return true;
        }
      );

      foreach ($timeslots as $timeslot) {
        $schedule_item = $this->buildScheduleItem($wrapped, $timeslot);

        if (!$schedule_item) {
          continue;
        }

        if ($groupByDay) {
          $day_of_week = Helpers::dayOfWeek(
            $timeslot["value"]
          );

          if (!isset($schedule[$day_of_week])) {
            $schedule[$day_of_week] = [];
          }

          $schedule[$day_of_week][] = $schedule_item;
        } else {
          $schedule[] = $schedule_item;
        }
      }
    }

    if ($groupByDay) {
      foreach ($schedule as &$day_of_week) {
        $this->sortByStartTime($day_of_week);
      }
    } else {
      $this->sortByStartTime($schedule);
    }

    return $schedule;
  }


  private function sortByStartTime(&$items, $direction = 'ASC') {
    usort(
      $items,
      function ($a, $b) use ($direction) {
        /*
         * $a and $b can be either a $timelot or a $schedule_item.
         *
         * A $timelot will be defined as:
         *   $a['value']
         *
         * A $schedule_item will be defined as:
         *   $a['start']['timestamp']
         *
         * Yes, this could be done cleaner, but time is not currently allotted.
         */
        if (isset($a['value'])) {
          $left = $a['value'];
          $right = $b['value'];
        } else {
          $left = $a['start']['timestamp'];
          $right = $b['start']['timestamp'];
        }

        if ($direction == 'ASC') {
          return ($left < $right) ? -1 : 1;
        } else {
          return ($left > $right) ? -1 : 1;
        }
      }
    );
  }


  private function getTimeslotProgram($wrapped_scheduled_timeslot) {
    try {
      return $wrapped_scheduled_timeslot->field_show->value();
    } catch (EntityMetadataWrapperException $e) {
      return NULL;
    }
  }


  private function getEpisodeProgram($wrapped_episode) {
    $program = NULL;

    if ($wrapped_episode) {
      try {
        $programs = $wrapped_episode->field_produced_for->value();

        if ($programs) {
          $program = $programs[0];
        }
      } catch (EntityMetadataWrapperException $e) {
        // swallow error
      }
    }

    return $program;
  }


  private function getEpisode($timeslot) {
    $query = new StationContentQuery();
    $query->getContent(
      $timeslot['value'],
      $timeslot['value2']
    );
    $result = $query->execute();

    if (empty($result)) {
      return NULL;
    }

    reset($result['node']);
    $nid = key($result['node']);
    return entity_metadata_wrapper('node', $nid);
  }


  private function getScheduleUrl($program, $wrapped_episode) {
    if ($wrapped_episode) {
      $episode = $wrapped_episode->value();
      return $this->getEntityUrl($episode);
    }

    return $this->getEntityUrl($program);
  }


  private function getScheduleTitle($timeslot_program, $episode_program) {
    $title = NULL;
    $timeslot_program_entity = entity_metadata_wrapper('node', $timeslot_program);
    $episode_program_entity = entity_metadata_wrapper('node', $episode_program);

    if ($timeslot_program) {
      $title = $this->getTeaserTitle($timeslot_program_entity);
    }

    $episode_program_nid = $episode_program_entity->getIdentifier();
    $timeslot_program_nid = $timeslot_program_entity->getIdentifier();

    $nids_differ = ($episode_program_nid != $timeslot_program_nid);
    if ($episode_program_nid && $nids_differ) {
      $title = $this->getTeaserTitle($episode_program_entity);
    }

    return $title;
  }


  private function getShowHost($timeslot_program, $episode_program) {
    $showhost_id = NULL;

    if ($timeslot_program && isset($timeslot_program->field_hosted_by['und'][0]['target_id'])) {
      $showhost_id = $timeslot_program->field_hosted_by['und'][0]['target_id'];
    }

    if ($episode_program && ($episode_program != $timeslot_program)) {
      if (isset($episode_program->field_hosted_by['und'][0]['target_id'])) {
        $showhost_id = $episode_program->field_hosted_by['und'][0]['target_id'];
      }
    }

    if (!$showhost_id) {
      return NULL;
    }

    $showhost = entity_metadata_wrapper('node', $showhost_id);
    return $showhost->value();
  }


  private function buildScheduleItem($wrapped_scheduled_timeslot, $timeslot) {
    $wrapped_episode = $this->getEpisode($timeslot);
    $timeslot_program = $this->getTimeslotProgram($wrapped_scheduled_timeslot);
    $episode_program = $this->getEpisodeProgram($wrapped_episode);

    if (!$timeslot_program && !$episode_program) {
      return NULL;
    }

    $title = $this->getScheduleTitle($timeslot_program, $episode_program);
    $showhost = $this->getShowHost($timeslot_program, $episode_program);
    $url = $this->getScheduleUrl($timeslot_program, $wrapped_episode);

    $showhost_title = NULL;
    $showhost_url = NULL;
    if ($showhost) {
      $showhost_title = $showhost->title;
      $showhost_url = $this->getEntityUrl($showhost);
    }

    $date_format = 'l M dS, Y';
    $time_format = 'g:i a';

    $formatted_date = Helpers::toCarbonString(
      $date_format,
      $timeslot['value']
    );

    $formatted_start_time = Helpers::toCarbonString(
      $time_format,
      $timeslot['value']
    );

    $formatted_end_time = Helpers::toCarbonString(
      $time_format,
      $timeslot['value2']
    );

    $schedule_item = [
      'title' => $title,
      'url' => $url,
      'showhost' => [
        'name' => $showhost_title,
        'url' => $showhost_url,
      ],
      'start' => [
        'formatted_date' => $formatted_date,
        'formatted_time' => $formatted_start_time,
        'timestamp' => $timeslot['value'],
      ],
      'finish' => [
        'formatted_time' => $formatted_end_time,
        'timestamp' => $timeslot['value2'],
      ],
    ];

    return $schedule_item;
  }


  public function processForProgram($nodes) {
    $now = time();
    $results = [];

    foreach ($nodes as $nid => $item) {
      $entity = entity_metadata_wrapper('node', $nid);
      $stream = $entity->field_stream->value();
      $timeslots = $entity->field_timeslot->value();

      $key = null;
      foreach ($timeslots as $index => $timeslot) {
        if ($timeslot['value'] >= $now) {
          $key = $index;
          break;
        }
      }

      if (is_null($key)) {
        continue;
      }

      $timeslot = $timeslots[$key];

      // Example rrule
      // RRULE:FREQ=DAILY;INTERVAL=1;COUNT=99;WKST=SU
      $rrule = $timeslot['rrule'];
      $rrule = explode(":", $rrule);
      $rrule = $rrule[1];
      $rrule = explode(";", $rrule);
      $rrule = $rrule[0];
      $rrule = explode("=", $rrule);
      $frequency = $rrule[1];

      $schedule_item = [
        'stream' => $stream->title,
        'starttime' => $timeslot['value'],
        'endtime' => $timeslot['value2'],
        'frequency' => $frequency,
      ];

      $results[] = $schedule_item;
    }

    return $results;
  }
}
