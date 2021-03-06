<?php

class RadioSpinitron {
  private static $connect;

  private static function getConnection() {
    if (!RadioSpinitron::$connect) {
      RadioSpinitron::$connect = Spinitron::instance();
    }

    return RadioSpinitron::$connect;
  }


  private static function includeSongs(&$playlist) {
    $playlistID = $playlist['PlaylistID'];
    $connection = RadioSpinitron::getConnection();
    $playlist['Songs'] = $connection->getSongs($playlistID);
  }


  /**
   * Get current playlist
   *
   * @return array|bool
   */
  public static function getCurrentPlaylist() {
    $connection = RadioSpinitron::getConnection();
    $playlist =  $connection->getCurrentPlaylist();
    RadioSpinitron::includeSongs($playlist);
    return $playlist;
  }


  public static function getLatestPlaylists($count) {
    $date = date("Y-m-d", time());

    $connection = RadioSpinitron::getConnection();
    $playlists = $connection->getPlaylistsByDate($date, 7);

    if (!$playlists) {
      return [];
    }

    return array_slice($playlists, 0, $count);
  }


  public static function getLatestSongs($count = null) {
    if (!$count) {
      $count = 99;
    }

    $connection = RadioSpinitron::getConnection();
    $songs = $connection->getSongs(null, $count);

    $songs = array_map(
      function ($item) {
        $date = $item['Date'];
        $time = $item['Timestamp'];
        $datetime = "{$date} {$time}";
        $timestamp = strtotime($datetime);
        $item['Date'] = date("m/j", $timestamp);
        $item['Timestamp'] = date("g:ia", $timestamp);
        return $item;
      },
      $songs
    );

    return $songs;
  }


  public static function getPlaylistDay($timestamp) {
    $date = date('Y-m-d', $timestamp);
    $connection = RadioSpinitron::getConnection();
    $playlists = $connection->getPlaylistsByDate($date);

    foreach ($playlists as &$playlist) {
      RadioSpinitron::includeSongs($playlist);
    }

    return $playlists;
  }


  public static function getEpisodePlaylist($node) {
    $wrapped_node = entity_metadata_wrapper('node', $node);
    $field_air_time = $wrapped_node->field_air_time->value();

    $connection = RadioSpinitron::getConnection();
    $date = date('Y-m-d', $field_air_time['value']);
    $playlists = $connection->getPlaylistsByDate($date);

    if (!$playlists) {
      return [];
    }

    // Filter out any playlists that occur outside of $field_air_time
    $playlists = array_filter(
      $playlists,
      function ($item) use ($field_air_time) {
        $start = $field_air_time['value'];
        $end = $field_air_time['value2'];

        $onair = "{$item['PlaylistDate']} {$item['OnairTime']}";
        $onair = strtotime($onair);

        $offair = "{$item['PlaylistDate']} {$item['OffairTime']}";
        $offair = strtotime($offair);

        if ($offair < $onair) {
          // If offair is the next day, then playlist date will be one day off
          $offair = strtotime("+1 day", $offair);
        }

        return ($start >= $onair && $end <= $offair);
      }
    );

    foreach ($playlists as &$playlist) {
      RadioSpinitron::includeSongs($playlist);
    }

    return $playlists;
  }


  /**
   * Get recent playlists for a program
   *
   * @param $program_nid
   * @return array|bool|null
   */
  public static function getProgramRecentPlaylists($program_nid) {
    $wrapped_node = entity_metadata_wrapper('node', $program_nid);

    if (!$wrapped_node) {
      return NULL;
    }

    $showId = $wrapped_node->field_spinitron_showid->value();

    if (!$showId) {
      return NULL;
    }

    $connection = RadioSpinitron::getConnection();
    $playlists = $connection->getPlaylistsByShow($showId);

    foreach ($playlists as &$playlist) {
      RadioSpinitron::includeSongs($playlist);
    }

    return $playlists;
  }


  /**
   * Get playlist for a program at a given time
   *
   * @param $program_nid
   * @param $timestamp
   * @return null|string
   */
  public static function getProgramPlaylistDay($program_nid, $timestamp) {
    $wrapped_node = entity_metadata_wrapper('node', $program_nid);

    if (!$wrapped_node) {
      return NULL;
    }

    $showId = $wrapped_node->field_spinitron_showid->value();

    if (!$showId) {
      return NULL;
    }

    $connection = RadioSpinitron::getConnection();
    $date = date('Y-m-d', $timestamp);
    $playlists = $connection->getPlaylistsByDate($date);

    $playlists = array_filter(
      $playlists,
      function ($item) use ($showId, $timestamp) {
        $onair = "{$item['PlaylistDate']} {$item['OnairTime']}";
        $onair = strtotime($onair);
        $dayBegin = Helpers::dayBegin($timestamp);

        $isShow = ($item['ShowID'] == $showId);
        $inRange = ($dayBegin <= $onair);
        return ($isShow && $inRange);
      }
    );

    foreach ($playlists as &$playlist) {
      RadioSpinitron::includeSongs($playlist);
    }

    return $playlists;
  }
}