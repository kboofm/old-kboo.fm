<?php

class TemplateQuery {
  public static function scheduledNow() {
    $query = new ScheduledTimeslotQuery();
    $query->getNow();
    $result = $query->execute();

    if (empty($result)) {
      return NULL;
    }

    $processor = new ScheduleProcessor();
    return $processor->processNow(
      $result['node']
    );
  }

  public static function scheduledNext($count = 1) {
    $query = new ScheduledTimeslotQuery();
    $query->getNext($count);
    $result = $query->execute();

    if (empty($result)) {
      return [];
    }

    $processor = new ScheduleProcessor();
    return $processor->processAfter(
      $result['node'],
      time(),
      $count
    );
  }
}