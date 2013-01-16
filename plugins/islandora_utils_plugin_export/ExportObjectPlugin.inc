<?php

module_load_include('inc', 'islandora_utils', 'IslandoraUtilsPlugin');
class ExportObjectPlugin extends IslandoraUtilsPlugin {

  /**
   * Constructor
   */
  function ExportObjectPlugin($pid = NULL, &$form_state = NULL, &$context = NULL) {
    parent::__construct($pid, $form_state, $context);
  }


  public function process() {
    module_load_include('inc', 'islandora', 'includes/IslandoraTuque');
    module_load_include('inc', 'islandora', 'includes/utilities');

    $values = $this->form_state['values'];
    $datastreams = array();

    if ($values['islandora_utils_plugin_export_download_local'] || $values['islandora_utils_plugin_export_download_remote']) {
      if (!isset($this->context['results']['plugin_export']['download_file_dir'])) {
        $file_dir = 'islandora_utils_plugin_export-' . date('Y-m-d-H-i-s');
        $file_dir_uri = "public://$file_dir";
        drupal_mkdir($file_dir_uri);
        //$file_name = 'islandora_utils_export'.date('Y-m-d-h:i:s').'.txt';
        //$file_dir = variable_get('file_public_path', file_directory_temp());
        //$results_file = file_create_filename($file_name, $file_dir);

        $this->context['results']['plugin_export']['download_file_dir'] = $file_dir;
        $this->context['results']['plugin_export']['download_file_dir_uri'] = $file_dir_uri;
      }

      if (!isset($this->context['results']['plugin_export']['file_list'])) {
        $this->context['results']['plugin_export']['file_list'] = array();
      }

      if ($values['islandora_utils_plugin_export_download_local'])  {
        try {
          $datastreams = $this->local_connection->api->a->listDatastreams($this->pid);
        } catch (Exception $exc) {
          $this->results[] = array(
            'type' => 'error',
            'pid' => $this->pid,
            'dsid' => $dsid,
            'message' => 'List datastreams for local object FAILED',
          );
        }


      }
      elseif ($values['islandora_utils_plugin_export_download_remote']) {
        try {
          $datastreams = $this->remote_connection->api->a->listDatastreams($this->pid);
        } catch (Exception $exc) {
          $this->results[] = array(
            'type' => 'error',
            'pid' => $this->pid,
            'dsid' => $dsid,
            'message' => 'List datastreams for remote object FAILED',
          );
        }
      }

      if (count($datastreams) == 0) {
        $this->results[] = array(
          'type' => 'error',
          'pid' => $this->pid,
          'dsid' => $dsid,
          'message' => 'No datastreams returned',
        );
      }

      foreach ($datastreams as $dsid => $datastream) {
        $file_name = $this->pid . '-' . $dsid;
        $this->context['results']['plugin_export']['file_list'][] = array(
          'pid' => $this->pid,
          'dsid' => $dsid,
          'label' => $datastream['label'],
          'mimetype' => $datastream['mimetype'],
          'filename' => $file_name,
        );

        if ($values['islandora_utils_plugin_export_download_local'])  {
          try {
            $ds_content = $this->local_connection->api->a->getDatastreamDissemination($this->pid, $dsid);
            $this->results[] = array(
              'type' => 'status',
              'pid' => $this->pid,
              'dsid' => $dsid,
              'message' => 'Export from local Islandora SUCCESSFUL',
            );
          } catch (Exception $exc) {
            $this->results[] = array(
              'type' => 'error',
              'pid' => $this->pid,
              'dsid' => $dsid,
              'message' => "Export from local Islandora FAILED",
            );
          }
        }
        elseif ($values['islandora_utils_plugin_export_download_remote']) {
          try {
            $ds_content = $this->remote_connection->api->a->getDatastreamDissemination($this->pid, $dsid);
            $this->results[] = array(
              'type' => 'status',
              'pid' => $this->pid,
              'dsid' => $dsid,
              'message' => 'Export from remote Islandora SUCCESSFUL',
            );
          } catch (Exception $exc) {
            $this->results[] = array(
              'type' => 'error',
              'pid' => $this->pid,
              'dsid' => $dsid,
              'message' => "Export from remote Islandora FAILED",
            );
          }


        }
        if ($ds_content) {
          if (file_put_contents($this->context['results']['plugin_export']['download_file_dir_uri'] . '/' . $file_name, $ds_content) === FALSE) {
            $this->results[] = array(
              'type' => 'error',
              'pid' => $this->pid,
              'dsid' => $dsid,
              'message' => "Save file locally FAILED",
            );
          }
        }
      }
    }

    if ($values['islandora_utils_plugin_export_ingest_local']) {
      if ($values['islandora_utils_plugin_export_ingest_local_purge']) {
        if (islandora_object_load($this->pid)) {
          try {
            $this->local_connection->api->m->purgeObject($this->pid);
          } catch (Exception $exc) {
            $this->results[] = array(
              'type' => 'error',
              'pid' => $this->pid,
              'dsid' => 'n/a',
              'message' => "Purge from local Islandora FAILED",
            );
          }
        }
      }

      try {
        $export_foxml = $this->remote_connection->api->m->export($this->pid);
      } catch (Exception $exc) {
        $this->results[] = array(
          'type' => 'error',
          'pid' => $this->pid,
          'dsid' => 'n/a',
          'message' => "Export object from remote Islandora FAILED",
        );
      }

      if ($export_foxml) {
        $this->results[] = array(
          'type' => 'status',
          'pid' => $this->pid,
          'dsid' => 'n/a',
          'message' => "Export object from remote Islandora SUCCESSFUL",
        );
        try {
          $params['string'] = $export_foxml;
          $ingest = $this->local_connection->api->m->ingest($params);
        } catch (Exception $exc) {
          $this->results[] = array(
            'type' => 'error',
            'pid' => $this->pid,
            'dsid' => 'n/a',
            'message' => "Local ingest from remote Islandora FAILED",
          );
        }
        if ($ingest) {
          $this->results[] = array(
            'type' => 'status',
            'pid' => $this->pid,
            'dsid' => 'n/a',
            'message' => "Local ingest from remote islandora SUCCESSFUL",
          );
        }
      }
    }
    /*
    if ($values['islandora_utils_plugin_export_ingest_remote']) {
      if ($this->pid) {
        $this->fedora_object = islandora_object_load($this->pid);
      }

      //ingest local object in remote fedora
      if (variable_get('islandora_utils_remote_islandora_base_url', '')) {
        $url = variable_get('islandora_utils_remote_islandora_base_url', '');
        if (!variable_get('islandora_utils_remote_islandora_drupal_filter', '')) {
          $remote_user = new stdClass();
          $remote_user->name = variable_get('islandora_utils_remote_islandora_username', 'fedoraAdmin');
          $remote_user->pass = variable_get('islandora_utils_remote_islandora_password', 'fedoraAdmin');
          $remote_user->uid = -1;
        }
        else {
          $remote_user = NULL;
        }

        $remote_connection = new IslandoraTuque($remote_user, $url);
        $export = $this->fedora_object->repository->api->m->export($this->fedora_object->id);
        $params['string'] = $export;
        $bob = $remote_connection->api->m->ingest($params);
        dsm($bob,'bob response');
      }
      else {
        dsm("can't ingest");
      }
    }

    if ($values['islandora_utils_plugin_export_ingest_local']) {
      dsm('ingest local');
      //functionize this bit of code
      if (variable_get('islandora_utils_remote_islandora_base_url', '')) {
        $url = variable_get('islandora_utils_remote_islandora_base_url', '');
        if (!variable_get('islandora_utils_remote_islandora_drupal_filter', '')) {
          $remote_user = new stdClass();
          $remote_user->name = variable_get('islandora_utils_remote_islandora_username', 'fedoraAdmin');
          $remote_user->pass = variable_get('islandora_utils_remote_islandora_password', 'fedoraAdmin');
          $remote_user->uid = -1;
        }
        else {
          $remote_user = NULL;
        }

        try {



        } catch (Exception $exc) {
          dsm($exc, 'opps');
        }

        $remote_connection = new IslandoraTuque($remote_user, $url);
        $export = $remote_connection->api->m->export('codearl:top');

        $local_connection = new IslandoraTuque();
        $params['string'] = $export;
        $bob = $local_connection->api->m->ingest($params);
        dsm($bob,'bobbbbbbbbbb');

      }
      else {
        dsm("can't ingest");
      }


    }
     *
     */
  }



}

