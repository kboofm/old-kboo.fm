<div>
  Next up:
  <ul>
    <?php
    $schedule = TemplateQuery::scheduledNext(2);
    foreach ($schedule as $schedule_item):
      ?>
      <li>
        <?php include 'schedule-item.tpl.php'; ?>
      </li>
    <?php endforeach; ?>
  </ul>
</div>
