<?php
$song_column_map = [
  'Time' => 'Timestamp',
  'Artist' => 'ArtistName',
  'Song' => 'SongName',
  'Album' => 'DiskName',
  'Label' => 'LabelName',
];

$count = count($spinitron_playlists) - 1;
?>

<?php foreach ($spinitron_playlists as $index => $playlist): ?>
  <div class="well">
    Playlist Date: <?php print $playlist['PlaylistDate']; ?>
    <br />
    Show Name: <?php print $playlist['ShowName']; ?>
    <br />
    On Air Time: <?php print $playlist['OnairTime']; ?>
    <br />
    Off Air Time: <?php print $playlist['OffairTime']; ?>
    <br />
    DJ Name : <?php print $playlist['DJName']; ?>
    <br />
  </div>


  <table class="table table-striped table-condensed hidden-xs">
    <thead>
      <tr>
        <?php foreach ($song_column_map as $human => $machine): ?>
          <th>
            <?php print $human; ?>
          </th>
        <?php endforeach; ?>
      </tr>
    </thead>

    <tbody>
    <?php foreach ($playlist['Songs'] as $song): ?>
      <tr>
        <?php foreach ($song_column_map as $machine): ?>
          <td>
            <?php print $song[$machine]; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
    </tbody>
  </table>

  <?php if ($index < $count): ?>
    <hr class="hidden-xs" />
  <?php endif; ?>


  <ul class="list-group visible-xs">
    <?php foreach ($playlist['Songs'] as $songs): ?>
      <li class="list-group-item">

        <?php foreach($song_column_map as $human => $machine): ?>
          <div class="row">

            <div class="col-xs-6">
            <span class="pull-right">
              <?php print $human; ?>
            </span>
            </div>

            <div class="col-xs-6">
              <?php print $song[$machine]; ?>
            </div>

          </div>
        <?php endforeach; ?>

      </li>
    <?php endforeach; ?>
  </ul>
<?php endforeach; ?>